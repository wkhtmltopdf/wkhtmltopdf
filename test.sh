#!/bin/bash
img=http://upload.wikimedia.org/wikipedia/en/thumb/0/0e/TS3_logo_4.JPG/200px-TS3_logo_4.JPG
#http://www.starcraft2.com/images/screenshots/ss100-hires.jpg
[ -d test ] || mkdir test
cd test

[ -f img.jpg ] || wget $img -O img.jpg

export WK=../wkhtmltopdf

function result() { printf "%-30s [%-4s]\n" "$1" "$2";}
function good() { result "$1" " OK ";}
function bad() { result "$1" "Fail";}
function fs() { du -b "$1" | sed -re 's/([0-9]*).*/\1/';}
function wk() { $WK --redirect-delay 0 -q $*;}

function testImgSupport() {
    [ -f img.$1 ] || convert img.jpg img.$1
    rm -rf tmp.pdf
    echo "<html><head><title>Img test $1</title></head><body><h1>The $1 image</h1><img src=\"img.$1\" /></body></html" > tmp.html
    wk tmp.html tmp.pdf
    S=`fs tmp.pdf`
    ([ -f tmp.pdf ] && [[ $S -ge 20000 ]]) && good "$1 Suppport ($S)" || bad "$1 Support ($S)"
}

function testLocalFileSupport() {
    rm -rf tmp.pdf
    echo "<html><head><title>Local Test</title></head><body><h1>Hello</h1></body></html" > tmp.html
    wk tmp.html tmp.pdf
    [ -f tmp.pdf ] && good LocalFile || bad LocalFile
}

function testRemove() {
    rm -rf tmp.pdf
    wk http://www.google.dk/ tmp.pdf
    [ -f tmp.pdf ] && good Remote || bad Remote
}

function testSSL() {
    rm -rf tmp.pdf
    wk https://eopen.microsoft.com/ tmp.pdf
    [ -f tmp.pdf ] && good SSL || bad SSL
}

good TestTest
testLocalFileSupport 
testImgSupport jpg
testImgSupport gif
testImgSupport png
testImgSupport mng
testImgSupport tiff
testRemove 
testSSL


