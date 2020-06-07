/* -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
 * vi:set ts=4 sts=4 sw=4 noet :
 *
 * Copyright 2010-2020 wkhtmltopdf authors
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

#ifndef __WKHTMLTOPDF_DLLBEGIN__
#define __WKHTMLTOPDF_DLLBEGIN__

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
     #define DLL_PUBLIC __declspec(dllexport)
  #else
     #define DLL_PUBLIC __declspec(dllimport)
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility("default")))
    #define DLL_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif

#if defined _WIN32
#define CALLTYPE __stdcall
#else
#define CALLTYPE
#endif

#ifdef __cplusplus
  #define CAPI(type) extern "C" DLL_PUBLIC type CALLTYPE
#else
  #define CAPI(type) DLL_PUBLIC type CALLTYPE
#endif

#endif /*__WKHTMLTOPDF_DLLBEGIN__*/
