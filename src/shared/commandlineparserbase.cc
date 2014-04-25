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

#include "commandlineparserbase.hh"
#include "outputter.hh"
#include <qwebframe.h>

bool ahsort(const ArgHandler * a, const ArgHandler * b) {
	QRegExp e("^(no|enable|disable|include-in|exclude-from)-");
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
void CommandLineParserBase::outputSwitches(Outputter * o, bool extended, bool doc) const {
	foreach (const QString & section, sections) {
		QList<const ArgHandler *> display;
		foreach (const ArgHandler * handler, sectionArgumentHandles[section]) {
#ifndef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
			if (!doc && handler->qthack) continue;
#else
			Q_UNUSED(doc);
#endif
			if (!extended && handler->extended) continue;
			display.push_back(handler);
		}
		qSort(display.begin(), display.end(), ahsort);
		if (display.size() == 0) continue;
		o->beginSection(section);
		if (!sectionDesc[section].isEmpty()) {
			o->beginParagraph();
			o->text(sectionDesc[section]);
			o->endParagraph();
		}
		o->beginSwitch();
		foreach (const ArgHandler * handler, display)
			o->cswitch(handler);
		o->endSwitch();
		o->endSection();
 	}
}

#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)

const char *CommandLineParserBase::appVersion() const {
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	return STRINGIZE(FULL_VERSION) " (with patched qt)";
#else
	return STRINGIZE(FULL_VERSION);
#endif
}

/*!
  Output version information aka. --version
  \param fd The file to output to
*/
void CommandLineParserBase::version(FILE * fd) const {
	fprintf(fd, "%s %s\n", appName().toLocal8Bit().constData(), appVersion());
}

/*!
  Output license information aka. --license
  \param fd The file to output to
*/
void CommandLineParserBase::license(FILE * fd) const {
 	Outputter * o = Outputter::text(fd,false);
  	outputName(o);
  	outputAuthors(o);
  	outputLicense(o);
	delete o;
}

void CommandLineParserBase::parseArg(int sections, const int argc, const char ** argv, bool & defaultMode, int & arg, char * page) {
	if (argv[arg][1] == '-') { //We have a long style argument
		//After an -- apperas in the argument list all that follows is interpreted as default arguments
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
		if (!(*(j.value()))(argv+arg+1, *this, page)) {
			fprintf(stderr, "Invalid argument(s) parsed to %s\n\n", argv[arg]);
			usage(stderr, false);
			exit(1);
		}
#ifndef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
		if (j.value()->qthack)
			fprintf(stderr, "The switch %s, is not support using unpatched qt, and will be ignored.", argv[arg]);
#endif
		//Skip already handled switch arguments
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
			if (!(*(k.value()))(argv+arg+1, *this, page)) {
				fprintf(stderr, "Invalid argument(s) parsed to -%c\n\n", argv[c][j]);
				usage(stderr, false);
				exit(1);
			}
#ifndef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
 			if (k.value()->qthack)
 				fprintf(stderr, "The switch -%c, is not support using unpatched qt, and will be ignored.", argv[c][j]);
#endif
			//Skip already handled switch arguments
			arg += k.value()->argn.size();
		}
	}
}
