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

class PageConverterPrivate: public QObject {
	Q_OBJECT
public:
	PageConverterPrivate(Settings & s, PageConverter & o);
	void copyFile(QFile & src, QFile & dst);
	void beginConvert();
	void convert();
	void cancel();

	QList<QString> phaseDescriptions;
	int currentPhase;
	Settings & settings;

	MultiPageLoader pageLoader;
	MultiPageLoader hfLoader;
	QString progressString;
private:
	PageConverter & outer;

	TempFile tempOut;
	
// 	int networkError;
// 	int loginTry;
//	QNetworkAccessManager networkAccessManager;
	//!Keep track of the numer of pages loading
//	QAtomicInt loading; 
//	QList<QString> temporaryFiles;
	QList<QWebPage *> pages;
	QPrinter * printer;
	QPainter * painter;
	int logicalPages;
	int logicalPage;
	int actualPages;
	QList<int> pageCount;
	int tocPages;


	QHash<QString, QString> calculateHeaderFooterParams(int d, int page);
	QHash<int, QHash<QString, QWebElement> > anchors;
	QHash<int, QVector< QPair<QWebElement,QString> > > localLinks;
	QHash<int, QVector< QPair<QWebElement,QString> > > externalLinks;
	QHash<int, QMap< QPair<int, QPair<qreal,qreal> >, QWebElement> > headings;
	QList<QWebPage *> headers;
	QList<QWebPage *> footers;

	QString hfreplace(const QString & q);
	QWebPage * loadHeaderFooter(QString url, int d, int page);
private slots:
	void loadProgress(int progress);
	
	// void amfinished(QNetworkReply * r);
// 	void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
// 	void loadFinished(bool ok);
// 	void loadStarted();
// 	void loadProgress(int progress);
// 	void sslErrors(QNetworkReply *reply, const QList<QSslError> &);
	void preparePrint(bool ok);
	void printPage(bool ok);
	
};

#endif //__TEXTUALFEEDBACK_P_HH__
