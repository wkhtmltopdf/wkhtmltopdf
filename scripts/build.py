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

OPENSSL = {
    'repository': 'https://github.com/openssl/openssl.git',
    'branch'    : 'OpenSSL_1_0_1-stable',
    'tag'       : 'OpenSSL_1_0_1g',
    'build'     : {
        'msvc*-win32*': {
            'configure' : 'VC-WIN32 no-asm',
            'debug'     : 'debug-VC-WIN32 no-asm',
            'build'     : ['ms\\do_ms.bat', 'nmake /f ms\\nt.mak install'],
            'libs'      : ['ssleay32.lib', 'libeay32.lib'],
            'os_libs'   : '-lUser32 -lAdvapi32 -lGdi32 -lCrypt32'
        },
        'msvc*-win64*': {
            'configure' : 'VC-WIN64A',
            'debug'     : 'debug-VC-WIN64A',
            'build'     : ['ms\\do_win64a.bat', 'nmake /f ms\\nt.mak install'],
            'libs'      : ['ssleay32.lib', 'libeay32.lib'],
            'os_libs'   : '-lUser32 -lAdvapi32 -lGdi32 -lCrypt32'
        },
        'mingw-w64-cross-win32': {
            'configure' : '--cross-compile-prefix=i686-w64-mingw32- no-shared no-asm mingw64',
            'build'     : ['make', 'make install_sw'],
            'libs'      : ['libssl.a', 'libcrypto.a'],
            'os_libs'   : '-lws2_32 -lgdi32 -lcrypt32'
        },
        'mingw-w64-cross-win64': {
            'configure' : '--cross-compile-prefix=x86_64-w64-mingw32- no-shared no-asm mingw64',
            'build'     : ['make', 'make install_sw'],
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

    'msvc': [
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
    'msvc2008-win32':        'msvc',
    'msvc2008-win64':        'msvc',
    'msvc2010-win32':        'msvc',
    'msvc2010-win64':        'msvc',
    'msvc2012-win32':        'msvc',
    'msvc2012-win64':        'msvc',
    'msvc2013-win32':        'msvc',
    'msvc2013-win64':        'msvc',
    'msvc-winsdk71-win32':   'msvc_winsdk71',
    'msvc-winsdk71-win64':   'msvc_winsdk71',
    'setup-schroot-centos5': 'setup_schroot',
    'setup-schroot-wheezy':  'setup_schroot',
    'update-all-schroots':   'update_schroot',
    'centos5-i386':          'linux_schroot',
    'centos5-amd64':         'linux_schroot',
    'wheezy-i386':           'linux_schroot',
    'wheezy-amd64':          'linux_schroot',
    'mingw-w64-cross-win32': 'mingw64_cross',
    'mingw-w64-cross-win64': 'mingw64_cross'
}

HOST_PACKAGES = ['git-core', 'xz-utils', 'build-essential', 'mingw-w64', 'nsis', 'debootstrap', 'schroot', 'rinse']
CHROOT_SETUP  = {
    'wheezy': [
        ('debootstrap', 'wheezy', 'http://ftp.us.debian.org/debian/'),
        ('write_file', 'etc/apt/sources.list', """
deb     http://ftp.debian.org/debian/ wheezy         main contrib non-free
deb     http://ftp.debian.org/debian/ wheezy-updates main contrib non-free
deb     http://security.debian.org/   wheezy/updates main contrib non-free
deb-src http://ftp.debian.org/debian/ wheezy         main contrib non-free
deb-src http://ftp.debian.org/debian/ wheezy-updates main contrib non-free
deb-src http://security.debian.org/   wheezy/updates main contrib non-free"""),
        ('shell', 'apt-get update'),
        ('shell', 'apt-get dist-upgrade --assume-yes'),
        ('shell', 'apt-get install --assume-yes xz-utils'),
        ('shell', 'apt-get build-dep --assume-yes libqt4-core'),
        ('write_file', 'update.sh', 'apt-get update\napt-get dist-upgrade --assume-yes\n'),
        ('schroot_conf', 'Debian Wheezy')
    ],

    'centos5': [
        ('rinse', 'centos-5'),
        ('shell', 'yum update -y'),
        ('append_file:amd64', 'etc/yum.conf', 'exclude = *.i?86\n'),
        ('shell', 'yum install -y gcc gcc-c++ make qt4-devel openssl-devel diffutils perl xz'),
        ('write_file', 'update.sh', 'yum update -y\n'),
        ('schroot_conf', 'CentOS 5')
    ]
}

# --------------------------------------------------------------- HELPERS

import os, sys, subprocess, shutil, fnmatch, multiprocessing

from os.path import exists

CPU_COUNT = max(2, multiprocessing.cpu_count()-1)   # leave one CPU free

def rchop(s, e):
    if s.endswith(e):
        return s[:-len(e)]
    return s

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
    version = text[:text.index('-')]
    os.chdir(os.path.join(basedir, '..'))
    try:
        hash = subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD'], stderr=subprocess.STDOUT).strip()
    except subprocess.CalledProcessError:
        return (text, version)
    return ('%s-%s' % (version, hash), version)

def build_openssl(config, basedir):
    cfg = None
    for key in OPENSSL['build']:
        if fnmatch.fnmatch(config, key):
            cfg = key

    if not cfg:
        return

    srcdir = os.path.join(basedir, 'openssl')
    dstdir = os.path.join(basedir, config, 'openssl')

    def is_compiled():
        compiled = exists(os.path.join(dstdir, 'include', 'openssl', 'ssl.h'))
        for lib in OPENSSL['build'][cfg]['libs']:
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
        opts = OPENSSL['build'][cfg]
        shell('perl Configure --openssldir=%s %s' % (dstdir, opts['configure']))
        for cmd in opts['build']:
            shell(cmd)
        shell('git clean -fdx')
        if not is_compiled():
            error("Unable to compile OpenSSL for your system, aborting.")

    return OPENSSL['build'][cfg]['os_libs']

# --------------------------------------------------------------- Linux chroot

def check_setup_schroot(config):
    import platform
    if not sys.platform.startswith('linux') or \
       not platform.architecture()[0].startswith('64') or \
       platform.linux_distribution()[0] not in ['Ubuntu', 'Debian']:
        error('This can only be run on a 64-bit Debian/Ubuntu distribution, aborting.')

    if os.geteuid() != 0:
        error('This script must be run as root.')

    try:
        login = subprocess.check_output(['logname'], stderr=subprocess.STDOUT).strip()
        if login == 'root':
            error('Please run via sudo to determine login for which schroot access is to be given.')
    except subprocess.CalledProcessError:
        error('Unable to determine the login for which schroot access is to be given.')

def build_setup_schroot(config, basedir):
    shell('apt-get update')
    shell('apt-get install --assume-yes %s' % (' '.join(HOST_PACKAGES)))

    login  = subprocess.check_output(['logname'], stderr=subprocess.STDOUT).strip()
    chroot = config[1+config.rindex('-'):]
    for arch in ['amd64', 'i386']:
        print '******************* %s-%s' % (chroot, arch)
        root_dir = '/opt/wkhtmltopdf-build/%s-%s' % (chroot, arch)
        rmdir(root_dir)
        mkdir_p(root_dir)
        for command in CHROOT_SETUP[chroot]:
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
            elif name == 'rinse':
                cmd = (arch == 'i386' and 'linux32 rinse' or 'rinse')
                shell('%s --arch %s --distribution %s --directory %s' % (cmd, arch, command[1], root_dir))
            elif name == 'shell':
                cmd = (arch == 'i386' and 'linux32 chroot' or 'chroot')
                shell('%s %s %s' % (cmd, root_dir, command[1]))
            elif name == 'write_file':
                open(os.path.join(root_dir, command[1]), 'w').write(command[2].strip())
            elif name == 'append_file':
                open(os.path.join(root_dir, command[1]), 'a').write(command[2].strip())
            elif name == 'schroot_conf':
                cfg = open('/etc/schroot/chroot.d/wkhtmltopdf-%s-%s' % (chroot, arch), 'w')
                cfg.write('[wkhtmltopdf-%s-%s]\n' % (chroot, arch))
                cfg.write('type=directory\ndirectory=%s/\n' % root_dir)
                cfg.write('description=%s %s for wkhtmltopdf\n' % (command[1], arch))
                cfg.write('users=%s\nroot-users=root\n' % login)
                if arch == 'i386':
                    cfg.write('personality=linux32\n')
                cfg.close()

def check_update_schroot(config):
    import platform
    if not sys.platform.startswith('linux') or \
       not platform.architecture()[0].startswith('64') or \
       platform.linux_distribution()[0] not in ['Ubuntu', 'Debian']:
        error('This can only be run on a 64-bit Debian/Ubuntu distribution, aborting.')

    if os.geteuid() != 0:
        error('This script must be run as root.')

    try:
        subprocess.check_output(['schroot', '--list'], stderr=subprocess.STDOUT)
    except subprocess.CalledProcessError:
        error('Unable to determine the list of available schroots.')

def build_update_schroot(config, basedir):
    names = subprocess.check_output(['schroot', '--list'], stderr=subprocess.STDOUT).strip()
    for name in names.split('\n'):
        print '******************* %s' % name[name.index('wkhtmltopdf-'):]
        shell('schroot -c %s -- /bin/bash /update.sh' % name[name.index('wkhtmltopdf-'):])

# --------------------------------------------------------------- MSVC (2008-2013)

MSVC_LOCATION = {
    'msvc2008': 'VS90COMNTOOLS',
    'msvc2010': 'VS100COMNTOOLS',
    'msvc2012': 'VS110COMNTOOLS',
    'msvc2013': 'VS120COMNTOOLS'
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
    process = subprocess.Popen('("%s" %s>nul)&&"%s" -c "import os; print repr(os.environ)"' % (
        os.path.join(vcdir, 'vcvarsall.bat'), vcarg, python), stdout=subprocess.PIPE, shell=True)
    stdout, _ = process.communicate()
    exitcode = process.wait()
    if exitcode != 0:
        error("%s: unable to initialize the environment" % msvc)

    os.environ.update(eval(stdout.strip()))

    build_msvc_common(config, basedir)

# --------------------------------------------------------------- MSVC via Windows SDK 7.1

def check_msvc_winsdk71(config):
    for pfile in ['ProgramFiles(x86)', 'ProgramFiles']:
        if pfile in os.environ and exists(os.path.join(os.environ[pfile], 'Microsoft SDKs', 'Windows', 'v7.1', 'Bin', 'SetEnv.cmd')):
            return
    error("Unable to detect the location of Windows SDK 7.1")

def build_msvc_winsdk71(config, basedir):
    arch = config[config.rindex('-'):]
    setenv = None
    for pfile in ['ProgramFiles(x86)', 'ProgramFiles']:
        if not pfile in os.environ:
            continue
        setenv = os.path.join(os.environ[pfile], 'Microsoft SDKs', 'Windows', 'v7.1', 'Bin', 'SetEnv.cmd')

    mode = debug and '/Debug' or '/Release'
    if arch == 'win64':
        args = '/2008 /x64 %s' % mode
    else:
        args = '/2008 /x86 %s' % mode

    python = sys.executable
    process = subprocess.Popen('("%s" %s>nul)&&"%s" -c "import os; print repr(os.environ)"' % (
        setenv, args, python), stdout=subprocess.PIPE, shell=True)
    stdout, _ = process.communicate()
    exitcode = process.wait()
    if exitcode != 0:
        error("unable to initialize the environment for Windows SDK 7.1")

    os.environ.update(eval(stdout.strip()))

    build_msvc_common(config, basedir)

def build_msvc_common(config, basedir):
    version, simple_version = get_version(basedir)
    ssl_libs = build_openssl(config, basedir)

    ssldir = os.path.join(basedir, config, 'openssl')
    qtdir  = os.path.join(basedir, config, 'qt')
    mkdir_p(qtdir)

    args = []
    args.extend(QT_CONFIG['common'])
    args.extend(QT_CONFIG['msvc'])
    args.append('-I %s\\include' % ssldir)
    args.append('-L %s\\lib' % ssldir)
    args.append('OPENSSL_LIBS="-L%s -lssleay32 -llibeay32 %s"' % \
        (ssldir.replace('\\', '\\\\'), ssl_libs))

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

    os.environ['WKHTMLTOX_VERSION'] = version

    shell('%s\\bin\\qmake %s\\..\\wkhtmltopdf.pro' % (qtdir, basedir))
    shell('nmake')

    found = False
    for pfile in ['ProgramFiles(x86)', 'ProgramFiles']:
        if not pfile in os.environ or not exists(os.path.join(os.environ[pfile], 'NSIS', 'makensis.exe')):
            continue
        found = True

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
    shell('%s-gcc --version' % MINGW_W64_PREFIX[rchop(config, '-dbg')])

def build_mingw64_cross(config, basedir):
    version, simple_version = get_version(basedir)
    ssl_libs = build_openssl(config, basedir)

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
    args.append('-device-option CROSS_COMPILE=%s-' % MINGW_W64_PREFIX[rchop(config, '-dbg')])

    os.environ['OPENSSL_LIBS'] = '-lssl -lcrypto -L %s/lib %s' % (ssldir, ssl_libs)

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
    os.environ['WKHTMLTOX_VERSION'] = version
    shell('%s/bin/qmake -set CROSS_COMPILE %s-' % (qtdir, MINGW_W64_PREFIX[rchop(config, '-dbg')]))
    shell('%s/bin/qmake -spec win32-g++-4.6 %s/../wkhtmltopdf.pro' % (qtdir, basedir))
    shell('make')
    shutil.copy('bin/libwkhtmltox0.a', 'bin/wkhtmltox.lib')

    os.chdir(os.path.join(basedir, '..'))
    shell('makensis -DVERSION=%s -DSIMPLE_VERSION=%s -DTARGET=%s wkhtmltox.nsi' % \
            (version, simple_version, config))

# -------------------------------------------------- Linux schroot environment

def check_linux_schroot(config):
    shell('schroot -c wkhtmltopdf-%s -- gcc --version' % rchop(config, '-dbg'))

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
    lines.append('export WKHTMLTOX_VERSION=%s' % version)
    lines.append('../qt/bin/qmake ../../../wkhtmltopdf.pro')
    lines.append('make -j%d || exit 1' % CPU_COUNT)
    lines.append('strip bin/wkhtmltopdf bin/wkhtmltoimage')
    lines.append('cp bin/wkhtmlto* ../wkhtmltox-%s/bin' % version)
    lines.append('cp -P bin/libwkhtmltox*.so.* ../wkhtmltox-%s/lib' % version)
    lines.append('cp ../../../include/wkhtmltox/*.h ../wkhtmltox-%s/include/wkhtmltox' % version)
    lines.append('cp ../../../include/wkhtmltox/dll*.inc ../wkhtmltox-%s/include/wkhtmltox' % version)
    lines.append('cd ..')
    lines.append('tar -c -v -f ../wkhtmltox-%s_linux-%s.tar wkhtmltox-%s/' % (version, config, version))
    lines.append('xz --compress -9 ../wkhtmltox-%s_linux-%s.tar' % (version, config))
    lines.append('# end of build script')

    open(script, 'w').write('\n'.join(lines))
    os.chdir(dir)
    shell('chmod +x build.sh')
    shell('schroot -c wkhtmltopdf-%s -- ./build.sh' % rchop(config, '-dbg'))

# --------------------------------------------------------------- command line

def usage(exit_code=2):
    print "Usage: scripts/build.py <target> [-clean] [-debug]\n\nThe supported targets are:\n",
    opts = list(BUILDERS.keys())
    opts.sort()
    for opt in opts:
        print '* %s' % opt
    sys.exit(exit_code)

def main():
    basedir = os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', 'static-build')
    mkdir_p(basedir)

    if len(sys.argv) == 1:
        usage(0)

    config = sys.argv[1]
    if config not in BUILDERS:
        usage()

    for arg in sys.argv[2:]:
        if not arg in ['-clean', '-debug']:
            usage()

    final_config = config
    if '-debug' in sys.argv[2:]:
        # use the debug OpenSSL configuration if possible
        ssl = OPENSSL['build']
        for key in ssl:
            if fnmatch.fnmatch(config, key) and 'debug' in ssl[key]:
                ssl[key]['configure'] = ssl[key]['debug']
        # use a debug build of QT and WebKit
        cfg = QT_CONFIG['common']
        cfg[cfg.index('-release')] = '-debug'
        cfg[cfg.index('-webkit')]  = '-webkit-debug'
        final_config += '-dbg'

    if '-clean' in sys.argv[2:]:
        rmdir(os.path.join(basedir, config))

    globals()['check_%s' % BUILDERS[config]](final_config)
    globals()['build_%s' % BUILDERS[config]](final_config, os.path.realpath(basedir))

if __name__ == '__main__':
    main()
