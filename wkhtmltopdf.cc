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
#include <QtWebKit>
#include <QtPlugin>

#ifdef QT_STATIC
Q_IMPORT_PLUGIN(qjpeg)
Q_IMPORT_PLUGIN(qgif)
Q_IMPORT_PLUGIN(qtiff)
Q_IMPORT_PLUGIN(qmng)
#endif

/*!
 * Print out program usage information
 * \param fd the filedescriptor where the output will be printed
 */
void WKHtmlToPdf::usage(FILE * fd) {
	fprintf(fd,
"Usage: wkhtmltopdf [OPTION]... [<input file> [<output file>]]\n"
"converts a html page to pdf, if no <output file> is specified\n"
"/dev/stdout is used, simular for <input file>.\n"
"\n"
"Options:\n"
"  -h, --help                      print this help.\n"
"      --version                   output version information and exit\n"
"  -q, --quiet                     be less verbose.\n"
"  -i, --input <url>               use url as input.\n"
"  -o, --output <url>              use url as output.\n"
"  -p, --proxy <proxy>             use a proxy.\n"
"  -O, --orientation <orientation> Set orientation to\n"
"                                  Landscape or Portrait\n"
#if QT_VERSION >= 0x040500 //Not printing the background was added in QT4.5
"  -b, --nobackground              Do not print background\n"
#endif
"  -s, --pagesize <size>           Set pape size to: A4, Letter, ect.\n"
"  -g, --grayscale                 PDF will be generated in grayscale.\n"
"  -l, --lowquality                Generates lower quality pdf/ps.\n"
"                                  Usefull to shrink the result document space.\n"
"  -d, --dpi <dpi>                 Set the dpi explicitly.\n"
#if QT_VERSION < 0x040500 //This bug was fixed in QT4.5
"                                  Be aware! There is currently a bug in QT, setting this to low\n"
"                                  will make the application CRASH!\n"
#endif
"  -T, --top <top>                 page top margin (default 10mm)\n"
"  -R, --right <right>             page right margin (default 10mm)\n"
"  -B, --bottom <bottom>           page bottom margin (default 10mm)\n"
"  -L, --left <left>               page left margin (default 10mm)\n"
#ifdef	WKHTMLTOPDF_QT_WEBFRAME_PATCH
"      --header-font-size <size>   header font size (default 11)\n"
"      --header-font-name <name>   header font name (default Areal)\n"
"      --header-left <text>        left aligned header text\n"		  
"      --header-center <text>      center aligned header text\n"		  
"      --header-right <text>       right aligned header text\n"
"      --header-line               display line below the header\n"
"      --footer-font-size <size>   footer font size (default 11)\n"
"      --footer-font-name <name>   footer font name (default Areal)\n"
"      --footer-left <text>        left aligned footer text\n"		  
"      --footer-center <text>      center aligned footer text\n"		  
"      --footer-right <text>       right aligned footer text\n"
"      --footer-line               display line above the footer\n"
"  -H, --default-header            Add a default header, with the name of the page to the left,\n"
"                                  and the page number to the right, this is short for:\n"
"                                  \"--header-left='[webpage]' --header-right='[page]/[toPage]' --top 2cm --header-line\"\n"
#endif
"      --read-args-from-stdin      Uses each line from stdin, as commandline options\n"
"                                  for a convertion. Using multiple lines here, is much\n"
"                                  faster then calling wkhtmltopdf multiple times\n"
"      --redirect-delay <msec>     wait some miliseconds for js-redirects (default 500)\n" 		   
"\n"
"Proxy:\n"
"  By default proxyinformation will be read from the environment\n"
"  variables: proxy, all_proxy and http_proxy, proxy options can\n"
"  also by specified with the -p switch\n"
"  <type> := \"http://\" | \"socks5://\"\n" 
"  <userinfo> := <username> (\":\" <password>)? \"@\"\n"
"  <proxy> := \"None\" | <type>? <userinfo>? <host> (\":\" <port>)?\n"
"\n"
#ifdef	WKHTMLTOPDF_QT_WEBFRAME_PATCH
"Header and footer text:\n"
"In a header or footer text the following variables can be used\n"
" * [page]     Replaced by the number of the pages currently beeing printed\n"
" * [fromPage] Replaced by the number of the first page to be printed\n"
" * [toPage]   Replaced by the number of the last page to be printed\n"
" * [webpage]  Replaced by the url of the page beeing printed\n"
"\n"
#endif
"Mail bug reports and suggestions to <antialze@gmail.com>.\n"
		);
}

/*!
 * Print out vertion and license information
 * \param fd the filedescriptor where the output will be printed
 */
void WKHtmlToPdf::version(FILE * fd) {
	fprintf(fd,
"wkhtmltopdf %d.%d.%d\n"
"Copyright (C) 2008,2009 Jakob Truelsen,\n"
"License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
"This is free software: you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law.\n"
"\n"
"Written by Jakob Truelsen\n"
"Patches by Mário Silva\n\n",
			MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION);
}

/*!
 * Set the page orientation from a string, allowed values are
 * Landscape or Portrait (case insensitive), print usage
 * and exit if other options are added.
 * \param o the string describing the orientation
 */
void WKHtmlToPdf::setOrientation(const char * o) {
	if(!strcasecmp(o,"Landscape"))
		orientation = QPrinter::Landscape;
	else if(!strcasecmp(o,"Portrait"))
		orientation = QPrinter::Portrait;
	else
		{usage(stderr);exit(1);}
}

/*!
 * Set the page/paper size from a string, basicly all thinkable values
 * are allowed, in the odd case that a unallowd value is parsed,
 * print usage information and exit
 * \param o the string describing the paper size.
 */
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

/*!
 * Parse a string describing a distance, into a real number and a unit.
 * \param o Tho string describing the distance
 */
std::pair<qreal, QPrinter::Unit> WKHtmlToPdf::parseUnitReal(const char * o) {
	qreal s=1.0; //Since not all units are provided by qt, we use this variable to skale 
	//Them into units that are.
	QPrinter::Unit u=QPrinter::Millimeter;
	//Skip the real number part
	int i=0; 
	while('0' <= o[i]  && o[i] <= '9') ++i;
	if(o[i] == '.') ++i;
	while('0' <= o[i]  && o[i] <= '9') ++i;
	
	//Try to match the unit used
	if(!strcasecmp(o+i,"") || !strcasecmp(o+i,"mm") || !strcasecmp(o+i,"millimeter")) {
		u=QPrinter::Millimeter;
	} else if(!strcasecmp(o+i,"cm") || !strcasecmp(o+i,"centimeter")) {
		u=QPrinter::Millimeter;		
		s=10.0; //1cm=10mm
	} else if(!strcasecmp(o+i,"m") || !strcasecmp(o+i,"meter")) {
		u=QPrinter::Millimeter;		
		s=1000.0; //1m=1000m
	} else if(!strcasecmp(o+i,"didot")) 
		u=QPrinter::Didot; //Todo is there a short for didot??
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

/*!
 * Set proxy information from a string
 * Proxy infromation will be parsed by the gramma described
 * in --help
 * \param proxy the proxy string to parse
 */
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
		//Todo we leak a little memory here
		char * leak = (char*)malloc(val-proxy+1);
		memcpy(leak,proxy,val-proxy);
		leak[val-proxy] = 0;
		proxy = val+1;
		proxyUser = leak;
		//Check to see that if a passwod is specified
		val = strchr(leak,':');
		if(val) { //There is a password
			val[0] = '\0'; //Seperate the username from the password
			proxyPassword = val+1;
		}
	}
	//Read hostname and port
	val = strchr(proxy,':');
	proxyPort = 1080; //Default proxy port
	if(val == NULL) proxyHost = proxy;
	else {
		//Todo we leak some more memory here
		char * leak = (char*)malloc(val-proxy+1);
		memcpy(leak,proxy,val-proxy);
		leak[val-proxy] = 0;
		proxyHost = leak;
		proxyPort = atoi(val);
	}
}

/*!
 * Parse a long style command line argument, and set configuration accordingly
 * \param arg the argument to parse without the leading --
 * \param morec the number of additional argument availeble, 
 * \param morev the additinal available arguments in a NULL terminated list.
 * \return the number of additional arguments used, or -1 if some error occured
 */
int WKHtmlToPdf::parseLongArg(const char * arg, int morec, const char ** morev) {
	size_t used=0;
	if(!strcmp(arg,"help")) {
		//Print usage information, and exit without an error
		usage(stdout); exit(0);
	} else if(!strcmp(arg,"version")) {
		//Print version information, and exit witout an error
		version(stdout); exit(0);
	} else if(!strcmp(arg,"quiet")) {
		//Suppres output
		quiet = true;
	}  else if(!strcmp(arg,"lowquality")) {
		//Lower the resolution of the output file (saves some space)
		resolution = QPrinter::ScreenResolution;
	} else if(!strcmp(arg,"nobackground")) {
		//Do not print the background
		background = false;
	} else if(!strcmp(arg,"footer-line")) {
		footer_line = true;
	} else if(!strcmp(arg,"header-line")) {
		header_line = true;
	} else {
		if(morec < 1) {usage(stderr);exit(1);}
		if(!strcmp(arg,"input"))
			//Specify the input url
			in = morev[++used];
		else if(!strcmp(arg,"output"))
			//Specify the name of the output file
			out = morev[++used];
		else if(!strcmp(arg,"proxy"))
			//Set up proxy
			setProxy(morev[++used]);
		else if(!strcmp(arg,"orientation"))
			//Change page orientation
			setOrientation(morev[++used]);
		else if(!strcmp(arg,"pagesize"))
			//Setup paper size
			setPageSize(morev[++used]);
		else if(!strcmp(arg,"grayscale"))
			//Change into grayscale mode
			colorMode = QPrinter::GrayScale;
		else if(!strcmp(arg,"dpi"))
			//Change the dpi (the output resolution)
			dpi=atoi(morev[++used]);
		else if(!strcmp(arg,"top"))
			//Set the top margin
			margin_top=parseUnitReal(morev[++used]);
		else if(!strcmp(arg,"right"))
			//Set the right margin
			margin_right=parseUnitReal(morev[++used]);
		else if(!strcmp(arg,"bottom"))
			//Set the bottom margin
			margin_bottom=parseUnitReal(morev[++used]);
		else if(!strcmp(arg,"left"))
			//Set the left margin
			margin_left=parseUnitReal(morev[++used]);
		else if(!strcmp(arg,"default-header")) {
			header_left="[webpage]";
			header_right="[page]/[toPage]";
			header_line=true;
			margin_top=parseUnitReal("2cm");
		} else if(!strcmp(arg,"header-font-name"))
			header_font_name=morev[++used];
		else if(!strcmp(arg,"header-font-size"))
			header_font_size=atoi(morev[++used]);
		else if(!strcmp(arg,"header-left"))
			header_left=morev[++used];
		else if(!strcmp(arg,"header-center"))
			header_center=morev[++used];
		else if(!strcmp(arg,"header-right"))
			header_right=morev[++used];
		else if(!strcmp(arg,"footer-font-name"))
			footer_font_name=morev[++used];
		else if(!strcmp(arg,"footer-font-size"))
			footer_font_size=atoi(morev[++used]);
		else if(!strcmp(arg,"footer-left"))
			footer_left=morev[++used];
		else if(!strcmp(arg,"footer-center"))
			footer_center=morev[++used];
		else if(!strcmp(arg,"footer-right"))
			footer_right=morev[++used];
		else if(!strcmp(arg,"redirect-delay"))
			jsredirectwait=atoi(morev[++used]);
		else return -1; //An invalid option was specified
	} 
	return used;
}

/*!
 * Parse command line arguments, and set configuration accordingly.
 * \param argc the number of command line arguments
 * \param argv a NULL terminated list with the arguments
 */
void WKHtmlToPdf::parseArgs(int argc, const char ** argv) {
	//Load default configuration

	//Note: We default to converting a file from stdin, this
	//might confuse some users, perhaps we should display usage.
	in = "/dev/stdin"; 
	out = "/dev/stdout";
	proxyHost = NULL; 
	quiet = false;
	jsredirectwait = 500;
	pageSize = QPrinter::A4; //Can a better value be guessed from some system setting?
	orientation = QPrinter::Portrait;
	colorMode = QPrinter::Color;
	resolution = QPrinter::HighResolution; //We default to producing good looking large files
	background = true; //Draw the background
	dpi = -1; //-1 indicates that we will use the default dpi provided by QT
	margin_top = margin_right = margin_bottom = margin_left=
		std::pair<qreal,QPrinter::Unit>(10,QPrinter::Millimeter);
	
	//Setup default header and footer settings
	header_font_size = footer_font_size = 11;
	header_font_name = footer_font_name = "Arial";
	header_line = footer_line = false;
	header_left = header_center = header_right = "";
	footer_left = footer_right = footer_center = "";
	
	char * val;
	//Load configuration from enviornment
	if((val = getenv("proxy"))) setProxy(val);
	if((val = getenv("all_proxy"))) setProxy(val);
	if((val = getenv("http_proxy"))) setProxy(val);

	//Provide some short commandline aliases
	std::map<const char, const char *> s2l;
	s2l['h'] = "help";      s2l['i'] = "input";      s2l['q']="quiet";
	s2l['o'] = "output";    s2l['p'] = "proxy";      s2l['s']="pagesize";
	s2l['g'] = "grayscale"; s2l['l'] = "lowquality"; s2l['O']="orientation";
	s2l['d'] = "dpi";       s2l['T'] = "top";        s2l['B']="bottom";
	s2l['R'] = "right";     s2l['L'] = "left";       s2l['b']="nobackground";
	s2l['H'] = "default-header";

	int x=0; //The number of default arguments read so fare
	for(int i=1; i < argc; ++i) {
		if(argv[i][0] != '-') {
			//Default arguments
			++x;
			if(x==1) in = argv[i]; //The first default argument is the input URL
			else if(x == 2)	out = argv[i]; //The second default is the output filename
			else {usage(stderr); exit(1);} //There are no bore default arguments
			continue;
		}
		if(argv[i][1] == '-') { //We have a long style argument
			int used = parseLongArg(argv[i]+2, argc - i, argv+i);
			//If some error occured print the usage information and exit indicating an error
			if(used == -1) {usage(stderr); exit(1);} 
			i += used;
		} else {
			int c=i;//Remember the current argument we are parsing
			for(int j=1; argv[c][j] != '\0'; ++j) {
				//If the short argument is invalid print usage information and exit
				if(s2l.count(argv[c][j]) == 0) {usage(stderr); exit(1);}
				//Set the correct options
				int used = parseLongArg(s2l[argv[c][j]], argc - i, argv+i);
				//If some error occured, print usage information and exit
				if(used == -1) {usage(stderr); exit(1);}
				i += used;
			}
		}
	}
}

/*!
 * Guess a url, by looking at a string
 * (shamelessly copied from Arora Project)
 * \param string The string the is suppose to be some kind of url
 */
QUrl WKHtmlToPdf::guessUrlFromString(const QString &string)
{
  QString urlStr = string.trimmed();

  // check if the string is just a host with a port
  QRegExp hostWithPort(QLatin1String("^[a-zA-Z\\.]+\\:[0-9]*$"));
  if (hostWithPort.exactMatch(urlStr))
    urlStr = QLatin1String("http://") + urlStr;

  // Check if it looks like a qualified URL. Try parsing it and see.
  QRegExp test(QLatin1String("^[a-zA-Z]+\\:.*"));
  bool hasSchema = test.exactMatch(urlStr);
  if (hasSchema) {
    bool isAscii = true;
    foreach (const QChar &c, urlStr) {
      if (c >= 0x80) {
	isAscii = false;
	break;
      }
    }

    QUrl url;
    if (isAscii) {
      url = QUrl::fromEncoded(urlStr.toAscii(), QUrl::TolerantMode);
    } else {
      url = QUrl(urlStr, QUrl::TolerantMode);
    }
    if (url.isValid())
      return url;
  }

  // Might be a file.
  if (QFile::exists(urlStr)) {
    QFileInfo info(urlStr);
    return QUrl::fromLocalFile(info.absoluteFilePath());
  }

  // Might be a shorturl - try to detect the schema.
  if (!hasSchema) {
    int dotIndex = urlStr.indexOf(QLatin1Char('.'));
    if (dotIndex != -1) {
      QString prefix = urlStr.left(dotIndex).toLower();
      QString schema = (prefix == QLatin1String("ftp")) ? prefix : QLatin1String("http");
      QUrl url(schema + QLatin1String("://") + urlStr, QUrl::TolerantMode);
      if (url.isValid())
	return url;
    }
  }

  // Fall back to QUrl's own tolerant parser.
  QUrl url = QUrl(string, QUrl::TolerantMode);

  // finally for cases where the user just types in a hostname add http
  if (url.scheme().isEmpty())
    url = QUrl(QLatin1String("http://") + string, QUrl::TolerantMode);
  return url;
}

/*!
 * Connect page signals, to our methods. And do other page
 * configurations.
 */
void WKHtmlToPdf::init() {
	//Allow for network control fine touning.
	page.setNetworkAccessManager(&am); 
	//If some ssl error occures we want sslErrors to be called, so the we can ignore it
	connect(&am, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)),this,
            SLOT(sslErrors(QNetworkReply*, const QList<QSslError>&)));
	//When loading is progressing we want loadProgress to be called
	connect(&page, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
	//Once the loading is done we want loadFinished to be called
	connect(&page, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
	connect(&page, SIGNAL(loadStarted()), this, SLOT(loadStarted()));
#ifdef WKHTMLTOPDF_QT_WEBFRAME_PATCH
	connect(page.mainFrame(), SIGNAL(printingNewPage(QPrinter*,int,int,int)), 
			this, SLOT(newPage(QPrinter*,int,int,int)));
#endif
}

/*!
 * Replace some variabels in a string used in a header or fooder
 * \param q the string to substitute in
 * \param f the number that [fromPage] is substituded for
 * \param t the number that [toPage] is substituded for
 * \param p the number that [page] is substituded for
 */
QString WKHtmlToPdf::hfreplace(const QString & q, int f, int t, int p) {
	QString _=q;
	return _
		.replace("[page]",QString::number(p),Qt::CaseInsensitive)
		.replace("[toPage]",QString::number(t),Qt::CaseInsensitive)
		.replace("[fromPage]",QString::number(f),Qt::CaseInsensitive)
		.replace("[webpage]",in);
}

/*!
 * Called whenever a new page is printed: Display some progress, and puts in
 * footers and headers
 * \param printer the printer that prints a new page
 * \prarm f the first page printed
 * \param t the last page printed
 * \param p the page currently printing
 */
void WKHtmlToPdf::newPage(QPrinter * printer, int f, int t, int p) {
	//Display some progress information to the user
	if(!quiet) {fprintf(stderr,"Printing page: %3d%%   \r",(p-f+1)*100/(t-f+1)); fflush(stdout);}

	//Get the painter assosiated with the printer
	QPainter & painter = *printer->paintEngine()->painter();

	//Webkit used all kinds of crasy cordinate transformation, and font setup
	//We save it here and restore some sane defaults
	painter.save();
	painter.resetMatrix();
	int h=printer->pageRect().height();
	int w=printer->pageRect().width();

	//If needed draw the header line
	if(header_line) painter.drawLine(0,0,w,0);
	//Guess the height of the header text
	painter.setFont(QFont(header_font_name, header_font_size));
	int dy = painter.boundingRect(0,0,w,h,Qt::AlignTop,"M").height();
	//Draw the header text
	QRect r=QRect(0,0-dy,w,h);
	painter.drawText(r, Qt::AlignTop | Qt::AlignLeft, hfreplace(header_left,f,t,p));
	painter.drawText(r, Qt::AlignTop | Qt::AlignHCenter, hfreplace(header_center,f,t,p));
	painter.drawText(r, Qt::AlignTop | Qt::AlignRight, hfreplace(header_right,f,t,p));

	//IF needed draw the footer line
	if(footer_line) painter.drawLine(0,h,w,h);
	//Guess the height of the footer text
	painter.setFont(QFont(footer_font_name, footer_font_size));
	dy = painter.boundingRect(0,0,w,h,Qt::AlignTop,"M").height();
	//Draw the fooder text
	r=QRect(0,0,w,h+dy);
	painter.drawText(r, Qt::AlignBottom | Qt::AlignLeft, hfreplace(footer_left,f,t,p));
	painter.drawText(r, Qt::AlignBottom | Qt::AlignHCenter, hfreplace(footer_center,f,t,p));
	painter.drawText(r, Qt::AlignBottom | Qt::AlignRight, hfreplace(footer_right,f,t,p));

	//Restore wkebkits crasy scaling and font settings
	painter.restore();
}

/*!
 * Parse arguments, load page and print it
 * \param argc the number of arguments
 * \param argv NULL terminated array of arguments
 */
void WKHtmlToPdf::run(int argc, const char ** argv) {
	loading=0;
	//Parse the arguments
	parseArgs(argc,argv);
	//Make a url of the input file
	QUrl url = guessUrlFromString(in);
	//If we must use a proxy, create a host of objects
	if(proxyHost) {
		QNetworkProxy proxy;
		proxy.setHostName(proxyHost);
		proxy.setPort(proxyPort);
		if(proxyUser) proxy.setUser(proxyUser);
		if(proxyPassword) proxy.setPassword(proxyPassword);
		am.setProxy(proxy);
	}
	//Disable stuff we don't need
	page.settings()->setAttribute(QWebSettings::JavaEnabled, false);
	page.settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
	page.settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, false);
#if QT_VERSION >= 0x040500
	//Newer vertions of QT have even more settings to change
	page.settings()->setAttribute(QWebSettings::PrintElementBackgrounds, background);
	page.settings()->setAttribute(QWebSettings::PluginsEnabled, false);
#endif 
	//Lets load the page
	page.mainFrame()->load(url);
}

/*!
 * Handel any ssl error by ignoring
 */
void WKHtmlToPdf::sslErrors(QNetworkReply *reply, const QList<QSslError> &error) {
	Q_UNUSED(error);
	//We ignore any ssl error, as it is next to impossible to send or receive
	//any private information with wkhtmltopdf anyhow, seeing as you cannot authenticate
	reply->ignoreSslErrors();
}

/*!
 * Print out the document to the pdf file
 */
void WKHtmlToPdf::printPage() {
	//If there are still pages loading wait til they are done
	if(loading != 0) return;

	//Give some user feed back
	if(!quiet) {fprintf(stderr, "Outputting page       \r"); fflush(stdout);}
	
	//Construct a printer object used to print the html to pdf
	QPrinter p(resolution);
	if(dpi != -1) p.setResolution(dpi);

	//Tell the printer object to print the file <out>
	p.setOutputFormat(
		strcmp(out + (strlen(out)-3),".ps")==0?
		QPrinter::PostScriptFormat : QPrinter::PdfFormat
		);
	p.setOutputFileName(out);
	
	//We currently only support margins with the same unit
	if(margin_left.second != margin_right.second ||
	   margin_left.second != margin_top.second ||	  
	   margin_left.second != margin_bottom.second) {
		fprintf(stderr, "Currently all margin units must be the same!\n");
		exit(1);
	}
	
	//Setup margins and papersize
	p.setPageMargins(margin_left.first, margin_top.first, 
					 margin_right.first, margin_bottom.first, 
					 margin_left.second);
	p.setPageSize(pageSize);
	p.setOrientation(orientation);
	p.setColorMode(colorMode);

	//Do the actual printing
	if(!p.isValid()) {
		fprintf(stderr,"Unable to write to output file\n");
	} else {
		page.mainFrame()->print(&p);
		if(!quiet) {printf("Done                 \n"); fflush(stdout);}
		//Inform the user that everything went well
	}
	//All went well, if there where no redirect since then, terminating
	if(loading == 0) qApp->quit();	
}

/*!
 * Once loading is finished, we start the printing
 * \param ok did the loading finish correctly?
 */
void WKHtmlToPdf::loadFinished(bool ok) {
	//Keep track of the number of pages currently loading
	loading.deref();
  	if(!ok) {
		//It went bad, return with 1
		fprintf(stderr, "Failed loading page\n");
		exit(1);
		return;
	}
	if(!quiet) {fprintf(stderr, "Waiting for redirect  \r");fflush(stdout);}
														
	//Wait a little while for js-redirects, and then print
	QTimer::singleShot(jsredirectwait, this, SLOT(printPage()));
}

/*!
 * Once loading starting, this is called
 */
void WKHtmlToPdf::loadStarted() {
	//Keep track of the number of pages currently loading
	loading.ref();
}

/*!
 * Called when the page is loading, display some progress to the using
 * \param progress the loading progress in percent
 */
void WKHtmlToPdf::loadProgress(int progress) {
	//Print out the load status
	if(!quiet) fprintf(stderr,"Loading page: %3d%%   \r",progress);
	fflush(stdout);
}

/*!
 * State mashine driven, shell like parser. This is used for
 * reading commandline options from stdin
 * \param buff the line to parse
 * \param nargc on return will hold the number of arguments read
 * \param nargv on return will hold the argumenst read and be NULL terminated
 */ 
enum State {skip, tok, q1, q2, q1_esc, q2_esc, tok_esc};
void parseString(char * buff, int &nargc, char **nargv) {
	State state = skip;
	int write_start=0;
	int write=0;
	for(int read=0; buff[read]!='\0'; ++read) {
		State next_state=state;
		switch(state) {
		case skip:
			//Whitespace skipping state
			if(buff[read]!=' ' && buff[read]!='\t' && buff[read]!='\r' && buff[read]!='\n') {
				--read; next_state=tok;
			}
			break;
		case tok:
			//Normal toking reading state
			if(buff[read]=='\'') next_state=q1;
			else if(buff[read]=='"') next_state=q2;
			else if(buff[read]=='\\') next_state=tok_esc;
			else if(buff[read]==' ' || buff[read]=='\t' || buff[read]=='\n' || buff[read]=='\r') {
				next_state=skip;
				if(write_start != write) {
					buff[write++]='\0';
					nargv[nargc++] = buff+write_start;
					if(nargc > 998) exit(1);
				}
				write_start = write;
			}
			else buff[write++] = buff[read];
			break;
		case q1:
			//State parsing a single qoute argument
			if(buff[read]=='\'') next_state=tok;
			else if(buff[read]=='\\') next_state=q1_esc;
			else buff[write++] = buff[read];
			break;
		case q2:
			//State parsing a double qoute argument
			if(buff[read]=='"') next_state=tok;
			else if(buff[read]=='\\') next_state=q2_esc;
			else buff[write++] = buff[read];
			break;
		case tok_esc:
			//Escape one char and return to the tokan parsing state
			next_state=tok; buff[write++] = buff[read];
			break;
		case q1_esc:
			//Espace one char and return to the single quote parsing state
			next_state=q1; buff[write++] = buff[read];
			break;
		case q2_esc:
			//Escape one char and return to the double qoute parsing state
			next_state=q2; buff[write++] = buff[read];
			break;
		}
		state=next_state;
	}
	//Remember the last parameter
	if(write_start != write) {
		buff[write++]='\0';
		nargv[nargc++] = buff+write_start;
	}
	nargv[nargc]=NULL;
}

int main(int argc, char * argv[]) {
	QApplication a(argc,argv); //Construct application, required for printing
	WKHtmlToPdf x; //Create convertion instance
	x.init();
	for(int i=1; i < argc; ++i)
		if(!strcmp(argv[i],"--read-args-from-stdin")) {
			char buff[20400];
			char *nargv[1000];
			nargv[0] = argv[0];
			while(fgets(buff,20398,stdin)) {
				int nargc=1;
				parseString(buff,nargc,nargv);
				x.run(nargc,(const char**)nargv);
				a.exec(); //Wait for application to terminate
			}
			exit(0);
		}
	x.run(argc,(const char **)argv); //Run convertion
	return a.exec(); //Wait for application to terminate
}
