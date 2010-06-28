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

#include "commandlineparser.hh"
#include "imageconverter.hh"
#include "progressfeedback.hh"
#include "settings.hh"
#include "utilities.hh"
#include <QApplication>
#include <QWebFrame>

int main(int argc, char** argv) {
	//This will store all our settings
	wkhtmltopdf::settings::Global settings;
	//Create a command line parser to parse commandline arguments
	CommandLineParser parser(settings);
	//Parse the arguments
	parser.parseArguments(argc, (const char**)argv);


	bool use_graphics=true;
#if defined(Q_WS_X11) || defined(Q_WS_MACX)
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	use_graphics=settings.useGraphics;
	if (!use_graphics) QApplication::setGraphicsSystem("raster");
#endif
#endif
	QApplication a(argc, argv, use_graphics);
	a.setStyle(new MyLooksStyle());

	//Create the actual page converter to convert the pages
	wkhtmltopdf::ImageConverter converter(settings);

	wkhtmltopdf::ProgressFeedback feedback(settings.quiet, converter);
	bool success = converter.convert();
	return handleError(success, converter.httpErrorCode());
}
