#!/bin/bash
wget http://support.googlecode.com/svn/trunk/scripts/googlecode_upload.py -O googlecode_upload.py

function ul() {
	source upload.conf
	python googlecode_upload.py -u "$USER" -w "$PASS" -s "$1" -p "wkhtmltopdf" -l "$2" "$3"
}

ul "wkhtmltopdf-$1 Source" "Type-Source,OpSys-All,Featured" "release-$1/wkhtmltopdf-$1.tar.bz2"
ul "wkhtmltopdf-$1 Linux Static Binary (i368)" "Type-Executable,OpSys-Linux,Featured" "release-$1/wkhtmltopdf-$1-static-i386.tar.bz2"
ul "wkhtmltopdf-$1 Linux Static Binary (amd64)" "Type-Executable,OpSys-Linux,Featured" "release-$1/wkhtmltopdf-$1-static-amd64.tar.bz2"
ul "wkhtmltox-$1 Windows Installer (i368)" "Type-Installer,OpSys-Windows,Featured" "release-$1/wkhtmltox-$1-installer.exe"
ul "wkhtmltoimage-$1 Linux Static Binary (i368)" "Type-Executable,OpSys-Linux,Featured" "release-$1/wkhtmltoimage-$1-static-i386.tar.bz2"
ul "wkhtmltoimage-$1 Linux Static Binary (amd64)" "Type-Executable,OpSys-Linux,Featured" "release-$1/wkhtmltoimage-$1-static-amd64.tar.bz2"
