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
#include "multipageloader_p.hh"
#include <QFile>
#include <QFileInfo>
#include <QTimer>
#include <QNetworkCookie>
#include <QUuid>

/*!
  \file multipageloader.hh
  \brief Defines the MultiPageLoader class
*/

/*!
  \file multipageloader_p.hh
  \brief Defines the MultiPageLoaderPrivate class
*/


MyNetworkAccessManager::MyNetworkAccessManager(bool block): blockAccess(block) {}

void MyNetworkAccessManager::allow(QString path) {
	QString x = QFileInfo(path).canonicalFilePath();
	if (x.isEmpty()) return;
	allowed.insert(x);
}

QNetworkReply * MyNetworkAccessManager::createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData) {
	if (req.url().scheme() == "file" && blockAccess) { 
		bool ok=false;
		QString path = QFileInfo(req.url().toLocalFile()).canonicalFilePath();
		QString old = "";
		while(path != old) {
			if (allowed.contains(path)) {
				ok=true;
				break;
			}
			old = path;
			path = QFileInfo(path).path();
		}
		if (!ok) {
			QNetworkRequest r2 = req;
			emit warning(QString("Blocked access to file %1").arg(QFileInfo(req.url().toLocalFile()).canonicalFilePath()));
			r2.setUrl(QUrl("about:blank"));
			return QNetworkAccessManager::createRequest(op, r2, outgoingData);
		}
	}
	return QNetworkAccessManager::createRequest(op, req, outgoingData);
}


MyQWebPage::MyQWebPage(MultiPageLoader & mpl, Settings & s): settings(s), multiPageLoader(mpl) {};
													
bool MyQWebPage::shouldInterruptJavaScript() {
	if (settings.stopSlowScripts) {
		multiPageLoader.warning("A slow script was stopped");
		return true;
	}
	return false;
}

void MyQWebPage::javaScriptAlert(QWebFrame *, const QString & msg) {
	multiPageLoader.warning(QString("Javascript alert: %1").arg(msg));
}

bool MyQWebPage::javaScriptConfirm(QWebFrame *, const QString & msg) {
	multiPageLoader.warning(QString("Javascript confirm: %1 (answered yes)").arg(msg));
	return true;
}

bool MyQWebPage::javaScriptPrompt(QWebFrame *, const QString & msg, const QString & defaultValue, QString * result) {
	multiPageLoader.warning(QString("Javascript prompt: %1 (answered %2)").arg(msg,defaultValue));
	result = (QString*)&defaultValue;
	return true;
}

void MyQWebPage::javaScriptConsoleMessage(const QString & message, int lineNumber, const QString & sourceID) {
	if (settings.debugJavascript)
		multiPageLoader.warning(QString("%1:%2 %3").arg(sourceID).arg(lineNumber).arg(message));
}

void MyCookieJar::addGlobalCookie(const QString & name, const QString & value) {
	globalCookies.append(QNetworkCookie(name.toUtf8(), value.toUtf8()));
}

QList<QNetworkCookie> MyCookieJar::cookiesForUrl(const QUrl & url) const {
	QList<QNetworkCookie> list = QNetworkCookieJar::cookiesForUrl(url);
	list.append(globalCookies);
	return list;
}

void MyCookieJar::loadFromFile(const QString & path) {
	QFile cookieJar(path);
	if (cookieJar.open(QIODevice::ReadOnly | QIODevice::Text) )
		setAllCookies(QNetworkCookie::parseCookies(cookieJar.readAll()));
}

void MyCookieJar::saveToFile(const QString & path) {
	QFile cookieJar(path);
	if (cookieJar.open(QIODevice::WriteOnly | QIODevice::Text) )
		foreach (const QNetworkCookie & cookie, allCookies()) {
			cookieJar.write(cookie.toRawForm());
			cookieJar.write(";\n");
		}
}


/*!
 * Track and handle network errors
 * \param reply The networkreply that has finished
 */
void MultiPageLoaderPrivate::amfinished(QNetworkReply * reply) {
	int error = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	if (error > 399 && httpErrorCode == 0) httpErrorCode = error;
}

/*!
 * Called when the page requires authentication, filles in the username
 * and password supplied on the command line
 */
void MultiPageLoaderPrivate::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator) {
	if (settings.username.isEmpty()) {
		//If no username is given, complain the such is required
		emit outer.error("Authentication Required");
		reply->abort();
		fail();
	} else if (loginTry >= 2) {
		//If the login has failed a sufficient number of times,
		//the username or password must be wrong
		emit outer.error("Invalid username or password");
		reply->abort();
		fail();
	} else {
		authenticator->setUser(settings.username);
		authenticator->setPassword(settings.password);
		++loginTry;
	}
}


/*!
 * Copy a file from some place to another
 * \param src The source to copy from
 * \param dst The destination to copy to
 */
bool MultiPageLoader::copyFile(QFile & src, QFile & dst) {
//      TODO enable again when
//      http://bugreports.qt.nokia.com/browse/QTBUG-6894
//      is fixed
//      QByteArray buf(1024*1024*5,0);
//      while ( qint64 r=src.read(buf.data(),buf.size())) {
//          if (r == -1) return false;
//          if (dst.write(buf.data(),r) != r) return false;
//      }

    if (dst.write( src.readAll() ) == -1) return false;

	src.close();
	dst.close();
	return true;
}

/*!
 * Handel any ssl error by ignoring
 */
void MultiPageLoaderPrivate::sslErrors(QNetworkReply *reply, const QList<QSslError> &) {
	//We ignore any ssl error, as it is next to impossible to send or receive
	//any private information with wkhtmltopdf anyhow, seeing as you cannot authenticate
	reply->ignoreSslErrors();
	emit outer.warning("SSL error ignored");
}

MultiPageLoaderPrivate::MultiPageLoaderPrivate(Settings & s, MultiPageLoader & o): 
	outer(o), settings(s), networkAccessManager(s.blockLocalFileAccess) {
	foreach(const QString & path, s.allowed)
		networkAccessManager.allow(path);

	//If some ssl error occures we want sslErrors to be called, so the we can ignore it
	connect(&networkAccessManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)),this,
	        SLOT(sslErrors(QNetworkReply*, const QList<QSslError>&)));

	connect(&networkAccessManager, SIGNAL(finished (QNetworkReply *)),
			this, SLOT(amfinished (QNetworkReply *) ) );

	connect(&networkAccessManager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator *)),this,
	        SLOT(authenticationRequired(QNetworkReply *, QAuthenticator *)));

	connect(&networkAccessManager, SIGNAL(warning(const QString &)), 
			this, SLOT(warning(const QString &)));
	cookieJar = new MyCookieJar();

	networkAccessManager.setCookieJar(cookieJar);

	if (!settings.cookieJar.isEmpty()) 
		cookieJar->loadFromFile(settings.cookieJar);
	
	typedef QPair<QString, QString> SSP;
	foreach (const SSP & pair, settings.cookies)
		cookieJar->addGlobalCookie(pair.first, pair.second);
		
	//If we must use a proxy, create a host of objects
	if (!settings.proxy.host.isEmpty()) {
		QNetworkProxy proxy;
		proxy.setHostName(settings.proxy.host);
		proxy.setPort(settings.proxy.port);
		proxy.setType(settings.proxy.type);
		// to retrieve a web page, it's not needed to use a fully transparent
		// http proxy. Moreover, the CONNECT() method is frequently disabled
		// by proxies administrators.
#if QT_VERSION >= 0x040500
		if (settings.proxy.type == QNetworkProxy::HttpProxy)
			proxy.setCapabilities(QNetworkProxy::CachingCapability);
#endif
		if (!settings.proxy.user.isEmpty())
			proxy.setUser(settings.proxy.user);
		if (!settings.proxy.password.isEmpty())
			proxy.setPassword(settings.proxy.password);
		networkAccessManager.setProxy(proxy);
	}
}

MultiPageLoaderPrivate::~MultiPageLoaderPrivate() {
	clearResources();
}

QWebPage * MultiPageLoaderPrivate::addResource(const QUrl & url) {
	if (url.scheme() == "file")
		networkAccessManager.allow(url.toLocalFile());
	QWebPage * page = new MyQWebPage(outer, settings); 
	pages.push_back(page);
	urls.push_back(url);
	page->setNetworkAccessManager(&networkAccessManager);

	page->mainFrame()->setZoomFactor(settings.zoomFactor);

	pageToIndex[page] = pages.size()-1;

	connect(page, SIGNAL(loadStarted()), this, SLOT(loadStarted()));
	connect(page, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
	connect(page, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
	connect(page, SIGNAL(printRequested(QWebFrame*)), this, SLOT(printRequested(QWebFrame*)));

	progressList.push_back(0);
	finishedList.push_back(false);
	signalPrintList.push_back(false);
	return page;
}
	
void MultiPageLoaderPrivate::load() {
	httpErrorCode = 0;
	loginTry = 0;
	progressSum=0;
	finishedSum=0;
	signalPrintSum=0;
	loadStartedEmitted=false;
	error=false;
	loadingPages=0;

	QString boundary = QUuid::createUuid().toString().remove('-').remove('{').remove('}');
	QByteArray postData;
	
	foreach (const Settings::PostItem & pi, settings.post) {
		//TODO escape values here
		postData.append("--");
		postData.append(boundary);
		postData.append("\ncontent-disposition: form-data; name=\"");
		postData.append(pi.name);
		postData.append('\"');
		if (pi.file) {
			QFile f(pi.value);
			if (!f.open(QIODevice::ReadOnly) ) {
				emit outer.error(QString("Unable to open file ")+pi.value);
				fail();
			}
			postData.append("; filename=\"");
			postData.append( QFileInfo(pi.value).fileName());
			postData.append("\"\n\n");
			postData.append( f.readAll() );
			//TODO ADD MIME TYPE
		} else {
			postData.append("\n\n");
			postData.append(pi.value);
		}
		postData.append('\n');
	}
	if (!postData.isEmpty()) {
		postData.append("--");
		postData.append(boundary);
		postData.append("--\n");
	}

	for (int i=0; i < pages.size(); ++i) {
		QNetworkRequest r = QNetworkRequest(urls[i]);
		typedef QPair<QString, QString> HT;
		foreach (const HT & j, settings.customHeaders)
			r.setRawHeader(j.first.toAscii(), j.second.toAscii());
	
		if (postData.isEmpty())
			pages[i]->mainFrame()->load(r);
		else {
			r.setHeader(QNetworkRequest::ContentTypeHeader, QString("multipart/form-data, boundary=")+boundary);
			pages[i]->mainFrame()->load(r, QNetworkAccessManager::PostOperation, postData);
		}
	}	
}

void MultiPageLoaderPrivate::clearResources() {
	pages.clear();
	urls.clear();
	progressList.clear();
	finishedList.clear();
	tempIn.remove();
}

void MultiPageLoaderPrivate::cancel() {
	foreach (QWebPage * page, pages) 
		page->triggerAction(QWebPage::Stop);
}

void MultiPageLoaderPrivate::fail() {
	error = true;
	cancel();
	clearResources();
}


/*!
 * Once loading starting, this is called
 */
void MultiPageLoaderPrivate::loadStarted() {
	loadingPages++;
	if (loadStartedEmitted) return;
	loadStartedEmitted=true;
	emit outer.loadStarted();
}

/*!
 * Called when the page is loading, display some progress to the using
 * \param progress the loading progress in percent
 */
void MultiPageLoaderPrivate::loadProgress(int progress) {
	if (!pageToIndex.count(QObject::sender())) return;
	
	int idx=pageToIndex[QObject::sender()];
		
	progressSum -= progressList[idx];
	progressList[idx] = progress;
	progressSum += progressList[idx];

	emit outer.loadProgress(progressSum / progressList.size());
}

void MultiPageLoaderPrivate::loadFinished(bool ok) {
	loadingPages--;
	error = error || !ok;
	if (!pageToIndex.count(QObject::sender())) return;
	
	int idx=pageToIndex[QObject::sender()];

	if (!ok)
		emit outer.error(QString("Failed loading page ") + urls[idx].toString());

	if (!finishedList[idx]) {
		finishedList[idx] = true;
		finishedSum += 1;
	}

	if (signalPrintSum == signalPrintList.size()) 
		timedFinished();
	else if (finishedSum == finishedList.size())
		QTimer::singleShot(settings.jsredirectwait, this, SLOT(timedFinished()));
}

void MultiPageLoaderPrivate::printRequested(QWebFrame *) {
	if (!pageToIndex.count(QObject::sender())) return;
	int idx=pageToIndex[QObject::sender()];
	if (!signalPrintList[idx]) {
		signalPrintList[idx] = true;
		signalPrintSum += 1;
	}
	if (signalPrintSum == signalPrintList.size()) 
		timedFinished();
}

void MultiPageLoaderPrivate::timedFinished() {
	if(loadingPages == 0) {
		if (!settings.cookieJar.isEmpty())
			cookieJar->saveToFile(settings.cookieJar);
		emit outer.loadFinished(!error);
	}
}

void MultiPageLoaderPrivate::warning(const QString & msg) {
	emit outer.warning(msg);
}

/*!
  \brief Construct a multipage loader object, load settings read from the supplied settings
  \param s The settings to be used while loading pages
*/
MultiPageLoader::MultiPageLoader(Settings & s):
	d(new MultiPageLoaderPrivate(s, *this)) {
}

MultiPageLoader::~MultiPageLoader() {
	delete d;
}

/*!
  \brief Add a resource, to be loaded described by a string
  @param string Url describing the resource to load
*/
QWebPage * MultiPageLoader::addResource(const QString & string) {
	QString url=string;
	if (url == "-") {
		QFile in;
		in.open(stdin,QIODevice::ReadOnly);
		url = d->tempIn.create(".html");
		QFile tmp(url);
		if(!tmp.open(QIODevice::WriteOnly) || !copyFile(in, tmp)) {
			emit error("Unable to create temporery file");
			return NULL;
		}
	}
	return addResource(guessUrlFromString(url));
}

/*!
  \brief Add a page to be loaded
  @param url Url of the page to load
*/
QWebPage * MultiPageLoader::addResource(const QUrl & url) {
	return d->addResource(url);
}

/*!
  \brief Guess a url, by looking at a string
  
  (shamelessly copied from Arora Project)
  \param string The string the is suppose to be some kind of url
*/
QUrl MultiPageLoader::guessUrlFromString(const QString &string) {
	QString urlStr = string.trimmed();

	// check if the string is just a host with a port
	QRegExp hostWithPort(QLatin1String("^[a-zA-Z\\.]+\\:[0-9]*$"));
	if (hostWithPort.exactMatch(urlStr))
		urlStr = QLatin1String("http://") + urlStr;

	// Check if it looks like a qualified URL. Try parsing it and see.
	QRegExp test(QLatin1String("^[a-zA-Z]+\\:.*"));
	bool hasSchema = test.exactMatch(urlStr);
	if (hasSchema) {
		bool isAscii = true;
		foreach (const QChar &c, urlStr) {
			if (c >= 0x80) {
				isAscii = false;
				break;
			}
		}

		QUrl url;
		if (isAscii) {
			url = QUrl::fromEncoded(urlStr.toAscii(), QUrl::TolerantMode);
		} else {
			url = QUrl(urlStr, QUrl::TolerantMode);
		}
		if (url.isValid())
			return url;
	}

	// Might be a file.
	if (QFile::exists(urlStr)) {
		QFileInfo info(urlStr);
		return QUrl::fromLocalFile(info.absoluteFilePath());
	}

	// Might be a shorturl - try to detect the schema.
	if (!hasSchema) {
		int dotIndex = urlStr.indexOf(QLatin1Char('.'));
		if (dotIndex != -1) {
			QString prefix = urlStr.left(dotIndex).toLower();
			QString schema = (prefix == QLatin1String("ftp")) ? prefix : QLatin1String("http");
			QUrl url(schema + QLatin1String("://") + urlStr, QUrl::TolerantMode);
			if (url.isValid())
				return url;
		}
	}

	// Fall back to QUrl's own tolerant parser.
	QUrl url = QUrl(string, QUrl::TolerantMode);

	// finally for cases where the user just types in a hostname add http
	if (url.scheme().isEmpty())
		url = QUrl(QLatin1String("http://") + string, QUrl::TolerantMode);
	return url;
}

/*!
  \brief Return the most severe http error code returned during loading
 */
int MultiPageLoader::httpErrorCode() {
	return d->httpErrorCode;
}

/*!
  \brief Begin loading all the resources added
*/
void MultiPageLoader::load() {
	d->load();
}

/*!
  \brief Clear all the resources
*/
void MultiPageLoader::clearResources() {
	d->clearResources();
}

/*!
  \brief Cancel the loading of the pages
*/
void MultiPageLoader::cancel() {
	d->cancel();
}

/*!
  \fn MultiPageLoader::loadFinished(bool ok)
  \brief Signal emmited when all pages have been loaded
  \param ok True if all the pages have been loaded sucessfully
*/

/*!
  \fn MultiPageLoader::loadProgress(int progress)
  \brief Signal emmited once load has progressed
  \param progress Progress in percent
*/

/*!
  \fn MultiPageLoader::loadStarted()
  \brief Signal emmited when loading has started
*/

/*!
  \fn void MultiPageLoader::warning(QString text)
  \brief Signal emmitted when a none fatal warning has occured
  \param text A string descripting the warning
*/

/*!
  \fn void MultiPageLoader::error(QString text)
  \brief Signal emmitted when a fatel error has occured
  \param text A string describing the error
*/
