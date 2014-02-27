// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010, 2011 wkhtmltopdf authors
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

#include "imagecommandlineparser.hh"
#include "outputter.hh"
#include <qwebframe.h>

/*!
  \file commandlineparser.hh
  \brief Defines the ImageCommandLineParser class
*/

/*!
  Output the man page to a given file
  \param fd The file to store the man page
*/
void ImageCommandLineParser::manpage(FILE * fd) const {
	Outputter * o = Outputter::man(fd);
 	outputManName(o);
 	outputSynopsis(o);
 	outputDescripton(o);
	outputSwitches(o, true, false);
 	outputContact(o);
	outputAuthors(o);
	delete o;
}

/*!
  Output usage information aka. --help
  \param fd The file to output the information to
  \param extended Should we show extended arguments
*/
void ImageCommandLineParser::usage(FILE * fd, bool extended) const {
	Outputter * o = Outputter::text(fd,false);
	outputName(o);
	outputSynopsis(o);
 	outputDescripton(o);
	outputSwitches(o, extended, false);
	if (extended) {
		outputProxyDoc(o);
	}
 	outputContact(o);
	delete o;
}

/*!
  Output the readme/manual
  \param fd The file to output to
  \param html Do we want the html manaul, or the README
*/
void ImageCommandLineParser::readme(FILE * fd, bool html) const {
	Outputter * o = html?Outputter::html(fd):Outputter::text(fd, true);
	outputDocStart(o);
	outputContact(o);
	outputLicense(o);
	outputAuthors(o);
	outputSynopsis(o);
	outputSwitches(o, true, true);
 	outputProxyDoc(o);
	outputStaticProblems(o);
	outputCompilation(o);
	outputInstallation(o);
	outputExamples(o);
	delete o;
}


/*!
 * Load default arguments and put them in the settings structure
 */
// void ImageCommandLineParser::loadDefaults() {
// 	d->settings.in = "-";
// 	d->settings.proxy.host = "";
// 	foreach (ArgHandler * h, d->longToHandler)
// 		h->useDefault(*d);

// 	//Load configuration from environment
// 	char * val;
// 	const char * vars[] = {"proxy","all_proxy","http_proxy", NULL};
// 	for (int i=0; vars[i]; ++i) {
// 		if ((val = getenv("proxy"))) {
// 			bool ok=false;
// 			Settings::ProxySettings p = Settings::strToProxy(val, &ok);
// 			if (ok)
// 				d->settings.proxy = p;
// 		}
// 	}
// }

/*!
 * Parse command line arguments, and set settings accordingly.
 * \param argc the number of command line arguments
 * \param argv a NULL terminated list with the arguments
 */
void ImageCommandLineParser::parseArguments(int argc, const char ** argv, bool final) {
	settings.in="";
    settings.out="";
	bool defaultMode=false;
	for (int i=1; i < argc; ++i) {
        if (i==argc-2 && (argv[i][0] != '-' || argv[i][1] == '\0')) { // the arg before last (in)
            settings.in = QString::fromLocal8Bit(argv[i]);
        } else if (i==argc-1 && (argv[i][0] != '-' || argv[i][1] == '\0')) { // the last arg (out)
            settings.out = QString::fromLocal8Bit(argv[i]);
		} else {
			parseArg(global, argc, argv, defaultMode, i, 0);
		}
	}

	if (final || settings.in=="" || settings.out=="") {
        fprintf(stderr, "You need to specify at least one input file, and exactly one output file\nUse - for stdin or stdout\n\n");
        usage(stderr, false);
        exit(1);
    }
}
