//-*- mode: c++; tab-width: 4; indent-tabs-mode: t; c-file-style: "stroustrup"; -*-
// This file is part of wkhtmltopdf.
//
// wkhtmltopdf is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// wkhtmltopdf is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with wkhtmltopdf.  If not, see <http://www.gnu.org/licenses/>.
#include "pageconverter_p.hh"
#include <qfileinfo.h>
#include <QAuthenticator>
#include <QTimer>
#include <QWebSettings>
#include <QWebPage>
#include <QWebFrame>
#include <QDir>
#include <QUuid>
#include <QPair>

/*!
 * Copy a file from some place to another
 * \param src The source to copy from
 * \param dst The destination to copy to
 */
void PageConverterPrivate::copyFile(QFile & src, QFile & dst) {
	QByteArray buf(1024*1024*5,0);
	while(qint64 r=src.read(buf.data(),buf.size()))
		dst.write(buf.data(),r);
	src.close();
	dst.close();
}

/*!
 * Guess a url, by looking at a string
 * (shamelessly copied from Arora Project)
 * \param string The string the is suppose to be some kind of url
 */
QUrl PageConverterPrivate::guessUrlFromString(const QString &string) {
	QString urlStr = string.trimmed();

	// check if the string is just a host with a port
	QRegExp hostWithPort(QLatin1String("^[a-zA-Z\\.]+\\:[0-9]*$"));
	if (hostWithPort.exactMatch(urlStr))
		urlStr = QLatin1String("http://") + urlStr;

	// Check if it looks like a qualified URL. Try parsing it and see.
	QRegExp test(QLatin1String("^[a-zA-Z]+\\:.*"));
	bool hasSchema = test.exactMatch(urlStr);
	if (hasSchema) {
		bool isAscii = true;
		foreach (const QChar &c, urlStr) {
			if (c >= 0x80) {
				isAscii = false;
				break;
			}
		}

		QUrl url;
		if (isAscii) {
			url = QUrl::fromEncoded(urlStr.toAscii(), QUrl::TolerantMode);
		} else {
			url = QUrl(urlStr, QUrl::TolerantMode);
		}
		if (url.isValid())
			return url;
	}

	// Might be a file.
	if (QFile::exists(urlStr)) {
		QFileInfo info(urlStr);
		return QUrl::fromLocalFile(info.absoluteFilePath());
	}

	// Might be a shorturl - try to detect the schema.
	if (!hasSchema) {
		int dotIndex = urlStr.indexOf(QLatin1Char('.'));
		if (dotIndex != -1) {
			QString prefix = urlStr.left(dotIndex).toLower();
			QString schema = (prefix == QLatin1String("ftp")) ? prefix : QLatin1String("http");
			QUrl url(schema + QLatin1String("://") + urlStr, QUrl::TolerantMode);
			if (url.isValid())
				return url;
		}
	}

	// Fall back to QUrl's own tolerant parser.
	QUrl url = QUrl(string, QUrl::TolerantMode);

	// finally for cases where the user just types in a hostname add http
	if (url.scheme().isEmpty())
		url = QUrl(QLatin1String("http://") + string, QUrl::TolerantMode);
	return url;
}

PageConverterPrivate::PageConverterPrivate(Settings & s, Feedback & f) :
	settings(s), feedback(f) {
	
	//If some ssl error occures we want sslErrors to be called, so the we can ignore it
	connect(&networkAccessManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)),this,
	        SLOT(sslErrors(QNetworkReply*, const QList<QSslError>&)));

	connect(&networkAccessManager, SIGNAL(finished (QNetworkReply *)),
			this, SLOT(amfinished (QNetworkReply *) ) );

	connect(&networkAccessManager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator *)),this,
	        SLOT(authenticationRequired(QNetworkReply *, QAuthenticator *)));

	//If we must use a proxy, create a host of objects
	if (!settings.proxy.host.isEmpty()) {
		QNetworkProxy proxy;
		proxy.setHostName(settings.proxy.host);
		proxy.setPort(settings.proxy.port);
		proxy.setType(settings.proxy.type);
		// to retrieve a web page, it's not needed to use a fully transparent
		// http proxy. Moreover, the CONNECT() method is frequently disabled
		// by proxies administrators.
#if QT_VERSION >= 0x040500
		if (settings.proxy.type == QNetworkProxy::HttpProxy)
			proxy.setCapabilities(QNetworkProxy::CachingCapability);
#endif
		if (!settings.proxy.user.isEmpty())
			proxy.setUser(settings.proxy.user);
		if (!settings.proxy.password.isEmpty())
			proxy.setPassword(settings.proxy.password);
		networkAccessManager.setProxy(proxy);
	}

#ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	if (!settings.defaultEncoding.isEmpty())
		QWebSettings::globalSettings()->setDefaultTextEncoding(settings.defaultEncoding);
	if (!settings.enableIntelligentShrinking) {
		QWebSettings::globalSettings()->setPrintingMaximumShrinkFactor(1.0);
		QWebSettings::globalSettings()->setPrintingMinimumShrinkFactor(1.0);
	}
#endif
	QWebSettings::globalSettings()->setAttribute(QWebSettings::JavaEnabled, settings.enablePlugins);
	QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled, settings.enableJavascript);
	QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
	QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, false);

#if QT_VERSION >= 0x040500
	//Newer vertions of QT have even more settings to change
	QWebSettings::globalSettings()->setAttribute(QWebSettings::PrintElementBackgrounds, settings.background);
	QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, settings.enablePlugins);
	if (!settings.userStyleSheet.isEmpty())
		QWebSettings::globalSettings()->setUserStyleSheetUrl(guessUrlFromString(settings.userStyleSheet));
#endif
}


/*!
 * Track and handle network errors
 * \param reply The networkreply that has finished
 */
void PageConverterPrivate::amfinished(QNetworkReply * reply) {
	int error = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	if(error > 399 && networkError == 0) networkError = error;
}

/*!
 * Called when the page requires authentication, filles in the username
 * and password supplied on the command line
 */
void PageConverterPrivate::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator) {
	if (settings.username.isEmpty()) {
		//If no username is given, complain the such is required
		feedback.error("Authentication Required\n");
		reply->abort();
	} else if (loginTry >= 2) {
		//If the login has failed a sufficient number of times,
		//the username or password must be wrong
		feedback.error("Invalid username or password");
		reply->abort();
	} else {
		authenticator->setUser(settings.username);
		authenticator->setPassword(settings.password);
		++loginTry;
	}
}


/*!
 * Once loading is finished, we start the printing
 * \param ok did the loading finish correctly?
 */
void PageConverterPrivate::loadFinished(bool ok) {
	//Keep track of the number of pages currently loading
	#warning "This is a race condition"
	loading.deref();
	if (!ok) {
		//It went bad, return with 1
		feedback.error("Failed loading page");
        #warning "FIX ME"
		exit(1);
		return;
	}

	feedback.nextState("Waiting for redirect");
	if (loading == 0) {
		//Wait a little while for js-redirects, and then print
		QTimer::singleShot(settings.jsredirectwait, this, SLOT(preparePrint()));
	}
}

/*!
 * Once loading starting, this is called
 */
void PageConverterPrivate::loadStarted() {
	//Keep track of the number of pages currently loading
	#warning "This is a race condition"
	loading.ref();
}

/*!
 * Called when the page is loading, display some progress to the using
 * \param progress the loading progress in percent
 */
void PageConverterPrivate::loadProgress(int progress) {
	//Print out the load status
	feedback.progress(progress, 100, "%", false);
	fflush(stdout);
}

/*!
 * Handel any ssl error by ignoring
 */
void PageConverterPrivate::sslErrors(QNetworkReply *reply, const QList<QSslError> &) {
	//We ignore any ssl error, as it is next to impossible to send or receive
	//any private information with wkhtmltopdf anyhow, seeing as you cannot authenticate
	reply->ignoreSslErrors();
}

void PageConverterPrivate::convert() {
	networkError = 0;
	loginTry = 0;
	// 	pages.clear();
// 	pageStart.clear();
	
  	if (!settings.cover.isEmpty())
		settings.in.push_front(settings.cover);

// 	if(strcmp(out,"-") != 0 && !QFileInfo(out).isWritable()) {
// 		fprintf(stderr, "Write access to '%s' is not allowed\n", out);
// 		exit(1);
// 	}
	foreach(QString url, settings.in) {
		QWebPage * page = new QWebPage();
		//Allow for network control fine touning.
		page->setNetworkAccessManager(&networkAccessManager);
		//When loading is progressing we want loadProgress to be called
		connect(page, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
		//Once the loading is done we want loadFinished to be called
		connect(page, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
		connect(page, SIGNAL(loadStarted()), this, SLOT(loadStarted()));

		page->mainFrame()->setZoomFactor(settings.zoomFactor);
		if (url == "-") {
			QFile in;
			in.open(stdin,QIODevice::ReadOnly);
			url = QDir::tempPath()+"/wktemp"+QUuid::createUuid().toString()+".html";
			temporaryFiles.push_back(url);
			QFile tmp(url);
			tmp.open(QIODevice::WriteOnly);
			copyFile(in,tmp);
		}

		page->mainFrame()->load(guessUrlFromString(url));
		pages.push_back(page);
	}
}


/*!
 * Prepares printing out the document to the pdf file
 */
void PageConverterPrivate::preparePrint() {
	//If there are still pages loading wait til they are done
	if (loading != 0) return;
	//Give some user feed back
// 	if (!quiet) {
// 		fprintf(stderr, "Outputting pages       \r");
// 		fflush(stdout);
// 	}

	printer = new QPrinter(settings.resolution);
	
	QString lout = settings.out;
	if (settings.dpi != -1) printer->setResolution(settings.dpi);
	if (settings.out == "-") {
		if (QFile::exists("/dev/stdout"))
			lout = "/dev/stdout";
		else {
			#warning "Sometimes we should add .ps instead here"
			lout = QDir::tempPath()+"/wktemp"+QUuid::createUuid().toString()+".pdf";
			temporaryFiles.push_back(lout);
		}
	}
	//Tell the printer object to print the file <out>
	printer->setOutputFormat(
		settings.out.endsWith(".ps", Qt::CaseInsensitive)?
	    QPrinter::PostScriptFormat : QPrinter::PdfFormat
		);
	printer->setOutputFileName(lout);

	//We currently only support margins with the same unit
	if (settings.margin.left.second != settings.margin.right.second ||
		settings.margin.left.second != settings.margin.top.second ||
		settings.margin.left.second != settings.margin.bottom.second) {
		#warning "FIX ME"
		// fprintf(stderr, "Currently all margin units must be the same!\n");
// 		exit(1);
	}

	//Setup margins and papersize
	printer->setPageMargins(settings.margin.left.first, settings.margin.top.first,
							settings.margin.right.first, settings.margin.bottom.first,
							settings.margin.left.second);
	printer->setPageSize(settings.pageSize);
	printer->setOrientation(settings.orientation);
	printer->setColorMode(settings.colorMode);

	if (!printer->isValid()) {
		#warning "FIX ME"
// 		fprintf(stderr,"Unable to write to output file\n");
// 		exit(1);
	}
	//pageStart.push_back(0);

#ifndef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	//If you do not have the hacks you get this crappy solution
	printer.setCollateCopies(settings.copies);
	printer.setCollateCopies(settings.collate);
	printPage();
	//pages[0]->mainFrame()->print(&printer);
#else
	painter = new QPainter();
	painter->begin(printer);
	
	logicalPages = 0;
	actualPages = 0;
	pageCount.clear();
	tocPages = 0;
	anchors.clear();
	localLinks.clear();
	externalLinks.clear();

	//Find and resolve all local links
	if(settings.useLocalLinks || settings.useExternalLinks) {
		QHash<QString, int> urlToDoc;
		for(int d=0; d < pages.size(); ++d) 
			urlToDoc[ pages[d]->mainFrame()->url().toString(QUrl::RemoveFragment) ]  = d;
		
		for(int d=0; d < pages.size(); ++d) {
			#warning "FIX ME"
			// if (!quiet) {
// 				fprintf(stderr, "Resolving Links %d of %d      \r",d+1,pages.size());
// 				fflush(stdout);
// 			}
			
			foreach(const QWebElement & elm,pages[d]->mainFrame()->findAllElements("a")) {
				QUrl href=QUrl(elm.attribute("href"));
				if(href.isEmpty()) continue;
				href=pages[d]->mainFrame()->url().resolved(href);
				if(urlToDoc.contains(href.toString(QUrl::RemoveFragment))) {
					if(settings.useLocalLinks) {
						int t = urlToDoc[href.toString(QUrl::RemoveFragment)];
						QWebElement e;
						if(!href.hasFragment()) 
							e = pages[t]->mainFrame()->findFirstElement("body");
						else {
							e = pages[t]->mainFrame()->findFirstElement("a[name=\""+href.fragment()+"\"]");
							if(e.isNull()) 
								e = pages[t]->mainFrame()->findFirstElement("*[id=\""+href.fragment()+"\"]");
						}
						if(e.isNull())
							qDebug() << "Unable to find target for local link " << href; 
						else {
							anchors[t][href.toString()] = e;
							localLinks[d].push_back( qMakePair(elm, href.toString()) );
						}
					}
				} else if(settings.useExternalLinks)
					externalLinks[d].push_back( qMakePair(elm, href.toString() ) );
			}
		}
	}

	headings.clear();
	
	//This is the first render face, it is done to calculate:
	// * The number of pages of each document
	// * A visual ordering of the header elemnt
	// * The location and page number of each header
	for(int d=0; d < pages.size(); ++d) {
		#warning "fix me"
// 		if (!quiet) {
// 			fprintf(stderr, "Counting pages %d of %d      \r",d+1,pages.size()+(print_toc?1:0));
// 			fflush(stdout);
// 		}

		painter->save();
		QWebPrinter wp(pages[d]->mainFrame(), printer, *painter);
		int count = wp.pageCount();
		pageCount.push_back(count);
		actualPages += count;
		if (settings.cover.isEmpty() || d != 0) {
			logicalPages += count;
			foreach(const QWebElement & e, pages[d]->mainFrame()->findAllElements("h1,h2,h3,h4,h5,h6,h7,h8,h9")) {
				QPair<int, QRectF> location = wp.elementLocation(e);
				headings[d][ qMakePair(location.first, qMakePair(location.second.y(), location.second.x()) ) ] = e;
			}
		} 
		painter->restore();
	}

	//Now that we know the ordering of the headers in each document we
	//can calculate the number of pages in the table of content
	if (settings.printToc) {
		#warning "fixme"
// 		if (!quiet) {
// 			fprintf(stderr, "Counting pages %d of %d      \r",pages.size()+1,pages.size()+1);
//  			fflush(stdout);
//  		}									   
//  		TocItem * root = new TocItem();
// 		for(int d=0; d < pages.size(); ++d) {
// 			if (cover[0] && d == 0) continue;
// 			// buildToc(root,pages[d]->mainFrame(),
//  					 anchors[d], externalLinks[d]
//  					 ,-1);
// 		}
		//tocPages = tocPrinter.countPages(root, &printer, painter);
		tocPages = 0;
		actualPages += tocPages;
		logicalPages += tocPages;
 		//delete root;
   	}
	actualPages *= settings.copies;
	int page=1;

	headers.clear();
	footers.clear();
// 	headerFooterLoading =
// 		(settings.header.htmlUrl.isEmpty()?0:actualPages) +
// 		(settings.fooher.htmlURl.isEmpty()?0:actualPages);
	if(!settings.header.htmlUrl.isEmpty() || !settings.footer.htmlUrl.isEmpty()) {
		for(int d=0; d < pages.size(); ++d) {
			if (!settings.cover.isEmpty() && d == 0) continue;
			for(int p=0; p < pageCount[d]; ++p) {
				if(!settings.header.htmlUrl.isEmpty())
					headers.push_back(loadHeaderFooter(settings.header.htmlUrl, d, page) );
				if(!settings.footer.htmlUrl.isEmpty())
					footers.push_back(loadHeaderFooter(settings.footer.htmlUrl, d, page) );
				++page;
			}
		}
	} else 
		printPage();
#endif
}


QWebPage * PageConverterPrivate::loadHeaderFooter(QString url, int d, int page) {
	QUrl u = guessUrlFromString(url);

	#warning "Fix me"
// 	QHash<QString, QString> values = calculateHeaderFooterParams(d, page);
// 	for(QHash<QString, QString>::iterator i=values.begin(); i != values.end(); ++i)
// 		u.addQueryItem(i.key(), i.value());
	
	QWebPage * p = new QWebPage();
	p->setNetworkAccessManager(&networkAccessManager);
	//connect(p, SIGNAL(loadFinished(bool)), this, SLOT(headerFooterLoadFinished(bool)));
	p->mainFrame()->load(u);
	return p;
}

// #ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
// QHash<QString, QString> WKHtmlToPdf::calculateHeaderFooterParams(int d, int page) {
// 	QHash<QString, QString> res;
	
// 	res["frompage"] = QString::number(page_offset);
// 	res["topage"] = QString::number(page_offset+logicalPages-1);
// 	res["page"] = QString::number(logicalPage);
// 	res["webpage"] = "foobar";
	
//  	QString sec[TocPrinter::levels];
// 	for (uint i=0; i < TocPrinter::levels; ++i) {
// 		QMap<int, TocItem*>::const_iterator j = tocPrinter.page2sectionslow[i].find(pageNum);
// 		if (j == tocPrinter.page2sectionslow[i].end()) {
// 			j = tocPrinter.page2sectionshigh[i].upperBound(pageNum);
// 			--j;
// 			if (j == tocPrinter.page2sectionshigh[i].end()) continue;
// 		}
// 		sec[i] = j.value()->value;
// 	}
// 	res["section"] = sec[0];
// 	res["subsection"] = sec[1];
// 	res["subsubsection"] = sec[2];
// 	return res;
// }



/*!
  Convert all the pages supplied in the settings into a pdf document
*/
void PageConverter::convert() {
	d->convert();
}

PageConverter::PageConverter(Settings & settings, Feedback & feedback):
	d(new PageConverterPrivate(settings, feedback)) {
}

PageConverter::~PageConverter() {
	delete d;
}


/*!
  \class Feedback
  \brief Class responcible for providing feedback to the user about the convertion process
*/

/*!
  \fn Feedback::~Feedback()
  Dummy virtual destructor
*/

/*!
  \fn Feedback::setQuiet(bool quiet)
  Indicates whether the user has specified that they want quiet processing
  \param quiet Show we be quiet
*/

/*!
  \fn Feedback::error(const QString & msg)
  Indicate to the user that some error has occured.
  \param msg A message describing the error
*/

/*!
  \fn Feedback::nextState(const Qstring & name)
  Indicate that the processing has reached a new phace
  \param name the name of the new phase
*/

/*!
  \fn Feedback::progress(long cur, long max, const QString & unit, bool displayOf)
  Indicate that there where some progress in the current phase
  \param cur The current progress in the phase
  \param max Indicating the end of the phase
  \param unit The unit of the phase link "%" or " pages"
  \param displayOff Indicate that we want an "of x" in the description, e.g 1 of 3 pages
*/


