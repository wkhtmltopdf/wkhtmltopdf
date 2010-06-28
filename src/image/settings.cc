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

#include "settings.hh"

namespace wkhtmltopdf {
	namespace settings {

	CropSettings:: CropSettings():
		left(-1),
		top(-1),
		width(-1),
		height(-1) {}

	//ScaleSettings::ScaleSettings():
	//	width(-1),
	//	height(-1) {}

	Global::Global():
		screenWidth(1024),
		quiet(false),
		transparent(false),
		useGraphics(false),
		in(""),
		out(""),
		fmt(""),
		quality(94),
		smartWidth(true) {}
	}
}
