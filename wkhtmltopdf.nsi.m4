!include "MUI2.nsh"
!include "EnvVarUpdate.nsh"

Name "wkhtmltox WKVERSION"
OutFile "wkhtmltox-WKVERSION-installer.exe"

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
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
  
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Wkhtmltopdf" pdf
  SetOutPath "$INSTDIR"
  file wkhtmltopdf.exe
SectionEnd

Section "Wkhtmltoimage" image
  SetOutPath "$INSTDIR"
  file wkhtmltoimage.exe
SectionEnd

Section /o "Modify PATH" mpath
  ${EnvVarUpdate} $0 "PATH" "A" "HKLM" "$INSTDIR" 
SectionEnd

LangString DESC_pdf   ${LANG_ENGLISH} "Install wkhtmltopdf"
LangString DESC_image ${LANG_ENGLISH} "Install wkhtmltoimage"
LangString DESC_mpath ${LANG_ENGLISH} "Update PATH, WILL BREAK PATH IN VISTA AN WIN7"

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${pdf} $(DESC_pdf)
    !insertmacro MUI_DESCRIPTION_TEXT ${image} $(DESC_image)
    !insertmacro MUI_DESCRIPTION_TEXT ${mpath} $(DESC_mpath)
 !insertmacro MUI_FUNCTION_DESCRIPTION_END

Section
  SetOutPath "$INSTDIR"
  file libgcc_s_dw2-1.dll
  file ssleay32.dll
  file libeay32.dll
  file mingwm10.dll
  WriteRegStr HKCU "Software\wkhtmltopdf" "" $INSTDIR
  WriteUninstaller "$INSTDIR\uninstall.exe"

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\wkhtmltopdf" \
                   "DisplayName" "wkhtmltopdf"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\wkhtmltopdf" \
                   "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
SectionEnd


;--------------------------------
;Uninstaller Section

Section "Uninstall"
  Delete "$INSTDIR\wkhtmltoimage.exe"
  Delete "$INSTDIR\wkhtmltopdf.exe"
  Delete "$INSTDIR\libgcc_s_dw2-1.dll"
  Delete "$INSTDIR\ssleay32.dll"
  Delete "$INSTDIR\libeay32.dll"
  Delete "$INSTDIR\mingwm10.dll"
;  ${un.EnvVarUpdate} $0 "PATH" "R" "HKLM" "$INSTDIR"
  Delete "$INSTDIR\uninstall.exe"
  RMDir "$INSTDIR"
  DeleteRegKey /ifempty HKCU "Software\wkhtmltopdf"
SectionEnd
