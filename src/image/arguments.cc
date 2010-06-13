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
#include "commandlineparser.hh"
#include "arghandler.inl"
#include <qglobal.h>

CommandLineParser::CommandLineParser(wkhtmltopdf::settings::Global & s):
	settings(s)
{
	mode(global);
	section("General Options");
	addDocArgs();
	addWebArgs(s.web);

	extended(false);
	qthack(false);
	addarg("scale-w",0,"Set width for resizing", new IntSetter(s.scale.width,"int"));
	addarg("scale-h",0,"Set height for resizing", new IntSetter(s.scale.height,"int"));

	addarg("crop-x",0,"Set x coordinate for croping", new IntSetter(s.crop.left,"int"));
	addarg("crop-y",0,"Set y coordinate for croping", new IntSetter(s.crop.top,"int"));
	addarg("crop-w",0,"Set width for croping", new IntSetter(s.crop.width,"int"));
	addarg("crop-h",0,"Set height for croping", new IntSetter(s.crop.height,"int"));
	addarg("format",'f',"Output format", new QStrSetter(s.fmt, "format") );
	extended(true);
	qthack(true);

#ifdef Q_WS_X11
	addarg("use-xserver",0,"Use the X server (some plugins and other stuff might not work without X11)", new ConstSetter<bool>(s.useGraphics,true));
#endif
	addGlobalLoadArgs(s.loadGlobal);
	addPageLoadArgs(s.loadPage);
}
