// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010 wkhtmltopdf authors
//
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

#include "arghandler.inl"
#include "commandlineparserbase.hh"
#include <wkhtmltox/loadsettings.hh>
using namespace wkhtmltopdf::settings;

struct LoadErrorHandlingTM: public SomeSetterTM<LoadPage::LoadErrorHandling> {
	static LoadPage::LoadErrorHandling strToT(const char * val, bool &ok) {
		return strToLoadErrorHandling(val, &ok);
	}
	static QString TToStr(const LoadPage::LoadErrorHandling & o, bool & ok) {
		ok=true;
		return loadErrorHandlingToStr(o);
	}
};
typedef SomeSetter<LoadErrorHandlingTM> LoadErrorHandlingSetting;

struct ProxyTM: public SomeSetterTM<Proxy> {
	static Proxy strToT(const char * val, bool &ok) {
		return strToProxy(val, &ok);
	}
};
/*!
  Argument handler setting a proxy variable
 */
typedef SomeSetter<ProxyTM> ProxySetter;


//All these function would have been lambda function, had C++ supported them, now we are forced to write them here

/*!
  Lambda: Call the usage method
*/
template <bool v>
struct HelpFunc {
	bool operator()(const char **, CommandLineParserBase & p, char *) {
		p.usage(stdout,v);
		exit(0);
	}
};

/*!
  Lambda: Call the man method
*/
struct ManPageFunc {
	bool operator()(const char **, CommandLineParserBase & p, char *) {
		p.manpage(stdout);
		exit(0);
	}
};

/*!
  Lambda: Call the man method
*/
template <bool T>
struct ReadmeFunc {
	bool operator()(const char **, CommandLineParserBase & p, char *) {
		p.readme(stdout, T);
		exit(0);
	}
};

/*!
  Lambda: Call the version method
*/
struct VersionFunc {
	bool operator()(const char **, CommandLineParserBase & p, char *) {
		p.version(stdout);
		exit(0);
	}
};

/*!
  The next arguments we add will belong to this section
  /param s The name of the section
  /param desc A description of the section
*/
void CommandLineParserBase::section(QString s, QString desc) {
	currentSection = s;
	sectionDesc[s] = desc;
	sections.push_back(s);
}

/*!
  Indicate whether the next arguments we add require a patched qt to work
  /param h Do we require a patch
*/
void CommandLineParserBase::qthack(bool h) {
	currentHack = h;
}

void CommandLineParserBase::mode(int m) {
	currentMode = m;
}

/*!
  Indicate whether the next arguments we add are "extended" and should not
  be shown in a simple --help
  \param e Are the arguments extended
*/
void CommandLineParserBase::extended(bool e) {
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
void CommandLineParserBase::addarg(QString l, char s, QString d, ArgHandler * h, bool display) {
	h->desc = d;
	h->longName = l;
	h->shortSwitch = s;
	h->display = display;
	h->qthack = currentHack;
	h->section = currentMode;
	h->extended = currentExtended;
	longToHandler[l] = h;
	if (s) shortToHandler[s] = h;
	sectionArgumentHandles[currentSection].push_back(h);
}

void CommandLineParserBase::addDocArgs() {
	extended(false);
	qthack(false);
	addarg("help", 'h', "Display help", new Caller<HelpFunc<false> >());
	addarg("version", 'V' ,"Output version information an exit", new Caller<VersionFunc>());
	addarg("extended-help", 'H',"Display more extensive help, detailing less common command switches", new Caller<HelpFunc<true> >());

	extended(true);
 	qthack(false);
	addarg("manpage", 0, "Output program man page", new Caller<ManPageFunc>());
 	addarg("htmldoc", 0, "Output program html help", new Caller<ReadmeFunc<true> >());
 	addarg("readme", 0, "Output program readme", new Caller<ReadmeFunc<false> >());
}


void CommandLineParserBase::addGlobalLoadArgs(LoadGlobal & s) {
	extended(true);
	qthack(false);

    addarg("cookie-jar", 0, "Read and write cookies from and to the supplied cookie jar file", new QStrSetter(s.cookieJar, "path") );
}

void CommandLineParserBase::addWebArgs(Web & s) {
	extended(true);
 	qthack(false);

	addarg("enable-plugins",0,"Enable installed plugins (plugins will likely not work)", new ConstSetter<bool>(s.enablePlugins,true));
 	addarg("disable-plugins",0,"Disable installed plugins", new ConstSetter<bool>(s.enablePlugins,false));

	addarg("minimum-font-size",0,"Minimum font size", new IntSetter(s.minimumFontSize,"int"));

#if QT_VERSION >= 0x040500 //Not printing the background was added in QT4.5
 	addarg("user-style-sheet",0,"Specify a user style sheet, to load with every page", new QStrSetter(s.userStyleSheet,"url"));
#endif

#if QT_VERSION >= 0x040600
	addarg("no-images",0,"Do not load or print images", new ConstSetter<bool>(s.loadImages, false));
	addarg("images",0,"Do load or print images", new ConstSetter<bool>(s.loadImages, true));
#endif

	addarg("disable-javascript",'n',"Do not allow web pages to run javascript", new ConstSetter<bool>(s.enableJavascript,false));
	addarg("enable-javascript",'n',"Do allow web pages to run javascript", new ConstSetter<bool>(s.enableJavascript,true));

	extended(true);
 	qthack(true);
#if QT_VERSION >= 0x040600
 	qthack(false);
#endif
 	addarg("encoding", 0, "Set the default text encoding, for input", new QStrSetter(s.defaultEncoding,"encoding"));


}

void CommandLineParserBase::addPageLoadArgs(LoadPage & s) {
	extended(true);
	qthack(false);
	addarg("proxy",'p',"Use a proxy", new ProxySetter(s.proxy, "proxy"));
	addarg("username",0,"HTTP Authentication username", new QStrSetter(s.username, "username"));
	addarg("password",0,"HTTP Authentication password", new QStrSetter(s.password, "password"));
	addarg("load-error-handling", 0, "Specify how to handle pages that fail to load: abort, ignore or skip", new LoadErrorHandlingSetting(s.loadErrorHandling, "handler"));
	addarg("custom-header",0,"Set an additional HTTP header (repeatable)", new MapSetter<>(s.customHeaders, "name", "value"));
	addarg("custom-header-propagation",0,"Add HTTP headers specified by --custom-header for each resource request.", new ConstSetter<bool>(s.repeatCustomHeaders, true));
	addarg("no-custom-header-propagation",0,"Do not add HTTP headers specified by --custom-header for each resource request.", new ConstSetter<bool>(s.repeatCustomHeaders, true));

	addarg("javascript-delay",0,"Wait some milliseconds for javascript finish", new IntSetter(s.jsdelay,"msec"));

	addarg("zoom",0,"Use this zoom factor", new FloatSetter(s.zoomFactor,"float",1.0));
	addarg("cookie",0,"Set an additional cookie (repeatable)", new MapSetter<>(s.cookies, "name", "value"));
	addarg("post", 0, "Add an additional post field (repeatable)", new MapSetter<PostItemCreator<false> >(s.post, "name", "value"));
	addarg("post-file", 0, "Post an additional file (repeatable)", new MapSetter<PostItemCreator<true> >(s.post, "name", "path"));

	addarg("disable-local-file-access", 0, "Do not allowed conversion of a local file to read in other local files, unless explecitily allowed with --allow", new ConstSetter<bool>(s.blockLocalFileAccess, true));
	addarg("enable-local-file-access", 0, "Allowed conversion of a local file to read in other local files.", new ConstSetter<bool>(s.blockLocalFileAccess, false));
	addarg("allow", 0, "Allow the file or files from the specified folder to be loaded (repeatable)", new StringListSetter(s.allowed,"path"));

	addarg("debug-javascript", 0,"Show javascript debugging output", new ConstSetter<bool>(s.debugJavascript, true));
	addarg("no-debug-javascript", 0,"Do not show javascript debugging output", new ConstSetter<bool>(s.debugJavascript, false));
#if QT_VERSION >= 0x040600
	addarg("stop-slow-scripts", 0, "Stop slow running javascripts", new ConstSetter<bool>(s.stopSlowScripts, true));
	addarg("no-stop-slow-scripts", 0, "Do not Stop slow running javascripts", new ConstSetter<bool>(s.stopSlowScripts, true));
#endif
	addarg("run-script", 0, "Run this additional javascript after the page is done loading (repeatable)", new StringListSetter(s.runScript, "js"));

}
