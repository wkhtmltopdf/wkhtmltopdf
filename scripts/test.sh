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

#This script will test a compiled wkhtmltopdf for some basic functionality

function result() { printf "%-30s [%-4s]\n" "$1" "$2";}
function good() { result "$1" " OK ";}
function bad() { result "$1" "Fail"; [ "$2" != "false" ] && export failed=$(($failed+1));}
function fs() { du -b "$1" | sed -re 's/([0-9]*).*/\1/';}
function wk() { $WK -q "$@"; }

function pages() {
    pdfinfo $1 | sed -nre 's/Pages:[\n ]*//p'
}

function testTest() {
    (which pdftotext > $LEVEL1 && which pdfinfo > $LEVEL1) && good $1 || bad $1
}

#Test if we can convert a html file containing
#An image of some format
function doTestImgSupport() {
    [ -f img.$1 ] || convert img.jpg img.$1
    rm -rf tmp.pdf
    echo "<html><head><title>Img test $1</title></head><body><h1>The $1 image</h1><img src=\"img.$1\" /></body></html>" > tmp.html
    wk tmp.html tmp.pdf 2>$LEVEL2 >$LEVEL1
    S=`fs tmp.pdf`
    ([ -f tmp.pdf ] && [[ $S -ge 20000 ]]) && good "$1 Suppport ($S)" || bad "$1 Support ($S)" "$2"
}

function testJPEGSupport() {
    doTestImgSupport jpg true
}

function testGIFSupport() {
    doTestImgSupport gif true
}

function testPNGSupport() {
    doTestImgSupport png true
}

function testMNGSupport() {
    doTestImgSupport mng false
}

function testTIFFSupport() {
    doTestImgSupport tiff false
}

function testLocalFile() {
    echo "<html><head><title>Local Test</title></head><body><h1>Hello</h1></body></html>" > tmp.html
    wk tmp.html tmp.pdf 2>$LEVEL2 >$LEVEL1
    ([ -f tmp.pdf ] && pdftotext tmp.pdf /dev/stdout | grep -q Hello) && good $1 || bad $1
}

function testUserStyleSheet() {
    echo "<html><head><title>Local Test</title></head><body><p>.</p></body></html>" > tmp.html
    echo "p:before {content: \"Hello \"}" > tmp.css
    wk tmp.html --user-style-sheet tmp.css tmp.pdf 2>$LEVEL2 >$LEVEL1
    ([ -f tmp.pdf ] && pdftotext tmp.pdf /dev/stdout | grep -q Hello) && good $1 || bad $1 false
}

function testPipeIn() {
    rm -rf tmp.pdf
    echo "<html><head><title>Local Test</title></head><body><h1>Hello</h1></body></html>" > tmp.html
    cat tmp.html | wk - tmp.pdf 2>$LEVEL2 >$LEVEL1
    ([ -f tmp.pdf ] && pdftotext tmp.pdf /dev/stdout | grep -q Hello) && good $1 || bad $1
}

function testPipeOutSupport() {
    echo "<html><head><title>Local Test</title></head><body><h1>Hello</h1></body></html>" > tmp.html
    wk tmp.html - > tmp.pdf  2>$LEVEL2
    ([ -f tmp.pdf ] && pdftotext tmp.pdf /dev/stdout | grep -q Hello) && good $1 || bad $1
}

function testRemote() {
    wk http://www.google.dk/ tmp.pdf 2>$LEVEL2 >$LEVEL1
    [ -f tmp.pdf ] && good $1 || bad $1
}

function testSSL() {
    wk https://www.google.com/accouts tmp.pdf 2>$LEVEL2 >$LEVEL1
    [ -f tmp.pdf ] && good $1 || bad $1
}

function testHeaderFooter() {
    echo "<html><head><title>Local Test</title></head><body><h1>monster</h1></body></html>" > tmp.html
    wk tmp.html --footer-left hat --header-right emacs tmp.pdf 2>$LEVEL2 >$LEVEL1
    ([ -f tmp.pdf ] &&
	pdftotext tmp.pdf /dev/stdout | grep -q monster &&
	pdftotext tmp.pdf /dev/stdout | grep -q emacs &&
	pdftotext tmp.pdf /dev/stdout | grep -q hat) && good $1 || bad $1
}

function testLoadError() {
    wk about:blank http://a.nstld --load-error-handling abort tmp.pdf 2>$LEVEL1 >$LEVEL1 && (bad $1 && return)
    wk about:blank http://a.nstld --load-error-handling skip tmp1.pdf 2>$LEVEL2 >$LEVEL1 || (bad $1 && return)
    wk about:blank http://a.nstld --load-error-handling ignore tmp2.pdf 2>$LEVEL2 >$LEVEL1 || (bad $1 && return)
    ([ -f tmp1.pdf ] && [ -f tmp2.pdf ] &&
	[ "$(pages tmp1.pdf)" == 1 ] &&
	[ "$(pages tmp2.pdf)" == 2 ] ) && good $1 || bad $1
}

function testFontSpacing() {
	echo "<html><body style=\"font-size: 7pt; font-family: Arial;\">vii vaa vuu vvv vee</body></html>" > tmp.html
    wk tmp.html tmp.pdf 2>$LEVEL2 >$LEVEL1
    ([ -f tmp.pdf ] &&
	pdftotext tmp.pdf /dev/stdout | grep -q "vii vaa vuu vvv vee") && good $1 || bad $1 false

}

function testToc() {
    echo "<html><head></head><body><h1>foo</h1><h2>bar</h2><h3>baz</h3></body>" > tmp.html
    wk toc tmp.html tmp.pdf 2>$LEVEL2 >$LEVEL1
    ([ -f tmp.pdf ] &&
	[ "$(pdftotext tmp.pdf /dev/stdout | grep -c foo)" == 2 ] &&
	[ "$(pdftotext tmp.pdf /dev/stdout | grep -c bar)" == 2 ]) && good $1 || bad $1
}

function testOutline() {
    echo "<html><head></head><body><h1>foo</h1><h2>bar</h2><h3>baz</h3></body>" > tmp.html
    wk --outline --outline-depth 2 tmp.html tmp.pdf 2>$LEVEL2 >$LEVEL1
    ([ -f tmp.pdf ] &&
	cat tmp.pdf | grep -q ".f.o.o" &&
	cat tmp.pdf | grep -q ".b.a.r" &&
	! cat tmp.pdf | grep -q ".b.a.z") && good $1 || bad $1
}

function testJSRedirect() {
    wk http://madalgo.au.dk/~jakobt/jsredirect.html tmp.pdf 2>$LEVEL2 >$LEVEL1
    ([ -f tmp.pdf ] &&
	pdftotext tmp.pdf /dev/stdout | grep -q Right) && good $1 || bad $1
}

function testServersideRedirect() {
    wk http://madalgo.au.dk/~jakobt/redirect.php tmp.pdf 2>$LEVEL2 >$LEVEL1
    ([ -f tmp.pdf ] &&
	pdftotext tmp.pdf /dev/stdout | grep -q Right) && good $1 || bad $1
}

function test404() {
    wk http://madalgo.au.dk/~jakobt/nosuchfile.nsf tmp.pdf 2>$LEVEL2 >$LEVEL1 && bad $1 || good $1
}

function testBadDest() {
    echo "<html><head></head><body><h1>foo</h1><h2>bar</h2><h3>baz</h3></body>" > tmp.html
    (! wk tmp.html /proc/cpuinfo 2> tmp.out >$LEVEL1 && grep -q "Error" tmp.out) && good $1 || bad $1
}

function testMultipleInputDocuments() {
    echo "<html><head><title>Local Test</title></head><body><h1>Hello</h1></body></html>" > tmp.html
    echo "<html><head><title>Local Test</title></head><body><h1>world</h1></body></html>" > tmp2.html
    wk tmp.html tmp2.html tmp.pdf 2>$LEVEL2 >$LEVEL1
    ([ -f tmp.pdf ] &&
	pdftotext tmp.pdf /dev/stdout | grep -q Hello &&
	pdftotext tmp.pdf /dev/stdout | grep -q world) && good $1 || bad $1
}

function testHtmlHeader() {
    echo "<html><body>Header</body></html>" > tmp.html
    echo "<html><head><title>Local Test</title></head><body><h1>world</h1></body></html>" > tmp2.html
    wk --header-html tmp.html tmp2.html tmp.pdf 2>$LEVEL2 >$LEVEL1
    ([ -f tmp.pdf ] &&
	pdftotext tmp.pdf /dev/stdout | grep -q Header &&
	pdftotext tmp.pdf /dev/stdout | grep -q world) && good $1 || bad $1
}

function testCustomHeader() {
    wk http://madalgo.au.dk/~jakobt/cookiewrite.php --custom-header "Cookie" "cookie=hello" tmp.pdf 2>$LEVEL2 >$LEVEL1
    ([ -f tmp.pdf ] &&
	pdftotext tmp.pdf /dev/stdout | grep -q hello) && good $1 || bad $1
}

function testCookies() {
    wk --cookie mykey myvalue1 --cookie mykey2 myvalue2 --cookie-jar tmp.jar http://cs.au.dk/~jakobt/cookie.php tmp.pdf 2>$LEVEL2 >$LEVEL1
    wk --cookie-jar tmp.jar http://cs.au.dk/~jakobt/cookie.php tmp2.pdf 2>$LEVEL2 >$LEVEL1
    (   [ -f tmp.pdf ] &&
	pdftotext tmp.pdf /dev/stdout | grep -q "mykey:myvalue1;" &&
	pdftotext tmp.pdf /dev/stdout | grep -q "mykey2:myvalue2;" &&
	[ -f tmp2.pdf ] &&
	pdftotext tmp2.pdf /dev/stdout | grep -q "writetest:success;") &&
    good $1 || bad $1
}

function testTitle() {
    title="\()fooæøåおさか おかみ"
    wk  --title "$title" http://google.com tmp.pdf 2>$LEVEL2 >$LEVEL1
    ([ -f tmp.pdf ] && [[ "$(pdfinfo tmp.pdf  | sed -nre 's/Title:[\t ]*//p')" == "$title" ]]) && good $1 || bad $1
}


function testQMakeBuild() {
    (cd .. && git checkout-index --prefix=./test/wkhtmltopdf/ -a) || (bad "$1 (1)" && return 1)
    cd wkhtmltopdf
    qmake 2>$LEVEL2 >$LEVEL1 || (bad "$1 (2)" && return 1)
    make -j2 2>$LEVEL2 >$LEVEL1 && good $1 || bad $1
    cd ..
}

function testArgumentFromStdin() {
    echo "<html><body>XFooZ</body></html>" > tmp.html
    wk --read-args-from-stdin 2>$LEVEL2 >$LEVEL1 <<EOF
-q tmp.html tmp1.pdf
-q tmp.html tmp2.pdf
EOF
    (   [ -f tmp1.pdf ] && pdftotext tmp1.pdf /dev/stdout | grep -q XFooZ &&
	[ -f tmp2.pdf ] && pdftotext tmp2.pdf /dev/stdout | grep -q XFooZ) && good $1 || bad $1
}

function usage() {
    echo "Usage $0 [OPTIONS] [EXPRESSION]"
    echo ""
    echo "Options:"
    echo "-h         Display this message"
    echo "-v         Be more verbose"
    echo "-q         Be more quiet"
    echo "-w path    wkhtmltopdf binary to test"
    echo ""
    echo "Expression:"
    echo "grep regular expresion on what tests to run"
}

LEVEL1=/dev/null
LEVEL2=/dev/stdout
LEVEL3=/dev/stdout

export WK=../bin/wkhtmltopdf

while getopts hvqw: O; do
    case "$O" in
	[h?])
	    usage
	    exit 1
	    ;;
	v)
	    shift 1
	    LEVEL1=/dev/stdout
	    ;;
	q)
	    shift 1
	    LEVEL2=/dev/null
	    ;;
	w)
	    shift 2
	    export WK=$OPTARG
	    ;;
    esac
done
R=$1
if [ -z "$R" ]; then
    R=.*
fi

TESTS=$(cat $0 | sed -nre 's/^function test(.*)\(\)\ \{.*/\1/p' | grep "$R");

#This is just some random image
img=http://madalgo.au.dk/img/Forside/KatrinebjMay-05.jpg
[ -d test ] || mkdir test
cd test
[ -f img.jpg ] || wget $img -O img.jpg

failed=0

TMPFILES="tmp.pdf tmp.html tmp.css tmp2.html tmp.jar tmp1.pdf tmp2.pdf wkhtmltopdf/"

for test in $TESTS; do
    rm -rf $TMPFILES
    eval test$test $test
done
rm -rf $TMPFILES
exit $failed
