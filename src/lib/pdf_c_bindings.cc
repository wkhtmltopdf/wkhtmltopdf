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

/**
 * \file pdf.h
 * \brief Provides C bindings for pdf convertion 
 */

#include "pdf_c_bindings_p.hh"

#include "dllbegin.inc"
using namespace wkhtmltopdf;

void MyPdfConverter::warning(const QString & message) {
	if (warning_cb) (*warning_cb)(reinterpret_cast<wkhtmltopdf_converter*>(this), message.toUtf8().constData());
}

void MyPdfConverter::error(const QString & message) {
	if (error_cb) (*error_cb)(reinterpret_cast<wkhtmltopdf_converter*>(this), message.toUtf8().constData());
}

void MyPdfConverter::phaseChanged() {
	if (phase_changed) (*phase_changed)(reinterpret_cast<wkhtmltopdf_converter*>(this));
}

void MyPdfConverter::progressChanged(int progress) {
	if (progress_changed) (*progress_changed)(reinterpret_cast<wkhtmltopdf_converter*>(this), progress);
}

void MyPdfConverter::finished(bool ok) {
	if (finished_cb) (*finished_cb)(reinterpret_cast<wkhtmltopdf_converter*>(this), ok);
}

MyPdfConverter::MyPdfConverter(settings::PdfGlobal * gs):
	warning_cb(0), error_cb(0), phase_changed(0), progress_changed(0), finished_cb(0),
	converter(*gs), globalSettings(gs) {}

MyPdfConverter::~MyPdfConverter() {
	delete globalSettings;
	for (size_t i=0; i < objectSettings.size(); ++i)
		delete objectSettings[i];
	objectSettings.clear();
}

/**
 * \brief Create a new global settings object for pdf convertion
 * 
 * Create a new global settings object for pdf convertion, settings can be altered with
 * \ref wkhtmltopdf_set_global_setting, and inspected with \ref wkhtmltopdf_get_global_setting.
 * Once the decired settings have been set a converter object can be created using \reg wkhtmltopdf_create_converter.
 *
 * \returns A wkhtmltopdf global settings object
 */
CAPI wkhtmltopdf_global_settings * wkhtmltopdf_create_global_settings() {
	return reinterpret_cast<wkhtmltopdf_global_settings *>(new settings::PdfGlobal());
}

/**
 * \brief Alter a setting in a global settings object
 *
 * \sa  wkhtmltopdf_create_global_settings, wkhtmltopdf_get_global_setting
 *
 * \param settings The settings object to change
 * \param name The name of the setting
 * \param value The new value for the setting
 * \returns 1 if the setting was updated successfully and 0 otherwize.
 */
CAPI int wkhtmltopdf_set_global_setting(wkhtmltopdf_global_settings * settings, const char * name, const char * value) {
	return reinterpret_cast<settings::PdfGlobal *>(settings)->set(name, value);
}

/**
 * \brief Retrive a setting in a global settings object
 *
 * \sa wkhtmltopdf_create_global_settings, wkhtmltopdf_set_global_setting
 * 
 * \param settings The settings object to inspect
 * \param name The name of the setting to read
 * \param value A buffer of length at least \a vs, where the value is storred.
 * \param vs The length of \a value
 * \returns 1 If the the setting exists and was read successfully and 0 otherwize
 */
CAPI int wkhtmltopdf_get_global_setting(wkhtmltopdf_global_settings * settings, const char * name, char * value, int vs) {
	QString res = reinterpret_cast<settings::PdfGlobal *>(settings)->get(name);
	if (res.isNull()) return 0;
	qstrncpy(value, res.toUtf8().constData(), vs);
	return 1;
}

CAPI wkhtmltopdf_object_settings * wkhtmltopdf_create_object_settings() {
	return reinterpret_cast<wkhtmltopdf_object_settings *>(new settings::PdfObject());
}

CAPI int wkhtmltopdf_set_object_setting(wkhtmltopdf_object_settings * settings, const char * name, const char * value) {
	return reinterpret_cast<settings::PdfObject *>(settings)->set(name, value);
}

CAPI int wkhtmltopdf_get_object_setting(wkhtmltopdf_object_settings * settings, const char * name, char * value, int vs) {
	QString res = reinterpret_cast<settings::PdfObject *>(settings)->get(name);
	if (res.isNull()) return 0;
	qstrncpy(value, res.toUtf8().constData(), vs);
	return 1;
}

CAPI wkhtmltopdf_converter * wkhtmltopdf_create_converter(wkhtmltopdf_global_settings * settings) {
	return reinterpret_cast<wkhtmltopdf_converter *>(
		new MyPdfConverter(reinterpret_cast<settings::PdfGlobal *>(settings)));
}

CAPI void wkhtmltopdf_destroy_converter(wkhtmltopdf_converter * converter) {
	delete reinterpret_cast<MyPdfConverter *>(converter);
}

CAPI void wkhtmltopdf_set_warning_callback(wkhtmltopdf_converter * converter, wkhtmltopdf_str_callback * cb) {
	reinterpret_cast<MyPdfConverter *>(converter)->warning_cb = cb;
}

CAPI void wkhtmltopdf_set_error_callback(wkhtmltopdf_converter * converter, wkhtmltopdf_str_callback * cb) {
	reinterpret_cast<MyPdfConverter *>(converter)->error_cb = cb;
}

CAPI void wkhtmltopdf_set_phase_changed_callback(wkhtmltopdf_converter * converter, wkhtmltopdf_void_callback * cb) {
	reinterpret_cast<MyPdfConverter *>(converter)->phase_changed = cb;
}

CAPI void wkhtmltopdf_set_progress_changed_callback(wkhtmltopdf_converter * converter, wkhtmltopdf_int_callback * cb) {
	reinterpret_cast<MyPdfConverter *>(converter)->progress_changed = cb;
}

CAPI void wkhtmltopdf_set_finished_callback(wkhtmltopdf_converter * converter, wkhtmltopdf_int_callback * cb) {
	reinterpret_cast<MyPdfConverter *>(converter)->finished_cb = cb;
}

CAPI void wkhtmltopdf_begin_convertion(wkhtmltopdf_converter * converter) {
	reinterpret_cast<MyPdfConverter *>(converter)->converter.beginConvertion();
}

CAPI int wkhtmltopdf_convert(wkhtmltopdf_converter * converter) {
	return reinterpret_cast<MyPdfConverter *>(converter)->converter.convert();
}

CAPI void wkhtmltopdf_cancel(wkhtmltopdf_converter * converter) {
	reinterpret_cast<MyPdfConverter *>(converter)->converter.cancel();
}

CAPI void wkhtmltopdf_add_resource(
	wkhtmltopdf_converter * converter, wkhtmltopdf_object_settings * settings, const char * data) {
	reinterpret_cast<MyPdfConverter *>(converter)->converter.addResource(
		*reinterpret_cast<settings::PdfObject *>(settings) );
	reinterpret_cast<MyPdfConverter *>(converter)->objectSettings.push_back(reinterpret_cast<settings::PdfObject *>(settings));
}

CAPI int wkhtmltopdf_current_phase(wkhtmltopdf_converter * converter) {
	return reinterpret_cast<MyPdfConverter *>(converter)->converter.currentPhase();
}

CAPI int wkhtmltopdf_phase_count(wkhtmltopdf_converter * converter) {
	return reinterpret_cast<MyPdfConverter *>(converter)->converter.phaseCount();
}

CAPI const char * wkhtmltopdf_phase_description(wkhtmltopdf_converter * converter, int phase) {
	return reinterpret_cast<MyPdfConverter *>(converter)->converter.phaseDescription(phase).toUtf8().constData();
}

CAPI const char * wkhtmltopdf_progress_string(wkhtmltopdf_converter * converter) {
	return reinterpret_cast<MyPdfConverter *>(converter)->converter.progressString().toUtf8().constData();
}

CAPI int wkhtmltopdf_http_error_code(wkhtmltopdf_converter * converter) {
	return reinterpret_cast<MyPdfConverter *>(converter)->converter.httpErrorCode();
}
