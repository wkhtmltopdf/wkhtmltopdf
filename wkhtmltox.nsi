!include "MUI2.nsh"
!include "x64.nsh"

Name             "wkhtmltox ${VERSION} (${WK_HASH})"
OutFile          "static-build\wkhtmltox-${ARCH}_${VERSION}-${WK_HASH}.exe"
!if ${ARCH} == "win64"
InstallDir       "$PROGRAMFILES64\wkhtmltopdf"
!else
InstallDir       "$PROGRAMFILES\wkhtmltopdf"
!endif
InstallDirRegKey HKLM "Software\wkhtmltopdf" "InstallPath"
VIProductVersion "${VERSION}.0"
VIAddVersionKey  "ProductName"     "wkhtmltox"
VIAddVersionKey  "FileDescription" "wkhtmltox ${VERSION} (${WK_HASH})"
VIAddVersionKey  "LegalCopyright"  "wkhtmltopdf authors"
VIAddVersionKey  "FileVersion"     "${VERSION}.0"

CRCCheck             force
SetCompressor /SOLID lzma

RequestExecutionLevel admin

!insertmacro MUI_PAGE_LICENSE "COPYING"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"

Section "Install"
  RMDir /r "$INSTDIR"

  SetOutPath "$INSTDIR\bin"
  File static-build\dist\bin\wkhtmltoimage.exe
  File static-build\dist\bin\wkhtmltopdf.exe
  File static-build\dist\bin\wkhtmltox.dll

  SetOutPath "$INSTDIR\lib"
  File static-build\dist\lib\wkhtmltox.lib

  SetOutPath "$INSTDIR\include\wkhtmltox"
  File static-build\dist\include\wkhtmltox\dllbegin.inc
  File static-build\dist\include\wkhtmltox\dllend.inc
  File static-build\dist\include\wkhtmltox\pdf.h
  File static-build\dist\include\wkhtmltox\image.h

  WriteRegStr HKLM "Software\wkhtmltopdf" "InstallPath" "$INSTDIR"
  WriteRegStr HKLM "Software\wkhtmltopdf" "PdfPath"     "$INSTDIR\bin\wkhtmltopdf.exe"
  WriteRegStr HKLM "Software\wkhtmltopdf" "ImagePath"   "$INSTDIR\bin\wkhtmltoimage.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\wkhtmltopdf" \
                   "DisplayName" "wkhtmltox ${VERSION} (${WK_HASH})"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\wkhtmltopdf" \
                   "UninstallString" "$\"$INSTDIR\uninstall.exe$\""

  WriteUninstaller "$INSTDIR\uninstall.exe"
SectionEnd

Section "Uninstall"
  Delete "$INSTDIR\bin\wkhtmltoimage.exe"
  Delete "$INSTDIR\bin\wkhtmltopdf.exe"
  Delete "$INSTDIR\bin\wkhtmltox.dll"
  Delete "$INSTDIR\lib\wkhtmltox.lib"
  Delete "$INSTDIR\include\wkhtmltox\dllbegin.inc"
  Delete "$INSTDIR\include\wkhtmltox\dllend.inc"
  Delete "$INSTDIR\include\wkhtmltox\pdf.h"
  Delete "$INSTDIR\include\wkhtmltox\image.h"
  Delete "$INSTDIR\uninstall.exe"
  RMDir  "$INSTDIR\bin"
  RMDir  "$INSTDIR\include\wkhtmltox"
  RMDir  "$INSTDIR\include"
  RMDir  "$INSTDIR\lib"
  RMDir  "$INSTDIR"
  DeleteRegKey HKLM "Software\wkhtmltopdf"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\wkhtmltopdf"
SectionEnd

Function .onInit
  ${If} ${RunningX64}
    SetRegView 64
  ${EndIf}
FunctionEnd