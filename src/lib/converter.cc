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


#include "converter_p.hh"
#include "multipageloader.hh"
#include <QWebFrame>
#include <qapplication.h>

#ifdef QT4_STATICPLUGIN_TEXTCODECS
#include <QtPlugin>
Q_IMPORT_PLUGIN(qcncodecs)
Q_IMPORT_PLUGIN(qjpcodecs)
Q_IMPORT_PLUGIN(qkrcodecs)
Q_IMPORT_PLUGIN(qtwcodecs)
#endif

namespace wkhtmltopdf {


void ConverterPrivate::updateWebSettings(QWebSettings * ws, const settings::Web & s) const {
#ifdef  __EXTENSIVE_WKHTMLTOPDF_QT_HACK__
	if (!s.defaultEncoding.isEmpty())
		ws->setDefaultTextEncoding(s.defaultEncoding);
	if (!s.enableIntelligentShrinking) {
		ws->setPrintingMaximumShrinkFactor(1.0);
		ws->setPrintingMinimumShrinkFactor(1.0);
	}
	ws->setPrintingMediaType(s.printMediaType?"print":"screen");
#endif
	ws->setAttribute(QWebSettings::JavaEnabled, s.enablePlugins);
	ws->setAttribute(QWebSettings::JavascriptEnabled, s.enableJavascript);
	ws->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
	ws->setAttribute(QWebSettings::JavascriptCanAccessClipboard, false);
	ws->setFontSize(QWebSettings::MinimumFontSize, s.minimumFontSize);
	//Newer versions of QT have even more settings to change
	ws->setAttribute(QWebSettings::PrintElementBackgrounds, s.background);
	ws->setAttribute(QWebSettings::AutoLoadImages, s.loadImages);
	ws->setAttribute(QWebSettings::PluginsEnabled, s.enablePlugins);
	if (!s.userStyleSheet.isEmpty())
		ws->setUserStyleSheetUrl(MultiPageLoader::guessUrlFromString(s.userStyleSheet));
}

void ConverterPrivate::fail() {
	error = true;
	conversionDone = true;
	clearResources();
	emit outer().finished(false);
	qApp->exit(0); // quit qt's event handling
}

/*!
 * Called when the page is loading, display some progress to the using
 * \param progress the loading progress in percent
 */
void ConverterPrivate::loadProgress(int progress) {
	progressString = QString::number(progress) + "%";
	emit outer().progressChanged(progress);
}

void ConverterPrivate::forwardError(QString error) {
	emit outer().error(error);
}

void ConverterPrivate::forwardWarning(QString warning) {
	emit outer().warning(warning);
}

void ConverterPrivate::cancel() {
	error=true;
}

bool ConverterPrivate::convert() {
	conversionDone=false;
	beginConvert();
	while (!conversionDone)
		qApp->processEvents(QEventLoop::WaitForMoreEvents | QEventLoop::AllEvents);
	return !error;
}


/*!
  \brief Count the number of phases that the conversion process goes though
*/
int Converter::phaseCount() {
	return priv().phaseDescriptions.size();
}

/*!
  \brief return the current phase of conversion
*/
int Converter::currentPhase() {
	return priv().currentPhase;
}

/*!
  \brief return a textual description of some phase
  \param phase the phase to get a description of, -1 for current phase
*/
QString Converter::phaseDescription(int phase) {
	if (phase < 0 || priv().phaseDescriptions.size() <= phase) phase=priv().currentPhase;
	if (phase < 0 || priv().phaseDescriptions.size() <= phase) return "Invalid";
	return priv().phaseDescriptions[phase];
}

/*!
  \brief return a textual description of progress in the current phase
*/
QString Converter::progressString() {
	return priv().progressString;
}

/*!
  \brief return the HTTP return code, of the converted page
*/
int Converter::httpErrorCode() {
	return priv().errorCode;
}

/*!
  \brief Start a asynchronous conversion of html pages to a pdf document.
  Once conversion is done an finished signal will be emitted
*/
void Converter::beginConversion() {
	priv().beginConvert();
}

/*!
  \brief Synchronous convert html pages to a pdf document.
*/
bool Converter::convert() {
	return priv().convert();
}

/*!
  \brief Cancel a running conversion
*/
void Converter::cancel() {
	priv().cancel();
}

void Converter::emitCheckboxSvgs(const settings::LoadPage & ls) {
	emit checkboxSvgChanged(ls.checkboxSvg);
	emit checkboxCheckedSvgChanged(ls.checkboxCheckedSvg);
	emit radiobuttonSvgChanged(ls.radiobuttonSvg);
	emit radiobuttonCheckedSvgChanged(ls.radiobuttonCheckedSvg);
}

}
