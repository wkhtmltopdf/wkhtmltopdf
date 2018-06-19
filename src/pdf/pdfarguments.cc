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

#include "arghandler.inl"
#include "pdfcommandlineparser.hh"
#include <QFile>
#include <qglobal.h>
#include <pdfconverter.hh>

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

using namespace wkhtmltopdf::settings;

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

struct DefaultTocFunc {
	bool operator()(const char **, CommandLineParserBase &, char *) {
		QFile file;
		file.open(stdout, QIODevice::WriteOnly | QIODevice::Text);
		QTextStream stream(&file);
		wkhtmltopdf::settings::TableOfContent toc;
		wkhtmltopdf::dumpDefaultTOCStyleSheet(stream, toc);
		exit(0);
	}
};


/*!
  Set the default header
*/
struct DefaultHeaderFunc {
	bool operator()(const char **, CommandLineParserBase & p, char * page) {
		reinterpret_cast<PdfObject*>(page)->header.left="[webpage]";
		reinterpret_cast<PdfObject*>(page)->header.right="[page]/[topage]";
		reinterpret_cast<PdfObject*>(page)->header.line=true;
		static_cast<PdfCommandLineParser&>(p).globalSettings.margin.top = strToUnitReal("2cm");
		return true;
	}
};

/*!
  Setup default book mode
*/
struct BookFunc {
	bool operator()(const char **, CommandLineParserBase &) {
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
  Construct the commandline parser adding all the arguments
  \param s The settings to store values in
*/
PdfCommandLineParser::PdfCommandLineParser(PdfGlobal & s, QList<PdfObject> & ps):
	readArgsFromStdin(false),
	globalSettings(s),
	pageSettings(ps) {
	section("Global Options");
	mode(global);

	addDocArgs();

	extended(false);
	qthack(false);

	addarg("quiet", 'q', "Be less verbose, maintained for backwards compatibility; Same as using --log-level none", new ConstSetter<LogLevel>(s.logLevel, None));
	addarg("log-level", 0, "Set log level to: none, error, warn or info", new LogLevelSetter(s.logLevel, "level"));

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
	addarg("margin-bottom",'B',"Set the page bottom margin", new UnitRealSetter(s.margin.bottom,"unitreal"));
 	addarg("margin-left",'L',"Set the page left margin", new UnitRealSetter(s.margin.left,"unitreal"));
 	addarg("margin-right",'R',"Set the page right margin", new UnitRealSetter(s.margin.right,"unitreal"));
 	addarg("margin-top",'T',"Set the page top margin", new UnitRealSetter(s.margin.top,"unitreal"));

 	addarg("dpi",'d',"Change the dpi explicitly (this has no effect on X11 based systems)", new IntSetter(s.dpi,"dpi"));
 	addarg("page-height", 0, "Page height", new UnitRealSetter(s.size.height,"unitreal"));
 	addarg("page-width", 0, "Page width", new UnitRealSetter(s.size.width,"unitreal"));

	addGlobalLoadArgs(s.load);

	extended(true);
 	qthack(true);

	addarg("image-quality", 0, "When jpeg compressing images use this quality", new IntSetter(s.imageQuality,"integer"));
	addarg("image-dpi", 0, "When embedding images scale them down to this dpi", new IntSetter(s.imageDPI, "integer"));
	addarg("no-pdf-compression", 0 , "Do not use lossless compression on pdf objects", new ConstSetter<bool>(s.useCompression,false));

#ifdef Q_OS_UNIX
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
 	addarg("default-header",0,"Add a default header, with the name of the page to the left, and the page number to the right, this is short for: --header-left='[webpage]' --header-right='[page]/[toPage]' --top 2cm --header-line", new Caller<DefaultHeaderFunc>());

	addarg("viewport-size", 0, "Set viewport size if you have custom scrollbars or css attribute overflow to emulate window size",new QStrSetter(s.viewportSize,""));
	addWebArgs(od.web);
	extended(true);
 	qthack(false);
 	addarg("no-background",0,"Do not print background", new ConstSetter<bool>(od.web.background, false));
 	addarg("background",0,"Do print background", new ConstSetter<bool>(od.web.background, true));

	extended(true);
 	qthack(true);
	addarg("include-in-outline", 0, "Include the page in the table of contents and outlines", new ConstSetter<bool>(od.includeInOutline, true));
	addarg("exclude-from-outline", 0, "Do not include the page in the table of contents and outlines", new ConstSetter<bool>(od.includeInOutline, false));

	addarg("disable-smart-shrinking", 0, "Disable the intelligent shrinking strategy used by WebKit that makes the pixel/dpi ratio non-constant",new ConstSetter<bool>(od.web.enableIntelligentShrinking, false));
 	addarg("enable-smart-shrinking", 0, "Enable the intelligent shrinking strategy used by WebKit that makes the pixel/dpi ratio non-constant",new ConstSetter<bool>(od.web.enableIntelligentShrinking, true));

	extended(false);
 	qthack(true);
	addarg("print-media-type",0,"Use print media-type instead of screen", new ConstSetter<bool>(od.web.printMediaType,true));
	addarg("no-print-media-type",0,"Do not use print media-type instead of screen", new ConstSetter<bool>(od.web.printMediaType, false));

 	extended(true);
 	qthack(true);
	addarg("enable-forms", 0, "Turn HTML form fields into pdf form fields", new ConstSetter<bool>(od.produceForms, true));
	addarg("disable-forms", 0, "Do not turn HTML form fields into pdf form fields", new ConstSetter<bool>(od.produceForms, false));
 	addarg("disable-internal-links",0,"Do not make local links", new ConstSetter<bool>(od.useLocalLinks, false));
 	addarg("enable-internal-links",0,"Make local links", new ConstSetter<bool>(od.useLocalLinks, true));
 	addarg("disable-external-links",0,"Do not make links to remote web pages", new ConstSetter<bool>(od.useExternalLinks, false));
 	addarg("enable-external-links",0,"Make links to remote web pages", new ConstSetter<bool>(od.useExternalLinks, true));
 	addarg("resolve-relative-links", 0, "Resolve relative external links into absolute links", new ConstSetter<bool>(s.resolveRelativeLinks, true));
 	addarg("keep-relative-links", 0, "Keep relative external links as relative external links", new ConstSetter<bool>(s.resolveRelativeLinks, false));

	addarg("enable-toc-back-links",0,"Link from section header to toc", new ConstSetter<bool>(od.toc.backLinks,true));
	addarg("disable-toc-back-links",0,"Do not link from section header to toc", new ConstSetter<bool>(od.toc.backLinks,false));

	addPageLoadArgs(od.load);
 	addarg("page-offset",0,"Set the starting page number", new IntSetter(s.pageOffset,"offset",1));

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
 	addarg("header-line",0,"Display line below the header", new ConstSetter<bool>(od.header.line,true));
 	addarg("no-header-line",0,"Do not display line below the header", new ConstSetter<bool>(od.header.line,false));
 	addarg("header-right",0,"Right aligned header text", new QStrSetter(od.header.right,"text"));
 	addarg("header-spacing",0,"Spacing between header and content in mm", new FloatSetter(od.header.spacing,"real"));
 	addarg("header-html",0,"Adds a html header", new QStrSetter(od.header.htmlUrl,"url"));

	addarg("replace",0, "Replace [name] with value in header and footer (repeatable)", new MapSetter<>(od.replacements, "name", "value"));

	section("TOC Options");
	mode(toc);
 	extended(true);
 	qthack(true);
 	addarg("xsl-style-sheet",0,"Use the supplied xsl style sheet for printing the table of contents", new QStrSetter(od.tocXsl,"file"));

	addarg("toc-header-text",0,"The header text of the toc", new QStrSetter(od.toc.captionText, "text"));
	addarg("disable-toc-links",0,"Do not link from toc to sections", new ConstSetter<bool>(od.toc.forwardLinks, false));
	addarg("disable-dotted-lines",0,"Do not use dotted lines in the toc", new ConstSetter<bool>(od.toc.useDottedLines,false));
	addarg("toc-text-size-shrink",0,"For each level of headings in the toc the font is scaled by this factor", new FloatSetter(od.toc.fontScale, "real"));
	addarg("toc-level-indentation",0,"For each level of headings in the toc indent by this length", new QStrSetter(od.toc.indentation, "width"));
}
