---
layout: default
---

All downloads are [hosted via GitHub releases](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/), so you can browse for a specific download or use the links below.

## Stable

The current stable series is **0.12.4**, which was released on November 22, 2016 -- see changes [since 0.12.3](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/tag/0.12.4).

Flavor          | Version      | Downloads                                                                                                                                                                                                                                                   | Comments
------          | -------      | ---------                                                                                                                                                                                                                                                   | --------
Windows (MSVC)  | 0.12.4       | [32-bit](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.4/wkhtmltox-0.12.4_msvc2015-win32.exe)             /   [64-bit](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.4/wkhtmltox-0.12.4_msvc2015-win64.exe)         | for Windows Vista/2008 or later; bundles VC++ Runtime 2015
Windows (MinGW) | 0.12.4       | [32-bit](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.4/wkhtmltox-0.12.4_mingw-w64-cross-win32.exe)      /   [64-bit](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.4/wkhtmltox-0.12.4_mingw-w64-cross-win64.exe)  | for Windows XP/2003 or later; standalone
Linux           | 0.12.4       | [32-bit](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.4/wkhtmltox-0.12.4_linux-generic-i386.tar.xz)      /   [64-bit](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.4/wkhtmltox-0.12.4_linux-generic-amd64.tar.xz) | depends on: zlib, fontconfig, freetype, X11 libs (libX11, libXext, libXrender)
OS X            | 0.12.4       | [32-bit](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.4/wkhtmltox-0.12.4_osx-carbon-i386.pkg)            /   [64-bit](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.4/wkhtmltox-0.12.4_osx-cocoa-x86-64.pkg)       | [**(has regression)**](https://github.com/wkhtmltopdf/wkhtmltopdf/issues/3241) for OS X 10.6 or later
Others          | 0.12.4       | [source code](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.4/wkhtmltox-0.12.4.tar.bz2)                                                                                                                                                 | read [INSTALL.md](https://github.com/wkhtmltopdf/wkhtmltopdf/blob/0.12.4/INSTALL.md#others) for compilation instructions

## Release Candidate

A release candidate for the next release **0.12.5-rc** is available, please test and report issues before the final release.

<table>
    <thead>
        <tr>
            <th>OS</th>
            <th>Flavor</th>
            <th>Downloads</th>
            <th>Comments</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan="2">Windows</td>
            <td>MSVC 2015</td>
            <td>
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox-0.12.5-0.20180604.140.rc~6f77c46~msvc2015-win32.exe">win32</a>
                /
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox-0.12.5-0.20180604.140.rc~6f77c46~msvc2015-win64.exe">win64</a>
            </td>
            <td>Installer for Windows Vista or later</td>
        </tr>
        <tr>
            <td><a href="http://mxe.cc">MXE</a> (Mingw-w64)</td>
            <td>
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox-0.12.5-0.20180604.140.rc~6f77c46~mxe-cross-win32.7z">win32</a>
                /
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox-0.12.5-0.20180604.140.rc~6f77c46~mxe-cross-win64.7z">win64</a>
            </td>
            <td>7z archive for Windows XP/2003 or later</td>
        </tr>
        <tr>
            <td rowspan="7">Linux</td>
            <td>Debian 9 (stretch)</td>
            <td>
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox_0.12.5-0.20180604.140.rc~6f77c46~stretch_amd64.deb">amd64</a>
                /
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox_0.12.5-0.20180604.140.rc~6f77c46~stretch_i386.deb">i386</a>
                /
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox_0.12.5-0.20180604.140.rc~6f77c46~raspbian.stretch_armhf.deb">raspbian</a>
            </td>
            <td>Package (.deb) built on Debian 9.4</td>
        </tr>
        <tr>
            <td>Debian 8 (jessie)</td>
            <td>
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox_0.12.5-0.20180604.140.rc~6f77c46~jessie_amd64.deb">amd64</a>
                /
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox_0.12.5-0.20180604.140.rc~6f77c46~jessie_i386.deb">i386</a>
            </td>
            <td>Package (.deb) built on Debian 8.10</td>
        </tr>
        <tr>
            <td>Ubuntu 18.04 (bionic)</td>
            <td>
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox_0.12.5-0.20180604.140.rc~6f77c46~bionic_amd64.deb">amd64</a>
                /
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox_0.12.5-0.20180604.140.rc~6f77c46~bionic_i386.deb">i386</a>
            </td>
            <td>Package (.deb) built on Ubuntu 18.04</td>
        </tr>
        <tr>
            <td>Ubuntu 16.04 (xenial)</td>
            <td>
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox_0.12.5-0.20180604.140.rc~6f77c46~xenial_amd64.deb">amd64</a>
                /
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox_0.12.5-0.20180604.140.rc~6f77c46~xenial_i386.deb">i386</a>
            </td>
            <td>Package (.deb) built on Ubuntu 16.04.4</td>
        </tr>
        <tr>
            <td>Ubuntu 14.04 (trusty)</td>
            <td>
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox_0.12.5-0.20180604.140.rc~6f77c46~trusty_amd64.deb">amd64</a>
                /
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox_0.12.5-0.20180604.140.rc~6f77c46~trusty_i386.deb">i386</a>
            </td>
            <td>Package (.deb) built on Ubuntu 14.04.5</td>
        </tr>
        <tr>
            <td>CentOS 7</td>
            <td>
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox-0.12.5-0.20180604.140.rc~6f77c46~centos7.x86_64.rpm">x86_64</a>
                /
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox-0.12.5-0.20180604.140.rc~6f77c46~centos7.i686.rpm">i686</a>
            </td>
            <td>Package (.rpm) built on CentOS 7.4.1804</td>
        </tr>
        <tr>
            <td>CentOS 6</td>
            <td>
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox-0.12.5-0.20180604.140.rc~6f77c46~centos6.x86_64.rpm">x86_64</a>
                /
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox-0.12.5-0.20180604.140.rc~6f77c46~centos6.i686.rpm">i686</a>
            </td>
            <td>Package (.rpm) built on CentOS 6.9</td>
        </tr>
        <tr>
            <td rowspan="2">macOS</td>
            <td>Cocoa</td>
            <td>
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox-0.12.5-0.20180604.140.rc~6f77c46~macos-cocoa.pkg">64-bit</a>
            </td>
            <td>Installer (.pkg) for OS X 10.7 or later</td>
        </tr>
        <tr>
            <td>Carbon</td>
            <td>
                <a href="https://builds.wkhtmltopdf.org/0.12.5-rc/wkhtmltox-0.12.5-0.20180604.140.rc~6f77c46~macos-carbon.pkg">32-bit</a>
            </td>
            <td>Installer (.pkg) for OS X 10.7 or later</td>
        </tr>
    </tbody>
</table>

## Bleeding Edge

An [alpha release](https://github.com/wkhtmltopdf/wkhtmltopdf/blob/0.13/README.md#013-alpha) based on Qt `5.4.2` (with an updated browser engine) is available. Please note that this is a work-in-progress and not all the features have been ported to it yet, so do not expect any support for any bugs you may encounter.

Flavor                | Downloads                                                                                                                                                                                                                                                   | Comments
---------             | ---------                                                                                                                                                                                                                                                   | --------
Windows (MSVC 2013)   | [32-bit](https://bitbucket.org/wkhtmltopdf/wkhtmltopdf/downloads/wkhtmltox-0.13.0-alpha-7b36694_msvc2013-win32.exe)         /    [64-bit](https://bitbucket.org/wkhtmltopdf/wkhtmltopdf/downloads/wkhtmltox-0.13.0-alpha-7b36694_msvc2013-win64.exe)        | for Windows Vista or later; bundles VC++ Runtime 2013
Windows (MinGW-w64)   | [32-bit](https://bitbucket.org/wkhtmltopdf/wkhtmltopdf/downloads/wkhtmltox-0.13.0-alpha-7b36694_mingw-w64-cross-win32.exe)  /    [64-bit](https://bitbucket.org/wkhtmltopdf/wkhtmltopdf/downloads/wkhtmltox-0.13.0-alpha-7b36694_mingw-w64-cross-win64.exe) | for Windows XP/2003 or later; bundles gcc DLLs
Linux (Debian Wheezy) | [32-bit](https://bitbucket.org/wkhtmltopdf/wkhtmltopdf/downloads/wkhtmltox-0.13.0-alpha-7b36694_linux-wheezy-i386.deb)      /    [64-bit](https://bitbucket.org/wkhtmltopdf/wkhtmltopdf/downloads/wkhtmltox-0.13.0-alpha-7b36694_linux-wheezy-amd64.deb)    | built on Debian 7.8
Linux (Debian Jessie) | [32-bit](https://bitbucket.org/wkhtmltopdf/wkhtmltopdf/downloads/wkhtmltox-0.13.0-alpha-7b36694_linux-jessie-i386.deb)      /    [64-bit](https://bitbucket.org/wkhtmltopdf/wkhtmltopdf/downloads/wkhtmltox-0.13.0-alpha-7b36694_linux-jessie-amd64.deb)    | built on Debian 8 (prerelease)
Linux (Ubuntu Trusty) | [32-bit](https://bitbucket.org/wkhtmltopdf/wkhtmltopdf/downloads/wkhtmltox-0.13.0-alpha-7b36694_linux-trusty-i386.deb)      /    [64-bit](https://bitbucket.org/wkhtmltopdf/wkhtmltopdf/downloads/wkhtmltox-0.13.0-alpha-7b36694_linux-trusty-amd64.deb)    | built on Ubuntu 14.04.2
Linux (Ubuntu Precise)| [32-bit](https://bitbucket.org/wkhtmltopdf/wkhtmltopdf/downloads/wkhtmltox-0.13.0-alpha-7b36694_linux-precise-i386.deb)     /    [64-bit](https://bitbucket.org/wkhtmltopdf/wkhtmltopdf/downloads/wkhtmltox-0.13.0-alpha-7b36694_linux-precise-amd64.deb)   | built on Ubuntu 12.04.5
Linux (CentOS 7)      | [64-bit](https://bitbucket.org/wkhtmltopdf/wkhtmltopdf/downloads/wkhtmltox-0.13.0-alpha-7b36694_linux-centos7-amd64.rpm)                                                                                                                                    | built on CentOS 7.1.1503
Linux (CentOS 6)      | [64-bit](https://bitbucket.org/wkhtmltopdf/wkhtmltopdf/downloads/wkhtmltox-0.13.0-alpha-7b36694_linux-centos6-amd64.rpm)                                                                                                                                    | built on CentOS 6.6

Builds for OS X will be made available as soon as support for it is added in the build script, which is an ongoing process. Please note that the above linux builds are **not** headless, and will require running under `xvfb`.

## Archive

Please note that bug reports **will not be accepted** against the following, which are considered obsolete. It is recommended to use the latest stable release instead, and report an issue if there is a regression from a previous release.

Date       | Release
----       | -------
2016-03-02 | [0.12.3.2](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/0.12.3.2/) (windows-only)
2016-01-30 | [0.12.3.1](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/0.12.3.1/) (windows-only)
2016-01-20 | [0.12.3](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/0.12.3/)
2015-07-12 | [0.12.2.4](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/0.12.2.4/) (windows-only)
2015-06-20 | [0.12.2.3](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/0.12.2.3/) (windows-only)
2015-04-06 | [0.12.2.2](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/0.12.2.2/) (windows-only)
2015-01-19 | [0.12.2.1](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/0.12.2.1/)
2015-01-09 | [0.12.2](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/0.12.2/)
2014-06-26 | [0.12.1](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/0.12.1/)
2014-02-06 | [0.12.0](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/0.12.0/)

If you need versions older than `0.12.0`, you can look at the [obsolete downloads](https://github.com/wkhtmltopdf/obsolete-downloads/blob/master/README.md).

## FAQ

#### Where are all the "distro-specific" binaries?

The generic linux binaries should work across all distributions as they are built on CentOS 6 with statically-linked `libpng` and `libjpeg`. However, in case of vulnerabilities in either library a new release will have to be done -- similar to what was required for Windows earlier. Support for the distro-specific binaries is still present in the build script but is likely to be removed in future.

#### My platform is not supported! What should I do?

Compile the source code with the instructions given above. There are unoffical build instructions for [SmartOS](https://github.com/wkhtmltopdf/wkhtmltopdf/issues/1794) and [OpenBSD](https://github.com/wkhtmltopdf/wkhtmltopdf/issues/1991), but they are still a work in progress.

#### Symantec reports a virus `WS.Reputation.1` for the Windows builds

This is a false positive reported because Symantec has not seen this file before -- see [this clarification](http://community.norton.com/forums/clarification-wsreputation1-detection) for details.
