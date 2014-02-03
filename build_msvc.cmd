@echo off

set OPENSSL_REPO=https://github.com/openssl/openssl.git
set OPENSSL_BRANCH=OpenSSL_1_0_1-stable
set OPENSSL_TAG=OpenSSL_1_0_1f

set BUILD_DIR=%~dp0static-build
mkdir %BUILD_DIR% 2> nul
cd /d %BUILD_DIR%

if exist %BUILD_DIR%\openssl_dist goto openssl_check
if exist %BUILD_DIR%\openssl      goto openssl_build

echo ================ downloading OpenSSL
git clone --branch %OPENSSL_BRANCH% --single-branch %OPENSSL_REPO% openssl
cd /d %BUILD_DIR%\openssl
git checkout %OPENSSL_TAG%

:openssl_build
echo ================ building OpenSSL
cd /d %BUILD_DIR%\openssl
perl Configure VC-WIN32 no-asm --prefix=%BUILD_DIR%\openssl_dist
call ms\do_ms.bat
nmake /f ms\nt.mak install
git clean -fdx

:openssl_check
cd /d %BUILD_DIR%\openssl_dist
if not exist include\openssl\ssl.h goto openssl_error
if not exist lib\libeay32.lib      goto openssl_error
if not exist lib\ssleay32.lib      goto openssl_error
goto openssl_done

:openssl_error
echo OpenSSL was not compiled properly; please remove the directory
echo   %BUILD_DIR%\openssl_dist
echo and try to compile again.
cd /d %~dp0
exit /b 1

:openssl_done

echo ================ building patched QT
cd /d %BUILD_DIR%
mkdir qt 2> nul
cd /d %BUILD_DIR%\qt

IF NOT EXIST bin\qmake.exe goto build_qt
IF NOT EXIST Makefile      goto build_qt
goto build_app

:configure_qt
set QT_CFG=
set QT_CFG=%QT_CFG% -opensource
set QT_CFG=%QT_CFG% -confirm-license
set QT_CFG=%QT_CFG% -release
set QT_CFG=%QT_CFG% -static
set QT_CFG=%QT_CFG% -graphicssystem raster
set QT_CFG=%QT_CFG% -mp
set QT_CFG=%QT_CFG% -no-exceptions
set QT_CFG=%QT_CFG% -no-accessibility
set QT_CFG=%QT_CFG% -no-stl
set QT_CFG=%QT_CFG% -no-sql-ibase
set QT_CFG=%QT_CFG% -no-sql-mysql
set QT_CFG=%QT_CFG% -no-sql-odbc
set QT_CFG=%QT_CFG% -no-sql-psql
set QT_CFG=%QT_CFG% -no-sql-sqlite
set QT_CFG=%QT_CFG% -no-sql-sqlite2
set QT_CFG=%QT_CFG% -no-qt3support
set QT_CFG=%QT_CFG% -no-xmlpatterns
set QT_CFG=%QT_CFG% -no-phonon
set QT_CFG=%QT_CFG% -no-phonon-backend
set QT_CFG=%QT_CFG% -no-qt3support
set QT_CFG=%QT_CFG% -no-opengl
set QT_CFG=%QT_CFG% -no-declarative
set QT_CFG=%QT_CFG% -no-script
set QT_CFG=%QT_CFG% -no-scripttools
set QT_CFG=%QT_CFG% -no-mmx
set QT_CFG=%QT_CFG% -no-3dnow
set QT_CFG=%QT_CFG% -no-sse
set QT_CFG=%QT_CFG% -no-sse2
set QT_CFG=%QT_CFG% -no-multimedia
set QT_CFG=%QT_CFG% -webkit
set QT_CFG=%QT_CFG% -qt-zlib
set QT_CFG=%QT_CFG% -qt-libpng
set QT_CFG=%QT_CFG% -qt-libjpeg
set QT_CFG=%QT_CFG% -no-libmng
set QT_CFG=%QT_CFG% -no-libtiff
set QT_CFG=%QT_CFG% -qt-style-windows
set QT_CFG=%QT_CFG% -qt-style-cleanlooks
set QT_CFG=%QT_CFG% -no-style-windowsxp
set QT_CFG=%QT_CFG% -no-style-windowsvista
set QT_CFG=%QT_CFG% -no-style-plastique
set QT_CFG=%QT_CFG% -no-style-motif
set QT_CFG=%QT_CFG% -no-style-cde
set QT_CFG=%QT_CFG% -nomake demos
set QT_CFG=%QT_CFG% -nomake docs
set QT_CFG=%QT_CFG% -nomake examples
set QT_CFG=%QT_CFG% -nomake tools
set QT_CFG=%QT_CFG% -nomake tests
set QT_CFG=%QT_CFG% -nomake translations

set QT_CFG=%QT_CFG% -openssl-linked
set QT_CFG=%QT_CFG% -I %BUILD_DIR%\openssl_dist\include
set QT_CFG=%QT_CFG% -L %BUILD_DIR%\openssl_dist\lib
set QT_CFG=%QT_CFG% -l libeay32
set QT_CFG=%QT_CFG% -l ssleay32
set QT_CFG=%QT_CFG% OPENSSL_LIBS="-L%BUILD_DIR%\openssl_dist\lib -lssleay32 -llibeay32 -lUser32 -lAdvapi32 -lGdi32 -lCrypt32"

%BUILD_DIR%\..\qt\configure %QT_CFG%
nmake

:build_app
echo ================ building wkhtmltopdf
cd /d %BUILD_DIR%
mkdir app 2> nul
cd app
%BUILD_DIR%\qt\bin\qmake %BUILD_DIR%\..\wkhtmltopdf.pro
nmake

cd /d %BUILD_DIR%
rd /s /q dist 2>nul
mkdir dist
mkdir dist\bin
mkdir dist\lib
mkdir dist\include
mkdir dist\include\wkhtmltox
copy  app\bin\wkhtmltopdf.exe   dist\bin
copy  app\bin\wkhtmltoimage.exe dist\bin
copy  app\bin\wkhtmltox.dll     dist\bin
copy  app\bin\wkhtmltox.lib     dist\lib
copy  %~dp0include\wkhtmltox\dllbegin.inc dist\include\wkhtmltox
copy  %~dp0include\wkhtmltox\dllend.inc   dist\include\wkhtmltox
copy  %~dp0include\wkhtmltox\pdf.h        dist\include\wkhtmltox
copy  %~dp0include\wkhtmltox\image.h      dist\include\wkhtmltox

set MAJOR_VERSION=
set MINOR_VERSION=
set PATCH_VERSION=
set WK_VERSION=
set WK_HASH=

cd /d %~dp0
FOR /F %%v IN (version.pri) DO SET %%v
set WK_VERSION=%MAJOR_VERSION%.%MINOR_VERSION%.%PATCH_VERSION%
FOR /F "delims=" %%h IN ('git rev-parse --short HEAD') DO set WK_HASH=%%h

if exist "%ProgramFiles%\NSIS\makensis.exe" goto build_installer
if exist "%ProgramFiles%\7-zip\7z.exe"      goto build_archive

echo Unable to detect either NSIS or 7-Zip; please examine folder
echo   %BUILD_DIR%\dist
echo to see the output.
cd /d %~dp0
exit /b 0

:build_installer
echo ================ building installer
cd /d %BUILD_DIR%
"%ProgramFiles%\NSIS\makensis.exe" /DVERSION=%WK_VERSION% /DWK_HASH=%WK_HASH% ..\wkhtmltox.nsi
cd /d %~dp0
exit /b 0

:build_archive
echo ================ building archive
cd /d %BUILD_DIR%\dist
"%ProgramFiles%\7-zip\7z.exe" a ..\wkhtmltox-%WK_VERSION%_%WK_HASH%.7z -mx9 -r *
cd /d %~dp0
exit /b 0
