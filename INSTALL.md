Clone this repository by running the following command:

    git clone --recursive https://github.com/wkhtmltopdf/wkhtmltopdf.git

If you are on Windows, make sure that you are cloning in a location without
spaces/special characters in the name. In case you already have a cloned
repository, update to the latest version by running the following commands:

    git pull
    git submodule update

Please ensure that you have enough disk space in the location you have cloned
the source code, as it will require approximately 1.2GiB for both the `qt` and
`wkhtmltopdf` repositories. Each target that is built will require an
additional 2.5GiB for compiling the source code and producing the final
installer (for Windows) or packages (for other OSes), which will be generated
in the `static-build/` folder.

You can create a debug build by passing the `-debug` flag to the build script.
It is recommended to always pass the `-clean` flag, which removes the
existing build folder and performs a clean build. Note that passing `-debug`
may increase the disk space required for the build and final binaries for
storing the additional debug information.

Linux
-----

Please ensure that the cloned repository is in the user's home directory
e.g. `~/wkhtmltopdf`. If you clone it in a different directory, it may
fail with `E: Failed to change to directory /your/dir: No such file or directory`.
Please note that [encrypted home directories](https://bugs.launchpad.net/ubuntu/+source/schroot/+bug/791908)
and [non standard home directories](https://github.com/wkhtmltopdf/wkhtmltopdf/issues/1804)
(i.e. not located in `/home`) are not supported -- you are advised to
use a VM instead to build wkhtmltopdf.

Building is supported only on latest stable Debian/Ubuntu 64-bit, and
the binaries are produced in a self-contained chroot environment for the
target distribution -- you will need to first setup the build environment
and then only you can perform the build for a 32-bit or 64-bit binary.
The following targets are currently supported:

Target         | Setup of Build Environment                    | Building 32-bit binaries                 |  Building 64-bit binaries
------         | --------------------------                    | ------------------------                 |  ------------------------
Generic        | `sudo scripts/build.py setup-schroot-generic` | `scripts/build.py linux-generic-i386`    | `scripts/build.py linux-generic-amd64`
MinGW-w64      | `sudo scripts/build.py setup-mingw-w64`       | `scripts/build.py mingw-w64-cross-win32` | `scripts/build.py mingw-w64-cross-win64`

The MinGW-w64 toolchain can cross-compile 32/64-bit Windows binaries from
Linux -- it is useful for targeting Windows XP/Windows 2003, which are not
supported by default when compiling with MSVC 2013. You may require a
working internet connection during the build to download and compile
the dependent libraries (e.g. OpenSSL).

Each target will require approximately 1.5GiB of disk space to hold both
the `i386` and `amd64` chroot environments for that target. By default,
the chroot environments are created under `/var/chroot` -- in case you
want to create them under another location (e.g. due to insufficient disk
space), please run the command `export WKHTMLTOX_CHROOT=/some/other/dir`
**before** the command for setup of the build environment.

While setting up the build environments, please ensure that you are logged
in as a regular user who has `sudo` access. It is possible to run the script
without `sudo` but you will need to have root privileges (e.g. via `su`). In
that scenario, you may get the error `Unable to determine the login for which schroot access is to be given`
-- you will have to set `export SUDO_USER=<username>` and try to run it again.
Other than the setup of build environment, **do not run any other command
with `root` privileges!** The compilation process can be run as a normal
user and running it as `root` may lead to errors or complete loss of data.

After the build environment is setup, you can run the command mentioned above
to build either the 32-bit or 64-bit binaries, which should generate a
native package (either DEB or RPM, depending on the distribution) in the
`static-build/` folder.

Windows
-------

* Install Visual Studio [2013 (update 5)](https://go.microsoft.com/fwlink/?LinkId=532496)
  or Visual Studio [2015 (update 3)](https://go.microsoft.com/fwlink/?LinkId=615448).
* Install the latest [ActivePerl](http://www.activestate.com/activeperl/downloads) release
* Install the latest [Python 2.7](http://www.python.org/downloads/windows/) release
* Install [NSIS 2.46](http://nsis.sourceforge.net/Download)
* Make sure that you can run "git". If not, add it to the PATH or reinstall
  with option "Run Git from the Windows Command Prompt".

Target          | Building 32-bit binaries               |  Building 64-bit binaries
------          | ------------------------               |  ------------------------
MSVC 2013       | `scripts\build.py msvc2013-win32`      | `scripts/build.py msvc2013-win64`
MSVC 2015       | `scripts\build.py msvc2015-win32`      | `scripts/build.py msvc2015-win64`

During the build, a working internet connection is required to download and
compile the dependent libraries (e.g. OpenSSL). The output installers should
be generated in the `static-build` folder.

Please note that if you want to target Windows XP/Windows 2003 (i.e. NT 5.x)
you should use the MinGW-w64 builds cross-compiled from Linux as the MSVC builds
target Windows Vista or later (i.e. NT 6.x) by default.

OS X
----

Building is supported only on OS X 10.7 or newer. You will need to have the
latest Xcode for your OS X version. Additionally, you will need to run the command
`sudo gem install fpm --no-ri --no-rdoc` in the terminal to install
[fpm](https://github.com/jordansissel/fpm), which is used for building the package.

Target          | Build Command
------          | -------------
32-bit Carbon   | `scripts/build.py osx-carbon-i386`
64-bit Cocoa    | `scripts/build.py osx-cocoa-x86-64`

During the build, a working internet connection is required to download and
compile the dependent libraries (e.g. libpng and libjpeg). The output package
should be generated in the `static-build` folder.

Others
------

In case you are running on a non-Debian/Ubuntu Linux distribution or want to
target a Linux distribution not listed above or another Unix-like OS, you
will need to use the `posix-local` target. It assumes that you already have
all the build dependencies installed beforehand and will generate a tarball
with the specification of `local-MACHINENAME` in the `static-build/` folder.

If you are able to get such a build running, contacting the developers via
the mailing list or submitting a patch with the build instructions would be
appreciated.
