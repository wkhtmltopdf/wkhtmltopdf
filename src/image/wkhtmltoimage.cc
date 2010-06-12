																													/*
 * File:   wkhtmltoimage.cc
 * Author: Christian Sciberras
 * Created: 18 May 2010
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

#include "commandlineparser.hh"
#include "settings.hh"
#include "utilities.cc"
#include "pageloader.hh"
#include <QApplication>

int main(int argc, char** argv) {
	//This will store all our settings
	Settings settings;
	//Create a command line parser to parse commandline arguments
	CommandLineParser parser(settings);
	//Setup default values in settings
	parser.loadDefaults();
	//Parse the arguments
	parser.parseArguments(argc, (const char**)argv);

	//Construct QApplication required for printing
	QApplication a( argc, argv );
	a.setStyle(new MyLooksStyle(settings));

	//Create the actual page converter to convert the pages
	PageLoader converter(settings);

	if (!converter.convert()) return EXIT_FAILURE;

	switch(converter.httpErrorCode) {

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
