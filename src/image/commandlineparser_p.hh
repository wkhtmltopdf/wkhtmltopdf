/*
 * File:   commandlineparser_p.hh
 * Author: Christian Sciberras
 * Created: 20 May 2010
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

#ifndef __COMMMAND_LINE_PARSER_P_HH__
#define __COMMMAND_LINE_PARSER_P_HH__

#include <cstdio>
#include <QString>
#include "commandlineparser.hh"
#include "settings.hh"
#include "outputter.hh"

class CommandLineParserPrivate;

class ArgHandler: public ArgHandlerBase {
public:
	virtual bool operator() (const char ** args, CommandLineParserPrivate & parser) = 0;
	virtual void useDefault(CommandLineParserPrivate & parser);
};

class CommandLineParserPrivate {
public:
	QString currentSection;
	Settings & settings;
	bool currentExtended;
	bool currentHack;

	QList<QString> sections;
	QHash<QString, ArgHandler *> longToHandler;
	QHash<char, ArgHandler *> shortToHandler;
	QHash<QString, QList<ArgHandler *> > sectionArgumentHandles;
	QHash<QString, QString> sectionDesc;

	//arguments.cc
	CommandLineParserPrivate(Settings & s);
	void section(QString s, QString desc="");
	void extended(bool);
	void addarg(QString, char, QString, ArgHandler * h, bool display=true);

	//docparts.cc
	void outputManName(Outputter * o) const;
	void outputSynopsis(Outputter * o) const;
	void outputDescripton(Outputter * o) const;
	void outputArgsFromStdin(Outputter * o) const;
	void outputNotPatched(Outputter * o, bool sure) const;
	void outputPageBreakDoc(Outputter * o) const;
	void outputContact(Outputter * o) const;
	void outputDocStart(Outputter * o) const;
	void outputCompilation(Outputter * o) const;
	void outputInstallation(Outputter * o) const;
	void outputExamples(Outputter * o) const;
	//commandlineparser.cc
	void outputSwitches(Outputter * o, bool extended, bool doc) const;
	void version(FILE * fd) const;
	void usage(FILE * fd, bool extended) const;
	void manpage(FILE * fd) const;
	void readme(FILE * fd, bool html) const;
};

#endif //__COMMMAND_LINE_PARSER_P_HH__
