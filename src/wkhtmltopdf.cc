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
#include "toc.hh"
#include "wkhtmltopdf.hh"
#include <QUuid>
#include <QtPlugin>
#include <iostream>
#include <qnetworkreply.h>
#include <string.h>

#ifdef QT_STATIC
//When doing a static build, we need to load the plugins to make images work
Q_IMPORT_PLUGIN(qjpeg)
Q_IMPORT_PLUGIN(qgif)
Q_IMPORT_PLUGIN(qtiff)
Q_IMPORT_PLUGIN(qmng)
#endif

#ifndef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
class MPrintEngine: public QPrintEngine {
private:
	QPrintEngine * inner;
	bool countPages;
	int count;
	WKHtmlToPdf * wk;
	bool first;
public:
	MPrintEngine(QPrintEngine * _, WKHtmlToPdf * w): inner(_), countPages(false), count(0), wk(w), first(true) {};
	bool abort() {return inner->abort();};
	int metric (QPaintDevice::PaintDeviceMetric id) const {return inner->metric(id);}
	QPrinter::PrinterState printerState() const {return inner->printerState();}
	QVariant property ( PrintEnginePropertyKey key ) const {return inner->property(key);}
	void setProperty ( PrintEnginePropertyKey a, const QVariant & b) {inner->setProperty(a,b);}
	int getCount() const {return count;}
	void resetCount() {count = 1;}
	void setCountPages(bool cp) {countPages=cp;}

	bool newPage() {
		if(countPages) {++count; return true;} 
		if(first) {first=false; wk->newPage(NULL,0,0,0);}
		bool r = inner->newPage();
		wk->newPage(NULL,0,0,0);
		return r;
	}
};

class MPrinter: public QPrinter {
public:
	void se(QPrintEngine * a, QPaintEngine * b) {setEngines(a,b);}
};
#endif

/*!
 * Guess a url, by looking at a string
 * (shamelessly copied from Arora Project)
 * \param string The string the is suppose to be some kind of url
 */
QUrl WKHtmlToPdf::guessUrlFromString(const QString &string) {
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


WKHtmlToPdf::WKHtmlToPdf() {
	initArgs();
}


// Needed to reset the pages and pageStart Vectors
void WKHtmlToPdf::resetPages() {
	pages.clear();
	pageStart.clear();
	in.clear();
}

/*!
 * Connect page signals, to our methods. And do other page
 * configurations.
 */
void WKHtmlToPdf::init() {
	am = new QNetworkAccessManager();
	//If some ssl error occures we want sslErrors to be called, so the we can ignore it
	connect(am, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)),this,
	        SLOT(sslErrors(QNetworkReply*, const QList<QSslError>&)));

	connect(am, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator *)),this,
	        SLOT(authenticationRequired(QNetworkReply *, QAuthenticator *)));

#ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	connect(&tocPrinter, SIGNAL(printingNewPage(QPrinter*,int,int,int)),
	        this, SLOT(newPage(QPrinter*,int,int,int)));
#endif
}

/*!
 * Replace some variabels in a string used in a header or fooder
 * \param q the string to substitute in
 * \param f the number that [fromPage] is substituded for
 * \param t the number that [toPage] is substituded for
 * \param p the number that [page] is substituded for
 */
QString WKHtmlToPdf::hfreplace(const QString & q) {
	QString _=q;
#ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	QString sec[TocPrinter::levels];
	for (uint i=0; i < TocPrinter::levels; ++i) {
		QMap<int, TocItem*>::const_iterator j = tocPrinter.page2sectionslow[i].find(pageNum);
		if (j == tocPrinter.page2sectionslow[i].end()) {
			j = tocPrinter.page2sectionshigh[i].upperBound(pageNum);
			--j;
			if (j == tocPrinter.page2sectionshigh[i].end()) continue;
		}
		sec[i] = j.value()->value;
	}
#endif
	return _
	       .replace("[page]",QString::number(pageNum+page_offset-1),Qt::CaseInsensitive)
	       .replace("[toPage]",QString::number(pageStart.back()+page_offset-1),Qt::CaseInsensitive)
	       .replace("[fromPage]",QString::number(page_offset),Qt::CaseInsensitive)
#ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	       .replace("[section]",sec[0],Qt::CaseInsensitive)
	       .replace("[subsection]",sec[1],Qt::CaseInsensitive)
	       .replace("[subsubsection]",sec[3],Qt::CaseInsensitive)
#endif
	       .replace("[webpage]",currentPage ==-1?"Table Of Content":in[currentPage]);
}

/*!
 * Called whenever a new page is printed: Display some progress, and puts in
 * footers and headers
 * \param printer the printer that prints a new page
 * \prarm f the first page printed
 * \param t the last page printed
 * \param p the page currently printing
 */
void WKHtmlToPdf::newPage(QPrinter *, int, int, int) {
	//Display some progress information to the user
	++pageNum;
	if (!quiet) {
		if (pageStart.back())
			fprintf(stderr,"Printing page: %d of %d\r",pageNum, pageStart.back());
		else
			fprintf(stderr,"Printing page: %d\r",pageNum);
		fflush(stdout);
	}

	if (currentPage == 0 && cover[0]) return;
	//Get the painter assosiated with the printer
	QPainter & painter = *printer->paintEngine()->painter();

	//Webkit used all kinds of crasy cordinate transformation, and font setup
	//We save it here and restore some sane defaults
	painter.save();
	painter.resetMatrix();
	int h=printer->pageRect().height();
	int w=printer->pageRect().width();

	//If needed draw the header line
	if (header_line) painter.drawLine(0,0,w,0);
	//Guess the height of the header text
	painter.setFont(QFont(header_font_name, header_font_size));
	int dy = painter.boundingRect(0,0,w,h,Qt::AlignTop,"M").height();
	//Draw the header text
	QRect r=QRect(0,0-dy,w,h);
	painter.drawText(r, Qt::AlignTop | Qt::AlignLeft, hfreplace(header_left));
	painter.drawText(r, Qt::AlignTop | Qt::AlignHCenter, hfreplace(header_center));
	painter.drawText(r, Qt::AlignTop | Qt::AlignRight, hfreplace(header_right));

	//IF needed draw the footer line
	if (footer_line) painter.drawLine(0,h,w,h);
	//Guess the height of the footer text
	painter.setFont(QFont(footer_font_name, footer_font_size));
	dy = painter.boundingRect(0,0,w,h,Qt::AlignTop,"M").height();
	//Draw the fooder text
	r=QRect(0,0,w,h+dy);
	painter.drawText(r, Qt::AlignBottom | Qt::AlignLeft, hfreplace(footer_left));
	painter.drawText(r, Qt::AlignBottom | Qt::AlignHCenter, hfreplace(footer_center));
	painter.drawText(r, Qt::AlignBottom | Qt::AlignRight, hfreplace(footer_right));

	//Restore wkebkits crasy scaling and font settings
	painter.restore();
}

/*!
 * Parse arguments, load page and print igt
 * \param argc the number of arguments
 * \param argv NULL terminated array of arguments
 */
void WKHtmlToPdf::run(int argc, const char ** argv) {
	loading=0;
	//Parse the arguments
	parseArgs(argc,argv);
	//If we must use a proxy, create a host of objects
	if (proxyHost) {
		QNetworkProxy proxy;
		proxy.setHostName(proxyHost);
		proxy.setPort(proxyPort);
		proxy.setType(proxyType);
		// to retrieve a web page, it's not needed to use a fully transparent
		// http proxy. Moreover, the CONNECT() method is frequently disabled
		// by proxies administrators.
#if QT_VERSION >= 0x040500
		if (proxyType == QNetworkProxy::HttpProxy)
			proxy.setCapabilities(QNetworkProxy::CachingCapability);
#endif
		if (proxyUser) proxy.setUser(proxyUser);
		if (proxyPassword) proxy.setPassword(proxyPassword);
		am->setProxy(proxy);
	}
#ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	if (cover[0]) in.push_front(cover);
#endif
	for (int i=0; i < in.size(); ++i) {
		QWebPage * page = new QWebPage();
		//Allow for network control fine touning.
		page->setNetworkAccessManager(am);
		//When loading is progressing we want loadProgress to be called
		connect(page, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
		//Once the loading is done we want loadFinished to be called
		connect(page, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
		connect(page, SIGNAL(loadStarted()), this, SLOT(loadStarted()));
#ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
		connect(page->mainFrame(), SIGNAL(printingNewPage(QPrinter*,int,int,int)),
		        this, SLOT(newPage(QPrinter*,int,int,int)));
		if (strcmp(default_encoding,""))
			page->settings()->setDefaultTextEncoding(default_encoding);
#endif
		//Disable stuff we don't need
		page->settings()->setAttribute(QWebSettings::JavaEnabled, enable_plugins);
		page->settings()->setAttribute(QWebSettings::JavascriptEnabled, !disable_javascript);
		page->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
		page->settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, false);
#if QT_VERSION >= 0x040500
		//Newer vertions of QT have even more settings to change
		page->settings()->setAttribute(QWebSettings::PrintElementBackgrounds, background);
		page->settings()->setAttribute(QWebSettings::PluginsEnabled, enable_plugins);
		if (strcmp(user_style_sheet,""))
			page->settings()->setUserStyleSheetUrl(guessUrlFromString(user_style_sheet));
#endif
		QString u= in[i];
		if (u == "-") {
			QFile in;
			in.open(0,QIODevice::ReadOnly);
			u = QDir::tempPath()+"/wktemp"+QUuid::createUuid().toString()+".html";
			temp.push_back(u);
			QFile tmp(u);
			tmp.open(QIODevice::WriteOnly);
			tmp.write(in.readAll());
		}
		loginTry=0;
		page->mainFrame()->load(guessUrlFromString(u));
		pages.push_back(page);
	}
}

/*!
 * Handel any ssl error by ignoring
 */
void WKHtmlToPdf::sslErrors(QNetworkReply *reply, const QList<QSslError> &error) {
	Q_UNUSED(error);
	//We ignore any ssl error, as it is next to impossible to send or receive
	//any private information with wkhtmltopdf anyhow, seeing as you cannot authenticate
	reply->ignoreSslErrors();
}

/*!
 * Print out the document to the pdf file
 */
void WKHtmlToPdf::printPage() {
	//If there are still pages loading wait til they are done
	if (loading != 0) return;
	//Give some user feed back
	if (!quiet) {
		fprintf(stderr, "Outputting pages       \r");
		fflush(stdout);
	}

	QPrinter printer_(resolution);
	QString lout = out;
	if (dpi != -1) printer_.setResolution(dpi);
	if (!strcmp(out,"-")) {
		if (QFile::exists("/dev/stdout"))
			lout = "/dev/stdout";
		else {
			lout = QDir::tempPath()+"/wktemp"+QUuid::createUuid().toString()+".pdf";
			temp.push_back(lout);
		}
	}
	//Tell the printer object to print the file <out>
	printer_.setOutputFormat(
	    strcmp(out + (strlen(out)-3),".ps")==0?
	    QPrinter::PostScriptFormat : QPrinter::PdfFormat
	);
	printer_.setOutputFileName(lout);

#ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	QPrinter & printer = printer_;
#else
	MPrinter printer;
	MPrintEngine * printEngine = new MPrintEngine(printer_.printEngine(), this);
	printer.se(printEngine, printer_.paintEngine());
#endif
	this->printer = &printer;

	//We currently only support margins with the same unit
	if (margin_left.second != margin_right.second ||
	        margin_left.second != margin_top.second ||
	        margin_left.second != margin_bottom.second) {
		fprintf(stderr, "Currently all margin units must be the same!\n");
		exit(1);
	}

	//Setup margins and papersize
	printer.setPageMargins(margin_left.first, margin_top.first,
	                       margin_right.first, margin_bottom.first,
	                       margin_left.second);
	printer.setPageSize(pageSize);
	printer.setOrientation(orientation);
	printer.setColorMode(colorMode);

	if (!printer.isValid()) {
		fprintf(stderr,"Unable to write to output file\n");
		exit(1);
	}
	pageNum = 0;

#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	QPainter painter;
	painter.begin(&printer);
	QVector< QVector<QWebFrame::Heading> > headings;

	if (print_toc || outline) {
		for (int i=0; i < in.size(); ++i) {
			if (cover[0] && i == 0) {
				headings.push_back( QVector<QWebFrame::Heading>() );
				continue;
			}
			if (!quiet) {
				fprintf(stderr, "Finding headings %d of %d      \r",i,in.size());
				fflush(stdout);
			}
			headings.push_back(pages[i]->mainFrame()->headings(&printer, printMediaType));
		}
	}

	if (print_toc) {
		TocItem * root = new TocItem();
		for (int i=0; i < in.size(); ++i)buildToc(root,headings[i],0);
		pageStart.back() += tocPrinter.countPages(root, &printer, &painter);
		delete root;
	}
#endif

	if (!print_toc && !outline &&
	        !header_left[0] && !header_center[0] && !header_right[0] &&
	        !footer_left[0] && !footer_center[0] && !footer_right[0]) {
		for (int i=0; i < in.size(); ++i) pageStart.push_back(0);
	} else {
#ifndef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
		printEngine->setCountPages(true);
#endif
		for (int i=0; i < in.size(); ++i) {
			if (!quiet) {
				fprintf(stderr, "Counting pages %d of %d      \r",i,in.size());
				fflush(stdout);
			}
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
			if (cover[0] && i == 0) {
				pageStart.push_front(0);
				pageStart.back() +=  pages[i]->mainFrame()->countPages(&printer, printMediaType);
				continue;
			}
#endif
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
			int count = pages[i]->mainFrame()->countPages(&printer, printMediaType);
#else
			printEngine->resetCount();
			pages[i]->mainFrame()->print(&printer);
			int count = printEngine->getCount();
#endif
			pageStart.push_back( pageStart.back() + count );
		}
#ifndef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
		printEngine->setCountPages(false);
#endif
	}

#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	TocItem * root = NULL;
	if (print_toc || outline) {
		root = new TocItem();
		for (int i=0; i < in.size(); ++i) buildToc(root,headings[i],pageStart[i]+1);
		tocPrinter.populateSections(root);
	}
	TocItem toc;
	if (print_toc) {
		tocPrinter.page2sectionshigh[0][0] = &toc;
		tocPrinter.page2sectionslow[0][0] = &toc;
		toc.value = QString(tocPrinter.header_text);

	}
#endif

	for (int i=0; i < in.size(); ++i) {
		currentPage = i;
		if (i != 0) printer.newPage();
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
		if (print_toc && i == (cover[0] == '\0'?0:1)) {
			currentPage = -1;
			tocPrinter.print(root, &printer, &painter);
			printer.newPage();
			currentPage = i;
		}
		pages[i]->mainFrame()->print(&printer,&painter, printMediaType);
#else
		pages[i]->mainFrame()->print(&printer);		
#endif
	}
	
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	if (outline) tocPrinter.outline(root, &printer);
	if (root) delete root;
#endif

	if (loading != 0) return;

	if (!quiet) {
		fprintf(stderr,"Done                 \n");
		fflush(stderr);
	}
	for (int i=0; i < pages.size(); ++i) delete pages[i];
	if (!strcmp(out,"-") && lout != "/dev/stdout") {
		QFile i(lout);
		QFile o;
		i.open(QIODevice::ReadOnly);
		o.open(1,QIODevice::WriteOnly);
		o.write(i.readAll());
	}
	for (int i=0; i < temp.size(); ++i) QFile::remove(temp[i]);
	qApp->quit();
}

/*!
 * Once loading is finished, we start the printing
 * \param ok did the loading finish correctly?
 */
void WKHtmlToPdf::loadFinished(bool ok) {
	//Keep track of the number of pages currently loading
	loading.deref();
	if (!ok) {
		//It went bad, return with 1
		fprintf(stderr, "Failed loading page\n");
		exit(1);
		return;
	}
	if (!quiet) {
		fprintf(stderr, "Waiting for redirect  \r");
		fflush(stdout);
	}

	if (loading == 0) {
		//Wait a little while for js-redirects, and then print
		QTimer::singleShot(jsredirectwait, this, SLOT(printPage()));
	}
}

/*!
 * Once loading starting, this is called
 */
void WKHtmlToPdf::loadStarted() {
	//Keep track of the number of pages currently loading
	loading.ref();
}

/*!
 * Called when the page is loading, display some progress to the using
 * \param progress the loading progress in percent
 */
void WKHtmlToPdf::loadProgress(int progress) {
	//Print out the load status
	if (!quiet) fprintf(stderr,"Loading page: %3d%%   \r",progress);
	fflush(stdout);
}


/*!
 * Called when the page requires authentication, filles in the username
 * and password supplied on the command line
 */
void WKHtmlToPdf::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator) {
	if (!strcmp(username,"")) {
		//If no username is given, complain the such is required
		fprintf(stderr, "Authentication Required\n");
		reply->abort();
	} else if (loginTry >= 2) {
		//If the login has failed a sufficient number of times,
		//the username or password must be wrong
		fprintf(stderr, "Invalid username or password\n");
		reply->abort();
	} else {
		authenticator->setUser(username);
		authenticator->setPassword(password);
		++loginTry;
	}
}

/*!
 * State mashine driven, shell like parser. This is used for
 * reading commandline options from stdin
 * \param buff the line to parse
 * \param nargc on return will hold the number of arguments read
 * \param nargv on return will hold the argumenst read and be NULL terminated
 */
enum State {skip, tok, q1, q2, q1_esc, q2_esc, tok_esc};
void parseString(char * buff, int &nargc, char **nargv) {
	State state = skip;
	int write_start=0;
	int write=0;
	for (int read=0; buff[read]!='\0'; ++read) {
		State next_state=state;
		switch (state) {
		case skip:
			//Whitespace skipping state
			if (buff[read]!=' ' && buff[read]!='\t' && buff[read]!='\r' && buff[read]!='\n') {
				--read;
				next_state=tok;
			}
			break;
		case tok:
			//Normal toking reading state
			if (buff[read]=='\'') next_state=q1;
			else if (buff[read]=='"') next_state=q2;
			else if (buff[read]=='\\') next_state=tok_esc;
			else if (buff[read]==' ' || buff[read]=='\t' || buff[read]=='\n' || buff[read]=='\r') {
				next_state=skip;
				if (write_start != write) {
					buff[write++]='\0';
					nargv[nargc++] = buff+write_start;
					if (nargc > 998) exit(1);
				}
				write_start = write;
			} else buff[write++] = buff[read];
			break;
		case q1:
			//State parsing a single qoute argument
			if (buff[read]=='\'') next_state=tok;
			else if (buff[read]=='\\') next_state=q1_esc;
			else buff[write++] = buff[read];
			break;
		case q2:
			//State parsing a double qoute argument
			if (buff[read]=='"') next_state=tok;
			else if (buff[read]=='\\') next_state=q2_esc;
			else buff[write++] = buff[read];
			break;
		case tok_esc:
			//Escape one char and return to the tokan parsing state
			next_state=tok;
			buff[write++] = buff[read];
			break;
		case q1_esc:
			//Espace one char and return to the single quote parsing state
			next_state=q1;
			buff[write++] = buff[read];
			break;
		case q2_esc:
			//Escape one char and return to the double qoute parsing state
			next_state=q2;
			buff[write++] = buff[read];
			break;
		}
		state=next_state;
	}
	//Remember the last parameter
	if (write_start != write) {
		buff[write++]='\0';
		nargv[nargc++] = buff+write_start;
	}
	nargv[nargc]=NULL;
}

int main(int argc, char * argv[]) {
	//This is a hack, to allow the generation of the man page without X11 support
	WKHtmlToPdf x;
	if (argc == 2 && !strcmp(argv[1],"--help")) {
		x.usage(stdout);
		exit(0);
	}
	if (argc == 2 && !strcmp(argv[1],"--version")) {
		x.version(stdout);
		exit(0);
	}
	bool use_graphics=true;
#ifdef Q_WS_X11
#ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	use_graphics=false;
	for (int i=1; i < argc; ++i)
		if (!strcmp(argv[i],"--use-xserver")) use_graphics=true;
	if (!use_graphics) QApplication::setGraphicsSystem("raster");
#endif
#endif
	QApplication a(argc,argv, use_graphics); //Construct application, required for printing
	a.setStyle(new QCommonStyle()); // Plain style

	x.init();
	for (int i=1; i < argc; ++i)
		if (!strcmp(argv[i],"--read-args-from-stdin")) {
			char buff[20400];
			char *nargv[1000];
			nargv[0] = argv[0];
			while (fgets(buff,20398,stdin)) {
				x.resetPages();
				int nargc=1;
				parseString(buff,nargc,nargv);
				x.run(nargc,(const char**)nargv);
				qApp->exec(); //Wait for application to terminate
			}
			exit(0);
		}
	x.run(argc,(const char **)argv); //Run convertion
	return qApp->exec(); //Wait for application to terminate
}
