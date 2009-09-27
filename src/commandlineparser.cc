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

/*!
  \class ArgHandler
  \brief Class responcible for handling an argument
*/

/*!
  \var ArgHandler::longName
  \brief The long name of the argument, e.g. "help" for "--help"
*/

/*!
  \var ArgHandler::desc
  \brief A descriptive text of the argument
*/

/*!
  \var ArgHandler::shortSwitch
  \brief Sort name, e.g. 'h' for '-h', if 0 there is no short name
*/

/*!
  \var ArgHandler::argn
  \brief The names of the arguments to the switch
*/

/*!											  
  \var ArgHandler::display
  \brief Indicate that the argument is not hidden
*/

/*!
  \fn ArgHandler::operator()(const char ** args, Settings & settings)
  Callend when the switch was specified
  \param args The arguments to the switch, garantied to have size of argn
  \param settings The settings to store the information in
*/
 
/*!
  Set give settings its default value

  This is a NOOP for ArgHandler
  \param parser The parser giving the request
*/
void ArgHandler::useDefault(CommandLineParserPrivate & parser) {
	Q_UNUSED(parser);
} 
  
/*!
  Get the description of this switch
*/  
QString ArgHandler::getDesc() const {
	return desc;
}

/*!
  Dummy virtual destructor
*/  
ArgHandler::~ArgHandler() {}


/*!
  \class CommandLineParserPrivate
  Implementation details for CommandLineParser
*/


/*!
  Sets a variable to some constant
*/
template <typename T> class ConstSetter: public ArgHandler {
public:
	T & dst;
	const T src;
	const T def;
	ConstSetter(T & arg, const T s, const T d): dst(arg), src(s), def(d) {};
	bool operator() (const char **, CommandLineParserPrivate &) {
		dst=src;
		return true;
	}
	virtual void useDefault(CommandLineParserPrivate &) {
		dst=def;
	}
};

/*!
  Base class used to set a variable to a value of some type T
*/
template <typename T>
struct SomeSetter: public ArgHandler {
	T & val;
	const T def;
	SomeSetter(T & a, QString an, T d): val(a), def(d) {
		argn.push_back(an);
	}
	virtual void useDefault(CommandLineParserPrivate &) {
		val=def;
	}
};

/*!
  Argument handler setting an int variable
*/
struct IntSetter: public SomeSetter<int> {
	IntSetter(int & a, QString an, int def): SomeSetter<int>(a,an,def) {};
	bool operator() (const char ** vals, CommandLineParserPrivate &) {
		val = atoi(vals[0]);
		return true;
	}
	virtual QString getDesc() const {
		if (def == -1) return desc;
		return desc + " (default " + QString::number(def) + ")";
	}
};

/*!
  Argument handler setting an float variable
*/
struct FloatSetter: public SomeSetter<float> {
	FloatSetter(float & a, QString an, float def): SomeSetter<float>(a,an,def) {};
	bool operator() (const char ** vals, CommandLineParserPrivate &) {
		val = atof(vals[0]);
		return true;
	}
	virtual QString getDesc() const {
		if (def == -1) return desc;
		return desc + " (default " + QString::number(def) + ")";
	}
};

/*!
  Argument handler setting a string variable
*/
struct StrSetter: public SomeSetter<const char*> {
	StrSetter(const char * & a, QString an, const char * def): SomeSetter<const char *>(a,an,def) {};
	bool operator() (const char ** vals, CommandLineParserPrivate &) {
		val = vals[0];
		return true;
	}
	virtual QString getDesc() const {
		if (def[0] == '\0') return desc;
		return desc + " (default " + def + ")";
	}
};

/*!
  Argument handler setting a string variable
*/
struct QStrSetter: public SomeSetter<QString> {
	QStrSetter(QString & a, QString an, QString def): SomeSetter<QString>(a,an,def) {};
	bool operator() (const char ** vals, CommandLineParserPrivate &) {
		val = QString::fromUtf8(vals[0]);
		return true;
	}
	virtual QString getDesc() const {
		if (def[0] == '\0') return desc;
		return desc + " (default " + def + ")";
	}
};

/*!
  Argument handler setting a real-number/unit combo variable  
 */
struct UnitRealSetter: public SomeSetter<QPair<qreal, QPrinter::Unit> > {
	UnitRealSetter(QPair<qreal, QPrinter::Unit> & a, QString an, QPair<qreal, QPrinter::Unit> def): SomeSetter<QPair<qreal, QPrinter::Unit> >(a,an,def) {};
	bool operator() (const char ** vals, CommandLineParserPrivate &) {
		//val=w->parseUnitReal(vals[0]);
		return true;
	}
};

/*!
  Argument handler responsible for calling a function
*/
template <typename T> struct Caller: public ArgHandler {
	Caller() {}
	Caller(QString a1) {
		argn.push_back(a1);
	}
	bool operator() (const char **vals, CommandLineParserPrivate & s) {
		return T()(vals,s);
	}
};

//All these function would have been lambda function, had C++ supported them, now we are forced to write them here

/*!
  Lamba: Call the usage method
*/
template <bool v>
struct HelpFunc {
	bool operator()(const char **, CommandLineParserPrivate & p) {
		p.usage(stdout,v);
		exit(0);
	}
};

/*!
  Lambda: Call the man method
*/
struct ManPageFunc {
	bool operator()(const char **vals, CommandLineParserPrivate & p) {
		p.manpage(stdout);
		exit(0);
	}
};



/*!
  Lambda: Call the man method
*/
template <bool T>
struct ReadmeFunc {
	bool operator()(const char **vals, CommandLineParserPrivate & p) {
		p.readme(stdout, T);
		exit(0);
	}
};


/*!
  Lambda: Call the version method
*/
struct VersionFunc {
	bool operator()(const char **vals, CommandLineParserPrivate & p) {
		p.version(stdout);
		exit(0);
	}
};

/*!
  Lambda: Call the setProxy method
*/
struct ProxyFunc {
	bool operator()(const char **vals, CommandLineParserPrivate & p) {
		//return w->setProxy(vals[0]);
	}
};

/*!
  Lambda: Call the setOrientation method
*/
struct OrientationFunc {
	bool operator()(const char ** vals, CommandLineParserPrivate & p) {
		//return w->setOrientation(vals[0]);
	}
};

/*!
  Lambda: Call the stePageSize method
*/
struct PageSizeFunc {
	bool operator()(const char ** vals, CommandLineParserPrivate & w) {
		//return w->setPageSize(vals[0]);
	}
};

/*!
  Set the default header
*/
struct DefaultHeaderFunc {
	bool operator()(const char **, CommandLineParserPrivate & p) {
		p.settings.header.left="[webpage]";
		p.settings.header.right="[page]/[toPage]";
		p.settings.header.line=true;
		//w->margin_top=w->parseUnitReal("2cm");
		return true;
	}
};

/*!
  Setup default book mode
*/
struct BookFunc {
	bool operator()(const char **, CommandLineParserPrivate & p) {
		p.settings.header.left="[section]";
		p.settings.header.right="[page]/[toPage]";
		p.settings.header.line=true;
		p.settings.outline = true;
		p.settings.printToc = true;
			//w->margin_top=w->parseUnitReal("2cm");
		return true;
	}
};


CommandLineParserPrivate::CommandLineParserPrivate(Settings & s):
	settings(s)
{
	section("General Settings");

	addarg("help",'h',"Display help",new Caller<HelpFunc<false> >());
	addarg("quiet",'q',"Be less verbose",new ConstSetter<bool>(s.quiet,true,false));
	addarg("version",'V',"Output version information an exit", new Caller<VersionFunc>());
	addarg("extended-help",0,"Display more extensive help", new Caller<HelpFunc<true> >());
	addarg("collate", 0, "Collate when printing multiple copies", new ConstSetter<bool>(s.collate,true,false));
	addarg("copies", 0, "Number of copies to print into the pdf file", new IntSetter(s.copies, "number", 1));
	addarg("orientation",'O',"Set orientation to Landscape or Portrait", new Caller<OrientationFunc>("orientation"));
	addarg("page-size",'s',"Set pape size to: A4, Letter, ect.", new Caller<PageSizeFunc>("size"));
	addarg("proxy",'p',"Use a proxy", new Caller<ProxyFunc>("proxy"));
	addarg("username",0,"HTTP Authentication username", new QStrSetter(s.username, "username",""));
	addarg("password",0,"HTTP Authentication password", new QStrSetter(s.password, "password",""));

	qthack(true);
	addarg("book",'b',"Set the options one would usualy set when printing a book", new Caller<BookFunc>());
	addarg("cover",0,"Use html document as cover. It will be inserted before the toc with no headers and footers",new StrSetter(s.cover,"url",""));
	addarg("default-header",'H',"Add a default header, with the name of the page to the left, and the page number to the right, this is short for: --header-left='[webpage]' --header-right='[page]/[toPage]' --top 2cm --header-line", new Caller<DefaultHeaderFunc>());
	addarg("toc",'t',"Insert a table of content in the beginning of the document", new ConstSetter<bool>(s.printToc,true,false));
	qthack(false);
	
	extended(true);
	addarg("manpage", 0, "Output program man page", new Caller<ManPageFunc>());
	addarg("htmldoc", 0, "Output program html help", new Caller<ReadmeFunc<true> >());
	addarg("readme", 0, "Output program readme", new Caller<ReadmeFunc<false> >());
	addarg("dpi",'d',"Change the dpi explicitly", new IntSetter(s.dpi,"dpi",-1));
	addarg("disable-javascript",'n',"Do not allow webpages to run javascript", new ConstSetter<bool>(s.enableJavascript,false,true));
	addarg("grayscale",'g',"PDF will be generated in grayscale", new ConstSetter<QPrinter::ColorMode>(s.colorMode,QPrinter::GrayScale,QPrinter::Color));
	addarg("lowquality",'l',"Generates lower quality pdf/ps. Useful to shrink the result document space", new ConstSetter<QPrinter::PrinterMode>(s.resolution,QPrinter::ScreenResolution,QPrinter::HighResolution));
	addarg("margin-bottom",'B',"Set the page bottom margin (default 10mm)", new UnitRealSetter(s.margin.bottom,"unitread",QPair<qreal,QPrinter::Unit>(10,QPrinter::Millimeter)));
	addarg("margin-left",'L',"Set the page left margin (default 10mm)", new UnitRealSetter(s.margin.left,"unitread",QPair<qreal,QPrinter::Unit>(10,QPrinter::Millimeter)));
	addarg("margin-right",'R',"Set the page right margin (default 10mm)", new UnitRealSetter(s.margin.right,"unitread",QPair<qreal,QPrinter::Unit>(10,QPrinter::Millimeter)));
	addarg("margin-top",'T',"Set the page top margin (default 10mm)", new UnitRealSetter(s.margin.top,"unitread",QPair<qreal,QPrinter::Unit>(10,QPrinter::Millimeter)));
	addarg("redirect-delay",0,"Wait some miliseconds for js-redirects", new IntSetter(s.jsredirectwait,"msec",200));
	addarg("enable-plugins",0,"Enable installed plugins (such as flash", new ConstSetter<bool>(s.enablePlugins,true,false));
	addarg("zoom",0,"Use this zoom factor", new FloatSetter(s.zoomFactor,"float",1.0));

	qthack(true);
	addarg("disable-internal-links",0,"Do no make local links", new ConstSetter<bool>(s.useLocalLinks,false,true));
	addarg("disable-external-links",0,"Do no make links to remote webpages", new ConstSetter<bool>(s.useExternalLinks,false,true));
	addarg("print-media-type",0,"Use print media-type instead of screen", new ConstSetter<bool>(s.printMediaType,true,false));
	addarg("page-offset",0,"Set the starting page number", new IntSetter(s.pageOffset,"offset",1));
	addarg("disable-smart-shrinking", 0, "Disable the intelligent shrinking strategy used by webkit that makes the pixel/dpi ratio none constant",new ConstSetter<bool>(s.enableIntelligentShrinking, false, true));
#ifdef Q_WS_X11
	addarg("use-xserver",0,"Use the X server (some plugins and other stuff might not work without X11)", new ConstSetter<bool>(s.useGraphics,true,false));
#endif

#if QT_VERSION >= 0x040600
	qthack(false);
#endif
	addarg("encoding",0,"Set the default text encoding, for input", new QStrSetter(s.defaultEncoding,"encoding",""));
	qthack(false);
	
#if QT_VERSION >= 0x040500 //Not printing the background was added in QT4.5
	addarg("no-background",0,"Do not print background", new ConstSetter<bool>(s.background,false,true));
	addarg("user-style-sheet",0,"Specify a user style sheet, to load with every page", new QStrSetter(s.userStyleSheet,"url",""));
#endif
	
	extended(false);
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	section("Headers And Hooters");
	qthack(true);
	extended(true);
	addarg("footer-center",0,"Centered footer text", new QStrSetter(footer_center,"text",""));
	addarg("footer-font-name",0,"Set footer font name", new StrSetter(footer_font_name,"name","Arial"));;
	addarg("footer-font-size",0,"Set footer font size", new IntSetter(footer_font_size,"size",11));
	addarg("footer-left",0,"Left aligned footer text", new QStrSetter(footer_left,"text",""));
	addarg("footer-line",0,"Display line above the footer", new ConstSetter<bool>(footer_line,true,false));
	addarg("footer-right",0,"Right aligned footer text", new QStrSetter(footer_right,"text",""));
	addarg("header-center",0,"Centered header text", new QStrSetter(header_center,"text",""));
	addarg("header-font-name",0,"Set header font name", new StrSetter(header_font_name,"name","Arial"));
	addarg("header-font-size",0,"Set header font size", new IntSetter(header_font_size,"size",11));
	addarg("header-left",0,"Left aligned header text", new QStrSetter(header_left,"text",""));
	addarg("header-line",0,"Display line below the header", new ConstSetter<bool>(header_line,true,false));
	addarg("header-right",0,"Right aligned header text", new QStrSetter(header_right,"text",""));
	extended(false);
	qthack(false);
	
	section("Table Of Content");
	extended(true);
	qthack(true);
	addarg("toc-font-name",0,"Set the font used for the toc", new StrSetter(tocPrinter.font_name,"name","Arial"));
	addarg("toc-no-dots",0,"Do not use dots, in the toc", new ConstSetter<bool>(tocPrinter.useDots,false,true));
	addarg("toc-depth",0,"Set the depth of the toc", new IntSetter(tocPrinter.depth,"level",3));
	addarg("toc-header-text",0,"The header text of the toc", new StrSetter(tocPrinter.header_text,"text","Table Of Contents"));
	addarg("toc-header-fs",0,"The font size of the toc header", new IntSetter(tocPrinter.header_font_size,"size",15));
	addarg("toc-disable-links",0,"Do not link from toc to sections", new ConstSetter<bool>(tocPrinter.forward_links,false, true));
	addarg("toc-disable-back-links",0,"Do not link from section header to toc", new ConstSetter<bool>(tocPrinter.back_links,false,true));
	for (uint i=0; i < TocPrinter::levels; ++i) {
		addarg(QString("toc-l")+QString::number(i+1)+"-font-size",0,QString("Set the font size on level ")+QString::number(i+1)+" of the toc",new IntSetter(tocPrinter.font_size[i],"size",12-2*i), i < 3);
		addarg(QString("toc-l")+QString::number(i+1)+"-indentation",0,QString("Set indentation on level ")+QString::number(i+1)+" of the toc",new IntSetter(tocPrinter.indentation[i],"num",i*20), i < 3);
	}
	qthack(true);
	extended(false);

	section("Outline");
	extended(true);
	qthack(true);
	addarg("outline",0,"Put an outline into the pdf", new ConstSetter<bool>(outline,true,false));
	addarg("outline-depth",0,"Set the depth of the outline", new IntSetter(tocPrinter.outline_depth,"level",4));
	qthack(true);
	extended(false);
#endif
}

void CommandLineParserPrivate::manpage(FILE * fd) const {
	Outputter * o = Outputter::man(fd);
 	outputName(o);
 	outputSynopsis(o);
 	outputDescripton(o);
 	//d->outputSwitches(o, true, false);
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
 	outputProxyDoc(o);
 	outputHeaderFooterDoc(o);
 	outputOutlineDoc(o);
#else
	outputNotPatched(o,true);
#endif
 	outputPageBreakDoc(o);
 	outputContact(o);
 	outputAuthors(o);
	delete o;
}

void CommandLineParserPrivate::usage(FILE * fd, bool extended) const {
	
}



void CommandLineParser::parse(int argc, const char ** argv) const {
}


void CommandLineParser::usage(FILE * fd, bool extended) const {
	d->usage(fd, extended);
}
	

void CommandLineParser::version(FILE * fd) const {
	d->version(fd);
 	// TextOutputter o(fd);
//  	d->outputName(o);
//  	d->outputLicense(o);
//  	d->outputAuthors(o);
}

void CommandLineParser::manpage(FILE * fd) const {
	d->manpage(fd);
}


void CommandLineParser::readme(FILE * fd, bool html) const {
// 	if(html) {
// 		HtmlOutputter o(fd);
// 		d->readme(o);
// 	} else {
// 		TextOutputter o(fd, true);
// 		d->readme(o);
// 	}
}

CommandLineParser::CommandLineParser(Settings & s):
	d(new CommandLineParserPrivate(s))
{
	;
}
	
CommandLineParser::~CommandLineParser() {
	delete d;
}






// 	d->outputPageBreakdoc(o);
// 	d->outputContact(o);
// 	d->outputAuthors(o);
