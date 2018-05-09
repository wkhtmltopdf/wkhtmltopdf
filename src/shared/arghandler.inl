// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010, 2011 wkhtmltopdf authors
//
// This file is part of wkhtmltopdf.
//
// wkhtmltopdf is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// wkhtmltopdf is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with wkhtmltopdf.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __ARGHANDLER_INL__
#define __ARGHANDLER_INL__
#include "commandlineparserbase.hh"
#include <loadsettings.hh>
#include <logging.hh>

template <typename T> class DstArgHandler: public ArgHandler {
public:
 	T & dst;
 	DstArgHandler(T & d): dst(d) {};

 	T & realDst(const CommandLineParserBase & cp, char * page) {
		return * reinterpret_cast<T*>(cp.mapAddress(reinterpret_cast<char *>(&dst), page));
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
	bool operator() (const char **, CommandLineParserBase & cp, char * ps) {
		p_t::realDst(cp, ps)=src;
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
	typedef wkhtmltopdf::settings::PostItem T;
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
struct MapSetter: public DstArgHandler< QList< typename T::T > > {
	typedef DstArgHandler<QList< typename T::T > > p_t;
	MapSetter(QList<typename T::T > & a, QString keyName, QString valueName) : p_t(a) {
		p_t::argn.push_back(keyName);
		p_t::argn.push_back(valueName);
	}
	virtual bool operator() (const char ** args, CommandLineParserBase & cp, char * ps) {
		p_t::realDst(cp, ps).append( T()(args[0], args[1]) );
		return true;
	}
};

struct StringListSetter: public DstArgHandler<QList<QString> > {
	typedef DstArgHandler<QList<QString> > p_t;
	StringListSetter(QList<QString> & a, QString valueName) : p_t (a) {
		p_t::argn.push_back(valueName);
	}
	virtual bool operator() (const char ** args, CommandLineParserBase & cp, char * ps) {
		p_t::realDst(cp, ps).append( args[0] );
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
struct SomeSetter: public DstArgHandler<typename TT::T > {
	typedef DstArgHandler<typename TT::T > p_t;
	typedef typename TT::T T;
	bool hasDef;

	SomeSetter(T & a, QString an, bool def=true): p_t(a), hasDef(def) {
		p_t::argn.push_back(an);
	}

	bool operator() (const char ** vals, CommandLineParserBase & cp, char * ps) {
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

struct LogLevelTM: public SomeSetterTM<wkhtmltopdf::settings::LogLevel> {
	static wkhtmltopdf::settings::LogLevel strToT(const char * val, bool & ok) {
		return wkhtmltopdf::settings::strToLogLevel(val, &ok);
	}
	static QString TToStr(const wkhtmltopdf::settings::LogLevel & l, bool & ok) {
		return wkhtmltopdf::settings::logLevelToStr(l, &ok);
	}
};
/*!
  Argument handler setting an loglevel variable
*/
typedef SomeSetter<LogLevelTM> LogLevelSetter;

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


/*!
  Argument handler responsible for calling a function
*/
template <typename T> struct Caller: public ArgHandler {
	Caller() {}
	Caller(QString a1) {
		argn.push_back(a1);
	}
	bool operator() (const char **vals, CommandLineParserBase & s, char * page) {
		return T()(vals, s, page);
	}
};
#endif //__ARGHANDLER_INL__
