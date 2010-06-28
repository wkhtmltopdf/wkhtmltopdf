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

function usage() {
    echo "Usage $0: [Options] Major Minor Patch [Build]"
    echo ""
    echo "Options:"
    echo "-h           Display this help message"
    echo "-q           Build against this branch of QT"
}

while getopts hq: O; do
    case "$O" in
	[?h])
	    usage;
	    exit 1
	    ;;
	q)
	    shift 2
	    QB="-q $OPTARG"
	    ;;
    esac
done

git status
if [[ $1 == "" ]] || [[ $2 == "" ]] || [[ $3 == "" ]]; then
    echo "Bad version"
    exit 1
fi
v="$1.$2.$3"
if [[ "$4" != "" ]]; then
    v="${v}_$4"
fi

echo "About to release $v"
read -p "Are you sure you are ready: " N
[ "$N" != "YES" ] && exit

echo "DEFINES += MAJOR_VERSION=$1 MINOR_VERSION=$2 PATCH_VERSION=$3 BUILD=\"$4\"" > version.pri
HEAD="$(git log --pretty=oneline  -n 1 | sed -e 's/ .*//')"
git commit -m "TEMPORERY DO NOT COMMIT $v" version.pri

rm -rf bin
mkdir bin
./scripts/static-build.sh $QB linux-i386 || (echo Build failed; git reset $HEAD --hard; exit 1)
cp bin/wkhtmltopdf-i386 bin/wkhtmltopdf
cp bin/wkhtmltoimage-i386 bin/wkhtmltoimage
if ! ./scripts/test.sh -q; then
    echo "Test failed"
    git reset $HEAD --hard
    exit 1
fi

./bin/wkhtmltopdf-i386 --readme > README_WKHTMLTOPDF
./bin/wkhtmltoimage-i386 --readme > README_WKHTMLTOIMAGE
./scripts/static-build.sh $QB linux-amd64 || (echo Build failed; git reset $HEAD --hard; exit 1)
./scripts/static-build.sh $QB windows || (echo Build failed; git reset $HEAD --hard; exit 1)

git commit --amend -m "Version $v" version.pri README_WKHTMLTOPDF README_WKHTMLTOIMAGE
git tag "$v"

rm -rf "release-$v"
mkdir "release-$v"
./bin/wkhtmltopdf-i386 --htmldoc > "./release-$v/wkhtmltopdf-$v-doc.html"
./bin/wkhtmltoimage-i386 --htmldoc > "./release-$v/wkhtmltoimage-$v-doc.html"

git checkout-index --prefix="./release-$v/wkhtmltopdf-$v/" -a
wget "http://code.google.com/p/wkhtmltopdf/wiki/ChangeLog" -qO - | sed -nre 's/.*<p>CHANGELOGBEGIN[ ]*<\/p>(.*)<p>CHANGELOGEND.*/\1/p' | html2text -utf8 -nobs | sed -e 's/ //g' > "./release-$v/wkhtmltopdf-$v/changelog"
tar -cjvf "release-$v/wkhtmltopdf-$v.tar.bz2" -C "release-$v" "wkhtmltopdf-$v"
cp bin/wkhtmltopdf.exe "release-$v/wkhtmltopdf.exe"
cp bin/wkhtmltoimage.exe "release-$v/wkhtmltoimage.exe"
m4 -D "WKVERSION=$v" wkhtmltopdf.nsi.m4 > "release-$v/wkhtmltopdf.nsi"
cd bin
tar -cjvf "../release-$v/wkhtmltopdf-$v-static-i386.tar.bz2" wkhtmltopdf-i386
tar -cjvf "../release-$v/wkhtmltopdf-$v-static-amd64.tar.bz2" wkhtmltopdf-amd64
tar -cjvf "../release-$v/wkhtmltoimage-$v-static-i386.tar.bz2" wkhtmltoimage-i386
tar -cjvf "../release-$v/wkhtmltoimage-$v-static-amd64.tar.bz2" wkhtmltoimage-amd64
cd "../release-$v"
for x in libgcc_s_dw2-1.dll ssleay32.dll libeay32.dll mingwm10.dll EnvVarUpdate.nsh; do
	[ -f "$x" ] && continue
	l=$(find ../static-build/windows/drive_c/mingw/bin -name "$x")
	[ -f "$l" ] && cp "$l" "$x" && continue
    wget "http://wkhtmltopdf.googlecode.com/files/$x" -O "$x" && continue
done
makensis wkhtmltopdf.nsi
