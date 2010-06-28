// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010 wkhtmltopdf authors
//
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

#ifndef __PDFCONVERTER_P_HH__
#define __PDFCONVERTER_P_HH__
#include "converter_p.hh"
#include "multipageloader.hh"
#include "outline.hh"
#include "pdfconverter.hh"
#include "settings.hh"
#include "tempfile.hh"
#include "tocprinter.hh"
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

namespace wkhtmltopdf {

class PageObject {
public:
	static QMap<QWebPage *, PageObject *> webPageToObject;

	settings::Page settings;
	LoaderObject * loaderObject;
	QWebPage * page;

#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	QHash<QString, QWebElement> anchors;
	QVector< QPair<QWebElement,QString> > localLinks;
	QVector< QPair<QWebElement,QString> > externalLinks;
#endif

	int firstPageNumber;
	QList<QWebPage *> headers;
	QList<QWebPage *> footers;
	int pageCount;
	TempFile tocStyleFile;
	TempFile tocFile;

	void clear() {
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
		anchors.clear();
		localLinks.clear();
		externalLinks.clear();
#endif
		headers.clear();
		footers.clear();
		webPageToObject.remove(page);
 		page=0;
		tocStyleFile.remove();
		tocFile.remove();
	}

	PageObject(const settings::Page & set): settings(set), loaderObject(0), page(0) {};

	~PageObject() {
		clear();
	}

};

class PdfConverterPrivate: public ConverterPrivate {
	Q_OBJECT
public:
	PdfConverterPrivate(settings::Global & s, PdfConverter & o);
	~PdfConverterPrivate();

	settings::Global & settings;

	MultiPageLoader pageLoader;

private:
	PdfConverter & out;
	void clearResources();
	TempFile tempOut;

	QList<PageObject> objects;

	QPrinter * printer;
	QPainter * painter;
	QString lout;
	QString title;

	int actualPages;
	int pageCount;

	int tocPages;

#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	MultiPageLoader hfLoader;
	MultiPageLoader tocLoader1;
	MultiPageLoader tocLoader2;

	MultiPageLoader * tocLoader;
	MultiPageLoader * tocLoaderOld;

	QHash<QString, PageObject *> urlToPageObj;

	Outline * outline;
	TocPrinter * tocPrinter;
	void findLinks(QWebFrame * frame, QVector<QPair<QWebElement, QString> > & local, QVector<QPair<QWebElement, QString> > & external, QHash<QString, QWebElement> & anchors);
	void beginPage(int actualPage);
	void endPage(PageObject & object, bool hasHeaderFooter, int objectPage,  int pageNumber);
	void fillParms(QHash<QString, QString> & parms, int page, const PageObject & object);
	QString hfreplace(const QString & q, const QHash<QString, QString> & parms);
	QWebPage * loadHeaderFooter(QString url, const QHash<QString, QString> & parms, const settings::Page & ps);
#endif

	void loadTocs();
	void loadHeaders();
public slots:
	void pagesLoaded(bool ok);
	void tocLoaded(bool ok);
	void headersLoaded(bool ok);

	void printDocument();

	void beginConvert();

	friend class PdfConverter;

	virtual Converter & outer();
};

}
#endif //__PDFCONVERTER_P_HH__
