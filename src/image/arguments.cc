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

#include "arghandler.inl"
#include "commandlineparser.hh"
#include <qglobal.h>

CommandLineParser::CommandLineParser(wkhtmltopdf::settings::Global & s):
	settings(s) {
	mode(global);
	section("General Options");
	addDocArgs();
	addWebArgs(s.web);

	extended(false);
	qthack(false);
	addarg("width",0,"Set screen width (default is 1024)", new IntSetter(s.screenWidth,"int"));
	// addarg("scale-w",0,"Set width for resizing", new IntSetter(s.scale.width,"int"));
	// addarg("scale-h",0,"Set height for resizing", new IntSetter(s.scale.height,"int"));

	addarg("crop-x",0,"Set x coordinate for croping", new IntSetter(s.crop.left,"int"));
	addarg("crop-y",0,"Set y coordinate for croping", new IntSetter(s.crop.top,"int"));
	addarg("crop-w",0,"Set width for croping", new IntSetter(s.crop.width,"int"));
	addarg("crop-h",0,"Set height for croping", new IntSetter(s.crop.height,"int"));
	addarg("format",'f',"Output file format (default is jpg)", new QStrSetter(s.fmt, "format") );
	addarg("quality",0,"Output image quality (between 0 and 100)", new IntSetter(s.quality, "int") );

	extended(true);
	qthack(true);
	addarg("disable-smart-width", '0', "Use the specified width even if it is not large enough for the content", new ConstSetter<bool>(s.smartWidth, false));
	addarg("transparent",0,"Make the background transparent in pngs", new ConstSetter<bool>(s.transparent, true));
#ifdef Q_WS_X11
	addarg("use-xserver",0,"Use the X server (some plugins and other stuff might not work without X11)", new ConstSetter<bool>(s.useGraphics,true));
#endif
	addGlobalLoadArgs(s.loadGlobal);
	addPageLoadArgs(s.loadPage);
}
