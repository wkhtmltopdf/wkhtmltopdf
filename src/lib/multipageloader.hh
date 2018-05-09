// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010 wkhtmltopdf authors
//
// This file is part of wkhtmltopdf.
//
// wkhtmltopdf is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// wkhtmltopdf is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with wkhtmltopdf.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __MULTIPAGELOADER_HH__
#define __MULTIPAGELOADER_HH__

#include <QFile>
#include <QObject>
#include <QUrl>
#if QT_VERSION >= 0x050000
#include <QtWebKitWidgets>
#else
#include <QWebPage>
#endif
#include <loadsettings.hh>

#include <dllbegin.inc>
namespace wkhtmltopdf {

class DLL_LOCAL MyQWebPage;

class DLL_LOCAL LoaderObject {
public:
	QWebPage & page;
	bool skip;

	LoaderObject(QWebPage & page);
};

class DLL_LOCAL MultiPageLoaderPrivate;
class DLL_LOCAL MultiPageLoader: public QObject {
	Q_OBJECT
public:
	MultiPageLoader(settings::LoadGlobal & s, int dpi, bool mainLoader = false);
	~MultiPageLoader();
	LoaderObject * addResource(const QString & url, const settings::LoadPage & settings, const QString * data=NULL);
	LoaderObject * addResource(const QUrl & url, const settings::LoadPage & settings);
	static QUrl guessUrlFromString(const QString &string);
	int httpErrorCode();
	static bool copyFile(QFile & src, QFile & dst);
public slots:
	void load();
	void clearResources();
	void cancel();
signals:
	void loadFinished(bool ok);
	void loadProgress(int progress);
	void loadStarted();
	void warning(QString text);
	void error(QString text);
private:
	MultiPageLoaderPrivate * d;
	friend class MultiPageLoaderPrivate;
	friend class MyQWebPage;
	friend class ResourceObject;
};

}
#include <dllend.inc>
#endif //__MULTIPAGELOADER_HH__
