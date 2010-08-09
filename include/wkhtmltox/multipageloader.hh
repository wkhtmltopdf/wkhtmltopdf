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
#ifdef __WKHTMLTOX_UNDEF_QT_DLL__
#ifdef QT_DLL
#undef QT_DLL
#endif
#endif

#include <QFile>
#include <QObject>
#include <QUrl>
#include <QWebPage>
#include <wkhtmltox/loadsettings.hh>

#include <wkhtmltox/dllbegin.inc>
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
	MultiPageLoader(settings::LoadGlobal & s);
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
#include <wkhtmltox/dllend.inc>
#endif //__MULTIPAGELOADER_HH__
