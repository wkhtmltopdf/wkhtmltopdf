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
#include "Commandlineparser_p.hh"

class TextOutputter: public Outputter {
public:
	static const int lw = 80;
	int w=0;

	void beginSection(const QString * name) {
		if(doc) {
			int x= 80 - name.size() - 4;
			if(x < 6) x = 60;
			for(int i=0; i < x/2; ++i) 
				fprintf(fd, "=");
			fprintf(fd, "> %s <",name.toUtf8().constData());
			for(int i=0; i < (x+1)/2; ++i) 
				fprintf(fd, "=");
			fprintf(fd, "\n");
		} else 
			fprintf(fd, "%s:\n",name.toLocal8Bit().constData());
	}
	
	void endSection() {
		fprintf(fd, "\n");
	}

	void beginParagraph() {
		if(doc) {
			w=0;
		} else {
			w=2;
			fprintf("  ");
		}
	}

	void text(const QString & t) {
		
	}
	
	void bold(cost QString & t) {
		text("*"+t+"*");
	}
	
	void italic(const QString & t) {
		text("_"+t+"_");
	}
	
	void link(const QString & t) {
		text("<"+t+">");
	}

	void endParagraph() {
		fprintf(fd,"\n\n");
	}

	void verbatim(const QString & t) {
		fprintf(fd,"%s", doc?t.toUtf8().constData():t.toLocal8Bit().constData());
	}
	
// 	void beginList(bool ordered) {
// 		this->ordered = ordered;
// 		order = 1;
// 	}
	
// 	void endList() {
// 		printf("\n");
// 	}
	
// 	void listItem(QString & t) {
// 		if(ordered) {
// 			fprintf(fd, "%2d: ",order);
// 			++order;
// 		} else 
// 			fprintf(fd, " * ");
// 		fprintf(fd,"%s\N", doc?t.toUtf8().constData():t.toLocal8Bit().constData());
// 	}

	void beginSwitch() {}

	void cswitch(cost ArgHandler * h) {

	}

	void endSwitch() {
		printf("\n");
	}		
	
};

/*!
  Create a raw text outputter, used for outputting --help and readme
  \param fd A file description to output to
  \param doc Ouput in readme format
  \param extended Output extended options
*/
Outputter * Outputter::text(FILE * fd, bool doc, bool extended) {
  return TextOutputter(fd,doc);
}
