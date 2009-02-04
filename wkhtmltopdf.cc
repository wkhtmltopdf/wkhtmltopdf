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

void WKHtmlToPdf::usage(FILE * fd) {
	fprintf(fd,
"Usage: wkhtmltopdf [OPTION]... [<input file> [<output file>]]\n"
"converts a html page to pdf, if no <output file> is specified\n"
"/dev/stdout is used, simular for <input file>.\n"
"\n"
"Options:\n"
"  -h, --help                      print this help.\n"
"      --version                    output version information and exit\n"
"  -q, --quiet                     be less verbose.\n"
"  -i, --input <url>               use url as input.\n"
"  -o, --output <url>              use url as output.\n"
"  -p, --proxy <proxy>             use a proxy.\n"
"  -O, --orientation <orientation> Set orientation to\n"
"                                  Landscape or Portrait\n"
#if QT_VERSION >= 0x040500
"  -b, --nobackground              Do not print background\n"
#endif
"  -s, --pagesize <size>           Set pape size to: A4, Letter, ect.\n"
"  -g, --grayscale                 PDF will be generated in grayscale.\n"
"  -l, --lowquality                Generates lower quality pdf/ps.\n"
"                                  Usefull to shrink the result document space.\n"
"  -d, --dpi <dpi>                 Set the dpi explicitly, be aware!\n"
"                                  There is currently a bug in QT, setting this to low\n"
"                                  will make the application CRASH!\n"
"\n"
"  -T, --top <top>                 set page top margin (in milimeters) (default 10)\n"
"  -R, --right <right>             set page right margin (in milimeters) (default 10)\n"
"  -B, --bottom <bottom>           set page bottom margin (in milimeters) (default 10)\n"
"  -L, --left <left>               set page left margin (in milimeters) (default 10)\n"
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

void WKHtmlToPdf::version(FILE * fd) {
  fprintf(fd,
"wkhtmltopdf %d.%d\n"
"Copyright (C) 2008 Jakob Truelsen,\n"
"License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
"This is free software: you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law.\n"
"\n"
"Written by Jakob Truelsen\n"
"Patches by Mário Silva\n\n",
	  MAJOR_VERSION, MINOR_VERSION);
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

std::pair<qreal, QPrinter::Unit> WKHtmlToPdf::parseUnitReal(const char * o) {
	qreal s=1.0;
	QPrinter::Unit u;
	int i=0; 
	while('0' <= o[i]  && o[i] <= '9') ++i;
	if(o[i] == '.') ++i;
	while('0' <= o[i]  && o[i] <= '9') ++i;
	
	if(!strcasecmp(o+i,"") || !strcasecmp(o+i,"mm") || !strcasecmp(o+i,"millimeter")) {
		u=QPrinter::Millimeter;
	} else if(!strcasecmp(o+i,"cm") || !strcasecmp(o+i,"centimeter")) {
		u=QPrinter::Millimeter;		
		s=10.0;
	} else if(!strcasecmp(o+i,"m") || !strcasecmp(o+i,"meter")) {
		u=QPrinter::Millimeter;		
		s=1000.0;
	} else if(!strcasecmp(o+i,"didot"))
		u=QPrinter::Didot;
	else if(!strcasecmp(o+i,"inch") || !strcasecmp(o+i,"in"))
		u=QPrinter::Inch;
	else if(!strcasecmp(o+i,"pica") || !strcasecmp(o+i,"pc"))
		u=QPrinter::Pica;
	else if(!strcasecmp(o+i,"cicero"))
		u=QPrinter::Cicero;	
	else if(!strcasecmp(o+i,"pixel") || !strcasecmp(o+i,"px"))
		u=QPrinter::DevicePixel;
	else if(!strcasecmp(o+i,"point") || !strcasecmp(o+i,"pt"))	
		u=QPrinter::Point;
	return std::pair<qreal, QPrinter::Unit>(s*atof(o), u);
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

int WKHtmlToPdf::parseLongArg(const char * arg, int morec, const char ** morev) {
	size_t used=0;
	if(!strcmp(arg,"help")) {
		usage(stdout); exit(0);
	} else if(!strcmp(arg,"version")) {
		version(stdout); exit(0);
	} else if(!strcmp(arg,"quiet")) {
		quiet = true;
	} else if(!strcmp(arg,"input")) {
		if(morec < 1) {usage(stderr);exit(1);}
		in = morev[++used];
	} else if(!strcmp(arg,"output")) {
		if(morec < 1) return -1;
		out = morev[++used];
	} else if(!strcmp(arg,"proxy")) {
		if(morec < 1) return -1;
		setProxy(morev[++used]);
	} else if(!strcmp(arg,"orientation")) {
		if(morec < 1) return -1;
		setOrientation(morev[++used]);
	} else if(!strcmp(arg,"pagesize")) {
		if(morec < 1) return -1;
		setPageSize(morev[++used]);
	} else if(!strcmp(arg,"grayscale")) {
		colorMode = QPrinter::GrayScale;
	} else if(!strcmp(arg,"lowquality")) {
		resolution = QPrinter::ScreenResolution;
	} else if(!strcmp(arg,"nobackground")) {
		background = false;
	} else if(!strcmp(arg,"dpi")) {
		if(morec < 1) return -1;
		dpi=atoi(morev[++used]);
	} else if(!strcmp(arg,"top")) {
		if(morec < 1) return -1;
		top=parseUnitReal(morev[++used]);
	} else if(!strcmp(arg,"right")) {
		if(morec < 1) return -1;
		right=parseUnitReal(morev[++used]);
	} else if(!strcmp(arg,"bottom")) {
		if(morec < 1) return -1;
		bottom=parseUnitReal(morev[++used]);
	} else if(!strcmp(arg,"left")) {
		if(morec < 1) return -1;
		left=parseUnitReal(morev[++used]);
	} else return -1;
	return used;
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
	background = true;
	dpi = -1;
	top = right = bottom = left=
		std::pair<qreal,QPrinter::Unit>(10,QPrinter::Millimeter);
	
	//Load configuration from enviornment
	if((val = getenv("proxy"))) setProxy(val);
	if((val = getenv("all_proxy"))) setProxy(val);
	if((val = getenv("http_proxy"))) setProxy(val);

	std::map<const char, const char *> s2l;
	s2l['h'] = "help";      s2l['i'] = "input";      s2l['q']="quiet";
	s2l['o'] = "output";    s2l['p'] = "proxy";      s2l['s']="pagesize";
	s2l['g'] = "grayscale"; s2l['l'] = "lowquality"; s2l['O']="orientation";
	s2l['d'] = "dpi";       s2l['T'] = "top";        s2l['B']="bottom";
	s2l['R'] = "right";     s2l['L'] = "left";       s2l['b']="nobackground";
	
	//QNetworkProxy::NoProxy;
	for(int i=1; i < argc; ++i) {
		if(argv[i][0] != '-') {
			//Default arguments
			++x;
			if(x==1) in = argv[i];
			else if(x == 2)	out = argv[i];
			else {usage(stderr); exit(1);}
			continue;
		}
		if(argv[i][1] == '-') {
			int used = parseLongArg(argv[i]+2, argc - i, argv+i);
			if(used == -1) {usage(stderr); exit(1);}
			i += used;
		} else {
			int c=i;
			for(int j=1; argv[c][j] != '\0'; ++j) {
				if(s2l.count(argv[c][j]) == 0) {usage(stderr); exit(1);}
				int used = parseLongArg(s2l[argv[c][j]], argc - i, argv+i);
				if(used == -1) {usage(stderr); exit(1);}
				i += used;
			}
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
#if QT_VERSION >= 0x040500
	v.settings()->setAttribute(QWebSettings::PrintElementBackgrounds, background);
#endif 
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
		exit(1);
		return;
	}
	//Print out that it went good
	if(!quiet) fprintf(stderr, "Outputting page       \r");
	fflush(stdout);
	//Construct a printer object used to print the html to pdf
	QPrinter p(resolution);
	if(dpi != -1) p.setResolution(dpi);

	//Tell the printer object to print the file <out>
	p.setOutputFormat(
		strcmp(out + (strlen(out)-3),".ps")==0?
		QPrinter::PostScriptFormat : QPrinter::PdfFormat
		);
	p.setOutputFileName(out);
	if(left.second != right.second ||
	   left.second != top.second ||	  
	   left.second != bottom.second) {
		fprintf(stderr, "Currently all margin units must be the same!\n");
		exit(1);
	}
	   
	p.setPageMargins(left.first, top.first, right.first, bottom.first, left.second);
	
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
	qApp->quit();
}

void WKHtmlToPdf::loadProgress(int progress) {
	//Print out the load status
	if(!quiet) fprintf(stderr,"Loading page: %d%%   \r",progress);
	fflush(stdout);
}

int main(int argc, char * argv[]) {
	QApplication a(argc,argv); //Construct application, required for printing
	WKHtmlToPdf x; //Create convertion instance
	x.run(argc,(const char **)argv); //Run convertion
	return a.exec(); //Wait for application to terminate
}
