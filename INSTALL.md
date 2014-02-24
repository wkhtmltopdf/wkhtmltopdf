Clone this repository by running the following command:

    git clone --recursive https://github.com/wkhtmltopdf/wkhtmltopdf.git

If you are on Windows, make sure that you are cloning in a location without spaces/special characters in the name.

Prerequisites: Debian/Ubuntu
----------------------------

All linux binaries are built in a chroot environment (Debian 6 "Squeeze"), while the windows binaries can be cross-compiled. The only packages required in the host OS are:

    debootstrap git-core xz-utils build-essential mingw-w64 nsis

Prerequisites: Windows
----------------------

* Install MSVC 2010: see http://qt-project.org/wiki/Category:Tools::msvc
* Do "Windows Update" to ensure that VC/SDK security patches are up-to-date
* Install latest ActivePerl from http://www.activestate.com/activeperl/downloads
* Install latest Python 2.7 from http://www.python.org/downloads/windows/
* Install NSIS 2.46 from http://nsis.sourceforge.net/Download
* Make sure that you can run "git". If not, add it to the PATH or reinstall
  with option "Run Git from the Windows Command Prompt".
* Start "Windows SDK 7.1 Command Prompt" for "Windows Server 2008 Release".
  A 32-bit environment is available by running (all on one line)

      %WINDIR%\System32\cmd.exe /E:ON /V:ON /T:0E /K "%ProgramFiles%\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd" /2008 /Release /x86

  or alternatively, a 64-bit environment can be started by running:

      %WINDIR%\System32\cmd.exe /E:ON /V:ON /T:0E /K "%ProgramFiles%\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd" /2008 /Release /x64

Building
--------

Switch to the checked-out folder and run the command ```python scripts/build```. This will present all the options which you can build. Select the appropriate target and all the requisite output will be generated in the ```static-build``` folder. For the linux static builds, it will download download and create a chroot environment for Debian 6 ```squeeze``` (approx 250MB downloaded per architecture)