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
#ifndef __SETTINGS_HH__
#define __SETTINGS_HH__
#include <QString>
#include <QNetworkProxy>
#include <QPrinter>
/*! \brief Class holding all user setting.

    This class holds all the user settings, settings can be filled in by hand,
    or with other methods. 
    \sa CommandLineParser::parse()
*/
struct Settings {
	/*! \brief Settings considdirng the table of content */
	struct TOCSettings {
		static const uint levels = 7;
		//! Should we print dots between the name and the page number?
		bool useDots;
		//! Maximal depth used in the table of content
		int depth;
		//! Font used for printing the TOC
		QString fontName;
		//! Font used for printing the TOC caption
		QString captionFontName;
		//! Indentation used on a given level
		int indentation[levels];
		//! Font size used on a given level
		int fontSize[levels];
		//! Font size used for print the TOC caption
		int captionFontSize;
		//! Name af the TOC
		QString captionText;
		//! Link from TOC to section headers
		bool forwardLinks;
		//! Link from section headers to TOC
		bool backLinks;
	};

	/*! \brief Settings considering headers and footers */
	struct HeaderFooterSettings {
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

	/*! \brief Settings consdering proxy */
	struct ProxySettings {
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
	struct MarginSettings {
		//!Margin applied to the top of the page
		QPair<qreal, QPrinter::Unit> top;
		//!Margin applied to the right of the page
		QPair<qreal, QPrinter::Unit> right;
		//!Margin applied to the bottom of the page
		QPair<qreal, QPrinter::Unit> bottom;
		//!Margin applied to the leftp of the page
		QPair<qreal, QPrinter::Unit> left;
	};
	
	/*! \brief Settings considering page size */
	struct SizeSettings {
		//! What size paper should we use
		QPrinter::PageSize pageSize; 
		//!Height of the page
		QPair<qreal, QPrinter::Unit> height;
		//!Width of the page
		QPair<qreal, QPrinter::Unit> width;
	};
	
	//! Proxy related settings
	ProxySettings proxy;
	//! Header related settings
	HeaderFooterSettings header;
	//! Header related settings
	HeaderFooterSettings footer;
	//! Margin related settings
	MarginSettings margin;
	//! Size related settings
	SizeSettings size;
	//! Settings regarding the TOC
	TOCSettings toc;

	//! Username used for http auth login
	QString username;
	//! Password used for http auth login
	QString password;
	//! Be less verbose
	bool quiet; 
	//! Should we print background images
	bool background;
	//! Should we print a table of content
	bool printToc;
	//! Should plugins be allowed
	bool enablePlugins;
	//! Should we use the graphics system
	bool useGraphics;
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
	//! Should we orientate in landscape or portrate
	QPrinter::Orientation orientation; 
	//! Color or grayscale
	QPrinter::ColorMode colorMode; 
	//! What overall resolution should we use
	QPrinter::PrinterMode resolution; 
	//! What dpi should be used when printing
	int dpi;
	//! When pagenumbers are printed, apply this offset to them all
	int pageOffset;
	//! How many copies do we wan to print
	int copies;
	//! Should be print a whole copy before beginnig the next
	bool collate;
	//! What zoom fator should we apply when printing
	float zoomFactor;
	//! Minimum font size
	int minimumFontSize;
	//! Should we used the print or the screen mediatype
	bool printMediaType;
	//! Encoding used to enterpit a document with do supplied encoding
	QString defaultEncoding;
	//! Stylesheet supplied by the user
	QString userStyleSheet;
	//! Url of document to use as cover
	QString cover;
	//! Should we generate an outline and put it into the pdf file
	bool outline;
	//! Maximal depth of the generated outline
	int outlineDepth;
	//! dump outline to this filename
	QString dumpOutline;
	//! List of input files
	QList<QString> in;
	//! The file where in to store the output
	QString out;
	//! Map of custum header valiabels
	QList< QPair<QString, QString> > customHeaders;
	//! Should we read arguments from stdin
	bool readArgsFromStdin;
	//! Map of cookies
	QList< QPair<QString, QString> > cookies;
	//! Replacements
	QList< QPair<QString, QString> > replacements;
	//! Path of the cookie jar file
	QString cookieJar;
	
	QList< PostItem > post;
	QString documentTitle;
	bool stopSlowScripts;
	bool debugJavascript;

	static QPrinter::PageSize strToPageSize(const char * s, bool * ok=0);
	static QPair<qreal, QPrinter::Unit> strToUnitReal(const char * s, bool * ok=0);
	static ProxySettings strToProxy(const char * s, bool * ok=0);
	static QPrinter::Orientation strToOrientation(const char * s, bool * ok=0);
};

#endif //__SETTINGS_HH__
