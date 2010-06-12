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
	extended(false);
	addDocArgs();
	
	extended(false);
	qthack(false);
	addarg("disable-javascript",'n',"Do not allow web pages to run javascript", new ConstSetter<bool>(s.enableJavascript,false));
	addarg("grayscale",'g',"Image will be generated in grayscale", new ConstSetter<QPrinter::ColorMode>(s.colorMode,QPrinter::GrayScale));
	addarg("enable-plugins",0,"Enable installed plugins (such as flash)", new ConstSetter<bool>(s.enablePlugins,true));

	addarg("scale-w",0,"Set width for resizing", new IntSetter(s.scale.width,"int"));
	addarg("scale-h",0,"Set height for resizing", new IntSetter(s.scale.height,"int"));

	addarg("crop-x",0,"Set x coordinate for croping", new IntSetter(s.crop.left,"int"));
	addarg("crop-y",0,"Set y coordinate for croping", new IntSetter(s.crop.top,"int"));
	addarg("crop-w",0,"Set width for croping", new IntSetter(s.crop.width,"int"));
	addarg("crop-h",0,"Set height for croping", new IntSetter(s.crop.height,"int"));

	addarg("minimum-font-size",0,"Minimum font size", new IntSetter(s.minimumFontSize,"int"));
	
	addarg("disable-smart-shrinking", 0, "Disable the intelligent shrinking strategy used by WebKit that makes the pixel/dpi ratio none constant",new ConstSetter<bool>(s.enableIntelligentShrinking, false));
#ifdef Q_WS_X11
	addarg("use-xserver",0,"Use the X server (some plugins and other stuff might not work without X11)", new ConstSetter<bool>(s.useGraphics,true));
#endif

	//addarg("encoding",0,"Set the default text encoding, for input", new QStrSetter(s.defaultEncoding,"encoding",""));
	
	addarg("no-background",0,"Do not print background", new ConstSetter<bool>(s.background,false));
	addarg("user-style-sheet",0,"Specify a user style sheet, to load with every page", new QStrSetter(s.userStyleSheet,"url"));

	addGlobalLoadArgs(s.loadGlobal);
	addPageLoadArgs(s.loadPage);
}
