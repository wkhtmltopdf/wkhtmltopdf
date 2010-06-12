/*
 * File:   arguments.cc
 * Author: Christian Sciberras
 * Created: 20 May 2010
 *   This file is part of wkhtmltoimage.
 *   wkhtmltoimage is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *   wkhtmltoimage is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *   You should have received a copy of the GNU General Public License
 *   along with wkhtmltoimage.  If not, see <http://www.gnu.org/licenses/>.
 */

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


struct StringListSetter: public ArgHandler {
	QList<QString> & dst;
	StringListSetter(QList<QString> & a, QString valueName) : dst(a) {
		argn.push_back(valueName);
	}
	virtual bool operator() (const char ** args, CommandLineParserPrivate &) {
		dst.append( args[0] );
		return true;
	}
	virtual void useDefault() {
		dst.clear();
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

struct ProxyTM: public SomeSetterTM<Settings::ProxySettings> {
	static Settings::ProxySettings strToT(const char * val, bool &ok) {
		return Settings::strToProxy(val, &ok);
	}
};
/*!
  Argument handler setting a proxy variable  
 */
typedef SomeSetter<ProxyTM> ProxySetter;

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
	addarg("help",'h',"Display help",new Caller<HelpFunc<false> >());
	addarg("quiet",'q',"Be less verbose",new ConstSetter<bool>(s.quiet,true,false));
	addarg("version",'V',"Output version information an exit", new Caller<VersionFunc>());
	addarg("extended-help",0,"Display more extensive help, detailing less common command switches", new Caller<HelpFunc<true> >());
	addarg("proxy",'p',"Use a proxy", new ProxySetter(s.proxy, "proxy"));
	addarg("username",0,"HTTP Authentication username", new QStrSetter(s.username, "username",""));
	addarg("password",0,"HTTP Authentication password", new QStrSetter(s.password, "password",""));
	extended(true);
	addarg("custom-header",0,"Set an additional HTTP header (repeatable)", new MapSetter<>(s.customHeaders, "name", "value"));
	addarg("manpage", 0, "Output program man page", new Caller<ManPageFunc>());
	addarg("htmldoc", 0, "Output program html help", new Caller<ReadmeFunc<true> >());
	addarg("readme", 0, "Output program readme", new Caller<ReadmeFunc<false> >());
	addarg("disable-javascript",'n',"Do not allow web pages to run javascript", new ConstSetter<bool>(s.enableJavascript,false,true));
	addarg("grayscale",'g',"Image will be generated in grayscale", new ConstSetter<QPrinter::ColorMode>(s.colorMode,QPrinter::GrayScale,QPrinter::Color));
	addarg("redirect-delay",0,"Wait some milliseconds for js-redirects", new IntSetter(s.jsredirectwait,"msec",200));
	addarg("enable-plugins",0,"Enable installed plugins (such as flash)", new ConstSetter<bool>(s.enablePlugins,true,false));

	addarg("scale-w",0,"Set width for resizing", new IntSetter(s.scale.width,"int",-1));
	addarg("scale-h",0,"Set height for resizing", new IntSetter(s.scale.height,"int",-1));

	addarg("crop-x",0,"Set x coordinate for croping", new IntSetter(s.crop.left,"int",0));
	addarg("crop-y",0,"Set y coordinate for croping", new IntSetter(s.crop.top,"int",0));
	addarg("crop-w",0,"Set width for croping", new IntSetter(s.crop.width,"int",-1));
	addarg("crop-h",0,"Set height for croping", new IntSetter(s.crop.height,"int",-1));

	addarg("minimum-font-size",0,"Minimum font size", new IntSetter(s.minimumFontSize,"int",5));
	addarg("cookie-jar", 0, "Read and write cookies from and to the supplied cookie jar file", new QStrSetter(s.cookieJar, "path", "") );
	addarg("cookie",0,"Set an additional cookie (repeatable)", new MapSetter<>(s.cookies, "name", "value"));
	addarg("post", 0, "Add an additional post field (repeatable)", new MapSetter<PostItemCreator<false> >(s.post, "name", "value"));
	addarg("post-file", 0, "Post an aditional file (repeatable)", new MapSetter<PostItemCreator<true> >(s.post, "name", "path"));
	addarg("disallow-local-file-access", 0, "Do not allowed conversion of a local file to read in other local files, unless explecitily allowed with --allow", new ConstSetter<bool>(s.blockLocalFileAccess, true, false));
	addarg("allow", 0, "Allow the file or files from the specified folder to be loaded (repeatable)", new StringListSetter(s.allowed,"path"));
	addarg("format",0,"Sets the image output format (eg: PNG), if not set, it tries to determine it from output file ame", new QStrSetter(s.fmt, "ext"));
	
	addarg("disable-smart-shrinking", 0, "Disable the intelligent shrinking strategy used by WebKit that makes the pixel/dpi ratio none constant",new ConstSetter<bool>(s.enableIntelligentShrinking, false, true));
	addarg("replace",0, "Replace [name] with value in header and footer (repeatable)", new MapSetter<>(s.replacements, "name", "value"));
#ifdef Q_WS_X11
	addarg("use-xserver",0,"Use the X server (some plugins and other stuff might not work without X11)", new ConstSetter<bool>(s.useGraphics,true,false));
#endif

#if QT_VERSION >= 0x040600
	//qthack(false);
#endif
	addarg("encoding",0,"Set the default text encoding, for input", new QStrSetter(s.defaultEncoding,"encoding",""));
	
#if QT_VERSION >= 0x040500 //Not printing the background was added in QT4.5
	addarg("no-background",0,"Do not print background", new ConstSetter<bool>(s.background,false,true));
	addarg("user-style-sheet",0,"Specify a user style sheet, to load with every page", new QStrSetter(s.userStyleSheet,"url",""));
#endif
	addarg("debug-javascript", 0,"Show javascript debugging output", new ConstSetter<bool>(s.debugJavascript, true, false));
#if QT_VERSION >= 0x040600
	addarg("stop-slow-scripts", 0, "Stop slow running javascripts", new ConstSetter<bool>(s.stopSlowScripts, true, false));
#endif	
	extended(false);
}
