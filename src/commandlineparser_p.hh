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
#ifndef __COMMMAND_LINE_PARSER_P_HH__
#define __COMMMAND_LINE_PARSER_P_HH__

#include <cstdio>
#include <QString>
#include "commandlineparser.hh"
#include "settings.hh"

class ArgHandler {
public:
	QString longName;
	QString desc;
	char shortSwitch;
	QVector<QString> argn;
	bool display;
	virtual bool operator() (const char ** args, Settings & settings) = 0;
	virtual void useDefault(Settings & settings);
	virtual QString getDesc() const;
	virtual ~ArgHandler();
};
 

class Outputter {
public:
	virtual void beginSection(const QString & name) = 0;
	virtual void endSection() = 0;
	virtual void beginParagraph() = 0;
	virtual void text(const QString & t) = 0;
	virtual void bold(const QString & t) =  0;
	virtual void italic(const QString & t) = 0;
	virtual void link(const QString & l) = 0;
	virtual void endParagraph() = 0;
	virtual void verbatim(const QString & t) = 0;
// 	virtual void beginList(bool ordered=false) = 0;
// 	virtual void endList() = 0;
// 	virtual void listItem(QString & t) = 0;
	virtual void beginSwitch() = 0;
	virtual void cswitch(const ArgHandler * h) = 0;
	virtual void endSwitch() = 0;
	static Outputter * text(FILE * fd, bool doc=false, bool extended=false);
	static Outputter * man(FILE * fd);
	static Outputter * html(FILE * fd);
};

class CommandLineParserPrivate {
public:

	void section(QString s);
	void qthack(bool);
	void extended(bool);
	void addarg(QString, char, QString, ArgHandler * h, bool display=true);

	CommandLineParserPrivate(Settings & s);
	void outputName(Outputter * o) const;
	void outputLicense(Outputter * o) const;
	void outputAuthors(Outputter * o) const;
	void outputSynopsis(Outputter * o) const;
	void outputDescripton(Outputter * o) const;
	void outputNotPatched(Outputter * o, bool sure) const;
	void outputPageBreakDoc(Outputter * o) const;
	void outputContact(Outputter * o) const;
};

#endif //__COMMMAND_LINE_PARSER_P_HH__
