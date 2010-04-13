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

# This script will compile static versions of wkhtmltopdf for linux and for windows (it must run in linux)
# It requires build-essential and wine to run. Please note that it will take quite a while

#Configuration for the static build

QT=qt-all-opensource-src-4.5.1
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

function usage() {
    echo "please specify what static binary you want build (linux, win or all)"
    exit 1
}

BASE=${PWD}
BUILD=${BASE}/static-build

VERSION=$2

function checkout() {
    #Create static build directory
    mkdir -p $BUILD

    cd ${BUILD}
    #Fetch most recent version of qt
    echo "Updating qt from remote"
    if [ ! -d qt ] ; then
	git clone git://gitorious.org/+wkhtml2pdf/qt/wkhtmltopdf-qt.git qt || (rm -rf qt && exit 1)
    fi
    cd qt
    git checkout staging -f || exit 1
	git reset --hard || exit 1
    git pull origin staging -f || exit 1
    #Fetch and unpack upx
    get http://upx.sourceforge.net/download/${UPX}.tar.bz2 ${UPX}.tar.bz2 || exit 1
    unpack ${UPX}.tar.bz2 || exit 1
}

function setup_build() {
    echo "Updating QT"
    [ -d qts ] || git clone ${BUILD}/qt qts || (rm -rf qts && exit 1)
    cd qts
	git checkout origin/staging -f || exit 1
	git reset --hard || exit 1
    git pull origin staging -f || exit 1
    if ! [ -z "$VERSION" ] ; then
	git checkout wkhtmltopdf-$VERSION || exit 1
    fi
    touch conf
    cat ${BASE}/static_qt_conf_base ${BASE}/static_qt_conf_${1} | sed -re 's/#.*//' | sed -re '/^[ \t]*$/d' | sort -u > conf_new
  
    cd ..

    echo "Updating wkhtmltopdf"
    [ -d wkhtmltopdf ] || git clone ${BASE} wkhtmltopdf || (rm -rf wkhtmltopdf && exit 1)
    cd wkhtmltopdf
    git checkout "$(cd ${BASE} && git branch --no-color | sed -nre 's/\* //p')" -f || exit 1
    git pull || exit 1
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

if ! cmp conf conf_new; then
   echo "Configuring QT"
   (yes yes | ./configure \`cat conf_new\` -prefix "\${HERE}/qt" && cp conf_new conf) || exit 1
fi
    
if ! make -j3 -q; then
   echo "Building QT"
   make -j3 || exit 1
   make install || exit 1
fi
cd ../wkhtmltopdf

echo "Building wkhtmltopdfe"
../qt/bin/qmake || exit 1
make clean || exit 1
make -j3 || exit 1
strip wkhtmltopdf || exit 1
EOF
    chmod +x build.sh
}

function build_linux_local() {
    cd ${BUILD}
    mkdir -p linux-local
    cd linux-local
    setup_build linux
    ./build.sh || exit 1
    cd ..
    ${BUILD}/${UPX}/upx --best ${BUILD}/linux-local/wkhtmltopdf/wkhtmltopdf -o ${BASE}/wkhtmltopdf || exit 1
}

function setup_chroot() {
    if [ ! -f linux-$2/strapped ]; then
	sudo rm -rf linux-$2
	(sudo debootstrap --arch=$2 --variant=buildd $1 ./linux-$2 http://ftp.us.debian.org/debian && sudo touch linux-$2/strapped) || exit 1
    fi
    if [ ! -d linux-$2/build ]; then
	sudo mkdir -p linux-$2/build || exit 1
	sudo chown --reference=. linux-$2/build -Rv || exit 1
    fi
    
    if [ ! -f linux-$2/installed ]; then
	echo -e "deb http://ftp.debian.org $1 main non-free contrib\ndeb-src http://ftp.debian.org $1 main non-free contrib" | sudo tee linux-$2/etc/apt/sources.list || exit 1
	sudo chroot linux-$2 apt-get -y update || exit 1
	sudo chroot linux-$2 apt-get -y build-dep libqt4-core && sudo touch linux-$2/installed || exit 1
    fi
    echo "cd /build && ./build.sh" > linux-$2/build/buildw.sh
    chmod +x linux-$2/build/buildw.sh || exit 1
}

function build_linux_chroot() {
    cd ${BUILD}
    setup_chroot etch $1
    cd linux-$1/build
    setup_build linux
    if [  "$1" == 'i386' ]; then
	sudo linux32 chroot ${BUILD}/linux-$1/ /build/buildw.sh || exit 1
    else
	sudo chroot ${BUILD}/linux-$1/ /build/buildw.sh || exit 1
    fi
    ${BUILD}/${UPX}/upx --best ${BUILD}/linux-$1/build/wkhtmltopdf/wkhtmltopdf -o ${BASE}/wkhtmltopdf-$1 || exit 1
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
    if ! cmp conf conf_new; then
	QTDIR=. bin/syncqt || exit 1
	(yes | wine configure.exe -I "C:\qts\include" -I "C:\mingw32\include\freetype2" `cat conf_new` -prefix "C:\qt"  && cp conf_new conf) || exit 1
    fi
    if ! wine mingw32-make -j3 -q; then
	wine mingw32-make -j3 || exit 1
	wine mingw32-make install || exit 1
    fi

    cd ../wkhtmltopdf
    wine ../qt/bin/qmake.exe wkhtmltopdf.pro -o Makefile -spec win32-g++ || exit 1
    wine mingw32-make clean || exit 1
    wine mingw32-make -j3 || exit 1
    wine strip.exe release/wkhtmltopdf.exe || exit 1
    rm -rf ${BASE}/wkhtmltopdf.exe
    ${BUILD}/${UPX}/upx --best release/wkhtmltopdf.exe -o ${BASE}/wkhtmltopdf.exe || exit 1
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
