// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
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
#ifndef __SETTINGS_HH__
#define __SETTINGS_HH__
#include <QString>
#include <QNetworkProxy>
#include <QPrinter>
namespace wkhtmltopdf {
namespace settings {

typedef QPair<qreal, QPrinter::Unit> UnitReal;

/*! \brief Settings consdering proxy */
struct Proxy {
	Proxy();
	//! Type of proxy to use
	QNetworkProxy::ProxyType type; 
	//! The port of the proxy to use
	int port; 
	//! The host name of the proxy to use or NULL
	QString host; 
	//! Username for the said proxy or NULL
	QString user; 
	//! Password for the said proxy or NULL
	QString password; 
};

struct PostItem {
	QString name;
	QString value;
	bool file;
};
	
/*! \brief Settings consdering margins */
struct Margin {
	Margin();
	//!Margin applied to the top of the page
	UnitReal top;
	//!Margin applied to the right of the page
	UnitReal right;
	//!Margin applied to the bottom of the page
	UnitReal bottom;
	//!Margin applied to the leftp of the page
	UnitReal left;
};
	
/*! \brief Settings considering page size */
struct Size {
	Size();
	//! What size paper should we use
	QPrinter::PageSize pageSize; 
	//!Height of the page
	UnitReal height;
	//!Width of the page
	UnitReal width;
};


/*! \brief Class holding all user setting.

    This class holds all the user settings, settings can be filled in by hand,
    or with other methods. 
    \sa CommandLineParser::parse()
*/
struct Global {
	Global();

	/*! \brief Settings considdirng the table of content */
// 	struct TOCSettings {
// 		TOCSettings();
// 		static const uint levels = 7;
// 		//! Should we print dots between the name and the page number?
// 		bool useDots;
// 		//! Maximal depth used in the table of content
// 		int depth;
// 		//! Font used for printing the TOC
// 		QString fontName;
// 		//! Font used for printing the TOC caption
// 		QString captionFontName;
// 		//! Indentation used on a given level
// 		int indentation[levels];
// 		//! Font size used on a given level
// 		int fontSize[levels];
// 		//! Font size used for print the TOC caption
// 		int captionFontSize;
// 		//! Name af the TOC
// 		QString captionText;
// 		//! Link from TOC to section headers
// 		bool forwardLinks;
// 		//! Link from section headers to TOC
// 		bool backLinks;
// 	};

	//! Proxy related settings
	Proxy proxy;

	//! Size related settings
	Size size;

	//! Be less verbose
	bool quiet; 
	
	//! Should we use the graphics system
	bool useGraphics;

	//! Should we orientate in landscape or portrate
	QPrinter::Orientation orientation; 

	//! Color or grayscale
	QPrinter::ColorMode colorMode; 

	//! What overall resolution should we use
	QPrinter::PrinterMode resolution; 

	//! What dpi should be used when printing
	int dpi;
	
	//! When pagenumbers are printed, apply this offset to them all
	//int pageOffset;

	//! How many copies do we wan to print
	int copies;

	//! Should be print a whole copy before beginnig the next
	bool collate;

	//! Should we generate an outline and put it into the pdf file
	bool outline;

	//! Maximal depth of the generated outline
	int outlineDepth;

	//! dump outline to this filename
	QString dumpOutline;

	//! The file where in to store the output
	QString out;

	//! Path of the cookie jar file
	QString cookieJar;
	
	QString documentTitle;

	bool useCompression;

	//! Margin related settings
	Margin margin;	
};

/*! \brief Settings considering headers and footers */
struct HeaderFooter {
	HeaderFooter();
	//! Size of the font used to render the text
	int fontSize;
	//! Name of font used to render text
	QString fontName;
	//! Text to render at the left
	QString left;
	//! Text to render at the right
	QString right;
	//! Text to render at the center
	QString center;
	//! Should a line seperate the header/footer and the document
	bool line;
	//! Url of the document the html document that should be used as a header/footer
	QString htmlUrl;
	//! Spacing
	float spacing;
};


struct Page {
	Page();
	//! Settings regarding the TOC
	//TOCSettings toc;
	
	//! Should we print a table of content
	//bool printToc;
	
	QString page;
	
	//! Header related settings
	HeaderFooter header;
	
	//! Header related settings
	HeaderFooter footer;
	
	//! Username used for http auth login
	QString username;
	
	//! Password used for http auth login
	QString password;
	
	//! Should we print background images
	bool background;
	
	//! Should external links be links in the PDF
	bool useExternalLinks;
	
	//! Should internal links be links in the PDF
	bool useLocalLinks;
	
	//! Should we enable Java Script
	bool enableJavascript; 
	
	//! Should the horrible intelligent shrking feature be enabled?
	bool enableIntelligentShrinking;
	
	//! How many milliseconds should we wait for a javascrit redirect
	int jsredirectwait;
	
	//! What zoom fator should we apply when printing
	float zoomFactor;
	
	//! Minimum font size
	int minimumFontSize;
	
	//! Should we used the print or the screen mediatype
	bool printMediaType;
	
	//! Encoding used to enterpit a document with do supplied encoding
	QString defaultEncoding;
	
	//! Map of custum header valiabels
	QList< QPair<QString, QString> > customHeaders;
	
	//! Map of cookies
	QList< QPair<QString, QString> > cookies;
	
	//! Replacements
	QList< QPair<QString, QString> > replacements;
	
	QList< PostItem > post;
	
	//! Block access to local files for the given page
	bool blockLocalFileAccess;
	
	//! If access to local files is not allowed in general, alow it for these files
	QList< QString > allowed;
	
	//! Stop javascript from running too long
	bool stopSlowScripts;		
	
	//! Output javascript debug messages
	bool debugJavascript;
	
	//! Convert forms on the pages into pdf forms
	bool produceForms;
	
	//! If loading of the page fails continue as if nothing happened
	bool ignoreLoadErrors;
	
	//! Stylesheet supplied by the user
	QString userStyleSheet;
	
	//! Should plugins be allowed
	bool enablePlugins;
};

QPrinter::PageSize strToPageSize(const char * s, bool * ok=0);
QString pageSizeToStr(QPrinter::PageSize ps);

UnitReal strToUnitReal(const char * s, bool * ok=0);
QString unitRealToStr(const UnitReal & ur, bool * ok);
	
Proxy strToProxy(const char * s, bool * ok=0);

QPrinter::Orientation strToOrientation(const char * s, bool * ok=0);
QString orientationToStr(QPrinter::Orientation o);
}
}
#endif //__SETTINGS_HH__
