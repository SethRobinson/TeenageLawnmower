:This builds the game and packages it into a nice installer in the dist directory.  Not really needed to use this, but some command line utils it uses are part of the Proton SDK repo on github.

REM setup compiling environment
@echo off
call vnet.bat
@echo on

set C_TARGET_EXE=game\mow.exe
SET C_FILENAME=TeenageLawnmowerFullInstaller.exe
if exist dist\%C_FILENAME% del dist\%C_FILENAME%
REM If we couldn't erase it, it may have been in use.  Signal error in that case.
if exist dist\%C_FILENAME% beeper.exe /p

rem make sure an old version doesn't exist
if exist %C_TARGET_EXE% del %C_TARGET_EXE%

REM If I needed to set special settings for this build I could set the %cl% environmental var.  But then I'd need to rebuild
REM each time.. It's much faster to just create a special beta project in vc6.
REM set cl=/D_BETA_

devenv source\source.sln /Project "SW" /build "Release" 
REM devenv source\source.sln /build Release

REM if we'd used the cl var, we'd undefine it here
REM set cl=

REM Make sure the file compiled ok
if not exist %C_TARGET_EXE% beeper.exe /p

REM Make sure cheat modes aren't activated, thus ruining the game

ctoenv.exe game\script\cheat.c "g_i_test_mode = 0" C_TEMP /r
if errorlevel 1 beeper.exe /p
ctoenv.exe game\script\cheat.c "g_i_skip_to_play = 0" C_TEMP /r
if errorlevel 1 beeper.exe /p
ctoenv.exe game\script\cheat.c "g_i_force_level_script = 0" C_TEMP /r
if errorlevel 1 beeper.exe /p
ctoenv.exe game\script\cheat.c "g_st_force_weather = \"\";" C_TEMP /r
if errorlevel 1 beeper.exe /p
ctoenv.exe game\script\cheat.c "g_i_force_money = 0" C_TEMP /r
if errorlevel 1 beeper.exe /p



REM I stopped using UPX after hearing it won't work right with xp SP2
REM upx %C_TARGET_EXE%
//upx %C_TARGET_EXE%

REM compile scripts into one fresh resource file
cd game
call prepare.bat
cd ..


REM get version information from the source code
echo Grabbing version # information from source code.

ctoenv.exe source\CGlobals.cpp "f_version = " C_VERSION /r
if errorlevel 1 beeper.exe /p
call setenv.bat
del setenv.bat

ctoenv.exe source\CGlobals.cpp "sprintf(st_version, \"" C_TEXT_VERSION_TEMP
if errorlevel 1 beeper.exe /p
call setenv.bat
del setenv.bat

SET C_TEXT_VERSION= %C_TEXT_VERSION_TEMP%
REM done with temp var, kill it
SET C_TEXT_VERSION_TEMP=

echo Building installer: %C_FILENAME%
cd installer
..\..\util\NSIS\makensis.exe tlm.nsi
Rem Check for error during installer packing
if not exist %C_FILENAME% beeper.exe /p
cd ..

REM Copy it to our final distribution directory
move installer\%C_FILENAME% dist

REM Leave the environmental vars active in case another batch file is run after this that uses them, for instance, to ftp the files
