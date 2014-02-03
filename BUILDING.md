Clone this repository by running the following command:
```git clone --recursive https://github.com/wkhtmltopdf/wkhtmltopdf.git wkhtmltopdf```

If you are on Windows, make sure that you are cloning in a location without spaces/special characters in the name.

Prerequisites: Debian/Ubuntu
----------------------------

All binaries are built in a chroot environment (Debian 6 "Squeeze"), so the only packages required in the host OS are: ```debootstrap git-core xz-utils```

Prerequisites: Windows
----------------------

* Follow instructions at http://qt-project.org/wiki/Category:Tools::msvc for installation of MSVC 2010
* Update all related patches from Windows Update to ensure that latest VC/SDK is installed
* Install ActivePerl from http://www.activestate.com/activeperl/downloads
* Install either 7-Zip (to make the archive) or NSIS (to make the installer)
* Make sure that you can run "git". If not, add it to the PATH or reinstall with option "Run Git from the Windows Command Prompt".

Building: Windows
-----------------

* Start "Windows SDK 7.1 Command Prompt" while targetting "Windows Server 2008 x86 Release". This can also be done by running
    ```%WINDIR%\System32\cmd.exe /E:ON /V:ON /T:0E /K "%ProgramFiles%\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd" /2008 /Release /x86```
* Switch to the checked-out folder and run the command ```build_msvc```. This will take quite a bit of time to complete, so be patient.

Building: Debian/Ubuntu
-----------------------

To build the binaries, run the command ```./build_linux.sh [linux-i386|linux-amd64]```. This will download and create a chroot environment (approx 250MB will be downloaded per architecture) and create the compressed tarball in the ```static-build``` folder.