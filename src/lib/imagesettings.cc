// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010-2020 wkhtmltopdf authors
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


#include "imagesettings.hh"
#include "reflect.hh"

#include "dllbegin.inc"
namespace wkhtmltopdf {
namespace settings {

template<>
struct DLL_LOCAL ReflectImpl<CropSettings>: public ReflectClass {
	ReflectImpl(CropSettings & c) {
		WKHTMLTOPDF_REFLECT(left);
		WKHTMLTOPDF_REFLECT(top);
		WKHTMLTOPDF_REFLECT(width);
		WKHTMLTOPDF_REFLECT(height);
	}
};

template<>
struct DLL_LOCAL ReflectImpl<ImageGlobal>: public ReflectClass {
	ReflectImpl(ImageGlobal & c) {
		WKHTMLTOPDF_REFLECT(screenWidth);
		WKHTMLTOPDF_REFLECT(screenHeight);
		ReflectClass::add("quiet", new QuietArgBackwardsCompatReflect(c.logLevel));	// Fake the "quiet" argument
		WKHTMLTOPDF_REFLECT(logLevel);
		WKHTMLTOPDF_REFLECT(transparent);
		WKHTMLTOPDF_REFLECT(useGraphics);
		WKHTMLTOPDF_REFLECT(in);
		WKHTMLTOPDF_REFLECT(out);
		WKHTMLTOPDF_REFLECT(fmt);
		WKHTMLTOPDF_REFLECT(selector);
		WKHTMLTOPDF_REFLECT(quality);
		WKHTMLTOPDF_REFLECT(loadGlobal);
		WKHTMLTOPDF_REFLECT(loadPage);
		WKHTMLTOPDF_REFLECT(smartWidth);
	}
};

const int CropSettings::DEFAULT = std::numeric_limits<int>::min();

CropSettings::CropSettings():
	left(DEFAULT),
	top(DEFAULT),
	width(DEFAULT),
	height(DEFAULT) {}

ImageGlobal::ImageGlobal():
	logLevel(Info),
	transparent(false),
	useGraphics(false),
	in(""),
	out(""),
	fmt(""),
	selector(""),
	screenWidth(1024),
	screenHeight(0),
	quality(94),
	smartWidth(true) {}

QString ImageGlobal::get(const char * name) {
	ReflectImpl<ImageGlobal> impl(*this);
	return impl.get(name);
}

bool ImageGlobal::set(const char * name, const QString & value) {
	ReflectImpl<ImageGlobal> impl(*this);
	return impl.set(name, value);
}


}
}
