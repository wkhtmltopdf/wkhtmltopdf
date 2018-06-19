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

#include "outputter.hh"
#include "pdfcommandlineparser.hh"
#include <QWebFrame>

#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)

/*!
  Output name and a short description
  \param o The outputter to output to
*/
void PdfCommandLineParser::outputManName(Outputter * o) const {
	o->beginSection("Name");
	o->paragraph("wkhtmltopdf - html to pdf converter");
	o->endSection();
}

/*!
  Output a short synopsis on how to call the command line program
  \param o The outputter to output to
*/
void PdfCommandLineParser::outputSynopsis(Outputter * o) const {
	o->beginSection("Synopsis");
	o->verbatim("wkhtmltopdf [GLOBAL OPTION]... [OBJECT]... <output file>\n");
	o->endSection();

	o->beginSection("Document objects");
	o->beginParagraph();
	o->text("wkhtmltopdf is able to put several objects into the output file, an object is either "
			"a single webpage, a cover webpage or a table of contents.  The objects are put into "
			"the output document in the order they are specified on the command line, options can "
			"be specified on a per object basis or in the global options area. Options from the ");
	o->sectionLink("Global Options");
	o->text(" section can only be placed in the global options area.");
	o->endParagraph();

	o->paragraph("A page objects puts the content of a single webpage into the output document.");
	o->verbatim("(page)? <input url/file name> [PAGE OPTION]...");
	o->beginParagraph();
	o->text("Options for the page object can be placed in the global options and the page "
			"options areas. The applicable options can be found in the ");
	o->sectionLink("Page Options");
	o->text(" and ");
	o->sectionLink("Headers And Footer Options");
	o->text(" sections.");
	o->endParagraph();

	o->paragraph("A cover objects puts the content of a single webpage into the output document, "
				 "the page does not appear in the table of contents, and does not have headers and footers.");
	o->verbatim("cover <input url/file name> [PAGE OPTION]...");
	o->paragraph("All options that can be specified for a page object can also be specified for a cover.");

	o->paragraph("A table of contents object inserts a table of contents into the output document.");
	o->verbatim("toc [TOC OPTION]...");
	o->beginParagraph();
	o->text("All options that can be specified for a page object can also be specified for a toc, "
			"further more the options from the ");
	o->sectionLink("TOC Options");
	o->text(" section can also be applied. The table of contents is generated via XSLT which means "
			"that it can be styled to look however you want it to look. To get an idea of how to "
			"do this you can dump the default xslt document by supplying the --dump-default-toc-xsl, and the outline it works on by supplying --dump-outline, see the ");
	o->sectionLink("Outline Options");
	o->text(" section.");
	o->endParagraph();

	o->endSection();
}


/*!
  Explain what the program does
  \param o The outputter to output to
*/
void PdfCommandLineParser::outputDescripton(Outputter * o) const {
	o->beginSection("Description");
	o->beginParagraph();
	o->text("Converts one or more HTML pages into a PDF document, ");
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	o->text("using wkhtmltopdf patched qt.");
#else
	o->bold("not");
	o->text(" using wkhtmltopdf patched qt.");
#endif
	o->endParagraph();
	o->endSection();
}

/*!
  Add explanation about reduced functionality without patched qt/webkit
  \param o The outputter to output to
  \param sure Is the functionality restricted in this wkhtmltopdf
*/
void PdfCommandLineParser::outputNotPatched(Outputter * o, bool sure) const {
	o->beginSection("Reduced Functionality");
	if (sure)
		o->paragraph("This version of wkhtmltopdf has been compiled against a version of "
					 "QT without the wkhtmltopdf patches. Therefore some features are missing, "
					 "if you need these features please use the static version.");
	else
		o->paragraph("Some versions of wkhtmltopdf are compiled against a version of QT "
					 "without the wkhtmltopdf patches. These versions are missing some features, "
					 "you can find out if your version of wkhtmltopdf is one of these by running wkhtmltopdf --version "
					 "if your version is against an unpatched QT, you can use the static version to get all functionality.");

	o->paragraph("Currently the list of features only supported with patch QT includes:");
	o->beginList();
	o->listItem("Printing more than one HTML document into a PDF file.");
	o->listItem("Running without an X11 server.");
	o->listItem("Adding a document outline to the PDF file.");
	o->listItem("Adding headers and footers to the PDF file.");
	o->listItem("Generating a table of contents.");
	o->listItem("Adding links in the generated PDF file.");
	o->listItem("Printing using the screen media-type.");
	o->listItem("Disabling the smart shrink feature of WebKit.");
	o->endList();
	o->endSection();
}

/*!
  Explain the page breaking is somewhat broken
  \param o The outputter to output to
*/
void PdfCommandLineParser::outputPageBreakDoc(Outputter * o) const {
	o->beginSection("Page Breaking");
	o->paragraph(
		"The current page breaking algorithm of WebKit leaves much to be desired. "
		"Basically WebKit will render everything into one long page, and then cut it up "
		"into pages. This means that if you have two columns of text where one is "
		"vertically shifted by half a line. Then WebKit will cut a line into to pieces "
		"display the top half on one page. And the bottom half on another page. "
		"It will also break image in two and so on.  If you are using the patched version of "
		"QT you can use the CSS page-break-inside property to remedy this somewhat. "
		"There is no easy solution to this problem, until this is solved try organizing "
		"your HTML documents such that it contains many lines on which pages can be cut "
		"cleanly.");
	o->endSection();
}

/*!
  Output documentation about headers and footers
  \param o The outputter to output to
*/
void PdfCommandLineParser::outputHeaderFooterDoc(Outputter * o) const {
	o->beginSection("Footers And Headers");
	o->paragraph("Headers and footers can be added to the document by the --header-* and --footer* "
				 "arguments respectively.  In header and footer text string supplied to e.g. --header-left, "
				 "the following variables will be substituted.");
	o->verbatim(
" * [page]       Replaced by the number of the pages currently being printed\n"
" * [frompage]   Replaced by the number of the first page to be printed\n"
" * [topage]     Replaced by the number of the last page to be printed\n"
" * [webpage]    Replaced by the URL of the page being printed\n"
" * [section]    Replaced by the name of the current section\n"
" * [subsection] Replaced by the name of the current subsection\n"
" * [date]       Replaced by the current date in system local format\n"
" * [isodate]    Replaced by the current date in ISO 8601 extended format\n"
" * [time]       Replaced by the current time in system local format\n"
" * [title]      Replaced by the title of the of the current page object\n"
" * [doctitle]   Replaced by the title of the output document\n"
" * [sitepage]   Replaced by the number of the page in the current site being converted\n"
" * [sitepages]  Replaced by the number of pages in the current site being converted\n"
"\n");
	o->paragraph("As an example specifying --header-right \"Page [page] of [topage]\", "
				 "will result in the text \"Page x of y\" where x is the number of the "
				 "current page and y is the number of the last page, to appear in the upper "
				 "left corner in the document.");
	o->paragraph("Headers and footers can also be supplied with HTML documents. As an example one "
				 "could specify --header-html header.html, and use the following content in header.html:");
	o->verbatim(
"<!DOCTYPE html>\n"
"<html><head><script>\n"
"function subst() {\n"
"    var vars = {};\n"
"    var query_strings_from_url = document.location.search.substring(1).split('&');\n"
"    for (var query_string in query_strings_from_url) {\n"
"        if (query_strings_from_url.hasOwnProperty(query_string)) {\n"
"            var temp_var = query_strings_from_url[query_string].split('=', 2);\n"
"            vars[temp_var[0]] = decodeURI(temp_var[1]);\n"
"        }\n"
"    }\n"
"    var css_selector_classes = ['page', 'frompage', 'topage', 'webpage', 'section', 'subsection', 'date', 'isodate', 'time', 'title', 'doctitle', 'sitepage', 'sitepages'];\n"
"    for (var css_class in css_selector_classes) {\n"
"        if (css_selector_classes.hasOwnProperty(css_class)) {\n"
"            var element = document.getElementsByClassName(css_selector_classes[css_class]);\n"
"            for (var j = 0; j < element.length; ++j) {\n"
"                element[j].textContent = vars[css_selector_classes[css_class]];\n"
"            }\n"
"        }\n"
"    }\n"
"}\n"
"</script></head><body style=\"border:0; margin: 0;\" onload=\"subst()\">\n"
"<table style=\"border-bottom: 1px solid black; width: 100%\">\n"
"  <tr>\n"
"    <td class=\"section\"></td>\n"
"    <td style=\"text-align:right\">\n"
"      Page <span class=\"page\"></span> of <span class=\"topage\"></span>\n"
"    </td>\n"
"  </tr>\n"
"</table>\n"
"</body></html>\n"
"\n"
		);
	o->paragraph("As can be seen from the example, the arguments are sent to the header/footer "
				 "html documents in get fashion.");
	o->endSection();
}

void PdfCommandLineParser::outputTableOfContentDoc(Outputter * o) const {
	o->beginSection("Table Of Contents");
	o->paragraph("A table of contents can be added to the document by adding a toc object "
				 "to the command line. For example:");
	o->verbatim("wkhtmltopdf toc http://qt-project.org/doc/qt-4.8/qstring.html qstring.pdf\n");
	o->paragraph("The table of contents is generated based on the H tags in the input "
				 "documents. First a XML document is generated, then it is converted to "
				 "HTML using XSLT.");
	o->paragraph("The generated XML document can be viewed by dumping it to a file using "
				 "the --dump-outline switch. For example:");
	o->verbatim("wkhtmltopdf --dump-outline toc.xml http://qt-project.org/doc/qt-4.8/qstring.html qstring.pdf\n");
	o->paragraph("The XSLT document can be specified using the --xsl-style-sheet switch. "
				 "For example:");
	o->verbatim("wkhtmltopdf toc --xsl-style-sheet my.xsl http://qt-project.org/doc/qt-4.8/qstring.html qstring.pdf\n");
	o->paragraph("The --dump-default-toc-xsl switch can be used to dump the default "
				 "XSLT style sheet to stdout. This is a good start for writing your "
				 "own style sheet");
	o->verbatim("wkhtmltopdf --dump-default-toc-xsl");
	o->paragraph("The XML document is in the namespace "
				 "\"http://wkhtmltopdf.org/outline\" "
				 "it has a root node called \"outline\" which contains a number of "
				 "\"item\" nodes. An item can contain any number of item. These are the "
				 "outline subsections to the section the item represents. A item node "
				 "has the following attributes:");
	o->beginList();
	o->listItem("\"title\" the name of the section.");
	o->listItem("\"page\" the page number the section occurs on.");
	o->listItem("\"link\" a URL that links to the section.");
	o->listItem("\"backLink\" the name of the anchor the section will link back to.");
	o->endList();

	o->paragraph("The remaining TOC options only affect the default style sheet "
				 "so they will not work when specifying a custom style sheet.");
	o->endSection();
}

/*!
  Output documentation about outlines
  \param o The outputter to output to
*/
void PdfCommandLineParser::outputOutlineDoc(Outputter * o) const {
	o->beginSection("Outlines");
	o->beginParagraph();
	o->text(
		"Wkhtmltopdf with patched qt has support for PDF outlines also known as "
		"book marks, this can be enabled by specifying the --outline switch. "
		"The outlines are generated based on the <h?> tags, for a in-depth "
		"description of how this is done see the ");
	o->sectionLink("Table Of Contents");
	o->text(" section. ");
	o->endParagraph();
	o->paragraph(
		"The outline tree can sometimes be very deep, if the <h?> tags where "
		"spread to generous in the HTML document.  The --outline-depth switch can "
		"be used to bound this.");
	o->endSection();
}

/*!
  Output contact information
  \param o The outputter to output to
*/
void PdfCommandLineParser::outputContact(Outputter * o) const {
	o->beginSection("Contact");
	o->beginParagraph();
	o->text("If you experience bugs or want to request new features please visit ");
	o->link("https://github.com/wkhtmltopdf/wkhtmltopdf/issues");
	o->endParagraph();
	o->endSection();
}

/*!
  Output beginning of the readme
  \param o The outputter to output to
*/
void PdfCommandLineParser::outputDocStart(Outputter * o) const {
	o->beginSection("wkhtmltopdf " STRINGIZE(FULL_VERSION) " Manual");
	o->paragraph("This file documents wkhtmltopdf, a program capable of converting html "
				 "documents into PDF documents.");
	o->endSection();
}

/*!
  Output information on how to use read-args-from-stdin
  \param o The outputter to output to
*/
void PdfCommandLineParser::outputArgsFromStdin(Outputter * o) const {
	o->beginSection("Reading arguments from stdin");
	o->paragraph("If you need to convert a lot of pages in a batch, and you feel that wkhtmltopdf "
				 "is a bit too slow to start up, then you should try --read-args-from-stdin,");
	o->paragraph("When --read-args-from-stdin each line of input sent to wkhtmltopdf on stdin "
				 "will act as a separate invocation of wkhtmltopdf, with the arguments specified "
				 "on the given line combined with the arguments given to wkhtmltopdf");
	o->paragraph("For example one could do the following:");
	o->verbatim("echo \"http://qt-project.org/doc/qt-4.8/qapplication.html qapplication.pdf\" >> cmds\n"
				"echo \"cover google.com http://en.wikipedia.org/wiki/Qt_(software) qt.pdf\" >> cmds\n"
				"wkhtmltopdf --read-args-from-stdin --book < cmds\n");
	o->endSection();
}

/*!
  Output information on how to install
  \param o The outputter to output to
*/
void PdfCommandLineParser::outputInstallation(Outputter * o) const {
	o->beginSection("Installation");
	o->paragraph(
		"There are several ways to install wkhtmltopdf.  You can download a "
		"already compiled binary, or you can compile wkhtmltopdf yourself. "
		"On windows the easiest way to install wkhtmltopdf is to download "
		"the latest installer. On Linux you can download the latest static "
		"binary, however you still need to install some other pieces of "
		"software, to learn more about this read the static version section "
		"of the manual.");
	o->endSection();
}

/*!
  Output documentation about page sizes
  \param o The outputter to output to

*/
void PdfCommandLineParser::outputPageSizes(Outputter * o) const {
	o->beginSection("Page sizes");
	o->beginParagraph();
	o->text("The default page size of the rendered document is A4, but by using the --page-size "
			"option this can be changed to almost anything else, such as: A3, Letter and Legal.  "
			"For a full list of supported pages sizes please see ");
	o->link("http://qt-project.org/doc/qt-4.8/qprinter.html#PaperSize-enum");
	o->text(".");
	o->endParagraph();
	o->paragraph("For a more fine grained control over the page size the "
				 "--page-height and --page-width options may be used");
	o->endSection();
}

/*!
  Output examples on how to use wkhtmltopdf
  \param o The outputter to output to
*/
void PdfCommandLineParser::outputExamples(Outputter * o) const {
	o->beginSection("Examples");
	o->paragraph("This section presents a number of examples of how to invoke wkhtmltopdf.");
	o->paragraph("To convert a remote HTML file to PDF:");
	o->verbatim("wkhtmltopdf http://www.google.com google.pdf\n");
	o->paragraph("To convert a local HTML file to PDF:");
	o->verbatim("wkhtmltopdf my.html my.pdf\n");
	o->paragraph("Produce the eler2.pdf sample file:");
	o->verbatim("wkhtmltopdf -H  http://geekz.co.uk/lovesraymond/archive/eler-highlights-2008 eler2.pdf\n");
	o->paragraph("Printing a book with a table of contents:");
	o->verbatim("wkhtmltopdf -H cover cover.html toc chapter1.html chapter2.html chapter3.html book.pdf\n");
	o->endSection();
}

//  LocalWords:  webkit bool unpatched beginList listItem endList WebKit http
//  LocalWords:  stroustrup wkhtmltopdf commandlineparser hh QWebFrame param px
//  LocalWords:  STRINGIZE outputter const beginSection beginParagraph QString
//  LocalWords:  ifdef endif endParagraph endSection GPLv GPL Truelsen Mário td
//  LocalWords:  Bouthenot PDF CSS username BNF frompage topage webpage toPage
//  LocalWords:  html subst unescape subsubsection getElementsByClassName args
//  LocalWords:  textContent onload readme stdin qapplication pdf cmds google
//  LocalWords:  todo gcc openssl sudo dep libqt gui xorg wget xvf svn linux ps
//  LocalWords:  PageSize enum eler glibc xserver xfonts libssl dev wkhtml cd
//  LocalWords:  nomake opensource xslt
