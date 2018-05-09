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

#ifndef __PDFCOMMANDLINEPARSER_HH__
#define __PDFCOMMANDLINEPARSER_HH__
#include "commandlineparserbase.hh"
#include <cstdio>
#include <pdfsettings.hh>

class PdfCommandLineParser: public CommandLineParserBase {
public:
	const static int global = 1;
	const static int page = 2;
	const static int toc = 4;
	bool readArgsFromStdin;
	wkhtmltopdf::settings::PdfGlobal & globalSettings;
	QList<wkhtmltopdf::settings::PdfObject> & pageSettings;

	wkhtmltopdf::settings::PdfObject od;

	//Arguments.cc
	PdfCommandLineParser(wkhtmltopdf::settings::PdfGlobal & globalSettings,
					  QList<wkhtmltopdf::settings::PdfObject> & pageSettings);

	//docparts.cc
	void outputManName(Outputter * o) const;
	void outputSynopsis(Outputter * o) const;
	void outputDescripton(Outputter * o) const;
	void outputPageSizes(Outputter * o) const;
	void outputArgsFromStdin(Outputter * o) const;
	void outputHeaderFooterDoc(Outputter * o) const;
	void outputTableOfContentDoc(Outputter * o) const;
	void outputOutlineDoc(Outputter * o) const;
	void outputNotPatched(Outputter * o, bool sure) const;
	void outputPageBreakDoc(Outputter * o) const;
	void outputContact(Outputter * o) const;
	void outputDocStart(Outputter * o) const;
	void outputInstallation(Outputter * o) const;
	void outputExamples(Outputter * o) const;

	//commandlineparser.cc
	virtual QString appName() const {return "wkhtmltopdf";}
	virtual void usage(FILE * fd, bool extended) const;
	virtual void manpage(FILE * fd) const;
	virtual void readme(FILE * fd, bool html) const;

	void parseArguments(int argc, const char ** argv, bool fromStdin=false);

	virtual char * mapAddress(char * d, char * ns) const {
		const char * _od = reinterpret_cast<const char *>(&od);
		if (_od > d || d >= _od + sizeof(wkhtmltopdf::settings::PdfObject)) return d;;
		return d - _od + ns;
	}

};
#endif //__PDFCOMMANDLINEPARSER_HH__
