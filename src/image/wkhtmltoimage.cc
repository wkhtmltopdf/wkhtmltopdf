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
#include "settings.hh"
#include "utilities.hh"
#include "imageconverter.hh"
#include <QApplication>

int main(int argc, char** argv) {
	//This will store all our settings
	wkhtmltopdf::settings::Global settings;
	//Create a command line parser to parse commandline arguments
	CommandLineParser parser(settings);
	//Setup default values in settings
	//parser.loadDefaults();
	//Parse the arguments
	parser.parseArguments(argc, (const char**)argv);

	//Construct QApplication required for printing
	QApplication a( argc, argv );
	a.setStyle(new MyLooksStyle());

	//Create the actual page converter to convert the pages
	wkhtmltopdf::ImageConverter converter(settings);

	if (!converter.convert()) return EXIT_FAILURE;

	switch(converter.httpErrorCode()) {

		case 401:
			return 3;

		case 404:
			return 2;

		case 0:
			return EXIT_SUCCESS;

		default:
			return EXIT_FAILURE;
	}
}
