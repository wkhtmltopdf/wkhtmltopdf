/*
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

#ifndef __PDF_H__
#define __PDF_H__
#include <wkhtmltox/dllbegin.inc>

struct wkhtmltopdf_global_settings;
typedef struct wkhtmltopdf_global_settings wkhtmltopdf_global_settings;

struct wkhtmltopdf_object_settings;
typedef struct wkhtmltopdf_object_settings wkhtmltopdf_object_settings;

struct wkhtmltopdf_converter;
typedef struct wkhtmltopdf_converter wkhtmltopdf_converter;

typedef void (*wkhtmltopdf_str_callback)(wkhtmltopdf_converter * converter, const char * str);
typedef void (*wkhtmltopdf_int_callback)(wkhtmltopdf_converter * converter, const int val);
typedef void (*wkhtmltopdf_void_callback)(wkhtmltopdf_converter * converter);

CAPI(int) wkhtmltopdf_init(int use_graphics);
CAPI(int) wkhtmltopdf_deinit();
CAPI(int) wkhtmltopdf_extended_qt();
CAPI(const char *) wkhtmltopdf_version();

CAPI(wkhtmltopdf_global_settings *) wkhtmltopdf_create_global_settings();
CAPI(wkhtmltopdf_object_settings *) wkhtmltopdf_create_object_settings();

CAPI(int) wkhtmltopdf_set_global_setting(wkhtmltopdf_global_settings * settings, const char * name, const char * value);
CAPI(int) wkhtmltopdf_get_global_setting(wkhtmltopdf_global_settings * settings, const char * name, char * value, int vs);
CAPI(int) wkhtmltopdf_set_object_setting(wkhtmltopdf_object_settings * settings, const char * name, const char * value);
CAPI(int) wkhtmltopdf_get_object_setting(wkhtmltopdf_object_settings * settings, const char * name, char * value, int vs);


CAPI(wkhtmltopdf_converter *) wkhtmltopdf_create_converter(wkhtmltopdf_global_settings * settings);
CAPI(void) wkhtmltopdf_destroy_converter(wkhtmltopdf_converter * converter);

CAPI(void) wkhtmltopdf_set_warning_callback(wkhtmltopdf_converter * converter, wkhtmltopdf_str_callback cb);
CAPI(void) wkhtmltopdf_set_error_callback(wkhtmltopdf_converter * converter, wkhtmltopdf_str_callback cb);
CAPI(void) wkhtmltopdf_set_phase_changed_callback(wkhtmltopdf_converter * converter, wkhtmltopdf_void_callback cb);
CAPI(void) wkhtmltopdf_set_progress_changed_callback(wkhtmltopdf_converter * converter, wkhtmltopdf_int_callback cb);
CAPI(void) wkhtmltopdf_set_finished_callback(wkhtmltopdf_converter * converter, wkhtmltopdf_int_callback cb);
/* CAPI(void) wkhtmltopdf_begin_conversion(wkhtmltopdf_converter * converter); */
/* CAPI(void) wkhtmltopdf_cancel(wkhtmltopdf_converter * converter); */
CAPI(int) wkhtmltopdf_convert(wkhtmltopdf_converter * converter);
CAPI(void) wkhtmltopdf_add_object(
	wkhtmltopdf_converter * converter, wkhtmltopdf_object_settings * setting, const char * data);

CAPI(int) wkhtmltopdf_current_phase(wkhtmltopdf_converter * converter);
CAPI(int) wkhtmltopdf_phase_count(wkhtmltopdf_converter * converter);
CAPI(const char *) wkhtmltopdf_phase_description(wkhtmltopdf_converter * converter, int phase);
CAPI(const char *) wkhtmltopdf_progress_string(wkhtmltopdf_converter * converter);
CAPI(int) wkhtmltopdf_http_error_code(wkhtmltopdf_converter * converter);
CAPI(long) wkhtmltopdf_get_output(wkhtmltopdf_converter * converter, const unsigned char **);

#include <wkhtmltox/dllend.inc>
#endif /*__PDF_H__*/
