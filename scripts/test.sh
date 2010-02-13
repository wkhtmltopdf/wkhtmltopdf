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

#This script will test a compiled wkhtmltopdf for some basic functionality

#This is just some random image
img=http://madalgo.au.dk/img/Forside/KatrinebjMay-05.jpg
[ -d test ] || mkdir test
cd test

[ -f img.jpg ] || wget $img -O img.jpg


export WK=../wkhtmltopdf 
failed=0

function result() { printf "%-30s [%-4s]\n" "$1" "$2";}
function good() { result "$1" " OK ";}
function bad() { result "$1" "Fail"; [ "$2" != "false" ] && export failed=$(($failed+1));}
function fs() { du -b "$1" | sed -re 's/([0-9]*).*/\1/';}
function wk() { $WK -q $*;}

#Test if we can convert a html file containing
#An image of some format
function testImgSupport() {
    [ -f img.$1 ] || convert img.jpg img.$1
    rm -rf tmp.pdf
    echo "<html><head><title>Img test $1</title></head><body><h1>The $1 image</h1><img src=\"img.$1\" /></body></html>" > tmp.html
    wk tmp.html tmp.pdf
    S=`fs tmp.pdf`
    ([ -f tmp.pdf ] && [[ $S -ge 20000 ]]) && good "$1 Suppport ($S)" || bad "$1 Support ($S)" "$2"
}

#Test if we can convert a local file, and that it has some of the right words
function testLocalFileSupport() {
    rm -rf tmp.pdf
    echo "<html><head><title>Local Test</title></head><body><h1>Hello</h1></body></html>" > tmp.html
    wk tmp.html tmp.pdf
    ([ -f tmp.pdf ] && pdftotext tmp.pdf /dev/stdout | grep -q Hello) && good LocalFile || bad LocalFile
}

function testUserStyleSheet {
    rm -f tmp.pdf tmp.html tmp.css
    echo "<html><head><title>Local Test</title></head><body><p>.</p></body></html>" > tmp.html
    echo "p:before {content: \"Hello \"}" > tmp.css
    wk tmp.html tmp.pdf --user-style-sheet tmp.css
    ([ -f tmp.pdf ] && pdftotext tmp.pdf /dev/stdout | grep -q Hello) && good UserStyle || bad UserStyle "$1"
}

function testPipeInSupport() {
    rm -rf tmp.pdf
    echo "<html><head><title>Local Test</title></head><body><h1>Hello</h1></body></html>" > tmp.html
    cat tmp.html | wk - tmp.pdf
    ([ -f tmp.pdf ] && pdftotext tmp.pdf /dev/stdout | grep -q Hello) && good PipeInFile || bad PipeInFile
}

function testPipeOutSupport() {
    rm -rf tmp.pdf
    echo "<html><head><title>Local Test</title></head><body><h1>Hello</h1></body></html>" > tmp.html
    wk tmp.html - > tmp.pdf
    ([ -f tmp.pdf ] && pdftotext tmp.pdf /dev/stdout | grep -q Hello) && good PipeOutFile || bad PipeQutFile
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
    wk https://www.google.com/accouts tmp.pdf
    [ -f tmp.pdf ] && good SSL || bad SSL
}

#Test if the header footer stuff works
function testHeaderFooter() {
    rm -rf tmp.pdf
    echo "<html><head><title>Local Test</title></head><body><h1>monster</h1></body></html>" > tmp.html
    wk tmp.html tmp.pdf --footer-left hat --header-right emacs
    ([ -f tmp.pdf ] && 
	pdftotext tmp.pdf /dev/stdout | grep -q monster &&
	pdftotext tmp.pdf /dev/stdout | grep -q emacs &&
	pdftotext tmp.pdf /dev/stdout | grep -q hat) && good HeaderFooter || bad HeaderFooter
}

function testToc() {
    echo "<html><head></head><body><h1>foo</h1><h2>bar</h2><h3>baz</h3></body>" > tmp.html
    wk tmp.html tmp.pdf --toc --toc-depth 2
    ([ -f tmp.pdf ] && 
	[ "$(pdftotext tmp.pdf /dev/stdout | grep -c foo)" == 2 ] &&
	[ "$(pdftotext tmp.pdf /dev/stdout | grep -c bar)" == 2 ] &&
	[ "$(pdftotext tmp.pdf /dev/stdout | grep -c baz)" == 1 ]) && good Toc || bad Toc 
}


function testOutline() {
    rm -rf tmp.df
    echo "<html><head></head><body><h1>foo</h1><h2>bar</h2><h3>baz</h3></body>" > tmp.html
    wk tmp.html tmp.pdf --outline --outline-depth 2
    ([ -f tmp.pdf ] && 
	cat tmp.pdf | grep -q ".f.o.o" &&
	cat tmp.pdf | grep -q ".b.a.r" &&
	! cat tmp.pdf | grep -q ".b.a.z") && good Outline || bad Outline
}

function testJSRedirect() {
    wk http://madalgo.au.dk/~jakobt/jsredirect.html tmp.pdf
    ([ -f tmp.pdf ] && 
	pdftotext tmp.pdf /dev/stdout | grep -q Right) && good JSRedicet || bad JSRedirect
}

function testServersideRedirect() {
    wk http://madalgo.au.dk/~jakobt/redirect.php tmp.pdf
    ([ -f tmp.pdf ] && 
	pdftotext tmp.pdf /dev/stdout | grep -q Right) && good ServersideRedicet || bad ServersideRedirect
}

function test404() {
    wk http://madalgo.au.dk/~jakobt/nosuchfile.nsf tmp.pdf && bad 404 || good 404
}

function testBadDest() {
    echo "<html><head></head><body><h1>foo</h1><h2>bar</h2><h3>baz</h3></body>" > tmp.html
    (! wk tmp.html /proc/cpuinfo 2> tmp.out && grep -q "Error" tmp.out) && good BadDest || bad BadDest
}

function testBadSource() {
    (! wk http://nosuchdomain.nosuchtld tmp.pdf 2> tmp.out && grep -q "Error" tmp.out) && good BadSource || bad BadSource
}

function testMultidoc() {
    rm -rf tmp.pdf
    echo "<html><head><title>Local Test</title></head><body><h1>Hello</h1></body></html>" > tmp.html
    echo "<html><head><title>Local Test</title></head><body><h1>world</h1></body></html>" > tmp2.html
    wk tmp.html tmp2.html tmp.pdf
    ([ -f tmp.pdf ] && 
	pdftotext tmp.pdf /dev/stdout | grep -q Hello &&
	pdftotext tmp.pdf /dev/stdout | grep -q world) && good MultiDoc || bad MultiDoc
}

function testHtmlHeader() {
    echo "<html><body>Header</body></html>" > tmp.html
    echo "<html><head><title>Local Test</title></head><body><h1>world</h1></body></html>" > tmp2.html
    wk --header-html tmp.html tmp2.html tmp.pdf
    ([ -f tmp.pdf ] && 
	pdftotext tmp.pdf /dev/stdout | grep -q Header &&
	pdftotext tmp.pdf /dev/stdout | grep -q world) && good HtmlHeader || bad HtmlHeader
}

function testCustomHeader() {
    wk http://madalgo.au.dk/~jakobt/cookiewrite.php --custom-header "Cookie" "cookie=hello" tmp.pdf
    ([ -f tmp.pdf ] && 
	pdftotext tmp.pdf /dev/stdout | grep -q hello) && good CustomHeader || bad CustomHeader
}

function testCookies() {
    rm -rf tmp.jar tmp.pdf tmp2.pdf
    wk --cookie mykey myvalue1 --cookie mykey2 myvalue2 --cookie-jar tmp.jar http://cs.au.dk/~jakobt/cookie.php tmp.pdf
    wk --cookie-jar tmp.jar http://cs.au.dk/~jakobt/cookie.php tmp2.pdf
    (   [ -f tmp.pdf ] &&
	pdftotext tmp.pdf /dev/stdout | grep -q "mykey:myvalue1;" &&
	pdftotext tmp.pdf /dev/stdout | grep -q "mykey2:myvalue2;" &&
	[ -f tmp2.pdf ] &&
	pdftotext tmp2.pdf /dev/stdout | grep -q "writetest:success;") &&
    good Cookies || bad Cookies
}


function testBuild() {
    rm -rf wkhtmltopdf
    (cd .. && git checkout-index --prefix=./test/wkhtmltopdf/ -a) || (bad "Build $1 (1)" && return 1)
    cd wkhtmltopdf
    if [[ "$1" == "qmake" ]]; then
	qmake 2>/dev/null >/dev/null || (bad "Build $1 (2)" && return 1)
    else
	cmake . 2>/dev/null >/dev/null || (bad "Build $1 (2)" && return 1)
    fi
    make -j2 >/dev/null 2>/dev/null && good "Build $1" || bad "Build $1 (3)"
    cd ..
}

function testNoneStatic() {
    ([ -f wkhtmltopdf/wkhtmltopdf ] && echo "<html><body>Foo</body></html>" | wkhtmltopdf/wkhtmltopdf -q - tmp.pdf && pdftotext tmp.pdf /dev/stdout | grep -q Foo) && good "None Static" || bad "None Static"
}

function testAgsFrmStdin() {
    echo "<html><body>XFooZ</body></html>" > tmp.html
    wk --read-args-from-stdin <<EOF
-q tmp.html tmp1.pdf
-q tmp.html tmp2.pdf
EOF
    (   [ -f tmp1.pdf ] && pdftotext tmp1.pdf /dev/stdout | grep -q XFooZ &&
	[ -f tmp2.pdf ] && pdftotext tmp2.pdf /dev/stdout | grep -q XFooZ) && good "Args from stdin" || bad "Arg from stdin"
}

good TestTest
testCookies
testLocalFileSupport
testPipeInSupport
testPipeOutSupport 
testUserStyleSheet false
testToc
testOutline
testImgSupport jpg true
testImgSupport gif true
testImgSupport png true
testImgSupport mng false
testImgSupport tiff false
testJSRedirect
testServersideRedirect
test404
testBadDest
testBadSource
testMultidoc
testHtmlHeader
testCustomHeader
testRemote 
testSSL
testHeaderFooter
testBuild qmake
#testNoneStatic
testAgsFrmStdin
#testBuild cmake
#Lets clean up
rm -rf *.pdf *.html wkhtmltopdf
exit $failed 
