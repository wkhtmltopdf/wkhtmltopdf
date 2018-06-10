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


/**
 * \file pdf.h
 * \brief Provides C bindings for pdf conversion
 */
#include "pdf_c_bindings_p.hh"
#include "utilities.hh"
#include <QApplication>
#include <QWebFrame>

#include <QHash>

#include "dllbegin.inc"
/**
 * \page pagesettings Setting
 * Settings can be supplied to PDF and image c-bindings using utf-8 encoded strings.
 * This is done by relatively simple reflection on the CropSettings, HeaderFooter, Margin,
 * ImageGlobal, PdfGlobal, PdfObject, Size, and TableOfContent classes.
 *
 * - The \ref wkhtmltopdf_global_settings corresponds to the PdfGlobal class and is documented in \ref pagePdfGlobal.
 * - The \ref wkhtmltopdf_object_settings corresponds to the PdfGlobal class and is documented in \ref pagePdfObject.
 * - The \ref wkhtmltopdf_image_settings corresponds to the ImageGlobal class and is documented in \ref pageImageGlobal.
 *
 * \section pageWeb Web page specific settings
 * The following web page specific settings apply
 * - \b web.background Should we print the background? Must be either "true" or "false".
 * - \b web.loadImages Should we load images? Must be either "true" or "false".
 * - \b web.enableJavascript Should we enable javascript? Must be either "true" or "false".
 * - \b web.enableIntelligentShrinking Should we enable intelligent shrinkng to fit more content
 *      on one page? Must be either "true" or "false". Has no effect for wkhtmltoimage.
 * - \b web.minimumFontSize The minimum font size allowed. E.g. "9"
 * - \b web.printMediaType Should the content be printed using the print media type instead
 *      of the screen media type. Must be either "true" or "false". Has no effect for wkhtmltoimage.
 * - \b web.defaultEncoding What encoding should we guess content is using if they do not
 *      specify it properly? E.g. "utf-8"
 * - \b web.userStyleSheet Url er path to a user specified style sheet.
 * - \b web.enablePlugins Should we enable NS plugins, must be either "true" or "false".
 *      Enabling this will have limited success.
 *
 * \section pageLoad Object Specific loading settings
 * The following settings apply for object loading.
 * - \b load.username The user name to use when loging into a website, E.g. "bart"
 * - \b load.password The password to used when logging into a website, E.g. "elbarto"
 * - \b load.jsdelay The mount of time in milliseconds to wait after a page has done loading until
 *      it is actually printed. E.g. "1200". We will wait this amount of time or until, javascript
 *      calls window.print().
 * - \b load.zoomFactor How much should we zoom in on the content? E.g. "2.2".
 * - \b load.customHeaders TODO
 * - \b load.repertCustomHeaders Should the custom headers be sent all elements loaded instead of
 *       only the main page? Must be either "true" or "false".
 * - \b load.cookies TODO
 * - \b load.post TODO
 * - \b load.blockLocalFileAccess Disallow local and piped files to access other local files. Must
 *      be either "true" or "false".
 * - \b load.stopSlowScript Stop slow running javascript. Must be either "true" or "false".
 * - \b load.debugJavascript Forward javascript warnings and errors to the warning callback.
 *      Must be either "true" or "false".
 * - \b load.loadErrorHandling How should we handle obejcts that fail to load. Must be one of:
 *      - "abort" Abort the conversion process
 *      - "skip" Do not add the object to the final output
 *      - "ignore" Try to add the object to the final output.
 * - \b load.proxy String describing what proxy to use when loading the object.
 * - \b load.runScript TODO
 *
 * \section pageHeaderFooter Header and footer settings
 * The same settings can be applied for headers and footers, here there are explained in
 * terms of the header.
 * - \b header.fontSize The font size to use for the header, e.g. "13"
 * - \b header.fontName The name of the font to use for the header. e.g. "times"
 * - \b header.left The string to print in the left part of the header, note that some sequences
 *      are replaced in this string, see the wkhtmltopdf manual.
 * - \b header.center The text to print in the center part of the header.
 * - \b header.right The text to print in the right part of the header.
 * - \b header.line Whether a line should be printed under the header (either "true" or "false").
 * - \b header.spacing The amount of space to put between the header and the content, e.g. "1.8". Be
 *      aware that if this is too large the header will be printed outside the pdf document. This
 *      can be corrected with the margin.top setting.
 * - \b header.htmlUrl Url for a HTML document to use for the header.
 *
 * \section pagePdfGlobal Pdf global settings
 * The \ref wkhtmltopdf_global_settings structure contains the following settings:
 * - \b size.pageSize  The paper size of the output document, e.g. "A4".
 * - \b size.width The with of the output document, e.g.  "4cm".
 * - \b size.height The height of the output document, e.g. "12in".
 * - \b orientation The orientation of the output document, must be either "Landscape" or "Portrait".
 * - \b colorMode Should the output be printed in color or gray scale, must be either "Color" or "Grayscale"
 * - \b resolution Most likely has no effect.
 * - \b dpi What dpi should we use when printing, e.g. "80".
 * - \b pageOffset A number that is added to all page numbers when printing headers, footers and table of content.
 * - \b copies How many copies should we print?. e.g. "2".
 * - \b collate Should the copies be collated? Must be either "true" or "false".
 * - \b outline Should a outline (table of content in the sidebar) be generated and put into the PDF? Must be either "true" or false".

 * - \b outlineDepth The maximal depth of the outline, e.g. "4".
 * - \b dumpOutline If not set to the empty string a XML representation of the outline is dumped to this file.
 * - \b out The path of the output file, if "-" output is sent to stdout, if empty the output is stored in a buffer.
 * - \b documentTitle The title of the PDF document.
 * - \b useCompression Should we use loss less compression when creating the pdf file? Must be either "true" or "false".
 * - \b margin.top Size of the top margin, e.g. "2cm"
 * - \b margin.bottom Size of the bottom margin, e.g. "2cm"
 * - \b margin.left Size of the left margin, e.g. "2cm"
 * - \b margin.right Size of the right margin, e.g. "2cm"
 * - \b imageDPI The maximal DPI to use for images in the pdf document.
 * - \b imageQuality The jpeg compression factor to use when producing the pdf document, e.g. "92".
 * - \b load.cookieJar Path of file used to load and store cookies.
 *
 * \section pagePdfObject Pdf object settings
 * The \ref wkhtmltopdf_object_settings structure contains the following settings:
 * - \b toc.useDottedLines Should we use a dotted line when creating a table of content?
 *      Must be either "true" or "false".
 * - \b toc.captionText The caption to use when creating a table of content.
 * - \b toc.forwardLinks Should we create links from the table of content into the actual content?
 *      Must be either "true or "false.
 * - \b toc.backLinks Should we link back from the content to this table of content.
 * - \b toc.indentation The indentation used for every table of content level, e.g. "2em".
 * - \b toc.fontScale How much should we scale down the font for every toc level? E.g. "0.8"
 * - \b page The URL or path of the web page to convert, if "-" input is read from stdin.
 * - \b header.* Header specific settings see \ref pageHeaderFooter.
 * - \b footer.* Footer specific settings see \ref pageHeaderFooter.
 * - \b useExternalLinks Should external links in the HTML document be converted into
 *      external pdf links? Must be either "true" or "false.
 * - \b useLocalLinks Should internal links in the HTML document be converted into pdf
 *      references? Must be either "true" or "false"
 * - \b replacements TODO
 * - \b produceForms Should we turn HTML forms into PDF forms? Must be either "true" or file".
 * - \b load.* Page specific settings related to loading content, see \ref pageLoad.
 * - \b web.* See \ref pageWeb.
 * - \b includeInOutline Should the sections from this document be included in the outline and table of content?
 * - \b pagesCount Should we count the pages of this document, in the counter used for TOC, headers and footers?
 * - \b tocXsl If not empty this object is a table of content object, "page" is ignored and this xsl style
 *      sheet is used to convert the outline XML into a table of content.
 */

/**
 * \struct wkhtmltopdf_global_settings
 * \brief A struct holding global settings
 *
 * See also \ref pagePdfGlobal
 */

/**
 * \struct wkhtmltopdf_object_settings
 * \brief A struct holding object settings
 *
 * See also \ref pagePdfObject
 */

/**
 * \struct wkhtmltopdf_converter
 * \brief A struct holding information related to a conversion process
 */

/**
 * \typedef wkhtmltopdf_str_callback
 * \brief Function pointer type used for the error and warning callbacks
 *
 * \param converter The converter that issued the callback
 * \param str A utf8 encoded string containing the error or warning message.
 *
 * \sa wkhtmltopdf_set_error_callback, wkhtmltopdf_set_warning_callback
 */

/**
 * \typedef wkhtmltopdf_int_callback
 * \brief Function pointer type used for the progress_changed and finished callbacks
 *
 * For the progress_changed callback the value indicated the progress
 * within the current phase in percent. For the finished callback the value
 * if 1 if the conversion has successful and 0 otherwise.
 *
 * \param converter The converter that issued the callback
 * \param val The integer value
 *
 * \sa wkhtmltopdf_set_progress_changed, wkhtmltopdf_set_finished_callback
 */

/**
 * \typedef wkhtmltopdf_void_callback
 * \brief Function pointer type used for the phase_changed callback
 *
 * \param converter The converter that issued the callback
 *
 * \sa wkhtmltopdf_set_phase_changed_callback
 */


using namespace wkhtmltopdf;
QApplication * a = 0;
int usage = 0;

void MyPdfConverter::warning(const QString & message) {
	if (warning_cb && globalSettings->logLevel > settings::Error) (warning_cb)(reinterpret_cast<wkhtmltopdf_converter*>(this), message.toUtf8().constData());
}

void MyPdfConverter::error(const QString & message) {
	if (error_cb && globalSettings->logLevel > settings::None) (error_cb)(reinterpret_cast<wkhtmltopdf_converter*>(this), message.toUtf8().constData());
}

void MyPdfConverter::phaseChanged() {
	if (phase_changed) (phase_changed)(reinterpret_cast<wkhtmltopdf_converter*>(this));
}

void MyPdfConverter::progressChanged(int progress) {
	if (progress_changed) (progress_changed)(reinterpret_cast<wkhtmltopdf_converter*>(this), progress);
}

void MyPdfConverter::finished(bool ok) {
	if (finished_cb) (finished_cb)(reinterpret_cast<wkhtmltopdf_converter*>(this), ok);
}

MyPdfConverter::MyPdfConverter(settings::PdfGlobal * gs):
	warning_cb(0), error_cb(0), phase_changed(0), progress_changed(0), finished_cb(0),
	converter(*gs), globalSettings(gs) {

    connect(&converter, SIGNAL(warning(const QString &)), this, SLOT(warning(const QString &)));
	connect(&converter, SIGNAL(error(const QString &)), this, SLOT(error(const QString &)));
	connect(&converter, SIGNAL(phaseChanged()), this, SLOT(phaseChanged()));
	connect(&converter, SIGNAL(progressChanged(int)), this, SLOT(progressChanged(int)));
	connect(&converter, SIGNAL(finished(bool)), this, SLOT(finished(bool)));
}

MyPdfConverter::~MyPdfConverter() {
	delete globalSettings;
	for (size_t i=0; i < objectSettings.size(); ++i)
		delete objectSettings[i];
	objectSettings.clear();
}


/**
 * \brief Check if the library is build against the wkhtmltopdf version of QT
 *
 * \return 1 if the library was build against the wkhtmltopdf version of QT and 0 otherwise
 */
CAPI(int) wkhtmltopdf_extended_qt() {
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	return 1;
#else
	return 0;
#endif
}

#define STRINGIZEE(a) # a
#define STRINGIZE(a) STRINGIZEE(a)

/**
 * \brief Return the version of wkhtmltopdf
 * Example: 0.12.1-development. The string is utf8 encoded and is owned by wkhtmltopdf.
 *
 * \return Qt version
 */
CAPI(const char *) wkhtmltopdf_version() {
	return STRINGIZE(FULL_VERSION);
}

/**
 * \brief Setup wkhtmltopdf
 *
 * Must be called before any other functions.
 *
 * \param use_graphics Should we use a graphics system
 * \returns 1 on success and 0 otherwise
 *
 * \sa wkhtmltopdf_deinit
 */
CAPI(int) wkhtmltopdf_init(int use_graphics) {
	++usage;

	if (qApp == 0) {
		char x[256];
		strcpy(x, "wkhtmltox");
		char * arg[] = {x, 0};
		int aa = 1;

#if QT_VERSION >= 0x050000 && defined(Q_OS_UNIX) && !defined(__EXTENSIVE_WKHTMLTOPDF_QT_HACK__)
		setenv("QT_QPA_PLATFORM", "offscreen", 0);
#endif

		bool ug = true;
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
#ifdef __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
		ug = use_graphics;
		if (!ug) QApplication::setGraphicsSystem("raster");
#else
		Q_UNUSED(use_graphics);
#endif
#else
		Q_UNUSED(use_graphics);
#endif
		a = new QApplication(aa, arg, ug);
		a->setApplicationName(x);
		MyLooksStyle * style = new MyLooksStyle();
		a->setStyle(style);
	}
	return 1;
}

/**
 * \brief Deinit wkhtmltopdf
 *
 * Free up resources used by wkhtmltopdf, when this has been called no other
 * wkhtmltopdf function can be called.
 *
 * \returns 1 on success and 0 otherwise
 *
 * \sa wkhtmltopdf_init
 */
CAPI(int) wkhtmltopdf_deinit() {
	--usage;
	if (usage != 0) return 1;
	if (a != 0) delete a;
	return 1;
}

/**
 * \brief Create a new global settings object for pdf conversion
 *
 * Create a new global settings object for pdf conversion, settings can be altered with
 * \ref wkhtmltopdf_set_global_setting, and inspected with \ref wkhtmltopdf_get_global_setting.
 * Once the desired settings have been set a converter object can be created using \reg wkhtmltopdf_create_converter.
 *
 * \returns A wkhtmltopdf global settings object
 */
CAPI(wkhtmltopdf_global_settings *) wkhtmltopdf_create_global_settings() {
	return reinterpret_cast<wkhtmltopdf_global_settings *>(new settings::PdfGlobal());
}

/**
 * \brief Destroy a global settings  object
 *
 * Normally one would not need to call this since ownership of the
 * settings object is transfarred to the converter.
 */
CAPI(void) wkhtmltopdf_destroy_global_settings(wkhtmltopdf_global_settings * obj) {
	delete reinterpret_cast<settings::PdfGlobal *>(obj);
}

/**
 * \brief Alter a setting in a global settings object
 *
 * \sa \ref pagePdfGlobal, wkhtmltopdf_create_global_settings, wkhtmltopdf_get_global_setting
 *
 * \param settings The settings object to change
 * \param name The name of the setting
 * \param value The new value for the setting (encoded in UTF-8)
 * \returns 1 if the setting was updated successfully and 0 otherwise.
 */
CAPI(int) wkhtmltopdf_set_global_setting(wkhtmltopdf_global_settings * settings, const char * name, const char * value) {
	return reinterpret_cast<settings::PdfGlobal *>(settings)->set(name, QString::fromUtf8(value));
}

/**
 * \brief Retrieve a setting in a global settings object
 *
 * \sa \ref pagesettings, wkhtmltopdf_create_global_settings, wkhtmltopdf_set_global_setting
 *
 * \param settings The settings object to inspect
 * \param name The name of the setting to read
 * \param value A buffer of length at least \a vs, where the value (encoded in UTF-8) is stored.
 * \param vs The length of \a value
 * \returns 1 If the the setting exists and was read successfully and 0 otherwise
 */
CAPI(int) wkhtmltopdf_get_global_setting(wkhtmltopdf_global_settings * settings, const char * name, char * value, int vs) {
	QString res = reinterpret_cast<settings::PdfGlobal *>(settings)->get(name);
	if (res.isNull()) return 0;
	qstrncpy(value, res.toUtf8().constData(), vs);
	return 1;
}

/**
 * \brief Create an object used to store object settings
 *
 * Create a new Object settings object for pdf conversion, settings can be altered with
 * \ref wkhtmltopdf_set_object_setting, and inspected with \ref wkhtmltopdf_get_object_setting.
 * Once the desired settings have been set the object can be added to a converter
 * by calling wkhtmltopdf_add_resource.
 *
 * \returns an object settings instance
 */
CAPI(wkhtmltopdf_object_settings *) wkhtmltopdf_create_object_settings() {
	return reinterpret_cast<wkhtmltopdf_object_settings *>(new settings::PdfObject());
}

/**
 * \brief Destroy a global settings  object
 *
 * Normally one would not need to call this since ownership of the
 * settings object is transfarred to the converter.
 */
CAPI(void) wkhtmltopdf_destroy_object_settings(wkhtmltopdf_object_settings * obj) {
	delete reinterpret_cast<settings::PdfObject *>(obj);
}


/**
 * \brief Alter a setting in a object settings object
 *
 * \sa \ref pagesettings, wkhtmltopdf_create_object_settings, wkhtmltopdf_get_object_setting
 *
 * \param settings The settings object to change
 * \param name The name of the setting
 * \param value The new value for the setting (encoded in UTF-8)
 * \returns 1 if the setting was updated successfully and 0 otherwise.
 */
CAPI(int) wkhtmltopdf_set_object_setting(wkhtmltopdf_object_settings * settings, const char * name, const char * value) {
	return reinterpret_cast<settings::PdfObject *>(settings)->set(name, QString::fromUtf8(value));
}

/**
 * \brief Retrieve a setting in a object settings object
 *
 * \sa \ref pagesettings, wkhtmltopdf_create_global_settings, wkhtmltopdf_set_global_setting
 *
 * \param settings The settings object to inspect
 * \param name The name of the setting to read
 * \param value A buffer of length at least \a vs, where the value is stored (encoded in UTF-8).
 * \param vs The length of \a value
 * \returns 1 If the the setting exists and was read successfully and 0 otherwise
 */
CAPI(int) wkhtmltopdf_get_object_setting(wkhtmltopdf_object_settings * settings, const char * name, char * value, int vs) {
	QString res = reinterpret_cast<settings::PdfObject *>(settings)->get(name);
	if (res.isNull()) return 0;
	qstrncpy(value, res.toUtf8().constData(), vs);
	return 1;
}

/**
 * \brief Create a wkhtmltopdf converter object
 *
 * The converter object is used to convert one or more objects(web sides) into a single pdf.
 * Once a settings object has been parsed, it may no longer be accessed, and will eventually be freed.
 * The returned converter object must be freed by calling \ref wkhtmltopdf_destroy_converter
 *
 * \param settings The global settings to use during conversion.
 * \returns A wkhtmltopdf converter object
 */
CAPI(wkhtmltopdf_converter *) wkhtmltopdf_create_converter(wkhtmltopdf_global_settings * settings) {

	return reinterpret_cast<wkhtmltopdf_converter *>(
		new MyPdfConverter(reinterpret_cast<settings::PdfGlobal *>(settings)));
}

/**
 * \brief Destroy a wkhtmltopdf converter object
 *
 * An object must be destroyed to free up its memory, once it has been destroyed it may no longer
 * be accessed.
 *
 * \param settings The converter object to destroy
 */
CAPI(void) wkhtmltopdf_destroy_converter(wkhtmltopdf_converter * converter) {
	reinterpret_cast<MyPdfConverter *>(converter)->deleteLater();
}

/**
 * \brief Set the function that should be called when an warning message is issued during conversion
 *
 * \param converter The converter object on which warnings we want the callback to be called
 * \param cb The function to call when warning message is issued
 *
 */
CAPI(void) wkhtmltopdf_set_warning_callback(wkhtmltopdf_converter * converter, wkhtmltopdf_str_callback cb) {
	reinterpret_cast<MyPdfConverter *>(converter)->warning_cb = cb;
}

/**
 * \brief Set the function that should be called when an errors occurs during conversion
 *
 * \param converter The converter object on which errors we want the callback to be called
 * \param cb The function to call when an error occurs
 */
CAPI(void) wkhtmltopdf_set_error_callback(wkhtmltopdf_converter * converter, wkhtmltopdf_str_callback cb) {
	reinterpret_cast<MyPdfConverter *>(converter)->error_cb = cb;
}

/**
 * \brief Set the function that should be called whenever conversion changes phase
 *
 * The number of the new phase can be found by calling \ref wkhtmltopdf_current_phase
 *
 * \param converter The converter which phase change events to call back from
 * \param cb The function to call when phases change
 *
 * \sa wkhtmltopdf_current_phase, wkhtmltopdf_phase_count, wkhtmltopdf_phase_description
 */
CAPI(void) wkhtmltopdf_set_phase_changed_callback(wkhtmltopdf_converter * converter, wkhtmltopdf_void_callback cb) {
	reinterpret_cast<MyPdfConverter *>(converter)->phase_changed = cb;
}

/**
 * \brief Set the function that should be called when progress have been done during conversion.
 *
 * The progress in percent within the current phase is given as an integer to the callback function.
 *
 * \param converter The converter which progress events to call back from
 * \param cb The function to call when progress has occurred.
 *
 * \sa wkhtmltopdf_progress_description
 */
CAPI(void) wkhtmltopdf_set_progress_changed_callback(wkhtmltopdf_converter * converter, wkhtmltopdf_int_callback cb) {
	reinterpret_cast<MyPdfConverter *>(converter)->progress_changed = cb;
}

/**
 * \brief Set the function that should be called once the conversion has finished.
 *

 * \param converter The converter which finish events to call back from
 * \param cb The function to call when the conversion has finished has occurred.
 *
 * \sa wkhtmltopdf_convert
 */
CAPI(void) wkhtmltopdf_set_finished_callback(wkhtmltopdf_converter * converter, wkhtmltopdf_int_callback cb) {
	reinterpret_cast<MyPdfConverter *>(converter)->finished_cb = cb;
}

//CAPI(void) wkhtmltopdf_begin_conversion(wkhtmltopdf_converter * converter) {
//	reinterpret_cast<MyPdfConverter *>(converter)->converter.beginConversion();
//}

/**
 * \brief Convert the input objects into a pdf document
 *
 * This is the main method for the conversion process, during conversion progress information
 * warning, and errors are reported using the supplied call backs. Once the conversion is done
 * the output pdf (or ps) file will be placed at the location of the "out" setting supplied in
 * the global settings object during construction of the converter. If this setting is not supplied
 * or set to the empty string, the output can be retrieved using the \ref wkhtmltopdf_get_output
 * function.
 *
 * \paragm converter The converter to perform the conversion on.
 *
 * \returns 1 on success and 0 otherwise
 */
CAPI(int) wkhtmltopdf_convert(wkhtmltopdf_converter * converter) {
	return reinterpret_cast<MyPdfConverter *>(converter)->converter.convert();
}

//CAPI(void) wkhtmltopdf_cancel(wkhtmltopdf_converter * converter) {
//	reinterpret_cast<MyPdfConverter *>(converter)->converter.cancel();
//}

/**
 * \brief add an object (web page to convert)
 *
 * Add the object described by the supplied object settings to the list of objects (web pages to convert),
 * objects are placed in the output pdf in the order of addition. Once the object has been added, the
 * supplied settings may no longer be accessed, it Wit eventually be freed by wkhtmltopdf.
 * If a none NULL and none empty utf8 encoded string is supplied to data, this HTML content will be converted
 * instead of the content located at  "page" setting of the supplied object settings instance.
 *
 * \param converter The converter to add the object to
 * \param settings The setting describing the object to add
 * \param data HTML content of the object to convert (encoded in UTF-8) or NULL
 */
CAPI(void) wkhtmltopdf_add_object(wkhtmltopdf_converter * converter, wkhtmltopdf_object_settings * settings, const char * data) {
	QString str= QString::fromUtf8(data);
	reinterpret_cast<MyPdfConverter *>(converter)->converter.addResource(
		*reinterpret_cast<settings::PdfObject *>(settings), &str);
	reinterpret_cast<MyPdfConverter *>(converter)->objectSettings.push_back(reinterpret_cast<settings::PdfObject *>(settings));
}

/**
 * \brief Get the number of the current conversion phase
 *
 * Conversion is done in a number of named phases, this
 * function will retrieve the number of the current conversion phase,
 * which will be a number between 0 and wkhtmltopdf_phase_count(converter)-1.
 *
 * The description (name) of any phase can be retrieved by calling the
 * \ref wkhtmltopdf_phase_description method.
 *
 * \param converter The converter to find the current phase of
 * \returns The current phase of the supplied converter
 */
CAPI(int) wkhtmltopdf_current_phase(wkhtmltopdf_converter * converter) {
	return reinterpret_cast<MyPdfConverter *>(converter)->converter.currentPhase();
}

/**
 * \brief Get the total number of phases the conversion process will go trough
 *
 * \param converter The converter to query
 * \returns The total number of phases in the conversion process
 *
 * \sa wkhtmltopdf_current_phase, wkhtmltopdf_phase_description
 */
CAPI(int) wkhtmltopdf_phase_count(wkhtmltopdf_converter * converter) {
	return reinterpret_cast<MyPdfConverter *>(converter)->converter.phaseCount();
}


/**
 * \brief Return a short utf8 description of a conversion phase
 *
 * \param converter The converter to query
 * \param phase The number of the conversion step of which we want a description
 * \returns A description of the conversion phase
 *
 * \sa wkhtmltopdf_current_phase, wkhtmltopdf_phase_description
 */
CAPI(const char *) wkhtmltopdf_phase_description(wkhtmltopdf_converter * converter, int phase) {
  MyPdfConverter* conv = reinterpret_cast<MyPdfConverter *>(converter);
	QString pd = conv->converter.phaseDescription(phase);
	if (!conv->utf8StringCache.contains(pd))
	{
		return conv->utf8StringCache.insert(pd, pd.toUtf8()).value().constData();
	}
	else
		return conv->utf8StringCache[pd].constData();
}

/**
 * \brief Return a short utf8 string indicating progress within a phase
 *
 * Will typically return a string like "40%"
 *
 * \param converter The converter to query
 * \returns A string containing a progress indication
 *
 * \sa wkhtmltopdf_set_progress_changed_callback
 */
CAPI(const char *) wkhtmltopdf_progress_string(wkhtmltopdf_converter * converter) {
  MyPdfConverter* conv = reinterpret_cast<MyPdfConverter *>(converter);
	QString ps = conv->converter.progressString();
	if (!conv->utf8StringCache.contains(ps))
		return conv->utf8StringCache.insert(ps, ps.toUtf8()).value().constData();
	else
		return conv->utf8StringCache[ps].constData();
}

/**
 * \brief Return the largest HTTP error code encountered during conversion
 *
 * Return the largest HTTP code greater then or equal to 300 encountered during loading
 * of any of the supplied objects, if no such error code is found 0 is returned.
 * This function will only return a useful result after \ref wkhtmltopdf_convert has been called.
 *
 * \param converter The converter to query
 * \returns The HTTP error code.
 */
CAPI(int) wkhtmltopdf_http_error_code(wkhtmltopdf_converter * converter) {
	return reinterpret_cast<MyPdfConverter *>(converter)->converter.httpErrorCode();
}

/**
 * \brief Get the output document generated during conversion.
 *
 * If no "out" location was specified in the global settings object, the binary
 * output (pdf document) of the convection process will be stored in a buffer.
 *
 * \param converter The converter to query
 * \param d A pointer to a pointer that will be made to point to the output data
 * \returns The length of the output data
 */
CAPI(long) wkhtmltopdf_get_output(wkhtmltopdf_converter * converter, const unsigned char ** d) {
	const QByteArray & out = reinterpret_cast<MyPdfConverter *>(converter)->converter.output();
	*d = (const unsigned char*)out.constData();
	return out.size();
}

//  LocalWords:  eval progn stroustrup innamespace sts sw noet wkhtmltopdf DLL
//  LocalWords:  ifdef WKHTMLTOX UNDEF undef endif pdf dllbegin namespace const
//  LocalWords:  QString cb bool ok globalSettings phaseChanged progressChanged
//  LocalWords:  objectSettings utf CropSettings HeaderFooter ImageGlobal dpi sa
//  LocalWords:  PdfGlobal PdfObject TableOfContent pagePdfGlobal pagePdfObject
//  LocalWords:  pageImageGlobal pageGlobalLoad pageSize colorMode Grayscale
//  LocalWords:  pageOffset outlineDepth dumpOutline stdout pageLoad pageWeb aa
//  LocalWords:  includeInOutline pagesCount tocXsl xsl struct typedef str CAPI
//  LocalWords:  param STRINGIZEE STRINGIZE deinit qApp strcpy wkhtmltox arg ug
//  LocalWords:  WS MACX MyLooksStyle setStyle isNull qstrncpy MyPdfConverter
//  LocalWords:  beginConversion beginConvertion paragm addResource
//  LocalWords:  currentPhase phaseCount urrent http httpErrorCode QByteArray
//  LocalWords:  constData
