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

class PageConverterPrivate: public QObject {
	Q_OBJECT
public:
	PageConverterPrivate(Settings & s, Feedback & f);
	void copyFile(QFile & src, QFile & dst);
	QUrl guessUrlFromString(const QString &string);
	void convert();
private:
	Settings & settings;
	Feedback & feedback;
	int networkError;
	int loginTry;
	QNetworkAccessManager networkAccessManager;
	//!Keep track of the numer of pages loading
 	QAtomicInt loading; 
	QList<QString> temporaryFiles;
	QList<QWebPage *> pages;
private slots:
	void amfinished(QNetworkReply * r);
	void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
	void loadFinished(bool ok);
	void loadStarted();
	void loadProgress(int progress);
	void sslErrors(QNetworkReply *reply, const QList<QSslError> &);
};

#endif //__TEXTUALFEEDBACK_P_HH__
