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

For the Carbon build, you will need to have the OS X 10.6 SDK installed
so that running `xcodebuild -sdk macosx10.6 -version Path` returns
the path of the installed SDK. You may also need to create a symlink for
`$SDK_DIR/usr/lib/libstdc++.dylib` -- please run the command
`cd $SDK_DIR/usr/lib; sudo ln -s libstdc++.6.0.9.dylib libstdc++.dylib`.
There are no special requirements for the Cocoa build.

Please ensure that you have the latest XCode, git, python 2.7 and xz which
are available for your OS version. You can install XCode by running
`xcode-select --install` in the shell if you are on OS X 10.9 or later.

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
