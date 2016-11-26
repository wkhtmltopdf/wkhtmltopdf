#!/usr/bin/env python

# Copyright 2016 wkhtmltopdf authors
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
# along with wkhtmltopdf.  If not, see <http://www.gnu.org/licenses/>.

from subprocess import check_output, check_call, CalledProcessError, STDOUT
from hashlib    import sha1
from sys        import stderr, exit
from os         import path, makedirs, remove
from yaml       import load as load_yaml
from shutil     import rmtree, copy as copy_file
from fnmatch    import fnmatch
from tarfile    import open as open_tar
from platform   import system

class Builder(object):
    def __init__(self, target, debug, build_dir):
        self.target    = target
        self.debug     = debug
        self.src_dir   = path.dirname(__file__)
        self.build_dir = build_dir
        with open(path.join(self.src_dir, 'build.yml'), 'r') as f:
            self.config = load_yaml(f.read())

    def rmdir(self, dir):
        if path.exists(dir):
            if system() == 'Windows':
                self.shell('attrib -R %s\* /S' % dir)
            rmtree(dir)

    def abort(self, msg):
        stderr.write(msg+'\n')
        exit(1)

    def shell(self, cmd, **kwargs):
        try:
            check_call(cmd, shell=True, **kwargs)
        except CalledProcessError:
            self.abort("command failed: %s" % cmd)

    def output(self, *cmd, **kwargs):
        try:
            return check_output(cmd, stderr=STDOUT, **kwargs).decode('utf-8').strip()
        except CalledProcessError:
            return None

    def mkdir_p(self, dir):
        if not path.exists(dir):
            makedirs(dir)

    def download_targz(self, config, outdir):
        self.shell('curl -L -o %(name)s.tar.gz %(url)s' % config, cwd=self.build_dir)

        location = path.join(self.build_dir, '%(name)s.tar.gz' % config)
        with open(location, 'rb') as f:
            hash = sha1(f.read()).hexdigest()
            if hash != config['sha1']:
                remove(location)
                self.abort('Checksum mismatch for %(name)s, aborting.' % config)

        tar  = open_tar(location)
        root = tar.getnames()[0].split('/')[0]
        self.rmdir(path.join(outdir, root))
        tar.extractall(outdir)
        return path.join(outdir, root)

    def build_deplibs(self, env={}, **kwargs):
        destdir = path.join(self.build_dir, 'deplibs')
        vars    = {'destdir': destdir}
        for key, value in kwargs.items():
            vars[key] = value % vars

        def is_compiled(config, libdir):
            for file in config.get('verify', []):
                if not path.exists(path.join(destdir, file)):
                    return False
            for target, source in config.get('install', {}).items():
                if not path.exists(path.join(destdir, target)):
                    if libdir and path.exists(path.join(libdir, source)):
                        self.mkdir_p(path.dirname(path.join(destdir, target)))
                        copy_file(path.join(libdir, source), path.join(destdir, target))
                    else:
                        return False
            return True

        self.mkdir_p(destdir)
        for lib in self.config['dependent-libs']:
            key = [key for key in lib['build'] \
                   if fnmatch(self.target, '*{}*'.format(key))]
            if not key or is_compiled(lib['build'][key[0]], None):
                continue

            config = lib['build'][key[0]]
            libdir = self.download_targz(lib, destdir)

            for location, replacements in config.get('replace', {}).items():
                with open(path.join(libdir, location), 'r') as f:
                    content = f.read()
                for search, replacement in replacements.items():
                    content = content.replace(search, replacement % vars)
                with open(path.join(libdir, location), 'w') as f:
                    f.write(content)

            for command in config['commands'].strip().split('\n'):
                self.shell(command % vars, cwd=libdir)

            if not is_compiled(config, libdir):
                self.abort('Unable to compile %(name)s, aborting.' % lib)

            self.rmdir(libdir)

    @property
    def version(self):
        with open(path.join(self.src_dir, 'VERSION'), 'r') as f:
            text = f.read().strip()

        if '-' not in text:
            return text

        hash = self.output('git', 'rev-parse', '--short', 'HEAD', cwd=self.src_dir)
        if not hash:
            return text

        return '{}-{}'.format(text.split('-')[0], hash)

class VagrantBuilder(Builder):
    def __init__(self, **kwargs):
        super(VagrantBuilder, self).__init__(**kwargs)

    @staticmethod
    def targets(config):
        pass

class Mingw64Builder(VagrantBuilder):
    vagrant_builders = ['mingw']

    def __init__(self, **kwargs):
        super(Mingw64Builder, self).__init__(**kwargs)

def main():
    builder = VagrantBuilder(target='mingw-w64-cross-win32', debug=False, build_dir='/tmp/wk')
    print(builder.version)
    builder.build_deplibs(
        mingw_w64='i686-w64-mingw32',
        mingw_flags='CPPFLAGS=-I%(destdir)s/include CFLAGS=-I%(destdir)s/include LDFLAGS=-L%(destdir)s/lib')

if __name__ == '__main__':
    main()
