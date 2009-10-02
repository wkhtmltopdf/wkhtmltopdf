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

#include <QPair>


PageConverterPrivate::PageConverterPrivate(Settings & s, PageConverter & o) :
	settings(s), pageLoader(s), hfLoader(s), outer(o) {

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
		QWebSettings::globalSettings()->setUserStyleSheetUrl(MultiPageLoader::guessUrlFromString(settings.userStyleSheet));
#endif

	connect(&pageLoader, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
	connect(&hfLoader, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
	connect(&pageLoader, SIGNAL(loadFinished(bool)), this, SLOT(preparePrint(bool)));
	connect(&hfLoader, SIGNAL(loadFinished(bool)), this, SLOT(printPage(bool)));
}


// /*!
//  * Once loading is finished, we start the printing
//  * \param ok did the loading finish correctly?
//  */
// void PageConverterPrivate::loadFinished(bool ok) {
// 	//Keep track of the number of pages currently loading
// 	#warning "This is a race condition"
// 	loading.deref();
// 	if (!ok) {
// 		//It went bad, return with 1
// 		emit outer.error("Failed loading page");
//         #warning "FIX ME"
// 		exit(1);
// 		return;
// 	}

// 	  #warning "FIX ME"
// 	//feedback.nextState("Waiting for redirect");
// 	if (loading == 0) {
// 		//Wait a little while for js-redirects, and then print
// 		QTimer::singleShot(settings.jsredirectwait, this, SLOT(preparePrint()));
// 	}
// }

/*!
 * Called when the page is loading, display some progress to the using
 * \param progress the loading progress in percent
 */
void PageConverterPrivate::loadProgress(int progress) {
	progressString = QString::number(progress) + "%";
	emit outer.progressChanged(progress);
}


void PageConverterPrivate::beginConvert() {
  	if (!settings.cover.isEmpty())
		settings.in.push_front(settings.cover);
// 	if(strcmp(out,"-") != 0 && !QFileInfo(out).isWritable()) {
// 		fprintf(stderr, "Write access to '%s' is not allowed\n", out);
// 		exit(1);
// 	}
	pageLoader.clearResources();
	foreach(QString url, settings.in) {
		QWebPage * page = pageLoader.addResource(url);
		pages.push_back(page);
	}
	pageLoader.load();
}


/*!
 * Prepares printing out the document to the pdf file
 */
void PageConverterPrivate::preparePrint(bool ok) {
	//If there are still pages loading wait til they are done
	//if (loading != 0) return;
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
			lout = tempOut.create(".pdf");
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
		printPage(true);
#endif
}


void PageConverterPrivate::printPage(bool ok) {
	//if (headerFooterLoading != 0 || loading != 0) return;

 	bool first=true;
 	int actualPage=1;
 	int cc=settings.collate?settings.copies:1;
 	int pc=settings.collate?1:settings.copies;
	
	bool hasHeaderFooter = 
		settings.header.line || settings.footer.line ||
		!settings.header.left.isEmpty() || !settings.footer.left.isEmpty() ||
		!settings.header.center.isEmpty() || !settings.footer.center.isEmpty() ||
		!settings.header.right.isEmpty() || !settings.footer.right.isEmpty();

	for(int cc_=0; cc_ < cc; ++cc_) {
		//TODO print front here
		//TODO print TOC here
		
// 		QPicture picture;

// 		while(true) {
// 			picture.setBoundingRect(
// 				QRect(0,0,
// 					  printer->pageRect().width() * picture.logicalDpiX()/printer->logicalDpiX(),
// 					  printer->pageRect().height() * picture.logicalDpiY()/printer->logicalDpiY()));
			
// 			QPainter p;
// 			p.begin(&picture);
// 			qDebug() << picture.boundingRect();
// 			QWebPrinter wp(headers[0]->mainFrame(), &picture, p);
// 			wp.spoolPage(1);
// 			float h = wp.elementLocation(headers[0]->mainFrame()->findFirstElement("body")).second.height() * printer->logicalDpiY() /picture.logicalDpiY();
// 			p.end();
// 			if(h < 100000) break;
// 			qDebug() << h;
// 			picture = QPicture();
// 		}

		logicalPage=1;
		for(int d=0; d < pages.size(); ++d) {
			painter->save();
			
			QWebPrinter wp(pages[d]->mainFrame(), printer, *painter);
			QString l1=pages[d]->mainFrame()->url().path().split("/").back()+"#";
			QString l2=pages[d]->mainFrame()->url().toString() + "#";

			//Sort anchors and links by page
			QHash<int, QHash<QString, QWebElement> > myAnchors;
			QHash<int, QVector< QPair<QWebElement,QString> > > myLocalLinks;
			QHash<int, QVector< QPair<QWebElement,QString> > > myExternalLinks;
			for(QHash<QString, QWebElement>::iterator i=anchors[d].begin();
				i != anchors[d].end(); ++i)
				myAnchors[ wp.elementLocation(i.value()).first][i.key()] = i.value();

			for(QVector< QPair<QWebElement,QString> >::iterator i=localLinks[d].begin();
				i != localLinks[d].end(); ++i)
				myLocalLinks[wp.elementLocation(i->first).first].push_back(*i);

			for(QVector< QPair<QWebElement,QString> >::iterator i=externalLinks[d].begin();
				i != externalLinks[d].end(); ++i)
				myExternalLinks[wp.elementLocation(i->first).first].push_back(*i);
						
			for(int p=0; p < wp.pageCount(); ++p) {
				for(int pc_=0; pc_ < pc; ++pc_) {
#warning "FIX ME"
//  					if (!quiet) {
// 						fprintf(stderr,"Printing page: %d of %d\r",actualPage, actualPages);
//  						fflush(stdout);
//  					}
					if(first)
						first=false;
					else
						printer->newPage();


					wp.spoolPage(p+1);
					
					for(QHash<QString, QWebElement>::iterator i=myAnchors[p+1].begin();
						i != myAnchors[p+1].end(); ++i) {
						QRectF r = wp.elementLocation(i.value()).second;
						painter->addAnchor(r, i.key());
					}
					for(QVector< QPair<QWebElement,QString> >::iterator i=myLocalLinks[p+1].begin();
						i != myLocalLinks[p+1].end(); ++i)  {
						QRectF r = wp.elementLocation(i->first).second;
						painter->addLink(r, i->second);
					}
					for(QVector< QPair<QWebElement,QString> >::iterator i=myExternalLinks[p+1].begin();
						i != myExternalLinks[p+1].end(); ++i)  {
						QRectF r = wp.elementLocation(i->first).second;
						painter->addHyperlink(r, QUrl(i->second));
					}
					++actualPage;

					if(hasHeaderFooter && (settings.cover.isEmpty() || d != 0)) {
						//Webkit used all kinds of crasy cordinate transformation, and font setup
						//We save it here and restore some sane defaults
						painter->save();
						painter->resetTransform();
						
						int h=printer->height();
						int w=printer->width();
						
						//If needed draw the header line
						if (settings.header.line) painter->drawLine(0, 0, w, 0);
						//Guess the height of the header text
						painter->setFont(QFont(settings.header.fontName, settings.header.fontSize));
						int dy = painter->boundingRect(0, 0, w, h, Qt::AlignTop, "M").height();
						//Draw the header text
						QRect r=QRect(0, 0-dy, w, h);
						painter->drawText(r, Qt::AlignTop | Qt::AlignLeft, hfreplace(settings.header.left));
						painter->drawText(r, Qt::AlignTop | Qt::AlignHCenter, hfreplace(settings.header.center));
						painter->drawText(r, Qt::AlignTop | Qt::AlignRight, hfreplace(settings.header.right));
						
						//IF needed draw the footer line
						if (settings.footer.line) painter->drawLine(0, h, w, h);
						//Guess the height of the footer text
						painter->setFont(QFont(settings.footer.fontName, settings.footer.fontSize));
						dy = painter->boundingRect(0, 0, w, h, Qt::AlignTop, "M").height();
						//Draw the fooder text
						r=QRect(0,0,w,h+dy);
						painter->drawText(r, Qt::AlignBottom | Qt::AlignLeft, hfreplace(settings.footer.left));
						painter->drawText(r, Qt::AlignBottom | Qt::AlignHCenter, hfreplace(settings.footer.center));
						painter->drawText(r, Qt::AlignBottom | Qt::AlignRight, hfreplace(settings.footer.right));
						
						//Restore webkits crasy scaling and font settings
						painter->restore();
					}


// 					painter->save();
// 					painter->resetTransform();
// 					painter->drawPicture(0,-208 - 4, picture);
// 					painter->restore();
// 					{
// 						painter->save();
// 						
// 						//MThread::msleep(100);
// 						painter->translate(0,-222);
// 						QWebPrinter whp(headers[0]->mainFrame(), printer, *painter);
// 						//painter->translate(0,-whp.elementLocation(headers[0]->mainFrame()->findFirstElement("body")).second.height());
// 						whp.spoolPage(1);
// 					}
				}
				if (settings.cover.isEmpty() || d != 0) ++logicalPage;
			}
			painter->restore();
		}
 	}
	
// #ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
// 	if (currentPage == 0 && cover[0]) return;
// #endif
// }

// 	}

// 	if (outline) tocPrinter.outline(root, &printer);
// 	if (root) delete root;

 	painter->end();
	
	//if (headerFooterLoading != 0 || loading != 0) return;
	
	#warning "FIX ME"
// 	if (!quiet) {
// 		fprintf(stderr,"Done                 \n");
// 		fflush(stderr);
// 	}
//  	for (int i=0; i < pages.size(); ++i) delete pages[i];
// 	pages.clear();
// 	for (int i=0; i < headers.size(); ++i) delete headers[i];
// 	headers.clear();
// 	for (int i=0; i < footers.size(); ++i) delete footers[i];
// 	footers.clear();		

// 	if (!strcmp(out,"-") && lout != "/dev/stdout") {
// 		QFile i(lout);
// 		QFile o;
// 		i.open(QIODevice::ReadOnly);
// 		o.open(stdout,QIODevice::WriteOnly);
// 		copyFile(i,o);
// 	}
// 	for (int i=0; i < temp.size(); ++i) QFile::remove(temp[i]);
   
// 	switch(networkError) {
// 	case 401: 
// 		qApp->exit(3);
// 		break;
// 	case 404: 
// 		qApp->exit(2);
// 		break;
// 	case 0:
// 		qApp->quit();
// 		break;
// 	default:
// 		qApp->exit(1);
// 	}
}


QWebPage * PageConverterPrivate::loadHeaderFooter(QString url, int d, int page) {
	QUrl u = MultiPageLoader::guessUrlFromString(url);

	QHash<QString, QString> values = calculateHeaderFooterParams(d, page);
	for(QHash<QString, QString>::iterator i=values.begin(); i != values.end(); ++i)
		u.addQueryItem(i.key(), i.value());
	
	return hfLoader.addResource(u);
}

/*!
 * Replace some variabels in a string used in a header or fooder
 * \param q the string to substitute in
 */
QString PageConverterPrivate::hfreplace(const QString & q) {
	QHash<QString, QString> values = calculateHeaderFooterParams(1,1);
	QString r=q;
	for(QHash<QString, QString>::iterator i=values.begin(); i != values.end(); ++i)
		r=r.replace("["+i.key()+"]", i.value(), Qt::CaseInsensitive);
	return r;
}


QHash<QString, QString> PageConverterPrivate::calculateHeaderFooterParams(int d, int page) {
	QHash<QString, QString> res;
	
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
	return res;
}


/*!
  \class PageConverter
  \brief Class responsible for converting html pages to pdf
  \todo explain something about the convertion process here, and mention stages
*/

/*!
  \brief Create a page converter object based on the supplied settings
  \param settings Settings for the conversion
*/
PageConverter::PageConverter(Settings & settings):
	d(new PageConverterPrivate(settings, *this)) {
}

/*!
  \brief The destructor for the page converter object
*/
PageConverter::~PageConverter() {
	delete d;
}

/*!
  \brief Count the number of phases that the convertion proccess goes though
*/
int PageConverter::phaseCount() {
	return d->phaseDescriptions.size();
}

/*!
  \brief return the current phase of conversion
*/
int PageConverter::currentPhase() {
	return d->currentPhase;
}

/*!
  \brief return a textual description of some phase
  \param phase the phase to get a description of, -1 for current phase
*/
QString PageConverter::phaseDescription(int phase) {
	return d->phaseDescriptions[(phase < 0 || d->phaseDescriptions.size() >= phase)?phase:d->currentPhase];
}

/*!
  \brief return a textual description of progress in the current phase
*/
QString PageConverter::progressString() {
	return d->progressString;
}

/*!
  \brief return the http return code, of the converted page
*/
int PageConverter::httpErrorCode() {
	return d->pageLoader.httpErrorCode();
}

/*!
  \brief add a resource we want to convert
  \param url The url of the object we want to convert
*/
void PageConverter::addResource(const QString & url) {
	d->settings.in.push_back(url);
}

/*!
  \brief Start a asynchronious convertion of html pages to a pdf document.
  Once convertion is done an finished signal will be emitted
*/
void PageConverter::beginConvertion() {
	d->beginConvert();
}

/*!
  \brief Synchronios convert html pages to a pdf document.
*/
void PageConverter::convert() {
	d->convert();
}

/*!
  \brief Cancel a running conversion
*/
void PageConverter::cancel() {
	d->cancel();
}

/*!
  \fn PageConverter::warning(const QString & message)
  \brief Signal emmited when some non fatal waring occures during conversion
  \param message The warning message
*/

/*!
  \fn PageConverter::error(const QString & message)
  \brief Signal emmitted when a fatal error has occured during conversion
  \param message A message describing the fatal error
*/

/*!
  \fn PageConverter::phaseChanged()
  \brief Signal emitted when the converter has reached a new phase
*/

/*!
  \fn PageConverter::progressChanged()
  \brief Signal emitted when some progress has been done in the convertion phase
*/

/*!
  \fn PageConverter::finised()
  \brief Signal emitted when convertion has finished.
*/




