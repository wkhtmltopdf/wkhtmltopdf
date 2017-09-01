// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010, 2011 wkhtmltopdf authors
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


#include "reflect.hh"

namespace wkhtmltopdf {
namespace settings {

bool ReflectSimple::set(const char * name, const QString & value) {
	bool ok=false;
	if (name[0]=='\0') set(value, &ok);
	return ok;
}

QString ReflectClass::get(const char * name) {
	int i=0;
	while (name[i] !=0 && name[i] != '.' && name[i] != '[') ++i;
	if (!elms.contains(QString::fromLocal8Bit(name, i))) return QString();
	return elms[QString::fromLocal8Bit(name,i)]->get(name + (name[i] == '.'?i+1:i));
}

bool ReflectClass::set(const char * name, const QString & value) {
	int i=0;
	while (name[i] !=0 && name[i] != '.' && name[i] != '[') ++i;
	if (!elms.contains(QString::fromLocal8Bit(name, i))) return false;
	return elms[QString::fromLocal8Bit(name,i)]->set(name + (name[i] == '.'?i+1:i), value);
}


ReflectClass::~ReflectClass() {
	for (QMap<QString, Reflect *>::iterator i=elms.begin(); i != elms.end(); ++i)
		delete i.value();
}

ReflectImpl<LoadGlobal>::ReflectImpl(LoadGlobal & c) {
	WKHTMLTOPDF_REFLECT(cookieJar);
}

ReflectImpl<LoadPage>::ReflectImpl(LoadPage & c) {
	WKHTMLTOPDF_REFLECT(username);
	WKHTMLTOPDF_REFLECT(password);
	WKHTMLTOPDF_REFLECT(clientSslKeyPath);
	WKHTMLTOPDF_REFLECT(clientSslKeyPassword);
	WKHTMLTOPDF_REFLECT(clientSslCrtPath);
	WKHTMLTOPDF_REFLECT(jsdelay);
	WKHTMLTOPDF_REFLECT(windowStatus);
	WKHTMLTOPDF_REFLECT(zoomFactor);
	WKHTMLTOPDF_REFLECT(customHeaders);
	WKHTMLTOPDF_REFLECT(repeatCustomHeaders);
	WKHTMLTOPDF_REFLECT(cookies);
	WKHTMLTOPDF_REFLECT(post);
	WKHTMLTOPDF_REFLECT(blockLocalFileAccess);
	WKHTMLTOPDF_REFLECT(allowed);
	WKHTMLTOPDF_REFLECT(stopSlowScripts);
	WKHTMLTOPDF_REFLECT(debugJavascript);
	WKHTMLTOPDF_REFLECT(loadErrorHandling);
	WKHTMLTOPDF_REFLECT(proxy);
	WKHTMLTOPDF_REFLECT(runScript);
	WKHTMLTOPDF_REFLECT(checkboxSvg);
	WKHTMLTOPDF_REFLECT(checkboxCheckedSvg);
	WKHTMLTOPDF_REFLECT(radiobuttonSvg);
	WKHTMLTOPDF_REFLECT(radiobuttonCheckedSvg);
	WKHTMLTOPDF_REFLECT(cacheDir);
	WKHTMLTOPDF_REFLECT(bypassProxyForHosts);
	WKHTMLTOPDF_REFLECT(proxyHostNameLookup);
}

ReflectImpl<Web>::ReflectImpl(Web & c) {
	WKHTMLTOPDF_REFLECT(background);
	WKHTMLTOPDF_REFLECT(loadImages);
	WKHTMLTOPDF_REFLECT(enableJavascript);
	WKHTMLTOPDF_REFLECT(enableIntelligentShrinking);
	WKHTMLTOPDF_REFLECT(minimumFontSize);
	WKHTMLTOPDF_REFLECT(printMediaType);
	WKHTMLTOPDF_REFLECT(defaultEncoding);
	WKHTMLTOPDF_REFLECT(userStyleSheet);
	WKHTMLTOPDF_REFLECT(enablePlugins);
}

}
}
