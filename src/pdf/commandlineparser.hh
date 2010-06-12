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
#ifndef __COMMMAND_LINE_PARSER_HH__
#define __COMMMAND_LINE_PARSER_HH__
#include "settings.hh"
#include "commandlineparserbase.hh"
#include <cstdio>

class CommandLineParser: public CommandLineParserBase {
public:
	const static int global = 1;
	const static int page = 2;
	const static int toc = 4;
	bool readArgsFromStdin;
	wkhtmltopdf::settings::Global & globalSettings;
	QList<wkhtmltopdf::settings::Page> & pageSettings;

	wkhtmltopdf::settings::Page od;
	
	//Arguments.cc
	CommandLineParser(wkhtmltopdf::settings::Global & globalSettings, 
					  QList<wkhtmltopdf::settings::Page> & pageSettings);

	//docparts.cc
	void outputManName(Outputter * o) const;
	void outputSynopsis(Outputter * o) const;
	void outputDescripton(Outputter * o) const;
	void outputPageSizes(Outputter * o) const;
	void outputArgsFromStdin(Outputter * o) const;
	void outputHeaderFooterDoc(Outputter * o) const;
	void outputOutlineDoc(Outputter * o) const;
	void outputNotPatched(Outputter * o, bool sure) const;
	void outputPageBreakDoc(Outputter * o) const;
	void outputContact(Outputter * o) const;
	void outputDocStart(Outputter * o) const;
	void outputCompilation(Outputter * o) const;
	void outputInstallation(Outputter * o) const;
	void outputExampels(Outputter * o) const;
	
	//commandlineparser.cc
	virtual QString appName() const {return "wkhtmltopdf";}
	virtual void usage(FILE * fd, bool extended) const;
	virtual void manpage(FILE * fd) const;
	virtual void readme(FILE * fd, bool html) const;

	void parseArguments(int argc, const char ** argv, bool fromStdin=false);

	virtual char * mapAddress(char * d, char * ns) const {
		char * od = reinterpret_cast<char *>(&od);
		if(od > d || d >= od + sizeof(wkhtmltopdf::settings::Page)) return d;;
		return d - od + ns;
	}
 
};

#endif //__COMMMAND_LINE_PARSER_HH__
