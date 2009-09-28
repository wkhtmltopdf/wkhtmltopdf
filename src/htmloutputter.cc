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

#define S(x) x.toUtf8().constData()

class HtmlOutputter: public Outputter {
private:
	FILE * fd;
public:
	HtmlOutputter(FILE * _): fd(_) {
		fprintf(fd, 
				"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
				"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\" dir=\"ltr\">\n"
				"<head>\n"
				"  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n"
				"  <title>wkhtmltopdf - Manual</title>\n"
				"</head><body>");
	}

	~HtmlOutputter() {
		fprintf(fd,"</body></html>");
	}

	void beginSection(const QString & name) {
		fprintf(fd, "<h1>%s</h1>\n", S(name));
	}

	void endSection() {
	}

	void beginParagraph() {
		fprintf(fd,"<p>");
	}

	void endParagraph() {
		fprintf(fd,"</p>\n");
	}

	void text(const QString & t) {
		fprintf(fd, "%s", S(t));
	}
	
	void bold(const QString & t) {
		fprintf(fd, "<b>%s</b>", S(t));
	}
	
	void italic(const QString & t) {
		fprintf(fd, "<i>%s</i>", S(t));
	}

	void link(const QString & t) {
		fprintf(fd, "<a href=\"%s\">%s</a>", S(t));
	}
	
	void verbatim(const QString & t) {
		fprintf(fd, "<pre>%s</pre>", S(t));
	}
	
	void beginSwitch() {}
	void cswitch(const ArgHandler * h) {}
	void endSwitch() {
	}

}

/*!
  Create a Html outputter
  \param fd A file description to output to
*/
Outputter * Outputter::html(FILE * fd) {
	return new HtmlOutputter(fd);
}
