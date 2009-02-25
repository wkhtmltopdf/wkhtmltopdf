#!/bin/bash
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

#This script will the a compiled wkhtmltopdf for some basic functionality

#This is just some random image
img=http://upload.wikimedia.org/wikipedia/en/thumb/0/0e/TS3_logo_4.JPG/200px-TS3_logo_4.JPG
[ -d test ] || mkdir test
cd test

[ -f img.jpg ] || wget $img -O img.jpg


export WK=../wkhtmltopdf

function result() { printf "%-30s [%-4s]\n" "$1" "$2";}
function good() { result "$1" " OK ";}
function bad() { result "$1" "Fail";}
function fs() { du -b "$1" | sed -re 's/([0-9]*).*/\1/';}
function wk() { $WK --redirect-delay 0 -q $*;}

#Test if we can convert a html file containing
#An image of some format
function testImgSupport() {
    [ -f img.$1 ] || convert img.jpg img.$1
    rm -rf tmp.pdf
    echo "<html><head><title>Img test $1</title></head><body><h1>The $1 image</h1><img src=\"img.$1\" /></body></html>" > tmp.html
    wk tmp.html tmp.pdf
    S=`fs tmp.pdf`
    ([ -f tmp.pdf ] && [[ $S -ge 20000 ]]) && good "$1 Suppport ($S)" || bad "$1 Support ($S)"
}

#Test if we can convert a local file, and that it has some of the right words
function testLocalFileSupport() {
    rm -rf tmp.pdf
    echo "<html><head><title>Local Test</title></head><body><h1>Hello</h1></body></html>" > tmp.html
    wk tmp.html tmp.pdf
    ([ -f tmp.pdf ] && pdftotext tmp.pdf /dev/stdout | grep -q Hello) && good LocalFile || bad LocalFile
}

#Test if we can convert a remove site.
function testRemote() {
    rm -rf tmp.pdf
    wk http://www.google.dk/ tmp.pdf
    [ -f tmp.pdf ] && good Remote || bad Remote
}

#Test if we support SSL based sites
function testSSL() {
    rm -rf tmp.pdf
    wk https://eopen.microsoft.com/ tmp.pdf
    [ -f tmp.pdf ] && good SSL || bad SSL
}

#Test if the header footer stuff works
function testHeaderFooter() {
    rm -rf tmp.pdf
	echo "<html><head><title>Local Test</title></head><body><h1>monster</h1></body></html>" > tmp.html
	wk tmp.html tmp.pdf --footer-left hat --header-right emacs
	pdftotext tmp.pdf /dev/stdout
    ([ -f tmp.pdf ] && 
		pdftotext tmp.pdf /dev/stdout | grep -q monster &&
		pdftotext tmp.pdf /dev/stdout | grep -q emacs &&
		pdftotext tmp.pdf /dev/stdout | grep -q hat) && good HeaderFooter || bad HeaderFooter
}


good TestTest
testLocalFileSupport 
testImgSupport jpg
testImgSupport gif
testImgSupport png
testImgSupport mng
testImgSupport tiff
testRemote 
testSSL
testHeaderFooter

#Lets clean up
rm tmp.html tmp.pdf
