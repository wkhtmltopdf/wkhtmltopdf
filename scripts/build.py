#!/usr/bin/env python
#
# Copyright 2014 wkhtmltopdf authors
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

# --------------------------------------------------------------- CONFIGURATION

BUILDERS = {
    'source-tarball':        'source_tarball',
    'msvc2013-win32':        'msvc',
    'msvc2013-win64':        'msvc',
    'setup-mingw_w64':       'setup_schroot',
    'setup-schroot-centos6': 'setup_schroot',
    'setup-schroot-centos7': 'setup_schroot',
    'setup-schroot-wheezy':  'setup_schroot',
    'setup-schroot-jessie':  'setup_schroot',
    'setup-schroot-trusty':  'setup_schroot',
    'setup-schroot-precise': 'setup_schroot',
    'update-all-schroots':   'update_schroot',
    'centos6-i386':          'linux_schroot',
    'centos6-amd64':         'linux_schroot',
    'centos7-amd64':         'linux_schroot',
    'wheezy-i386':           'linux_schroot',
    'wheezy-amd64':          'linux_schroot',
    'jessie-i386':           'linux_schroot',
    'jessie-amd64':          'linux_schroot',
    'trusty-i386':           'linux_schroot',
    'trusty-amd64':          'linux_schroot',
    'precise-i386':          'linux_schroot',
    'precise-amd64':         'linux_schroot',
    'mingw-w64-cross-win32': 'mingw64_cross',
    'mingw-w64-cross-win64': 'mingw64_cross',
    'posix-local':           'posix_local',
    'osx-cocoa-x86-64':      'osx',
    'osx-carbon-i386':       'osx'
}

QT_CONFIG = {
    'common' : [
        '-opensource',
        '-confirm-license',
        '-fast',
        '-release',
        '-static',
        '-graphicssystem raster',
        '-webkit',
        '-exceptions',              # required by XmlPatterns
        '-xmlpatterns',             # required for TOC support
        '-system-zlib',
        '-system-libpng',
        '-system-libjpeg',
        '-no-libmng',
        '-no-libtiff',
        '-no-accessibility',
        '-no-stl',
        '-no-qt3support',
        '-no-phonon',
        '-no-phonon-backend',
        '-no-opengl',
        '-no-declarative',
        '-no-script',
        '-no-scripttools',
        '-no-sql-ibase',
        '-no-sql-mysql',
        '-no-sql-odbc',
        '-no-sql-psql',
        '-no-sql-sqlite',
        '-no-sql-sqlite2',
        '-no-mmx',
        '-no-3dnow',
        '-no-sse',
        '-no-sse2',
        '-no-multimedia',
        '-nomake demos',
        '-nomake docs',
        '-nomake examples',
        '-nomake tools',
        '-nomake tests',
        '-nomake translations'
    ],

    'msvc': [
        '-mp',
        '-qt-style-windows',
        '-qt-style-cleanlooks',
        '-no-style-windowsxp',
        '-no-style-windowsvista',
        '-no-style-plastique',
        '-no-style-motif',
        '-no-style-cde',
        '-openssl-linked'           # static linkage for OpenSSL
    ],

    'posix': [
        '-silent',                  # perform a silent build
        '-xrender',                 # xrender support is required
        '-largefile',
        '-iconv',                   # iconv support is required for text codecs
        '-openssl',                 # load OpenSSL binaries at runtime
        '-no-rpath',
        '-no-dbus',
        '-no-nis',
        '-no-cups',
        '-no-pch',
        '-no-gtkstyle',
        '-no-nas-sound',
        '-no-sm',
        '-no-xshape',
        '-no-xinerama',
        '-no-xcursor',
        '-no-xfixes',
        '-no-xrandr',
        '-no-mitshm',
        '-no-xinput',
        '-no-xkb',
        '-no-glib',
        '-no-gstreamer',
        '-no-icu',
        '-no-openvg',
        '-no-xsync',
        '-no-audio-backend',
        '-no-sse3',
        '-no-ssse3',
        '-no-sse4.1',
        '-no-sse4.2',
        '-no-avx',
        '-no-neon'
    ],

    'mingw-w64-cross' : [
        '-silent',                  # perform a silent build
        '-openssl-linked',          # static linkage for OpenSSL
        '-no-reduce-exports',
        '-no-rpath',
        '-xplatform win32-g++-4.6'
    ],

    'osx': [
        '-silent',                  # perform a silent build
        '-no-framework',
        '-no-dwarf2',
        '-xrender',                 # xrender support is required
        '-openssl',                 # load OpenSSL binaries at runtime
        '-largefile',
        '-no-rpath'
    ]
}

FPM_SETUP = {
    'common': {
        '--name':        'wkhtmltox',
        '--description': 'convert HTML to PDF and various image formats using QtWebkit',
        '--version':     '$1',
        '--license':     'LGPLv3',
        '--vendor':      'wkhtmltopdf.org',
        '--url':         'http://wkhtmltopdf.org',
        '--maintainer':  'Ashish Kulkarni <kulkarni.ashish@gmail.com>',
        '-s':            'dir',
        '-C':            'dist',
        '--prefix':      '/usr/local',
        '--category':    'utils'
    },
    'wheezy': {
        '-t':                'deb',
        '--deb-compression': 'xz',
        '--provides':        'wkhtmltopdf',
        '--conflicts':       'wkhtmltopdf',
        '--replaces':        'wkhtmltopdf',
        '--depends':         ['fontconfig', 'libfontconfig1', 'libfreetype6', 'libpng12-0', 'zlib1g', 'libjpeg8', 'libssl1.0.0',
                              'libx11-6', 'libxext6', 'libxrender1', 'xfonts-base', 'xfonts-75dpi', 'libstdc++6', 'libc6']
    },
    'jessie': {
        '-t':                'deb',
        '--deb-compression': 'xz',
        '--provides':        'wkhtmltopdf',
        '--conflicts':       'wkhtmltopdf',
        '--replaces':        'wkhtmltopdf',
        '--depends':         ['fontconfig', 'libfontconfig1', 'libfreetype6', 'libpng12-0', 'zlib1g', 'libjpeg62-turbo', 'libssl1.0.0',
                              'libx11-6', 'libxext6', 'libxrender1', 'xfonts-base', 'xfonts-75dpi', 'libstdc++6', 'libc6']
    },
    'trusty': {
        '-t':                'deb',
        '--deb-compression': 'xz',
        '--provides':        'wkhtmltopdf',
        '--conflicts':       'wkhtmltopdf',
        '--replaces':        'wkhtmltopdf',
        '--depends':         ['fontconfig', 'libfontconfig1', 'libfreetype6', 'libpng12-0', 'zlib1g', 'libjpeg-turbo8', 'libssl1.0.0',
                              'libx11-6', 'libxext6', 'libxrender1', 'xfonts-base', 'xfonts-75dpi', 'libstdc++6', 'libc6']
    },
    'precise': {
        '-t':                'deb',
        '--deb-compression': 'xz',
        '--provides':        'wkhtmltopdf',
        '--conflicts':       'wkhtmltopdf',
        '--replaces':        'wkhtmltopdf',
        '--depends':         ['fontconfig', 'libfontconfig1', 'libfreetype6', 'libpng12-0', 'zlib1g', 'libjpeg8', 'libssl1.0.0',
                              'libx11-6', 'libxext6', 'libxrender1', 'xfonts-base', 'xfonts-75dpi', 'libstdc++6', 'libc6']
    },
    'centos6': {
        '-t':                'rpm',
        '--epoch':           '1',
        '--rpm-compression': 'bzip2',
        '--depends':         ['fontconfig', 'freetype', 'libpng', 'zlib', 'libjpeg', 'openssl', 'libstdc++', 'glibc',
                              'libX11', 'libXext', 'libXrender', 'xorg-x11-fonts-Type1', 'xorg-x11-fonts-75dpi']
    },
    'centos7': {
        '-t':                'rpm',
        '--epoch':           '1',
        '--rpm-compression': 'xz',
        '--depends':         ['fontconfig', 'freetype', 'libpng', 'zlib', 'libjpeg-turbo', 'openssl', 'libstdc++', 'glibc',
                              'libX11', 'libXext', 'libXrender', 'xorg-x11-fonts-Type1', 'xorg-x11-fonts-75dpi']
    },
    'osx': {
        '-t':                         'osxpkg',
        '-C':                         'pkg',
        '--prefix':                   '/usr/local/share/wkhtmltox-installer',
        '--osxpkg-identifier-prefix': 'org.wkhtmltopdf',
        '--after-install':            'extract.sh'
    }
}

CHROOT_SETUP  = {
    'wheezy': [
        ('debootstrap', 'wheezy', 'http://ftp.us.debian.org/debian/'),
        ('write_file', 'etc/apt/sources.list', """
deb http://ftp.debian.org/debian/ wheezy         main contrib non-free
deb http://ftp.debian.org/debian/ wheezy-updates main contrib non-free
deb http://security.debian.org/   wheezy/updates main contrib non-free"""),
        ('shell', 'apt-get update'),
        ('shell', 'apt-get dist-upgrade --assume-yes'),
        ('shell', 'apt-get install --assume-yes xz-utils libssl-dev libpng-dev libjpeg8-dev zlib1g-dev rubygems'),
        ('shell', 'apt-get install --assume-yes libfontconfig1-dev libfreetype6-dev libx11-dev libxext-dev libxrender-dev'),
        ('shell', 'gem install fpm --no-ri --no-rdoc'),
        ('write_file', 'update.sh', 'apt-get update\napt-get dist-upgrade --assume-yes\ngem update fpm\n'),
        ('fpm_setup',  'fpm_package.sh'),
        ('schroot_conf', 'Debian Wheezy')
    ],

    'jessie': [
        ('debootstrap', 'jessie', 'http://ftp.us.debian.org/debian/'),
        ('write_file', 'etc/apt/sources.list', """
deb http://ftp.debian.org/debian/ jessie         main contrib non-free
deb http://ftp.debian.org/debian/ jessie-updates main contrib non-free
deb http://security.debian.org/   jessie/updates main contrib non-free"""),
        ('shell', 'apt-get update'),
        ('shell', 'apt-get dist-upgrade --assume-yes'),
        ('shell', 'apt-get install --assume-yes xz-utils libssl-dev libpng-dev libjpeg62-turbo-dev zlib1g-dev rubygems ruby-dev'),
        ('shell', 'apt-get install --assume-yes libfontconfig1-dev libfreetype6-dev libx11-dev libxext-dev libxrender-dev'),
        ('shell', 'gem install fpm --no-ri --no-rdoc'),
        ('write_file', 'update.sh', 'apt-get update\napt-get dist-upgrade --assume-yes\ngem update fpm\n'),
        ('fpm_setup',  'fpm_package.sh'),
        ('schroot_conf', 'Debian Jessie')
    ],

    'trusty': [
        ('debootstrap', 'trusty', 'http://archive.ubuntu.com/ubuntu/'),
        ('write_file', 'etc/apt/sources.list', """
deb http://archive.ubuntu.com/ubuntu/ trusty          main restricted universe multiverse
deb http://archive.ubuntu.com/ubuntu/ trusty-updates  main restricted universe multiverse
deb http://archive.ubuntu.com/ubuntu/ trusty-security main restricted universe multiverse"""),
        ('shell', 'apt-get update'),
        ('shell', 'apt-get dist-upgrade --assume-yes'),
        ('shell', 'apt-get install --assume-yes xz-utils libssl-dev libpng-dev libjpeg-turbo8-dev zlib1g-dev ruby-dev'),
        ('shell', 'apt-get install --assume-yes libfontconfig1-dev libfreetype6-dev libx11-dev libxext-dev libxrender-dev'),
        ('shell', 'gem install fpm --no-ri --no-rdoc'),
        ('write_file', 'update.sh', 'apt-get update\napt-get dist-upgrade --assume-yes\ngem update fpm\n'),
        ('fpm_setup',  'fpm_package.sh'),
        ('schroot_conf', 'Ubuntu Trusty')
    ],

    'precise': [
        ('debootstrap', 'precise', 'http://archive.ubuntu.com/ubuntu/'),
        ('write_file', 'usr/sbin/policy-rc.d', "#!/bin/bash\nexit 101\n"),
        ('shell', 'chmod a+x /usr/sbin/policy-rc.d'),
        ('write_file', 'etc/apt/sources.list', """
deb http://archive.ubuntu.com/ubuntu/ precise          main restricted universe multiverse
deb http://archive.ubuntu.com/ubuntu/ precise-updates  main restricted universe multiverse
deb http://archive.ubuntu.com/ubuntu/ precise-security main restricted universe multiverse"""),
        ('shell', 'apt-get update'),
        ('shell', 'apt-get dist-upgrade --assume-yes'),
        ('shell', 'apt-get install --assume-yes xz-utils libssl-dev libpng-dev libjpeg8-dev zlib1g-dev rubygems'),
        ('shell', 'apt-get install --assume-yes libfontconfig1-dev libfreetype6-dev libx11-dev libxext-dev libxrender-dev'),
        ('shell', 'gem install fpm --no-ri --no-rdoc'),
        ('write_file', 'update.sh', 'apt-get update\napt-get dist-upgrade --assume-yes\ngem update fpm\n'),
        ('fpm_setup',  'fpm_package.sh'),
        ('schroot_conf', 'Ubuntu Precise')
    ],

    'centos6': [
        ('rinse', 'centos-6'),
        ('shell', 'yum update -y'),
        ('append_file:amd64', 'etc/yum.conf', 'exclude = *.i?86\n'),
        ('shell', 'yum install -y gcc gcc-c++ make diffutils perl ruby-devel rubygems rpm-build libffi-devel'),
        ('shell', 'yum install -y openssl-devel libX11-devel libXrender-devel libXext-devel fontconfig-devel freetype-devel libjpeg-devel libpng-devel zlib-devel'),
        ('shell', 'gem install fpm --no-ri --no-rdoc'),
        ('write_file', 'update.sh', 'yum update -y\ngem update fpm\n'),
        ('fpm_setup',  'fpm_package.sh'),
        ('schroot_conf', 'CentOS 6')
    ],

    'centos7:amd64': [
        ('rinse', 'centos-7'),
        ('shell', 'yum install -y deltarpm'),
        ('shell', "sed -i 's/mirrorlist/#mirrorlist/g' /etc/yum.repos.d/CentOS-Base.repo"),
        ('shell', "sed -i 's/#baseurl/baseurl/g' /etc/yum.repos.d/CentOS-Base.repo"),
        ('shell', 'yum update -y'),
        ('append_file', 'etc/yum.conf', 'exclude = *.i?86\n'),
        ('shell', 'yum install -y gcc gcc-c++ make diffutils perl ruby-devel rubygems rpm-build libffi-devel'),
        ('shell', 'yum install -y openssl-devel libX11-devel libXrender-devel libXext-devel fontconfig-devel freetype-devel libjpeg-turbo-devel libpng-devel zlib-devel'),
        ('shell', 'gem install fpm --no-ri --no-rdoc'),
        ('write_file', 'update.sh', 'yum update -y\ngem update fpm\n'),
        ('fpm_setup',  'fpm_package.sh'),
        ('schroot_conf', 'CentOS 7')
    ],

    'mingw_w64:amd64': [
        ('set_alias', 'mingw-w64'),
        ('debootstrap', 'trusty', 'http://archive.ubuntu.com/ubuntu/'),
        ('write_file', 'etc/apt/sources.list', """
deb http://archive.ubuntu.com/ubuntu/ trusty          main restricted universe multiverse
deb http://archive.ubuntu.com/ubuntu/ trusty-updates  main restricted universe multiverse
deb http://archive.ubuntu.com/ubuntu/ trusty-security main restricted universe multiverse"""),
        ('shell', 'apt-get update'),
        ('shell', 'apt-get dist-upgrade --assume-yes'),
        ('shell', 'apt-get install --assume-yes build-essential mingw-w64 nsis python ruby perl gperf bison flex git'),
        ('write_file', 'update.sh', 'apt-get update\napt-get dist-upgrade --assume-yes\n'),
        ('schroot_conf', 'MinGW-w64 on Ubuntu Trusty')
    ]
}

DEPENDENT_LIBS = {
    'openssl': {
        'order' : 1,
        'url'   : 'https://openssl.org/source/openssl-1.0.2a.tar.gz',
        'sha1'  : '46ecd325b8e587fa491f6bb02ad4a9fb9f382f5f',
        'build' : {
            'msvc*-win32*': {
                'result': ['include/openssl/ssl.h', 'lib/ssleay32.lib', 'lib/libeay32.lib'],
                'replace': [('util/pl/VC-32.pl', ' /MT', ' %(cflags)s')],
                'commands': [
                    'perl Configure --openssldir=%(destdir)s VC-WIN32 no-asm',
                    'ms\\do_ms.bat',
                    'nmake /f ms\\nt.mak install'],
            },
            'msvc*-win64*': {
                'result': ['include/openssl/ssl.h', 'lib/ssleay32.lib', 'lib/libeay32.lib'],
                'replace': [('util/pl/VC-32.pl', ' /MT', ' %(cflags)s')],
                'commands': [
                    'perl Configure --openssldir=%(destdir)s VC-WIN64A',
                    'ms\\do_win64a.bat',
                    'nmake /f ms\\nt.mak install']
            },
            'mingw-w64-cross-win*': {
                'result': ['include/openssl/ssl.h', 'lib/libssl.a', 'lib/libcrypto.a'],
                'commands': [
                    'perl Configure --openssldir=%(destdir)s --cross-compile-prefix=%(mingw_w64)s- no-shared no-asm mingw64',
                    'make',
                    'make install_sw']
            }
        }
    },

    'zlib': {
        'order' : 2,
        'url'   : 'http://downloads.sourceforge.net/libpng/zlib-1.2.8.tar.gz',
        'sha1'  : 'a4d316c404ff54ca545ea71a27af7dbc29817088',
        'build' : {
            'msvc*': {
                'result': {
                    'include/zlib.h' : 'zlib.h',
                    'include/zconf.h': 'zconf.h',
                    'lib/zdll.lib'   : 'zlib.lib'
                },
                'replace':  [('win32/Makefile.msc', '-MD', '%(cflags)s')],
                'commands': ['nmake /f win32/Makefile.msc zlib.lib']
            },
            'mingw-w64-cross-win*': {
                'result': {
                    'include/zlib.h' : 'zlib.h',
                    'include/zconf.h': 'zconf.h',
                    'lib/libz.a'     : 'libz.a'
                },
                'replace':  [('win32/Makefile.gcc', 'PREFIX =', 'PREFIX = %(mingw_w64)s-')],
                'commands': ['make -f win32/Makefile.gcc']
            }
        }
    },

    'libpng': {
        'order' : 3,
        'url' : 'http://downloads.sourceforge.net/libpng/libpng-1.2.53.tar.gz',
        'sha1': '22f3cc22d26727af05d7c9a970a7d050b6761bd7',
        'build' : {
            'msvc*': {
                'result': {
                    'include/png.h'       : 'png.h',
                    'include/pngconf.h'   : 'pngconf.h',
                    'lib/libpng.lib'      : 'libpng.lib'
                },
                'replace': [
                    ('scripts/makefile.vcwin32', '-I..\\zlib', '-I..\\deplibs\\include'),
                    ('scripts/makefile.vcwin32', '..\\zlib\\zlib.lib', '..\\deplibs\\lib\\zdll.lib'),
                    ('scripts/makefile.vcwin32', '-MD', '%(cflags)s')],
                'commands': ['nmake /f scripts/makefile.vcwin32 libpng.lib']
            },
            'mingw-w64-cross-win*': {
                'result': {
                    'include/png.h'       : 'png.h',
                    'include/pngconf.h'   : 'pngconf.h',
                    'lib/libpng.a'        : 'libpng.a'
                },
                'replace': [
                    ('scripts/makefile.gcc', 'ZLIBINC = ../zlib', 'ZLIBINC = %(destdir)s/include'),
                    ('scripts/makefile.gcc', 'ZLIBLIB = ../zlib', 'ZLIBLIB = %(destdir)s/lib'),
                    ('scripts/makefile.gcc', 'CC = gcc', 'CC = %(mingw_w64)s-gcc'),
                    ('scripts/makefile.gcc', 'AR_RC = ar', 'AR_RC = %(mingw_w64)s-ar'),
                    ('scripts/makefile.gcc', 'RANLIB = ranlib', 'RANLIB = %(mingw_w64)s-ranlib')],
                'commands': ['make -f scripts/makefile.gcc libpng.a']
            },
            'osx-carbon-i386': {
                'result': ['include/png.h', 'include/pngconf.h', 'lib/libpng.a'],
                'commands': [
                    'CFLAGS="-arch i386" ./configure --disable-shared --prefix=%(destdir)s',
                    'make install']
            },
            'osx-cocoa-x86-64': {
                'result': ['include/png.h', 'include/pngconf.h', 'lib/libpng.a'],
                'commands': [
                    'CFLAGS="-arch x86_64" ./configure --disable-shared --prefix=%(destdir)s',
                    'make install']
            }
        }
    },

    'libjpeg': {
        'order' : 4,
        'url' : 'http://ijg.org/files/jpegsrc.v9a.tar.gz',
        'sha1': 'd65ed6f88d318f7380a3a5f75d578744e732daca',
        'build' : {
            'msvc*': {
                'result': {
                    'include/jpeglib.h' : 'jpeglib.h',
                    'include/jmorecfg.h': 'jmorecfg.h',
                    'include/jerror.h'  : 'jerror.h',
                    'include/jconfig.h' : 'jconfig.h',
                    'lib/libjpeg.lib'   : 'libjpeg.lib'
                },
                'replace':  [('makefile.vc', '!include <win32.mak>', ''),
                             ('makefile.vc', '$(cc)', 'cl'),
                             ('makefile.vc', '$(cflags) $(cdebug) $(cvars)', '-c -nologo -D_CRT_SECURE_NO_DEPRECATE %(cflags)s -O2 -W3')],
                'commands': [
                    'copy /y jconfig.vc jconfig.h',
                    'nmake /f makefile.vc libjpeg.lib']
            },
            'mingw-w64-cross-win*': {
                'result': ['include/jpeglib.h', 'include/jmorecfg.h', 'include/jerror.h', 'include/jconfig.h', 'lib/libjpeg.a'],
                'commands': [
                    './configure --host=%(mingw_w64)s --disable-shared --prefix=%(destdir)s',
                    'make install']
            },
            'osx-carbon-i386': {
                'result': ['include/jpeglib.h', 'include/jmorecfg.h', 'include/jerror.h', 'include/jconfig.h', 'lib/libjpeg.a'],
                'commands': [
                    'CFLAGS="-arch i386" ./configure --disable-shared --prefix=%(destdir)s',
                    'make install']
            },
            'osx-cocoa-x86-64': {
                'result': ['include/jpeglib.h', 'include/jmorecfg.h', 'include/jerror.h', 'include/jconfig.h', 'lib/libjpeg.a'],
                'commands': [
                    'CFLAGS="-arch x86_64" ./configure --disable-shared --prefix=%(destdir)s',
                    'make install']
            }
        }
    },

    'xz': {
        'order' : 5,
        'url' : 'http://tukaani.org/xz/xz-5.2.1.tar.gz',
        'sha1': '6022493efb777ff4e872b63a60be1f1e146f3c0b',
        'build' : {
            'osx*': {
                'result': ['bin/xz'],
                'commands': [
                    'CFLAGS="-arch i386 -mmacosx-version-min=10.6" ./configure --disable-nls --enable-small --disable-shared --disable-threads --prefix=%(destdir)s',
                    'make -C src/liblzma', 'make -C src/xz', 'make install-strip']
            }
        }
    }
}

EXCLUDE_SRC_TARBALL = [
    'qt/config.profiles*',
    'qt/demos*',
    'qt/dist*',
    'qt/doc*',
    'qt/examples*',
    'qt/imports*',
    'qt/templates*',
    'qt/tests*',
    'qt/translations*',
    'qt/util*',
    'qt/lib/fonts*',
    'qt/src/3rdparty/*ChangeLog*',
    'qt/src/3rdparty/ce-compat*',
    'qt/src/3rdparty/clucene*',
    'qt/src/3rdparty/fonts*',
    'qt/src/3rdparty/freetype*',
    'qt/src/3rdparty/javascriptcore*',
    'qt/src/3rdparty/libgq*',
    'qt/src/3rdparty/libmng*',
    'qt/src/3rdparty/libtiff*',
    'qt/src/3rdparty/patches*',
    'qt/src/3rdparty/phonon*',
    'qt/src/3rdparty/pixman*',
    'qt/src/3rdparty/powervr*',
    'qt/src/3rdparty/ptmalloc*',
    'qt/src/3rdparty/s60*',
    'qt/src/3rdparty/wayland*'
]

# --------------------------------------------------------------- HELPERS

import os, sys, platform, subprocess, shutil, re, fnmatch, multiprocessing, urllib, hashlib, tarfile

from os.path import exists

if platform.system() == 'Windows':
    try:
        import winreg
    except ImportError:
        import _winreg as winreg
    CPU_COUNT = max(2, multiprocessing.cpu_count()-1)   # leave one CPU free
else:
    CPU_COUNT = max(2, multiprocessing.cpu_count())

def rchop(s, e):
    if s.endswith(e):
        return s[:-len(e)]
    return s

def message(msg):
    sys.stdout.write(msg)
    sys.stdout.flush()

def error(msg):
    message(msg+'\n')
    sys.exit(1)

def shell(cmd):
    ret = os.system(cmd)
    if ret != 0:
        error("%s\ncommand failed: exit code %d" % (cmd, ret))

def chroot_shell(name, cmd):
    ret = os.system('schroot -c wkhtmltopdf-%s -- %s ' % (name, cmd))
    if ret != 0:
        error("command inside chroot failed: exit code %d" % ret)

def get_output(*cmd):
    try:
        return subprocess.check_output(cmd, stderr=subprocess.STDOUT).strip()
    except:
        return None

def rmdir(path):
    if exists(path):
        if platform.system() == 'Windows':
            shell('attrib -R %s\* /S' % path)
        shutil.rmtree(path)

def mkdir_p(path):
    if not exists(path):
        os.makedirs(path)

def get_registry_value(key, value=None):
    for mask in [0, winreg.KEY_WOW64_64KEY, winreg.KEY_WOW64_32KEY]:
        try:
            reg_key = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, key, 0, winreg.KEY_READ | mask)
            return winreg.QueryValueEx(reg_key, value)[0]
        except WindowsError:
            pass
    return None

def get_version(basedir):
    def make4digit(ver):
        while ver.count('.') < 3:
            ver += '.0'
        return ver
    mkdir_p(basedir)
    text = open(os.path.join(basedir, '..', 'VERSION'), 'r').read()
    if '-' not in text:
        return (text, make4digit(text))
    version = make4digit(text[:text.index('-')])
    os.chdir(os.path.join(basedir, '..'))
    hash = get_output('git', 'rev-parse', '--short', 'HEAD')
    if not hash:
        return (text, version)
    return ('%s-%s' % (text, hash), version)

def qt_config(key, *opts):
    input, output = [], []
    input.extend(QT_CONFIG['common'])
    input.extend(QT_CONFIG[key])
    input.extend(opts)
    cfg = os.environ.get('WKHTMLTOX_QT_CONFIG')
    if cfg:
        input.extend(cfg.split())
    for arg in input:
        if not arg.startswith('remove:-'):
            output.append(arg)
        elif arg[1+arg.index(':'):] in output:
            output.remove(arg[1+arg.index(':'):])
    return ' '.join(output)

def fpm_setup(cfg):
    input, output = {}, ''
    input.update(FPM_SETUP['common'])
    input.update(FPM_SETUP[cfg])
    for key in input:
        if type(input[key]) is list:
            for val in input[key]:
                output += ' %s "%s"' % (key, val)
        else:
            output += ' %s "%s"' % (key, input[key])
    return output, input

def download_file(url, sha1, dir):
    name = url.split('/')[-1]
    loc  = os.path.join(dir, name)
    if os.path.exists(loc):
        hash = hashlib.sha1(open(loc, 'rb').read()).hexdigest()
        if hash == sha1:
            return loc
        os.remove(loc)
        message('Checksum mismatch for %s, re-downloading.\n' % name)
    def hook(cnt, bs, total):
        pct = int(cnt*bs*100/total)
        message("\rDownloading: %s [%d%%]" % (name, pct))
    urllib.urlretrieve(url, loc, reporthook=hook)
    message("\r")
    hash = hashlib.sha1(open(loc, 'rb').read()).hexdigest()
    if hash != sha1:
        os.remove(loc)
        error('Checksum mismatch for %s, aborting.' % name)
    message("\rDownloaded: %s [checksum OK]\n" % name)
    return loc

def download_tarball(url, sha1, dir, name):
    loc = download_file(url, sha1, dir)
    tar = tarfile.open(loc)
    sub = tar.getnames()[0]
    if '/' in sub:
        sub = sub[:sub.index('/')]
    src = os.path.join(dir, sub)
    tgt = os.path.join(dir, name)
    rmdir(src)
    tar.extractall(dir)
    rmdir(tgt)
    os.rename(src, tgt)
    return tgt

def _is_compiled(dst, loc):
    present = True
    for name in loc['result']:
        if isinstance(name, tuple):
            present = present and bool([n for n in name if exists(os.path.join(dst, n))])
        else:
            present = present and exists(os.path.join(dst, name))
    return present

def build_deplibs(config, basedir, **kwargs):
    mkdir_p(os.path.join(basedir, config))

    dstdir = os.path.join(basedir, config, 'deplibs')
    vars   = {'destdir': dstdir }
    vars.update(kwargs)
    for lib in sorted(DEPENDENT_LIBS, key=lambda x: DEPENDENT_LIBS[x]['order']):
        cfg = None
        for key in DEPENDENT_LIBS[lib]['build']:
            if fnmatch.fnmatch(config, key):
                cfg = key

        if not cfg or _is_compiled(dstdir, DEPENDENT_LIBS[lib]['build'][cfg]):
            continue

        build_cfg = DEPENDENT_LIBS[lib]['build'][cfg]
        message('========== building: %s\n' % lib)
        srcdir = download_tarball(DEPENDENT_LIBS[lib]['url'], DEPENDENT_LIBS[lib]['sha1'],
                                  basedir, os.path.join(config, lib))

        for location, source, target in build_cfg.get('replace', []):
            data = open(os.path.join(srcdir, location), 'r').read()
            open(os.path.join(srcdir, location), 'w').write(data.replace(source, target % vars))

        os.chdir(srcdir)
        for command in build_cfg['commands']:
            shell(command % vars)
        if not type(build_cfg['result']) is list:
            for target in build_cfg['result']:
                mkdir_p(os.path.dirname(os.path.join(dstdir, target)))
                shutil.copy(build_cfg['result'][target], os.path.join(dstdir, target))
        os.chdir(dstdir)
        if not _is_compiled(dstdir, build_cfg):
            error("Unable to compile %s for your system, aborting." % lib)

        rmdir(srcdir)

def check_running_on_debian():
    if not sys.platform.startswith('linux') or not exists('/etc/apt/sources.list'):
        error('This can only be run on a Debian/Ubuntu distribution, aborting.')

    if os.geteuid() != 0:
        error('This script must be run as root.')

    if platform.architecture()[0] == '64bit' and 'amd64' not in ARCH:
        ARCH.insert(0, 'amd64')

PACKAGE_NAME = re.compile(r'ii\s+(.+?)\s+.*')
def install_packages(*names):
    lines = get_output('dpkg-query', '--list').split('\n')
    avail = [PACKAGE_NAME.match(line).group(1) for line in lines if PACKAGE_NAME.match(line)]
    inst  = [name for name in names if name in avail]

    if len(inst) != len(names):
        shell('apt-get update')
        shell('apt-get install --assume-yes %s' % (' '.join(names)))

# --------------------------------------------------------------- Linux chroot

ARCH = ['i386']

def check_setup_schroot(config):
    check_running_on_debian()
    login = os.environ.get('SUDO_USER') or get_output('logname')
    if not login or login == 'root':
        error('Unable to determine the login for which schroot access is to be given.')

def build_setup_schroot(config, basedir):
    install_packages('git', 'debootstrap', 'schroot', 'rinse', 'debian-archive-keyring')
    os.environ['HOME'] = '/tmp' # workaround bug in gem when home directory doesn't exist

    login  = os.environ.get('SUDO_USER') or get_output('logname')
    chroot = config[1+config.rindex('-'):]

    command_list = CHROOT_SETUP.get(chroot)
    if not command_list and ('%s:amd64' % chroot) in CHROOT_SETUP:
        command_list = CHROOT_SETUP['%s:amd64' % chroot]
        if 'i386' in ARCH:
            del ARCH[ARCH.index('i386')]

    for arch in ARCH:
        alias = '%s-%s' % (chroot, arch)
        if command_list[0][0] == 'set_alias':
            alias = command_list[0][1]
            del command_list[0]
        message('******************* %s\n' % alias)
        base_dir = os.environ.get('WKHTMLTOX_CHROOT') or '/var/chroot'
        root_dir = os.path.join(base_dir, alias)
        os.system('umount %s/proc' % root_dir)
        os.system('umount %s/sys'  % root_dir)
        rmdir(root_dir)
        mkdir_p(root_dir)
        for command in command_list:
            # handle architecture-specific commands
            name = command[0]
            if ':' in name:
                if name[1+name.rindex(':'):] != arch:
                    continue
                else:
                    name = name[:name.rindex(':')]

            # handle commands
            if name == 'debootstrap':
                shell('debootstrap --arch=%(arch)s --variant=buildd %(distro)s %(dir)s %(url)s' % {
                    'arch': arch, 'dir': root_dir, 'distro': command[1], 'url': command[2] })
                cmd = (arch == 'i386' and 'linux32 chroot' or 'chroot')
                shell('%s %s mount -t proc  proc  /proc' % (cmd, root_dir))
                shell('%s %s mount -t sysfs sysfs /sys'  % (cmd, root_dir))
            elif name == 'rinse':
                cmd = (arch == 'i386' and 'linux32 rinse' or 'rinse')
                shell('%s --arch %s --distribution %s --directory %s' % (cmd, arch, command[1], root_dir))
                shell('%s %s mount -t proc  proc  /proc' % (cmd, root_dir))
                shell('%s %s mount -t sysfs sysfs /sys'  % (cmd, root_dir))
            elif name == 'shell':
                cmd = (arch == 'i386' and 'linux32 chroot' or 'chroot')
                shell('%s %s %s' % (cmd, root_dir, command[1]))
            elif name == 'write_file':
                open(os.path.join(root_dir, command[1]), 'w').write(command[2].strip())
            elif name == 'append_file':
                open(os.path.join(root_dir, command[1]), 'a').write(command[2].strip())
            elif name == 'download_file':
                name = command[1].split('/')[-1]
                loc  = os.path.join(root_dir, command[2], name)
                if exists(loc): os.remove(loc)
                def hook(cnt, bs, total):
                    pct = int(cnt*bs*100/total)
                    message("\rDownloading: %s [%d%%]" % (name, pct))
                urllib.urlretrieve(command[1], loc, reporthook=hook)
                message("\rDownloaded: %s%s\n" % (name, ' '*10))
            elif name == 'fpm_setup':
                args, cfg = fpm_setup(chroot)
                cmd = '#!/bin/sh\nXZ_OPT=-9 fpm --force %s --package ../%s-$1_linux-%s-$2.%s .\n'
                loc = os.path.join(root_dir, command[1])
                open(loc, 'w').write(cmd % (args, cfg['--name'], chroot, cfg['-t']))
                shell('chmod a+x %s' % loc)
            elif name == 'schroot_conf':
                cfg = open('/etc/schroot/chroot.d/wkhtmltopdf-%s' % alias, 'w')
                cfg.write('[wkhtmltopdf-%s]\n' % alias)
                cfg.write('type=directory\ndirectory=%s/\n' % root_dir)
                cfg.write('description=%s %s for wkhtmltopdf\n' % (command[1], arch))
                cfg.write('users=%s\nroot-users=root\n' % login)
                if arch == 'i386' and 'amd64' in ARCH:
                    cfg.write('personality=linux32\n')
                cfg.close()
        os.system('umount %s/proc' % root_dir)
        os.system('umount %s/sys'  % root_dir)

def check_update_schroot(config):
    check_running_on_debian()
    if not get_output('schroot', '--list'):
        error('Unable to determine the list of available schroots.')

def build_update_schroot(config, basedir):
    for name in get_output('schroot', '--list').split('\n'):
        message('******************* %s\n' % name[name.index('wkhtmltopdf-'):])
        shell('schroot -c %s -- /bin/bash /update.sh' % name[name.index('wkhtmltopdf-'):])

def check_source_tarball(config):
    if not get_output('git', 'rev-parse', '--short', 'HEAD'):
        error("This can only be run inside a git checkout.")

    if not exists(os.path.join(os.getcwd(), 'qt', '.git')):
        error("Please initialize and download the Qt submodule before running this.")

def _filter_tar(info):
    name = info.name[1+info.name.index('/'):]
    if name.endswith('.git') or [p for p in EXCLUDE_SRC_TARBALL if fnmatch.fnmatch(name, p)]:
        return None

    info.uid   = info.gid   = 1000
    info.uname = info.gname = 'wkhtmltopdf'
    return info

def build_source_tarball(config, basedir):
    version, simple_version = get_version(basedir)
    root_dir = os.path.realpath(os.path.join(basedir, '..'))
    os.chdir(os.path.join(root_dir, 'qt'))
    shell('git clean -fdx')
    shell('git reset --hard HEAD')
    os.chdir(root_dir)
    shell('git clean -fdx')
    shell('git reset --hard HEAD')
    shell('git submodule update')
    open('VERSION', 'w').write(version)
    with tarfile.open('wkhtmltox-%s.tar.bz2' % version, 'w:bz2') as tar:
        tar.add('.', 'wkhtmltox-%s/' % version, filter=_filter_tar)
    shell('git reset --hard HEAD')

# --------------------------------------------------------------- MSVC (2013 only)

MSVC_LOCATION = {
    'msvc2013': 'VS120COMNTOOLS'
}
MSVC_RUNTIME = {
    'msvc2013-win32': ('df7f0a73bfa077e483e51bfb97f5e2eceedfb6a3', 'http://download.microsoft.com/download/2/E/6/2E61CFA4-993B-4DD4-91DA-3737CD5CD6E3/vcredist_x86.exe'),
    'msvc2013-win64': ('8bf41ba9eef02d30635a10433817dbb6886da5a2', 'http://download.microsoft.com/download/2/E/6/2E61CFA4-993B-4DD4-91DA-3737CD5CD6E3/vcredist_x64.exe')
}

def check_msvc(config):
    version, arch = rchop(config, '-dbg').split('-')
    env_var = MSVC_LOCATION[version]
    if not env_var in os.environ:
        error("%s does not seem to be installed." % version)

    vcdir = os.path.join(os.environ[env_var], '..', '..', 'VC')
    if not exists(os.path.join(vcdir, 'vcvarsall.bat')):
        error("%s: unable to find vcvarsall.bat" % version)

    if arch == 'win32' and not exists(os.path.join(vcdir, 'bin', 'cl.exe')):
        error("%s: unable to find the x86 compiler" % version)

    if arch == 'win64' and not exists(os.path.join(vcdir, 'bin', 'amd64', 'cl.exe')) \
                       and not exists(os.path.join(vcdir, 'bin', 'x86_amd64', 'cl.exe')):
        error("%s: unable to find the amd64 compiler" % version)

    perl = get_output('perl', '-V')
    if not perl or 'perl5' not in perl:
        error("perl does not seem to be installed.")

    nsis = get_registry_value(r'SOFTWARE\NSIS')
    if not nsis or not exists(os.path.join(nsis, 'makensis.exe')):
        error("NSIS does not seem to be installed.")

def build_msvc(config, basedir):
    msvc, arch = rchop(config, '-dbg').split('-')
    vcdir = os.path.join(os.environ[MSVC_LOCATION[msvc]], '..', '..', 'VC')
    vcarg = 'x86'
    if arch == 'win64':
        if exists(os.path.join(vcdir, 'bin', 'amd64', 'cl.exe')):
            vcarg = 'amd64'
        else:
            vcarg = 'x86_amd64'

    python = sys.executable
    process = subprocess.Popen('("%s" %s>nul)&&"%s" -c "import os, sys; sys.stdout.write(repr(dict(os.environ)))"' % (
        os.path.join(vcdir, 'vcvarsall.bat'), vcarg, python), stdout=subprocess.PIPE, shell=True)
    stdout, _ = process.communicate()
    exitcode = process.wait()
    if exitcode != 0:
        error("%s: unable to initialize the environment" % msvc)

    os.environ.update(eval(stdout.strip()))

    version, simple_version = get_version(basedir)
    cflags  = config.endswith('-dbg') and '/MDd /Zi' or '/MD'
    build_deplibs(config, basedir, cflags=cflags)

    sha1, url = MSVC_RUNTIME[rchop(config, '-dbg')]
    shutil.copy(download_file(url, sha1, basedir), os.path.join(basedir, config, 'vcredist.exe'))

    libdir = os.path.join(basedir, config, 'deplibs')
    qtdir  = os.path.join(basedir, config, 'qt')
    mkdir_p(qtdir)

    configure_args = qt_config('msvc',
        '-I %s\\include' % libdir,
        '-L %s\\lib' % libdir,
        'OPENSSL_LIBS="-L%s\\\\lib -lssleay32 -llibeay32 -lUser32 -lAdvapi32 -lGdi32 -lCrypt32"' % libdir.replace('\\', '\\\\'))

    os.chdir(qtdir)
    if not exists('is_configured'):
        shell('%s\\..\\qt\\configure.exe %s' % (basedir, configure_args))
        open('is_configured', 'w').write('')
    shell('nmake')

    appdir = os.path.join(basedir, config, 'app')
    mkdir_p(appdir)
    os.chdir(appdir)
    rmdir('bin')
    mkdir_p('bin')

    os.environ['WKHTMLTOX_VERSION'] = version

    shell('%s\\bin\\qmake %s\\..\\wkhtmltopdf.pro' % (qtdir, basedir))
    shell('nmake')

    makensis = os.path.join(get_registry_value(r'SOFTWARE\NSIS'), 'makensis.exe')
    os.chdir(os.path.join(basedir, '..'))
    shell('"%s" /DVERSION=%s /DSIMPLE_VERSION=%s /DTARGET=%s /DMSVC /DARCH=%s wkhtmltox.nsi' % \
            (makensis, version, simple_version, config, arch))

# ------------------------------------------------ MinGW-W64 Cross Environment

MINGW_W64_PREFIX = {
    'mingw-w64-cross-win32' : 'i686-w64-mingw32',
    'mingw-w64-cross-win64' : 'x86_64-w64-mingw32',
}

def check_mingw64_cross(config):
    chroot_shell('mingw-w64', '%s-gcc --version' % MINGW_W64_PREFIX[rchop(config, '-dbg')])

def build_mingw64_cross(config, basedir):
    os.chdir(os.path.realpath(os.path.join(basedir, '..')))
    chroot_shell('mingw-w64', 'python scripts/build.py %s -chroot-build' % ' '.join(sys.argv[1:]))

def chroot_build_mingw64_cross(config, basedir):
    version, simple_version = get_version(basedir)
    build_deplibs(config, basedir, mingw_w64=MINGW_W64_PREFIX.get(rchop(config, '-dbg')))

    libdir = os.path.join(basedir, config, 'deplibs')
    qtdir  = os.path.join(basedir, config, 'qt')

    configure_args = qt_config('mingw-w64-cross',
        '--prefix=%s'   % qtdir,
        '-I%s/include'  % libdir,
        '-L%s/lib'      % libdir,
        '-device-option CROSS_COMPILE=%s-' % MINGW_W64_PREFIX[rchop(config, '-dbg')])

    os.environ['OPENSSL_LIBS'] = '-lssl -lcrypto -L%s/lib -lws2_32 -lgdi32 -lcrypt32' % libdir

    mkdir_p(qtdir)
    os.chdir(qtdir)

    if not exists('is_configured'):
        for var in ['CFLAGS', 'CXXFLAGS']:
            os.environ[var] = '-w'
        shell('%s/../qt/configure %s' % (basedir, configure_args))
        shell('touch is_configured')
    shell('make -j%d' % CPU_COUNT)

    appdir = os.path.join(basedir, config, 'app')
    mkdir_p(appdir)
    os.chdir(appdir)
    shell('rm -f bin/*')

    # set up cross compiling prefix correctly
    os.environ['WKHTMLTOX_VERSION'] = version
    shell('%s/bin/qmake -set CROSS_COMPILE %s-' % (qtdir, MINGW_W64_PREFIX[rchop(config, '-dbg')]))
    shell('%s/bin/qmake -spec win32-g++-4.6 %s/../wkhtmltopdf.pro' % (qtdir, basedir))
    shell('make')
    shutil.copy('bin/libwkhtmltox0.a', 'bin/wkhtmltox.lib')
    shell('rm -f bin/lib*.dll')
    for dll in ['libgcc_s_sjlj-1.dll', 'libstdc++-6.dll', 'libwinpthread-1.dll']:
        dll_path = get_output('dpkg', '-S', dll)
        if dll_path:
            for line in dll_path.split('\n'):
                loc = line[1+line.index(':'):].strip()
                if exists(loc) and MINGW_W64_PREFIX[rchop(config, '-dbg')] in loc:
                    shell('cp %s bin/' % loc)

    os.chdir(os.path.join(basedir, '..'))
    shell('makensis -DVERSION=%s -DSIMPLE_VERSION=%s -DTARGET=%s -DMINGW -DARCH=%s wkhtmltox.nsi' % \
            (version, simple_version, config, rchop(config, '-dbg').split('-')[-1]))

# -------------------------------------------------- Linux schroot environment

def check_linux_schroot(config):
    shell('schroot -c wkhtmltopdf-%s -- gcc --version' % rchop(config, '-dbg'))

def build_linux_schroot(config, basedir):
    version, simple_version = get_version(basedir)

    dir    = os.path.join(basedir, config)
    script = os.path.join(dir, 'build.sh')
    dist   = os.path.join(dir, 'dist')

    mkdir_p(dir)
    rmdir(dist)

    configure_args = qt_config('posix', '--prefix=%s' % os.path.join(dir, 'qt'))

    lines = ['#!/bin/bash']
    lines.append('# start of autogenerated build script')
    lines.append('mkdir -p app qt')
    lines.append('cd qt')
    lines.append('export CFLAGS="-w"')
    lines.append('export CXXLAGS="-w"')
    lines.append('if [ ! -f is_configured ]; then')
    lines.append('  ../../../qt/configure %s || exit 1' % configure_args)
    lines.append('  touch is_configured')
    lines.append('fi')
    lines.append('if ! make -j%d -q; then\n  make -j%d || exit 1\nfi' % (CPU_COUNT, CPU_COUNT))
    lines.append('cd ../app')
    lines.append('rm -f bin/*')
    lines.append('export WKHTMLTOX_VERSION=%s' % version)
    lines.append('../qt/bin/qmake ../../../wkhtmltopdf.pro')
    lines.append('make install INSTALL_ROOT=%s || exit 1' % dist)
    lines.append('cd ..')
    lines.append('/fpm_package.sh %s %s' % (version, config[1+config.index('-'):]))
    lines.append('# end of build script')

    open(script, 'w').write('\n'.join(lines))
    os.chdir(dir)
    shell('chmod +x build.sh')
    shell('schroot -c wkhtmltopdf-%s -- ./build.sh' % rchop(config, '-dbg'))


# -------------------------------------------------- POSIX local environment

def check_posix_local(config):
    pass

def build_posix_local(config, basedir):
    version, simple_version = get_version(basedir)

    app    = os.path.join(basedir, config, 'app')
    qt     = os.path.join(basedir, config, 'qt')
    dist   = os.path.join(basedir, config, 'wkhtmltox-%s' % version)
    make   = get_output('which gmake') and 'gmake' or 'make'

    mkdir_p(qt)
    mkdir_p(app)

    rmdir(dist)
    mkdir_p(os.path.join(dist, 'bin'))
    mkdir_p(os.path.join(dist, 'include', 'wkhtmltox'))
    mkdir_p(os.path.join(dist, 'lib'))

    os.chdir(qt)
    if not exists('is_configured'):
        shell('../../../qt/configure %s' % qt_config('posix', '--prefix=%s' % qt))
        shell('touch is_configured')

    if subprocess.call([make, '-j%d' % CPU_COUNT]):
        shell('%s -j%d' % (make, CPU_COUNT))

    os.chdir(app)
    shell('rm -f bin/*')
    os.environ['WKHTMLTOX_VERSION'] = version
    shell('../qt/bin/qmake ../../../wkhtmltopdf.pro')
    shell('%s -j%d' % (make, CPU_COUNT))
    shell('cp bin/wkhtmlto* ../wkhtmltox-%s/bin' % version)
    shell('cp -P bin/libwkhtmltox*.so.* ../wkhtmltox-%s/lib' % version)
    shell('cp ../../../include/wkhtmltox/*.h ../wkhtmltox-%s/include/wkhtmltox' % version)
    shell('cp ../../../include/wkhtmltox/dll*.inc ../wkhtmltox-%s/include/wkhtmltox' % version)

    os.chdir(os.path.join(basedir, config))
    shell('tar -c -v -f ../wkhtmltox-%s_local-%s.tar wkhtmltox-%s/' % (version, platform.node(), version))
    shell('xz --compress --force --verbose -9 ../wkhtmltox-%s_local-%s.tar' % (version, platform.node()))

# --------------------------------------------------------------- OS X

def check_osx(config):
    if not platform.system() == 'Darwin' or not platform.mac_ver()[0]:
        error('This can only be run on a OS X system!')

    if not get_output('xcode-select', '--print-path'):
        error('Xcode is not installed, aborting.')

    if not get_output('which', 'fpm'):
        error('Please install fpm by running "sudo gem install fpm --no-ri --no-rdoc"')

def build_osx(config, basedir):
    version, simple_version = get_version(basedir)
    build_deplibs(config, basedir)

    osxver    = platform.mac_ver()[0][:platform.mac_ver()[0].rindex('.')]
    framework = config.split('-')[1]
    if osxver == '10.6':
        osxcfg = '-%s -platform macx-g++42' % framework
    else:
        osxcfg = '-%s -platform unsupported/macx-clang' % framework

    flags = ''
    if framework == 'carbon' and osxver != '10.6':
        for item in ['CFLAGS', 'CXXFLAGS']:
            flags += '"QMAKE_%s += %s" ' % (item, '-fvisibility=hidden -fvisibility-inlines-hidden')

    def get_dir(name):
        return os.path.join(basedir, config, name)

    configure_args = qt_config('osx', osxcfg,
        '--prefix=%s'   % get_dir('qt'),
        '-I %s/include' % get_dir('deplibs'),
        '-L %s/lib'     % get_dir('deplibs'))

    rmdir(get_dir('dist'))
    rmdir(get_dir('pkg'))
    mkdir_p(get_dir('qt'))
    mkdir_p(get_dir('app'))
    mkdir_p(get_dir('pkg'))

    os.chdir(get_dir('qt'))
    if not exists('is_configured'):
        shell('../../../qt/configure %s' % configure_args)
        shell('touch is_configured')

    shell('make -j%d' % CPU_COUNT)

    os.chdir(get_dir('app'))
    shell('rm -f bin/*')
    os.environ['WKHTMLTOX_VERSION'] = version
    shell('../qt/bin/qmake %s ../../../wkhtmltopdf.pro' % flags)
    shell('make -j%d' % CPU_COUNT)

    if osxver not in ['10.6', '10.7']:
        for item in ['wkhtmltoimage', 'wkhtmltopdf', 'libwkhtmltox.%s.dylib' % simple_version]:
            shell(' '.join([
                'install_name_tool', '-change',
                '/System/Library/Frameworks/CoreText.framework/Versions/A/CoreText',
                '/System/Library/Frameworks/ApplicationServices.framework/Versions/A/Frameworks/CoreText.framework/CoreText',
                'bin/'+item]))

    shell('make install INSTALL_ROOT=%s' % get_dir('dist'))

    def _osx_tar(info):
        info.uid   = info.gid   = 0
        info.uname = 'root'
        info.gname = 'wheel'
        return info

    # create tarball for application and copy xz
    os.chdir(get_dir('dist'))
    with tarfile.open('../pkg/app.tar', 'w') as tar:
        tar.add('.', './', filter=_osx_tar)
    xz = os.path.join(get_dir('deplibs'), 'bin', 'xz')
    shell('%s --compress --force --verbose -9 ../pkg/app.tar' % xz)
    shutil.copy(xz, '../pkg/')

    args, cfg = fpm_setup('osx')
    with open('../pkg/uninstall-wkhtmltox', 'w') as f:
        os.chmod('../pkg/uninstall-wkhtmltox', 0o755)
        f.write("""#!/bin/bash
if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as sudo uninstall-wkhtmltox" 1>&2
   exit 1
fi
cd /usr/local
if which pkgutil >/dev/null; then
    pkgutil --forget %s.%s
fi
""" % (cfg['--osxpkg-identifier-prefix'], cfg['--name']))
        for root, dirs, files in os.walk(get_dir('dist')):
            for file in files:
                f.write('echo REMOVE /usr/local/%(name)s && rm -f %(name)s\n' % { 'name': os.path.relpath(os.path.join(root, file)) })
        f.write('echo REMOVE /usr/local/include/wkhtmltox && rm -df /usr/local/include/wkhtmltox\n')
        f.write('echo REMOVE /usr/local/bin/uninstall-wkhtmltox && rm -f /usr/local/bin/uninstall-wkhtmltox')

    open('../extract.sh', 'w').write("""#!/bin/bash
TGTDIR=/usr/local
BASEDIR=%s
cd $BASEDIR
./xz --decompress app.tar.xz
cd $TGTDIR
tar xf $BASEDIR/app.tar
mv $BASEDIR/uninstall-wkhtmltox $TGTDIR/bin
rm -fr $BASEDIR
""" % cfg['--prefix'])

    os.chdir(os.path.join(basedir, config))
    shell('fpm --force %s --package ../%s-%s_%s.pkg .' % (args.replace('$1', version), cfg['--name'], version, config))

# --------------------------------------------------------------- command line

def usage(exit_code=2):
    message("Usage: scripts/build.py <target> [-clean] [-debug]\n\nThe supported targets are:\n")
    opts = list(BUILDERS.keys())
    opts.sort()
    for opt in opts:
        message('* %s\n' % opt)
    sys.exit(exit_code)

def main():
    rootdir = os.path.realpath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..'))
    basedir = os.path.join(rootdir, 'static-build')
    if len(sys.argv) == 1:
        usage(0)

    config = sys.argv[1]
    if config not in BUILDERS:
        usage()

    for arg in sys.argv[2:]:
        if not arg in ['-clean', '-debug', '-chroot-build']:
            usage()

    final_config = config
    if '-debug' in sys.argv[2:]:
        final_config += '-dbg'
        QT_CONFIG['common'].extend(['remove:-release', 'remove:-webkit', '-debug', '-webkit-debug'])

    if '-clean' in sys.argv[2:]:
        rmdir(os.path.join(basedir, config))

    os.chdir(rootdir)
    if '-chroot-build' in sys.argv[2:]:
        globals()['chroot_build_%s' % BUILDERS[config]](final_config, basedir)
        return

    globals()['check_%s' % BUILDERS[config]](final_config)
    globals()['build_%s' % BUILDERS[config]](final_config, basedir)

if __name__ == '__main__':
    main()
