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


#ifdef _MSC_VER
#define strcasecmp _stricmp
#endif

#include "loadsettings.hh"
#include <QMap>
#include <stdexcept>
namespace wkhtmltopdf {
namespace settings {

QList<QString> LoadPage::mediaFilesExtensions = QList<QString> ()
		<< "css"
		<< "js"
		<< "png"
		<< "jpg"
		<< "jpeg"
		<< "gif";


LoadPage::LoadErrorHandling strToLoadErrorHandling(const char * s, bool * ok) {
	if (ok) *ok = true;
	if (!strcasecmp(s, "abort")) return LoadPage::abort;
	if (!strcasecmp(s, "skip")) return LoadPage::skip;
	if (!strcasecmp(s, "ignore")) return LoadPage::ignore;
	*ok = false;
	return LoadPage::abort;
}

QString loadErrorHandlingToStr(LoadPage::LoadErrorHandling leh) {
	switch (leh) {
	case LoadPage::abort: return "abort";
	case LoadPage::skip: return "skip";
	case LoadPage::ignore: return "ignore";
	}
	throw std::logic_error("Internal error in loadErrorHandlingToStr");
}

/*!
  Read proxy settings from a string, the grammar is described in the manual
  \param proxy the proxy string to parse
  \param ok If supplied indicates whether the proxy was valid
*/
Proxy strToProxy(const char * proxy, bool * ok) {
	Proxy p;
	if (ok) *ok=true;
	//Allow users to use no proxy, even if one is specified in the env
	if (!strcmp(proxy,"none")) {
		p.host = "";
		return p;
	}

	p.type = QNetworkProxy::HttpProxy;
	//Read proxy type bit "http://" or "socks5://"
	if (!strncmp(proxy,"http://",7)) {
		proxy += 7;
	} else if (!strncmp(proxy,"socks5://",9)) {
		p.type = QNetworkProxy::Socks5Proxy;
		proxy += 9;
	}

	//Read username and password
	char * val = (char *) strrchr(proxy,'@');
	p.user = p.password = "";
	if (val != NULL) {
		p.user = QString(proxy).left(val-proxy);
		proxy = val+1;

		int idx = p.user.indexOf(':');
		if (idx != -1) {
			p.password = p.user.mid(idx+1);
			p.user = p.user.left(idx);
		}
	}

 	//Read hostname and port
 	val = (char *) strchr(proxy,':');
 	p.port = 1080; //Default proxy port
 	if (val == NULL) p.host = proxy;
 	else {
		p.port = QString(val+1).toInt(ok);
		if (p.port < 0 || p.port > 65535) {
			p.port = 1080;
			*ok = false;
		}
		p.host = QString(proxy).left(val-proxy);
 	}
	if (ok && p.host.size() == 0) *ok = false;
	return p;
}

QString proxyToStr(const Proxy & p) {
	QString res="";
	if (p.type == QNetworkProxy::HttpProxy)
		res += "http://";
	else if (p.type == QNetworkProxy::Socks5Proxy)
		res += "socks5://";

	if (!p.user.isEmpty()) {
		res += "@" + p.user;
		if (!p.password.isEmpty()) res += ":" + p.password;
	}
	res += p.host;
	if (!p.host.isEmpty()) res += ":" + QString::number(p.port);
	return res;
}

Proxy::Proxy():
    type(QNetworkProxy::NoProxy),
	port(-1),
	host(),
	user(),
	password() {}

LoadGlobal::LoadGlobal():
	cookieJar("") {}

LoadPage::LoadPage():
	jsdelay(200),
	windowStatus(""),
	zoomFactor(1.0),
	repeatCustomHeaders(false),
	blockLocalFileAccess(false),
	stopSlowScripts(true),
	debugJavascript(false),
	loadErrorHandling(abort),
	mediaLoadErrorHandling(ignore),
	cacheDir(""),
	proxyHostNameLookup(false) {};

}
}
