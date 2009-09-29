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
#ifndef __ARGHANDLERS_INC__
#define __ARGHANDLERS_INC__
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
		return QString(val);
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

#endif //__ARGHANDLERS_INC__
