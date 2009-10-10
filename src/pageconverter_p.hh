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
#ifndef __TEXTUALFEEDBACK_P_HH__
#define __TEXTUALFEEDBACK_P_HH__
#include "multipageloader.hh"
#include "outline.hh"
#include "pageconverter.hh"
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

class PageConverterPrivate: public QObject {
	Q_OBJECT
public:
	PageConverterPrivate(Settings & s, PageConverter & o);
	~PageConverterPrivate();
	void copyFile(QFile & src, QFile & dst);

	QList<QString> phaseDescriptions;
	int currentPhase;
	Settings & settings;

	MultiPageLoader pageLoader;
	QString progressString;
private:
	PageConverter & outer;
	void clearResources();
	TempFile tempOut;
	bool error;
	QList<QWebPage *> pages;
	QPrinter * printer;
	QPainter * painter;
	QString lout;
	int logicalPages;
	int logicalPage;
	int actualPages;
	
	int tocPages;

	bool convertionDone;

#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	MultiPageLoader hfLoader;
	QHash<int, QHash<QString, QWebElement> > anchors;
	QHash<int, QVector< QPair<QWebElement,QString> > > localLinks;
	QHash<int, QVector< QPair<QWebElement,QString> > > externalLinks;

	QList<QWebPage *> headers;
	QList<QWebPage *> footers;
	Outline * outline;
	TocPrinter * tocPrinter;
	void beginPage(int & actualPage, bool & first);
	void endPage(bool actual, bool hasHeaderFooter);
	QString hfreplace(const QString & q, const QHash<QString, QString> & parms);
	QWebPage * loadHeaderFooter(QString url, const QHash<QString, QString> & parms);
#endif

	void fail();
public slots:
	void loadProgress(int progress);
	void preparePrint(bool ok);
	void printPage(bool ok);
	void beginConvert();
	void cancel();
	bool convert();
	void forwardError(QString error);
	void forwardWarning(QString warning);
};

#endif //__TEXTUALFEEDBACK_P_HH__
