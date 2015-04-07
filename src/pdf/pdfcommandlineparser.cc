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

#include "outputter.hh"
#include "pdfcommandlineparser.hh"
#include <qwebframe.h>

using namespace wkhtmltopdf::settings;
/*!
  \file commandlineparser.hh
  \brief Defines the PdfCommandLineParser class
*/

/*!
  \file commandlineparser_p.hh
  \brief Defines the PdfCommandLineParserPrivate, ArgHandler and Outputter class
*/

/*!
  Output the man page to a given file
  \param fd The file to store the man page
*/
void PdfCommandLineParser::manpage(FILE * fd) const {
	Outputter * o = Outputter::man(fd);
 	outputManName(o);
 	outputSynopsis(o);
 	outputDescripton(o);
	outputSwitches(o, true, false);
	outputProxyDoc(o);
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
 	outputHeaderFooterDoc(o);
 	outputOutlineDoc(o);
	outputTableOfContentDoc(o);
#else
	outputNotPatched(o,true);
#endif
	outputPageSizes(o);
	outputArgsFromStdin(o);
 	outputPageBreakDoc(o);
 	outputContact(o);
 	outputAuthors(o);
	delete o;
}

/*!
  Output usage information aka. --help
  \param fd The file to output the information to
  \param extended Should we show extended arguments
*/
void PdfCommandLineParser::usage(FILE * fd, bool extended) const {
	Outputter * o = Outputter::text(fd,false);
	outputName(o);
	outputSynopsis(o);
 	outputDescripton(o);
	outputSwitches(o, extended, false);
#ifndef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	outputNotPatched(o, true);
#endif
	if (extended) {
		outputPageSizes(o);
		outputArgsFromStdin(o);
		outputProxyDoc(o);
		outputHeaderFooterDoc(o);
		outputOutlineDoc(o);
		outputTableOfContentDoc(o);
	}
 	outputContact(o);
	delete o;
}

/*!
  Output the readme/manual
  \param fd The file to output to
  \param html Do we want the html manual, or the README
*/
void PdfCommandLineParser::readme(FILE * fd, bool html) const {
	Outputter * o = html?Outputter::html(fd):Outputter::text(fd, true);
	outputDocStart(o);
	outputContact(o);
	outputNotPatched(o, false);
	outputLicense(o);
	outputAuthors(o);
	outputSynopsis(o);
	outputSwitches(o, true, true);
 	outputProxyDoc(o);
 	outputHeaderFooterDoc(o);
 	outputOutlineDoc(o);
	outputTableOfContentDoc(o);
 	outputPageBreakDoc(o);
	outputPageSizes(o);
	outputArgsFromStdin(o);
	outputStaticProblems(o);
	outputInstallation(o);
	outputExamples(o);
	delete o;
}

/*!
 * Load default arguments and put them in the settings structure
 */
//void PdfCommandLineParser::loadDefaults() {
	//settings.in.clear();
	//settings.proxy.host = "";
	//foreach(ArgHandler * h, longToHandler)
	//	h->useDefault(*d);

	//Load configuration from environment
	//char * val;
	//const char * vars[] = {"proxy","all_proxy","http_proxy", NULL};
	//for(int i=0; vars[i]; ++i) {
	//	if ((val = getenv("proxy"))) {
	//		bool ok=false;
	//		Settings::ProxySettings p = Settings::strToProxy(val, &ok);
	//		if (ok)
	//			settings.proxy = p;
	//	}
	//}
//}


/*!
 * Parse command line arguments, and set settings accordingly.
 * \param argc the number of command line arguments
 * \param argv a NULL terminated list with the arguments
 */
void PdfCommandLineParser::parseArguments(int argc, const char ** argv, bool fromStdin) {
	bool defaultMode = false;
	int arg=1;

	PdfObject def;

	//Parse global options
	for (;arg < argc;++arg) {
		if (argv[arg][0] != '-' || argv[arg][1] == '\0' || defaultMode) break;
		parseArg(global | page, argc, argv, defaultMode, arg, (char *)&def);
	}

	if (readArgsFromStdin && !fromStdin) return;

	//Parse page options
	while (arg < argc-1) {
		pageSettings.push_back(def);
		PdfObject & ps = pageSettings.back();
		int sections = page;
		if (!strcmp(argv[arg],"cover")) {
			++arg;
			if (arg >= argc-1) {
				fprintf(stderr, "You need to specify a input file to cover\n\n");
				usage(stderr, false);
				exit(1);
			}
			ps.page = QString::fromLocal8Bit(argv[arg++]);
			// parse page options and then override the header/footer settings
			for (;arg < argc;++arg) {
				if (argv[arg][0] != '-' || argv[arg][1] == '\0' || defaultMode) break;
				parseArg(sections, argc, argv, defaultMode, arg, (char*)&ps);
			}

			ps.header.left = ps.header.right = ps.header.center = "";
			ps.footer.left = ps.footer.right = ps.footer.center = "";
			ps.header.line = ps.footer.line = false;
			ps.header.htmlUrl = ps.footer.htmlUrl = "";
			ps.includeInOutline = false;

			continue;
		} else if (!strcmp(argv[arg],"toc")) {
			++arg;
			sections = page | toc;
			ps.isTableOfContent = true;
		} else {
			if (!strcmp(argv[arg],"page")) {
				++arg;
				if (arg >= argc-1) {
					fprintf(stderr, "You need to specify a input file to page\n\n");
					usage(stderr, false);
					exit(1);
				}
			}
			QByteArray a(argv[arg]);
			ps.page = QString::fromLocal8Bit(a);
			++arg;
		}
		for (;arg < argc;++arg) {
			if (argv[arg][0] != '-' || argv[arg][1] == '\0' || defaultMode) break;
			parseArg(sections, argc, argv, defaultMode, arg, (char*)&ps);
		}
	}

	if (pageSettings.size() == 0 || argc < 2) {
		fprintf(stderr, "You need to specify at least one input file, and exactly one output file\nUse - for stdin or stdout\n\n");
		usage(stderr, false);
		exit(1);
	}
	globalSettings.out = QString::fromLocal8Bit(argv[argc-1]);
}
