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
#include <cstdio>

class CommandLineParserPrivate;

/*! \brief The class is responcible for parsing command line information
*/
class CommandLineParser {
public:
	CommandLineParser(wkhtmltopdf::settings::Global & globalSettings, 
					  QList<wkhtmltopdf::settings::Page> & pageSettings);
	~CommandLineParser();
	void version(FILE * fd) const;
	void usage(FILE * fd, bool extended) const;
	void manpage(FILE * fd) const;
	void readme(FILE * fd, bool html) const;
	void loadDefaults();
	void parseArguments(int argc, const char ** argv, bool fromStdin=false);
	bool readArgsFromStdin() const;
private:
	CommandLineParserPrivate * d;
};
#endif //__COMMMAND_LINE_PARSER_HH__
