---
layout: default
---

All downloads are currently hosted via [Github releases](https://github.com/wkhtmltopdf/wkhtmltopdf/releases), so you can browse for a specific download or use the links below.

**Do not use wkhtmltopdf with any untrusted HTML** -- be sure to sanitize any user-supplied HTML/JS, otherwise it can lead to complete takeover of the server it is running on! Please read the [project status](status.html) for the gory details.

## Release Candidate

This is a [release candidate](https://github.com/wkhtmltopdf/packaging/releases/0.12.6-rc) for **0.12.6**, the final release is expected on June 11, 2020 -- see changes [since 0.12.5](https://github.com/wkhtmltopdf/wkhtmltopdf/blob/master/CHANGELOG.md#v0126-unreleased).

<table>
    <thead>
        <tr>
            <th>OS/Distribution</th>
            <th>Supported on</th>
            <th colspan="7" style="text-align: center">Architectures</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan="2" valign="top">Windows</td>
            <td>Installer (Vista or later)</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.msvc2015-win64.exe">64-bit</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.msvc2015-win32.exe">32-bit</a>
            </td>
            <td colspan="5">&nbsp;</td>
        </tr>
        <tr>
            <td>7z Archive (XP/2003 or later)</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.mxe-cross-win64.7z">64-bit</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.mxe-cross-win32.7z">32-bit</a>
            </td>
            <td colspan="5">&nbsp;</td>
        </tr>
        <tr>
            <td>macOS</td>
            <td>Installer (10.7 or later)</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.macos-cocoa.pkg">64-bit</a>
            </td>
            <td colspan="6">&nbsp;</td>
        </tr>
        <tr>
            <td rowspan="2" valign="top"><a href="https://www.debian.org/releases/">Debian</a></td>
            <td>10 (<code>buster</code>)</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.buster_amd64.deb">amd64</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.buster_i386.deb">i386</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.buster_arm64.deb">arm64</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.buster_armhf.deb">armhf</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.buster_armel.deb">armel</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.buster_ppc64le.deb">ppc64le</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.raspberrypi.buster_armhf.deb">raspberrypi</a>
            </td>
        </tr>
        <tr>
            <td>9 (<code>stretch</code>)</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.stretch_amd64.deb">amd64</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.stretch_i386.deb">i386</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.stretch_arm64.deb">arm64</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.stretch_armhf.deb">armhf</a>
             </td><td>
                <!--<a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.stretch_armel.deb">armel</a>-->
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.stretch_ppc64le.deb">ppc64le</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.raspberrypi.stretch_armhf.deb">raspberrypi</a>
            </td>
        </tr>
        <tr>
            <td rowspan="3" valign="top"><a href="https://releases.ubuntu.com">Ubuntu</a></td>
            <td>20.04 (<code>focal</code>)</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.focal_amd64.deb">amd64</a>
             </td><td>&nbsp;</td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.focal_arm64.deb">arm64</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.focal_armhf.deb">armhf</a>
             </td><td>&nbsp;</td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.focal_ppc64le.deb">ppc64le</a>
            </td>
            <td>&nbsp;</td>
        </tr>
        <tr>
            <td>18.04 (<code>bionic</code>)</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.bionic_amd64.deb">amd64</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.bionic_i386.deb">i386</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.bionic_arm64.deb">arm64</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.bionic_armhf.deb">armhf</a>
             </td><td>&nbsp;</td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.bionic_ppc64le.deb">ppc64le</a>
            </td>
            <td>&nbsp;</td>
        </tr>
        <tr>
            <td>16.04 (<code>xenial</code>)</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.xenial_amd64.deb">amd64</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.xenial_i386.deb">i386</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.xenial_arm64.deb">arm64</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.xenial_armhf.deb">armhf</a>
             </td><td>&nbsp;</td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox_0.12.6-0.20200605.30.rc.faa06fa.xenial_ppc64le.deb">ppc64le</a>
            </td>
            <td>&nbsp;</td>
        </tr>
        <tr>
            <td rowspan="3" valign="top"><a href="https://wiki.centos.org/Download">CentOS</a></td>
            <td>8</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.centos8.x86_64.rpm">x86_64</a>
             </td><td>&nbsp;</td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.centos8.aarch64.rpm">aarch64</a>
             </td><td colspan="2">&nbsp;</td><td>
                <!--<a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.centos8.ppc64le.rpm">ppc64le</a>-->
            </td>
            <td>&nbsp;</td>
        </tr>
        <tr>
            <td>7</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.centos7.x86_64.rpm">x86_64</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.centos7.i686.rpm">i686</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.centos7.aarch64.rpm">aarch64</a>
             </td><td colspan="2">&nbsp;</td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.centos7.ppc64le.rpm">ppc64le</a>
            </td>
            <td>&nbsp;</td>
        </tr>
        <tr>
            <td>6</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.centos6.x86_64.rpm">x86_64</a>
             </td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.centos6.i686.rpm">i686</a>
            </td>
            <td colspan="5">&nbsp;</td>
        </tr>
        <tr>
            <td><a href="https://aws.amazon.com/amazon-linux-2/">Amazon Linux</a></td>
            <td>2</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.amazonlinux2.x86_64.rpm">x86_64</a>
             </td><td>&nbsp;</td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.amazonlinux2.aarch64.rpm">aarch64</a>
            </td>
            <td colspan="4">&nbsp;</td>
        </tr>
        <tr>
            <td><a href="https://software.opensuse.org/distributions/leap">openSUSE Leap</a></td>
            <td>15</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.opensuse.leap15.x86_64.rpm">x86_64</a>
             </td><td>&nbsp;</td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.opensuse.leap15.aarch64.rpm">aarch64</a>
             </td><td colspan="2">&nbsp;</td><td>
                <a href="https://github.com/wkhtmltopdf/packaging/releases/download/0.12.6-rc/wkhtmltox-0.12.6-0.20200605.30.rc.faa06fa.opensuse.leap15.ppc64le.rpm">ppc64le</a>
            </td>
            <td>&nbsp;</td>
        </tr>
    </tbody>
</table>

All of the above packages were [produced automatically via Azure Pipelines](https://github.com/wkhtmltopdf/packaging) and were built on the latest OS/distribution patch release at the time the packages were built.

## Stable

The current stable series is **0.12.5**, which was released on June 11, 2018 -- see changes [since 0.12.4](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/0.12.5/).

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
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox-0.12.5-1.msvc2015-win32.exe">win32</a>
                /
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox-0.12.5-1.msvc2015-win64.exe">win64</a>
            </td>
            <td>Installer for Windows Vista or later</td>
        </tr>
        <tr>
            <td><a href="http://mxe.cc">MXE</a> (MinGW-w64)</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox-0.12.5-1.mxe-cross-win32.7z">win32</a>
                /
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox-0.12.5-1.mxe-cross-win64.7z">win64</a>
            </td>
            <td>7z archive for Windows XP/2003 or later</td>
        </tr>
        <tr>
            <td rowspan="10">Linux</td>
            <td>Debian 10 (buster)</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.buster_amd64.deb">amd64</a>
                /
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.buster_i386.deb">i386</a>
            </td>
            <td>Package (.deb) built on Debian 10.1</td>
        </tr>
        <tr>
            <td>Debian 9 (stretch)</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.stretch_amd64.deb">amd64</a>
                /
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.stretch_i386.deb">i386</a>
                /
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.raspbian.stretch_armhf.deb">raspbian</a>
            </td>
            <td>Package (.deb) built on Debian 9.4</td>
        </tr>
        <tr>
            <td>Debian 8 (jessie)</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.jessie_amd64.deb">amd64</a>
                /
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.jessie_i386.deb">i386</a>
            </td>
            <td>Package (.deb) built on Debian 8.10</td>
        </tr>
        <tr>
            <td>Ubuntu 20.04 (focal)</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.focal_amd64.deb">amd64</a>
            </td>
            <td>Package (.deb) built on Ubuntu 20.04</td>
        </tr>
        <tr>
            <td>Ubuntu 18.04 (bionic)</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.bionic_amd64.deb">amd64</a>
                /
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.bionic_i386.deb">i386</a>
            </td>
            <td>Package (.deb) built on Ubuntu 18.04</td>
        </tr>
        <tr>
            <td>Ubuntu 16.04 (xenial)</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.xenial_amd64.deb">amd64</a>
                /
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.xenial_i386.deb">i386</a>
            </td>
            <td>Package (.deb) built on Ubuntu 16.04.4</td>
        </tr>
        <tr>
            <td>Ubuntu 14.04 (trusty)</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.trusty_amd64.deb">amd64</a>
                /
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox_0.12.5-1.trusty_i386.deb">i386</a>
            </td>
            <td>Package (.deb) built on Ubuntu 14.04.5</td>
        </tr>
        <tr>
            <td>CentOS 8</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox-0.12.5-1.centos8.x86_64.rpm">x86_64</a>
            </td>
            <td>Package (.rpm) built on CentOS 8.0.1905</td>
        </tr>
        <tr>
            <td>CentOS 7</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox-0.12.5-1.centos7.x86_64.rpm">x86_64</a>
                /
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox-0.12.5-1.centos7.i686.rpm">i686</a>
            </td>
            <td>Package (.rpm) built on CentOS 7.4.1804</td>
        </tr>
        <tr>
            <td>CentOS 6</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox-0.12.5-1.centos6.x86_64.rpm">x86_64</a>
                /
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox-0.12.5-1.centos6.i686.rpm">i686</a>
            </td>
            <td>Package (.rpm) built on CentOS 6.9</td>
        </tr>
        <tr>
            <td rowspan="2">macOS</td>
            <td>Cocoa</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox-0.12.5-1.macos-cocoa.pkg">64-bit</a>
            </td>
            <td>Installer (.pkg) for OS X 10.7 or later</td>
        </tr>
        <tr>
            <td>Carbon</td>
            <td>
                <a href="https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/wkhtmltox-0.12.5-1.macos-carbon.pkg">32-bit</a>
            </td>
            <td>Installer (.pkg) for OS X 10.7 or later</td>
        </tr>
    </tbody>
</table>

Please make sure to verify the [checksums](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/SHA256SUMS) which [are signed](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/download/0.12.5/SHA256SUMS.asc) using the key [`86DA 3091 E255 54F1 56F2 14F6 A86E F3CB 8C64 CF2A`](https://pgp.mit.edu/pks/lookup?search=ashish+kulkarni+open+source&op=vindex&fingerprint=on), especially if you are using it from an automated script/process.

## Archive

Please note that bug reports **will not be accepted** against the following, which are considered obsolete. It is recommended to use the latest stable release instead, and report an issue if there is a regression from a previous release.

Date       | Release
----       | -------
2019-04-30 | [0.12.1.4](https://github.com/wkhtmltopdf/packaging/releases/0.12.1.4-2/) (linux-only)
2016-11-22 | [0.12.4](https://github.com/wkhtmltopdf/wkhtmltopdf/releases/0.12.4/)
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

#### Why are there no "generic" Linux builds (_which were provided earlier_)?

Although the builds are static, it is very important to understand what it means in the context of Qt -- on which wkhtmltopdf is built. A static build means that _only_ Qt is linked in this manner -- the remaining system packages still need to be installed. Over a period of time, major areas of divergence between distributions were found by trial and error:

* **different library versions**: not every distribution provides the same versions. This was especially the case for `libpng` and `libjpeg`, with a lot of distributions choosing between the 1.2, 1.5 and 1.6 series for the former and multiple versions of `libjpeg` and/or its fork `libjpeg-turbo`. While this could be addressed easily by linking them statically (and was actually done so for previous releases) -- it broke down when it came to the next point.
* **different OpenSSL versions**: due to OpenSSL having a bad track record then (_it's better now_), distributions started aggressively upgrading their OpenSSL version and disabling unused parts of the library. This led to a situation where there was effectively zero backward compatibility and things started breaking randomly -- see [#3001](https://github.com/wkhtmltopdf/wkhtmltopdf/issues/3001) for a very long read of the problems faced. This was the direct motivation to create a [separate packaging repository](https://github.com/wkhtmltopdf/packaging).
* **incompatible libc**: not every distribution has the same glibc version. If you compile with a later version, it won't work on a distribution which uses an older version. This was worked around earlier by using CentOS 6 (which had an old enough glibc version). But due to the rise of Docker, the `alpine` image became very popular. This doesn't use glibc at all, but the musl libc. So the generic binaries never really worked on Alpine.

While Python has also tried to do this using [manylinux](https://github.com/pypa/manylinux) -- it doesn't always work well (e.g. `alpine` is _not_ recommended with binary wheels if you google for it), and requires you to statically link everything. This may work for them, but wkhtmltopdf also depends on the runtime configuration on actual fonts installed (i.e. `fontconfig` and `freetype2`). It's not possible to abstract everything out and test/fix everything for every OS/distribution with the limited resources this project has -- it makes more sense to make distribution-specific versions which are almost guaranteed to work, as they use the specific versions that the distribution has packaged.

#### I don't see an appropriate download for my platform!

If the distribution you are using is listed:
  * but not the specific patch release -- try it, as it's very likely to work regardless.
  * the major release isn't listed -- we only support LTS versions, so try a LTS version older than your release.
  * cannot install package -- you can always extract it (google for `extract from <format>`), but you'll need to have the dependencies installed.

Head over to the [packaging repository](https://github.com/wkhtmltopdf/packaging) and start a discussion if your platform isn't listed.

#### How do I use it with [FaaS](https://en.wikipedia.org/wiki/Function_as_a_service) setups?

You'll need to extract the distribution-specific package, bundle it with necessary libraries, configuration and/or fonts and then upload it. See [this ticket](https://github.com/wkhtmltopdf/wkhtmltopdf/issues/4523) for AWS Lambda and [this StackOverflow question](https://stackoverflow.com/q/46639273) for Google Cloud Functions. PRs are welcome to expand this section, if you have more information about this -- this is not a setup that the maintainer uses 😄

#### Symantec reports a virus `WS.Reputation.1` for the Windows builds

This is a false positive reported because Symantec has not seen this file before -- see [this clarification](http://community.norton.com/forums/clarification-wsreputation1-detection) for details.
