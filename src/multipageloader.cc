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

/*!
  \file multipageloader.hh
  \brief Defines the MultiPageLoader class
*/

/*!
  \file multipageloader_p.hh
  \brief Defines the MultiPageLoaderPrivate class
*/

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
	} else if (loginTry >= 2) {
		//If the login has failed a sufficient number of times,
		//the username or password must be wrong
		emit outer.error("Invalid username or password");
		reply->abort();
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
void MultiPageLoaderPrivate::copyFile(QFile & src, QFile & dst) {
	QByteArray buf(1024*1024*5,0);
	while(qint64 r=src.read(buf.data(),buf.size()))
		dst.write(buf.data(),r);
	src.close();
	dst.close();
}

/*!
 * Handel any ssl error by ignoring
 */
void MultiPageLoaderPrivate::sslErrors(QNetworkReply *reply, const QList<QSslError> &) {
	//We ignore any ssl error, as it is next to impossible to send or receive
	//any private information with wkhtmltopdf anyhow, seeing as you cannot authenticate
	reply->ignoreSslErrors();
}

MultiPageLoaderPrivate::MultiPageLoaderPrivate(Settings & s, MultiPageLoader & o): 
	outer(o), settings(s) {
	
	//If some ssl error occures we want sslErrors to be called, so the we can ignore it
	connect(&networkAccessManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)),this,
	        SLOT(sslErrors(QNetworkReply*, const QList<QSslError>&)));

	connect(&networkAccessManager, SIGNAL(finished (QNetworkReply *)),
			this, SLOT(amfinished (QNetworkReply *) ) );

	connect(&networkAccessManager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator *)),this,
	        SLOT(authenticationRequired(QNetworkReply *, QAuthenticator *)));

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
	QWebPage * page = new QWebPage(); 
	pages.push_back(page);
	urls.push_back(url);
	page->setNetworkAccessManager(&networkAccessManager);
	//Todo multipage loader foobar
	page->mainFrame()->setZoomFactor(settings.zoomFactor);

	pageToIndex[page] = pages.size()-1;

	connect(page, SIGNAL(loadStarted()), this, SLOT(loadStarted()));
	connect(page, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
	connect(page, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));

	progressList.push_back(0);
	finishedList.push_back(false);
	return page;
}
	
void MultiPageLoaderPrivate::load() {
	httpErrorCode = 0;
	loginTry = 0;
	progressSum=0;
	finishedSum=0;
	loadStartedEmitted=false;
	error=false;
	loadingPages=0;
	for(int i=0; i < pages.size(); ++i) {
		QNetworkRequest r = QNetworkRequest(urls[i]);
		for(QHash<QString, QString>::const_iterator j = settings.customHeaders.constBegin(); j != settings.customHeaders.constEnd(); ++j)
			r.setRawHeader(j.key().toAscii(), j.value().toAscii());
		pages[i]->mainFrame()->load(r);
	}

// 		if (url == "-") {
// 			QFile in;
// 			in.open(stdin,QIODevice::ReadOnly);
// 			url = QDir::tempPath()+"/wktemp"+QUuid::createUuid().toString()+".html";
// 			temporaryFiles.push_back(url);
// 			QFile tmp(url);
// 			tmp.open(QIODevice::WriteOnly);
// 			copyFile(in,tmp);
// 		}

// 		page->mainFrame()->load(guessUrlFromString(url));
	
}

void MultiPageLoaderPrivate::clearResources() {
	pages.clear();
	urls.clear();
	progressList.clear();
	finishedList.clear();
}

void MultiPageLoaderPrivate::cancel() {
	foreach (QWebPage * page, pages) 
		page->triggerAction(QWebPage::Stop);
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
	if (!finishedList[idx]) {
		finishedList[idx] = true;
		finishedSum += 1;
	}

	if (finishedSum == finishedList.size())
		QTimer::singleShot(settings.jsredirectwait, this, SLOT(timedFinished()));
}

void MultiPageLoaderPrivate::timedFinished() {
	if(loadingPages == 0) 
		emit outer.loadFinished(!error);
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
	return addResource(guessUrlFromString(string));
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
