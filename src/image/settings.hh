/*
 * File:   convert.cc
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

#ifndef __SETTINGS_HH__
#define __SETTINGS_HH__
#include <QString>
#include <qnetworkproxy.h>
#include <QPrinter>

/*! \brief Class holding all user settings.

    This class holds all the user settings, settings can be filled in by hand,
    or with other methods. 
    \sa CommandLineParser::parse()
*/
struct Settings {
	/*! \brief Settings for proxy */
	struct ProxySettings {
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

	// @dep: what is this for?
	struct PostItem {
		QString name;
		QString value;
		bool file;
	};

	/*! \brief Settings for cropping image */
	struct CropSettings {
		//! Cropping left/x coord
		int left;
		//! Cropping top/y coord
		int top;
		//! Cropping width/w dime
		int width;
		//! Cropping height/h dime
		int height;
	};

	/*! \brief Settings for scaling image */
	struct ScaleSettings {
		//! Scale width/w dime
		int width;
		//! Scale height/h dime
		int height;
	};
	
	//! Proxy related settings
	ProxySettings proxy;
	//! Crop related settings
	CropSettings crop;
	//! Scale related settings
	ScaleSettings scale;

	//! Username used for http auth login
	QString username;
	//! Password used for http auth login
	QString password;
	//! Be less verbose
	bool quiet; 
	//! Should we print background images
	bool background;
	//! Should plugins be allowed
	bool enablePlugins;
	//! Should we use the graphics system
	bool useGraphics;
	//! Should we enable Java Script
	bool enableJavascript; 
	//! Should the horrible intelligent shrking feature be enabled?
	bool enableIntelligentShrinking;
	//! How many milliseconds should we wait for a javascrit redirect
	int jsredirectwait;
	//! Color or grayscale
	QPrinter::ColorMode colorMode;
	//! Minimum font size
	int minimumFontSize;
	//! Encoding used to enterpit a document which doesn't supply encoding
	QString defaultEncoding;
	//! Stylesheet supplied by the user
	QString userStyleSheet;
	//! The file/url for input
	QString in;
	//! The file for output
	QString out;
	//! The output format
	QString fmt;
	//! Map of custum header valiabels
	QList< QPair<QString, QString> > customHeaders;
	//! Map of cookies
	QList< QPair<QString, QString> > cookies;
	//! Replacements
	QList< QPair<QString, QString> > replacements;
	//! Path of the cookie jar file
	QString cookieJar;
	
	QList< PostItem > post;
	bool stopSlowScripts;
	bool debugJavascript;
	bool blockLocalFileAccess;
	QList< QString > allowed;
	bool produceForms;

	static ProxySettings strToProxy(const char * s, bool * ok=0);
};

#endif //__SETTINGS_HH__
