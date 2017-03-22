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

PROJECT_SETUP = {
    'name':         'wkhtmltox',
    'description':  'convert HTML to PDF and various image formats using QtWebkit',
    'license':      'LGPLv3',
    'maintainer':   'Ashish Kulkarni <kulkarni.ashish@gmail.com>',
    'url':          'http://wkhtmltopdf.org/'
}

BUILDERS = {
    'source-tarball':        'source_tarball',
    'msvc2013-win32':        'msvc',
    'msvc2013-win64':        'msvc',
    'msvc2015-win32':        'msvc',
    'msvc2015-win64':        'msvc',
    'setup-mingw-w64':       'setup_mingw_w64',
    'setup-schroot-generic': 'setup_schroot',
    'update-all-schroots':   'update_schroot',
    'linux-generic-i386':    'linux_generic',
    'linux-generic-amd64':   'linux_generic',
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
        '-no-javascript-jit',       # can cause crashes/excess memory usage
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

LINUX_SCHROOT_SETUP = {
    'generic': {
        'title'             : 'Generic (based on CentOS 6)',
        'packaging_tool'    : 'yum',
        'build_arch'        : ['amd64', 'i386'],
        'compression'       : 'bzip2',
        'wrapper_command'   : 'scl enable devtoolset-3 python27 git19 -- ',
        'runtime_packages'  : 'glibc libstdc++ zlib openssl freetype fontconfig '\
                              'libX11 libXext libXrender xorg-x11-fonts-Type1 xorg-x11-fonts-75dpi',
        'build_packages'    : 'scl-utils devtoolset-3-gcc-c++ python27 git19 ruby perl git make gzip diffutils gperf bison flex '\
                              'zlib-devel openssl-devel freetype-devel fontconfig-devel '\
                              'libX11-devel libXrender-devel libXext-devel',
        'rinse'             : ('centos-6', """
[slc6-scl]
name=Scientific Linux CERN (SLC6) - SCL addons
baseurl=http://linuxsoft.cern.ch/cern/scl/slc6X/$basearch/yum/scl/
gpgcheck=0
enabled=1
""")}
}

DEPENDENT_LIBS = {
    'openssl': {
        'order' : 1,
        'url'   : 'https://openssl.org/source/openssl-1.0.2k.tar.gz',
        'sha1'  : '5f26a624479c51847ebd2f22bb9f84b3b44dcb44',
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
        'url'   : 'http://downloads.sourceforge.net/libpng/zlib-1.2.11.tar.gz',
        'sha1'  : 'e6d119755acdf9104d7ba236b1242696940ed6dd',
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
        'url' : 'http://downloads.sourceforge.net/libpng/libpng-1.6.28.tar.gz',
        'sha1': '004556d65f21baed83755f8e094112711e39ebae',
        'build' : {
            'msvc*': {
                'result': {
                    'include/png.h'       : 'png.h',
                    'include/pngconf.h'   : 'pngconf.h',
                    'include/pnglibconf.h': 'pnglibconf.h',
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
                    'include/pnglibconf.h': 'pnglibconf.h',
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
            },
            'linux-generic-*': {
                'result': ['include/png.h', 'include/pngconf.h', 'lib/libpng.a'],
                'commands': [
                    'CFLAGS="-fPIC" ./configure --disable-shared --enable-static --prefix=%(destdir)s',
                    'make install']
            }
        }
    },

    'libjpeg': {
        'order' : 4,
        'url' : 'http://ijg.org/files/jpegsrc.v9b.tar.gz',
        'sha1': '15dd867617a88abd07573e06a86ace9bdb998ac2',
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
            },
            'linux-generic-*': {
                'result': ['include/jpeglib.h', 'include/jmorecfg.h', 'include/jerror.h', 'include/jconfig.h', 'lib/libjpeg.a'],
                'commands': [
                    'CFLAGS="-fPIC" ./configure --disable-shared --enable-static --prefix=%(destdir)s',
                    'make install']
            }
        }
    },

    'xz': {
        'order' : 5,
        'url' : 'http://tukaani.org/xz/xz-5.2.3.tar.gz',
        'sha1': '529638eec3597e429cc54c74551ac0a89169e841',
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

def silent_shell(cmd):
    message('    %s\n' % cmd)
    try:
        subprocess.check_output(cmd, stderr=subprocess.STDOUT, shell=True)
    except subprocess.CalledProcessError as e:
        error("\n%s\ncommand failed: exit code %d" % (e.output.decode('utf-8'), e.returncode))

def chroot_shell(name, cmd):
    distro  = get_chroot_list().get(name)
    wrapper = LINUX_SCHROOT_SETUP.get(distro, {}).get('wrapper_command', '')
    ret = os.system('schroot -c wkhtmltox-%s -- %s%s ' % (name, wrapper, cmd))
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
    mkdir_p(basedir)
    text = open(os.path.join(basedir, '..', 'VERSION'), 'r').read().strip()
    if '-' not in text:
        return (text, text)
    version = text[:text.index('-')]
    os.chdir(os.path.join(basedir, '..'))
    hash = get_output('git', 'rev-parse', '--short', 'HEAD')
    if not hash:
        return (text, version)
    return ('%s-%s' % (text, hash), version)

def nsis_version(ver):
    while ver.count('.') < 3:
        ver += '.0'
    return ver

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
            silent_shell(command % vars)
        if not type(build_cfg['result']) is list:
            for target in build_cfg['result']:
                mkdir_p(os.path.dirname(os.path.join(dstdir, target)))
                shutil.copy(build_cfg['result'][target], os.path.join(dstdir, target))
        os.chdir(dstdir)
        if not _is_compiled(dstdir, build_cfg):
            error("Unable to compile %s for your system, aborting." % lib)

        rmdir(srcdir)

def build_qt(qtdir, make_cmd, configure_cmd):
    configured = ''
    if exists(os.path.join(qtdir, 'configured')):
        configured = open(os.path.join(qtdir, 'configured'), 'r').read()
    if not 'qt' in configured or not exists(qtdir):
        mkdir_p(qtdir)
        os.chdir(qtdir)
        shell(configure_cmd)
        open(os.path.join(qtdir, 'configured'), 'a').write('qt\n')
    os.chdir(qtdir)
    shell(make_cmd)

def check_running_on_debian():
    if not sys.platform.startswith('linux') or not exists('/etc/apt/sources.list') or platform.architecture()[0] != '64bit':
        error('This can only be run on a 64-bit Debian/Ubuntu distribution, aborting.')

    if os.geteuid() != 0:
        error('This script must be run as root.')

def install_packages(*names):
    inst = get_output('dpkg-query', '--show', '--showformat', '${Package}\n').split('\n')
    miss = [name for name in names if name not in inst]

    if miss:
        shell('apt-get update')
        shell('apt-get install --assume-yes %s' % (' '.join(miss)))

def get_chroot_list():
    prefix = 'chroot:wkhtmltox-'
    result = {}
    for line in get_output('schroot', '--list').split('\n'):
        if not line.startswith(prefix):
            continue
        name = line[len(prefix):]
        for distro in LINUX_SCHROOT_SETUP:
            if 'chroot_alias' in LINUX_SCHROOT_SETUP[distro]:
                if name == LINUX_SCHROOT_SETUP[distro]['chroot_alias']:
                    result[name] = distro
                continue
            for arch in LINUX_SCHROOT_SETUP[distro]['build_arch']:
                if name == '%s-%s' % (distro, arch):
                    result[name] = distro
    return result

# --------------------------------------------------------------- Linux chroot

def check_setup_schroot(config):
    check_running_on_debian()
    login = os.environ.get('SUDO_USER') or get_output('logname')
    if not login or login == 'root':
        error('Unable to determine the login for which schroot access is to be given.')

def build_setup_schroot(config, basedir):
    install_packages('git', 'debootstrap', 'schroot', 'rinse', 'debian-archive-keyring',
                     'build-essential', 'tar', 'xz-utils')

    login  = os.environ.get('SUDO_USER') or get_output('logname')
    target = config.split('-', 2)[2]
    distro = LINUX_SCHROOT_SETUP.get(target)
    allenv = get_chroot_list()

    for arch in distro['build_arch']:
        alias    = distro.get('chroot_alias', '%s-%s' % (target, arch))
        base_dir = os.environ.get('WKHTMLTOX_CHROOT') or '/var/chroot'
        root_dir = os.path.join(base_dir, 'wkhtmltox-%s' % alias)
        pkg_list = '%s %s' % (distro['build_packages'], distro.get('runtime_packages', ''))
        chroot   = (arch == 'i386' and 'linux32 chroot' or 'chroot')

        if alias in allenv:
            message('******************* %s (skipped)\n' % alias)
            continue

        message('******************* %s\n' % alias)
        os.system('umount %s/proc 2>/dev/null' % root_dir)
        os.system('umount %s/sys  2>/dev/null' % root_dir)
        rmdir(root_dir)
        mkdir_p(root_dir)

        def do_setup(*cmds):
            shell('mount -t proc  proc  %s/proc' % root_dir)
            shell('mount -t sysfs sysfs %s/sys'  % root_dir)
            for cmd in cmds:
                shell('%s %s %s' % (chroot, root_dir, cmd))
            shell('umount %s/proc' % root_dir)
            shell('umount %s/sys'  % root_dir)

        if distro['packaging_tool'] == 'yum':
            rinse = (arch == 'i386' and 'linux32 rinse' or 'rinse')
            rinse_distro, extra_repos = distro['rinse']
            shell('%s --arch %s --distribution %s --directory %s' % (rinse, arch, rinse_distro, root_dir))

            if arch == 'amd64':
                open(os.path.join(root_dir, 'etc/yum.conf'), 'a').write('exclude = *.i?86\n')

            if extra_repos:
                open(os.path.join(root_dir, 'etc/yum.repos.d/wkhtmltopdf.repo'), 'w').write(extra_repos)

            do_setup('yum clean all', 'yum update -y', 'yum install -y %s' % pkg_list)

        open('/etc/schroot/chroot.d/wkhtmltox', 'a').write("""
[wkhtmltox-%(alias)s]
type=directory
directory=%(dir)s
description=%(title)s for wkhtmltox
users=%(login)s
root-users=root
%(personality)s""" % { 'alias': alias, 'dir': root_dir, 'title': distro['title'], 'login': login,
                       'personality': (arch == 'i386' and 'personality=linux32\n' or '\n') })

def check_update_schroot(config):
    check_running_on_debian()
    if not get_chroot_list():
        error('Unable to determine the list of available schroots.')

def build_update_schroot(config, basedir):
    for name, distro in get_chroot_list().iteritems():
        message('******************* %s\n' % name)
        tool = LINUX_SCHROOT_SETUP[distro]['packaging_tool']
        if tool == 'apt':
            chroot_shell(name, 'apt-get update')
            chroot_shell(name, 'apt-get dist-upgrade --assume-yes')
        elif tool == 'yum':
            chroot_shell(name, 'yum update -y')

def check_setup_mingw_w64(config):
    check_running_on_debian()

def build_setup_mingw_w64(config, basedir):
    install_packages('build-essential', 'mingw-w64', 'nsis', 'perl', 'git', 'ruby', 'gperf', 'bison', 'flex')

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
    'msvc2013': ('VS120COMNTOOLS', '12.0'),
    'msvc2015': ('VS140COMNTOOLS', '14.0')
}
MSVC_RUNTIME = {
    'msvc2013-win32': ('df7f0a73bfa077e483e51bfb97f5e2eceedfb6a3', 'http://download.microsoft.com/download/2/E/6/2E61CFA4-993B-4DD4-91DA-3737CD5CD6E3/vcredist_x86.exe'),
    'msvc2013-win64': ('8bf41ba9eef02d30635a10433817dbb6886da5a2', 'http://download.microsoft.com/download/2/E/6/2E61CFA4-993B-4DD4-91DA-3737CD5CD6E3/vcredist_x64.exe'),
    'msvc2015-win32': ('bfb74e498c44d3a103ca3aa2831763fb417134d1', 'https://download.microsoft.com/download/9/3/F/93FCF1E7-E6A4-478B-96E7-D4B285925B00/vc_redist.x86.exe'),
    'msvc2015-win64': ('3155cb0f146b927fcc30647c1a904cd162548c8c', 'https://download.microsoft.com/download/9/3/F/93FCF1E7-E6A4-478B-96E7-D4B285925B00/vc_redist.x64.exe')
}

def check_msvc(config):
    version, arch = rchop(config, '-dbg').split('-')
    env_var, _ = MSVC_LOCATION[version]
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
    env_var, reg_ver = MSVC_LOCATION[msvc]
    vcdir = os.path.join(os.environ[env_var], '..', '..', 'VC')
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

    build_qt(qtdir, 'nmake', '%s\\..\\qt\\configure.exe %s' % (basedir, configure_args))

    appdir = os.path.join(basedir, config, 'app')
    mkdir_p(appdir)
    os.chdir(appdir)
    rmdir('bin')
    mkdir_p('bin')

    os.environ['WKHTMLTOX_VERSION'] = version

    shell('%s\\bin\\qmake %s\\..\\wkhtmltopdf.pro' % (qtdir, basedir))
    shell('nmake')

    if config.endswith('-dbg'):
        return

    makensis = os.path.join(get_registry_value(r'SOFTWARE\NSIS'), 'makensis.exe')
    os.chdir(os.path.join(basedir, '..'))
    shell('"%s" /DVERSION=%s /DSIMPLE_VERSION=%s /DTARGET=%s /DMSVC=%s /DARCH=%s wkhtmltox.nsi' % \
            (makensis, version, nsis_version(simple_version), config, reg_ver, arch))

# ------------------------------------------------ MinGW-W64 Cross Environment

MINGW_W64_PREFIX = {
    'mingw-w64-cross-win32' : 'i686-w64-mingw32',
    'mingw-w64-cross-win64' : 'x86_64-w64-mingw32',
}

def check_mingw64_cross(config):
    shell('%s-gcc --version' % MINGW_W64_PREFIX[rchop(config, '-dbg')])

def build_mingw64_cross(config, basedir):
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

    build_qt(qtdir, 'make -j%d' % CPU_COUNT,
        '%s/../qt/configure %s' % (basedir, configure_args))

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

    if config.endswith('-dbg'):
        return

    os.chdir(os.path.join(basedir, '..'))
    shell('makensis -DVERSION=%s -DSIMPLE_VERSION=%s -DTARGET=%s -DMINGW -DARCH=%s wkhtmltox.nsi' % \
            (version, nsis_version(simple_version), config, rchop(config, '-dbg').split('-')[-1]))

# -------------------------------------------------- Linux schroot environment

def check_linux_generic(config):
    chroot_env = ('amd64' in config) and 'generic-amd64' or 'generic-i386'
    chroot_shell(chroot_env, 'gcc --version')

def build_linux_generic(config, basedir):
    chroot_env = ('amd64' in config) and 'generic-amd64' or 'generic-i386'
    os.chdir(os.path.realpath(os.path.join(basedir, '..')))
    chroot_shell(chroot_env, 'python scripts/build.py %s -chroot-build' % ' '.join(sys.argv[1:]))

    if config.endswith('-dbg'):
        return

    version, simple_version = get_version(basedir)
    os.chdir(os.path.join(basedir, config))
    shell('XZ_OPT=-9 tar Jcf ../%s-%s_%s.tar.xz wkhtmltox/' % (PROJECT_SETUP['name'], version, config))

def chroot_build_linux_generic(config, basedir):
    version, simple_version = get_version(basedir)
    build_deplibs(config, basedir)

    libdir = os.path.join(basedir, config, 'deplibs')
    qtdir  = os.path.join(basedir, config, 'qt')
    mkdir_p(qtdir)

    configure_args = qt_config('posix',
        '--prefix=%s'   % qtdir,
        '-I%s/include'  % libdir,
        '-L%s/lib'      % libdir,
        '-DOPENSSL_NO_SSL2')
    build_qt(qtdir, 'make -j%d' % CPU_COUNT, '%s/../qt/configure %s' % (basedir, configure_args))

    app    = os.path.join(basedir, config, 'app')
    dist   = os.path.join(basedir, config, 'wkhtmltox')

    mkdir_p(app)
    rmdir(dist)
    mkdir_p(dist)

    os.chdir(app)
    shell('rm -f bin/*')

    os.environ['WKHTMLTOX_VERSION'] = version
    shell('%s/bin/qmake %s/../wkhtmltopdf.pro' % (qtdir, basedir))
    shell('make install INSTALL_ROOT=%s' % dist)

# -------------------------------------------------- POSIX local environment

def check_posix_local(config):
    pass

def build_posix_local(config, basedir):
    version, simple_version = get_version(basedir)

    app    = os.path.join(basedir, config, 'app')
    qt     = os.path.join(basedir, config, 'qt')
    dist   = os.path.join(basedir, config, 'wkhtmltox-%s' % version)
    make   = get_output('which', 'gmake') and 'gmake' or 'make'

    mkdir_p(qt)
    mkdir_p(app)

    rmdir(dist)
    mkdir_p(os.path.join(dist, 'bin'))
    mkdir_p(os.path.join(dist, 'include', 'wkhtmltox'))
    mkdir_p(os.path.join(dist, 'lib'))

    build_qt(qt, '%s -j%d' % (make, CPU_COUNT),
        '%s/../qt/configure %s' % (basedir, qt_config('posix', '--prefix=%s' % qt)))

    os.chdir(app)
    shell('rm -f bin/*')
    os.environ['WKHTMLTOX_VERSION'] = version
    shell('../qt/bin/qmake ../../../wkhtmltopdf.pro')
    shell('%s -j%d' % (make, CPU_COUNT))
    shell('cp bin/wkhtmlto* ../wkhtmltox-%s/bin' % version)
    shell('cp -P bin/libwkhtmltox*.so.* ../wkhtmltox-%s/lib' % version)
    shell('cp ../../../include/wkhtmltox/*.h ../wkhtmltox-%s/include/wkhtmltox' % version)
    shell('cp ../../../include/wkhtmltox/dll*.inc ../wkhtmltox-%s/include/wkhtmltox' % version)

    if config.endswith('-dbg'):
        return

    os.chdir(os.path.join(basedir, config))
    shell('tar -c -v -f ../wkhtmltox-%s_local-%s.tar wkhtmltox-%s/' % (version, platform.node(), version))
    shell('xz --compress --force --verbose -9 ../wkhtmltox-%s_local-%s.tar' % (version, platform.node()))

# --------------------------------------------------------------- OS X

OSXPKG_IDENTIFIER = 'org.wkhtmltopdf'
OSXPKG_PREFIX     = '/usr/local/share/wkhtmltox-installer'

def check_osx(config):
    if not platform.system() == 'Darwin' or not platform.mac_ver()[0]:
        error('This can only be run on a OS X system!')

    if not get_output('xcode-select', '--print-path'):
        error('Xcode is not installed, aborting.')

    if not config.endswith('-dbg') and not get_output('which', 'fpm'):
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

    build_qt(get_dir('qt'), 'make -j%d' % CPU_COUNT, '../../../qt/configure %s' % configure_args)

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

    if config.endswith('-dbg'):
        return

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
""" % (OSXPKG_IDENTIFIER, PROJECT_SETUP['name']))
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
tar oxf $BASEDIR/app.tar
mv $BASEDIR/uninstall-wkhtmltox $TGTDIR/bin
rm -fr $BASEDIR
""" % OSXPKG_PREFIX)

    os.chdir(os.path.join(basedir, config))
    fpm_args = '--force --prefix %s --category utils -s dir -C pkg' % OSXPKG_PREFIX
    for key in PROJECT_SETUP:
        fpm_args += ' --%s "%s"' % (key, PROJECT_SETUP[key])
    fpm_args += ' --osxpkg-identifier-prefix "%s" --version "%s"' % (OSXPKG_IDENTIFIER, version)
    fpm_args += ' -t osxpkg --package ../%s-%s_%s.pkg' % (PROJECT_SETUP['name'], version, config)
    fpm_args += ' --after-install extract.sh .'
    shell('fpm %s' % fpm_args)

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

    if not exists(os.path.join(rootdir, 'qt', 'configure')):
        error('error: source code for Qt not available, cannot proceed.')

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
        rmdir(os.path.join(basedir, final_config))

    os.chdir(rootdir)
    if '-chroot-build' in sys.argv[2:]:
        globals()['chroot_build_%s' % BUILDERS[config]](final_config, basedir)
        return

    globals()['check_%s' % BUILDERS[config]](final_config)
    globals()['build_%s' % BUILDERS[config]](final_config, basedir)

if __name__ == '__main__':
    main()
