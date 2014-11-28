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
 * \mainpage
 *
 * libwkhtmltox is divided into several parts.
 *
 * \section PDFH To PDF c-bindings
 *
 * The file \ref pdf.h contains a
 * fairly high level and stable pure c binding to wkhtmltopdf.  These
 * binding are well documented and do not depend on QT.  Using this is
 * the recommended way of interfacing with the PDF portion of
 * libwkhtmltox.
 *
 * Using these binding it is relatively straight forward to convert one
 * or more HTML document to a PDF file, using the following process:
 *
 * - \ref wkhtmltopdf_init is called.
 * - A \ref wkhtmltopdf_global_settings object is creating by calling
 *   \ref wkhtmltopdf_create_global_settings.
 * - Non web page specific \ref pagesettings for the conversion are set by multiple
 *   calls to \ref wkhtmltopdf_set_global_setting.
 * - A \ref wkhtmltopdf_converter object is created by calling
 *   \ref wkhtmltopdf_create_converter, which consumes the global_settings instance.
 * - A number of object (web pages) are added to the conversion process, this is done by
 *   - Creating a \ref wkhtmltopdf_object_settings instance by calling
 *     \ref wkhtmltopdf_create_object_settings.
 *   - Setting web page specific \ref pagesettings by multiple calls to
 *     \ref wkhtmltopdf_set_object_setting.
 *   - Adding the object to the conversion process by calling \ref wkhtmltopdf_add_object
 * - A number of callback function are added to the converter object.
 * - The conversion is performed by calling \ref wkhtmltopdf_convert.
 * - The converter object is destroyed by calling \ref wkhtmltopdf_destroy_converter.
 *
 * \section IMAGEH To image c-bindings
 *
 * The file \ref image.h contains a
 * fairly high level and stable pure c binding to wkhtmltoimage.  These
 * binding are well documented and do not depend on QT.  Using this is
 * the recommended way of interfacing with the image portion of
 * libwkhtmltox.
 *
 * Using these binding it is relatively straight forward to convert one
 * or more HTML document to a raster image or SVG document, using the following process:
 *
 * - \ref wkhtmltoimage_init is called.
 * - A \ref wkhtmltoimage_global_settings object is creating by calling
 *   \ref wkhtmltoimage_create_global_settings.
 * - \ref pagesettings for the conversion are set by multiple
 *   calls to \ref wkhtmltoimage_set_global_setting.
 * - A \ref wkhtmltoimage_converter object is created by calling
 *   \ref wkhtmltoimage_create_converter, which consumes the global_settings instance.
 * - A number of callback function are added to the converter object.
 * - The conversion is performed by calling \ref wkhtmltoimage_convert.
 * - The converter object is destroyed by calling \ref wkhtmltoimage_destroy_converter.
 *
 * \section RESTH The rest of the headers.
 *
 * The rest of the headers directly exposes the C++ QT dependent class
 * used internally by wkhtmltopdf and wkhtmltoimage. They are not
 * fairly well documented and are not guaranteed to remain stable.
 * unless you know what you are doing you should not use these bindings.
 *
 */
