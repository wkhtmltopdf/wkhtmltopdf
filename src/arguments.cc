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
#include <qglobal.h>

/*!
  \class ArgHandler
  \brief Class responsible for handling an argument
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
  \var ArgHandler::extended
  \brief Indicate if the argument is an extended argument
*/

/*!											  
  \var ArgHandler::qthack
  \brief Indicate that the argument is only available with hacked qt
*/

/*!
  \fn ArgHandler::operator()(const char ** args, CommandLineParserPrivate & parser)
  Callend when the switch was specified
  \param args The arguments to the switch, guarantied to have size of argn
  \param settings The settings to store the information in
*/

/*!
  \fn ArgHandler::useDefault(CommandLineParserPrivate & parser)
  Set give settings its default value

  This is a NOOP for ArgHandler
  \param parser The parser giving the request
*/
void ArgHandler::useDefault(CommandLineParserPrivate & parser) {
	Q_UNUSED(parser);
} 

/*!
  \fn ArgHandler::getDesc() const
  Get the description of this switch
*/  
QString ArgHandler::getDesc() const {
	return desc;
}

/*!
  \fn ArgHandler::~ArgHandler()
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

struct StringPairCreator {
	typedef QPair<QString, QString> T;
	inline T operator()(const QString & key, const QString & value) const {
		return T(key, value);
	}
};

template <bool file> 
struct PostItemCreator {
	typedef typename Settings::PostItem T;
	inline T operator()(const QString & key, const QString & value) const {
		T p;
		p.name = key;
		p.value = value;
		p.file = file;
		return p;
	}
};

/*!
  Putting values into a map
*/
template <typename T=StringPairCreator>
struct MapSetter: public ArgHandler {
	QList< typename T::T > & dst;
	MapSetter(QList<typename T::T > & a, QString keyName, QString valueName) : dst(a) {
		argn.push_back(keyName);
		argn.push_back(valueName);
	}
	virtual bool operator() (const char ** args, CommandLineParserPrivate &) {
		dst.append( T()(args[0], args[1]) );
		return true;
	}
	virtual void useDefault() {
		dst.clear();
	}
};


/*!
  SomeSetter template method base
*/
template <typename TT> 
struct SomeSetterTM {
	typedef TT T;
	//T strToT(const char * val, bool & ok);
	static QString TToStr(const T &, bool & ok) {ok=false; return "";}
};

/*!
  TemplateMethod class used to set a single variable of some type TT::T
*/
template <typename TT>
struct SomeSetter: public ArgHandler {
	typedef typename TT::T T;
	T & val;
	T def;
	bool hasDef;

	SomeSetter(T & a, QString an, T d): val(a), def(d), hasDef(true) {
		argn.push_back(an);
	}

	SomeSetter(T & a, QString an): val(a), hasDef(false) {
		argn.push_back(an);
	}

	virtual void useDefault(CommandLineParserPrivate &) {
		if (hasDef)
			val=def;
	}

	bool operator() (const char ** vals, CommandLineParserPrivate &) {
		bool ok;
		val = TT::strToT(vals[0], ok);
		return ok;
	}

	virtual QString getDesc() const {
		if (!hasDef) return desc;
		bool ok;
		QString x = TT::TToStr(def,ok);
		if (!ok) return desc;
		return desc + " (default " + x + ")";
	}
};

struct IntTM: public SomeSetterTM<int> {
	static int strToT(const char * val, bool & ok) {
		return QString(val).toInt(&ok);
	}
	static QString TToStr(const int & t, bool & ok) {
		ok=(t!=-1);
		return QString::number(t);
	}
};
/*!
  Argument handler setting an int variable
*/
typedef SomeSetter<IntTM> IntSetter;

struct FloatTM: public SomeSetterTM<float> {
	static float strToT(const char * val, bool & ok) {
		return QString(val).toFloat(&ok);
	}
	static QString TToStr(const float & t, bool & ok) {
		ok=(t!=-1);
		return QString::number(t);
	}
};
/*!
  Argument handler setting an float variable
*/
typedef SomeSetter<FloatTM> FloatSetter;

struct StrTM: public SomeSetterTM<const char *> {
	static const char * strToT(const char * val, bool & ok) {
		ok=true;
		return val;
	}
	static QString TToStr(const char * t, bool & ok) {
		ok = (t[0] != '\0');
		return QString(t);
	}
};
/*!
  Argument handler setting a string variable
*/
typedef SomeSetter<StrTM> StrSetter;

struct QStrTM: public SomeSetterTM<QString> {
	static QString strToT(const char * val, bool & ok) {
		ok=true;
		return QString::fromLocal8Bit(val);
	}
	static QString TToStr(QString t, bool & ok) {
		ok=!t.isEmpty();
		return t;
	}
};
/*!
  Argument handler setting a string variable
*/
typedef SomeSetter<QStrTM> QStrSetter;

struct UnitRealTM: public SomeSetterTM<QPair<qreal, QPrinter::Unit> > {
	static QPair<qreal, QPrinter::Unit> strToT(const char * val, bool &ok) {
		return Settings::strToUnitReal(val, &ok);
	}
};
/*!
  Argument handler setting a real-number/unit combo variable  
 */
typedef SomeSetter<UnitRealTM> UnitRealSetter;

struct PageSizeTM: public SomeSetterTM<QPrinter::PageSize> {
	static QPrinter::PageSize strToT(const char * val, bool &ok) {
		return Settings::strToPageSize(val, &ok);
	}
};
/*!
  Argument handler setting a page size variable  
 */
typedef SomeSetter<PageSizeTM> PageSizeSetter;

struct ProxyTM: public SomeSetterTM<Settings::ProxySettings> {
	static Settings::ProxySettings strToT(const char * val, bool &ok) {
		return Settings::strToProxy(val, &ok);
	}
};
/*!
  Argument handler setting a proxy variable  
 */
typedef SomeSetter<ProxyTM> ProxySetter;

struct OrientationTM: public SomeSetterTM<QPrinter::Orientation> {
	static QPrinter::Orientation strToT(const char * val, bool &ok) {
		return Settings::strToOrientation(val, &ok);
	}
};
/*!
  Argument handler setting a orientation variable  
 */
typedef SomeSetter<OrientationTM> OrientationSetter;


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
  Lambda: Call the usage method
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
	bool operator()(const char **, CommandLineParserPrivate & p) {
		p.manpage(stdout);
		exit(0);
	}
};

/*!
  Lambda: Call the man method
*/
template <bool T>
struct ReadmeFunc {
	bool operator()(const char **, CommandLineParserPrivate & p) {
		p.readme(stdout, T);
		exit(0);
	}
};

/*!
  Lambda: Call the version method
*/
struct VersionFunc {
	bool operator()(const char **, CommandLineParserPrivate & p) {
		p.version(stdout);
		exit(0);
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
		p.settings.margin.top = Settings::strToUnitReal("2cm");
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
		p.settings.margin.top = Settings::strToUnitReal("2cm");
		return true;
	}
};

/*!
  The next arguments we add will belong to this section
  /param s The name of the section
  /param desc A description of the section
*/
void CommandLineParserPrivate::section(QString s, QString desc) {
	currentSection = s;
	sectionDesc[s] = desc;
	sections.push_back(s);
}

/*!
  Indicate whether the next arguments we add require a patched qt to work
  /param h Do we require a patch
*/
void CommandLineParserPrivate::qthack(bool h) {
	currentHack = h;
}

/*!
  Indicate whether the next arguments we add are "extended" and should not 
  be shown in a simple --help
  \param e Are the arguments extended
*/
void CommandLineParserPrivate::extended(bool e) {
	currentExtended = e;
}

/*!
  Add an argument to the list of arguments
  \param l The long "--" name of the argument
  \param s The short '-' name of the argument or 0 if unspecified
  \param d Description of the argument
  \param h The handler for the argument
  \param display Is the argument hidden
*/
void CommandLineParserPrivate::addarg(QString l, char s, QString d, ArgHandler * h, bool display) {
	h->desc = d;
	h->longName = l;
	h->shortSwitch = s;
	h->display = display;
	h->qthack = currentHack;
	h->extended = currentExtended;
	longToHandler[l] = h;
	if(s) shortToHandler[s] = h;
	sectionArgumentHandles[currentSection].push_back(h);
}

/*!
  Construct the commandline parser adding all the arguments
  \param s The settings to store values in
*/
CommandLineParserPrivate::CommandLineParserPrivate(Settings & s):
	settings(s)
{
	section("General Options");
	extended(false);
	qthack(false);
	addarg("help",'h',"Display help",new Caller<HelpFunc<false> >());
	addarg("quiet",'q',"Be less verbose",new ConstSetter<bool>(s.quiet,true,false));
	addarg("version",'V',"Output version information an exit", new Caller<VersionFunc>());
	addarg("extended-help",0,"Display more extensive help, detailing less common command switches", new Caller<HelpFunc<true> >());
	addarg("collate", 0, "Collate when printing multiple copies", new ConstSetter<bool>(s.collate,true,false));
	addarg("copies", 0, "Number of copies to print into the pdf file", new IntSetter(s.copies, "number", 1));
	addarg("orientation",'O',"Set orientation to Landscape or Portrait", new OrientationSetter(s.orientation, "orientation", QPrinter::Portrait));
	addarg("page-size",'s',"Set paper size to: A4, Letter, etc.", new PageSizeSetter(s.size.pageSize, "size", QPrinter::A4));
	addarg("proxy",'p',"Use a proxy", new ProxySetter(s.proxy, "proxy"));
	addarg("username",0,"HTTP Authentication username", new QStrSetter(s.username, "username",""));
	addarg("password",0,"HTTP Authentication password", new QStrSetter(s.password, "password",""));
	qthack(true);
	addarg("book",'b',"Set the options one would usually set when printing a book", new Caller<BookFunc>());
	addarg("cover",0,"Use html document as cover. It will be inserted before the toc with no headers and footers",new QStrSetter(s.cover,"url",""));
	addarg("default-header",'H',"Add a default header, with the name of the page to the left, and the page number to the right, this is short for: --header-left='[webpage]' --header-right='[page]/[toPage]' --top 2cm --header-line", new Caller<DefaultHeaderFunc>());
	addarg("toc",'t',"Insert a table of content in the beginning of the document", new ConstSetter<bool>(s.printToc,true,false));
	qthack(false);
	
	extended(true);
	addarg("custom-header",0,"Set an additional HTTP header (repeatable)", new MapSetter<>(s.customHeaders, "name", "value"));
	addarg("manpage", 0, "Output program man page", new Caller<ManPageFunc>());
	addarg("htmldoc", 0, "Output program html help", new Caller<ReadmeFunc<true> >());
	addarg("readme", 0, "Output program readme", new Caller<ReadmeFunc<false> >());
	addarg("dpi",'d',"Change the dpi explicitly (this has no effect on X11 based systems)", new IntSetter(s.dpi,"dpi",-1));
	addarg("page-height", 0, "Page height (default unit millimeter)", new UnitRealSetter(s.size.height,"unitreal",QPair<qreal,QPrinter::Unit>(-1,QPrinter::Millimeter)));
	addarg("page-width", 0, "Page width  (default unit millimeter)", new UnitRealSetter(s.size.width,"unitreal",QPair<qreal,QPrinter::Unit>(-1,QPrinter::Millimeter)));
	addarg("disable-javascript",'n',"Do not allow web pages to run javascript", new ConstSetter<bool>(s.enableJavascript,false,true));
	addarg("grayscale",'g',"PDF will be generated in grayscale", new ConstSetter<QPrinter::ColorMode>(s.colorMode,QPrinter::GrayScale,QPrinter::Color));
	addarg("lowquality",'l',"Generates lower quality pdf/ps. Useful to shrink the result document space", new ConstSetter<QPrinter::PrinterMode>(s.resolution,QPrinter::ScreenResolution,QPrinter::HighResolution));
	addarg("margin-bottom",'B',"Set the page bottom margin (default 10mm)", new UnitRealSetter(s.margin.bottom,"unitreal",QPair<qreal,QPrinter::Unit>(10,QPrinter::Millimeter)));
	addarg("margin-left",'L',"Set the page left margin (default 10mm)", new UnitRealSetter(s.margin.left,"unitreal",QPair<qreal,QPrinter::Unit>(10,QPrinter::Millimeter)));
	addarg("margin-right",'R',"Set the page right margin (default 10mm)", new UnitRealSetter(s.margin.right,"unitreal",QPair<qreal,QPrinter::Unit>(10,QPrinter::Millimeter)));
	addarg("margin-top",'T',"Set the page top margin (default 10mm)", new UnitRealSetter(s.margin.top,"unitreal",QPair<qreal,QPrinter::Unit>(10,QPrinter::Millimeter)));
	addarg("redirect-delay",0,"Wait some milliseconds for js-redirects", new IntSetter(s.jsredirectwait,"msec",200));
	addarg("enable-plugins",0,"Enable installed plugins (such as flash", new ConstSetter<bool>(s.enablePlugins,true,false));
	addarg("zoom",0,"Use this zoom factor", new FloatSetter(s.zoomFactor,"float",1.0));
	addarg("read-args-from-stdin",0,"Read command line arguments from stdin", new ConstSetter<bool>(s.readArgsFromStdin,true,false));
	addarg("cookie-jar", 0, "Read and write cookies from and to the supplied cookie jar file", new QStrSetter(s.cookieJar, "path", "") );
	addarg("cookie",0,"Set an additional cookie (repeatable)", new MapSetter<>(s.cookies, "name", "value"));
	addarg("post", 0, "Add an additional post field (repeatable)", new MapSetter<PostItemCreator<false> >(s.post, "name", "value"));
	addarg("post-file", 0, "Post an aditional file (repeatable)", new MapSetter<PostItemCreator<true> >(s.post, "name", "path"));
	addarg("title", 0, "The title of the generated pdf file (The title of the first document is used if not specified)", new QStrSetter(s.documentTitle,"text",""));
	qthack(true);
	addarg("disable-internal-links",0,"Do no make local links", new ConstSetter<bool>(s.useLocalLinks,false,true));
	addarg("disable-external-links",0,"Do no make links to remote web pages", new ConstSetter<bool>(s.useExternalLinks,false,true));
	addarg("print-media-type",0,"Use print media-type instead of screen", new ConstSetter<bool>(s.printMediaType,true,false));
	addarg("page-offset",0,"Set the starting page number", new IntSetter(s.pageOffset,"offset",1));
	addarg("disable-smart-shrinking", 0, "Disable the intelligent shrinking strategy used by WebKit that makes the pixel/dpi ratio none constant",new ConstSetter<bool>(s.enableIntelligentShrinking, false, true));
	addarg("replace",0, "Replace [name] with value in header and footer (repeatable)", new MapSetter<>(s.replacements, "name", "value"));
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
	addarg("debug-javascript", 0,"Show javascript debugging output", new ConstSetter<bool>(s.debugJavascript, true, false));
#if QT_VERSION >= 0x040600
	addarg("stop-slow-scripts", 0, "Stop slow running javascripts", new ConstSetter<bool>(s.stopSlowScripts, true, false));
#endif	
	extended(false);
	section("Headers And Footer Options");
	qthack(true);
	extended(true);
	addarg("footer-center",0,"Centered footer text", new QStrSetter(s.footer.center,"text",""));
	addarg("footer-font-name",0,"Set footer font name", new QStrSetter(s.footer.fontName,"name","Arial"));;
	addarg("footer-font-size",0,"Set footer font size", new IntSetter(s.footer.fontSize,"size",11));
	addarg("footer-left",0,"Left aligned footer text", new QStrSetter(s.footer.left,"text",""));
	addarg("footer-line",0,"Display line above the footer", new ConstSetter<bool>(s.footer.line,true,false));
	addarg("footer-right",0,"Right aligned footer text", new QStrSetter(s.footer.right,"text",""));
	addarg("footer-spacing",0,"Spacing between footer and content in mm", new FloatSetter(s.footer.spacing,"real",0.0));
	addarg("footer-html",0,"Adds a html footer", new QStrSetter(s.footer.htmlUrl,"url",""));
	addarg("header-center",0,"Centered header text", new QStrSetter(s.header.center,"text",""));
	addarg("header-font-name",0,"Set header font name", new QStrSetter(s.header.fontName,"name","Arial"));
	addarg("header-font-size",0,"Set header font size", new IntSetter(s.header.fontSize,"size",11));
	addarg("header-left",0,"Left aligned header text", new QStrSetter(s.header.left,"text",""));
	addarg("header-line",0,"Display line below the header", new ConstSetter<bool>(s.header.line,true,false));
	addarg("header-right",0,"Right aligned header text", new QStrSetter(s.header.right,"text",""));
	addarg("header-spacing",0,"Spacing between header and content in mm", new FloatSetter(s.header.spacing,"real",0.0));
	addarg("header-html",0,"Adds a html header", new QStrSetter(s.header.htmlUrl,"url",""));
	extended(false);
	qthack(false);
	
	section("Table Of Content Options");
	extended(true);
	qthack(true);
	addarg("toc-font-name",0,"Set the font used for the toc", new QStrSetter(s.toc.fontName,"name","Arial"));
	addarg("toc-no-dots",0,"Do not use dots, in the toc", new ConstSetter<bool>(s.toc.useDots,false,true));
	addarg("toc-depth",0,"Set the depth of the toc", new IntSetter(s.toc.depth,"level",3));
	addarg("toc-header-text",0,"The header text of the toc", new QStrSetter(s.toc.captionText,"text","Table Of Contents"));
	addarg("toc-header-fs",0,"The font size of the toc header", new IntSetter(s.toc.captionFontSize,"size",15));
	addarg("toc-disable-links",0,"Do not link from toc to sections", new ConstSetter<bool>(s.toc.forwardLinks,false, true));
	addarg("toc-disable-back-links",0,"Do not link from section header to toc", new ConstSetter<bool>(s.toc.backLinks,false,true));
	for (uint i=0; i < Settings::TOCSettings::levels; ++i) {
		addarg(QString("toc-l")+QString::number(i+1)+"-font-size",0,QString("Set the font size on level ")+QString::number(i+1)+" of the toc",new IntSetter(s.toc.fontSize[i],"size",12-2*i), i < 3);
		addarg(QString("toc-l")+QString::number(i+1)+"-indentation",0,QString("Set indentation on level ")+QString::number(i+1)+" of the toc",new IntSetter(s.toc.indentation[i],"num",i*20), i < 3);
	}
	qthack(true);
	extended(false);

	section("Outline Options");
	extended(true);
	qthack(true);
	addarg("outline",0,"Put an outline into the pdf", new ConstSetter<bool>(s.outline,true,false));
	addarg("outline-depth",0,"Set the depth of the outline", new IntSetter(s.outlineDepth,"level",4));
	qthack(true);
	extended(false);
}
