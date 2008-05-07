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
	connect(&v, SIGNAL(loadFinished()), this, SLOT(loadFinished()));
}

void WKHtmlToPdf::loadFinished() {
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
