// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010 wkhtmltopdf authors
//
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

#ifndef __SETTINGS_HH__
#define __SETTINGS_HH__
#include "loadsettings.hh"
#include "websettings.hh"
#include <QString>

namespace wkhtmltopdf {
namespace settings {


/*! \brief Settings for cropping image */
struct CropSettings {
	CropSettings();
	//! Cropping left/x coord
	int left;
	//! Cropping top/y coord
	int top;
	//! Cropping width/w dime
	int width;
	//! Cropping height/h dime
	int height;
};

// /*! \brief Settings for scaling image */
// struct ScaleSettings {
// 	ScaleSettings();
// 	//! Scale width/w dime
// 	int width;
// 	//! Scale height/h dime
// 	int height;
// };

/*! \brief Class holding all user settings.

    This class holds all the user settings, settings can be filled in by hand,
    or with other methods.
    \sa CommandLineParser::parse()
*/
struct Global {
	Global();

	//! Crop related settings
	CropSettings crop;
	//! Scale related settings
	// ScaleSettings scale;

	LoadGlobal loadGlobal;
	LoadPage loadPage;
	Web web;

	//! Be less verbose
	bool quiet;

	bool transparent;

	//! Should we use the graphics system
	bool useGraphics;

	QString in;
	//! The file for output
	QString out;
	//! The output format
	QString fmt;

	//! Set the screen width
	int screenWidth;

	//! Image Quality
	int quality;

	bool smartWidth;
};

}
}
#endif //__SETTINGS_HH__
