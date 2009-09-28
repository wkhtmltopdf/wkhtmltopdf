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

void CommandLineParserPrivate::outputSwitches(Outputter * o, bool extended, bool doc) const {
	foreach(const QString & section, sections) {
		QList<const ArgHandler *> display;
		foreach(const ArgHandler * handler, sectionArgumentHandles[section]) {
#ifndef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
			if(!doc && handler->qthack) continue;
#endif
			if(!extended && handler->extended) continue;
			display.push_back(handler);
		}
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

void CommandLineParserPrivate::manpage(FILE * fd) const {
	Outputter * o = Outputter::man(fd);
 	outputName(o);
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
 	outputPageBreakDoc(o);
 	outputContact(o);
 	outputAuthors(o);
	delete o;
}

void CommandLineParserPrivate::usage(FILE * fd, bool extended) const {
	Outputter * o = Outputter::text(fd,false);
	outputName(o);
	outputSynopsis(o);
 	outputDescripton(o);
	outputSwitches(o, extended, false);
#ifndef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__	
	outputNotPatched(o,true);
#endif
 	outputContact(o);
	delete o;
}


void CommandLineParserPrivate::version(FILE * fd) const {
 	Outputter * o = Outputter::text(fd,false);
  	outputName(o);
  	outputLicense(o);
  	outputAuthors(o);
	delete o;
}

void CommandLineParserPrivate::readme(FILE * fd, bool html) const {
	Outputter * o = html?Outputter::html(fd):Outputter::text(fd);
	outputDocStart(o);
	outputContact(o);
	outputNotPatched(o,true);
	outputLicense(o);
	outputAuthors(o);
	outputSynopsis(o);
	outputSwitches(o, true, true);
 	outputProxyDoc(o);
 	outputHeaderFooterDoc(o);
 	outputOutlineDoc(o);
 	outputPageBreakDoc(o);
	outputCompilation(o);
	outputInstallation(o);
	outputExampels(o);
	delete o;
}


void CommandLineParser::parse(int argc, const char ** argv) const {
}


void CommandLineParser::usage(FILE * fd, bool extended) const {
	d->usage(fd, extended);
}

void CommandLineParser::version(FILE * fd) const {
	d->version(fd);
}

void CommandLineParser::manpage(FILE * fd) const {
	d->manpage(fd);
}

void CommandLineParser::readme(FILE * fd, bool html) const {
	d->readme(fd,html);
}

CommandLineParser::CommandLineParser(Settings & s):
	d(new CommandLineParserPrivate(s))
{
}
	
CommandLineParser::~CommandLineParser() {
	delete d;
}






// 	d->outputPageBreakdoc(o);
// 	d->outputContact(o);
// 	d->outputAuthors(o);
