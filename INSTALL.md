Clone this repository by running the following command:

    git clone --recursive https://github.com/wkhtmltopdf/wkhtmltopdf.git

If you are on Windows, make sure that you are cloning in a location without spaces/special characters in the name.

Prerequisites: Linux
--------------------

Building is currently supported only on the current 64-bit Debian stable release (```wheezy```), but it should work on other Debian/Ubuntu releases as well. Please run ```scripts/setup-linux.sh``` as root, which will create all the chroot environments and configure them for building properly. A total of 4 chroot environments is created (approx 1GB will need to be downloaded):
 * Debian-wheezy i386  (should work on Ubuntu 12.04 LTS and later)
 * Debian-wheezy amd64 (should work on Ubuntu 12.04 LTS and later)
 * CentOS-5 i386  (should work on systems where older GLIBC is installed)
 * CentOS-5 amd64 (should work on systems where older GLIBC is installed)

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

Switch to the checked-out folder and run the command ```scripts/build.py``` (or ```scripts\build.py``` if you are on Windows). This will present all the options which you can build. Select the appropriate target and all the requisite output will be generated in the ```static-build``` folder.