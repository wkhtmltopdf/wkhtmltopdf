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

# This script will compile static versions of wkhtmltopdf for linux and for windows (it must run in linux)
# It requires build-essential and wine to run. Please note that it will take quite a while

#Configuration for the static build

MIRROR=kent
MINGWFILES="binutils-2.19.1-mingw32-bin.tar.gz \
mingw32-make-3.81-20090910.tar.gz \
gcc-full-4.4.0-mingw32-bin-2.tar.lzma \
w32api-3.13-mingw32-dev.tar.gz \
mingwrt-3.16-mingw32-dev.tar.gz \
mingwrt-3.16-mingw32-dll.tar.gz"

GNUWIN32FILES="openssl-0.9.8h-1-lib.zip \
openssl-0.9.8h-1-bin.zip "
#freetype-2.3.5-1-bin.zip \
#freetype-2.3.5-1-lib.zip "
QTBRANCH=
J="$((1 + $(cat /proc/cpuinfo | grep -c processor)))"

function usage() {
	echo "Usage: $0 [OPTIONS] target"
	echo ""
	echo "Options:"
	echo "-h            Display this help message"
	echo "-q branch     Use this qt branch"
	echo "-v version    Compile this version of wkhtmltopdf"
	echo ""
	echo "Target:"
	echo "linux-local   Compile under local linux distro"
	echo "linux-amd64   Compile under debian 64bit chroot"
	echo "linux-i368    Compile under debian 32bit chroot"
	echo "windows       Compile windows binary using wine"
}
VERSION=""
QTBRANCH="staging"

while getopts hq:v: O; do
	case "$O" in
		[h?])
			usage
			exit 1
			;;
		v)
			shift 2
			VERSION=$OPTARG
			;;
		q)
			shift 2
			QTBRANCH=$OPTARG
			;;
	esac
done


if file /bin/true | grep -q 64-bit; then
    UPX=upx-3.03-amd64_linux
else
    UPX=upx-3.03-i386_linux
fi

#Helper functions
function get() {
    [ -f $2.download ] || (rm -rf $2; wget $1 -O $2 && touch $2.download)
}
function unpack() {
    [ -f $1.unpack ] || (echo "unpacking $1"; (tar -xf $1 || unzip -o $1) && touch $1.unpack)
}


BASE=${PWD}
BUILD=${BASE}/static-build

VERSION=$2

function git_fetch_and_update() {
    if [ ! -d "$1" ]; then
	git clone "$2" "$1" || (rm -rf "$1" && return 1)
    fi
    cd "$1"
    git fetch origin
    if ! (git checkout "$3" -f 2>/dev/null && git pull origin "$3" 2>/dev/null); then
	git branch -a
	git checkout origin/"$3" -f 2>/dev/null || return 1
	git branch -D "$3" 2>/dev/null
	git checkout origin/"$3" -b "$3" || return 1
	git pull origin "$3" -f || return 1
    fi
    cd ..
}


function checkout() {
    #Create static build directory
    mkdir -p $BUILD
    cd ${BUILD}

    #Fetch and unpack upx
    get http://upx.sourceforge.net/download/${UPX}.tar.bz2 ${UPX}.tar.bz2 || exit 1
    unpack ${UPX}.tar.bz2 || exit 1

    #Fetch most recent version of qt
    echo "Updating qt from remote"
    git_fetch_and_update qt git://gitorious.org/+wkhtml2pdf/qt/wkhtmltopdf-qt.git "$QTBRANCH" || exit 1
    cd qt
}

function setup_build() {
    echo "Updating QT"
	git_fetch_and_update qts ${BUILD}/qt "$QTBRANCH" || exit 1
    cd qts
    if ! [ -z "$VERSION" ] ; then
		git checkout wkhtmltopdf-$VERSION || exit 1
    fi
    touch conf
    cat ${BASE}/static_qt_conf_base ${BASE}/static_qt_conf_${1} | sed -re 's/#.*//' | sed -re '/^[ \t]*$/d' | sort -u > conf_new
    cd ..

    echo "$(cd ${BASE} && git branch --no-color | sed -nre 's/\* //p')"
	git_fetch_and_update wkhtmltopdf "${BASE}" "$(cd ${BASE} && git branch --no-color | sed -nre 's/\* //p')" || exit 1
	cd wkhtmltopdf
    if ! [ -z "$VERSION" ] ; then
		git checkout "$VERSION" || exit 1
    fi
    cd ..
    [ "$1" == "win" ] && return
    cat > build.sh <<EOF
unset LANG
unset LC_ALL
unset LANGUAGE
HERE="\${PWD}"
mkdir -p qt/lib

cd qts

function do_configure() {
   echo "Configuring QT"
   (yes yes | ./configure \`cat conf_new\` -prefix "\${HERE}/qt" && cp conf_new conf) || exit 1
}

if ! cmp conf conf_new; then
  QTDIR=. bin/syncqt || exit 1
  do_configure
fi

if ! make -j${J} -q; then
   echo "Building QT"
   (make -j${J} && make install) || (make distclean; do_configure && make -j${J} && make install) || exit 1
fi
cd ../wkhtmltopdf

echo "Building wkhtmltopdfe"
(make distclean; ../qt/bin/qmake && make -j${J}) || exit 1
strip ./bin/wkhtmltopdf || exit 1
strip ./bin/wkhtmltoimage || exit 1
EOF
    chmod +x build.sh
}

function packandcopylinux() {
    WK=${BUILD}/linux-$1/build/wkhtmltopdf
    rm -rf  ${BASE}/bin/wkhtmltopdf-$1  ${BASE}/bin/wkhtmltoimage-$1 ${BASE}/bin/libwkhtmltopdf-$1.tar.lzma
    ${BUILD}/${UPX}/upx --best ${WK}/bin/wkhtmltopdf -o ${BASE}/bin/wkhtmltopdf-$1 || exit 1
    ${BUILD}/${UPX}/upx --best ${WK}/bin/wkhtmltoimage -o ${BASE}/bin/wkhtmltoimage-$1 || exit 1
    rm -rf ${WK}/lib
    mkdir -p ${WK}/lib
    cp ${WK}/bin/libwkhtmltox*.so ${WK}/lib || exit 1
    cd ${WK} && tar -c --lzma -f ${BASE}/bin/libwkhtmltox-$1.tar.lzma lib include examples/Makefile examples/pdf_c_api.c
}


function build_linux_local() {
    cd ${BUILD}
    mkdir -p linux-local
    cd linux-local
    setup_build linux
    ./build.sh || exit 1
    cd ..
    packandcopylinux local
}

function setup_chroot() {
    if [ ! -f linux-$2/strapped ]; then
	sudo rm -rf linux-$2
	(sudo debootstrap --arch=$2 --variant=buildd $1 ./linux-$2 http://ftp.us.debian.org/debian/ && sudo touch linux-$2/strapped) || exit 1
    fi
    if [ ! -d linux-$2/build ]; then
	sudo mkdir -p linux-$2/build || exit 1
	sudo chown --reference=. linux-$2/build -Rv || exit 1
    fi

    if [ ! -f linux-$2/installed ]; then
	echo -e "deb http://ftp.us.debian.org/debian/ $1 main non-free contrib\ndeb-src http://ftp.us.debian.org/debian/ $1 main non-free contrib" | sudo tee linux-$2/etc/apt/sources.list || exit 1
	sudo chroot linux-$2 apt-get -y update || exit 1
	sudo chroot linux-$2 apt-get -y build-dep libqt4-core && sudo touch linux-$2/installed || exit 1
    fi
    echo "cd /build && ./build.sh" > linux-$2/build/buildw.sh
    chmod +x linux-$2/build/buildw.sh || exit 1
}

function build_linux_chroot() {
    cd ${BUILD}
    setup_chroot lenny $1
    cd linux-$1/build
    setup_build linux
    if [  "$1" == 'i386' ]; then
	sudo linux32 chroot ${BUILD}/linux-$1/ /build/buildw.sh || exit 1
    else
	sudo chroot ${BUILD}/linux-$1/ /build/buildw.sh || exit 1
    fi
    packandcopylinux $1
}

function build_windows() {
    cd ${BUILD}

    export WINEPREFIX=${BUILD}/windows
    if [ ! -f ${BUILD}/windows/create ]; then
	    cat > tmp <<EOF
REGEDIT4

[HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\Environment]
"PATH"="C:\\\\windows;C:\\\\windows\\\\system32;C:\\\\mingw\\\\bin"
"INCLUDE"="C:\\\\mingw\\\\include;C:\\\\mingw\\\\include\\\\c++\\\\3.4.5"
"LIB"="C:\\\\mingw\\\\lib"
EOF
	    wine regedit tmp || exit 1
	    touch ${BUILD}/windows/create
    fi
#http://ftp.gnome.org/pub/gnome/binaries/win32/dependencies/fontconfig-2.4.2-tml-20071015.zip
    mkdir -p windows/drive_c/mingw
    cd windows/drive_c/mingw
    for file in ${MINGWFILES}; do
 		get http://${MIRROR}.dl.sourceforge.net/sourceforge/mingw/${file} ${file} || exit 1
 		unpack ${file} || exit 1
    done
    for file in ${GNUWIN32FILES}; do
		get http://downloads.sourceforge.net/gnuwin32/${file} ${file} || exit 1
		unpack ${file} || exit 1
    done

    cd ..
    setup_build win

    unset CPLUS_INCLUDE_PATH
    unset C_INCLUDE_PATH
    export CPLUS_INCLUDE_PATH=
    export C_INCLUDE_PATH=C:\qts\include

    mkdir -p qt
    cp -r qts/mkspecs qt
    cd qts
    if ! [ -f Makefile ] || ! cmp conf conf_new; then
		QTDIR=. bin/syncqt || exit 1
		(yes | wine configure.exe -I "C:\qts\include" -I "C:\mingw32\include\freetype2" `cat conf_new` -prefix "C:\qt" && cp conf_new conf) || exit 1
    fi
    if ! wine mingw32-make -j${J} -q; then
		wine mingw32-make -j${J} || exit 1
		wine mingw32-make install || exit 1
    fi

    cd ../wkhtmltopdf
    wine mingw32-make dist-clean
    wine ../qt/bin/qmake.exe wkhtmltopdf.pro -o Makefile -spec win32-g++ || exit 1
    wine mingw32-make clean || exit 1
    wine mingw32-make -j${J} || exit 1
    wine strip.exe bin/wkhtmltopdf.exe || exit 1
    wine strip.exe bin/wkhtmltoimage.exe || exit 1

    rm -rf  ${BASE}/bin/wkhtmltopdf.exe  ${BASE}/bin/wkhtmltoimage.exe ${BASE}/bin/libwkhtmltox.zip
    ${BUILD}/${UPX}/upx --best bin/wkhtmltopdf.exe -o ${BASE}/bin/wkhtmltopdf.exe || exit 1
    ${BUILD}/${UPX}/upx --best bin/wkhtmltoimage.exe -o ${BASE}/bin/wkhtmltoimage.exe || exit 1
    rm -rf lib
    mkdir -p lib
    cp bin/wkhtmltox*.dll lib || exit 1
    zip -9 ${BASE}/bin/libwkhtmltox.zip  lib include examples/Makefile examples/pdf_c_api.c
}

case "$1" in
'linux-local')
	checkout
	build_linux_local
	;;
'linux-i386')
	checkout
	build_linux_chroot i386
	;;
'linux-amd64')
	checkout
	build_linux_chroot amd64
	;;
'windows')
	checkout
	build_windows
	;;
'release')
	checkout
	build_linux_chroot i386
	build_linux_chroot amd64
	build_windows
	;;
*)
	usage
	;;
esac
