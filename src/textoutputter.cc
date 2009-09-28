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
#include <qstringlist.h>

#define S(t) (doc?(t).toUtf8().constData():(t).toLocal8Bit().constData())

class TextOutputter: public Outputter {
public:
	FILE * fd;
	static const int lw = 80;
	int w;
	bool doc;
	bool extended;
	bool first;
	TextOutputter(FILE * _, bool d, bool e): fd(_), doc(d), extended(e) {}

	void beginSection(const QString & name) {
		if(doc) {
			int x= 80 - name.size() - 4;
			if(x < 6) x = 60;
			for(int i=0; i < x/2; ++i) 
				fprintf(fd, "=");
			fprintf(fd, "> %s <", S(name) );
			for(int i=0; i < (x+1)/2; ++i) 
				fprintf(fd, "=");
			fprintf(fd, "\n");
		} else 
			fprintf(fd, "%s:\n", S(name) );
	}
	
	void endSection() {
	}

	void beginParagraph() {
		first=true;
		if(doc) {
			w=0;
		} else {
			w=2;
			fprintf(fd,"  ");
		}
	}

	void text(const QString & t) {
		first=true;
		QStringList list = t.split(" ");
		foreach(const QString & s, list) {
			if( w + s.size() + (first?0:1) > lw) {
				printf("\n");
				if(doc) {
					w=0;
				} else {
					w=2;
					fprintf(fd,"  ");
				}
				first=true;
			}
			if(first) first=false;
			else {
				printf(" ");
				++w;
			}
			w += s.size();
			printf("%s", S(s));
		}
	}
	
	void bold(const QString & t) {
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
		if(doc)
			fprintf(fd,"%s\n", S(t));
		else {
			foreach(const QString & s, t.split("\n"))
				fprintf(fd,"  %s\n",S(s));
		}
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

	void cswitch(const ArgHandler * h) {
		w=0;
		if(!doc) {fprintf(fd,"  "); w=2;}
		if(h->shortSwitch != 0)
			fprintf(fd,"-%c, ",h->shortSwitch);
		else
			fprintf(fd,"    ");
		fprintf(fd,"--%s",S(h->longName));
		w+=4 + 2 + h->longName.size();
		foreach(const QString & arg, h->argn) {
			fprintf(fd," <%s>",S(arg));
			w+=3+arg.size();
		}
		while(w < 34) {
			fprintf(fd," ");
			++w;
		}
		foreach(const QString & s, h->getDesc().split(" ")) {
			if(w+1+s.size() > lw) {
				printf("\n");
				w=0;
				while(w < 34) {
					fprintf(fd," ");
					++w;
				}
			}
			fprintf(fd, " %s", S(s));
			w += s.size() + 1;
		}
		fprintf(fd,"\n");
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
	return new TextOutputter(fd, doc, extended);
}
