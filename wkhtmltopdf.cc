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
	//If the number of arguments is incorrect, print out usage information
	if(argc != 3) {
		fprintf(stderr,
				"Usage: wkhtmltopdf <input file> <output file>\n"
				"\t<input file>  URL or local path to html document\n"
				"\t<output file> Name of ps or pdf file to be produced\n");
		exit(1);
	}
	//Store the name of the output file
	out = argv[2];
	//Make a url of the input file
	QUrl url(argv[1]);
	//When loading is progressing we want loadProgress to be called
	connect(&v, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
	//Once the loading is done we want loadFinished to be called
	connect(&v, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
	//Tell the vebview to load for the newly created url
	v.load(url);
}

void WKHtmlToPdf::loadFinished(bool ok) {
	if(!ok) {
		//It went bad, return with 1
		printf("Failed loading page\n");
		app->exit(1);
		return;
	}
	//Print out that it went good
	printf("Outputting page       \r");
	fflush(stdout);
	//Construct a printer object used to print the html to pdf
	QPrinter p(QPrinter::HighResolution);
	//Tell the printer object to print the file <out>
	p.setOutputFileName(out);
	//Tell the printer object that we use A4 paper
	p.setPaperSize(QPrinter::A4);
	//Do the actual printing
	v.print(&p);
	//Inform the user that everything went well
	printf("Done                 \n");
	app->quit();
}

void WKHtmlToPdf::loadProgress(int progress) {
	//Print out the load status
	printf("Loading page: %d%%   \r",progress);
	fflush(stdout);
}

int main(int argc, char * argv[]) {
	QApplication a(argc,argv); //Construct application, required for printing
	app = &a;
	WKHtmlToPdf x; //Create convertion instance
	x.run(argc,argv); //Run convertion
	return a.exec(); //Wait for application to terminate
}
