#!/bin/bash
#
# Copyright 2010 wkhtmltopdf authors
#
# This file is part of wkhtmltopdf.
#
# wkhtmltopdf is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# wkhtmltopdf is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with wkhtmltopdf.  If not, see <http:#www.gnu.org/licenses/>.

mkdir -p include/wkhtmltox
find src -iname '*.pro' -or -iname '*.pri' | while read name; do
	cat "$name" | sed -nre 's/PUBLIC_HEADERS \+=[ \t]*(.*[^\t ])[^\t ]*/\1/p' | sed -re 's/[\t ]+/\n/g' | sort -u | while read file; do
		path="$(dirname $name)/$file"
		[ -f "$path" ] || continue
		cat $path | sed -n '4,$p' | sed -e '1s/ \*/\/\*\n \*/'> tmpheader
		dst="include/wkhtmltox/$(basename $path)"

		[ -f "${dst}" ] && cmp -s "$dst" tmpheader && rm tmpheader && continue
		echo "$dst"
		mv tmpheader "$dst"
	done
done
