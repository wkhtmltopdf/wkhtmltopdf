/* -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
 * vi:set ts=4 sts=4 sw=4 noet :
 *
 * Copyright 2010, 2011 wkhtmltopdf authors
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

/* This is a simple example program showing how to use the wkhtmltopdf c bindings */
#include <stdbool.h>
#include <stdio.h>
#include <wkhtmltox/pdf.h>

/* Print out loading progress information */
void progress_changed(wkhtmltopdf_converter * c, int p) {
	printf("%3d%%\r",p);
	fflush(stdout);
}

/* Print loading phase information */
void phase_changed(wkhtmltopdf_converter * c) {
	int phase = wkhtmltopdf_current_phase(c);
	printf("%s\n", wkhtmltopdf_phase_description(c, phase));
}

/* Print a message to stderr when an error occurs */
void error(wkhtmltopdf_converter * c, const char * msg) {
	fprintf(stderr, "Error: %s\n", msg);
}

/* Print a message to stderr when a warning is issued */
void warning(wkhtmltopdf_converter * c, const char * msg) {
	fprintf(stderr, "Warning: %s\n", msg);
}

/* Main method convert pdf */
int main() {
	wkhtmltopdf_global_settings * gs;
	wkhtmltopdf_object_settings * os;
	wkhtmltopdf_converter * c;

	/* Init wkhtmltopdf in graphics less mode */
	wkhtmltopdf_init(false);

	/*
	 * Create a global settings object used to store options that are not
	 * related to input objects, note that control of this object is parsed to
	 * the converter later, which is then responsible for freeing it
	 */
	gs = wkhtmltopdf_create_global_settings();
	/* We want the result to be storred in the file called test.pdf */
	wkhtmltopdf_set_global_setting(gs, "out", "test.pdf");

	wkhtmltopdf_set_global_setting(gs, "load.cookieJar", "myjar.jar");
	/*
	 * Create a input object settings object that is used to store settings
	 * related to a input object, note again that control of this object is parsed to
	 * the converter later, which is then responsible for freeing it
	 */
	os = wkhtmltopdf_create_object_settings();
	/* We want to convert to convert the qstring documentation page */
	wkhtmltopdf_set_object_setting(os, "page", "http://doc.qt.io/qt-5/qstring.html");

	/* Create the actual converter object used to convert the pages */
	c = wkhtmltopdf_create_converter(gs);

	/* Call the progress_changed function when progress changes */
	wkhtmltopdf_set_progress_changed_callback(c, progress_changed);

	/* Call the phase _changed function when the phase changes */
	wkhtmltopdf_set_phase_changed_callback(c, phase_changed);

	/* Call the error function when an error occurs */
	wkhtmltopdf_set_error_callback(c, error);

	/* Call the warning function when a warning is issued */
	wkhtmltopdf_set_warning_callback(c, warning);

	/*
	 * Add the the settings object describing the qstring documentation page
	 * to the list of pages to convert. Objects are converted in the order in which
	 * they are added
	 */
	wkhtmltopdf_add_object(c, os, NULL);

	/* Perform the actual conversion */
	if (!wkhtmltopdf_convert(c))
		fprintf(stderr, "Conversion failed!");

	/* Output possible http error code encountered */
	printf("httpErrorCode: %d\n", wkhtmltopdf_http_error_code(c));

	/* Destroy the converter object since we are done with it */
	wkhtmltopdf_destroy_converter(c);

	/* We will no longer be needing wkhtmltopdf funcionality */
	wkhtmltopdf_deinit();

	return 0;
}
