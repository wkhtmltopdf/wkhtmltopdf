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

#ifndef __COMMANDLINEPARSERBASE_HH__
#define __COMMANDLINEPARSERBASE_HH__
#include <loadsettings.hh>
#include <websettings.hh>
class Outputter;
class CommandLineParserBase;

class ArgHandler {
public:
	QString longName;
	QString desc;
	char shortSwitch;
	QVector<QString> argn;
	bool display;
	bool extended;
	bool qthack;
	virtual QString getDesc() const;
	virtual ~ArgHandler();
	int section;
	virtual bool operator() (const char ** args, CommandLineParserBase & parser, char * page) = 0;
};

class CommandLineParserBase {
public:
	int currentMode;
	QString currentSection;
	bool currentExtended;
	bool currentHack;

	QList<QString> sections;
	QHash<QString, ArgHandler *> longToHandler;
	QHash<char, ArgHandler *> shortToHandler;
	QHash<QString, QList<ArgHandler *> > sectionArgumentHandles;
	QHash<QString, QString> sectionDesc;

	//basearguments.cc
	void section(QString s, QString desc="");
	void mode(int m);
	void qthack(bool);
	void extended(bool);

	void addarg(QString, char, QString, ArgHandler * h, bool display=true);
	void addDocArgs();
	void addWebArgs(wkhtmltopdf::settings::Web & s);
	void addGlobalLoadArgs(wkhtmltopdf::settings::LoadGlobal & s);
	void addPageLoadArgs(wkhtmltopdf::settings::LoadPage & s);

	//commondocparts.cc
	void outputName(Outputter * o) const;
	void outputLicense(Outputter * o) const;
	void outputAuthors(Outputter * o) const;
	void outputStaticProblems(Outputter * o) const;
	void outputProxyDoc(Outputter * o) const;

	//commandlineparserbase.cc
	void outputSwitches(Outputter * o, bool extended, bool doc) const;
	virtual char * mapAddress(char * d, char *) const {return d;}
	virtual void license(FILE * fd) const;
	virtual void version(FILE * fd) const;
	void parseArg(int sections, const int argc, const char ** argv, bool & defaultMode, int & arg, char * page);

	virtual QString appName() const = 0;
	const char *appVersion() const;
	virtual void usage(FILE * fd, bool extended) const = 0;
	virtual void manpage(FILE * fd) const = 0;
	virtual void readme(FILE * fd, bool html) const = 0;
};
#endif //__COMMANDLINEPARSERBASE_HH__
