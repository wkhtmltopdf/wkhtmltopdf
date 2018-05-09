/* -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
 * vi:set ts=4 sts=4 sw=4 noet :
 *
 * Copyright 2010 wkhtmltopdf authors
 *
 * This file is part of wkhtmltopdf.
 *
 * wkhtmltopdf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * wkhtmltopdf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with wkhtmltopdf.  If not, see <http: *www.gnu.org/licenses/>.
 */

#ifndef __IMAGE_H__
#define __IMAGE_H__

#ifdef BUILDING_WKHTMLTOX
#include "dllbegin.inc"
#else
#include <wkhtmltox/dllbegin.inc>
#endif

struct wkhtmltoimage_global_settings;
typedef struct wkhtmltoimage_global_settings wkhtmltoimage_global_settings;

struct wkhtmltoimage_converter;
typedef struct wkhtmltoimage_converter wkhtmltoimage_converter;

typedef void (*wkhtmltoimage_str_callback)(wkhtmltoimage_converter * converter, const char * str);
typedef void (*wkhtmltoimage_int_callback)(wkhtmltoimage_converter * converter, const int val);
typedef void (*wkhtmltoimage_void_callback)(wkhtmltoimage_converter * converter);

CAPI(int) wkhtmltoimage_init(int use_graphics);
CAPI(int) wkhtmltoimage_deinit();
CAPI(int) wkhtmltoimage_extended_qt();
CAPI(const char *)wkhtmltoimage_version();

CAPI(wkhtmltoimage_global_settings *) wkhtmltoimage_create_global_settings();

CAPI(int) wkhtmltoimage_set_global_setting(wkhtmltoimage_global_settings * settings, const char * name, const char * value);
CAPI(int) wkhtmltoimage_get_global_setting(wkhtmltoimage_global_settings * settings, const char * name, char * value, int vs);

CAPI(wkhtmltoimage_converter *) wkhtmltoimage_create_converter(wkhtmltoimage_global_settings * settings, const char * data);
CAPI(void) wkhtmltoimage_destroy_converter(wkhtmltoimage_converter * converter);

CAPI(void) wkhtmltoimage_set_warning_callback(wkhtmltoimage_converter * converter, wkhtmltoimage_str_callback cb);
CAPI(void) wkhtmltoimage_set_error_callback(wkhtmltoimage_converter * converter, wkhtmltoimage_str_callback cb);
CAPI(void) wkhtmltoimage_set_phase_changed_callback(wkhtmltoimage_converter * converter, wkhtmltoimage_void_callback cb);
CAPI(void) wkhtmltoimage_set_progress_changed_callback(wkhtmltoimage_converter * converter, wkhtmltoimage_int_callback cb);
CAPI(void) wkhtmltoimage_set_finished_callback(wkhtmltoimage_converter * converter, wkhtmltoimage_int_callback cb);
CAPI(int) wkhtmltoimage_convert(wkhtmltoimage_converter * converter);
/* CAPI(void) wkhtmltoimage_begin_conversion(wkhtmltoimage_converter * converter); */
/* CAPI(void) wkhtmltoimage_cancel(wkhtmltoimage_converter * converter); */

CAPI(int) wkhtmltoimage_current_phase(wkhtmltoimage_converter * converter);
CAPI(int) wkhtmltoimage_phase_count(wkhtmltoimage_converter * converter);
CAPI(const char *) wkhtmltoimage_phase_description(wkhtmltoimage_converter * converter, int phase);
CAPI(const char *) wkhtmltoimage_progress_string(wkhtmltoimage_converter * converter);
CAPI(int) wkhtmltoimage_http_error_code(wkhtmltoimage_converter * converter);
CAPI(long) wkhtmltoimage_get_output(wkhtmltoimage_converter * converter, const unsigned char **);

#ifdef BUILDING_WKHTMLTOX
#include "dllend.inc"
#else
#include <wkhtmltox/dllend.inc>
#endif

#endif /*__IMAGE_H__*/
