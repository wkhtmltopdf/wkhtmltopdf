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
// #include "toc.hh"
// #include "wkhtmltopdf.hh"
#include <QUuid>
#include <QtPlugin>
#include <iostream>

#include <string.h>
#include <cstdlib>
#include <qapplication.h>
#include <QCommonStyle>
#include "settings.hh"

#include "pageconverter.hh"
#include "textualfeedback.hh"

#ifdef QT_STATIC
//When doing a static build, we need to load the plugins to make images work
Q_IMPORT_PLUGIN(qjpeg)
Q_IMPORT_PLUGIN(qgif)
Q_IMPORT_PLUGIN(qtiff)
Q_IMPORT_PLUGIN(qmng)
#endif


// void WKHtmlToPdf::run(int argc, const char ** argv) {
	
// #ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
//  	if (cover[0]) in.push_front(cover);
// 	}
// #endif


// 	for (int i=0; i < in.size(); ++i) {
// 		QWebPage * page = new QWebPage();
// 		//Allow for network control fine touning.
// 		page->setNetworkAccessManager(am);
// 		//When loading is progressing we want loadProgress to be called
// 		connect(page, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
// 		//Once the loading is done we want loadFinished to be called
// 		connect(page, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
// 		connect(page, SIGNAL(loadStarted()), this, SLOT(loadStarted()));

// 		page->mainFrame()->setZoomFactor(zoom_factor);
// 		QString u= in[i];
// 		if (u == "-") {
// 			QFile in;
// 			in.open(stdin,QIODevice::ReadOnly);
// 			u = QDir::tempPath()+"/wktemp"+QUuid::createUuid().toString()+".html";
// 			temp.push_back(u);
// 			QFile tmp(u);
// 			tmp.open(QIODevice::WriteOnly);
// 			copyFile(in,tmp);
// 		}
// 		loginTry=0;
// 		page->mainFrame()->load(guessUrlFromString(u));
// 		pages.push_back(page);
// 	}
// }

// /*!
//  * Print out the document to the pdf file
//  */
// void WKHtmlToPdf::preparePrint() {
// 	//If there are still pages loading wait til they are done
// 	if (loading != 0) return;
// 	//Give some user feed back
// 	if (!quiet) {
// 		fprintf(stderr, "Outputting pages       \r");
// 		fflush(stdout);
// 	}

// 	printer = new QPrinter(resolution);
	
// 	QString lout = out;
// 	if (dpi != -1) printer->setResolution(dpi);
// 	if (!strcmp(out,"-")) {
// 		if (QFile::exists("/dev/stdout"))
// 			lout = "/dev/stdout";
// 		else {
// 			lout = QDir::tempPath()+"/wktemp"+QUuid::createUuid().toString()+".pdf";
// 			temp.push_back(lout);
// 		}
// 	}
// 	//Tell the printer object to print the file <out>
// 	printer->setOutputFormat(
// 	    strcmp(out + (strlen(out)-3),".ps")==0?
// 	    QPrinter::PostScriptFormat : QPrinter::PdfFormat
// 	);
// 	printer->setOutputFileName(lout);

// 	//We currently only support margins with the same unit
// 	if (margin_left.second != margin_right.second ||
// 	        margin_left.second != margin_top.second ||
// 	        margin_left.second != margin_bottom.second) {
// 		fprintf(stderr, "Currently all margin units must be the same!\n");
// 		exit(1);
// 	}

// 	//Setup margins and papersize
// 	printer->setPageMargins(margin_left.first, margin_top.first,
// 	                       margin_right.first, margin_bottom.first,
// 	                       margin_left.second);
// 	printer->setPageSize(pageSize);
// 	printer->setOrientation(orientation);
// 	printer->setColorMode(colorMode);

// 	if (!printer->isValid()) {
// 		fprintf(stderr,"Unable to write to output file\n");
// 		exit(1);
// 	}
// 	pageNum = 0;
// 	pageStart.push_back(0);

// #ifndef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
// 	//If you do not have the hacks you get this crappy solution
// 	printer.setCollateCopies(copies);
// 	printer.setCollateCopies(collate);
// 	pages[0]->mainFrame()->print(&printer);
// #else
// 	painter = new QPainter();
// 	painter->begin(printer);
	
// 	logicalPages = 0;
// 	actualPages = 0;
// 	pageCount.clear();
// 	tocPages = 0;
// 	anchors.clear();
// 	localLinks.clear();
// 	externalLinks.clear();

// 	//Find and resolve all local links
// 	if(useLocalLinks || useExternalLinks) {
// 		QHash<QString, int> urlToDoc;
// 		for(int d=0; d < pages.size(); ++d) 
// 			urlToDoc[ pages[d]->mainFrame()->url().toString(QUrl::RemoveFragment) ]  = d;
		
// 		for(int d=0; d < pages.size(); ++d) {
// 			if (!quiet) {
// 				fprintf(stderr, "Resolving Links %d of %d      \r",d+1,pages.size());
// 				fflush(stdout);
// 			}
			
// 			QList<QWebElement> links = pages[d]->mainFrame()->findAllElements("a");
// 			for(QList<QWebElement>::iterator i=links.begin(); i != links.end(); ++i) {
// 				QUrl href=QUrl(i->attribute("href"));
// 				if(href.isEmpty()) continue;
// 				href=pages[d]->mainFrame()->url().resolved(href);
// 				if(urlToDoc.contains(href.toString(QUrl::RemoveFragment))) {
// 					if(useLocalLinks) {
// 						int t = urlToDoc[href.toString(QUrl::RemoveFragment)];
// 						QWebElement e;
// 						if(!href.hasFragment()) 
// 							e = pages[t]->mainFrame()->findFirstElement("body");
// 						else {
// 							e = pages[t]->mainFrame()->findFirstElement("a[name=\""+href.fragment()+"\"]");
// 							if(e.isNull()) 
// 								e = pages[t]->mainFrame()->findFirstElement("*[id=\""+href.fragment()+"\"]");
// 						}
// 						if(e.isNull())
// 							qDebug() << "Unable to find target for local link " << href; 
// 						else {
// 							anchors[t][href.toString()] = e;
// 							localLinks[d].push_back( qMakePair(*i, href.toString()) );
// 						}
// 					}
// 				} else if(useExternalLinks)
// 					externalLinks[d].push_back( qMakePair(*i, href.toString() ) );
// 			}
// 		}
// 	}
	
// 	//This is the first render face, it is done to calculate:
// 	// * The number of pages of each document
// 	// * A visual ordering of the header elemnt
// 	// * The location and page number of each header
// 	for(int d=0; d < pages.size(); ++d) {
// 		if (!quiet) {
// 			fprintf(stderr, "Counting pages %d of %d      \r",d+1,pages.size()+(print_toc?1:0));
// 			fflush(stdout);
// 		}

// 		painter->save();
// 		QWebPrinter wp(pages[d]->mainFrame(), printer, *painter);
// 		int count = wp.pageCount();
// 		pageCount.push_back(count);
// 		actualPages += count;
// 		if (!cover[0] || d != 0) {
// 			logicalPages += count;
// 			QList<QWebElement> l = pages[d]->mainFrame()->findAllElements("h1,h2,h3,h4,h5,h6,h7,h8,h9");
// 			for(QList<QWebElement>::const_iterator i = l.begin(); i != l.end(); ++i) {
// 				QPair<int, QRectF> location = wp.elementLocation(*i);
// 				headings[d][ qMakePair(location.first, qMakePair(location.second.y(), location.second.x()) ) ] = *i;
// 			}
// 		} 
// 		painter->restore();
// 	}

// 	//Now that we know the ordering of the headers in each document we
// 	//can calculate the number of pages in the table of content
// 	if (print_toc) {
// 		if (!quiet) {
// 			fprintf(stderr, "Counting pages %d of %d      \r",pages.size()+1,pages.size()+1);
//  			fflush(stdout);
//  		}									   
//  		TocItem * root = new TocItem();
// 		for(int d=0; d < pages.size(); ++d) {
// 			if (cover[0] && d == 0) continue;
// 			// buildToc(root,pages[d]->mainFrame(),
// // 					 anchors[d], externalLinks[d]
// // 					 ,-1);
// 		}
// 		//tocPages = tocPrinter.countPages(root, &printer, painter);
// 		actualPages += tocPages;
// 		logicalPages += tocPages;
//  		delete root;
//    	}
// 	actualPages *= copies;
// 	int page=1;

// 	headerFooterLoading =
// 		(html_header.isEmpty()?0:actualPages) +
// 		(html_footer.isEmpty()?0:actualPages);
// 	if(!html_header.isEmpty() || !html_header.isEmpty()) {
// 		for(int d=0; d < pages.size(); ++d) {
// 			if (cover[0] && d == 0) continue;
// 			for(int p=0; p < pageCount[d]; ++p) {
// 				if(!html_header.isEmpty())
// 					headers.push_back(loadHeaderFooter(html_header, d, page) );
// 				if(!html_footer.isEmpty())
// 					footers.push_back(loadHeaderFooter(html_footer, d, page) );
// 				++page;
// 			}
// 		}
// 	} else 
// 		printPage();
// #endif
// }

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


// /*!
//  * Replace some variabels in a string used in a header or fooder
//  * \param q the string to substitute in
//  * \param f the number that [fromPage] is substituded for
//  * \param t the number that [toPage] is substituded for
//  * \param p the number that [page] is substituded for
//  */
// QString WKHtmlToPdf::hfreplace(const QString & q) {
// 	QHash<QString, QString> values = calculateHeaderFooterParams(1,1);
// 	QString r=q;
// 	for(QHash<QString, QString>::iterator i=values.begin(); i != values.end(); ++i)
// 		r=r.replace("["+i.key()+"]", i.value(), Qt::CaseInsensitive);
// 	return r;
// }


// QWebPage * WKHtmlToPdf::loadHeaderFooter(QString url, int d, int page) {
// 	QUrl u = guessUrlFromString(url);

// 	QHash<QString, QString> values = calculateHeaderFooterParams(d, page);
// 	for(QHash<QString, QString>::iterator i=values.begin(); i != values.end(); ++i)
// 		u.addQueryItem(i.key(), i.value());
	
// 	QWebPage * p = new QWebPage();
// 	p->setNetworkAccessManager(am);
// 	connect(p, SIGNAL(loadFinished(bool)), this, SLOT(headerFooterLoadFinished(bool)));
// 	p->mainFrame()->setZoomFactor(1);
// 	p->mainFrame()->load(u);
// 	return p;
// }

// //void WKHtmlToPdf::headerFooterLoadStarted() {
// //	headerFooterLoading.ref();
// //}

// void WKHtmlToPdf::headerFooterLoadFinished(bool) {
// 	headerFooterLoading.deref();
// 	qDebug() << "hat";
// 	if (headerFooterLoading == 0) {
		
// 		//Wait a little while for js-redirects, and then print
// 		QTimer::singleShot(100, this, SLOT(printPage()));
// 	}
// }
// #endif

// // struct MThread: public QThread {
// // 	static void msleep(unsigned long l) {
// // 		QThread::msleep(l);
// // 	}
// // };


// void WKHtmlToPdf::printPage() {
// 	if (headerFooterLoading != 0 || loading != 0) return;

//  	bool first=true;
//  	int actualPage=1;
//  	int cc=collate?copies:1;
//  	int pc=collate?1:copies;
	
// 	bool hasHeaderFooter = header_line || footer_line
// 		|| !header_left.isEmpty() || !header_center.isEmpty() || !header_right.isEmpty()
// 		|| !footer_left.isEmpty() || !footer_center.isEmpty() || !footer_right.isEmpty();

// 	for(int cc_=0; cc_ < cc; ++cc_) {
// 		//TODO print front here
// 		//TODO print TOC here
		
// // 		QPicture picture;

// // 		while(true) {
// // 			picture.setBoundingRect(
// // 				QRect(0,0,
// // 					  printer->pageRect().width() * picture.logicalDpiX()/printer->logicalDpiX(),
// // 					  printer->pageRect().height() * picture.logicalDpiY()/printer->logicalDpiY()));
			
// // 			QPainter p;
// // 			p.begin(&picture);
// // 			qDebug() << picture.boundingRect();
// // 			QWebPrinter wp(headers[0]->mainFrame(), &picture, p);
// // 			wp.spoolPage(1);
// // 			float h = wp.elementLocation(headers[0]->mainFrame()->findFirstElement("body")).second.height() * printer->logicalDpiY() /picture.logicalDpiY();
// // 			p.end();
// // 			if(h < 100000) break;
// // 			qDebug() << h;
// // 			picture = QPicture();
// // 		}

// 		logicalPage=1;
// 		for(int d=0; d < pages.size(); ++d) {
// 			painter->save();
			
// 			QWebPrinter wp(pages[d]->mainFrame(), printer, *painter);
// 			QString l1=pages[d]->mainFrame()->url().path().split("/").back()+"#";
// 			QString l2=pages[d]->mainFrame()->url().toString() + "#";

// 			//Sort anchors and links by page
// 			QHash<int, QHash<QString, QWebElement> > myAnchors;
// 			QHash<int, QVector< QPair<QWebElement,QString> > > myLocalLinks;
// 			QHash<int, QVector< QPair<QWebElement,QString> > > myExternalLinks;
// 			for(QHash<QString, QWebElement>::iterator i=anchors[d].begin();
// 				i != anchors[d].end(); ++i)
// 				myAnchors[ wp.elementLocation(i.value()).first][i.key()] = i.value();

// 			for(QVector< QPair<QWebElement,QString> >::iterator i=localLinks[d].begin();
// 				i != localLinks[d].end(); ++i)
// 				myLocalLinks[wp.elementLocation(i->first).first].push_back(*i);

// 			for(QVector< QPair<QWebElement,QString> >::iterator i=externalLinks[d].begin();
// 				i != externalLinks[d].end(); ++i)
// 				myExternalLinks[wp.elementLocation(i->first).first].push_back(*i);
						
// 			for(int p=0; p < wp.pageCount(); ++p) {
// 				for(int pc_=0; pc_ < pc; ++pc_) {
//  					if (!quiet) {
// 						fprintf(stderr,"Printing page: %d of %d\r",actualPage, actualPages);
//  						fflush(stdout);
//  					}
// 					if(first)
// 						first=false;
// 					else
// 						printer->newPage();


// 					wp.spoolPage(p+1);
					
// 					for(QHash<QString, QWebElement>::iterator i=myAnchors[p+1].begin();
// 						i != myAnchors[p+1].end(); ++i) {
// 						QRectF r = wp.elementLocation(i.value()).second;
// 						painter->addAnchor(r, i.key());
// 					}
// 					for(QVector< QPair<QWebElement,QString> >::iterator i=myLocalLinks[p+1].begin();
// 						i != myLocalLinks[p+1].end(); ++i)  {
// 						QRectF r = wp.elementLocation(i->first).second;
// 						painter->addLink(r, i->second);
// 					}
// 					for(QVector< QPair<QWebElement,QString> >::iterator i=myExternalLinks[p+1].begin();
// 						i != myExternalLinks[p+1].end(); ++i)  {
// 						QRectF r = wp.elementLocation(i->first).second;
// 						painter->addHyperlink(r, QUrl(i->second));
// 					}
// 					++actualPage;

// 					if(hasHeaderFooter && (!cover[0] || d != 0)) {
// 						//Webkit used all kinds of crasy cordinate transformation, and font setup
// 						//We save it here and restore some sane defaults
// 						painter->save();
// 						painter->resetTransform();
						
// 						int h=printer->height();
// 						int w=printer->width();
						
// 						//If needed draw the header line
// 						if (header_line) painter->drawLine(0,0,w,0);
// 						//Guess the height of the header text
// 						painter->setFont(QFont(header_font_name, header_font_size));
// 						int dy = painter->boundingRect(0,0,w,h,Qt::AlignTop,"M").height();
// 						//Draw the header text
// 						QRect r=QRect(0,0-dy,w,h);
// 						painter->drawText(r, Qt::AlignTop | Qt::AlignLeft, hfreplace(header_left));
// 						painter->drawText(r, Qt::AlignTop | Qt::AlignHCenter, hfreplace(header_center));
// 						painter->drawText(r, Qt::AlignTop | Qt::AlignRight, hfreplace(header_right));
						
// 						//IF needed draw the footer line
// 						if (footer_line) painter->drawLine(0,h,w,h);
// 						//Guess the height of the footer text
// 						painter->setFont(QFont(footer_font_name, footer_font_size));
// 						dy = painter->boundingRect(0,0,w,h,Qt::AlignTop,"M").height();
// 						//Draw the fooder text
// 						r=QRect(0,0,w,h+dy);
// 						painter->drawText(r, Qt::AlignBottom | Qt::AlignLeft, hfreplace(footer_left));
// 						painter->drawText(r, Qt::AlignBottom | Qt::AlignHCenter, hfreplace(footer_center));
// 						painter->drawText(r, Qt::AlignBottom | Qt::AlignRight, hfreplace(footer_right));
						
// 						//Restore webkits crasy scaling and font settings
// 						painter->restore();
// 					}


// // 					painter->save();
// // 					painter->resetTransform();
// // 					painter->drawPicture(0,-208 - 4, picture);
// // 					painter->restore();
// // 					{
// // 						painter->save();
// // 						
// // 						//MThread::msleep(100);
// // 						painter->translate(0,-222);
// // 						QWebPrinter whp(headers[0]->mainFrame(), printer, *painter);
// // 						//painter->translate(0,-whp.elementLocation(headers[0]->mainFrame()->findFirstElement("body")).second.height());
// // 						whp.spoolPage(1);
// // 					}
// 				}
// 				if (!cover[0] || d != 0) ++logicalPage;
// 			}
// 			painter->restore();
// 		}
//  	}
	
// // #ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
// // 	if (currentPage == 0 && cover[0]) return;
// // #endif
// // }

// // 	}

// // 	if (outline) tocPrinter.outline(root, &printer);
// // 	if (root) delete root;

//  	painter->end();

// 	if (headerFooterLoading != 0 || loading != 0) return;

// 	if (!quiet) {
// 		fprintf(stderr,"Done                 \n");
// 		fflush(stderr);
// 	}
//  	for (int i=0; i < pages.size(); ++i) delete pages[i];
// 	pages.clear();
// // 	for (int i=0; i < headers.size(); ++i) delete headers[i];
// // 	headers.clear();
// // 	for (int i=0; i < footers.size(); ++i) delete footers[i];
// // 	footers.clear();		

// // 	if (!strcmp(out,"-") && lout != "/dev/stdout") {
// // 		QFile i(lout);
// // 		QFile o;
// // 		i.open(QIODevice::ReadOnly);
// // 		o.open(stdout,QIODevice::WriteOnly);
// // 		copyFile(i,o);
// // 	}
// // 	for (int i=0; i < temp.size(); ++i) QFile::remove(temp[i]);
   
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
// }


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


#include "commandlineparser.hh"

int main(int argc, char * argv[]) {
	//This will store all our settings
	Settings settings;
	
	//Create a command line parser to parse commandline arguments
	CommandLineParser parser(settings);
	//Setup default values in settings
	parser.loadDefaults();
	//Parse the arguments
	parser.parseArguments(argc, (const char**)argv);

	//Construct QApplication required for printing
	bool use_graphics=true;
#ifdef Q_WS_X11
#ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	use_graphics=settings.useGraphics;
	if (!use_graphics) QApplication::setGraphicsSystem("raster");
#endif
#endif
	QApplication a(argc, argv, use_graphics);
	a.setStyle(new QCommonStyle());

	//Progress feedback will be send to stdout
	TextualFeedback feedback(stdout);
	
	//Create the actual page converter to convert the pages
	PageConverter converter(settings, feedback);

	for (int i=1; i < argc; ++i)
		if (!strcmp(argv[i],"--read-args-from-stdin")) {
			char buff[20400];
			char *nargv[1000];
			nargv[0] = argv[0];
			while (fgets(buff,20398,stdin)) {
				//x.resetPages();
				int nargc=1;
				parseString(buff,nargc,nargv);
				parser.loadDefaults();
				parser.parseArguments(nargc, (const char**)nargv);
				converter.convert();
				//x.run(nargc,(const char**)nargv);
				//qApp->exec(); //Wait for application to terminate
			}
			exit(0);
		}
	converter.convert();
	//qApp->exit();
}
