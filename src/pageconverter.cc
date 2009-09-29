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
#include "pageconverter_p.hh"
#include <qfileinfo.h>
#include <QAuthenticator>
#include <QTimer>

/*!
 * Copy a file from some place to another
 * \param src The source to copy from
 * \param dst The destination to copy to
 */
void PageConverterPrivate::copyFile(QFile & src, QFile & dst) {
	QByteArray buf(1024*1024*5,0);
	while(qint64 r=src.read(buf.data(),buf.size()))
		dst.write(buf.data(),r);
	src.close();
	dst.close();
}

/*!
 * Guess a url, by looking at a string
 * (shamelessly copied from Arora Project)
 * \param string The string the is suppose to be some kind of url
 */
QUrl PageConverterPrivate::guessUrlFromString(const QString &string) {
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
PageConverterPrivate::PageConverterPrivate(Settings & s, Feedback & f) :
	settings(s), feedback(f) {
	
	//If some ssl error occures we want sslErrors to be called, so the we can ignore it
	connect(&networkAccessManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)),this,
	        SLOT(sslErrors(QNetworkReply*, const QList<QSslError>&)));

	connect(&networkAccessManager, SIGNAL(finished (QNetworkReply *)),
			this, SLOT(amfinished (QNetworkReply *) ) );

	connect(&networkAccessManager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator *)),this,
	        SLOT(authenticationRequired(QNetworkReply *, QAuthenticator *)));

}


/*!
 * Track and handle network errors
 * \param reply The networkreply that has finished
 */
void PageConverterPrivate::amfinished(QNetworkReply * reply) {
	int error = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	if(error > 399 && networkError == 0) networkError = error;
}

/*!
 * Called when the page requires authentication, filles in the username
 * and password supplied on the command line
 */
void PageConverterPrivate::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator) {
	if (settings.username.isEmpty()) {
		//If no username is given, complain the such is required
		feedback.error("Authentication Required\n");
		reply->abort();
	} else if (loginTry >= 2) {
		//If the login has failed a sufficient number of times,
		//the username or password must be wrong
		feedback.error("Invalid username or password");
		reply->abort();
	} else {
		authenticator->setUser(settings.username);
		authenticator->setPassword(settings.password);
		++loginTry;
	}
}


/*!
 * Once loading is finished, we start the printing
 * \param ok did the loading finish correctly?
 */
void PageConverterPrivate::loadFinished(bool ok) {
	//Keep track of the number of pages currently loading
	#warning "This is a race condition"
	loading.deref();
	if (!ok) {
		//It went bad, return with 1
		feedback.error("Failed loading page");
        #warning "FIX ME"
		exit(1);
		return;
	}

	feedback.nextState("Waiting for redirect");
	if (loading == 0) {
		//Wait a little while for js-redirects, and then print
		QTimer::singleShot(settings.jsredirectwait, this, SLOT(preparePrint()));
	}
}

/*!
 * Once loading starting, this is called
 */
void PageConverterPrivate::loadStarted() {
	//Keep track of the number of pages currently loading
	#warning "This is a race condition"
	loading.ref();
}

/*!
 * Called when the page is loading, display some progress to the using
 * \param progress the loading progress in percent
 */
void PageConverterPrivate::loadProgress(int progress) {
	//Print out the load status
	feedback.progress(progress, 100, "%", false);
	fflush(stdout);
}

/*!
 * Handel any ssl error by ignoring
 */
void PageConverterPrivate::sslErrors(QNetworkReply *reply, const QList<QSslError> &) {
	//We ignore any ssl error, as it is next to impossible to send or receive
	//any private information with wkhtmltopdf anyhow, seeing as you cannot authenticate
	reply->ignoreSslErrors();
}

void PageConverterPrivate::convert() {
	networkError = 0;
	loginTry = 0;
	// 	pages.clear();
// 	pageStart.clear();
}


void PageConverter::convert() {
	d->convert();
}

PageConverter::PageConverter(Settings & settings, Feedback & feedback):
	d(new PageConverterPrivate(settings, feedback)) {
}

PageConverter::~PageConverter() {
	delete d;
}
