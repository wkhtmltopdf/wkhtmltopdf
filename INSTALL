Clone this repository by running the following command:

    git clone --recursive https://github.com/wkhtmltopdf/wkhtmltopdf.git

If you are on Windows, make sure that you are cloning in a location
without spaces/special characters in the name.

Prerequisites: Debian/Ubuntu
----------------------------

All binaries are built in a chroot environment (Debian 6 "Squeeze"), so
the only packages required in the host OS are:

    debootstrap git-core xz-utils

Prerequisites: Windows
----------------------

* Install MSVC 2010: see http://qt-project.org/wiki/Category:Tools::msvc
* Do "Windows Update" to ensure that VC/SDK security patches are up-to-date
* Install ActivePerl from http://www.activestate.com/activeperl/downloads
* Install either NSIS (for the installer) or 7-Zip (for the archive)
* Make sure that you can run "git". If not, add it to the PATH or reinstall
  with option "Run Git from the Windows Command Prompt".

Building: Windows
-----------------

* Start "Windows SDK 7.1 Command Prompt" for "Windows Server 2008 Release".
  A 32-bit environment is available by running (all on one line)

      %WINDIR%\System32\cmd.exe /E:ON /V:ON /T:0E /K
      "%ProgramFiles%\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd"
      /2008 /Release /x86

  or alternatively, a 64-bit environment can be started by running:

      %WINDIR%\System32\cmd.exe /E:ON /V:ON /T:0E /K
      "%ProgramFiles%\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd"
      /2008 /Release /x64

* Switch to the checked-out folder and run the command ```build_msvc```.
  This will take quite a bit of time to complete, so be patient.
  Installer or archive will be generated in the "static_build" folder.

Building: Debian/Ubuntu
-----------------------

To build the binaries, run the command

    ./build_linux.sh [linux-i386|linux-amd64]

This will download and create a chroot environment (approx 250MB will be
downloaded per architecture) and create the compressed tarball in the
"static-build" folder.
