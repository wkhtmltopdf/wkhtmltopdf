---
layout: default
---

## Stable

The current stable version is [0.12.0](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/tag/0.12.0), which was released on Febuary 6, 2014.

OS      | Downloads
---     | ---------
Windows | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.0/wkhtmltox-win32_0.12.0-03c001d.exe)         &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.0/wkhtmltox-win64_0.12.0-03c001d.exe) (vulnerable to [Heartbleed](http://heartbleed.com/), use 0.12.1 instead)
Linux   | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.0/wkhtmltox-linux-i386_0.12.0-03c001d.tar.xz) &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.0/wkhtmltox-linux-amd64_0.12.0-03c001d.tar.xz)

## Development

A release candidate [0.12.1-rc](https://github.com/wkhtmltopdf/wkhtmltopdf/tree/c22928d) of the master branch is available, which includes [fixes since the 0.12.0 release](https://github.com/wkhtmltopdf/wkhtmltopdf/blob/c22928d/CHANGELOG.md).

OS/Distro             | Downloads                                                                                                                                                                                                                                                         | Comments
---------             | ---------                                                                                                                                                                                                                                                         | --------
Windows (MSVC 2013)   | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-c22928d_msvc2013-win32.exe)        &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-c22928d_msvc2013-win64.exe)         | for Windows Vista or later; faster
Windows (MinGW-w64)   | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-c22928d_mingw-w64-cross-win32.exe) &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-c22928d_mingw-w64-cross-win64.exe)  | will work on Windows XP/2003; slower
Linux (Debian Wheezy) | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-c22928d_linux-wheezy-i386.tar.xz)  &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-c22928d_linux-wheezy-amd64.tar.xz)  | for recent distributions (i.e. glibc 2.13 or later)
Linux (CentOS 5)      | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-c22928d_linux-centos5-i386.tar.xz) &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-c22928d_linux-centos5-amd64.tar.xz) | for old distributions (i.e. glibc 2.5 or later)
Mac OS X              | not available                                                                                                                                                                                                                                                     | need volunteer(s) for changing build script + making snapshots

The next release will be made once the [preparations for 0.12.1](https://github.com/wkhtmltopdf/wkhtmltopdf/issues/1663) have been completed.

## Archive

If you're looking for older versions (such as 0.9, 0.10, 0.11), you can find them on [Google Code](http://code.google.com/p/wkhtmltopdf/downloads/list?can=1).

**Please note that bug reports won't be accepted for old versions.**
