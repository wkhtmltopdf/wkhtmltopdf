#!/bin/bash
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

# This script will compile a static version of wkhtmltopdf for OS X

# configurable options
QT_DOWNLOAD_URL="https://api.github.com/repos/wkhtmltopdf/qt/tarball"
BUILD_DIR_BASE="static-build"
PLATFORM_NAME="macosx-$(sw_vers -productVersion)"

function usage() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "-h                  Display this help message"
    echo "-r <architecture>   Create a release build/bundle for this architecture (e.g. universal, i386, x86_64)"
}

# default non-release
CREATE_RELEASE=0

while getopts "hr:" opt; do
    case $opt in
        h)
            usage
            exit 1
            ;;
        r)
            # shift 2
            ARCH=$OPTARG
            # set up config options
            if [ "$ARCH" == "universal" ]; then
                CONFIG_OPTS_ARCH="-arch i386 -arch x86_64"
            else
                CONFIG_OPTS_ARCH="-arch ${ARCH}"
            fi
            BUILD_DIR=$BUILD_DIR_BASE/$PLATFORM_NAME-$OPTARG
            CREATE_RELEASE=1
            ;;
        \?)
            echo "Invalid option: -$OPTARG"
            exit 1
            ;;
    esac
done

# store current dir for reference
WKHTMLTOPDF_DIR=$(pwd)

# build processes is number of cores
J=$(sysctl -n hw.ncpu)

# libc++ for mavericks and higher, otherwise clang
MAVERICKS=$(uname -r | awk -F. '$1 >= 13')
if ! [ -z "$MAVERICKS" ]; then
    PLATFORM="unsupported/macx-clang-libc++"
else
    PLATFORM="unsupported/macx-clang"
fi

# is qt present? a master zip download from GitHub or the cloned repo won't contain the qt files yet - pull qt submodule or download qt zip if needed
if ! [ -f "qt/configure" ]; then
    if [ -d ".git" ]; then
        echo "Updating submodules (QT)"
        git submodule init || exit 1
        git submodule update --recursive || exit 1
    else
        echo "Downloading QT"
        curl -L -o qt.tar.gz ${QT_DOWNLOAD_URL} || exit 1
        echo "Extracting QT"
        tar --strip 1 -C qt/ -xzf qt.tar.gz || exit 1
    fi
fi

# separate build directory?
if ! [ -z "$BUILD_DIR" ]; then
    echo "Setting up separate build directory in ${BUILD_DIR}"
    mkdir -p "${BUILD_DIR}" || exit 1

    # are we working with a git repo?
    if [ -d ".git" ]; then
        git clone ./ "${BUILD_DIR}" || exit 1
        cd "${BUILD_DIR}" || exit 1

        # use local data for qt repo so we don't have to re-fetch from origin
        echo "Copying QT to separate build dir"
        git config -f .gitmodules --replace-all submodule.qt.url "${WKHTMLTOPDF_DIR}/qt" || exit 1
        git submodule init || exit 1
        git submodule update --recursive || exit 1
    else
        echo "Copying files to separate build directory"
        rsync -av --exclude="qt.tar.gz" --exclude="${BUILD_DIR_BASE}" ./ "${BUILD_DIR}" || exit 1
        cd "${BUILD_DIR}" || exit 1
    fi
fi

cd qt

# set up configure options
CONFIG_OPTS="-opensource -confirm-license -prefix ./ -confirm-license -opensource -qt-zlib -qt-libmng -qt-libtiff -qt-libpng -qt-libjpeg -static -cocoa -fast -release -nomake demos -nomake examples -nomake tools -nomake docs -nomake translations -nomake tests -xmlpatterns -webkit -xrender -openssl -largefile -rpath -graphicssystem raster -no-qt3support -no-dbus -no-javascript-jit -no-sql-ibase -no-sql-mysql -no-sql-odbc -no-sql-psql -no-sql-sqlite -no-sql-sqlite2 -no-3dnow -no-ssse3 -no-sse2 -no-sse -no-mmx -no-dwarf2 -no-framework -no-nis -no-cups -no-iconv -no-pch -no-gtkstyle -no-nas-sound -no-sm -no-xshape -no-xinerama -no-xfixes -no-xrandr -no-xrender -no-mitshm -no-xkb -no-glib -no-openvg -no-opengl -no-xsync -no-egl -no-phonon -no-phonon-backend -no-scripttools -no-multimedia -no-declarative -no-accessibility -no-stl"
if ! [ -z "$CONFIG_OPTS_ARCH" ]; then
    CONFIG_OPTS="${CONFIG_OPTS} ${CONFIG_OPTS_ARCH}"
fi
if ! [ -z "$PLATFORM" ]; then
    CONFIG_OPTS="${CONFIG_OPTS} -platform ${PLATFORM}"
fi
echo "Configuring QT"
make confclean
./configure ${CONFIG_OPTS} || exit 1

echo "Building QT"
make -j${J} || exit 1

cd ..
echo "Building wkthmltopdf"
make distclean
qt/bin/qmake || exit 1
make -j${J} || exit 1

cd src/lib && make staticlib
cd -

# fix arch for examples if needed
if ! [ -z "$CONFIG_OPTS_ARCH" ]; then
    if ! [ -f examples/Makefile.orig ]; then
        cp examples/Makefile examples/Makefile.orig
    fi
    cat examples/Makefile.orig | sed -e "s/CFLAGS=/CFLAGS=${CONFIG_OPTS_ARCH}/" > examples/Makefile
fi

# build examples
echo "Building examples"
cd examples && make
cd -

# create release?
if [ "$CREATE_RELEASE" -eq "1" ]; then
    echo "Creating release"
    RELEASE_VERSION=$(cat VERSION | tr -d '\n')
    if [ -d ".git" ]; then
        RELEASE_VERSION="${RELEASE_VERSION}-$(git rev-parse --short HEAD)"
    fi
    INSTALL_ROOT=${WKHTMLTOPDF_DIR}/${BUILD_DIR_BASE}/install-${PLATFORM_NAME}-${ARCH}-${RELEASE_VERSION}
    mkdir -p ${INSTALL_ROOT}
    # install
    INSTALL_ROOT=${INSTALL_ROOT} make install
    # build tarball
    RELEASE_TARBALL="${BUILD_DIR_BASE}/wkhtmltox-${PLATFORM_NAME}-${ARCH}-${RELEASE_VERSION}.tar.xz"
    cd "${INSTALL_ROOT}" && tar -cJvf "${WKHTMLTOPDF_DIR}/${RELEASE_TARBALL}" * || exit 1
    echo "Release tarball created: ${RELEASE_TARBALL}"
fi

