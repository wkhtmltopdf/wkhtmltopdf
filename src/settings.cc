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
#include <settings.hh>

/*!
  \file settings.hh
  \brief Defines the Settings class
*/

/*!
  Convert a string to a paper size, basically all thinkable values are allowed.
  if a unknown value is given A4 is returned
  \param s The string to convert
  \param ok If supplied indicates if the conversion was successful
*/
QPrinter::PageSize Settings::strToPageSize(const char * s, bool * ok) {
	if (ok) *ok=true;
	if (!strcasecmp("A0", s)) return QPrinter::A0;
	if (!strcasecmp("A1", s)) return QPrinter::A1;
	if (!strcasecmp("A2", s)) return QPrinter::A2;
	if (!strcasecmp("A3", s)) return QPrinter::A3;
	if (!strcasecmp("A4", s)) return QPrinter::A4;
	if (!strcasecmp("A5", s)) return QPrinter::A5;
	if (!strcasecmp("A6", s)) return QPrinter::A6;
	if (!strcasecmp("A7", s)) return QPrinter::A7;
	if (!strcasecmp("A8", s)) return QPrinter::A8;
	if (!strcasecmp("A9", s)) return QPrinter::A9;
	if (!strcasecmp("B0", s)) return QPrinter::B0;
	if (!strcasecmp("B1", s)) return QPrinter::B1;
	if (!strcasecmp("B10", s)) return QPrinter::B10;
	if (!strcasecmp("B2", s)) return QPrinter::B2;
	if (!strcasecmp("B3", s)) return QPrinter::B3;
	if (!strcasecmp("B4", s)) return QPrinter::B4;
	if (!strcasecmp("B5", s)) return QPrinter::B5;
	if (!strcasecmp("B6", s)) return QPrinter::B6;
	if (!strcasecmp("B7", s)) return QPrinter::B7;
	if (!strcasecmp("B8", s)) return QPrinter::B8;
	if (!strcasecmp("B9", s)) return QPrinter::B9;
	if (!strcasecmp("C5E", s)) return QPrinter::C5E;
	if (!strcasecmp("Comm10E", s)) return QPrinter::Comm10E;
	if (!strcasecmp("DLE", s)) return QPrinter::DLE;
	if (!strcasecmp("Executive", s)) return QPrinter::Executive;
	if (!strcasecmp("Folio", s)) return QPrinter::Folio;
	if (!strcasecmp("Ledger", s)) return QPrinter::Ledger;
	if (!strcasecmp("Legal", s)) return QPrinter::Legal;
	if (!strcasecmp("Letter", s)) return QPrinter::Letter;
	if (!strcasecmp("Tabloid", s)) return QPrinter::Tabloid;
	if (ok) *ok = false;
	return QPrinter::A4;
}

/*!
  Read orientation from a string, possible values are landscape and portrait (case insensitive)
  \param s The string containing the orientation
  \param ok If supplied indicates whether the s was valid
*/
QPrinter::Orientation Settings::strToOrientation(const char * s, bool * ok) {
	if (ok) *ok = true;
 	if (!strcasecmp(s,"Landscape")) return QPrinter::Landscape;
 	if (!strcasecmp(s,"Portrait")) return QPrinter::Portrait;
	if (ok) *ok = false;
	return QPrinter::Portrait;
}

/*!
  Parse a string describing a distance, into a real number and a unit.
  \param o Tho string describing the distance
  \param ok If supplied indicates whether the s was valid
*/
QPair<qreal, QPrinter::Unit> Settings::strToUnitReal(const char * o, bool * ok) {
	qreal s=1.0; //Since not all units are provided by qt, we use this variable to scale
	//Them into units that are.
	QPrinter::Unit u=QPrinter::Millimeter;
	//Skip the real number part
	int i=0;
	while ('0' <= o[i]  && o[i] <= '9') ++i;
	if (o[i] == '.' || o[i] == '.') ++i;
	while ('0' <= o[i]  && o[i] <= '9') ++i;
	//Try to match the unit used
	if (!strcasecmp(o+i,"") || !strcasecmp(o+i,"mm") || !strcasecmp(o+i,"millimeter")) {
		u=QPrinter::Millimeter;
	} else if (!strcasecmp(o+i,"cm") || !strcasecmp(o+i,"centimeter")) {
		u=QPrinter::Millimeter;
		s=10.0; //1cm=10mm
	} else if (!strcasecmp(o+i,"m") || !strcasecmp(o+i,"meter")) {
		u=QPrinter::Millimeter;
		s=1000.0; //1m=1000m
	} else if (!strcasecmp(o+i,"didot"))
		u=QPrinter::Didot; //Todo is there a short for didot??
	else if (!strcasecmp(o+i,"inch") || !strcasecmp(o+i,"in"))
		u=QPrinter::Inch;
	else if (!strcasecmp(o+i,"pica") || !strcasecmp(o+i,"pc"))
		u=QPrinter::Pica;
	else if (!strcasecmp(o+i,"cicero"))
		u=QPrinter::Cicero;
	else if (!strcasecmp(o+i,"pixel") || !strcasecmp(o+i,"px"))
		u=QPrinter::DevicePixel;
	else if (!strcasecmp(o+i,"point") || !strcasecmp(o+i,"pt"))
		u=QPrinter::Point;
	else {
		if(ok) ok=false;
		return qMakePair((qreal)QString(o).left(i).toDouble()*s, u);
	}
	return qMakePair((qreal)QString(o).left(i).toDouble(ok)*s, u);
}

/*!
  Read proxy settings from a string, the grammar is described in the manual
  \param proxy the proxy string to parse
  \param ok If supplied indicates whether the proxy was valid
*/
Settings::ProxySettings Settings::strToProxy(const char * proxy, bool * ok) {
	Settings::ProxySettings p;
	if(ok) *ok=true;
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
	char * val = (char *) strchr(proxy,'@');
	p.user = p.password = "";
	if (val != NULL) {
		p.user = QString(proxy).left(val-proxy);
		proxy = val+1;
		
		int idx = p.user.indexOf(':');
		if(idx != -1) {
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
		if(p.port < 0 || p.port > 65535) {
			p.port = 1080;
			*ok = false;
		}
		p.host = QString(proxy).left(val-proxy);
 	}
	if(ok && p.host.size() == 0) *ok = false;
	return p;
}
