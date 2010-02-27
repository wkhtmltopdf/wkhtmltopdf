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
#include <QWebFrame>

#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)

/*!
  Output the name and version of the program, and also whether we are using a patched qt
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputName(Outputter * o) const {
	o->beginSection("Name");
	o->paragraph(QString("wkhtmltopdf ")+QString::number(MAJOR_VERSION)+"."+QString::number(MINOR_VERSION)+"."+QString::number(PATCH_VERSION)+(QString(STRINGIZE(BUILD)).isEmpty()?"":" ")+STRINGIZE(BUILD));
	o->endSection();
}


/*!
  Output name and a short description
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputManName(Outputter * o) const {
	o->beginSection("Name");
	o->paragraph("wkhtmltopdf - html to pdf converter");
	o->endSection();
}

/*!
  Output copyright stuff
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputLicense(Outputter * o) const {
	o->beginSection("License");
	o->paragraph("Copyright (C) 2008,2009 Wkhtmltopdf Authors.");
	o->endParagraph();
	o->beginParagraph();
	o->text("License GPLv3+: GNU GPL version 3 or later ");
	o->link("http://gnu.org/licenses/gpl.html");
	o->text(". This is free software: you are free to change and redistribute it. "
			"There is NO WARRANTY, to the extent permitted by law.");
	o->endParagraph();
}

/*!
  Output list of authors
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputAuthors(Outputter * o) const {
	o->beginSection("Authors");
	o->paragraph(
		QString::fromUtf8(
			"Written by Jakob Truelsen. "
			"Patches by Mário Silva, Benoit Garret and Emmanuel Bouthenot."));
	o->endSection();
}

/*!
  Output a short synopsis on how to call the command line program
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputSynopsis(Outputter * o) const {
	o->beginSection("Synopsis");
	o->verbatim("wkhtmltopdf [OPTIONS]... <input file> [More input files] <output file>\n");
	o->endSection();
}

/*!
  Explain what the program does
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputDescripton(Outputter * o) const {
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
void CommandLineParserPrivate::outputNotPatched(Outputter * o, bool sure) const {
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
	o->listItem("Printing more then one HTML document into a PDF file.");
	o->listItem("Running without an X11 server.");
	o->listItem("Adding a document outline to the PDF file.");
	o->listItem("Adding headers and footers to the PDF file.");
	o->listItem("Generating a table of contents.");
	o->listItem("Adding links in the generated PDF file.");
	o->listItem("Printing using the screen media-type.");
	o->listItem("Disabling the smart shrink feature of webkit.");
	o->endList();
	o->endSection();
}

/*!
  Explain the page breaking is somewhat broken
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputPageBreakDoc(Outputter * o) const {
	o->beginSection("Page Breaking");
	o->paragraph(
		"The current page breaking algorithm of WebKit leaves much to be desired. "
		"Basically webkit will render everything into one long page, and then cut it up "
		"into pages. This means that if you have two columns of text where one is "
		"vertically shifted by half a line. Then webkit will cut a line into to pieces "
		"display the top half on one page. And the bottom half on another page. "
		"It will also break image in two and so on.  If you are using the patched version of "
		"QT you can use the CSS page-break-inside property to remedy this somewhat. "
		"There is no easy solution to this problem, until this is solved try organising "
		"your HTML documents such that it contains many lines on which pages can be cut "
		"cleanly.");
	o->beginParagraph();
	o->text("See also: ");
	o->link("http://code.google.com/p/wkhtmltopdf/issues/detail?id=9");
	o->text(", ");
	o->link("http://code.google.com/p/wkhtmltopdf/issues/detail?id=33");
	o->text(" and ");
	o->link("http://code.google.com/p/wkhtmltopdf/issues/detail?id=57");
	o->text(".");
	o->endParagraph();
	o->endSection();
}

/*!
  Output documentation about the proxy settings
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputProxyDoc(Outputter * o) const {
	o->beginSection("Specifying A Proxy");
	o->paragraph(
		"By default proxy information will be read from the environment"
		" variables: proxy, all_proxy and http_proxy, proxy options can"
		" also by specified with the -p switch");
	o->verbatim(
		"<type> := \"http://\" | \"socks5://\"\n"
		"<serif> := <username> (\":\" <password>)? \"@\"\n"
		"<proxy> := \"None\" | <type>? <sering>? <host> (\":\" <port>)?\n");
	o->paragraph("Here are some examples (In case you are unfamiliar with the BNF):");
	o->verbatim("http://user:password@myproxyserver:8080\n"
				"socks5://myproxyserver\n"
				"None\n");
	o->endSection();
}

/*!
  Output documentation about headers and footers
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputHeaderFooterDoc(Outputter * o) const {
	o->beginSection("Footers And Headers");
	o->paragraph("Headers and footers can be added to the document by the --header-* and --footer* "
				 "arguments respectfully.  In header and footer text string supplied to e.g. --header-left, "
				 "the following variables will be substituted.");
	o->verbatim(
" * [page]       Replaced by the number of the pages currently being printed\n"
" * [frompage]   Replaced by the number of the first page to be printed\n"
" * [topage]     Replaced by the number of the last page to be printed\n"
" * [webpage]    Replaced by the URL of the page being printed\n"
" * [section]    Replaced by the name of the current section\n"
" * [subsection] Replaced by the name of the current subsection\n"
"\n");
	o->paragraph("As an example specifying --header-right \"Page [page] of [toPage]\", "
				 "will result in the text \"Page x of y\" where x is the number of the "
				 "current page and y is the number of the last page, to appear in the upper "
				 "left corner in the document.");
	o->paragraph("Headers and footers can also be supplied with HTML documents. As an example one "
				 "could specify --header-html header.html, and use the following content in header.html:");
	o->verbatim(
"<html><head><script>\n"
"function subst() {\n"
"  var vars={};\n"
"  var x=document.location.search.substring(1).split('&');\n"
"  for(var i in x) {var z=x[i].split('=',2);vars[z[0]] = unescape(z[1]);}\n"
"  var x=['frompage','topage','page','webpage','section','subsection','subsubsection'];\n"
"  for(var i in x) {\n"
"    var y = document.getElementsByClassName(x[i]);\n"
"    for(var j=0; j<y.length; ++j) y[j].textContent = vars[x[i]];\n"
"  }\n"
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
	o->paragraph("As can be seen from the example, the arguments are sent to the header/footer html "
				 "documents in get fashion.");
	o->endSection();
}

/*!
  Output documentation about outlines
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputOutlineDoc(Outputter * o) const {
	o->beginSection("Outlines");
	o->paragraph(
		"Wkhtmltopdf with patched qt has support for PDF outlines also known as "
		"book marks, this can be enabled by specifying the --outline switch. "
		"The outlines are generated based on the <h?> tags, for a in-depth "
		"description of how this is done see the \"Table Of Contest\" section. ");
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
void CommandLineParserPrivate::outputContact(Outputter * o) const {
	o->beginSection("Contact");
	o->beginParagraph();
	o->text("If you experience bugs or want to request new features please visit ");
	o->link("http://code.google.com/p/wkhtmltopdf/issues/list");
	o->text(", if you have any problems or comments please feel free to contact me: see ");
	o->link("http://www.madalgo.au.dk/~jakobt/#about");
	o->endParagraph();
	o->endSection();
}

/*!
  Output beginning of the readme
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputDocStart(Outputter * o) const {
	o->beginSection(QString("wkhtmltopdf ")+QString::number(MAJOR_VERSION)+"."+QString::number(MINOR_VERSION)+"."+QString::number(PATCH_VERSION)+(QString(STRINGIZE(BUILD)).isEmpty()?"":" ")+STRINGIZE(BUILD) + " Manual");
	o->paragraph("This file documents wkhtmltopdf, a program capable of converting html "
				 "documents into PDF documents.");
	o->endSection();
}

/*!
  Output information on how to use read-args-from-stdin
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputArgsFromStdin(Outputter * o) const {
	o->beginSection("Reading arguments from stdin");
	o->paragraph("If you need to convert a lot of pages in a batch, and you feel that wkhtmltopdf "
				 "is a bit to slow to start up, then you should try --read-args-from-stdin,");
	o->paragraph("When --read-args-from-stdin each line of input sent to wkhtmltopdf on stdin "
				 "will act as a separate invocation of wkhtmltopdf, with the arguments specified "
				 "on the given line combined with the arguments given to wkhtmltopdf");
	o->paragraph("For example one could do the following:");
	o->verbatim("echo \"http://doc.trolltech.com/4.5/qapplication.html qapplication.pdf\" >> cmds\n"
				"echo \"--cover google.com http://en.wikipedia.org/wiki/Qt_(toolkit) qt.pdf\" >> cmds\n"
				"wkhtmltopdf --read-args-from-stdin --book < cmds\n");
	o->endSection();
}

/*!
  Output information on how to compile
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputCompilation(Outputter * o) const {
	o->beginSection("Compilation");
	o->paragraph("It can happen that the static binary does not work for your system "
				 "for one reason or the other, in that case you might need to compile "
				 "wkhtmltopdf yourself.");
	o->beginParagraph();
	o->bold("GNU/Linux:");
	o->endParagraph();
	o->paragraph("Before compilation you will need to install dependencies: X11, gcc, "
				 "git and openssl. On Debian/Ubuntu this can be done as follows:");
	o->verbatim("sudo apt-get build-dep libqt4-gui libqt4-network libqt4-webkit\n"
				"sudo apt-get install openssl build-essential xorg git-core git-doc libssl-dev\n");
	o->paragraph("On other systems you must use your own package manager, the packages "
				 "might be named differently.");
	o->paragraph("First you must check out the modified version of QT");
	o->verbatim("git clone git://gitorious.org/+wkhtml2pdf/qt/wkhtmltopdf-qt.git wkhtmltopdf-qt");
	o->paragraph("Next you must configure, compile and install QT, note this will take "
				 "quite some time, depending on what arguments you use to configure qt");
	o->verbatim("cd wkhtmltopdf-qt\n"
				"./configure -nomake tools,examples,demos,docs,translations -opensource -prefix ../wkqt\n"
				"make -j3\n"
				"make install\n"
				"cd ..\n");
	o->paragraph("All that is needed now is, to compile wkhtmltopdf.");
	o->verbatim("git clone git://github.com/antialize/wkhtmltopdf.git wkhtmltopdf\n"
				"cd wkhtmltopdf\n"
				"../wkqt/bin/qmake\n"
				"make -j3\n");
	o->paragraph("You show now have a binary called wkhtmltopdf in the currently folder that "
				 "you can use, you can optionally install it by running");
	o->verbatim("make install");

	o->beginParagraph();
	o->bold("Other operative systems and advanced features");
	o->endParagraph();

	o->beginParagraph();
	o->text("If you want more details or want to compile under other operative systems"
			"other then GNU/Linux, please see");
	o->link("http://code.google.com/p/wkhtmltopdf/wiki/compilation");
	o->text(".");
	o->endParagraph();
	o->endSection();
}

/*!
  Output information on how to install
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputInstallation(Outputter * o) const {
	o->beginSection("Installation");
	o->paragraph(
		"There are several ways to install wkhtmltopdf.  You can download a "
		"already compiled binary, or you can compile wkhtmltopdf yourself. "
		"On windows the easiest way to install wkhtmltopdf is to download "
		"the latest installer. On linux you can download the latest static "
		"binary, however you still need to install some other pieces of "
		"software, to learn more about this read the static version section "
		"of the manual.");
	o->endSection(); 
}

/*!
  Output documentation about page sizes
  \param o The outputter to output to

*/
void CommandLineParserPrivate::outputPageSizes(Outputter * o) const {
	o->beginSection("Page sizes");
	o->beginParagraph();
	o->text("The default page size of the rendered document is A4, but using this --page-size option"
			"this can be changed to almost anything else, such as: A3, Letter and Legal.  "
			"For a full list of supported pages sizes please see ");
	o->link("http://doc.trolltech.com/4.6/qprinter.html#PageSize-enum");
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
void CommandLineParserPrivate::outputExampels(Outputter * o) const {
	o->beginSection("Examples");
	o->paragraph("This section presents a number of examples of how to invoke wkhtmltopdf.");
	o->paragraph("To convert a remote HTML file to PDF:");
	o->verbatim("wkhtmltopdf http://www.google.com google.pdf\n");
	o->paragraph("To convert a local HTML file to PDF:");
	o->verbatim("wkhtmltopdf my.html my.pdf\n");
	o->paragraph("You can also convert to PS files if you like:");
	o->verbatim("wkhtmltopdf my.html my.ps\n");
	o->paragraph("Produce the eler2.pdf sample file:");
	o->verbatim("wkhtmltopdf http://geekz.co.uk/lovesraymond/archive/eler-highlights-2008 eler2.pdf -H --outline\n");
	o->endSection();
}

/*!
  Output information on the problems with the static version
  \param o The outputter to output to
*/
void CommandLineParserPrivate::outputStaticProblems(Outputter * o) const {
	o->beginSection("Static version");
	o->beginParagraph();
	o->text("On the wkhtmltopdf website you can download a static version of wkhtmltopdf ");
	o->link("http://code.google.com/p/wkhtmltopdf/downloads/list");
	o->text(". This static binary will work on most systems and comes with a build in patched QT.");
	o->endParagraph();

	o->beginParagraph();
	o->text("Unfortunately the static binary is not particularly static, on Linux it depends "
			"on both glibc and openssl, furthermore you will need to have an xserver installed "
			"but not necessary running. You will need to have different fonts install including "
			"xfonts-scalable (Type1), and msttcorefonts.  See ");
	o->link("http://code.google.com/p/wkhtmltopdf/wiki/static");
	o->text(" for trouble shouting.");
	o->endParagraph();
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
//  LocalWords:  nomake opensource
