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

#include <string.h>
#include <iostream>
#include "wkhtmltopdf.hh"
#include <qnetworkreply.h>
#include <map>
QApplication * app;

void WKHtmlToPdf::usage(FILE * fd) {
	fprintf(fd,
"Usage: wkhtmltopdf [OPTION]... [<input file> [<output file>]]\n"
"converts a html page to pdf, if no <output file> is specified\n"
"/dev/stdout is used, simular for <input file>.\n"
"\n"
"Options:\n"
"  -h, --help                      print this help.\n"
"  -q, --quiet                     be less verbose.\n"
"  -i, --input <url>               use url as input.\n"
"  -o, --output <url>              use url as output.\n"
"  -p, --proxy <proxy>             use a proxy.\n"
"  -O, --orientation <orientation> Set orientation to\n"
"                                  Landscape or Portrait\n"
"  -s, --pagesize <size>           Set pape size to: A4, Letter, ect.\n"
"  -g, --grayscale                 PDF will be generated in grayscale.\n"
"  -l, --lowquality                Generates lower quality pdf/ps. \n"
"                                  Usefull to shrink the result document space. \n"
"\n"
"Proxy:\n"
"  By default proxyinformation will be read from the environment\n"
"  variables: proxy, all_proxy and http_proxy, proxy options can\n"
"  also by specified with the -p switch\n"
"  <type> := \"http://\" | \"socks5://\"\n" 
"  <userinfo> := <username> (\":\" <password>)? \"@\"\n"
"  <proxy> := \"None\" | <type>? <userinfo>? <host> (\":\" <port>)?\n"
"\n"
"Mail bug reports and suggestions to <antialze@gmail.com>.\n"
		);
}

void WKHtmlToPdf::setOrientation(const char * o) {
	if(!strcasecmp(o,"Landscape"))
		orientation = QPrinter::Landscape;
	else if(!strcasecmp(o,"Portrait"))
		orientation = QPrinter::Portrait;
	else
		{usage(stderr);exit(1);}
}

void WKHtmlToPdf::setPageSize(const char * o) {
	if(false);
	else if(!strcasecmp("A0",o)) pageSize=QPrinter::A0;
	else if(!strcasecmp("A1",o)) pageSize=QPrinter::A1;
	else if(!strcasecmp("A2",o)) pageSize=QPrinter::A2;
	else if(!strcasecmp("A3",o)) pageSize=QPrinter::A3;
	else if(!strcasecmp("A4",o)) pageSize=QPrinter::A4;
	else if(!strcasecmp("A5",o)) pageSize=QPrinter::A5;
	else if(!strcasecmp("A6",o)) pageSize=QPrinter::A6;
	else if(!strcasecmp("A7",o)) pageSize=QPrinter::A7;
	else if(!strcasecmp("A8",o)) pageSize=QPrinter::A8;
	else if(!strcasecmp("A9",o)) pageSize=QPrinter::A9;
	else if(!strcasecmp("B0",o)) pageSize=QPrinter::B0;
	else if(!strcasecmp("B1",o)) pageSize=QPrinter::B1;
	else if(!strcasecmp("B10",o)) pageSize=QPrinter::B10;
	else if(!strcasecmp("B2",o)) pageSize=QPrinter::B2;
	else if(!strcasecmp("B3",o)) pageSize=QPrinter::B3;
	else if(!strcasecmp("B4",o)) pageSize=QPrinter::B4;
	else if(!strcasecmp("B5",o)) pageSize=QPrinter::B5;
	else if(!strcasecmp("B6",o)) pageSize=QPrinter::B6;
	else if(!strcasecmp("B7",o)) pageSize=QPrinter::B7;
	else if(!strcasecmp("B8",o)) pageSize=QPrinter::B8;
	else if(!strcasecmp("B9",o)) pageSize=QPrinter::B9;
	else if(!strcasecmp("C5E",o)) pageSize=QPrinter::C5E;
	else if(!strcasecmp("Comm10E",o)) pageSize=QPrinter::Comm10E;
	else if(!strcasecmp("DLE",o)) pageSize=QPrinter::DLE;
	else if(!strcasecmp("Executive",o)) pageSize=QPrinter::Executive;
	else if(!strcasecmp("Folio",o)) pageSize=QPrinter::Folio;
	else if(!strcasecmp("Ledger",o)) pageSize=QPrinter::Ledger;
	else if(!strcasecmp("Legal",o)) pageSize=QPrinter::Legal;
	else if(!strcasecmp("Letter",o)) pageSize=QPrinter::Letter;
	else if(!strcasecmp("Tabloid",o)) pageSize=QPrinter::Tabloid;
	else {usage(stderr);exit(1);}
}



void WKHtmlToPdf::setProxy(const char * proxy) {
	//Allow users to use no proxy, even if one is specified in the env
	if(!strcmp(proxy,"none")) {proxyHost = NULL;return;}

	//Read proxy type bit "http://" or "socks5://"
	proxyType = QNetworkProxy::HttpProxy;
	if(!strncmp(proxy,"http://",7)) 
		proxy += 7;
	else if(!strncmp(proxy,"socks5://",9)) {
		proxyType = QNetworkProxy::Socks5Proxy;
		proxy += 8;
	}	
	//Read username and password
	char * val = strchr(proxy,'@');
	proxyUser = proxyPassword = NULL;
	if(val != NULL) {
		char * leak = (char*)malloc(val-proxy+1);
		memcpy(leak,proxy,val-proxy);
		leak[val-proxy] = 0;
		proxy = val+1;
		proxyUser = leak;
		val = strchr(leak,':');
		if(val) { //There is a password
			val[0] = '\0';
			proxyPassword = val+1;
		}
	}
	//Read hostname and port
	val = strchr(proxy,':');
	proxyPort = 1080;
	if(val == NULL) proxyHost = proxy;
	else {
		char * leak = (char*)malloc(val-proxy+1);
		memcpy(leak,proxy,val-proxy);
		leak[val-proxy] = 0;
		proxyHost = leak;
		proxyPort = atoi(val);
	}
}

void WKHtmlToPdf::parseArgs(int argc, const char ** argv) {
	int x=0;
	char * val;
	//Load default configuration
	in = "/dev/stdin";
	out = "/dev/stdout";
	proxyHost = NULL;
	quiet = false;
	pageSize = QPrinter::A4;
	orientation = QPrinter::Portrait;
	colorMode = QPrinter::Color;
	resolution = QPrinter::HighResolution;
	//Load configuration from enviornment
	if((val = getenv("proxy"))) setProxy(val);
	if((val = getenv("all_proxy"))) setProxy(val);
	if((val = getenv("http_proxy"))) setProxy(val);
	
	//QNetworkProxy::NoProxy;
	for(int i=1; i < argc; ++i) {
		if(argv[i][0] != '-') {
			//Default arguments
			++x;
			if(x==1) in = argv[i];
			else if(x == 2)	out = argv[i];
			else {usage(stderr);exit(1);}
			continue;
		}
		if(argv[i][1] == '-') { //Long style arguments
			if(!strcmp(argv[i],"--help")) {
				usage(stdout); exit(0);
			} else if(!strcmp(argv[i],"--quiet")) {
				quiet = true;
			} else if(!strcmp(argv[i],"--input")) {
				if(i+1>= argc) {usage(stderr);exit(1);}
				in = argv[++i];
			} else if(!strcmp(argv[i],"--output")) {
				if(i+1>= argc) {usage(stderr);exit(1);}
				out = argv[++i];
			} else if(!strcmp(argv[i],"--proxy")) {
				if(i+1>= argc) {usage(stderr);exit(1);}
				setProxy(argv[++i]);
			} else if(!strcmp(argv[i],"--orientation")) {
				if(i+1>= argc) {usage(stderr);exit(1);}
				setOrientation(argv[++i]);
			} else if(!strcmp(argv[i],"--pagesize")) {
				if(i+1>= argc) {usage(stderr);exit(1);}
				setPageSize(argv[++i]);
			} else if(!strcmp(argv[i],"--grayscale")) {
				colorMode = QPrinter::GrayScale;
			} else if(!strcmp(argv[i],"--lowquality")) {
				resolution = QPrinter::ScreenResolution;
			} else {usage(stderr);exit(1);}
			continue;
		}
		//Short style arguments
		int c=i;
		for(int j=1; argv[c][j] != '\0'; ++j) {
			switch(argv[c][j]) {
			case 'h':
				usage(stdout); exit(0);
			case 'i':
				if(i+1>= argc) {usage(stderr);exit(1);}
				in = argv[++i];
				break;
			case 'q':
				quiet=true;
				break;
			case 'o':
				if(i+1>= argc) {usage(stderr);exit(1);}
				out = argv[++i];
				break;
			case 'p':
				if(i+1>= argc) {usage(stderr);exit(1);}
				setProxy(argv[++i]);
				break;
			case 's':
				if(i+1>= argc) {usage(stderr);exit(1);}
				setPageSize(argv[++i]);
				break;
			case 'g':
				colorMode = QPrinter::GrayScale;
				break;
			case 'l':
				resolution = QPrinter::ScreenResolution;
				break;
			case 'O':
				if(i+1>= argc) {usage(stderr);exit(1);}
				setOrientation(argv[++i]);
				break;
			default:
				usage(stderr);exit(1);}
		}
	}
}

void WKHtmlToPdf::run(int argc, const char ** argv) {
	//Parse the arguments
	parseArgs(argc,argv);
	//Make a url of the input file
	QUrl url(in);

	QNetworkAccessManager * am = new QNetworkAccessManager();

	//If we must use a proxy, create a host of objects
	if(proxyHost) {
		QNetworkProxy proxy;
		proxy.setHostName(proxyHost);
		proxy.setPort(proxyPort);
		if(proxyUser) proxy.setUser(proxyUser);
		if(proxyPassword) proxy.setPassword(proxyPassword);
		am->setProxy(proxy);

	}
	v.page()->setNetworkAccessManager(am);
	connect(am, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)),this,
            SLOT(sslErrors(QNetworkReply*, const QList<QSslError>&)));

	//When loading is progressing we want loadProgress to be called
	connect(&v, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
	//Once the loading is done we want loadFinished to be called
	connect(&v, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
	//Tell the vebview to load for the newly created url
	v.load(url);
}

void WKHtmlToPdf::sslErrors(QNetworkReply *reply, const QList<QSslError> &error) {
	reply->ignoreSslErrors();
}

void WKHtmlToPdf::loadFinished(bool ok) {
	if(!ok) {
		//It went bad, return with 1
		fprintf(stderr, "Failed loading page\n");
		app->exit(1);
		return;
	}
	//Print out that it went good
	if(!quiet) fprintf(stderr, "Outputting page       \r");
	fflush(stdout);
	//Construct a printer object used to print the html to pdf
	QPrinter p(resolution);
	//Tell the printer object to print the file <out>
	p.setOutputFormat(
		strcmp(out + (strlen(out)-3),".ps")==0?
		QPrinter::PostScriptFormat : QPrinter::PdfFormat
		);
	p.setOutputFileName(out);
	
	p.setPageSize(pageSize);
	p.setOrientation(orientation);
	p.setColorMode(colorMode);
	//Do the actual printing
	if(!p.isValid()) {
		fprintf(stderr,"Unable to write to output file\n");
	} else {
		v.print(&p);
		if(!quiet) printf("Done                 \n");
		//Inform the user that everything went well
	}
	app->quit();
}

void WKHtmlToPdf::loadProgress(int progress) {
	//Print out the load status
	if(!quiet) fprintf(stderr,"Loading page: %d%%   \r",progress);
	fflush(stdout);
}

int main(int argc, char * argv[]) {
	QApplication a(argc,argv); //Construct application, required for printing
	app = &a;
	WKHtmlToPdf x; //Create convertion instance
	x.run(argc,(const char **)argv); //Run convertion
	return a.exec(); //Wait for application to terminate
}
