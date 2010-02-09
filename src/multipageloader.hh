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
#ifndef __MULTIPAGELOADER_HH__
#define __MULTIPAGELOADER_HH__
#include "settings.hh"
#include <QFile>
#include <QObject>
#include <QUrl>
#include <QWebPage>

class MyQWebPage;
class MultiPageLoaderPrivate;
class MultiPageLoader: public QObject {
	Q_OBJECT
public:
	MultiPageLoader(Settings & s);
	~MultiPageLoader();
	QWebPage * addResource(const QString & string);
	QWebPage * addResource(const QUrl & url);
	static QUrl guessUrlFromString(const QString &string);
	int httpErrorCode();
	static bool copyFile(QFile & src, QFile & dst);
public slots:
	void load();
	void clearResources();
	void cancel();
signals:
	void printRequested(QWebFrame * frame);
	void loadFinished(bool ok);
	void loadProgress(int progress);
	void loadStarted();
	void warning(QString text);
	void error(QString text);
private:
	MultiPageLoaderPrivate * d;
	friend class MultiPageLoaderPrivate;
	friend class MyQWebPage;
};

#endif //__MULTIPAGELOADER_HH__
