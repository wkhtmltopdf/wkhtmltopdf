Clone this repository by running the following command:

    git clone --recursive https://github.com/wkhtmltopdf/wkhtmltopdf.git

If you are on Windows, make sure that you are cloning in a location without
spaces/special characters in the name.

Prerequisites: Linux
--------------------

Building is currently supported only on recent Debian/Ubuntu releases. All
binaries are produced in a self-contained chroot environment for the target
distribution, so you will need to setup it up first by running
```scripts/build.py```. The following targets are currently supported:

Target         | Command for Setup
------         | -----------------
Debian Wheezy  | ```sudo scripts/build.py setup-schroot-wheezy```
Ubuntu Trusty  | ```sudo scripts/build.py setup-schroot-trusty```
Ubuntu Precise | ```sudo scripts/build.py setup-schroot-precise```
CentOS 6       | ```sudo scripts/build.py setup-schroot-centos6```
CentOS 5       | ```sudo scripts/build.py setup-schroot-centos5```
MinGW-w64      | ```sudo scripts/build.py setup-mingw-w64```

Please note that you should run the above commands while logged in as a
regular user who has ```sudo``` access and that the cloned respository
should be in the user's home directory e.g. ```~/wkhtmltopdf```. **Do
not attempt to clone the repository or run any other command as root!**

The MinGW-w64 toolchain can cross-compile 32/64-bit Windows binaries from
Linux -- it is useful for targetting Windows XP/Windows 2003, which are not
supported by default when compiling with MSVC 2013.

Prerequisites: Windows
----------------------

* Install Visual Studio 2008 or later ([2013 Express](http://www.microsoft.com/en-US/download/details.aspx?id=40787)
  is recommended) or follow instructions for [Windows SDK 7.1](http://qt-project.org/wiki/Category:Tools::msvc)
* Do "Windows Update" to ensure that VC/SDK security patches are up-to-date
* Install the latest [ActivePerl](http://www.activestate.com/activeperl/downloads) release
* Install the latest [Python 2.7](http://www.python.org/downloads/windows/) release
* Install [NSIS 2.46](http://nsis.sourceforge.net/Download)
* Make sure that you can run "git". If not, add it to the PATH or reinstall
  with option "Run Git from the Windows Command Prompt".

Prerequisites: OS X
-------------------

wkhtmltopdf can be build for the Carbon (32Bit) and Cocoa (64Bit) frameworks.
Carbon builds can be made on OS X 10.6 and newer, Cocoa builds require OS X 10.7
or newer. In any case you need to have the following installed:

* The latest Xcode for your OS X version.
* Python 2.7.x. (Note: OS X 10.6 comes with an older verison, a newer one is
  available on the [Python downloads](https://www.python.org/download/releases/2.7.6/) page.
  After installing you should run the `Update Shell Profile.command` in `/Applications/Python 2.7`
  to make it the default Python in the shell.)
* Git (For OS X 10.6 please use [git 1.8.4.2](https://code.google.com/p/git-osx-installer/downloads/detail?name=git-1.8.4.2-intel-universal-snow-leopard.dmg)
  since at the time of writing newer versions fail on execution.)
* [xz 5.0.5](http://sourceforge.net/projects/macpkg/files/XZ/5.0.5/)


### Cocoa

To build wkhtmltopdf for Cocoa make sure to follow the instructions in the
**Building** section below and start the build with

```
./scripts/build.py osx-cocoa-x86-64 -clean
```

from within the directory where you have checked out the Git repository,
e. g. `~/wkhtmltopdf`. There are no further special requirements for
the Cocoa build.

### Carbon

For the Carbon build, you will need to have the OS X 10.6 SDK installed
so that running `xcodebuild -sdk macosx10.6 -version Path` returns
the path of the installed SDK.

Newer versions of Xcode no longer contain a 10.6 SDK. It's possible to extract
the SDK files from an older Xcode download (available with an Apple developer
account) without installing this old Xcode version. Search for *Xcode 3.2.6* 
in the downloads section and download *Xcode 3.2.6 and iOS SDK 4.3*. If you've
downloaded the DMG file to `~/Downloads` you can do the following to extract
the SDK:

```
cd ~/Downloads
mkdir OSX10.6.SDK
cd OSX10.6.SDK
hdiutil attach ../xcode_3.2.6_and_ios_sdk_4.3.dmg
cp /Volumes/Xcode\ and\ iOS\ SDK/Packages/MacOSX10.6.pkg .
xar -xf MacOSX10.6.pkg .
cat Payload | gunzip -dc | cpio -i
mv SDKs/MacOSX10.6.sdk/ .
rm -df SDKs Bom PackageInfo Payload MacOSX10.6.pkg
```

There is now a folder named `MacOSX10.6.sdk` in `~/Downloads/OSX10.6.SDK`
which needs to be moved to `/Developer/SDKs/` or 
`/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/`,
depending on the version of your current Xcode installation. Moving the SDK
normally needs `sudo`.

The Carbon build process also needs `libstdc++.dylib`. Symlinking is
sufficient, so please run the command:

```
cd $SDK_DIR/usr/lib
sudo ln -s libstdc++.6.0.9.dylib libstdc++.dylib
```

where `$SDK_DIR` is the location of the 10.6 SDK. Symlinking is also possible
in `/usr/local/lib`, in this case the symlink should point to `/usr/lib/libstdc++.dylib`.

To build wkhtmltopdf for Carbon make sure to follow the instructions in the
**Building** section below and start the build with

```
./scripts/build.py osx-carbon-i386 -clean
```

from within the directory where you have checked out the Git repository,
e. g. `~/wkhtmltopdf`.


Building
--------

* Ensure that you are using the correct Qt version by running ```git submodule update```
* Run the command ```scripts/build.py``` (or ```scripts\build.py``` if you
  are on Windows) to get a list of all targets which can be built.
* If you want to compile on a distribution not listed above or for another
  Unix-like OS, please use the ```posix-local``` target -- it assumes
  that you have already installed all build dependencies beforehand.
* If you want to customize the default Qt configuration options for your
  target, please set the ```WKHTMLTOX_QT_CONFIG``` environment variable
  before running the above command. Adding a prefix of ```remove:```
  will ensure that it will be removed from the options, if already present.
  e.g. ```WKHTMLTOX_QT_CONFIG="remove:-no-fast"``` will disable the fast
  makefile generation mode while configuring Qt.
* After the target has been built, the output installer/package will be
  available in the ```static-build``` folder.
