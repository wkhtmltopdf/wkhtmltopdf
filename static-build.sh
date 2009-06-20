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

# This script will compile static vertions of wkhtmltopdf for linux and for windows (it must run in linux)
# It requires build-essential and wine to run. Please note that it will take quite a while

#Configuration for the static build
QT=qt-all-opensource-src-4.5.1
MIRROR=kent
MINGWFILES="binutils-2.19.1-mingw32-bin.tar.gz mingw32-make-3.81-20080326-3.tar.gz \
gcc-g++-3.4.5-20060117-3.tar.gz gcc-core-3.4.5-20060117-3.tar.gz w32api-3.13-mingw32-dev.tar.gz \
mingwrt-3.15.2-mingw32-dev.tar.gz mingwrt-3.15.2-mingw32-dll.tar.gz"
OPENSSL=openssl-0.9.8h-1-lib.zip
UPX=upx-3.03-i386_linux

#Helper functions
function get() {
	[ -f $2.download ] || (rm -rf $2; wget $1 -O $2 && touch $2.download)
}
function unpack() {
	[ -f $1.unpack ] || (echo "unpacking $1"; (tar -xf $1 || unzip $1) && touch $1.unpack)
}
function applypatch() {
	cmp $1 $2 && return 0;
	[ -f $2 ] && patch -R -p1 < $2;
	patch -p1 < $1 && cp $1 $2;
}

BUILD=/tmp/build
#Create static build directory
mkdir -p $BUILD
cat qt-*.patch > $BUILD/qt.patch
cat static_qt_conf_base static_qt_conf_win | sed -re 's/#.*//' | sed -re '/^[ \t]*$/d' | sort -u > $BUILD/conf_win
cat static_qt_conf_base static_qt_conf_linux | sed -re 's/#.*//' | sed -re '/^[ \t]*$/d' | sort -u > $BUILD/conf_linux

BASE=${PWD}

cd ${BUILD}
#Fetch most the vertion of qt
get http://download.qtsoftware.com/qt/source/${QT}.tar.bz2 $QT.tar.bz2 || exit 1

#Fetch an unpack upx
get http://upx.sourceforge.net/download/${UPX}.tar.bz2 ${UPX}.tar.bz2 || exit 1
unpack ${UPX}.tar.bz2 || exit 1

if [[ "$1" == "all" ]] || [[ "$1" == "linux" ]]; then

	cd ${BUILD}
	mkdir -p linux
	cd linux
	ln -s ${BUILD}/${QT}.tar.bz2 .
	unpack ${QT}.tar.bz2 || exit 1
	cd ${QT}
	if ! cmp ${BUILD}/conf_linux conf_linux; then
 		(yes yes | ./configure `cat ${BUILD}/conf_linux` -prefix "${BUILD}/linux/qt" && cp ${BUILD}/conf_linux conf_linux) || exit 1
	fi
	applypatch ${BUILD}/qt.patch qt.patch || exit 1
	if ! make -j5 -q; then
 		make -j5 || exit 1
 		make install || exit 1
	fi
	cd ..
	rm -rf wkhtmltopdf
	svn export ${BASE} wkhtmltopdf --force || exit 1
	cd wkhtmltopdf
	../qt/bin/qmake || exit 1
	make -j5 || exit 1
	strip wkhtmltopdf || exit 1
	rm -rf ${BUILD}/wkhtmltopdf
	${BUILD}/${UPX}/upx --best wkhtmltopdf -o ${BASE}/wkhtmltopdf || exit 1
fi
if [[ "$1" == "all" ]] || [[ "$1" == "win" ]]; then 
	export WINEPREFIX=`pwd`/mingw
    #Setup configuration for mingw
	ln -s / "mingw/dosdevices/z:"
	cat <<EOF > tmp
REGEDIT4

[HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\Environment]
"PATH"="C:\\\\windows;C:\\\\windows\\\\system32;C:\\\\mingw\\\\bin"
"INCLUDE"="C:\\\\mingw\\\\include;C:\\\\mingw\\\\include\\\\c++\\\\3.4.5"
"LIB"="C:\\\\mingw\\\\lib"
EOF
	wine regedit tmp || exit 1
	rm -f tmp

	#Allowing the build to access files in the unix fs, is a bad idee.
	rm -rf "mingw/dosdevices/z:"

	echo "Building windows vertion of qt"
	#Install mingw
	mkdir -p mingw/drive_c/mingw
	cd mingw/drive_c/mingw
	for file in ${MINGWFILES}; do
		get http://${MIRROR}.dl.sourceforge.net/sourceforge/mingw/${file} ${file} || exit 1
		unpack ${file} || exit 1
	done
	get http://downloads.sourceforge.net/gnuwin32/${OPENSSL} ${OPENSSL} || exit 1
	unpack ${OPENSSL} || exit 1

	#Unpack, configure and build windows qt
	unset CPLUS_INCLUDE_PATH
	unset C_INCLUDE_PATH
	cd ${BUILD}/mingw/drive_c
	ln -s ${BUILD}/${QT}.tar.bz2 .
	unpack ${QT}.tar.bz2 || exit 1
	mkdir -p qt
	cp -r ${QT}/mkspecs qt
	cd ${QT}
	if ! cmp ${BUILD}/conf_win conf_win; then
		(yes | wine configure.exe `cat ${BUILD}/conf_win` -prefix "C:\qt" && cp ${BUILD}/conf_win conf_win) || exit 1
	fi
	applypatch ${BUILD}/qt.patch qt.patch || exit 1
	if ! wine mingw32-make -j5 -q; then
		wine mingw32-make -j5 || exit 1
		wine mingw32-make install || exit 1
	fi
	cd ..
	rm -rf wkhtmltopdf
	svn export ${BASE} wkhtmltopdf --force || exit 1
	cd wkhtmltopdf
	wine ../qt/bin/qmake.exe wkhtmltopdf.pro -o Makefile -spec win32-g++ || exit 1
	wine mingw32-make -j5 || exit 1
	wine strip.exe release/wkhtmltopdf.exe || exit 1
	rm -rf ${BUILD}/wkhtmltopdf.exe
	${BUILD}/${UPX}/upx --best release/wkhtmltopdf.exe -o ${BASE}/wkhtmltopdf.exe || exit 1
fi
if [[ "$1" != "all" ]] && [[ "$1" != "linux" ]] && [[ "$1" != "win" ]]; then
	echo "please specify what static binary you want build (linux, win or all)"
	exit 1
fi
