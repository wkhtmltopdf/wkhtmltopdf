---
layout: default
---

## Stable

The current stable version is [0.12.0](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/tag/0.12.0), which was released on Febuary 6, 2014.

OS      | Downloads
---     | ---------
Windows | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.0/wkhtmltox-win32_0.12.0-03c001d.exe)         &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.0/wkhtmltox-win64_0.12.0-03c001d.exe) (**vulnerable to [Heartbleed](http://heartbleed.com/)**)
Linux   | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.0/wkhtmltox-linux-i386_0.12.0-03c001d.tar.xz) &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.0/wkhtmltox-linux-amd64_0.12.0-03c001d.tar.xz)

Mac OS X builds could not be produced due to issues in QT 4.8.5 &mdash; you will have to use the latest development snapshot instead.

*Please note that the Windows version was compiled with an outdated version of OpenSSL, which is vulnerable to the Heartbleed bug. It is recommended to use the development version below.*
 
## Development

A rolling development snapshot [0.12.1-f6f29b0](https://github.com/wkhtmltopdf/wkhtmltopdf/tree/f6f29b0) of the master branch is available, which includes [fixes since the 0.12.0 release](https://github.com/wkhtmltopdf/wkhtmltopdf/blob/f6f29b0/CHANGELOG.md).

OS/Distro             | Downloads
---------             | ---------
Windows               | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-f6f29b0_msvc2013-win32.exe)        &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-f6f29b0_msvc2013-win64.exe)
Linux (Debian Wheezy) | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-f6f29b0_linux-wheezy-i386.tar.xz)  &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-f6f29b0_linux-wheezy-amd64.tar.xz)
Linux (CentOS 5)      | [32-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-f6f29b0_linux-centos5-i386.tar.xz) &emsp; [64-bit](http://downloads.sourceforge.net/project/wkhtmltopdf/0.12.1-dev/wkhtmltox-0.12.1-f6f29b0_linux-centos5-amd64.tar.xz)
Mac OS X              | not available

We expect to release version 0.12.1 after QT 4.8.6 is released and after all critical bugs in the current release have been addressed. Mac OS X builds are not available due to support for them not being present in the build script, and volunteer(s) are needed who will be able to make builds for development snapshots.

## Archive

If you're looking for older versions (such as 0.9, 0.10, 0.11), you can find them on [Google Code](http://code.google.com/p/wkhtmltopdf/downloads/list?can=1).

**Please note that bug reports won't be accepted for old versions.**
