# Microsoft Developer Studio Project File - Name="SWEd" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SWEd - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SWEd.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SWEd.mak" CFG="SWEd - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SWEd - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SWEd - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SWEd - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "shared" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DSHOW" /D "_WIN32_DCOM" /D DIRECTINPUT_VERSION=0x0800 /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 fmodvc.lib Quartz.lib strmiids.lib dinput8.lib dxerr8.lib d3dx8dt.lib d3d8.lib dxguid.lib Winmm.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\game\SWEd.exe"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "shared" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_DSHOW" /D "_WIN32_DCOM" /D DIRECTINPUT_VERSION=0x0800 /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 fmodvc.lib Quartz.lib strmiids.lib dinput8.lib dxerr8.lib d3dx8dt.lib d3d8.lib dxguid.lib Winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\game\SWEddebug.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SWEd - Win32 Release"
# Name "SWEd - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CDlgEditConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_3d.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\dlg_edit_ent_default.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\dlg_edit_entity.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\dlg_edit_sub.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\dlg_ent_choose.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_ent_choose.h
# End Source File
# Begin Source File

SOURCE=.\dlg_ent_edit_base.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\dlg_item_list.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_landscape.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\dlg_log.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\dlg_new.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\dlg_new_cat.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\dlg_pick_entity.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\dlg_sub_prop.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\dlg_swed.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\dlg_terraform.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\dlg_weather.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg_world_edit.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\dlg_zone_prop.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\SWEd.cpp
# ADD CPP /YX
# End Source File
# End Group
# Begin Group "Things"

# PROP Default_Filter ""
# Begin Group "Entities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\shared\3d\CAnimInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CEnt.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CEntBall.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CEntHuman.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CEntItem.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CEntPlayer.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CPic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CPicInput.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CPicMeter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\ent_utils.cpp
# End Source File
# End Group
# Begin Group "Control"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\shared\3d\CAliasBind.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CCamera.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CControlEdit.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CControlEntEdit.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CControlPlayer.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CControlTopView.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CFloatCam.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CInput.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CInputJoystick.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CJoystick.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CMouse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThingSubEdit.cpp
# ADD CPP /YX
# End Source File
# End Group
# Begin Group "Particles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\shared\3d\CParticle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CParticleEmitter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CParticleEmitterTokenizer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CParticleEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CParticleEventSequence.cpp
# End Source File
# End Group
# Begin Group "Zone"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\shared\3d\CBuilding.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CBuildingManager.cpp
# ADD CPP /YX
# End Source File
# End Group
# Begin Group "Text"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\shared\3d\CConversation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CFontInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CTextBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CTextBox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CTextChoice.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThingFont.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThingInput.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThingInput.h
# End Source File
# End Group
# Begin Group "Landscape"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\shared\3d\CGridTile.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CTerrain.cpp
# ADD CPP /YX
# End Source File
# End Group
# Begin Group "Visual"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\shared\3d\CSkyBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CVisual.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CVisualMulti.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CVisualSky.cpp
# ADD CPP /YX
# End Source File
# End Group
# Begin Group "Misc"

# PROP Default_Filter ""
# Begin Group "SphereTree"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\shared\3d\SphereTree\frustum.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\SphereTree\frustum.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\SphereTree\pool.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\SphereTree\sphere.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\SphereTree\sphere.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\SphereTree\spherepack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\SphereTree\spherepack.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\SphereTree\stl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\SphereTree\vector.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\shared\3d\CLightManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CLightning.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CommonFuncs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CSphere.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThing_globals.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThing_util.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThingStats.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\D3DHelperFuncs.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\shared\3d\CManager.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThing.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThingList.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThingSound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CWeather.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CWeatherSetting.cpp
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\shared\3d\CResource.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResource3DS.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResourceAnim.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResourceMD2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResourcePMesh.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResourceRTM.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResourceSphere.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResourceSprite.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResourceTex.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CRTM.CPP
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\Md2.cpp
# End Source File
# End Group
# Begin Group "Util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\shared\All.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CRandom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CTextParse.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CTimer.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\d3dapp.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\d3dfile.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\d3dfont.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\d3dutil.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\dxutil.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\Material.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\mfc_all.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\profile.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\std_all.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\uni.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\uni.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\unicode_utils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\vectormath.cpp
# ADD CPP /YX
# End Source File
# End Group
# Begin Group "file"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\shared\3d\CMod.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\file_swed.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\file_utils.cpp
# End Source File
# End Group
# Begin Group "Backend Engine And Data"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\shared\3d\CBaseEntData.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CBaseEntData.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CEntData.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CEntData.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CEntLib.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CEntPerson.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CMap.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CMapBlock.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CMapTile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CMessageHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CSubManager.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CSubZone.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CTileExtra.h
# End Source File
# End Group
# Begin Group "Script"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\shared\CScript.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CScriptEngine.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CScriptFunction.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CScriptOp.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CScriptParse.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CScriptVar.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CScriptVar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CScriptVarBase.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\script_3d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\script_config.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\script_config.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\Script_utils.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\Script_utils_ent.cpp

!IF  "$(CFG)" == "SWEd - Win32 Release"

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# End Group
# Begin Group "CFileLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\adler32.c
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\CFileLib.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\CFL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\CFL_delta16.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\CFL_delta32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\CFL_delta8.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\CFL_none.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\CFL_zlib.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\CFLDirectoryEntry.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\CFLLibrary.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\CFLMaker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\CFLResource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\CFLResourceFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\compress.c
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\crc32.c
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\deflate.c
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\gzio.c
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\infblock.c
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\infcodes.c
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\inffast.c
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\inflate.c
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\inftrees.c
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\infutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\ioapi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\iowin32.c

!IF  "$(CFG)" == "SWEd - Win32 Release"

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\trees.c
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\uncompr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\unzip.c
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\zip.c
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CFileLib\zutil.c
# End Source File
# End Group
# Begin Group "Sound"

# PROP Default_Filter ""
# Begin Group "FMOD"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\shared\Sound\FMOD\FMSoundManager.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\shared\Sound\ISoundManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\Sound\ISoundManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\Sound\SoundUtil.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CDX.CPP

!IF  "$(CFG)" == "SWEd - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "SWEd - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CGlobals.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\SWEd.rc
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\timer.cpp
# ADD CPP /YX
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\shared\3d\3dsImpExp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\all.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CAnimInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CBit8.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CBuilding.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CBuildingManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CCalControl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CCamera.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CControlEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CControlEntEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CControlPlayer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CControlTopView.h
# End Source File
# Begin Source File

SOURCE=.\CDX.H
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CEnt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CEnt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CEntBall.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CEntHuman.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CEntItem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CEntLib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CEntPerson.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CEntPlayer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CFloatCam.h
# End Source File
# Begin Source File

SOURCE=.\CGlobals.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CGridTile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CInput.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CLinkedList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CMap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CMapBlock.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CMessageHeader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CommonFuncs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\cal3d\cal3d\src\cal3d\coremodel.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CParticle.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CParticleEmitter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CParticleEmitterTokenizer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CParticleEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CParticleEventSequence.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CPic.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResource3DS.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResourceCalCore.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResourceCalRender.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResourcePMesh.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResourceRTM.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResourceSphere.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResourceSprite.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CResourceTex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CRTM.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CSkyBox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CSphere.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CSphereTex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CSubManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CSubZone.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CTerrain.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CTextParse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThing.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThing_globals.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThing_util.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThingFont.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThingList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThingStats.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CThingSubEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\CTimer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\custom_time.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CVisualMulti.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CVisualSky.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\CWeatherSetting.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\d3dapp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\d3dfile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\d3dfont.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\d3dres.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\d3dutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Pro\dx81\include\d3dx8math.h
# End Source File
# Begin Source File

SOURCE=.\dlg_3d.h
# End Source File
# Begin Source File

SOURCE=.\dlg_edit_ent_default.h
# End Source File
# Begin Source File

SOURCE=.\dlg_edit_entity.h
# End Source File
# Begin Source File

SOURCE=.\dlg_edit_sub.h
# End Source File
# Begin Source File

SOURCE=.\dlg_ent_edit_base.h
# End Source File
# Begin Source File

SOURCE=.\dlg_item_list.h
# End Source File
# Begin Source File

SOURCE=.\dlg_landscape.h
# End Source File
# Begin Source File

SOURCE=.\dlg_log.h
# End Source File
# Begin Source File

SOURCE=.\dlg_new.h
# End Source File
# Begin Source File

SOURCE=.\dlg_new_cat.h
# End Source File
# Begin Source File

SOURCE=.\dlg_pick_entity.h
# End Source File
# Begin Source File

SOURCE=.\dlg_sub_prop.h
# End Source File
# Begin Source File

SOURCE=.\dlg_terraform.h
# End Source File
# Begin Source File

SOURCE=.\dlg_weather.h
# End Source File
# Begin Source File

SOURCE=.\dlg_world_edit.h
# End Source File
# Begin Source File

SOURCE=.\dlg_zone_prop.h
# End Source File
# Begin Source File

SOURCE=.\dls_swed.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\dxutil.h
# End Source File
# Begin Source File

SOURCE=.\file_swed.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\file_utils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\Material.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\Md2.h
# End Source File
# Begin Source File

SOURCE=.\MFCGlobals.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\MinMax.h
# End Source File
# Begin Source File

SOURCE=..\..\..\cal3d\cal3d\src\cal3d\model.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\profile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\RecyclingArray.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\RecyclingArrayDyn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\cal3d\cal3d\src\cal3d\renderer.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\Script_utils.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SWEd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\timer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\vectormath.h
# End Source File
# Begin Source File

SOURCE=..\..\..\shared\3d\Vertex3D.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\DirectX.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon5.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon6.ico
# End Source File
# Begin Source File

SOURCE=.\res\SWEd.ico
# End Source File
# Begin Source File

SOURCE=.\res\SWEd.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
