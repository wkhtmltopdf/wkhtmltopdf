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

#ifndef __WEBSETTINGS_HH__
#define __WEBSETTINGS_HH__

#include <QNetworkProxy>
#include <QString>

#include <dllbegin.inc>
namespace wkhtmltopdf {
namespace settings {

struct DLL_PUBLIC Web {
	Web();

	//! Should we print background images
	bool background;

	//! Should we load images
	bool loadImages;

	//! Should we enable Javascript
	bool enableJavascript;

	//! Should the horrible intelligent shrinking feature be enabled?
	bool enableIntelligentShrinking;

	//! Minimum font size
	int minimumFontSize;

	//! Should we use the print or the screen media type
	bool printMediaType;

	//! Encoding used to enterpit a document with do supplied encoding
	QString defaultEncoding;

	//! Stylesheet supplied by the user
	QString userStyleSheet;

	//! Should plugins be allowed
	bool enablePlugins;
};

}
}
#include <dllend.inc>
#endif //__WEBSETTINGS_HH__
