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
#include <QNetworkCookieJar>
#include <QFileInfo>

class MyNetworkAccessManager: public QNetworkAccessManager {
	Q_OBJECT
private:
	bool blockAccess;
	QSet<QString> allowed;
public:
	void allow(QString path);
	MyNetworkAccessManager(bool block);
	QNetworkReply * createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0);
signals:
	void warning(const QString & text);
};

class MyQWebPage: public QWebPage {
	Q_OBJECT ;
private:
	Settings & settings;
	MultiPageLoader & multiPageLoader;
public:
	MyQWebPage(MultiPageLoader & mpl, Settings & s);
	virtual void javaScriptAlert(QWebFrame * frame, const QString & msg);
	virtual bool javaScriptConfirm(QWebFrame * frame, const QString & msg);
	virtual bool javaScriptPrompt(QWebFrame * frame, const QString & msg, const QString & defaultValue, QString * result);
	virtual void javaScriptConsoleMessage(const QString & message, int lineNumber, const QString & sourceID);
public slots:
	bool shouldInterruptJavaScript();
};

class MyCookieJar: public QNetworkCookieJar {
private:
	QList<QNetworkCookie> globalCookies;
public:
	void addGlobalCookie(const QString & name, const QString & value);
	QList<QNetworkCookie> cookiesForUrl(const QUrl & url) const;
	void loadFromFile(const QString & path);
	void saveToFile(const QString & path);
};

class MultiPageLoaderPrivate: public QObject {
	Q_OBJECT
public:
	MyCookieJar * cookieJar;
	MultiPageLoader & outer;
	Settings & settings;
	int httpErrorCode;
	QList<QWebPage *> pages;
	QList<QUrl> urls;
	QList<QString> temporaryFiles;

	int loginTry;

	MyNetworkAccessManager networkAccessManager;
	QHash<QObject *, int> pageToIndex;

	QList<int> progressList;
	QList<bool> finishedList;
	QList<bool> signalPrintList;
	int signalPrintSum;
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
	void warning(const QString & msg);
	void loadStarted();
	void loadProgress(int progress);
	void loadFinished(bool ok);
	void timedFinished();
	void sslErrors(QNetworkReply *reply, const QList<QSslError> &);
	void amfinished(QNetworkReply * reply);
	void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
	void printRequested(QWebFrame * frame);
};
