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
#include "commandlineparser_p.hh"
#include <qwebframe.h>

/*!
  \file commandlineparser.hh
  \brief Defines the CommandLineParser class
*/

/*!
  \file commandlineparser_p.hh
  \brief Defines the CommandLineParserPrivate, ArgHandler and Outputter class
*/

bool ahsort(const ArgHandler * a, const ArgHandler * b) {
	QRegExp e("^(no|enable|disable)-");
	QString x=a->longName;
	QString y=b->longName;
	x.remove(e);
	y.remove(e);
	if (x == y) {
		QRegExp e("^no-");
		x=a->longName;
		y=b->longName;
		x.replace(e,"zzzz");
		y.replace(e,"zzzz");
	}
	return x < y;
}

/*!
  Output description of switches to an outputter
  \param o The outputter to output to
  \param extended Should we also output extended arguments
  \param doc Indicate to the outputter that it is writing documentation
*/
void CommandLineParserPrivate::outputSwitches(Outputter * o, bool extended, bool doc) const {
	foreach(const QString & section, sections) {
		QList<const ArgHandler *> display;
		foreach(const ArgHandler * handler, sectionArgumentHandles[section]) {
#ifndef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
			if(!doc && handler->qthack) continue;
#else 
			Q_UNUSED(doc);
#endif
			if(!extended && handler->extended) continue;
			display.push_back(handler);
		}
		qSort(display.begin(), display.end(), ahsort);
		if(display.size() == 0) continue;
		o->beginSection(section);
		if(!sectionDesc[section].isEmpty()) {
			o->beginParagraph();
			o->text(sectionDesc[section]);
			o->endParagraph();
		}
		o->beginSwitch();
		foreach(const ArgHandler * handler, display)
			o->cswitch(handler);
		o->endSwitch();
		o->endSection();
 	}
}

/*!
  Output the man page to a given file
  \param fd The file to store the man page
*/
void CommandLineParserPrivate::manpage(FILE * fd) const {
	Outputter * o = Outputter::man(fd);
 	outputManName(o);
 	outputSynopsis(o);
 	outputDescripton(o);
	outputSwitches(o, true, false);
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
 	outputProxyDoc(o);
 	outputHeaderFooterDoc(o);
 	outputOutlineDoc(o);
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
void CommandLineParserPrivate::usage(FILE * fd, bool extended) const {
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
	}
 	outputContact(o);
	delete o;
}

/*!
  Output version information aka. --version
  \param fd The file to output to
*/
void CommandLineParserPrivate::version(FILE * fd) const {
 	Outputter * o = Outputter::text(fd,false);
  	outputName(o);
  	outputLicense(o);
  	outputAuthors(o);
	delete o;
}

/*!
  Output the readme/manual
  \param fd The file to output to
  \param html Do we want the html manaul, or the README
*/
void CommandLineParserPrivate::readme(FILE * fd, bool html) const {
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
 	outputPageBreakDoc(o);
	outputPageSizes(o);
	outputArgsFromStdin(o);
	outputStaticProblems(o);
	outputCompilation(o);
	outputInstallation(o);
	outputExampels(o);
	delete o;
}

/*!
  Output usage information aka. --help
  \param fd The file to output the information to
  \param extended Should we show extended arguments
*/
void CommandLineParser::usage(FILE * fd, bool extended) const {
	d->usage(fd, extended);
}

/*!
  Output version information aka. --version
  \param fd The file to output to
*/
void CommandLineParser::version(FILE * fd) const {
	d->version(fd);
}

/*!
  Output the man page to a given file
  \param fd The file to store the man page
*/
void CommandLineParser::manpage(FILE * fd) const {
	d->manpage(fd);
}

/*!
  Output the readme/manual
  \param fd The file to output to
  \param html Do we want the html manaul, or the README
*/
void CommandLineParser::readme(FILE * fd, bool html) const {
	d->readme(fd,html);
}

/*!
  Construct a commandline parser, storing its values in some settings
  \param s The settings to store the values in.
*/
CommandLineParser::CommandLineParser(Settings & s):
	d(new CommandLineParserPrivate(s))
{
}
	
CommandLineParser::~CommandLineParser() {
	delete d;
}

/*!
 * Load default arguments and put them in the settings structure
 */
void CommandLineParser::loadDefaults() {
	//d->settings.in.clear();
	//d->settings.proxy.host = "";
	//foreach(ArgHandler * h, d->longToHandler) 
	//	h->useDefault(*d);

	//Load configuration from enviornment
	//char * val;
	//const char * vars[] = {"proxy","all_proxy","http_proxy", NULL};
	//for(int i=0; vars[i]; ++i) {
	//	if ((val = getenv("proxy"))) {
	//		bool ok=false;
	//		Settings::ProxySettings p = Settings::strToProxy(val, &ok);
	//		if(ok) 
	//			d->settings.proxy = p;
	//	}
	//}
}


void CommandLineParserPrivate::parseArg(int sections, const int argc, const char ** argv, bool & defaultMode, int & arg, Settings::PageSettings & pageSettings) {
	if (argv[arg][1] == '-') { //We have a long style argument
		//After an -- apperas in the argument list all that follows is interpited as default arguments
		if (argv[arg][2] == '0') {
			defaultMode=true;
			return;
		}
		//Try to find a handler for this long switch
		QHash<QString, ArgHandler*>::iterator j = longToHandler.find(argv[arg]+2);
		if (j == longToHandler.end()) { //Ups that argument did not exist
			fprintf(stderr, "Unknown long argument %s\n\n", argv[arg]);
			usage(stderr, false);
			exit(1);
		}
		if (!(j.value()->section & sections)) {
			fprintf(stderr, "%s specified in incorrect location\n\n", argv[arg]);
			usage(stderr, false);
			exit(1);
		}
		//Check to see if there is enough arguments to the switch
		if (argc-arg < j.value()->argn.size()+1) {
			fprintf(stderr, "Not enough arguments parsed to %s\n\n", argv[arg]);
			usage(stderr, false);
			exit(1);
		}
		if (!(*(j.value()))(argv+arg+1, *this, pageSettings)) {
			fprintf(stderr, "Invalid argument(s) parsed to %s\n\n", argv[arg]);
			usage(stderr, false);
			exit(1);
		}
#ifndef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
		if (j.value()->qthack)
			fprintf(stderr, "The switch %s, is not support using unpatched qt, and will be ignored.", argv[i]);
#endif
		//Skip allredy handled switch arguments
		arg += j.value()->argn.size();
	} else {
		int c=arg;//Remember the current argument we are parsing
		for (int j=1; argv[c][j] != '\0'; ++j) {
			QHash<char, ArgHandler*>::iterator k = shortToHandler.find(argv[c][j]);
			//If the short argument is invalid print usage information and exit
			if (k == shortToHandler.end()) {
				fprintf(stderr, "Unknown switch -%c\n\n", argv[c][j]);
				usage(stderr, false);
				exit(1);
			}
			if (!(k.value()->section & sections)) {
				fprintf(stderr, "-%c specified in incorrect location\n\n", argv[c][j]);
				usage(stderr, false);
				exit(1);
			}
			//Check to see if there is enough arguments to the switch
			if (argc-arg < k.value()->argn.size()+1) {
				fprintf(stderr, "Not enough arguments parsed to -%c\n\n", argv[c][j]);
				usage(stderr, false);
				exit(1);
			}
			if (!(*(k.value()))(argv+arg+1, *this, pageSettings)) {
				fprintf(stderr, "Invalid argument(s) parsed to -%c\n\n", argv[c][j]);
				usage(stderr, false);
				exit(1);
			}
#ifndef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
 			if (k.value()->qthack)
 				fprintf(stderr, "The switch -%c, is not support using unpatched qt, and will be ignored.", argv[c][j]);
#endif
			//Skip allredy handled switch arguments
			arg += k.value()->argn.size();
		}
	}
}


bool CommandLineParser::readArgsFromStdin() const {
	return d->readArgsFromStdin;
}

/*!
 * Parse command line arguments, and set settings accordingly.
 * \param argc the number of command line arguments
 * \param argv a NULL terminated list with the arguments
 */
void CommandLineParser::parseArguments(int argc, const char ** argv, bool fromStdin) {
	bool defaultMode = false;
	int arg=1;
	
	Settings::PageSettings def;
	
	//Parse global options
	for(;arg < argc;++arg) {
		if (argv[arg][0] != '-' || argv[arg][1] == '\0' || defaultMode) break;
		d->parseArg(d->global | d->page, argc, argv, defaultMode, arg, def);
	}
	
	if (d->readArgsFromStdin && !fromStdin) return;

	//Parse page options
	for(;arg < argc-1;++arg) {
		d->settings.pages.push_back(def);
		
		Settings::PageSettings & ps = d->settings.pages.back();
		int sections = d->page;
		if (!strcmp(argv[arg],"cover")) {
			++arg;
			if(argc >= argc-1) {
				fprintf(stderr, "You need to specify a input file to cover\n\n");
				usage(stderr, false);
				exit(1);
			}

			ps.page = QString::fromLocal8Bit(argv[arg]);
			++arg;
			//Setup varius cover settings her
		} else if (!strcmp(argv[arg],"toc")) {
			sections = d->page | d->toc;
		} else {
			if (!strcmp(argv[arg],"page")) {
				++arg;
				if(argc >= argc-1) {
					fprintf(stderr, "You need to specify a input file to page\n\n");
					usage(stderr, false);
					exit(1);
				}
			}
			ps.page = QString::fromLocal8Bit(argv[arg]);
			++arg;
		}
		for(;arg < argc-1;++arg) {
			if (argv[arg][0] != '-' || argv[arg][1] == '\0' || defaultMode) break;
			d->parseArg(sections, argc, argv, defaultMode, arg, def);
		}
	}
	
	if (d->settings.pages.size() == 0 || argc < 2) {
		fprintf(stderr, "You need to specify atleast one input file, and exactly one output file\nUse - for stdin or stdout\n\n");
		d->usage(stderr, false);
		exit(1);
	}
	d->settings.out = argv[argc-1];
}


