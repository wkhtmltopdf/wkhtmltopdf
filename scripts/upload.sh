#!/bin/bash
#
# Copyright 2010, 2011 wkhtmltopdf authors
#
# This file is part of wkhtmltopdf.
#
# wkhtmltopdf is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# wkhtmltopdf is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with wkhtmltopdf.  If not, see <http:#www.gnu.org/licenses/>.

cd "release-$1"
wget http://support.googlecode.com/svn/trunk/scripts/googlecode_upload.py -O googlecode_upload.py

function ul() {
	source ../upload.conf
	python googlecode_upload.py -u "$USER" -w "$PASS" -s "$1" -p "wkhtmltopdf" -l "$2" "$3"
}

function checkf() {
    if ! [ -f "$1" ] ; then
	echo "$1 is missing"
	exit 1;
    fi
}

checkf "wkhtmltopdf-$1.tar.bz2"
checkf "wkhtmltopdf-$1-static-i386.tar.bz2"
checkf "wkhtmltopdf-$1-static-amd64.tar.bz2"
checkf "wkhtmltox-$1-installer.exe"
checkf "wkhtmltoimage-$1-static-i386.tar.bz2"
checkf "wkhtmltoimage-$1-static-amd64.tar.bz2"
checkf "libwkhtmltox-$1.zip"
checkf "libwkhtmltox-$1-i386.tar.bz2"
checkf "libwkhtmltox-$1-amd64.tar.bz2"

ul "wkhtmltopdf-$1 Source" "Type-Source,OpSys-All,Featured" "wkhtmltopdf-$1.tar.bz2"
ul "wkhtmltopdf-$1 Linux Static Binary (i368)" "Type-Executable,OpSys-Linux,Featured" "wkhtmltopdf-$1-static-i386.tar.bz2"
ul "wkhtmltopdf-$1 Linux Static Binary (amd64)" "Type-Executable,OpSys-Linux,Featured" "wkhtmltopdf-$1-static-amd64.tar.bz2"
ul "wkhtmltox-$1 Windows Installer (i368)" "Type-Installer,OpSys-Windows,Featured" "wkhtmltox-$1-installer.exe"
ul "wkhtmltoimage-$1 Linux Static Binary (i368)" "Type-Executable,OpSys-Linux,Featured" "wkhtmltoimage-$1-static-i386.tar.bz2"
ul "wkhtmltoimage-$1 Linux Static Binary (amd64)" "Type-Executable,OpSys-Linux,Featured" "wkhtmltoimage-$1-static-amd64.tar.bz2"
ul "libwkhtmltox-$1 Linux Static Library (i368)" "Type-Archive,OpSys-Linux" "libwkhtmltox-$1-i386.tar.bz2"
ul "libwxhtmltox-$1 Linux Static Library (amd64)" "Type-Archive,OpSys-Linux" "libwkhtmltox-$1-amd64.tar.bz2"
ul "libwkhtmltox-$1 Windows Static Library (i368)" "Type-Archive,OpSys-Windows" "libwkhtmltox-$1.zip"
