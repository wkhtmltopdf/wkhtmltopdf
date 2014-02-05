#!/bin/bash
#
# Copyright 2010, 2011, 2014 wkhtmltopdf authors
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

SUITE=squeeze

BASE=${PWD}
BUILD=${BASE}/static-build
J="$((1 + $(cat /proc/cpuinfo | grep -c processor)))"

function usage() {
	echo "Usage: $0 target"
	echo ""
	echo "Target:"
	echo "  linux-local   compile under local linux distribution"
	echo "  linux-amd64   compile under Debian $SUITE 64bit chroot"
	echo "  linux-i386    compile under Debian $SUITE 32bit chroot"
}

function setup_chroot() {
	cd ${BUILD}
	if [ ! -f linux-$1/is_strapped ]; then
		sudo rm -fr linux-$1
		sudo debootstrap --arch=$1 --variant=buildd $SUITE linux-$1 \
			http://ftp.us.debian.org/debian/ && sudo touch linux-$1/is_strapped || exit 1
	fi

	if [ ! -d linux-$1/build ]; then
		sudo mkdir -p linux-$1/build linux-$1/dist
		sudo chown --reference=${BASE} linux-$1/build linux-$1/dist -Rv || exit 1
	fi

	if [ ! -f linux-$1/is_installed ]; then
		echo -e "deb http://ftp.us.debian.org/debian/ $SUITE main non-free contrib\ndeb-src http://ftp.us.debian.org/debian/ $SUITE main non-free contrib" | sudo tee linux-$1/etc/apt/sources.list || exit 1
		sudo chroot linux-$1 apt-get -y update || exit 1
		sudo chroot linux-$1 apt-get -y build-dep libqt4-core && sudo touch linux-$1/is_installed
	fi

	sudo mkdir -p linux-$1/source
	sudo umount linux-$1/source
	sudo mount --bind ${BASE} linux-$1/source || exit 1
}

function setup_build() {
	cd ${BUILD}/linux-$1/build
	cat > build.sh <<EOF
unset LANG
unset LC_ALL
unset LANGUAGE

SRC=\$1
BLD=\$2
DST=\$3

mkdir -p \$BLD/qt \$BLD/app
if [ ! -f \$DST/qt/bin/qmake ]; then
	cd \$BLD/qt
	\$SRC/qt/configure -opensource -confirm-license -fast -release -static -graphicssystem raster -exceptions \\
		-no-accessibility -no-stl -no-sql-ibase -no-sql-mysql -no-sql-odbc -no-sql-psql -no-sql-sqlite -no-sql-sqlite2 \\
		-no-qt3support -no-xmlpatterns -no-phonon -no-phonon-backend -no-opengl -no-declarative -no-script -no-scripttools \\
		-no-mmx -no-3dnow -no-sse -no-sse2 -no-multimedia -no-dbus -qt-zlib -qt-libpng -qt-libjpeg -no-libmng -no-libtiff \\
		-openssl -xrender -webkit -largefile -rpath --prefix=\$DST/qt \\
		-nomake demos -nomake docs -nomake examples -nomake tools -nomake tests -nomake translations \\
		-no-nis -no-cups -no-iconv -no-pch -no-gtkstyle -no-nas-sound -no-sm -no-xshape -no-xinerama -no-xcursor \\
		-no-xfixes -no-xrandr -no-mitshm -no-xinput -no-xkb -no-glib -no-openvg -no-xsync -no-audio-backend \\
		-no-sse3 -no-ssse3 -no-sse4.1 -no-sse4.2 -no-avx -no-neon
	if ! make -j${J} -q; then
		echo "building QT"
		make -j${J} || exit 1
	fi
	make -j${J} install
fi
cd \$BLD/app
cp \$BLD/qt/.qmake.cache .
GIT_DIR=\$SRC/.git \$DST/qt/bin/qmake \$SRC/wkhtmltopdf.pro
make -j${J} || exit 1
strip bin/wkhtmltopdf bin/wkhtmltoimage
rm -fR \$DST/wkhtmltox
mkdir -p \$DST/wkhtmltox \$DST/wkhtmltox/bin \$DST/wkhtmltox/lib \$DST/wkhtmltox/include/wkhtmltox
cp \$BLD/app/bin/wkhtmlto* \$DST/wkhtmltox/bin
cp -P \$BLD/app/bin/libwkhtmltox*.so.* \$DST/wkhtmltox/lib
cp \$SRC/include/wkhtmltox/*.h \$DST/wkhtmltox/include/wkhtmltox
cp \$SRC/include/wkhtmltox/dll*.inc \$DST/wkhtmltox/include/wkhtmltox
EOF
	chmod +x build.sh
}

function package_build() {
	cd ${BUILD}
	RELEASE_VERSION=`cat $BASE/VERSION`-`git --git-dir=$BASE/.git rev-parse --short HEAD`
	cd ${BUILD}/linux-$1/dist
	tar -c -v --xz -f ${BUILD}/wkhtmltox-linux-$1_${RELEASE_VERSION}.tar.xz wkhtmltox/
}

mkdir -p ${BUILD}
case "$1" in
"linux-i386")
	setup_chroot  i386
	setup_build   i386
	sudo linux32 chroot ${BUILD}/linux-i386/ /build/build.sh /source /build /dist || exit 1
	sudo umount ${BUILD}/linux-$1/source
	package_build i386
	;;
'linux-amd64')
	setup_chroot  amd64
	setup_build   amd64
	sudo chroot ${BUILD}/linux-amd64/ /build/build.sh  /source /build /dist || exit 1
	sudo umount ${BUILD}/linux-$1/source
	package_build amd64
	;;
"linux-local")
	mkdir -p ${BUILD}/linux-local ${BUILD}/linux-local/build ${BUILD}/linux-local/dist
	setup_build   local
	${BUILD}/linux-local/build/build.sh ${BASE} ${BUILD}/linux-local/build ${BUILD}/linux-local/dist || exit 1
	package_build local
	;;
*)
	usage
	;;
esac
