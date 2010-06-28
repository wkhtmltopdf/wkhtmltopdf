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

#ifndef __IMAGECONVERTER_P_HH__
#define __IMAGECONVERTER_P_HH__
#include "converter_p.hh"
#include "imageconverter.hh"
#include "multipageloader.hh"
namespace wkhtmltopdf {

class ImageConverterPrivate: public ConverterPrivate {
	Q_OBJECT
public:
	ImageConverterPrivate(ImageConverter & o, wkhtmltopdf::settings::Global & s);

	wkhtmltopdf::settings::Global settings;
	MultiPageLoader loader;
private:
	ImageConverter & out;
	void clearResources();

	LoaderObject * loaderObject;

public slots:
	void pagesLoaded(bool ok);
	void beginConvert();

	friend class ImageConverter;

	virtual Converter & outer();
};

}
#endif //__IMAGECONVERTER_P_HH__
