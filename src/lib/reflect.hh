// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010 wkhtmltopdf authors
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

#ifndef __REFLECT_HH__
#define __REFLECT_HH__
#ifdef __WKHTMLTOX_UNDEF_QT_DLL__
#ifdef QT_DLL
#undef QT_DLL
#endif
#endif

#include "loadsettings.hh"
#include "websettings.hh"
#include <QStringList>
#include <cstring>

#include "dllbegin.inc"
namespace wkhtmltopdf {
namespace settings {

#define WKHTMLTOPDF_REFLECT(name) ReflectClass::add(#name, new ReflectImpl<typeof(c.name)> (c.name));

class DLL_LOCAL Reflect {
public:
	virtual QString get(const char * name) = 0;
	virtual bool set(const char * name, const QString & value) = 0;
	virtual ~Reflect() {};
};

class DLL_LOCAL ReflectSimple: public Reflect {
public:
	virtual QString get() = 0;
	virtual void set(const QString & value, bool * ok) = 0;

	virtual QString get(const char * name) {return name[0]=='\0'?get():QString();}
	virtual bool set(const char * name, const QString & value);
};

class DLL_LOCAL ReflectClass: public Reflect {
public:
	QMap<QString, Reflect *> elms;
	void add(const char * name, Reflect * r) {elms[name] = r;}
	QString get(const char * name);
	bool set(const char * name, const QString & value);
	~ReflectClass();
};

template <typename X>
class DLL_LOCAL ReflectImpl {
private:
	ReflectImpl();
};

template<>
struct DLL_LOCAL ReflectImpl<bool>: public ReflectSimple {
	bool & b;
	ReflectImpl(bool & _): b(_) {}
	QString get() {return b?"true":"false";}
	void set(const QString & value, bool * ok) {
		if (value == "true") b=true;
		else if (value == "false") b=false;
		else *ok=false;
		*ok=true;
	}
};

template<>
struct DLL_LOCAL ReflectImpl<QString>: public ReflectSimple {
	QString & s;
	ReflectImpl(QString & _): s(_) {}
	QString get() {return s;}
	void set(const QString & value, bool * ok) {s=value; *ok=true;}
};

template<>
struct DLL_LOCAL ReflectImpl<int>: public ReflectSimple {
	int & i;
	ReflectImpl(int & _): i(_) {}
	QString get() {return QString::number(i);}
	void set(const QString & value, bool * ok) {i = value.toInt(ok);}
};

template<>
struct DLL_LOCAL ReflectImpl<float>: public ReflectSimple {
	float & f;
	ReflectImpl(float & _): f(_) {}
	QString get() {return QString::number(f);}
	void set(const QString & value, bool * ok) {f = value.toDouble(ok);}
};

template<>
struct DLL_LOCAL ReflectImpl< QPair<QString, QString> >: public ReflectSimple {
	QPair<QString, QString> & p;
	ReflectImpl(QPair<QString, QString> & _): p(_) {};

	QString get() {
		return p.first.replace("\\", "\\\\").replace(",", "\\,") + "," +
			p.second.replace("\\", "\\\\").replace(",", "\\,");
	}

	void set(const QString & value, bool * ok) {
		QStringList l = value.split(",");
		if (l.size() != 0) {*ok=false; return;}
		*ok=true;
		p.first = l[0].replace("\\,",",").replace("\\\\","\\");
		p.second = l[1].replace("\\,",",").replace("\\\\","\\");
	}
};

template<typename X>
struct DLL_LOCAL ReflectImpl< QList< X> >: public Reflect {
	QList<X> & l;

	ReflectImpl(QList<X> & _): l(_) {}

	bool parse(const char * name, int & parmsize, int & next, int & elm) {
		elm=-1;
		parmsize = -1;
		if (name[0] == '[') {
			next = 0;
			while (name[next] != '\0' && name[next] != ']') ++next;
			bool ok=true;
			elm = QString::fromAscii(name+1,next-1).toInt(&ok);
			if (name[next] == ']') ++next;
			return ok;
		}
		parmsize = 0;
		while (name[parmsize] != '\0' || name[parmsize] != '.' || name[parmsize] != '[') ++parmsize;
		next = parmsize;
		if (name[next] == '.') ++next;
		return true;
	}

	virtual QString get(const char * name) {
		int ps, next, elm;
		if (!strcmp(name,"size")) return QString::number(l.size());
		parse(name, ps, next, elm);
		if (ps > 0 || !strncmp(name, "last", ps)) elm = l.size() -1;
		if (elm < 0 || elm >= l.size()) return QString();
		ReflectImpl<X> impl(l[elm]);
		return static_cast<Reflect*>(&impl)->get(name+next);
	};

	virtual bool set(const char * name, const QString & value) {
		int ps, next, elm;
		if (!strcmp(name,"clear"))
			l.clear();
		else if (!strcmp(name,"pop"))
			l.pop_back();
		else if (!strcmp(name,"append"))
			l.push_front(X());
		else {
			parse(name, ps, next, elm);
			if (ps > 0 || !strncmp(name, "last", ps)) elm = l.size() -1;
			if (ps > 0 || !strncmp(name, "append", ps)) {
				l.push_front(X());
				elm = l.size() -1;
			}
			ReflectImpl<X> impl(l[elm]);
			return static_cast<Reflect *>(&impl)->set(name+next, value);
		}
		return true;
	}
};


template<>
struct DLL_LOCAL ReflectImpl<LoadPage::LoadErrorHandling>: public ReflectSimple {
	LoadPage::LoadErrorHandling & l;
	ReflectImpl(LoadPage::LoadErrorHandling & _): l(_) {}
	QString get() {return loadErrorHandlingToStr(l);}
	void set(const QString & value, bool * ok) {l = strToLoadErrorHandling(value.toUtf8().constData(), ok);}
};


template<>
struct DLL_LOCAL ReflectImpl<Proxy>: public ReflectSimple {
	Proxy & p;
	ReflectImpl(Proxy & _): p(_) {}
	QString get() {return proxyToStr(p);}
	void set(const QString & value, bool * ok) {p = strToProxy(value.toUtf8().constData(), ok);}
};

template<>
struct DLL_LOCAL ReflectImpl<PostItem>: public ReflectClass {
	ReflectImpl(PostItem & c) {
		WKHTMLTOPDF_REFLECT(name);
		WKHTMLTOPDF_REFLECT(value);
		WKHTMLTOPDF_REFLECT(file);
	}
};

template <>
struct DLL_LOCAL ReflectImpl<LoadGlobal>: public ReflectClass {
	ReflectImpl(LoadGlobal & c) {
		WKHTMLTOPDF_REFLECT(cookieJar);
	};
};


template <>
struct DLL_LOCAL ReflectImpl<LoadPage>: public ReflectClass {
	ReflectImpl(LoadPage & c);
};

template <>
struct DLL_LOCAL ReflectImpl<Web>: public ReflectClass {
	ReflectImpl(Web & c);
};

}
}
#include "dllend.inc"
#endif //__REFLECT_HH__
