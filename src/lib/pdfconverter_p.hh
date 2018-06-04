// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010 wkhtmltopdf authors
//
// This file is part of wkhtmltopdf.
//
// wkhtmltopdf is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// wkhtmltopdf is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with wkhtmltopdf.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __PDFCONVERTER_P_HH__
#define __PDFCONVERTER_P_HH__

#include "converter_p.hh"
#include "multipageloader.hh"
#include "outline.hh"
#include "pdfconverter.hh"
#include "pdfsettings.hh"
#include "tempfile.hh"
#include <QAtomicInt>
#include <QFile>
#include <QMutex>
#include <QPainter>
#include <QPrinter>
#include <QRegExp>
#include <QWaitCondition>
#include <QWebPage>
#include <qnetworkreply.h>
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
#include <QWebElement>
#endif

#include "dllbegin.inc"
namespace wkhtmltopdf {

class DLL_LOCAL PageObject {
public:
	static QMap<QWebPage *, PageObject *> webPageToObject;

	settings::PdfObject settings;
	LoaderObject * loaderObject;
	QWebPage * page;
	QString data;
	int number;

#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	QHash<QString, QWebElement> anchors;
	QVector< QPair<QWebElement,QString> > localLinks;
	QVector< QPair<QWebElement,QString> > externalLinks;
    // height length to reserve for header when printing page
    double headerReserveHeight;
    // height length to reserve for footer when printing page
    double footerReserveHeight;
    // keeps preloaded header to calculate header height
    QWebPage * measuringHeader;
    // keeps preloaded footer to calculate header height
    QWebPage * measuringFooter;
    // webprinter instance
    QWebPrinter *web_printer;
#endif

	int firstPageNumber;
	QList<QWebPage *> headers;
	QList<QWebPage *> footers;
	int pageCount;
	TempFile tocFile;

	void clear() {
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
		anchors.clear();
		localLinks.clear();
		externalLinks.clear();
		if (web_printer != 0)
			delete web_printer;
		web_printer=0;
#endif
		headers.clear();
		footers.clear();
		webPageToObject.remove(page);
 		page=0;
		tocFile.removeAll();
	}

	PageObject(const settings::PdfObject & set, const QString * d=NULL):
		settings(set), loaderObject(0), page(0)
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
		, headerReserveHeight(0), footerReserveHeight(0), measuringHeader(0), measuringFooter(0), web_printer(0)
#endif
	{
		if (d) data=*d;
	};

	~PageObject() {
		clear();
	}

};

class DLL_LOCAL PdfConverterPrivate: public ConverterPrivate {
	Q_OBJECT
public:
	PdfConverterPrivate(settings::PdfGlobal & s, PdfConverter & o);
	~PdfConverterPrivate();

	settings::PdfGlobal & settings;

	MultiPageLoader pageLoader;

private:
	PdfConverter & out;
	void clearResources();
	TempFile tempOut;
	QByteArray outputData;

	QList<PageObject> objects;
	QSize viewportSize;
	QPrinter * printer;
	QPainter * painter;
	QString lout;
	QString title;
	int currentObject;
	int actualPages;
	int pageCount;
	int tocPages;
	bool tocChanged;
	int actualPage;
	int pageNumber;
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	int objectPage;


	QHash<int, QHash<QString, QWebElement> > pageAnchors;
	QHash<int, QVector< QPair<QWebElement,QString> > > pageLocalLinks;
	QHash<int, QVector< QPair<QWebElement,QString> > > pageExternalLinks;
	QHash<int, QVector<QWebElement> > pageFormElements;
	bool pageHasHeaderFooter;
	
    // loader for measuringHeader and measuringFooter
    MultiPageLoader measuringHFLoader;

	MultiPageLoader hfLoader;
	MultiPageLoader tocLoader1;
	MultiPageLoader tocLoader2;

	MultiPageLoader * tocLoader;
	MultiPageLoader * tocLoaderOld;

	QHash<QString, PageObject *> urlToPageObj;

	Outline * outline;
	void findLinks(QWebFrame * frame, QVector<QPair<QWebElement, QString> > & local, QVector<QPair<QWebElement, QString> > & external, QHash<QString, QWebElement> & anchors);
	void endPage(PageObject & object, bool hasHeaderFooter, int objectPage,  int pageNumber);
	void fillParms(QHash<QString, QString> & parms, int page, const PageObject & object);
	QString hfreplace(const QString & q, const QHash<QString, QString> & parms);
	QWebPage * loadHeaderFooter(QString url, const QHash<QString, QString> & parms, const settings::PdfObject & ps);
    qreal calculateHeaderHeight(PageObject & object, QWebPage & header);

#endif
	QWebPage * currentHeader;
	QWebPage * currentFooter;
    QPrinter * createPrinter(const QString & tempFile);

#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	void handleTocPage(PageObject & obj);
	void preprocessPage(PageObject & obj);
	void spoolPage(int page);
	void spoolTo(int page);
	void handleHeader(QWebPage * frame, int page);
	void handleFooter(QWebPage * frame, int page);
	void beginPrintObject(PageObject & obj);
	void endPrintObject(PageObject & obj);
#endif

	void loadTocs();
	void loadHeaders();
public slots:
    void measuringHeadersLoaded(bool ok);
    void pagesLoaded(bool ok);
	void tocLoaded(bool ok);
	void headersLoaded(bool ok);

	void printDocument();

	void beginConvert();

	friend class PdfConverter;

	virtual Converter & outer();
};

}
#include "dllend.inc"
#endif //__PDFCONVERTER_P_HH__
