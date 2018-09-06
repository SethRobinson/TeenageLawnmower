;NSIS Modern User Interface version 1.70
;Header Bitmap Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;General
  SetCompressor lzma
  !define _TITLE_ "Teenage Lawnmower"
  !define _VERSION_ "$%C_TEXT_VERSION%"
  !define _COMPILE_DATE_ "${__DATE__}"

  ;Name and file
  Name "${_TITLE_}"
 ; Icon "tlm.ico"
!define MUI_ICON "tlm.ico" 
;!define MUI_UNICON "tlm.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\classic-uninstall.ico"

  OutFile "$%C_FILENAME%"


!define MUI_WELCOMEFINISHPAGE_BITMAP "welcome_side.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "welcome_side.bmp"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\TLawnmower"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKLM "Software\RTSOFT\TLM" "path"

BrandingText " "
;InitPluginsDir
;  File /oname=$PLUGINSDIR\splash.bmp "path\to\your\bitmap.bmp"
;  advsplash::show 1000 600 400 -1 $PLUGINSDIR\splash
;  Pop $0

;--------------------------------
;Interface Configuration

  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_BITMAP "header.bmp"
  !define MUI_HEADERIMAGE_UNBITMAP "header.bmp"
  !define MUI_ABORTWARNING
 
;--------------------------------
;Pages

  
  !define MUI_WELCOMEPAGE_TEXT "Get ready to smoke some weeds!\r\n\r\n\
This wizard will guide you through the installation of ${_TITLE_}. (${_VERSION_} released on ${_COMPILE_DATE_})\r\n\r\n\
Click Next to continue."
  !define MUI_WELCOMEPAGE_TITLE "${_TITLE_} ${_VERSION_} Installer (Full Version)"
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "readme.txt"
  
  
  ;Customize component texts
  
  !define MUI_COMPONENTSPAGE_TEXT_COMPLIST "Select extra components to install."
  ;!define MUI_COMPONENTSPAGE_TEXT_TOP "Choose which extra features of Teenage Lawnmower you want to install."
  !insertmacro MUI_PAGE_COMPONENTS
  
  
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
 
!define MUI_FINISHPAGE_LINK "Click here to visit the RTsoft website"
!define MUI_FINISHPAGE_LINK_LOCATION "http://www.rtsoft.com/"
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_FUNCTION "LaunchLink"
 
 !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
 
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

;Remove the - part in from of Main Game if you don't want it to be hidden...
Section "-Main Game" SecMain
SectionIn RO
  SetOutPath "$INSTDIR"
  File "..\game\mow.exe"
  File "..\game\fmod.dll"
  
  SetOutPath "$INSTDIR\data"
  File "..\game\data\*.*"

  SetOutPath "$INSTDIR\script"
  File /r "..\game\script\*.*"

	SetOutPath "$INSTDIR\scriptg"
  File /r "..\game\scriptg\*.*"


  SetOutPath "$INSTDIR\docs"
  File "..\game\docs\help.htm"
  File "..\game\docs\help.ico"
  File "..\game\docs\rtsoft.URL"
  File "..\game\docs\whatsnew.htm"
  File /r "..\game\docs\images"
 
  SetOutPath "$INSTDIR\save"
  File "..\game\save\dir.txt"

 
  SetOutPath "$INSTDIR"
  File /r "..\game\interface"
  File /r "..\game\sound"
  File /r "..\game\texture"
  File /r "..\game\model"

CreateDirectory "$SMPROGRAMS\${_TITLE_}"
;ok, this will create a Folder in your Startmenue


   
  ;Store installation folder
  WriteRegStr HKLM "Software\RTSOFT\TLM" "path" $INSTDIR

  ;write uninstall strings
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${_TITLE_}" "DisplayName" "${_TITLE_} (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${_TITLE_}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  CreateShortCut "$SMPROGRAMS\${_TITLE_}\${_TITLE_}.lnk" "$INSTDIR\mow.EXE" "" "$INSTDIR\mow.exe"
  WriteIniStr "$SMPROGRAMS\${_TITLE_}\Comment on this game.url" "InternetShortcut" "URL" "http://www.rtsoft.com/pages/feedback.htm?game=${_TITLE_}&version=${_VERSION_}"
  WriteIniStr "$SMPROGRAMS\${_TITLE_}\Robinson Technologies Website.url" "InternetShortcut" "URL" "http://www.rtsoft.com"
  CreateShortCut "$SMPROGRAMS\${_TITLE_}\Read whatsnew.htm.lnk" "$INSTDIR\docs\whatsnew.htm" ; use defaults for parameters, icon, etc.
  CreateShortCut "$SMPROGRAMS\${_TITLE_}\Quick Help.lnk" "$INSTDIR\docs\help.htm" ; use defaults for parameters, icon, etc.
  CreateShortCut "$SMPROGRAMS\${_TITLE_}\Uninstall ${_TITLE_}.lnk" "$INSTDIR\Uninstall.exe" ; use defaults for parameters, icon, etc.

 ;just in case they had an old version of TLM installed, let's kill it's menu, too confusing
 Delete "$SMPROGRAMS\Teenage Lawn Mower\*.*"
  RMDir "$SMPROGRAMS\Teenage Lawn Mower"


SectionEnd


;Installer Sections

Section "Desktop Icon" SecDesktopIcon
  SetOutPath "$INSTDIR"
  CreateShortCut "$DESKTOP\${_TITLE_}.lnk" "$INSTDIR\mow.EXE" 

SectionEnd

Function LaunchLink
  ExecShell "" "$INSTDIR\mow.EXE"
FunctionEnd



;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecMain ${LANG_ENGLISH} "The main game files, these are required to play the game."
  LangString DESC_SecDesktopIcon ${LANG_ENGLISH} "This option will throw one of those handy desktop icons on the main menu for easy access to the game."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} $(DESC_SecMain)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDesktopIcon} $(DESC_SecDesktopIcon)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

;default delete

  Delete "$INSTDIR\mow.exe"
  Delete "$INSTDIR\fmod.dll"
  Delete "$INSTDIR\temp"
  Delete "$INSTDIR\Uninstall.exe"
  
  RMDir /r "$INSTDIR\data"
  RMDir /r "$INSTDIR\docs"
  RMDir /r "$INSTDIR\interface"
  RMDir /r "$INSTDIR\sound"
  RMDir /r "$INSTDIR\script"
  RMDir /r "$INSTDIR\scriptg"
  RMDir /r "$INSTDIR\texture"
  RMDir /r "$INSTDIR\model"
  ;just in case a demo was here earlier
  RMDir /r "$INSTDIR\demo_stuff"


  DeleteRegKey HKLM "Software\RTSOFT\TLM\path"

  DeleteRegKey /ifempty HKLM "Software\RTSOFT\TLM"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${_TITLE_}"
  Delete "$SMPROGRAMS\${_TITLE_}\*.*"
  RMDir "$SMPROGRAMS\${_TITLE_}"
  Delete "$DESKTOP\${_TITLE_}.lnk"

;delete user stuff too?

MessageBox MB_YESNO "Would you like to also delete your settings and saved game files?" IDNO skip_it 
RMDir /r "$INSTDIR\Save" 
Delete "$INSTDIR\errors.txt"
Delete "$INSTDIR\log.txt"
Delete "$INSTDIR\defaults.dat"
RMDir "$INSTDIR" 
skip_it:


SectionEnd