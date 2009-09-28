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

#define S(x) ((x).toLocal8Bit().constData())

class ManOutputter: public Outputter {
private:
	FILE * fd;
public:
	ManOutputter(FILE * _): fd(_) {
		fprintf(fd,".TH WKHTMLTOPDF 1 \"2009 February 23\"\n\n");
	}

	void beginSection(const QString & name) {
		fprintf(fd, ".SH %s\n", S(name));
	}

	void endSection() {
		fprintf(fd, "\n");
	}

	void beginParagraph() {
	}

	void endParagraph() {
		fprintf(fd, "\n\n");
	}
	
	void text(const QString & t) {
		fprintf(fd, "%s", S(t));
	}
	
	void bold(const QString & t) {
		fprintf(fd, "\\fB%s\\fP", S(t));
	}
	
	void italic(const QString & t) {
		fprintf(fd, "\\fB%s\\fP", S(t));
	}

	void link(const QString & t) {
		fprintf(fd, "<%s>", S(t));
	}

	void verbatim(const QString & t) {
		fprintf(fd, "%s", S(t));
	}
	
	void beginSwitch() {
		fprintf(fd, ".PD 0\n");
	}
	
	void cswitch(const ArgHandler * h) {
		fprintf(fd, ".TP\n");
		fprintf(fd, "\\fB");
		if(h->shortSwitch != 0)
			fprintf(fd, "\\-%c, ", h->shortSwitch);
		else
			fprintf(fd, "    ");
		fprintf(fd,"\\-\\-%s\\fR", S(h->longName));
		
		for(QVector<QString>::const_iterator i = h->argn.constBegin(); i != h->argn.constEnd(); ++i)
			fprintf(fd," \\fI<%s>\\fR", S(*i));
		
		fprintf(fd, "\n%s\n",S(h->desc));
	}
	
	void endSwitch() {
		fprintf(fd, ".PD\n");
		fprintf(fd, "\n");
	}
};

/*!
  Create a man page outputter
  \param fd A file description to output to
*/
Outputter * Outputter::man(FILE * fd) {
  return new ManOutputter(fd);
}
