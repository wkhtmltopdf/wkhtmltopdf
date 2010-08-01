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

#include "image_c_bindings_p.hh"
#include "pdf.h"

#include "dllbegin.inc"
using namespace wkhtmltopdf;

void MyImageConverter::warning(const QString & message) {
	if (warning_cb) (warning_cb)(reinterpret_cast<wkhtmltoimage_converter*>(this), message.toUtf8().constData());
}

void MyImageConverter::error(const QString & message) {
	if (error_cb) (error_cb)(reinterpret_cast<wkhtmltoimage_converter*>(this), message.toUtf8().constData());
}

void MyImageConverter::phaseChanged() {
	if (phase_changed) (phase_changed)(reinterpret_cast<wkhtmltoimage_converter*>(this));
}

void MyImageConverter::progressChanged(int progress) {
	if (progress_changed) (progress_changed)(reinterpret_cast<wkhtmltoimage_converter*>(this), progress);
}

void MyImageConverter::finished(bool ok) {
	if (finished_cb) (finished_cb)(reinterpret_cast<wkhtmltoimage_converter*>(this), ok);
}

MyImageConverter::MyImageConverter(settings::ImageGlobal * gs):
	warning_cb(0), error_cb(0), phase_changed(0), progress_changed(0), finished_cb(0),
	converter(*gs), globalSettings(gs) {

    connect(&converter, SIGNAL(warning(const QString &)), this, SLOT(warning(const QString &)));
	connect(&converter, SIGNAL(error(const QString &)), this, SLOT(error(const QString &)));
	connect(&converter, SIGNAL(phaseChanged()), this, SLOT(phaseChanged()));
	connect(&converter, SIGNAL(progressChanged(int)), this, SLOT(progressChanged(int)));
	connect(&converter, SIGNAL(finished(bool)), this, SLOT(finished(bool)));
}

MyImageConverter::~MyImageConverter() {
	delete globalSettings;
}

CAPI int wkhtmltoimage_init(int useGraphics) {
	return wkhtmltopdf_init(useGraphics);
}

CAPI int wkhtmltoimage_deinit() {
	return wkhtmltopdf_deinit();
}

CAPI int wkhtmltoimage_extended_qt() {
	return wkhtmltopdf_extended_qt();
}

CAPI wkhtmltoimage_global_settings * wkhtmltoimage_create_global_settings() {
	return reinterpret_cast<wkhtmltoimage_global_settings *>(new settings::ImageGlobal());
}

CAPI int wkhtmltoimage_set_global_option(wkhtmltoimage_global_settings * settings, const char * name, const char * value) {
	return reinterpret_cast<settings::ImageGlobal *>(settings)->set(name, value);
}

CAPI int wkhtmltoimage_get_global_option(wkhtmltoimage_global_settings * settings, const char * name, char * value, int vs) {
	QString res = reinterpret_cast<settings::ImageGlobal *>(settings)->get(name);
	if (res.isNull()) return 0;
	qstrncpy(value, res.toUtf8().constData(), vs);
	return 1;
}

CAPI wkhtmltoimage_converter * wkhtmltoimage_create_converter(wkhtmltoimage_global_settings * settings) {
	return reinterpret_cast<wkhtmltoimage_converter *>(
		new MyImageConverter(reinterpret_cast<settings::ImageGlobal *>(settings)));
}

CAPI void wkhtmltoimage_destroy_converter(wkhtmltoimage_converter * converter) {
	delete reinterpret_cast<MyImageConverter *>(converter);
}

CAPI void wkhtmltoimage_set_warning_callback(wkhtmltoimage_converter * converter, wkhtmltoimage_str_callback cb) {
	reinterpret_cast<MyImageConverter *>(converter)->warning_cb = cb;
}

CAPI void wkhtmltoimage_set_error_callback(wkhtmltoimage_converter * converter, wkhtmltoimage_str_callback cb) {
	reinterpret_cast<MyImageConverter *>(converter)->error_cb = cb;
}

CAPI void wkhtmltoimage_set_phase_changed_callback(wkhtmltoimage_converter * converter, wkhtmltoimage_void_callback cb) {
	reinterpret_cast<MyImageConverter *>(converter)->phase_changed = cb;
}

CAPI void wkhtmltoimage_set_progress_changed_callback(wkhtmltoimage_converter * converter, wkhtmltoimage_int_callback cb) {
	reinterpret_cast<MyImageConverter *>(converter)->progress_changed = cb;
}

CAPI void wkhtmltoimage_set_finished_callback(wkhtmltoimage_converter * converter, wkhtmltoimage_int_callback cb) {
	reinterpret_cast<MyImageConverter *>(converter)->finished_cb = cb;
}

CAPI void wkhtmltoimage_begin_convertion(wkhtmltoimage_converter * converter) {
	reinterpret_cast<MyImageConverter *>(converter)->converter.beginConvertion();
}

CAPI int wkhtmltoimage_convert(wkhtmltoimage_converter * converter) {
	return reinterpret_cast<MyImageConverter *>(converter)->converter.convert();
}

CAPI void wkhtmltoimage_cancel(wkhtmltoimage_converter * converter) {
	reinterpret_cast<MyImageConverter *>(converter)->converter.cancel();
}

CAPI int wkhtmltoimage_current_phase(wkhtmltoimage_converter * converter) {
	return reinterpret_cast<MyImageConverter *>(converter)->converter.currentPhase();
}

CAPI int wkhtmltoimage_phase_count(wkhtmltoimage_converter * converter) {
	return reinterpret_cast<MyImageConverter *>(converter)->converter.phaseCount();
}

CAPI const char * wkhtmltoimage_phase_description(wkhtmltoimage_converter * converter, int phase) {
	return reinterpret_cast<MyImageConverter *>(converter)->converter.phaseDescription(phase).toUtf8().constData();
}

CAPI const char * wkhtmltoimage_progress_string(wkhtmltoimage_converter * converter) {
	return reinterpret_cast<MyImageConverter *>(converter)->converter.progressString().toUtf8().constData();
}

CAPI int wkhtmltoimage_http_error_code(wkhtmltoimage_converter * converter) {
	return reinterpret_cast<MyImageConverter *>(converter)->converter.httpErrorCode();
}
