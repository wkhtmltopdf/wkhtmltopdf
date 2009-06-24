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
#include "wkhtmltopdf.hh"
#include <QtWebKit>

//Argumet handler setting a variable of the type T to some constant
template <typename T> class AHConstSetter: public ArgHandler {
public:
    T & dst;
    const T src;	
    const T def;
    AHConstSetter(T & arg, const T s, const T d): dst(arg), src(s), def(d) {};
    bool operator() (const char ** args, WKHtmlToPdf * w) {Q_UNUSED(args); Q_UNUSED(w); dst=src; return true;}
    virtual void useDefault() {dst=def;}
};

template <typename T>
struct AHSomeSetter: public ArgHandler {
    T & val;
    const T def;
    AHSomeSetter(T & a, QString an, T d): val(a), def(d) {argn.push_back(an);}
    virtual void useDefault() {val=def;}
};

//Argument handler setting an int variable 
struct AHIntSetter: public AHSomeSetter<int> {
	AHIntSetter(int & a, QString an, int def): AHSomeSetter<int>(a,an,def) {};
	bool operator() (const char ** vals, WKHtmlToPdf * w) {Q_UNUSED(w); val = atoi(vals[0]); return true;}
	virtual QString getDesc() const {
		if(def == -1) return desc;
		return desc + " (default " + QString::number(def) + ")";
    }
};
//Argument handler setting a string variable 
struct AHStrSetter: public AHSomeSetter<const char*> {
    AHStrSetter(const char * & a, QString an, const char * def): AHSomeSetter<const char *>(a,an,def) {};
    bool operator() (const char ** vals, WKHtmlToPdf * w) {Q_UNUSED(w); val = vals[0]; return true;}
	virtual QString getDesc() const {
		if(def[0] == '\0') return desc;
		return desc + " (default " + def + ")";
	}
};
//Argument handler setting a real-number/unit combo variable 
struct AHUnitRealSetter: public AHSomeSetter<QPair<qreal, QPrinter::Unit> > {
	AHUnitRealSetter(QPair<qreal, QPrinter::Unit> & a, QString an, QPair<qreal, QPrinter::Unit> def): AHSomeSetter<QPair<qreal, QPrinter::Unit> >(a,an,def) {};
	bool operator() (const char ** vals, WKHtmlToPdf *w) {val=w->parseUnitReal(vals[0]); return true;}
};

//Argument handler responsible for calling a function
template <typename T> struct AHCaller: public ArgHandler {
	AHCaller() {}
	AHCaller(QString a1) {argn.push_back(a1);}
	bool operator() (const char **vals, WKHtmlToPdf * w) {return T()(vals,w);}
};

//All these function would have been lambda function, had C++ supported them, now we are forced to write them here

//Call the help method
struct HelpFunc {bool operator()(const char **vals, WKHtmlToPdf * w) {
	Q_UNUSED(vals); w->usage(stdout); exit(0);
}};
//Call the version method
struct VersionFunc {bool operator()(const char **vals, WKHtmlToPdf * w) {
	Q_UNUSED(vals); w->version(stdout); exit(0);
}};
//Call the sotProxy method
struct ProxyFunc{bool operator()(const char **vals, WKHtmlToPdf *w) {
	return w->setProxy(vals[0]);
}};
//Call the setOrientation method
struct OrientationFunc{bool operator()(const char ** vals, WKHtmlToPdf * w) {
	return w->setOrientation(vals[0]);
}};
//Call the stePageSize method
struct PageSizeFunc{bool operator()(const char ** vals, WKHtmlToPdf * w) {
	return w->setPageSize(vals[0]);
}};

//Setup default header values
struct DefaultHeaderFunc{bool operator()(const char **vals, WKHtmlToPdf * w) {
	Q_UNUSED(vals);
	w->header_left="[webpage]";
	w->header_right="[page]/[toPage]";
	w->header_line=true;
	w->margin_top=w->parseUnitReal("2cm");
	return true;
}};

//Setup default book mode
struct BookFunc{bool operator()(const char **vals, WKHtmlToPdf * w) {
	Q_UNUSED(vals);
	w->header_left="[section]";
	w->header_right="[page]/[toPage]";
	w->header_line=true;
	w->margin_top=w->parseUnitReal("2cm");
	w->outline = true;
	w->print_toc = true;
	return true;
}};

/*!
 * Set the page orientation from a string, allowed values are
 * Landscape or Portrait (case insensitive), print usage
 * and exit if other options are added.
 * \param o the string describing the orientation
 */
bool WKHtmlToPdf::setOrientation(const char * o) {
	if(!strcasecmp(o,"Landscape"))
		orientation = QPrinter::Landscape;
	else if(!strcasecmp(o,"Portrait"))
		orientation = QPrinter::Portrait;
	else
		return false;
	return true;
}

/*!
 * Set the page/paper size from a string, basicly all thinkable values
 * are allowed, in the odd case that a unallowd value is parsed,
 * print usage information and exit
 * \param o the string describing the paper size.
 */
bool WKHtmlToPdf::setPageSize(const char * o) {
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
	else return false;
	return true;
}

/*!
 * Parse a string describing a distance, into a real number and a unit.
 * \param o Tho string describing the distance
 */
QPair<qreal, QPrinter::Unit> WKHtmlToPdf::parseUnitReal(const char * o) {
	qreal s=1.0; //Since not all units are provided by qt, we use this variable to skale 
	//Them into units that are.
	QPrinter::Unit u=QPrinter::Millimeter;
	//Skip the real number part
	int i=0; 
	while('0' <= o[i]  && o[i] <= '9') ++i;
	if(o[i] == '.' || o[i] == '.') ++i;
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
	return QPair<qreal, QPrinter::Unit>(s*atof(o), u);
}

/*!
 * Set proxy information from a string
 * Proxy infromation will be parsed by the gramma described
 * in --help
 * \param proxy the proxy string to parse
 */
bool WKHtmlToPdf::setProxy(const char * proxy) {
	//Allow users to use no proxy, even if one is specified in the env
	if(!strcmp(proxy,"none")) {proxyHost = NULL;return true;}

	//Read proxy type bit "http://" or "socks5://"
	if(!strncmp(proxy,"http://",7)) {
		proxyType = QNetworkProxy::HttpProxy;
		proxy += 7;
	}
	else if(!strncmp(proxy,"socks5://",9)) {
		proxyType = QNetworkProxy::Socks5Proxy;
		proxy += 9;
	}
	//Read username and password
	char * val = (char *) strchr(proxy,'@');
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
	val = (char *) strchr(proxy,':');
	proxyPort = 1080; //Default proxy port
	if(val == NULL) proxyHost = proxy;
	else {
		//Todo we leak some more memory here
		char * leak = (char*)malloc(val-proxy+1);
		memcpy(leak,proxy,val-proxy);
		leak[val-proxy] = 0;
		proxyHost = leak;
		proxyPort = atoi(val+1);
	}
	return true;
}

/*!
 * Load the default configuration
 */
void WKHtmlToPdf::loadDefaults() {
	proxyHost = NULL; 
	pageSize = QPrinter::A4; //Can a better value be guessed from some system setting?
	orientation = QPrinter::Portrait;

	for(QMap<QString, ArgHandler*>::iterator i=longToHandler.begin(); i != longToHandler.end(); ++i)
		i.value()->useDefault();

	char * val;
	//Load configuration from enviornment
	if((val = getenv("proxy"))) setProxy(val);
	if((val = getenv("all_proxy"))) setProxy(val);
	if((val = getenv("http_proxy"))) setProxy(val);
}

/*!
 * Parse command line arguments, and set configuration accordingly.
 * \param argc the number of command line arguments
 * \param argv a NULL terminated list with the arguments
 */
void WKHtmlToPdf::parseArgs(int argc, const char ** argv) {
	//Load the default settigs
	loadDefaults();
	bool defaultMode = false;
	for(int i=1; i < argc; ++i) {
		if(argv[i][0] != '-' || argv[i][1] == '\0' || defaultMode ) {
			//Default arguments alwayes input or output files,
			//The following code is a bit of a hack, we register all 
			//default arguments as input urls, we then later 
			//extract the last one to use as output
			in.push_back(argv[i]);
		} else if(argv[i][1] == '-') { //We have a long style argument
			//After an -- apperas in the argument list all that follows is interpited as default arguments
			if(argv[i][2] == '0') {defaultMode=true; continue;}
			//Try to find a handler for this long switch
			QMap<QString, ArgHandler*>::iterator j = longToHandler.find(argv[i]+2);
			if(j == longToHandler.end()) { //Ups that argument did not exist
				fprintf(stderr,"Unknown long argument %s\n\n",argv[i]);
				usage(stderr); exit(1);
			}
			//Check to see if there is enough arguments to the switch
			if(argc-i < j.value()->argn.size()+1) {
				fprintf(stderr,"Not enough arguments parsed to %s\n\n",argv[i]);
				usage(stderr); exit(1);
			}
			if(!(*(j.value()))(argv+i+1,this)) {usage(stderr); exit(1);}
			//Skip allredy handled switch arguments
			i += j.value()->argn.size();
		} else {
			int c=i;//Remember the current argument we are parsing
			for(int j=1; argv[c][j] != '\0'; ++j) {
				QMap<char, ArgHandler*>::iterator k = shortToHandler.find(argv[c][j]);
				//If the short argument is invalid print usage information and exit
				if(k == shortToHandler.end()) {
					fprintf(stderr,"Unknown switch -%c\n\n",argv[c][j]);
					usage(stderr); exit(1);
				}
				//Check to see if there is enough arguments to the switch
				if(argc-i < k.value()->argn.size()+1) {
					fprintf(stderr,"Not enough arguments parsed to -%c\n\n",argv[c][j]);
					usage(stderr); exit(1);
				}
				if(!(*(k.value()))(argv+i+1,this)) {usage(stderr); exit(1);}
				//Skip allredy handled switch arguments
				i += k.value()->argn.size();
			}
		}
	}
	
	if(in.size() < 2) {
		fprintf(stderr,"You need to specify atleast one input file, and exactly one output file\nUse - for stdin or stdout\n\n");
		usage(stderr); exit(1);
	}
	//The last default argument was realy the name of the output file
	out = in.back(); 
	in.pop_back();
}

/*!
 * Add an argument handler, 
 * \param l The long name of the argument, i.e. the --name
 * \param s The short switch of the argument, i.e. the -n, or 0 if none
 * \param d A descripton of what the argument does
 * \param h The argument handler
 */
void WKHtmlToPdf::addarg(QString l, char s, QString d, ArgHandler * h, bool display) {
	h->longName=l; h->shortSwitch=s; h->desc=d;
	h->display = display;
	longToHandler[l] = h;
	if(s) shortToHandler[s] = h;
};

/*!
 *  Initialize argument handling by adding all the argument handlers
 */
void WKHtmlToPdf::initArgs() {
	addarg("disable-javascript",'n',"Do not allow webpages to run javascript", new AHConstSetter<bool>(disable_javascript,true,false));
	addarg("dpi",'d',"Change the dpi explicitly", new AHIntSetter(dpi,"dpi",-1));
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	addarg("default-header",'H',"Add a default header, with the name of the page to the left, and the page number to the right, this is short for: --header-left='[webpage]' --header-right='[page]/[toPage]' --top 2cm --header-line", new AHCaller<DefaultHeaderFunc>());
	addarg("footer-center",0,"Centered footer text", new AHStrSetter(footer_center,"text",""));
	addarg("footer-font-name",0,"Set footer font name", new AHStrSetter(footer_font_name,"name","Arial"));;
	addarg("footer-font-size",0,"Set footer font size", new AHIntSetter(footer_font_size,"size",11));
	addarg("footer-left",0,"Left aligned footer text", new AHStrSetter(footer_left,"text",""));
	addarg("footer-line",0,"Display line above the footer", new AHConstSetter<bool>(footer_line,true,false));
	addarg("footer-right",0,"Right aligned footer text", new AHStrSetter(footer_right,"text",""));
	addarg("header-center",0,"Centered header text", new AHStrSetter(header_center,"text",""));
	addarg("header-font-name",0,"Set header font name", new AHStrSetter(header_font_name,"name","Arial"));
	addarg("header-font-size",0,"Set header font size", new AHIntSetter(header_font_size,"size",11));
	addarg("header-left",0,"Left aligned header text", new AHStrSetter(header_left,"text",""));
	addarg("header-line",0,"Display line below the header", new AHConstSetter<bool>(header_line,true,false));
	addarg("header-right",0,"Right aligned header text", new AHStrSetter(header_right,"text",""));
	addarg("toc",'t',"Insert a table of content in the beginning of the document", new AHConstSetter<bool>(print_toc,true,false));
	addarg("toc-font-name",0,"Set the font used for the toc", new AHStrSetter(tocPrinter.font_name,"name","Arial"));
	addarg("toc-no-dots",0,"Do not use dots, in the toc", new AHConstSetter<bool>(tocPrinter.useDots,false,true));
	addarg("toc-depth",0,"Set the depth of the toc", new AHIntSetter(tocPrinter.depth,"level",3));
	addarg("toc-header-text",0,"The header text of the toc", new AHStrSetter(tocPrinter.header_text,"text","Table Of Contents"));
	addarg("toc-header-fs",0,"The font size of the toc header", new AHIntSetter(tocPrinter.header_font_size,"size",15));
	addarg("print-media-type",0,"Use print media-type instead of screen", new AHConstSetter<bool>(printMediaType,true,false));
	for(uint i=0; i < TocPrinter::levels; ++i) {
		addarg(QString("toc-l")+QString::number(i+1)+"-font-size",0,QString("Set the font size on level ")+QString::number(i+1)+" of the toc",new AHIntSetter(tocPrinter.font_size[i],"size",12-2*i), i < 3);
		addarg(QString("toc-l")+QString::number(i+1)+"-indentation",0,QString("Set indentation on level ")+QString::number(i+1)+" of the toc",new AHIntSetter(tocPrinter.indentation[i],"num",i*20), i < 3); 
	}
	addarg("outline",0,"Put an outline into the pdf", new AHConstSetter<bool>(outline,true,false));
	addarg("outline-depth",0,"Set the depth of the outline", new AHIntSetter(tocPrinter.outline_depth,"level",4));
	addarg("book",'b',"Set the options one would usualy set when printing a book", new AHCaller<BookFunc>());	
	addarg("cover",0,"Use html document as cover. It will be inserted before the toc with no headers and footers",new AHStrSetter(cover,"url",""));
	addarg("encoding",0,"Set the default text encoding, for input", new AHStrSetter(default_encoding,"encoding",""));
#endif
	addarg("grayscale",'g',"PDF will be generated in grayscale", new AHConstSetter<QPrinter::ColorMode>(colorMode,QPrinter::GrayScale,QPrinter::Color));
	addarg("help",'h',"Display help",new AHCaller<HelpFunc>());
	addarg("lowquality",'l',"Generates lower quality pdf/ps. Useful to shrink the result document space", new AHConstSetter<QPrinter::PrinterMode>(resolution,QPrinter::ScreenResolution,QPrinter::HighResolution));
	addarg("margin-bottom",'B',"Set the page bottom margin (default 10mm)", new AHUnitRealSetter(margin_bottom,"unitread",QPair<qreal,QPrinter::Unit>(10,QPrinter::Millimeter)));
	addarg("margin-left",'L',"Set the page left margin (default 10mm)", new AHUnitRealSetter(margin_left,"unitread",QPair<qreal,QPrinter::Unit>(10,QPrinter::Millimeter)));
	addarg("margin-right",'R',"Set the page right margin (default 10mm)", new AHUnitRealSetter(margin_right,"unitread",QPair<qreal,QPrinter::Unit>(10,QPrinter::Millimeter)));
	addarg("margin-top",'T',"Set the page top margin (default 10mm)", new AHUnitRealSetter(margin_top,"unitread",QPair<qreal,QPrinter::Unit>(10,QPrinter::Millimeter)));
#if QT_VERSION >= 0x040500 //Not printing the background was added in QT4.5
	addarg("no-background",0,"Do not print background", new AHConstSetter<bool>(background,false,true));
	addarg("user-style-sheet",0,"Specify a user style sheet, to load with every page", new AHStrSetter(user_style_sheet,"url",""));
#endif
	addarg("orientation",'O',"Set orientation to Landscape or Portrait", new AHCaller<OrientationFunc>("orientation"));
	addarg("page-size",'s',"Set pape size to: A4, Letter, ect.", new AHCaller<PageSizeFunc>("size"));
	addarg("proxy",'p',"Use a proxy", new AHCaller<ProxyFunc>("proxy"));
	addarg("quit",'q',"Be less verbose",new AHConstSetter<bool>(quiet,true,false));
	addarg("redirect-delay",0,"Wait some miliseconds for js-redirects", new AHIntSetter(jsredirectwait,"msec",200));
	addarg("version",'V',"Output version information an exit", new AHCaller<VersionFunc>());
	addarg("enable-plugins",0,"Enable installed plugins (such as flash", new AHConstSetter<bool>(enable_plugins,true,false));
#ifdef Q_WS_X11
	addarg("use-xserver",0,"Use the X server (some plugins and other stuff might not work without X11)", new AHConstSetter<bool>(use_x11,true,false));
#endif
}

/*!
 * Print out program usage information
 * \param fd the filedescriptor where the output will be printed
 */
void WKHtmlToPdf::usage(FILE * fd) {
	fprintf(fd,
"Usage: wkhtmltopdf [OPTION]... <input file> [more input files] <output file> \n"
"converts htmlpages into a pdf\n"
"\n"
"Options:\n");
	//Go ower the list of supported arguments in alphabetital order
	for(QMap<QString, ArgHandler*>::const_iterator i=longToHandler.begin(); i != longToHandler.end(); ++i) {
		const ArgHandler * h = i.value();
		if(!h->display) continue;
		//Print its short switch
		if(h->shortSwitch) fprintf(fd,"  -%c,",h->shortSwitch);
		else fprintf(fd,"     ");
		int w=5; //Keep track of the number of chars written
		//Write out the longswitch
		fprintf(fd," --%s",(const char*)h->longName.toLocal8Bit());
		w+=3+h->longName.length();
		//Write out all of the arguments to the switch
		for(int j=0; j<h->argn.size(); ++j) {
			fprintf(fd," <%s>",(const char*)h->argn[j].toLocal8Bit());
			w+=3+h->argn[j].length();
		}
		//Write out the description with word wrapping
		QStringList list = (h->getDesc()+".").split(" ");
		int j=0;
		while(j < list.size()) {//We will get here once per line
			//A line should start after some indentation
			for(;w<33;++w) fprintf(fd," ");
			//A line is not allowed to tart with a '-' as this, messes with the man page generation
			if(list[j][0] == '-') {fprintf(fd," *"); w+=2;}
			//Write as many words as we can, before running out of space
			for(; j < list.size() && w < 84; ++j) {
				fprintf(fd," %s",(const char*)list[j].toLocal8Bit());
				w += list[j].length()+1;
			}
			fprintf(fd,"\n");
			w=0;
		}
	}
	fprintf(fd,
"\n"
"Proxy:\n"
"  By default proxy information will be read from the environment\n"
"  variables: proxy, all_proxy and http_proxy, proxy options can\n"
"  also by specified with the -p switch\n"
"  <type> := \"http://\" | \"socks5://\"\n" 
"  <userinfo> := <username> (\":\" <password>)? \"@\"\n"
"  <proxy> := \"None\" | <type>? <userinfo>? <host> (\":\" <port>)?\n"
"\n"
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
"Header and footer text:\n"
"In a header or footer text the following variables can be used\n"
" * [page]       Replaced by the number of the pages currently beeing printed\n"
" * [fromPage]   Replaced by the number of the first page to be printed\n"
" * [toPage]     Replaced by the number of the last page to be printed\n"
" * [webpage]    Replaced by the url of the page beeing printed\n"
" * [section]    Replaced by the name of the current section\n"
" * [subsection] Replaced by the name of the current subsection\n" 
"\n"
#else 
"This version of wkhtmltopdf has been compiled against a version of qt\n"
"without the wkhtmltopdf patches, so some features are missign, if you need\n"
"these features please use the staic edition\n"
"\n"
#endif
"Mail bug reports and suggestions to <antialze@gmail.com>.\n"
		);
}

/*!
 * Print out version and license information
 * \param fd the filedescriptor where the output will be printed
 */
void WKHtmlToPdf::version(FILE * fd) {
	fprintf(fd,
"wkhtmltopdf %d.%d.%d" 
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
" using wkhtmltopdf patched qt\n"
#else
" NOT using wkhtmltopdf patched qt\n"
#endif
"Copyright (C) 2008,2009 Jakob Truelsen,\n"
"License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
"This is free software: you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law.\n"
"\n"
"Written by Jakob Truelsen\n"
"Patches by Mário Silva and Emmanuel Bouthenot\n\n",
			MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION);
}
