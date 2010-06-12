/*
 * File:   settings.cc
 * Author: Christian Sciberras
 * Created: 20 May 2010
 *   This file is part of wkhtmltoimage.
 *   wkhtmltoimage is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *   wkhtmltoimage is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *   You should have received a copy of the GNU General Public License
 *   along with wkhtmltoimage.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <settings.hh>

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
