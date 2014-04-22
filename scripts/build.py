#!/usr/bin/env python2.7
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

OPENSSL = {
    'repository': 'https://github.com/openssl/openssl.git',
    'branch'    : 'OpenSSL_1_0_1-stable',
    'tag'       : 'OpenSSL_1_0_1g',
    'build'     : {
        'msvc2010-win32': {
            'configure' : 'VC-WIN32 no-asm',
            'build'     : ['ms\\do_ms.bat', 'nmake /f ms\\nt.mak install'],
            'libs'      : ['ssleay32.lib', 'libeay32.lib'],
            'os_libs'   : '-lUser32 -lAdvapi32 -lGdi32 -lCrypt32'
        },
        'msvc2010-win64': {
            'configure' : 'VC-WIN64A',
            'build'     : ['ms\\do_win64a.bat', 'nmake /f ms\\nt.mak install'],
            'libs'      : ['ssleay32.lib', 'libeay32.lib'],
            'os_libs'   : '-lUser32 -lAdvapi32 -lGdi32 -lCrypt32'
        },
        'mingw-w64-cross-win32': {
            'configure' : '--cross-compile-prefix=i686-w64-mingw32- no-shared no-asm mingw64',
            'build'     : ['make', 'make install'],
            'libs'      : ['libssl.a', 'libcrypto.a'],
            'os_libs'   : '-lws2_32 -lgdi32 -lcrypt32'
        },
        'mingw-w64-cross-win64': {
            'configure' : '--cross-compile-prefix=x86_64-w64-mingw32- no-shared no-asm mingw64',
            'build'     : ['make', 'make install'],
            'libs'      : ['libssl.a', 'libcrypto.a'],
            'os_libs'   : '-lws2_32 -lgdi32 -lcrypt32'
        }
    }
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
        '-qt-zlib',                 # use bundled versions of libraries
        '-qt-libpng',
        '-qt-libjpeg',
        '-no-libmng',
        '-no-libtiff',
        '-no-accessibility',
        '-no-stl',
        '-no-qt3support',
        '-no-phonon',
        '-no-phonon-backend',
        '-no-opengl',
        '-no-declarative',
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

    'msvc2010': [
        '-mp',
        '-no-script',
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
        '-script',                  # "make install" does not copy QtScript/qscriptengine.h
        '-xrender',                 # xrender support is required
        '-largefile',
        '-rpath',
        '-openssl',                 # load OpenSSL binaries at runtime
        '-no-dbus',
        '-no-nis',
        '-no-cups',
        '-no-iconv',
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
        '-D ENABLE_VIDEO=0',        # required as otherwise gstreamer gets linked in
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
        '-script',                  # "make install" does not copy QtScript/qscriptengine.h
        '-openssl-linked',          # static linkage for OpenSSL
        '-no-reduce-exports',
        '-no-rpath',
        '-xplatform win32-g++-4.6'
    ]
}

BUILDERS = {
    'centos':                'centos',
    'msvc2010-win32':        'msvc2010',
    'msvc2010-win64':        'msvc2010',
    'centos5-i386':          'linux_schroot',
    'centos5-amd64':         'linux_schroot',
    'wheezy-i386':           'linux_schroot',
    'wheezy-amd64':          'linux_schroot',
    'mingw-w64-cross-win32': 'mingw64_cross',
    'mingw-w64-cross-win64': 'mingw64_cross'
}

# --------------------------------------------------------------- HELPERS

import os, sys, subprocess, shutil, multiprocessing

from os.path import exists

CPU_COUNT = max(2, multiprocessing.cpu_count()-1)   # leave one CPU free

def error(msg):
    print msg
    sys.exit(1)

def shell(cmd):
    ret = os.system(cmd)
    if ret != 0:
        error("command failed: exit code %d" % ret)

def rmdir(path):
    if exists(path):
        shutil.rmtree(path)

def mkdir_p(path):
    if not exists(path):
        os.makedirs(path)

def get_version(basedir):
    text = open(os.path.join(basedir, '..', 'VERSION'), 'r').read()
    if '-' not in text:
        return (text, text)
    text = text[:text.index('-')]
    os.chdir(os.path.join(basedir, '..'))
    hash = subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD']).strip()
    return ('%s-%s' % (text, hash), text)

def build_openssl(config, basedir):
    if not config in OPENSSL['build']:
        return

    srcdir = os.path.join(basedir, 'openssl')
    dstdir = os.path.join(basedir, config, 'openssl')

    def is_compiled():
        compiled = exists(os.path.join(dstdir, 'include', 'openssl', 'ssl.h'))
        for lib in OPENSSL['build'][config]['libs']:
            compiled = compiled and exists(os.path.join(dstdir, 'lib', lib))
        return compiled

    if not exists(os.path.join(srcdir, '.git')):
        rmdir(srcdir)
        rmdir(dstdir)
        os.chdir(basedir)
        shell('git clone --branch %s --single-branch %s openssl' % (OPENSSL['branch'], OPENSSL['repository']))

    os.chdir(srcdir)
    shell('git clean -fdx')
    shell('git reset --hard HEAD')
    shell('git checkout %s' % (OPENSSL['tag']))

    if not is_compiled():
        opts = OPENSSL['build'][config]
        shell('perl Configure --openssldir=%s %s' % (dstdir, opts['configure']))
        for cmd in opts['build']:
            shell(cmd)
        shell('git clean -fdx')
        if not is_compiled():
            error("Unable to compile OpenSSL for your system, aborting.")

# --------------------------------------------------------------- MSVC 2010

def check_msvc2010(config):
    for key in ['Configuration', 'TARGET_PLATFORM', 'TARGET_CPU']:
        if not key in os.environ:
            error("Please run under appropriate 'Windows SDK 7.1 Command Prompt'.")

    if os.environ['TARGET_PLATFORM'] not in ['XP', 'LH', 'SRV', 'LHS']:
        error("Please configure for 'Windows Server 2008 Release' or earlier.")

    if os.environ['Configuration'] != 'Release':
        error("Please configure for release mode.")

    if os.environ['TARGET_CPU'] not in ['x86', 'x64']:
        error("Please configure CPU for either x86 or x64.")

    if os.environ['TARGET_CPU'] == 'x86' and config == 'msvc2010-win64':
        error("Error: SDK configured for x86 but trying to build 64-bit.")

    if os.environ['TARGET_CPU'] == 'x64' and config == 'msvc2010-win32':
        error("Error: SDK configured for x64 but trying to build 32-bit.")

def build_msvc2010(config, basedir):
    build_openssl(config, basedir)

    ssldir = os.path.join(basedir, config, 'openssl')
    qtdir  = os.path.join(basedir, config, 'qt')
    mkdir_p(qtdir)

    args = []
    args.extend(QT_CONFIG['common'])
    args.extend(QT_CONFIG['msvc2010'])
    args.append('-I %s\\include' % ssldir)
    args.append('-L %s\\lib' % ssldir)
    args.append('OPENSSL_LIBS="-L%s -lssleay32 -llibeay32 %s"' % \
        (ssldir.replace('\\', '\\\\'), OPENSSL['build'][config]['os_libs']))

    os.chdir(qtdir)
    if not exists('is_configured'):
        shell('%s\\..\\qt\\configure.exe %s' % (basedir, ' '.join(args)))
        open('is_configured', 'w').write('')
    shell('nmake')

    appdir = os.path.join(basedir, config, 'app')
    mkdir_p(appdir)
    os.chdir(appdir)
    rmdir('bin')
    mkdir_p('bin')

    shell('%s\\bin\\qmake %s\\..\\wkhtmltopdf.pro' % (qtdir, basedir))
    shell('nmake')

    found = False
    for pfile in ['ProgramFiles(x86)', 'ProgramFiles']:
        if not pfile in os.environ or not exists(os.path.join(os.environ[pfile], 'NSIS', 'makensis.exe')):
            continue
        found = True

        version, simple_version = get_version(basedir)
        makensis = os.path.join(os.environ[pfile], 'NSIS', 'makensis.exe')
        os.chdir(os.path.join(basedir, '..'))
        shell('"%s" /DVERSION=%s /DSIMPLE_VERSION=%s /DTARGET=%s wkhtmltox.nsi' % \
                (makensis, version, simple_version, config))

    if not found:
        print "\n\nCould not build installer as NSIS was not found."

# ------------------------------------------------ MinGW-W64 Cross Environment

MINGW_W64_PREFIX = {
    'mingw-w64-cross-win32' : 'i686-w64-mingw32',
    'mingw-w64-cross-win64' : 'x86_64-w64-mingw32',
}

def check_mingw64_cross(config):
    shell('%s-gcc --version' % MINGW_W64_PREFIX[config])

def build_mingw64_cross(config, basedir):
    build_openssl(config, basedir)

    ssldir = os.path.join(basedir, config, 'openssl')
    build  = os.path.join(basedir, config, 'qt_build')
    qtdir  = os.path.join(basedir, config, 'qt')

    mkdir_p(build)

    args = []
    args.extend(QT_CONFIG['common'])
    args.extend(QT_CONFIG['mingw-w64-cross'])
    args.append('--prefix=%s'   % qtdir)
    args.append('-I %s/include' % ssldir)
    args.append('-L %s/lib'     % ssldir)
    args.append('-device-option CROSS_COMPILE=%s-' % MINGW_W64_PREFIX[config])

    os.environ['OPENSSL_LIBS'] = '-lssl -lcrypto -L %s/lib %s' % (ssldir, OPENSSL['build'][config]['os_libs'])

    os.chdir(build)
    if not exists('is_configured'):
        shell('%s/../qt/configure %s' % (basedir, ' '.join(args)))
        shell('touch is_configured')
    shell('make -j%d' % CPU_COUNT)
    shell('make install')

    appdir = os.path.join(basedir, config, 'app')
    mkdir_p(appdir)
    os.chdir(appdir)
    shell('rm -f bin/*')

    # set up cross compiling prefix correctly (isn't set by make install)
    os.environ['QTDIR'] = qtdir
    shell('%s/bin/qmake -set CROSS_COMPILE %s-' % (qtdir, MINGW_W64_PREFIX[config]))
    shell('%s/bin/qmake -spec win32-g++-4.6 %s/../wkhtmltopdf.pro' % (qtdir, basedir))
    shell('make')
    shutil.copy('bin/libwkhtmltox0.a', 'bin/wkhtmltox.lib')

    version, simple_version = get_version(basedir)
    os.chdir(os.path.join(basedir, '..'))
    shell('makensis -DVERSION=%s -DSIMPLE_VERSION=%s -DTARGET=%s wkhtmltox.nsi' % \
            (version, simple_version, config))

# -------------------------------------------------- Linux schroot environment

def check_linux_schroot(config):
    shell('schroot -c wkhtmltopdf-%s -- gcc --version' % config)

def build_linux_schroot(config, basedir):
    version, simple_version = get_version(basedir)

    dir    = os.path.join(basedir, config)
    script = os.path.join(dir, 'build.sh')
    dist   = os.path.join(dir, 'wkhtmltox-%s' % version)

    mkdir_p(os.path.join(dir, 'qt_build'))
    mkdir_p(os.path.join(dir, 'app'))

    rmdir(dist)
    mkdir_p(os.path.join(dist, 'bin'))
    mkdir_p(os.path.join(dist, 'include', 'wkhtmltox'))
    mkdir_p(os.path.join(dist, 'lib'))

    args = []
    args.extend(QT_CONFIG['common'])
    args.extend(QT_CONFIG['posix'])
    args.append('--prefix=../qt')

    lines = ['#!/bin/bash']
    lines.append('# start of autogenerated build script')
    lines.append('cd qt_build')
    if config == 'centos5-i386':
        lines.append('export CFLAGS=-march=i486')
        lines.append('export CXXFLAGS=-march=i486')

    lines.append('if [ ! -f is_configured ]; then')
    lines.append('  ../../../qt/configure %s || exit 1' % ' '.join(args))
    lines.append('  touch is_configured')
    lines.append('fi')
    lines.append('if ! make -j%d -q; then\n  make -j%d || exit 1\nfi' % (CPU_COUNT, CPU_COUNT))
    lines.append('make install || exit 1')
    lines.append('cd ../app')
    lines.append('rm -f bin/*')
    lines.append('GIT_DIR=../../../.git ../qt/bin/qmake ../../../wkhtmltopdf.pro')
    lines.append('make -j%d || exit 1' % CPU_COUNT)
    lines.append('strip bin/wkhtmltopdf bin/wkhtmltoimage')
    lines.append('cp bin/wkhtmlto* ../wkhtmltox-%s/bin' % version)
    lines.append('cp -P bin/libwkhtmltox*.so.* ../wkhtmltox-%s/lib' % version)
    lines.append('cp ../../../include/wkhtmltox/*.h ../wkhtmltox-%s/include/wkhtmltox' % version)
    lines.append('cp ../../../include/wkhtmltox/dll*.inc ../wkhtmltox-%s/include/wkhtmltox' % version)
    lines.append('cd ..')
    lines.append('tar -c -v --use-compress-program=xz -f ../wkhtmltox-%s_linux-%s.tar.xz wkhtmltox-%s/' % (version, config, version))
    lines.append('# end of build script')

    open(script, 'w').write('\n'.join(lines))
    os.chdir(dir)
    shell('chmod +x build.sh')
    shell('schroot -c wkhtmltopdf-%s -- ./build.sh' % config)

def check_centos(config):
    pass

def build_centos(config, basedir):
    version, simple_version = get_version(basedir)

    dir    = os.path.join(basedir, config)
    script = os.path.join(dir, 'build.sh')
    dist   = os.path.join(dir, 'wkhtmltox-%s' % version)

    mkdir_p(os.path.join(dir, 'qt_build'))
    mkdir_p(os.path.join(dir, 'app'))

    rmdir(dist)
    mkdir_p(os.path.join(dist, 'bin'))
    mkdir_p(os.path.join(dist, 'include', 'wkhtmltox'))
    mkdir_p(os.path.join(dist, 'lib'))

    args = []
    args.extend(QT_CONFIG['common'])
    args.extend(QT_CONFIG['posix'])
    args.append('--prefix=../qt')

    lines = ['#!/bin/bash']
    lines.append('# start of autogenerated build script')
    lines.append('cd qt_build')
    #if config == 'centos5-i386':
    #    lines.append('export CFLAGS=-march=i486')
    #    lines.append('export CXXFLAGS=-march=i486')

    lines.append('if [ ! -f is_configured ]; then')
    lines.append('  ../../../qt/configure %s || exit 1' % ' '.join(args))
    lines.append('  touch is_configured')
    lines.append('fi')
    lines.append('if ! make -j%d -q; then\n  make -j%d || exit 1\nfi' % (CPU_COUNT, CPU_COUNT))
    lines.append('make install || exit 1')
    lines.append('cd ../app')
    lines.append('rm -f bin/*')
    lines.append('GIT_DIR=../../../.git ../qt/bin/qmake ../../../wkhtmltopdf.pro')
    lines.append('make -j%d || exit 1' % CPU_COUNT)
    #lines.append('strip bin/wkhtmltopdf bin/wkhtmltoimage')
    lines.append('cp bin/wkhtmlto* ../wkhtmltox-%s/bin' % version)
    lines.append('cp -P bin/libwkhtmltox*.so.* ../wkhtmltox-%s/lib' % version)
    lines.append('cp ../../../include/wkhtmltox/*.h ../wkhtmltox-%s/include/wkhtmltox' % version)
    lines.append('cp ../../../include/wkhtmltox/dll*.inc ../wkhtmltox-%s/include/wkhtmltox' % version)
    lines.append('cd ..')
    #lines.append('tar -c -v --use-compress-program=xz -f ../wkhtmltox-%s_linux-%s.tar.xz wkhtmltox-%s/' % (version, config, version))
    lines.append('# end of build script')

    open(script, 'w').write('\n'.join(lines))
    os.chdir(dir)
    shell('chmod +x build.sh')
    #shell('schroot -c wkhtmltopdf-%s -- ./build.sh' % config)


# --------------------------------------------------------------- command line

def usage():
    print "Usage: scripts/build.py [target] where target is one of:\n *",
    opts = list(BUILDERS.keys())
    opts.sort()
    print '\n * '.join(opts)
    sys.exit(0)

def main():
    basedir = os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', 'static-build')
    mkdir_p(basedir)

    if len(sys.argv) != 2 or sys.argv[1] not in BUILDERS:
        usage()

    config = sys.argv[1]
    globals()['check_%s' % BUILDERS[config]](config)
    globals()['build_%s' % BUILDERS[config]](config, os.path.realpath(basedir))

if __name__ == '__main__':
    main()
