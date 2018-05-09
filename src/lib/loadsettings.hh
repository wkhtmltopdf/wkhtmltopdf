// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010, 2012 wkhtmltopdf authors
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

#ifndef __LOADSETTINGS_HH__
#define __LOADSETTINGS_HH__

#include <QNetworkProxy>
#include <QString>

#include <dllbegin.inc>
namespace wkhtmltopdf {
namespace settings {

/*! \brief Settings considering proxy */
struct DLL_PUBLIC Proxy {
	Proxy();
	//! Type of proxy to use
	QNetworkProxy::ProxyType type;
	//! The port of the proxy to use
	int port;
	//! The host name of the proxy to use or NULL
	QString host;
	//! Username for the said proxy or NULL
	QString user;
	//! Password for the said proxy or NULL
	QString password;
};

struct DLL_PUBLIC PostItem {
	QString name;
	QString value;
	bool file;
};

struct DLL_PUBLIC LoadGlobal {
	LoadGlobal();
	//! Path of the cookie jar file
	QString cookieJar;
};

struct DLL_PUBLIC LoadPage {
	LoadPage();

	enum LoadErrorHandling {
		abort,
		skip,
		ignore
	};

	//! Username used for http auth login
	QString username;

	//! Password used for http auth login
	QString password;

	//! Path to the ssl client cert private key in OpenSSL PEM format
	QString clientSslKeyPath;

	//! Password to ssl client cert private key
	QString clientSslKeyPassword;

	//! Path to the ssl client cert public key in OpenSSL PEM format, optionally followed by intermediate ca and trusted certs
	QString clientSslCrtPath;

	//! How many milliseconds should we wait for a Javascript redirect
	int jsdelay;

	//! What window.status value should we wait for
	QString windowStatus;

	//! What zoom factor should we apply when printing
	// TODO MOVE
	float zoomFactor;

	//! Map of custom header variables
	QList< QPair<QString, QString> > customHeaders;

	//! Set if the custom header should be repeated for each resource request
	bool repeatCustomHeaders;

	//! Map of cookies
	QList< QPair<QString, QString> > cookies;

	QList< PostItem > post;

	//! Block access to local files for the given page
	bool blockLocalFileAccess;

	//! If access to local files is not allowed in general, allow it for these files
	QList< QString > allowed;

	//! Stop Javascript from running too long
	bool stopSlowScripts;

	//! Output Javascript debug messages
	bool debugJavascript;

	//! What should we do about load errors
	LoadErrorHandling loadErrorHandling;
	LoadErrorHandling mediaLoadErrorHandling;

	//! Proxy related settings
	Proxy proxy;

	//! Additional javascript to run on a page once it has loaded
	QList< QString > runScript;

	QString checkboxSvg;
	QString checkboxCheckedSvg;
	QString radiobuttonSvg;
	QString radiobuttonCheckedSvg;

	QString cacheDir;
	static QList<QString> mediaFilesExtensions;

	// Hosts to bypass
	QList< QString > bypassProxyForHosts;

	//! Whether to use the proxy for resolving hostnames
	bool proxyHostNameLookup;
};

DLL_PUBLIC LoadPage::LoadErrorHandling strToLoadErrorHandling(const char * s, bool * ok=0);
DLL_PUBLIC QString loadErrorHandlingToStr(LoadPage::LoadErrorHandling leh);

DLL_PUBLIC Proxy strToProxy(const char * s, bool * ok=0);
DLL_PUBLIC QString proxyToStr(const Proxy & proxy);

}
}
#include <dllend.inc>
#endif //__LOADSETTINGS_HH__
