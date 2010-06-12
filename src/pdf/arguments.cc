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
#include <QFile>
#include "pageconverter.hh"
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
  \class CommandLineParserPrivate
  Implementation details for CommandLineParser
*/


template <typename T> class DstArgHandler: public ArgHandler {
public:
	T & dst;
	DstArgHandler(T & d): dst(d) {};

	T & realDst(CommandLineParserPrivate & cp, Page & ps) {
		//Very very ugly hack, when the setting is within the page settisgs offset
		//Dummy struct return its location within the supplied page settings
		//The dest is within the fake page offset struct 
		
		char * d = reinterpret_cast<char *>(&dst);
		char * od = reinterpret_cast<char *>(&cp.od);
		if(od > d || d >= od + sizeof(Page)) return dst;
		return * reinterpret_cast<T*>(d - od + reinterpret_cast<char *>(&ps));
	}
};


/*!
  Sets a variable to some constant
*/
template <typename T> class ConstSetter: public DstArgHandler<T> {
public:
	typedef DstArgHandler<T> p_t;
	const T src;
	ConstSetter(T & arg, const T s): p_t(arg), src(s) {};
	bool operator() (const char **, CommandLineParserPrivate & cp, Page & ps) {
		p_t::realDst(cp,ps)=src;
		return true;
	}

	virtual QString getDesc() const {
		if (src != p_t::dst) return p_t::desc;
		return p_t::desc + " (default)";
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
	typedef PostItem T;
	inline T operator()(const QString & key, const QString & value) const {
		T p;
		p.name = key;
		p.value = value;
		p.file = file;
		return p;
	}
};


struct StringListSetter: public DstArgHandler<QList<QString> > {
	typedef DstArgHandler<QList<QString> > p_t;
	StringListSetter(QList<QString> & a, QString valueName) : p_t (a) {
		p_t::argn.push_back(valueName);
	}
	virtual bool operator() (const char ** args, CommandLineParserPrivate & cp, Page & ps) {
		p_t::realDst(cp, ps).append( args[0] );
		return true;
	}
};


/*!
  Putting values into a map
*/
template <typename T=StringPairCreator>
struct MapSetter: public DstArgHandler< QList< typename T::T > > {
	typedef DstArgHandler< QList< typename T::T > > p_t;
	MapSetter(QList<typename T::T > & a, QString keyName, QString valueName) : p_t(a) {
		p_t::argn.push_back(keyName);
		p_t::argn.push_back(valueName);
	}
	virtual bool operator() (const char ** args, CommandLineParserPrivate & cp, Page & ps) {
		p_t::realDst(cp, ps).append( T()(args[0], args[1]) );
		return true;
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
struct SomeSetter: public DstArgHandler< typename TT::T > {
	typedef DstArgHandler< typename TT::T > p_t;
	typedef typename TT::T T;
	bool hasDef;

	SomeSetter(T & a, QString an, bool def=true): p_t(a), hasDef(def) {
		p_t::argn.push_back(an);
	}

	bool operator() (const char ** vals, CommandLineParserPrivate & cp, Page & ps) {
		bool ok;
		p_t::realDst(cp, ps) = TT::strToT(vals[0], ok);
		return ok;
	}

	virtual QString getDesc() const {
		if (!hasDef) return p_t::desc;
		bool ok;
		QString x = TT::TToStr(p_t::dst, ok);
		if (!ok) return p_t::desc;
		return p_t::desc + " (default " + x + ")";
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
	static QString TToStr(const QString & t, bool & ok) {
		ok=!t.isEmpty();
		return t;
	}
};
/*!
  Argument handler setting a string variable
*/
typedef SomeSetter<QStrTM> QStrSetter;

struct UnitRealTM: public SomeSetterTM<UnitReal> {
	static UnitReal strToT(const char * val, bool &ok) {
		return strToUnitReal(val, &ok);
	}
	static QString TToStr(const UnitReal & u, bool & ok) {
		return unitRealToStr(u, &ok);
	}
};
/*!
  Argument handler setting a real-number/unit combo variable  
 */
typedef SomeSetter<UnitRealTM> UnitRealSetter;

struct PageSizeTM: public SomeSetterTM<QPrinter::PageSize> {
	static QPrinter::PageSize strToT(const char * val, bool &ok) {
		return strToPageSize(val, &ok);
	}
	static QString TToStr(const QPrinter::PageSize & s, bool & ok) {
		ok=true;
		return pageSizeToStr(s);
	}
};
/*!
  Argument handler setting a page size variable  
 */
typedef SomeSetter<PageSizeTM> PageSizeSetter;

struct ProxyTM: public SomeSetterTM<Proxy> {
	static Proxy strToT(const char * val, bool &ok) {
		return strToProxy(val, &ok);
	}
};
/*!
  Argument handler setting a proxy variable  
 */
typedef SomeSetter<ProxyTM> ProxySetter;

struct OrientationTM: public SomeSetterTM<QPrinter::Orientation> {
	static QPrinter::Orientation strToT(const char * val, bool &ok) {
		return strToOrientation(val, &ok);
	}
	static QString TToStr(const QPrinter::Orientation & o, bool & ok) {
		ok=true;
		return orientationToStr(o);
	}
};
/*!
  Argument handler setting a orientation variable  
 */
typedef SomeSetter<OrientationTM> OrientationSetter;


struct LoadErrorHandlingTM: public SomeSetterTM<Page::LoadErrorHandling> {
	static Page::LoadErrorHandling strToT(const char * val, bool &ok) {
		return strToLoadErrorHandling(val, &ok);
	}
	static QString TToStr(const Page::LoadErrorHandling & o, bool & ok) {
		ok=true;
		return loadErrorHandlingToStr(o);
	}
};
typedef SomeSetter<LoadErrorHandlingTM> LoadErrorHandlingSetting;


/*!
  Argument handler responsible for calling a function
*/
template <typename T> struct Caller: public ArgHandler {
	Caller() {}
	Caller(QString a1) {
		argn.push_back(a1);
	}
	bool operator() (const char **vals, CommandLineParserPrivate & s, Page & page) {
		return T()(vals, s, page);
	}
};

//All these function would have been lambda function, had C++ supported them, now we are forced to write them here

/*!
  Lambda: Call the usage method
*/
template <bool v>
struct HelpFunc {
	bool operator()(const char **, CommandLineParserPrivate & p, Page &) {
		p.usage(stdout,v);
		exit(0);
	}
};


struct DefaultTocFunc {
	bool operator()(const char **, CommandLineParserPrivate &, Page &) {
		QFile file;
		file.open(stdout, QIODevice::WriteOnly | QIODevice::Text);
		QTextStream stream(&file);
		wkhtmltopdf::settings::TableOfContent toc;
		wkhtmltopdf::dumpDefaultTOCStyleSheet(stream, toc);
		exit(0);
	}
};

/*!
  Lambda: Call the man method
*/
struct ManPageFunc {
	bool operator()(const char **, CommandLineParserPrivate & p, Page &) {
		p.manpage(stdout);
		exit(0);
	}
};

/*!
  Lambda: Call the man method
*/
template <bool T>
struct ReadmeFunc {
	bool operator()(const char **, CommandLineParserPrivate & p, Page &) {
		p.readme(stdout, T);
		exit(0);
	}
};

/*!
  Lambda: Call the version method
*/
struct VersionFunc {
	bool operator()(const char **, CommandLineParserPrivate & p, Page &) {
		p.version(stdout);
		exit(0);
	}
};

/*!
  Set the default header
*/
struct DefaultHeaderFunc {
	bool operator()(const char **, CommandLineParserPrivate & p, Page & page) {
		page.header.left="[webpage]";
		page.header.right="[page]/[toPage]";
		page.header.line=true;
		p.globalSettings.margin.top = strToUnitReal("2cm");
		return true;
	}
};

/*!
  Setup default book mode
*/
struct BookFunc {
	bool operator()(const char **, CommandLineParserPrivate &) {
		//p.settings.header.left="[section]";
		//p.settings.header.right="[page]/[toPage]";
		//p.settings.header.line=true;
		//p.settings.outline = true;
		//p.settings.printToc = true;
		//p.settings.margin.top = Settings::strToUnitReal("2cm");
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

void CommandLineParserPrivate::mode(int m) {
	currentMode = m;
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
	h->section = currentMode;
	longToHandler[l] = h;
	if(s) shortToHandler[s] = h;
	sectionArgumentHandles[currentSection].push_back(h);
}

/*!
  Construct the commandline parser adding all the arguments
  \param s The settings to store values in
*/
CommandLineParserPrivate::CommandLineParserPrivate(Global & s, QList<Page> & ps):
	readArgsFromStdin(false),
	globalSettings(s),
	pageSettings(ps)
{
	section("Global Options");
	mode(global);
	
	extended(false);
	qthack(false);
	addarg("help", 'h', "Display help", new Caller<HelpFunc<false> >());
	addarg("quiet", 'q', "Be less verbose", new ConstSetter<bool>(s.quiet,true));
	addarg("version", 'V' ,"Output version information an exit", new Caller<VersionFunc>());
	
	addarg("extended-help", 'H',"Display more extensive help, detailing less common command switches", new Caller<HelpFunc<true> >());
	addarg("no-collate", 0, "Do not collate when printing multiple copies", new ConstSetter<bool>(s.collate, false));
	addarg("collate", 0, "Collate when printing multiple copies", new ConstSetter<bool>(s.collate, true));

	addarg("copies", 0, "Number of copies to print into the pdf file", new IntSetter(s.copies, "number"));
	addarg("orientation",'O',"Set orientation to Landscape or Portrait", new OrientationSetter(s.orientation, "orientation"));
	addarg("page-size",'s',"Set paper size to: A4, Letter, etc.", new PageSizeSetter(s.size.pageSize, "Size"));

	addarg("grayscale",'g',"PDF will be generated in grayscale", new ConstSetter<QPrinter::ColorMode>(s.colorMode,QPrinter::GrayScale));

	addarg("lowquality",'l',"Generates lower quality pdf/ps. Useful to shrink the result document space", new ConstSetter<QPrinter::PrinterMode>(s.resolution,QPrinter::ScreenResolution));
 	addarg("title", 0, "The title of the generated pdf file (The title of the first document is used if not specified)", new QStrSetter(s.documentTitle,"text"));

	addarg("read-args-from-stdin", 0, "Read command line arguments from stdin", new ConstSetter<bool>(readArgsFromStdin, true) );

	extended(true);
 	qthack(false);
	addarg("output-format",0, "Specify an output format to use pdf or ps, instead of looking at the extention of the output filename", new QStrSetter(s.outputFormat, "format"));

	addarg("margin-bottom",'B',"Set the page bottom margin", new UnitRealSetter(s.margin.bottom,"unitreal"));
 	addarg("margin-left",'L',"Set the page left margin", new UnitRealSetter(s.margin.left,"unitreal"));
 	addarg("margin-right",'R',"Set the page right margin", new UnitRealSetter(s.margin.right,"unitreal"));
 	addarg("margin-top",'T',"Set the page top margin", new UnitRealSetter(s.margin.top,"unitreal"));

 	addarg("manpage", 0, "Output program man page", new Caller<ManPageFunc>());
 	addarg("htmldoc", 0, "Output program html help", new Caller<ReadmeFunc<true> >());
 	addarg("readme", 0, "Output program readme", new Caller<ReadmeFunc<false> >());
 	addarg("dpi",'d',"Change the dpi explicitly (this has no effect on X11 based systems)", new IntSetter(s.dpi,"dpi"));
 	addarg("page-height", 0, "Page height", new UnitRealSetter(s.size.height,"unitreal"));
 	addarg("page-width", 0, "Page width", new UnitRealSetter(s.size.width,"unitreal"));

// 	addarg("book",'b',"Set the options one would usually set when printing a book", new Caller<BookFunc>());
	addarg("cookie-jar", 0, "Read and write cookies from and to the supplied cookie jar file", new QStrSetter(s.cookieJar, "path") );


	extended(true);
 	qthack(true);
	addarg("no-pdf-compression", 0 , "Do not use lossless compression on pdf objects", new ConstSetter<bool>(s.useCompression,false));

 #ifdef Q_WS_X11
 	addarg("use-xserver",0,"Use the X server (some plugins and other stuff might not work without X11)", new ConstSetter<bool>(s.useGraphics,true));
#endif

 	section("Outline Options");
 	extended(true);
 	qthack(true);
	addarg("outline",0,"Put an outline into the pdf", new ConstSetter<bool>(s.outline,true));
 	addarg("no-outline",0,"Do not put an outline into the pdf", new ConstSetter<bool>(s.outline,false));
 	addarg("outline-depth",0,"Set the depth of the outline", new IntSetter(s.outlineDepth,"level"));
 	addarg("dump-outline",0,"Dump the outline to a file",new QStrSetter(s.dumpOutline,"file"));
	addarg("dump-default-toc-xsl",0,"Dump the default TOC xsl style sheet to stdout", new Caller<DefaultTocFunc>());

	section("Page Options");
	mode(page);
 	extended(true);
 	qthack(false);
 	addarg("default-header",0,"Add a default header, with the name of the page to the left, and the page number to the right, this is short for: --header-left='[webpage]' --header-right='[page]/[toPage]' --top 2cm --header-line", new Caller<DefaultHeaderFunc>());
	addarg("proxy",'p',"Use a proxy", new ProxySetter(od.proxy, "proxy"));
 	addarg("username",0,"HTTP Authentication username", new QStrSetter(od.username, "username"));
 	addarg("password",0,"HTTP Authentication password", new QStrSetter(od.password, "password"));
	addarg("load-error-handling", 0, "Specify how to handle pages that fail to load: abort, ignore or skip", new LoadErrorHandlingSetting(od.loadErrorHandling, "handler"));
	addarg("custom-header",0,"Set an additional HTTP header (repeatable)", new MapSetter<>(od.customHeaders, "name", "value"));
	addarg("custom-header-propagation",0,"Add HTTP headers specified by --custom-header for each resource request.", new ConstSetter<bool>(od.repeatCustomHeaders, true));
	addarg("no-custom-header-propagation",0,"Do not add HTTP headers specified by --custom-header for each resource request.", new ConstSetter<bool>(od.repeatCustomHeaders, true));

	addarg("disable-javascript",'n',"Do not allow web pages to run javascript", new ConstSetter<bool>(od.enableJavascript,false));
	addarg("enable-javascript",'n',"Do allow web pages to run javascript", new ConstSetter<bool>(od.enableJavascript,true));
	
	addarg("javascript-delay",0,"Wait some milliseconds for javascript finish", new IntSetter(od.jsdelay,"msec"));
 	addarg("enable-plugins",0,"Enable installed plugins (plugins will likely not work)", new ConstSetter<bool>(od.enablePlugins,true));
 	addarg("disable-plugins",0,"Disable installed plugins", new ConstSetter<bool>(od.enablePlugins,false));

	addarg("minimum-font-size",0,"Minimum font size", new IntSetter(od.minimumFontSize,"int"));
 	addarg("zoom",0,"Use this zoom factor", new FloatSetter(od.zoomFactor,"float",1.0));
 	addarg("cookie",0,"Set an additional cookie (repeatable)", new MapSetter<>(od.cookies, "name", "value"));
 	addarg("post", 0, "Add an additional post field (repeatable)", new MapSetter<PostItemCreator<false> >(od.post, "name", "value"));
 	addarg("post-file", 0, "Post an additional file (repeatable)", new MapSetter<PostItemCreator<true> >(od.post, "name", "path"));
	
	addarg("disable-local-file-access", 0, "Do not allowed conversion of a local file to read in other local files, unless explecitily allowed with --allow", new ConstSetter<bool>(od.blockLocalFileAccess, true));
	addarg("enable-local-file-access", 0, "Allowed conversion of a local file to read in other local files.", new ConstSetter<bool>(od.blockLocalFileAccess, false));
 	addarg("allow", 0, "Allow the file or files from the specified folder to be loaded (repeatable)", new StringListSetter(od.allowed,"path"));

#if QT_VERSION >= 0x040500 //Not printing the background was added in QT4.5
 	addarg("no-background",0,"Do not print background", new ConstSetter<bool>(od.background, false));
 	addarg("background",0,"Do print background", new ConstSetter<bool>(od.background, true));
 	addarg("user-style-sheet",0,"Specify a user style sheet, to load with every page", new QStrSetter(od.userStyleSheet,"url"));
#endif
   	
 	addarg("debug-javascript", 0,"Show javascript debugging output", new ConstSetter<bool>(od.debugJavascript, true));
	addarg("no-debug-javascript", 0,"Do not show javascript debugging output", new ConstSetter<bool>(od.debugJavascript, false));
#if QT_VERSION >= 0x040600
 	addarg("stop-slow-scripts", 0, "Stop slow running javascripts", new ConstSetter<bool>(od.stopSlowScripts, true));
 	addarg("no-stop-slow-scripts", 0, "Do not Stop slow running javascripts", new ConstSetter<bool>(od.stopSlowScripts, true));
 	addarg("no-images",0,"Do not load or print images", new ConstSetter<bool>(od.loadImages, false));
 	addarg("images",0,"Do load or print images", new ConstSetter<bool>(od.loadImages, true));
#endif	

 	extended(true);
 	qthack(true);
	addarg("enable-forms", 0, "Turn HTML form fields into pdf form fields", new ConstSetter<bool>(od.produceForms, true));
	addarg("disable-forms", 0, "Do not turn HTML form fields into pdf form fields", new ConstSetter<bool>(od.produceForms, false));
 	addarg("disable-internal-links",0,"Do not make local links", new ConstSetter<bool>(od.useLocalLinks, false));
 	addarg("enable-internal-links",0,"Make local links", new ConstSetter<bool>(od.useLocalLinks, true));
 	addarg("disable-external-links",0,"Do not make links to remote web pages", new ConstSetter<bool>(od.useExternalLinks, false));
 	addarg("enable-external-links",0,"Make links to remote web pages", new ConstSetter<bool>(od.useExternalLinks, true));
	
	addarg("print-media-type",0,"Use print media-type instead of screen", new ConstSetter<bool>(od.printMediaType,true));
	addarg("no-print-media-type",0,"Do not use print media-type instead of screen", new ConstSetter<bool>(od.printMediaType, false));

	addarg("enable-toc-back-links",0,"Do not link from section header to toc", new ConstSetter<bool>(od.toc.backLinks,true));
	addarg("disable-toc-back-links",0,"Do not link from section header to toc", new ConstSetter<bool>(od.toc.backLinks,true));


// 	addarg("page-offset",0,"Set the starting page number", new IntSetter(s.pageOffset,"offset",1));

 	addarg("disable-smart-shrinking", 0, "Disable the intelligent shrinking strategy used by WebKit that makes the pixel/dpi ratio none constant",new ConstSetter<bool>(od.enableIntelligentShrinking, false));
 	addarg("enable-smart-shrinking", 0, "Enable the intelligent shrinking strategy used by WebKit that makes the pixel/dpi ratio none constant",new ConstSetter<bool>(od.enableIntelligentShrinking, true));

#if QT_VERSION >= 0x040600
 	qthack(false);
#endif
 	addarg("encoding", 0, "Set the default text encoding, for input", new QStrSetter(od.defaultEncoding,"encoding"));

	section("Headers And Footer Options");
 	qthack(true);
 	extended(true);
 	addarg("footer-center",0,"Centered footer text", new QStrSetter(od.footer.center,"text"));
 	addarg("footer-font-name",0,"Set footer font name", new QStrSetter(od.footer.fontName,"name"));;
 	addarg("footer-font-size",0,"Set footer font size", new IntSetter(od.footer.fontSize,"size"));
 	addarg("footer-left",0,"Left aligned footer text", new QStrSetter(od.footer.left,"text"));
 	addarg("footer-line",0,"Display line above the footer", new ConstSetter<bool>(od.footer.line,true));
 	addarg("no-footer-line",0,"Do not display line above the footer", new ConstSetter<bool>(od.footer.line,false));
 	addarg("footer-right",0,"Right aligned footer text", new QStrSetter(od.footer.right,"text"));
 	addarg("footer-spacing",0,"Spacing between footer and content in mm", new FloatSetter(od.footer.spacing,"real"));
 	addarg("footer-html",0,"Adds a html footer", new QStrSetter(od.footer.htmlUrl,"url"));
 	addarg("header-center",0,"Centered header text", new QStrSetter(od.header.center,"text"));
 	addarg("header-font-name",0,"Set header font name", new QStrSetter(od.header.fontName,"name"));
 	addarg("header-font-size",0,"Set header font size", new IntSetter(od.header.fontSize,"size"));
 	addarg("header-left",0,"Left aligned header text", new QStrSetter(od.header.left,"text"));
 	addarg("no-header-line",0,"Display line below the header", new ConstSetter<bool>(od.header.line,false));
 	addarg("header-right",0,"Right aligned header text", new QStrSetter(od.header.right,"text"));
 	addarg("header-spacing",0,"Spacing between header and content in mm", new FloatSetter(od.header.spacing,"real"));
 	addarg("header-html",0,"Adds a html header", new QStrSetter(od.header.htmlUrl,"url"));
	
	addarg("replace",0, "Replace [name] with value in header and footer (repeatable)", new MapSetter<>(od.replacements, "name", "value"));

	section("TOC Options");
	mode(toc);
 	extended(true);
 	qthack(true);
 	addarg("xsl-style-sheet",0,"Use the supplied xsl style sheet for printing the table of content", new QStrSetter(od.tocXsl,"file"));

	addarg("toc-header-text",0,"The header text of the toc", new QStrSetter(od.toc.captionText, "text"));
	addarg("disable-toc-links",0,"Do not link from toc to sections", new ConstSetter<bool>(od.toc.forwardLinks, false));
	addarg("disable-dotted-lines",0,"Do not use dottet lines in the toc", new ConstSetter<bool>(od.toc.useDottedLines,false));
	addarg("toc-text-size-shrink",0,"For each level of headings in the toc the font is scaled by this facter", new FloatSetter(od.toc.fontScale, "real"));
	addarg("toc-level-indentation",0,"For each level of headings in the toc indent by this length", new QStrSetter(od.toc.indentation, "width"));
}
