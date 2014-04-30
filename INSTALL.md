Clone this repository by running the following command:

    git clone --recursive https://github.com/wkhtmltopdf/wkhtmltopdf.git

If you are on Windows, make sure that you are cloning in a location without spaces/special characters in the name.

Prerequisites: Linux
--------------------

Building is currently supported only on recent 64-bit Debian/Ubuntu releases. All binaries are produced in a self-contained chroot environment for the target distribution, so you will need to setup it up first by running ```scripts/build.py```. The following distributions are currently supported:

Distribution   | Command for Setup
------------   | -----------------
Debian Wheezy  | ```scripts/build.py setup-schroot-wheezy```
CentOS 5       | ```scripts/build.py setup-schroot-centos5```

Please note a MinGW-w64 toolchain (which can cross-compile Windows binaries from Linux) is setup by default -- it is useful for targetting Windows XP/Windows 2003, which are not supported by default when compiling with MSVC 2013.

Prerequisites: Windows
----------------------

* Install Visual Studio 2008 or later ([2013 Express](http://www.microsoft.com/en-US/download/details.aspx?id=40787) is recommended) or follow instructions for [Windows SDK 7.1](http://qt-project.org/wiki/Category:Tools::msvc)
* Do "Windows Update" to ensure that VC/SDK security patches are up-to-date
* Install latest ActivePerl from http://www.activestate.com/activeperl/downloads
* Install latest Python 2.7 from http://www.python.org/downloads/windows/
* Install NSIS 2.46 from http://nsis.sourceforge.net/Download
* Make sure that you can run "git". If not, add it to the PATH or reinstall
  with option "Run Git from the Windows Command Prompt".

Building
--------

Switch to the checked-out folder and run the command ```scripts/build.py``` (or ```scripts\build.py``` if you are on Windows). This will present all the options which you can build. Select the appropriate target and all the requisite output will be generated in the ```static-build``` folder.
