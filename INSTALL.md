Clone this repository by running the following command:

    git clone --recursive https://github.com/wkhtmltopdf/wkhtmltopdf.git

If you are on Windows, make sure that you are cloning in a location without spaces/special characters in the name.

Prerequisites: Linux
--------------------

Building is currently supported only on the current 64-bit Debian stable release (**wheezy**), but it could work on other Debian/Ubuntu releases as well. Please run ```scripts/setup-linux.sh``` as root, which will create all the chroot environments and configure them for building properly. A total of 4 chroot environments is created (approx 2GB will need to be downloaded):
 * Debian-wheezy i386  (should work on Ubuntu 12.04 LTS and later)
 * Debian-wheezy amd64 (should work on Ubuntu 12.04 LTS and later)
 * CentOS-5 i386  (should work on systems where older GLIBC is present)
 * CentOS-5 amd64 (should work on systems where older GLIBC is present)

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
