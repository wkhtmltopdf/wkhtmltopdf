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
#include "multipageloader.hh"
#include "tempfile.hh"
#include <QAtomicInt>
#include <QAuthenticator>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWebFrame>

class MultiPageLoaderPrivate: public QObject {
	Q_OBJECT
public:
	MultiPageLoader & outer;
	Settings & settings;
	int httpErrorCode;
	QList<QWebPage *> pages;
	QList<QUrl> urls;
	QList<QString> temporaryFiles;

	int loginTry;

	QNetworkAccessManager networkAccessManager;
	QHash<QObject *, int> pageToIndex;

	QList<int> progressList;
	QList<bool> finishedList;
	int progressSum;
	int finishedSum;
	bool loadStartedEmitted;
	bool error;
	int loadingPages;
	TempFile tempIn;

	MultiPageLoaderPrivate(Settings & s, MultiPageLoader & o);
	~MultiPageLoaderPrivate(); 
	QWebPage * addResource(const QUrl & url);
	void load();
	void clearResources();
	void cancel();
	void fail();
public slots:
	void loadStarted();
	void loadProgress(int progress);
	void loadFinished(bool ok);
	void timedFinished();
	void sslErrors(QNetworkReply *reply, const QList<QSslError> &);
	void amfinished(QNetworkReply * reply);
	void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
};
