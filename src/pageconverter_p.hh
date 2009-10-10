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
#include "pageconverter.hh"
#include <QFile>
#include <QRegExp>
#include <qnetworkreply.h>
#include <QAtomicInt>
#include <QWebPage>
#include <QPainter>
#include <QPrinter>
#include <QWebElement>
#include "multipageloader.hh"
#include "tempfile.hh"
#include <QWaitCondition>
#include <QMutex>
#include "outline.hh"
#include "tocprinter.hh"

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
	MultiPageLoader hfLoader;
	QString progressString;
private:
	PageConverter & outer;
	void clearResources();
	TempFile tempOut;
	bool error;
	QList<QWebPage *> pages;
	QPrinter * printer;
	QPainter * painter;
	Outline * outline;
	TocPrinter * tocPrinter;
	QString lout;
	int logicalPages;
	int logicalPage;
	int actualPages;
	
	int tocPages;

	bool convertionDone;

	QHash<int, QHash<QString, QWebElement> > anchors;
	QHash<int, QVector< QPair<QWebElement,QString> > > localLinks;
	QHash<int, QVector< QPair<QWebElement,QString> > > externalLinks;

	QList<QWebPage *> headers;
	QList<QWebPage *> footers;

	void fail();
	void beginPage(int & actualPage, bool & first);
	void endPage(bool actual, bool hasHeaderFooter);
	QString hfreplace(const QString & q, const QHash<QString, QString> & parms);
	QWebPage * loadHeaderFooter(QString url, const QHash<QString, QString> & parms);
public slots:
	void loadProgress(int progress);
	void preparePrint(bool ok);
	void printPage(bool ok);
	void beginConvert();
	void cancel();
	bool convert();
};

#endif //__TEXTUALFEEDBACK_P_HH__
