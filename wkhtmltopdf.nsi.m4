!include "MUI2.nsh"
!include "EnvVarUpdate.nsh"

Name "wkhtmltopdf WKVERSION"
OutFile "wkhtmltopdf-WKVERSION-installer.exe"

RequestExecutionLevel admin

;Default installation folder
InstallDir "$PROGRAMFILES\wkhtmltopdf"
  
;Get installation folder from registry if available
InstallDirRegKey HKCU "Software\wkhtmltopdf" ""

;--------------------------------
;Interface Settings

!define MUI_ABORTWARNING

;--------------------------------
;Pages
	
!insertmacro MUI_PAGE_LICENSE "../COPYING"
; !insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
  
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section
  SetOutPath "$INSTDIR"
  file wkhtmltopdf.exe
  file libgcc_s_dw2-1.dll
  file ssleay32.dll
  file libeay32.dll
  WriteRegStr HKCU "Software\wkhtmltopdf" "" $INSTDIR
  WriteUninstaller "$INSTDIR\uninstall.exe"

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\wkhtmltopdf" \
                   "DisplayName" "wkhtmltopdf"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\wkhtmltopdf" \
                   "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
;  ${EnvVarUpdate} $0 "PATH" "A" "HKLM" "$INSTDIR" 
SectionEnd


;--------------------------------
;Uninstaller Section

Section "Uninstall"
  Delete "$INSTDIR\wkhtmltopdf.exe"
  Delete "$INSTDIR\libgcc_s_dw2-1.dll"
  Delete "$INSTDIR\ssleay32.dll"
  Delete "$INSTDIR\libeay32.dll"
;  ${un.EnvVarUpdate} $0 "PATH" "R" "HKLM" "$INSTDIR"
  Delete "$INSTDIR\uninstall.exe"
  RMDir "$INSTDIR"
  DeleteRegKey /ifempty HKCU "Software\wkhtmltopdf"
SectionEnd