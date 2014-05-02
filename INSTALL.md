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
CentOS 5       | ```sudo scripts/build.py setup-schroot-centos5```
Ubuntu Trusty  | ```sudo scripts/build.py setup-schroot-trusty```
CentOS 6       | ```sudo scripts/build.py setup-schroot-centos6```
Ubuntu Precise | ```sudo scripts/build.py setup-schroot-precise```
MinGW-w64      | ```sudo scripts/build.py setup-mingw-w64```

Please note that you should run the above commands while logged in as a
regular user who has ```sudo``` access. **Do not attempt to clone the
repository or run any other command as root!**

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

Building
--------

Switch to the checked-out folder and run the command ```scripts/build.py```
(or ```scripts\build.py``` if you are on Windows). This will present all
the targets which you can build. Select the appropriate target and the
output package will be generated in the ```static-build``` folder.
