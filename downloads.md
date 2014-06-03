---
layout: default
---

## Stable

The current stable version is [0.12.0](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/tag/0.12.0), which was released on Febuary 6, 2014.

OS      | Downloads
---     | ---------
Windows | vulnerable to [Heartbleed](http://heartbleed.com/), use 0.12.1 instead
Linux   | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.0/wkhtmltox-linux-i386_0.12.0-03c001d.tar.xz) &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.0/wkhtmltox-linux-amd64_0.12.0-03c001d.tar.xz)

## Testing

The final release candidate [0.12.1-rc2](https://github.com/wkhtmltopdf/wkhtmltopdf/tree/61cda93) for the next release is available, which includes [fixes since the 0.12.0 release](https://github.com/wkhtmltopdf/wkhtmltopdf/blob/61cda93/CHANGELOG.md).

Flavor                | Downloads                                                                                                                                                                                                                                                         | Comments
---------             | ---------                                                                                                                                                                                                                                                         | --------
Windows (MSVC 2013)   | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-61cda93_msvc2013-win32.exe)        &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-61cda93_msvc2013-win64.exe)         | **recommended**; for Windows Vista or later
Windows (MinGW-w64)   | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-61cda93_mingw-w64-cross-win32.exe) &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-61cda93_mingw-w64-cross-win64.exe)  | for Windows XP/2003 or later; slower
Linux (Debian Wheezy) | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-61cda93_linux-wheezy-i386.tar.xz)  &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-61cda93_linux-wheezy-amd64.tar.xz)  | built on Debian 7.5
Linux (Ubuntu Trusty) | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-61cda93_linux-trusty-i386.tar.xz)  &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-61cda93_linux-trusty-amd64.tar.xz)  | built on Ubuntu 14.04
Linux (Ubuntu Precise)| [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-61cda93_linux-precise-i386.tar.xz) &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-61cda93_linux-precise-amd64.tar.xz) | built on Ubuntu 12.04.4
Linux (CentOS 6)      | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-61cda93_linux-centos6-i386.tar.xz) &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-61cda93_linux-centos6-amd64.tar.xz) | built on CentOS 6.5
Linux (CentOS 5)      | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-61cda93_linux-centos5-i386.tar.xz) &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-61cda93_linux-centos5-amd64.tar.xz) | built on CentOS 5.10
OS X 10.6+ (Carbon)   | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-61cda93_osx-carbon-i386.tar.xz)                                                                                                                                         | **recommended**; produces selectable text and smaller file sizes
OS X 10.6+ (Cocoa)    | [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-61cda93_osx-cocoa-x86-64.tar.xz)                                                                                                                                        | text not selectable and larger file sizes, see [QTBUG-10094](https://bugreports.qt-project.org/browse/QTBUG-10094)

The next release will be made once the [preparations for 0.12.1](https://github.com/wkhtmltopdf/wkhtmltopdf/issues/1663) have been completed.

## Archive

If you're looking for older versions (such as 0.9, 0.10, 0.11), you can find them on [Google Code](http://code.google.com/p/wkhtmltopdf/downloads/list?can=1).

**Please note that bug reports won't be accepted for versions older than 0.12**
