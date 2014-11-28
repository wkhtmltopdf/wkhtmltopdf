// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
// Copyright 2010, 2011 wkhtmltopdf authors
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

#include "outputter.hh"

/*!
  \class Outputter
  \brief Interface for classes used to output documentation
*/

/*!
  \fn Outputter::beginSection(const QString & name)
  Add a new section to the output
  \param name The name of the section
*/

/*!
  \fn Outputter::endSection()
  End the current section
*/

/*!
  \fn Outputter::beginParagraph()
  Begin a text paragraph
*/

/*!
  \fn Outputter::text(const QString & t)
  Add some text to the current paragraph
  \param t The text to add
*/

/*!
  \fn Outputter::bold(const QString & t) =  0;
  Add some bold text to the current paragraph
  \param t The text to add
*/

/*!
  \fn Outputter::italic(const QString & t)
  Add some italic text tho the current paragraph
  \param t The text to add
*/

/*!
  \fn Outputter::link(const QString & l)
  Add a hyperlink to somewhere
  \param l The url to link to
*/

/*!
  \fn Outputter::endParagraph()
  End the current paragraph
*/

/*!
  \fn Outputter::verbatim(const QString & t)
  Add a pice of code or verbatime text
  \param t The text to add
*/

/*!
  \fn Outputter::beginList(bool ordered=false)
  Begin a ordered or unordered listing
  \param ordered Should the list be ordered
*/

/*!
  \fn Outputter::endList()
  End the current listing
*/

/*!
  \fn Outputter::listItem(QString & t)
  Add an item to the current listing
  \param t The text to add
*/

/*!
  \fn Outputter::beginSwitch()
  Begin a section with command line switches
*/


/*!
  \fn Outputter::cswitch(const ArgHandler * h, bool doc)
  Output a switch description
  \param h The switch to add
  \param doc Are we outputting to documentation
*/

/*!
  \fn Outputter::endSwitch()
  End a switch section
*/


/*!
  Output a paragraph of simple text
*/
void Outputter::paragraph(const QString & t) {
	beginParagraph();
	text(t);
	endParagraph();
}
