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

#include "wkhtmltopdf.hh"
QApplication * app;

void WKHtmlToPdf::run(int argc, char ** argv) {
	if(argc != 3) {
		fprintf(stderr,
				"Usage: wkhtmltopdf <input file> <output file>\n"
				"\t<input file>  URL or local path to html document\n"
				"\t<output file> Name of ps or pdf file to be produced\n");
		exit(1);
	}
	QUrl url(argv[1]);
	v.load(url);
	out = argv[2];
	connect(&v, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
	//connect(&v, SIGNAL(loadFinished()), this, SLOT(loadFinished_())); only used in qt 4.4 release candidates
	connect(&v, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
}
void WKHtmlToPdf::_loadFinished() {loadFinished(true);}

void WKHtmlToPdf::loadFinished(bool ok) {
	printf("Outputting page       \r");
	fflush(stdout);
	QPrinter p(QPrinter::HighResolution);
	p.setOutputFileName(out);
	p.setPaperSize(QPrinter::A4);
	v.print(&p);
	printf("Done                 \n");
	app->quit();
}

void WKHtmlToPdf::loadProgress(int progress) {
	printf("Loading page: %d%%   \r",progress);
	fflush(stdout);
}

int main(int argc, char * argv[]) {
	QApplication a(argc,argv);
	app = &a;
	WKHtmlToPdf x;
	x.run(argc,argv);
	return a.exec();
}
