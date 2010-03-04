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
#include <QAuthenticator>
#include <QDir>
#include <QFile>
#include <QPair>
#include <QTimer>
#include <QWebFrame>
#include <QWebPage>
#include <QWebSettings>
#include <qapplication.h>
#include <qfileinfo.h>
#ifdef Q_OS_WIN32
#include <io.h>
#include <fcntl.h>
#endif
 
/*!
  \file pageconverter.hh
  \brief Defines the PageConverter class
*/

/*!
  \file pageconverter_p.hh
  \brief Defines the PageConverterPrivate class
*/

bool looksLikeHtmlAndNotAUrl(QString str) {
	QString s = str.split("?")[0];
	return s.count('<') > 0 || s.count('<') > 0;
}

PageConverterPrivate::PageConverterPrivate(Settings & s, PageConverter & o) :
	settings(s), pageLoader(s),
	outer(o), printer(0), painter(0)
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	, hfLoader(s), outline(0), tocPrinter(0)
#endif
{
		
#ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	phaseDescriptions.push_back("Loading pages");
	phaseDescriptions.push_back("Resolving links");
	phaseDescriptions.push_back("Counting pages");
	phaseDescriptions.push_back("Loading headers and footers");
	if (!settings.defaultEncoding.isEmpty())
		QWebSettings::globalSettings()->setDefaultTextEncoding(settings.defaultEncoding);
	if (!settings.enableIntelligentShrinking) {
		QWebSettings::globalSettings()->setPrintingMaximumShrinkFactor(1.0);
		QWebSettings::globalSettings()->setPrintingMinimumShrinkFactor(1.0);
	}
	QWebSettings::globalSettings()->setPrintingMediaType(settings.printMediaType?"print":"screen");
#else
	phaseDescriptions.push_back("Loading page");
#endif
	phaseDescriptions.push_back("Printing pages");
	phaseDescriptions.push_back("Done");

	QWebSettings::globalSettings()->setAttribute(QWebSettings::JavaEnabled, settings.enablePlugins);
	QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled, settings.enableJavascript);
	QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
	QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, false);

	QWebSettings::globalSettings()->setFontSize(QWebSettings::MinimumFontSize, s.minimumFontSize);

#if QT_VERSION >= 0x040500
	//Newer vertions of QT have even more settings to change
	QWebSettings::globalSettings()->setAttribute(QWebSettings::PrintElementBackgrounds, settings.background);
	QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, settings.enablePlugins);
	if (!settings.userStyleSheet.isEmpty())
		QWebSettings::globalSettings()->setUserStyleSheetUrl(MultiPageLoader::guessUrlFromString(settings.userStyleSheet));
#endif

	connect(&pageLoader, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
	connect(&pageLoader, SIGNAL(loadFinished(bool)), this, SLOT(preparePrint(bool)));
	connect(&pageLoader, SIGNAL(error(QString)), this, SLOT(forwardError(QString)));
	connect(&pageLoader, SIGNAL(warning(QString)), this, SLOT(forwardWarning(QString)));

#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__	
	connect(&hfLoader, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
	connect(&hfLoader, SIGNAL(loadFinished(bool)), this, SLOT(printPage(bool)));
	connect(&hfLoader, SIGNAL(error(QString)), this, SLOT(forwardError(QString)));
	connect(&hfLoader, SIGNAL(warning(QString)), this, SLOT(forwardWarning(QString)));
#endif
}

PageConverterPrivate::~PageConverterPrivate() {
	clearResources();
}

void PageConverterPrivate::forwardError(QString error) {
	emit outer.error(error);
}

void PageConverterPrivate::forwardWarning(QString warning) {
	emit outer.warning(warning);
}


/*!
 * Called when the page is loading, display some progress to the using
 * \param progress the loading progress in percent
 */
void PageConverterPrivate::loadProgress(int progress) {
	progressString = QString::number(progress) + "%";
	emit outer.progressChanged(progress);
}


void PageConverterPrivate::beginConvert() {
	clearResources();
	error=false;
	progressString = "0%";
	currentPhase=0;

  	if (!settings.cover.isEmpty())
		settings.in.push_front(settings.cover);

	if (!settings.header.htmlUrl.isEmpty() && looksLikeHtmlAndNotAUrl(settings.header.htmlUrl)) {
		emit outer.error("--header-html should be a URL and not a string containing HTML code.");
		fail();
		return;
	}

	if (!settings.footer.htmlUrl.isEmpty() && looksLikeHtmlAndNotAUrl(settings.footer.htmlUrl)) {
		emit outer.error("--header-html should be a URL and not a string containing HTML code.");
		fail();
		return;
	}
	
#ifndef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__	
	if (settings.in.size() > 1) {
		emit outer.error("This version of wkhtmltopdf is build against an unpatched version of QT, and does not support more then one input document.");
		fail();
		return;
	}
#endif
	
	foreach(QString url, settings.in)
		pages.push_back(pageLoader.addResource(url));
	
	emit outer.phaseChanged();
	loadProgress(0);

	pageLoader.load();
}

void PageConverterPrivate::fail() {
	error = true;
	convertionDone = true;
	clearResources();
	emit outer.finished(false);
	
	qApp->exit(0); // quit qt's event handling
}

/*!
 * Prepares printing out the document to the pdf file
 */
void PageConverterPrivate::preparePrint(bool ok) {
	if (!ok) {
		fail(); 
		return;
	}

	lout = settings.out;
	if (settings.out == "-") {
#ifndef Q_OS_WIN32
		 if (QFile::exists("/dev/stdout"))
 			lout = "/dev/stdout";
		 else
#endif
			 lout = tempOut.create(".pdf");
	}

	printer = new QPrinter(settings.resolution);
	if (settings.dpi != -1) printer->setResolution(settings.dpi);
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
		emit outer.error("Currently all margin units must be the same!");
		fail();
		return;
	}

	//Setup margins and papersize
	printer->setPageMargins(settings.margin.left.first, settings.margin.top.first,
							settings.margin.right.first, settings.margin.bottom.first,
							settings.margin.left.second);
	
	if((settings.size.height.first != -1) && (settings.size.width.first != -1)) {
		printer->setPaperSize(QSizeF(settings.size.width.first,settings.size.height.first), settings.size.height.second);
	}
	else {
		printer->setPaperSize(settings.size.pageSize);
	}
	
	printer->setOrientation(settings.orientation);
	printer->setColorMode(settings.colorMode);

	if (!printer->isValid()) {
		emit outer.error("Unable to write to destination");
		fail();
		return;
	}

#ifndef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	//If you do not have the hacks you get this crappy solution
	printer->setCollateCopies(settings.copies);
	printer->setCollateCopies(settings.collate);
	printPage(true);
#else
	painter = new QPainter();
	
	QString title = settings.documentTitle;
	if (title == "") 
		title = pages[0]->mainFrame()->title();
	printer->setDocName(title);
	if (!painter->begin(printer)) {
		emit outer.error("Unable to write to destination");
		fail();
		return;
	}
	
	logicalPages = 0;
	actualPages = 0;
	tocPages = 0;
	
	//Find and resolve all local links
	if(settings.useLocalLinks || settings.useExternalLinks) {
		currentPhase = 1;
		emit outer.phaseChanged();
		
		QHash<QString, int> urlToDoc;
		for(int d=0; d < pages.size(); ++d) 
			urlToDoc[ pages[d]->mainFrame()->url().toString(QUrl::RemoveFragment) ]  = d;
		
		for(int d=0; d < pages.size(); ++d) {

			progressString = QString("Page ")+QString::number(d+1)+QString(" of ")+QString::number(pages.size());
			emit outer.progressChanged((d+1)*100 / pages.size());
			
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

	currentPhase = 2;
	emit outer.phaseChanged();
	outline = new Outline(settings);
	//This is the first render face, it is done to calculate:
	// * The number of pages of each document
	// * A visual ordering of the header element
	// * The location and page number of each header
	for(int d=0; d < pages.size(); ++d) {
		int tot = pages.size()+(settings.printToc?1:0);
		progressString = QString("Page ")+QString::number(d+1)+QString(" of ")+QString::number(tot);
		emit outer.progressChanged((d+1)*100 / tot);

		painter->save();
		QWebPrinter wp(pages[d]->mainFrame(), printer, *painter);
		int count = wp.pageCount();
		//pageCount.push_back(count);
		actualPages += count;
		if (settings.cover.isEmpty() || d != 0) {
			outline->addWebPage("", wp, pages[d]->mainFrame());
			logicalPages += count;
		} 
		painter->restore();
	}


	//Now that we know the ordering of the headers in each document we
	//can calculate the number of pages in the table of content
	tocPrinter = NULL;
	if (settings.printToc) {
		int k=pages.size()+1;
		progressString = QString("Page ")+QString::number(k)+QString(" of ")+QString::number(k);
		emit outer.progressChanged(100);
		
		tocPrinter = new TocPrinter(outline, printer, *painter);
		actualPages += tocPrinter->pageCount();
		logicalPages += tocPrinter->pageCount();
   	}
	actualPages *= settings.copies;
	int page=1;

	headers.clear();
	footers.clear();
	if(!settings.header.htmlUrl.isEmpty() || !settings.footer.htmlUrl.isEmpty()) {
		QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled, true);
		currentPhase = 3;
		emit outer.phaseChanged();
		for(int p=0; p < outline->pageCount(); ++p) {
			QHash<QString, QString> parms;
			outline->fillHeaderFooterParms(page, parms);

			if(!settings.header.htmlUrl.isEmpty())
				headers.push_back(loadHeaderFooter(settings.header.htmlUrl, parms) );
			if(!settings.footer.htmlUrl.isEmpty())
				footers.push_back(loadHeaderFooter(settings.footer.htmlUrl, parms) );
			++page;
		}
		hfLoader.load();
	} else 
		printPage(true);
#endif
}

#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
void PageConverterPrivate::beginPage(int & actualPage, bool & first) {
	progressString = QString("Page ") + QString::number(actualPage) + QString(" of ") + QString::number(actualPages);
	emit outer.progressChanged(actualPage * 100 / actualPages);
	if(first)
		first=false;
	else
		printer->newPage();
	actualPage++;
}

void PageConverterPrivate::endPage(bool actual, bool hasHeaderFooter) {
	if(hasHeaderFooter && actual) {

		QHash<QString, QString> parms;
		outline->fillHeaderFooterParms(logicalPage, parms);

		//Webkit used all kinds of crazy coordinate transformation, and font setup
		//We save it here and restore some sane defaults
		painter->save();
		painter->resetTransform();
						
		int h=printer->height();
		int w=printer->width();
						
		double spacing = settings.header.spacing * printer->height() / printer->heightMM();
		//If needed draw the header line
		if (settings.header.line) painter->drawLine(0, -spacing, w, -spacing);
		//Guess the height of the header text
		painter->setFont(QFont(settings.header.fontName, settings.header.fontSize));
		int dy = painter->boundingRect(0, 0, w, h, Qt::AlignTop, "M").height();
		//Draw the header text
		QRect r=QRect(0, 0-dy-spacing, w, h);
		painter->drawText(r, Qt::AlignTop | Qt::AlignLeft, hfreplace(settings.header.left, parms));
		painter->drawText(r, Qt::AlignTop | Qt::AlignHCenter, hfreplace(settings.header.center, parms));
		painter->drawText(r, Qt::AlignTop | Qt::AlignRight, hfreplace(settings.header.right, parms));
		
		spacing = settings.footer.spacing * printer->height() / printer->heightMM();
		//IF needed draw the footer line
		if (settings.footer.line) painter->drawLine(0, h + spacing, w, h + spacing);
		//Guess the height of the footer text
		painter->setFont(QFont(settings.footer.fontName, settings.footer.fontSize));
		dy = painter->boundingRect(0, 0, w, h, Qt::AlignTop, "M").height();
		//Draw the footer text
		r=QRect(0,0,w,h+dy+ spacing);
		painter->drawText(r, Qt::AlignBottom | Qt::AlignLeft, hfreplace(settings.footer.left, parms));
		painter->drawText(r, Qt::AlignBottom | Qt::AlignHCenter, hfreplace(settings.footer.center, parms));
		painter->drawText(r, Qt::AlignBottom | Qt::AlignRight, hfreplace(settings.footer.right, parms));
		
		//Restore Webkit's crazy scaling and font settings
		painter->restore();
	}

	if(actual && logicalPage <= headers.size()) {
		painter->save();
		painter->resetTransform();
		double spacing = settings.header.spacing * printer->height() / printer->heightMM();
		painter->translate(0, -spacing);
		QWebPrinter wp(headers[logicalPage-1]->mainFrame(), printer, *painter);
		painter->translate(0,-wp.elementLocation(headers[logicalPage-1]->mainFrame()->findFirstElement("body")).second.height());
		wp.spoolPage(1);
		painter->restore();
	}

	if(actual && logicalPage <= footers.size()) {
		painter->save();
		painter->resetTransform();
		double spacing = settings.footer.spacing * printer->height() / printer->heightMM();
		painter->translate(0, printer->height()+ spacing);
		QWebPrinter wp(footers[logicalPage-1]->mainFrame(), printer, *painter);
		wp.spoolPage(1);
		painter->restore();
	}

}
#endif


void PageConverterPrivate::printPage(bool ok) {
	if (!ok) {
		fail();
		return;
	}

#ifndef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	currentPhase = 1;
	emit outer.phaseChanged();
	pages[0]->mainFrame()->print(printer);
	progressString = "";
	emit outer.progressChanged(-1);
#else
 	bool first=true;
 	int actualPage=1;
 	int cc=settings.collate?settings.copies:1;
 	int pc=settings.collate?1:settings.copies;
	
	bool hasHeaderFooter = 
		settings.header.line || settings.footer.line ||
		!settings.header.left.isEmpty() || !settings.footer.left.isEmpty() ||
		!settings.header.center.isEmpty() || !settings.footer.center.isEmpty() ||
		!settings.header.right.isEmpty() || !settings.footer.right.isEmpty();

	currentPhase = 4;
	emit outer.phaseChanged();

	progressString = "Preparing";
	emit outer.progressChanged(0);
			
	for(int cc_=0; cc_ < cc; ++cc_) {
		logicalPage=1;
		for(int d=0; d < pages.size(); ++d) {
			//Output the table of content now
			if(tocPrinter && d == (settings.cover.isEmpty()?0:1)) {
				painter->save();
				for(int p=0; p < tocPrinter->pageCount(); ++p) {
					for(int pc_=0; pc_ < pc; ++pc_) {
						beginPage(actualPage,first);
						tocPrinter->spoolPage(p);
						endPage(true, hasHeaderFooter);
					}
					++logicalPage;
				}
				painter->restore();
			}
			painter->save();

			//output 
			QWebPrinter wp(pages[d]->mainFrame(), printer, *painter);
			QString l1=pages[d]->mainFrame()->url().path().split("/").back()+"#";
			QString l2=pages[d]->mainFrame()->url().toString() + "#";

			if (settings.cover.isEmpty() || d != 0) {
				//The toc printer adds an extra TOC document to the beginning of the outline
				//Using a cover does not add anything to cover
				int delta = (tocPrinter?1:0) - (settings.cover.isEmpty()?0:1);
				if(tocPrinter) tocPrinter->fillLinks(d+delta, localLinks[d]);
				outline->fillAnchors(d+delta, anchors[d]);				
			}

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
					beginPage(actualPage,first);
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
					endPage(settings.cover.isEmpty() || d != 0, hasHeaderFooter);
				}
				if (settings.cover.isEmpty() || d != 0) ++logicalPage;
			}
			painter->restore();
		}
 	}
	outline->printOutline(printer);
	outline->dumpOutline();
 	painter->end();
#endif
	if (settings.out == "-" && lout != "/dev/stdout") {
		QFile i(lout);
		QFile o;
#ifdef Q_OS_WIN32
		_setmode(_fileno(stdout), _O_BINARY);
#endif
		if( !i.open(QIODevice::ReadOnly) || 
			!o.open(stdout,QIODevice::WriteOnly) ||
			!MultiPageLoader::copyFile(i,o) ) {
			emit outer.error("Count not write to stdout");
			fail();
			return;
		}
	}
	clearResources();
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	currentPhase = 5;
#else
	currentPhase = 2;
#endif
	emit outer.phaseChanged();
	convertionDone = true;
	emit outer.finished(true);
	
  qApp->exit(0); // quit qt's event handling
}

#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
QWebPage * PageConverterPrivate::loadHeaderFooter(QString url, const QHash<QString, QString> & parms) {
	QUrl u = MultiPageLoader::guessUrlFromString(url);

	for(QHash<QString, QString>::const_iterator i=parms.begin(); i != parms.end(); ++i)
		u.addQueryItem(i.key(), i.value());
	
	return hfLoader.addResource(u);
}

/*!
 * Replace some variables in a string used in a header or footer
 * \param q the string to substitute in
 */
QString PageConverterPrivate::hfreplace(const QString & q, const QHash<QString, QString> & parms) {
	QString r=q;
	for(QHash<QString, QString>::const_iterator i=parms.begin(); i != parms.end(); ++i)
		r=r.replace("["+i.key()+"]", i.value(), Qt::CaseInsensitive);
	return r;
}
#endif

bool PageConverterPrivate::convert() {
	convertionDone=false;
	beginConvert();
	while(!convertionDone) {
    qApp->exec();
	  //qApp->processEvents(QEventLoop::WaitForMoreEvents | QEventLoop::AllEvents);
	}
	return !error;
}

void PageConverterPrivate::clearResources() {
	pageLoader.clearResources();
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	hfLoader.clearResources();

	if (outline)
		delete outline;
	outline = NULL;

	if (tocPrinter)
		delete tocPrinter;
	tocPrinter = NULL;
	anchors.clear();
	localLinks.clear();
	externalLinks.clear();

	foreach (QWebPage * page, headers)
		delete page;
	headers.clear();

	foreach (QWebPage * page, footers)
		delete page;
	footers.clear();
#endif
	foreach(QWebPage * page, pages)
		delete page;
	pages.clear();
	if (printer) 
		delete printer;
	printer = NULL;
	
	if (painter)
		delete painter;
	painter = NULL;
}

void PageConverterPrivate::cancel() {
	error=true;
}

/*!
  \class PageConverter
  \brief Class responsible for converting html pages to pdf
  \todo explain something about the conversion process here, and mention stages
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
  \brief Count the number of phases that the conversion process goes though
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
	if (phase < 0 || d->phaseDescriptions.size() <= phase) phase=d->currentPhase;
	if (phase < 0 || d->phaseDescriptions.size() <= phase) return "Invalid";
	return d->phaseDescriptions[phase];
}

/*!
  \brief return a textual description of progress in the current phase
*/
QString PageConverter::progressString() {
	return d->progressString;
}

/*!
  \brief return the HTTP return code, of the converted page
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
  \brief Start a asynchronous conversion of html pages to a pdf document.
  Once conversion is done an finished signal will be emitted
*/
void PageConverter::beginConvertion() {
	d->beginConvert();
}

/*!
  \brief Synchronous convert html pages to a pdf document.
*/
bool PageConverter::convert() {
	return d->convert();
}

/*!
  \brief Cancel a running conversion
*/
void PageConverter::cancel() {
	d->cancel();
}

/*!
  \brief Returns the settings object associated with the page converter
*/
const Settings & PageConverter::settings() const {
	return d->settings;
}

/*!
  \fn PageConverter::warning(const QString & message)
  \brief Signal emitted when some non fatal warning occurs during conversion
  \param message The warning message
*/

/*!
  \fn PageConverter::error(const QString & message)
  \brief Signal emitted when a fatal error has occurred during conversion
  \param message A message describing the fatal error
*/

/*!
  \fn PageConverter::phaseChanged()
  \brief Signal emitted when the converter has reached a new phase
*/

/*!
  \fn PageConverter::progressChanged()
  \brief Signal emitted when some progress has been done in the conversion phase
*/

/*!
  \fn PageConverter::finised()
  \brief Signal emitted when conversion has finished.
*/




