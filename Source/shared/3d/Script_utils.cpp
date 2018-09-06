/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */




#include "Script_utils.h"
#include "CThing_Globals.h"
#include "CEntItem.h"
#include "..\\CScriptEngine.h"
#include "CEntLib.h"
#include "CThingList.h"
#include "..\\uni.h"
#include "CVisual.h"
#include "CMap.h"
#include "CSubManager.h"
#include "CTerrain.h"
#include "CParticleEmitter.h"
#include "..\\sound\\ISoundManager.h"
#include "CPic.h"
#include "CTextBox.h"
#include "CTextChoice.h"
#include "CThingFont.h"
#include "CConversation.h"
#include "file_utils.h"
#include "CWeather.h"
#include "d3dapp.h"
#include "CMouse.h"
#include "CCamera.h"
#include "CThingSound.h"
#include "script_utils_ent.h"	   
#include "CLightning.h"
#include "script_config.h"
#include "CPicMeter.h"
#include "CMessage.h"
#include "script_3d.h"
#include "CThing_util.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int g_i_touched_by_id;
   
void func_CreateEntityFromTemplate(CScriptPassVars * p_vars);
void func_EntSetPosition(CScriptPassVars *p_vars);
void func_EntSetPositionXYZ(CScriptPassVars *p_vars);
void func_EntKill(CScriptPassVars *p_vars);
void func_EntSetUniformScale(CScriptPassVars *p_vars);
void func_EntSetScale(CScriptPassVars *p_vars);
void func_EntGetScale(CScriptPassVars *p_vars);

void func_EntGetPosition(CScriptPassVars *p_vars);
void func_EntSetPhysics(CScriptPassVars *p_vars);
void func_GetRandomPointInSubZone(CScriptPassVars *p_vars);
void func_GetRandomPointInZone(CScriptPassVars *p_vars);
void func_SetOnHit(CScriptPassVars *p_vars);
void func_SetOnTouch(CScriptPassVars *p_vars);
void func_CreateParticle(CScriptPassVars *p_vars);
void func_ParticleKillTimer(CScriptPassVars *p_vars);
void func_EntSetKillTimer(CScriptPassVars *p_vars);

void func_EntSetNoDraw(CScriptPassVars *p_vars);
void func_ParticleSetPause(CScriptPassVars *p_vars);
void func_SoundPlay(CScriptPassVars *p_vars);
void func_SoundPlayH(CScriptPassVars *p_vars);
void func_SoundClearCache(CScriptPassVars *p_vars);
void func_SoundMusicPlay(CScriptPassVars *p_vars);
void func_SoundStayAlive(CScriptPassVars *p_vars);
void func_SoundPreload(CScriptPassVars *p_vars);
void func_Sound3DPreload(CScriptPassVars *p_vars);

void func_EntSetPicInputSound(CScriptPassVars *p_vars);
void func_EntGetByName(CScriptPassVars *p_vars);
void func_EntSetFollow(CScriptPassVars *p_vars);
void func_EntSetFollowOffset(CScriptPassVars *p_vars);
void func_ParticleSetFrameFollow(CScriptPassVars *p_vars);
void func_EntGetExist(CScriptPassVars *p_vars);
void func_EntGetNoDraw(CScriptPassVars *p_vars);
void func_PreloadGraphic(CScriptPassVars *p_vars);
void func_CreateEntPic(CScriptPassVars *p_vars);
void func_EntSetSprite(CScriptPassVars *p_vars);
void func_CreateEntPicMeter(CScriptPassVars *p_vars);
void func_EntPicSetTrigger(CScriptPassVars *p_vars);
void func_EntSetShadow(CScriptPassVars *p_vars);
void func_EntSetLayer(CScriptPassVars *p_vars);
void func_DialogAdd(CScriptPassVars *p_vars);
void func_DialogClear(CScriptPassVars *p_vars);
void func_CreateDialog(CScriptPassVars *p_vars);
void func_DialogSet(CScriptPassVars *p_vars);
void func_FontSet(CScriptPassVars *p_vars);
void func_DialogWaitForOk(CScriptPassVars *p_vars);
void func_CreateDialogChoice(CScriptPassVars *p_vars);
void func_CreateConversation(CScriptPassVars *p_vars);
void func_ConversationGetChoice(CScriptPassVars *p_vars);
void func_ConversationGetDialog(CScriptPassVars *p_vars);
void func_DialogChoiceSetCurrent (CScriptPassVars *p_vars);

void func_DialogChoiceAdd(CScriptPassVars *p_vars);
void func_ConversationSetPic(CScriptPassVars *p_vars);
void func_DialogChoiceWait(CScriptPassVars *p_vars);
void func_DialogChoiceClear(CScriptPassVars *p_vars);
void func_CreateEntScript(CScriptPassVars *p_vars);
void func_ZoneLoad(CScriptPassVars *p_vars);
void func_CreateEntWeather(CScriptPassVars *p_vars);
void func_PostQuitMessage(CScriptPassVars *p_vars);
void func_SetPauseLevel(CScriptPassVars *p_vars);
void func_GetPauseLevel(CScriptPassVars *p_vars);
void func_EntGetPauseLevel(CScriptPassVars *p_vars);
void func_EntSetPauseLevel(CScriptPassVars *p_vars);
void func_CreateEntMouse(CScriptPassVars *p_vars);
void func_EntSetIgnoreKill(CScriptPassVars *p_vars);
void func_EntSetXY(CScriptPassVars *p_vars);
void func_CreateEntButton(CScriptPassVars *p_vars);
void func_EntSetButtonHighlight(CScriptPassVars *p_vars);
void func_EntSetButtonHighlightSound(CScriptPassVars *p_vars);
void func_EntKillAllPics(CScriptPassVars *p_vars);
void func_EntReplaceTexture(CScriptPassVars *p_vars);
void func_EntSetTilt(CScriptPassVars *p_vars);
void func_EntGetTilt(CScriptPassVars *p_vars);
void func_EntSetDirection(CScriptPassVars *p_vars);
void func_EntGetDirection(CScriptPassVars *p_vars);
void func_EntSetTargetDirection(CScriptPassVars *p_vars);
void func_EntSetTargetTilt(CScriptPassVars *p_vars);

void func_EntSetTargetPosition(CScriptPassVars *p_vars);
void func_BindKeyToScript(CScriptPassVars *p_vars);
void func_Sound3DPlay(CScriptPassVars *p_vars);
void func_SoundEntPlay(CScriptPassVars *p_vars);
void func_SoundMusicStop(CScriptPassVars *p_vars);
void func_SoundKill(CScriptPassVars *p_vars);
void func_EntGetRandomByName(CScriptPassVars *p_vars);
void func_CreateLightning(CScriptPassVars *p_vars);
void func_EntLightningSetFollow(CScriptPassVars *p_vars);

void func_EntWeatherFlash(CScriptPassVars *p_vars);
void func_EntWeatherFade(CScriptPassVars *p_vars);
void func_SoundPlayInstance(CScriptPassVars *p_vars);
void func_FileExists(CScriptPassVars *p_vars);
void func_EntSetGroundFriction(CScriptPassVars *p_vars);
void func_EntSetBounce(CScriptPassVars *p_vars);
void func_ShowMessage(CScriptPassVars *p_vars);

void func_DialogSetStyleDefault(CScriptPassVars *p_vars);



void add_global_functions_to_script_0()
{
	if (!cls_tglo.p_script_engine)
	{
		log_error("Scripting engine not initted, can't add 3d functions.");
	}
						 
	CScriptPassVars *p_vars;

	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_VECTOR3); //position
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetPosition", func_EntSetPosition, p_vars);

    p_vars = new CScriptPassVars();
    
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //position
    p_vars->a_vars[2].SetType(C_SCRIPT_VAR_FLOAT); //position
    p_vars->a_vars[3].SetType(C_SCRIPT_VAR_FLOAT); //position
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetPositionXYZ", func_EntSetPositionXYZ, p_vars);
    
	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_VECTOR3); //position
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetTargetPosition", func_EntSetTargetPosition, p_vars);


	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->return_var.SetType(C_SCRIPT_VAR_VECTOR3); //position
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntGetPosition", func_EntGetPosition, p_vars);


	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //who should follow
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //id of who to follow
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetDirection", func_EntSetDirection, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent
	p_vars->return_var.SetType(C_SCRIPT_VAR_FLOAT);  //dir they are facing (y axis)
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntGetDirection", func_EntGetDirection, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //who should follow
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //id of who to follow
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetTilt", func_EntSetTilt, p_vars);

    	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //who we should get.  ent or camera ok
	p_vars->return_var.SetType(C_SCRIPT_VAR_FLOAT); //id of who to follow
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntGetTilt", func_EntGetTilt, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //who should follow
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //id of who to follow
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetTargetTilt", func_EntSetTargetTilt, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //who should follow
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //id of who to follow
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetTargetDirection", func_EntSetTargetDirection, p_vars);


	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id of who to kill
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntKill", func_EntKill, p_vars);

	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //position
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetUniformScale", func_EntSetUniformScale, p_vars);

	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_VECTOR3); //position
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetScale", func_EntSetScale, p_vars);

    p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->return_var.SetType(C_SCRIPT_VAR_VECTOR3); //position
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntGetScale", func_EntGetScale, p_vars);


    	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //friction, 0.01 is normal?
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetGroundFriction", func_EntSetGroundFriction, p_vars);

        	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //friction, 0.50 means keep half the power?
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetBounce", func_EntSetBounce, p_vars);


	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //physics level, 0 for none
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetPhysics", func_EntSetPhysics, p_vars);


   	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //file name. will check real and resource based files.  ".." not permitted.
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //1 for file existed
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("FileExists", func_FileExists, p_vars);


    
    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //string input of sub zone name.  Send "" to specify only non-defined sub zone area.
	p_vars->return_var.SetType(C_SCRIPT_VAR_VECTOR3); //output of position
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("GetRandomPointInSubZone", func_GetRandomPointInSubZone, p_vars);


    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //like above but gets from anywhere.  send it 1 to force a non-hard tile
	p_vars->return_var.SetType(C_SCRIPT_VAR_VECTOR3); //output of position
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("GetRandomPointInZone", func_GetRandomPointInZone, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //should be a bool but...
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SetOnHit", func_SetOnHit, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //name of particle script
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //int we created
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CreateParticle", func_CreateParticle, p_vars);

	
	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_VECTOR3); //position to create sound
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); 	//sound file
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT); //repeat times, 1 for once
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //return id of ent created, -1 for error
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("Sound3DPlay", func_Sound3DPlay, p_vars);

   	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent to get position from
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); 	//sound file
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //return id of ent created, -1 for error
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SoundEntPlay", func_SoundEntPlay, p_vars);



	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //should be a bool but...
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetNoDraw", func_EntSetNoDraw, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //should be a bool but...
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntGetNoDraw", func_EntGetNoDraw, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //should be a bool but...
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntGetExist", func_EntGetExist, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of f system
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //should be a bool but...
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("ParticleSetPause", func_ParticleSetPause, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //sound to play
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //id of sound created
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SoundPlay", func_SoundPlay, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //sound to play
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //id of sound created
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SoundPlayInstance", func_SoundPlayInstance, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of sound to play
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //loops
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SoundPlayH", func_SoundPlayH, p_vars);

  	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of sound to play
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //loops
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SoundStayAlive", func_SoundStayAlive, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //should be a bool but...
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SetOnTouch", func_SetOnTouch, p_vars);

    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent to operate on
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //offset
    p_vars->a_vars[2].SetType(C_SCRIPT_VAR_FLOAT); 
    p_vars->a_vars[3].SetType(C_SCRIPT_VAR_FLOAT); 
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetFollowOffset", func_EntSetFollowOffset, p_vars);
    


	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //who should follow
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //id of who to follow
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetFollow", func_EntSetFollow, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //should be a bool but...
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("ParticleSetFrameFollow", func_ParticleSetFrameFollow, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //string input of id
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //output of id
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntGetByName", func_EntGetByName, p_vars);


	SetupScriptEntStuff();

 	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //string input of id
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //output of id
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntGetRandomByName", func_EntGetRandomByName, p_vars);



	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //id
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("PreloadGraphic", func_PreloadGraphic, p_vars);

	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //ent lib name
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //ent template name
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //return id of ent created, -1 for error
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CreateEntityFromTemplate", func_CreateEntityFromTemplate, p_vars);


	p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //return id of ent created, -1 for error
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //ent id
	
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CreateEntPic", func_CreateEntPic, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //return id of ent created, -1 for error
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //type of meter to create 1 is left based
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_FLOAT); //type of meter to create 1 is left based
	
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CreateEntPicMeter", func_CreateEntPicMeter, p_vars);

    

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //type of trigger, -1 is none, 1 is same or higher.
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_FLOAT); //trigger amount
	p_vars->a_vars[3].SetType(C_SCRIPT_VAR_INT); //script to run
	p_vars->a_vars[4].SetType(C_SCRIPT_VAR_WCHAR); //script function to run
	

	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntPicSetTrigger", func_EntPicSetTrigger, p_vars);


	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //filename of sprite to load
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetSprite", func_EntSetSprite, p_vars);

 	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //X
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_FLOAT); //Y
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetXY", func_EntSetXY, p_vars);


	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //layer
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetLayer", func_EntSetLayer, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //name of ent to make
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //return id of ent created, -1 for error
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CreateDialog", func_CreateDialog, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("DialogClear", func_DialogClear, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //text to add
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("DialogAdd", func_DialogAdd, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //text to add
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("DialogSet", func_DialogSet, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //index of choice
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("DialogChoiceSetCurrent", func_DialogChoiceSetCurrent, p_vars);


	//note, if the font name and size is the same, it won't be changed.
	
	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //font id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //font name
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_WCHAR); //font filename
	p_vars->a_vars[3].SetType(C_SCRIPT_VAR_INT); //font size
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("FontSet", func_FontSet, p_vars);

 	p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_FORCE_WAIT);  //reserved?
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of dialog box
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("DialogWaitForOk", func_DialogWaitForOk, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //return id of ent created, -1 for error
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //name of ent to make
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CreateDialogChoice", func_CreateDialogChoice, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //return id of ent created, -1 for error
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //name of ent to make
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CreateConversation", func_CreateConversation, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //return id of ent created, -1 for error
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of dialog box
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("ConversationGetDialog", func_ConversationGetDialog, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //return id of ent created, -1 for error
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of dialog box
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("ConversationGetChoice", func_ConversationGetChoice, p_vars);


	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of choice dialog box
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //id of choice, when selected this is the # it will return
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_WCHAR); //text of choice
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("DialogChoiceAdd", func_DialogChoiceAdd, p_vars);


	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of conversation
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //filename of pic
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("ConversationSetPic", func_ConversationSetPic, p_vars);


	p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_FORCE_WAIT);  //reserved?
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of choice dialog box
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("DialogChoiceWait", func_DialogChoiceWait, p_vars);


   	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("DialogChoiceClear", func_DialogChoiceClear, p_vars);

 	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //ent id
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //return id of ent created, -1 for error
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CreateEntScript", func_CreateEntScript, p_vars);
 

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //name of particle script
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_VECTOR3); //start of lighting rod
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_VECTOR3); //end
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //int we created
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CreateLightning", func_CreateLightning, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of lightning
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //id of who to follow
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EnteLightningSetFollow", func_EntLightningSetFollow, p_vars);



 	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //ent name
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //return id of ent created, -1 for error
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CreateEntWeather", func_CreateEntWeather, p_vars);

 	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //weather ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //time of flash
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntWeatherFlash", func_EntWeatherFlash, p_vars);

 	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //weather ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //weather filename
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT); //length of fade
	p_vars->a_vars[3].SetType(C_SCRIPT_VAR_FLOAT); //lerp amount
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntWeatherFade", func_EntWeatherFade, p_vars);

	p_vars = new CScriptPassVars();
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("PostQuitMessage", func_PostQuitMessage, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //sound to play
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //how many times to repeat it
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //id of sound created
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SoundMusicPlay", func_SoundMusicPlay, p_vars);

 	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //sound to play
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SetPauseLevel", func_SetPauseLevel, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //id of sound created
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("GetPauseLevel", func_GetPauseLevel, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //sound to play
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //sound to play
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetPauseLevel", func_EntSetPauseLevel, p_vars);


	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //sound to play
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //id of sound created
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntGetPauseLevel", func_EntGetPauseLevel, p_vars);
	

	p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //return id of ent created, -1 for error
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //ent id
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CreateEntMouse", func_CreateEntMouse, p_vars);
	

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //1 for true
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetIgnoreKill", func_EntSetIgnoreKill, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //return id of ent created, -1 for error
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //ent name
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //normal button image
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_WCHAR); //clicked button image
	p_vars->a_vars[3].SetType(C_SCRIPT_VAR_WCHAR); //script to run when clicked
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CreateEntButton", func_CreateEntButton, p_vars);
    
    
    
        p_vars = new CScriptPassVars();
        p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of button to mod
        p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //filename of pic to load
        cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetButtonHighlight", func_EntSetButtonHighlight, p_vars);

        p_vars = new CScriptPassVars();
        p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of button to mod
        p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //filename of sound to play
        cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetButtonHighlightSound", func_EntSetButtonHighlightSound, p_vars);
        
	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //ent id
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("ZoneLoad", func_ZoneLoad, p_vars);

/*********** same as EntSetKillTimer but here as legacy support for old scripts ***********/
  	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //PS ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //time to die in
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("ParticleKillTimer", func_EntSetKillTimer, p_vars);

//********************************************************************

    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //PS ent id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //time to die in
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetKillTimer", func_EntSetKillTimer, p_vars);


	p_vars = new CScriptPassVars();
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntKillAllPics", func_EntKillAllPics, p_vars);


	
	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); 	//texture set name
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_WCHAR); //old texture
	p_vars->a_vars[3].SetType(C_SCRIPT_VAR_WCHAR); //new tex
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntReplaceTexture", func_EntReplaceTexture, p_vars);

 	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //name of keys, separated by commas
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); 	//script to run
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT); //repeat rate, -1 for none
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("BindKeyToScript", func_BindKeyToScript, p_vars);

  	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //shadow mode, 0 for none, 1 for accurate, 2 for fake
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetShadow", func_EntSetShadow, p_vars);


    
    p_vars = new CScriptPassVars();
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("SoundMusicStop", func_SoundMusicStop, p_vars);
    
    
    p_vars = new CScriptPassVars();
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("SoundClearCache", func_SoundClearCache, p_vars);
    
    
    
    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of sound
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("SoundKill", func_SoundKill, p_vars);

    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("SoundPreload", func_SoundPreload, p_vars);
    
    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("Sound3DPreload", func_Sound3DPreload, p_vars);
    
   
    
    p_vars = new CScriptPassVars();
    
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("DialogSetStyleDefault", func_DialogSetStyleDefault, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //ent id
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("ShowMessage", func_ShowMessage, p_vars);


    add_global_functions_to_script_0_config();
    add_global_functions_to_script_0_3d();

	//ALSO add any global vars we need
    int i_last_choice = cls_tglo.p_script_engine->GetScript(0)->AddVar("g_i_last_choice", ::C_SCRIPT_VAR_INT);
    int i_last_string = cls_tglo.p_script_engine->GetScript(0)->AddVar("g_st_last_string", ::C_SCRIPT_VAR_WCHAR);
    g_i_touched_by_id = cls_tglo.p_script_engine->GetScript(0)->AddVar("g_id_touched_by", ::C_SCRIPT_VAR_INT);
   
}

 
char * GetScriptName(int i_script)
{
    return cls_tglo.p_script_engine->GetScript(i_script)->GetName();
}


void func_EntKill(CScriptPassVars *p_vars)
{

    if (cls_tglo.p_thing_list->b_deleting_list) return; //killing game, don't worry about it
    
    //kill an int from his ID
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CThing * p_temp = (CThing*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntKill> Ent %d is invalid in script %s.", p_vars->a_vars[0].GetInt(),
            GetScriptName(p_vars->i_script));
		return;
	}
  //let's set the position and also force an update.

	p_temp->b_delete_flag = true; //die!

}



void func_EntSetPosition(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CVisual * p_temp = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntSetPosition> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.
	if (p_temp->get_type() == C_THING_CAMERA)
	{
		((CCamera*)p_temp)->SetTargetPosition(p_vars->a_vars[1].GetVector3());
	}
	p_temp->setObjectPosition(*p_vars->a_vars[1].GetVector3());

}


void func_EntSetPositionXYZ(CScriptPassVars *p_vars)
{
    //move the ent to the correct spot
    //this is kind of a slow look up by id.  Maybe do something about this later...
    CVisual * p_temp = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
    if (!p_temp)
    {
        log_error("EntSetPositionXYZ> Int %d is invalid.", p_vars->a_vars[0].GetInt());
        return;
    }
    
    D3DXVECTOR3 vec_pos;

    vec_pos.x = p_vars->a_vars[1].GetFloat();
    vec_pos.y = p_vars->a_vars[2].GetFloat();
    vec_pos.z = p_vars->a_vars[3].GetFloat();
    


    //let's set the position and also force an update.
    if (p_temp->get_type() == C_THING_CAMERA)
    {
    
        ((CCamera*)p_temp)->SetTargetPosition(&vec_pos);
    }
    p_temp->setObjectPosition(vec_pos);
    
}

void func_FileExists(CScriptPassVars *p_vars)
{
    uni uni_file(p_vars->a_vars[0].GetWChar());

    if (strstr(uni_file.st_data, "..") != NULL)
    {

        log_error("FileExist command refused, can't have .. in it.");
      	p_vars->return_var.SetVar(::C_SCRIPT_VAR_INT, 0, 0, NULL);

        return;
    }

    if (GetApp()->GetCFL()->fileExists(uni_file.st_data))
    {
      	p_vars->return_var.SetVar(::C_SCRIPT_VAR_INT, 1, 0, NULL);
        return;
    }
  
   //couldn't find the file
  	p_vars->return_var.SetVar(::C_SCRIPT_VAR_INT, 0, 0, NULL);
}


void func_EntGetPosition(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CVisual * p_temp = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntGetPosition> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.

	p_vars->return_var.SetVar(C_SCRIPT_VAR_VECTOR3,0,0,p_temp->getObjectPosition());

}


void func_EntSetUniformScale(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CVisual * p_temp = (CVisual *)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntSetUniformScale> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.



	//is it an entitem?
	if (p_temp->get_type() == C_THING_ENT_GENERIC)
	{
	   ((CEntItem*)p_temp)->SetRelativeScaleUniform(p_vars->a_vars[1].GetFloat());
	} else
	{
	   p_temp->set_scale_uniform(p_vars->a_vars[1].GetFloat());
	}

}


void func_EntSetGroundFriction(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CEnt * p_temp = (CEnt *)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntSetGroundFriction> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.
	   p_temp->SetGroundFriction(p_vars->a_vars[1].GetFloat());
}

void func_EntSetBounce(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CEnt * p_temp = (CEnt *)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntSetBounce> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.
	   p_temp->SetBounce(p_vars->a_vars[1].GetFloat());
}


void func_EntSetScale(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CEntItem * p_temp = (CEntItem*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntSetScale> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.

	p_temp->SetRelativeScale(*p_vars->a_vars[1].GetVector3());

}



void func_EntGetScale(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CEntItem * p_temp = (CEntItem*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntGetScale> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
    
    D3DXVECTOR3 vec_temp = p_temp->GetRelativeScale();
 
    //let's set the position and also force an update.
  	p_vars->return_var.SetVar(C_SCRIPT_VAR_VECTOR3,0,0, &vec_temp);


}

void func_CreateEntityFromTemplate(CScriptPassVars * p_vars)
{

        //locate the template they want to use by string name
	    unsigned int  ui_class = cls_tglo.p_ent_lib->get_class_id_from_string(uni(p_vars->a_vars[0].GetWChar()).to_st());
		if (!cls_tglo.p_ent_lib->class_exists(ui_class))
		{
			log_error("CreateEntityFromTemplate> No class of %s found.",uni(p_vars->a_vars[0].GetWChar()).to_st() );
			p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, -1, 0, NULL); //signal error to script	
			return;
		}

		//locate the item index by name too
		int i_ent_id = cls_tglo.p_ent_lib->get_ent_id_from_string(ui_class, uni(p_vars->a_vars[1].GetWChar()).to_st());

		if (i_ent_id == -1)
		{
			log_error("CreateEntityFromTemplate> No entity with a name of %s exists in class %s.",
				uni(p_vars->a_vars[1].GetWChar()).to_st(),uni(p_vars->a_vars[0].GetWChar()).to_st());
			p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, -1, 0, NULL); //signal error to script	
			return;
		
		
		}
  
		CEntItem *p_ent = new CEntItem();	//an ent item is an entity build/owning an item
		cls_tglo.p_thing_list->add_new_thing(p_ent);

		CEntData *p_item_data = NULL; //this will hold the actual item data that gets loaded/saved
		(*cls_tglo.p_ent_lib->get_base_item(ui_class, i_ent_id)).p_ent_default->clone(&p_item_data);
		p_ent->setup_item_from_cent(p_item_data);
		//preload it
		p_ent->dx_create();
		p_vars->return_var.SetVar(::C_SCRIPT_VAR_INT, p_ent->get_id(), 0, NULL); //signal error to script	
		
//		log_msg("Created thing %d, %d.", ui_class, i_ent_id);
}



void func_EntSetPhysics(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CEnt * p_temp = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntSetPhysics> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.

	p_temp->SetPhysics(p_vars->a_vars[1].GetInt());

}


void func_EntSetShadow(CScriptPassVars *p_vars)
{
	CEnt * p_temp = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntSetShadow> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.

	p_temp->SetShadowMode(p_vars->a_vars[1].GetInt());

}


void func_EntSetNoDraw(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CVisual * p_temp = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntSetNoDraw> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.
	p_temp->set_no_draw(p_vars->a_vars[1].GetInt() != 0);

}

 void func_EntGetNoDraw(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CVisual * p_temp = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
		log_error("EntGetNoDraw> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
    
	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_temp->get_no_draw(),0,  NULL);
}
 void func_EntGetExist(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CVisual * p_temp = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
		log_error("EntGetExist> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
    
	//it's alive!
	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 1, 0, NULL);
}


void func_SetOnHit(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CEntItem * p_temp = (CEntItem*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("SetOnHit> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.

	p_temp->SetOnHitScript(p_vars->a_vars[1].GetInt() != 0);

}

void func_SetOnTouch(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CEntItem * p_temp = (CEntItem*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("SetOnTouch> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.

	p_temp->SetOnTouchScript(p_vars->a_vars[1].GetInt()!=0);

}


void func_GetRandomPointInZone(CScriptPassVars *p_vars)
{
  
	//first find the sub zone by string..
     	
	int i_counter = 0;
	for(;i_counter < 5000; i_counter++)
	{
		
		D3DXVECTOR3 vec_pos = D3DXVECTOR3(
			frandom_range(0, cls_tglo.p_map->get_size_x() * cls_tglo.p_map->get_block_size()),
			0,
			frandom_range(0, cls_tglo.p_map->get_size_y() * cls_tglo.p_map->get_block_size())
			);
		
		//ok, is the position we just generated valid?
	  if (p_vars->a_vars[0].GetInt() == 0)
      {
          //let's go ahead and give them a valid Y value too
			vec_pos.y = cls_tglo.p_terrain->get_height(vec_pos.x, vec_pos.z);
			p_vars->return_var.SetVar(C_SCRIPT_VAR_VECTOR3,0,0, &vec_pos);
            return;
      } else
      {
          //must not be hard
        if (!cls_tglo.p_map->tile_is_hard_world(vec_pos.x, vec_pos.z))
		{
			//this is good!
			
			//let's go ahead and give them a valid Y value too
			vec_pos.y = cls_tglo.p_terrain->get_height(vec_pos.x, vec_pos.z);
			p_vars->return_var.SetVar(C_SCRIPT_VAR_VECTOR3,0,0, &vec_pos);
		    return;	
		}

      }
    	
	}
	
 log_error("func_GetRandomPointInZone couldn't find a random point. :( ");

}
 


void func_GetRandomPointInSubZone(CScriptPassVars *p_vars)
{
  
	//first find the sub zone by string..
		
	int i_sub_zone;
    
    if (strlen(uni(p_vars->a_vars[0].GetWChar()).to_st()) == 0)
    {
        i_sub_zone = -1; //default
    } else
    {
    

    i_sub_zone = cls_tglo.p_map->p_sub_manager->GetSubZoneByName(uni(p_vars->a_vars[0].GetWChar()).to_st());

	if (i_sub_zone == -1)
	{
		log_error("Unable to locate sub zone %s.", uni(p_vars->a_vars[0].GetWChar()).to_st());
			return; //didn't set any vars
	}
    }

//	log_msg("Found subzone %d.", i_sub_zone);
	//now we need to get a random location that is contained inside a sub zone.
	//get random point in sub_zone.. this is curently slow and dangerous and only safe on a map where basically the
	//whole thing is this subzone.  (ie, the lawn mowing game)
	
//later I should use this code to figure out the ratio and build a custom grab list if it's too small
    //an area
//    int i_zone_squares = cls_tglo.p_map->p_sub_manager->p_a_sub_zone[i_sub_zone].i_zone_squares;
//    float f_ratio =  float(i_zone_squares) / float (cls_tglo.p_map->i_tile_num);
//    char st_crap[256];
//    sprintf(st_crap, "Zone %s has %d squares. Ratio is %.2f",uni(p_vars->a_vars[0].GetWChar()).to_st(), i_zone_squares, f_ratio);
//    log_msg(st_crap);


	int i_counter = 0;
	for(;i_counter < 5000; i_counter++)
	{
		
		D3DXVECTOR3 vec_pos = D3DXVECTOR3(
			frandom_range(0, cls_tglo.p_map->get_size_x() * cls_tglo.p_map->get_block_size()),
			0,
			frandom_range(0, cls_tglo.p_map->get_size_y() * cls_tglo.p_map->get_block_size())
			);
		
		//ok, is the position we just generated valid?
	
        
        if (
            
            (i_sub_zone == cls_tglo.p_map->get_tile_pointer(cls_tglo.p_map->get_tile_x(vec_pos.x),
			cls_tglo.p_map->get_tile_y(vec_pos.z))->i_sub_zone)
            &&
            (!cls_tglo.p_map->tile_is_hard_world(vec_pos.x, vec_pos.z))
            )
		{
			//this is good!
			
			//let's go ahead and give them a valid Y value too
			vec_pos.y = cls_tglo.p_terrain->get_height(vec_pos.x, vec_pos.z);
			p_vars->return_var.SetVar(C_SCRIPT_VAR_VECTOR3,0,0, &vec_pos);
		    return;	
		}
		
	}
	
 log_error("GetRandomPointInSubZone couldn't find a random point. :( ");

}

void func_ParticleSetPause(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CParticleEmitter * p_temp = (CParticleEmitter*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("ParticleSetPause> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.
	p_temp->set_no_new_particles(p_vars->a_vars[1].GetInt()!= 0);

}

void func_EntSetKillTimer(CScriptPassVars *p_vars)
{
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CThing * p_temp = cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntSetKillTimer> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.
	p_temp->SetKillTimer(p_vars->a_vars[1].GetInt());

}


//create a particle system
void func_CreateParticle(CScriptPassVars *p_vars)
{

	//create particle system
	CParticleEmitter *p_ps = new CParticleEmitter();
	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, cls_tglo.p_thing_list->add_new_thing(p_ps),
		0, NULL);
	//setup what kind of particle system we need
	p_ps->load_and_compile_particle_system(uni(p_vars->a_vars[0].GetWChar()).to_st());
//	p_ps->set_visual_follow(this);
	p_ps->dx_create();
 
 }

void func_CreateLightning(CScriptPassVars *p_vars)
{

	//create particle system
	CLightning *p_ps = new CLightning();
	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, cls_tglo.p_thing_list->add_new_thing(p_ps),
		0, NULL);
	//setup what kind of particle system we need
	p_ps->SetStartPoint(p_vars->a_vars[1].GetVector3());
	p_ps->SetEndPoint(p_vars->a_vars[2].GetVector3());


	p_ps->dx_create();
 
 }

void func_EntLightningSetFollow(CScriptPassVars *p_vars)
{

	CLightning * p_temp = (CLightning*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntLightningSetFollow> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.
	p_temp->SetFollowTarget(p_vars->a_vars[1].GetInt());

 }


 //play a sound once
void func_SoundPlay(CScriptPassVars *p_vars)
{
     p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, cls_tglo.p_sound->Play(CDir(uni(p_vars->a_vars[0].GetWChar()).to_st())),
      0, NULL);
}


void func_SoundPlayInstance(CScriptPassVars *p_vars)
{
     uni u_name(p_vars->a_vars[0].GetWChar());

    char st_name[256];
    strcpy(st_name, u_name.st_data);

    if (!exist(st_name))
    {
        sprintf(st_name, "%s%s", GetExeDir(), u_name.st_data);
    }
    
        int h_sound = cls_tglo.p_sound->SoundInstanceCreateAndLoad(st_name);
        cls_tglo.p_sound->SoundInstancePlay(h_sound, 1);
   
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, h_sound ,  0, NULL);
    
}


void func_SoundPlayH(CScriptPassVars *p_vars)
{
 cls_tglo.p_sound->SoundInstancePlay(p_vars->a_vars[0].GetInt(), p_vars->a_vars[1].GetInt());
}

void func_SoundStayAlive(CScriptPassVars *p_vars)
{
 cls_tglo.p_sound->SoundInstanceSetStayAlive(p_vars->a_vars[0].GetInt(), p_vars->a_vars[1].GetInt()!=0);
}

void func_SoundMusicPlay(CScriptPassVars *p_vars)
{
    p_vars->return_var.SetInt(cls_tglo.p_sound->PlayMusic(CDir(uni(p_vars->a_vars[0].GetWChar()).to_st()), p_vars->a_vars[1].GetInt()));
}

//create a particle system


void func_EntGetByName(CScriptPassVars *p_vars)
{
	
	//can through all items and look for a certain name, then return the id. -1 if we
	//can't find anyone
	static char st_temp[32];
	strcpy(st_temp, uni(p_vars->a_vars[0].GetWChar()).to_st());
	CThing * p_thing = NULL;
	
	while (p_thing = cls_tglo.p_thing_list->get_next())
	{
	//	log_msg("Comparing %s to %s.", p_thing->get_thing_name(), st_temp);
		if (stricmp(p_thing->get_thing_name(), st_temp) == 0)
		{
		    if (!p_thing->b_delete_flag)
			{
			
			//reset next for the next guy
			cls_tglo.p_thing_list->reset_get_next();
			//found match!
			p_vars->return_var.SetVar(::C_SCRIPT_VAR_INT, p_thing->get_id(), 0, NULL);
			return;
			}
		}
	}
	
	//found jack and or crap
	p_vars->return_var.SetVar(::C_SCRIPT_VAR_INT, -1, 0, NULL);
}



void func_EntGetRandomByName(CScriptPassVars *p_vars)
{
	
	//can through all items and look for a certain name, then return the id. -1 if we
	//can't find anyone
	static char st_temp[32];
	strcpy(st_temp, uni(p_vars->a_vars[0].GetWChar()).to_st());
	CThing * p_thing = NULL;

	
	//first count how many total there are

	int i_count = 0;


	while (p_thing = cls_tglo.p_thing_list->get_next())
	{
	//	log_msg("Comparing %s to %s.", p_thing->get_thing_name(), st_temp);
		if (stricmp(p_thing->get_thing_name(), st_temp) == 0)
		{
		    if (!p_thing->b_delete_flag)
			{
			i_count++;
			}
		}
	}
	
	if (i_count == 0)
	{
		
		//found jack and or crap
		p_vars->return_var.SetVar(::C_SCRIPT_VAR_INT, -1, 0, NULL);
		return;
	}

	
	//choose which one they should get

	int i_chosen = random_range(1, i_count);

     i_count = 0;
	
	
	while (p_thing = cls_tglo.p_thing_list->get_next())
	{
		//	log_msg("Comparing %s to %s.", p_thing->get_thing_name(), st_temp);
		if (stricmp(p_thing->get_thing_name(), st_temp) == 0)
		{
			if (!p_thing->b_delete_flag)
			{
				i_count++;
				
				if (i_count == i_chosen)
				{
					
					//reset next for the next guy
					cls_tglo.p_thing_list->reset_get_next();
					p_vars->return_var.SetVar(::C_SCRIPT_VAR_INT, p_thing->get_id(), 0, NULL);
					return;
				}
			}
		}
	}
	
  log_msg("Error in EntGetRandomByName, we should never get here.");

}



void func_EntSetFollow(CScriptPassVars *p_vars)
{
	//make parm 0 follow parm 1 around
	
	CVisual * p_temp = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntSetFollow> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
    
    //now who should we follow?
	if (p_vars->a_vars[1].GetInt() == -1)
    {
        //special case, they want to STOP following somebody
        p_temp->set_visual_follow(NULL);
        return;
    }
	CVisual * p_target = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[1].GetInt());
	if (!p_target)
	{
		log_error("EntSetFollow> second parm %d is invalid.", p_vars->a_vars[1].GetInt());
		return;
	}
  	
	//let's set the position and also force an update.
	p_temp->set_visual_follow(p_target);

}

void func_EntSetFollowOffset(CScriptPassVars *p_vars)
{
     
    CVisual * p_temp = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
    if (!p_temp)
    {
        log_error("EntSetFollowOffset> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
        return;
    }
    
    D3DXVECTOR3 vec;
    vec.x = p_vars->a_vars[1].GetFloat();
    vec.y = p_vars->a_vars[2].GetFloat();
    vec.z = p_vars->a_vars[3].GetFloat();

    //let's set the position and also force an update.
    p_temp->set_visual_follow_offset(vec);
    
}


void func_ParticleSetFrameFollow(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CParticleEmitter * p_temp = (CParticleEmitter*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("ParticleSetFrameFollow> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.
	p_temp->SetFrameFollow(p_vars->a_vars[1].GetInt()!=0);

}

void func_PreloadGraphic(CScriptPassVars *p_vars)
{
    //for speed let's load this now into our resource manager
	int i_resource_type =  C_RESOURCE_TEXTURE;

	if (file_extension_is(uni(p_vars->a_vars[0].GetWChar()).to_st(), ".x"))
	{
		 i_resource_type = C_RESOURCE_PMESH;
	}
    
	int i_id = cls_tglo.p_manager->get_dynamic_resource_by_string(i_resource_type, uni(p_vars->a_vars[0].GetWChar()).to_st());
	cls_tglo.p_manager->load_if_needed(i_id);
}


//create a generic Cpic object
void func_CreateEntPic(CScriptPassVars *p_vars)
{

	//create new cpic
	CPic *p_ps = new CPic();
	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, cls_tglo.p_thing_list->add_new_thing(p_ps),
		0, NULL);
		//set the name also
    uni uni_temp(0);	
	uni_temp.set_us(p_vars->a_vars[0].GetWChar());
	if (strlen(uni_temp.st_data) >= C_MAX_THING_NAME_LENGTH)
	{
		log_error("%s is too long a thing name. %d is the max.", uni_temp.st_data,C_MAX_THING_NAME_LENGTH );
	} else
	{
		p_ps->set_thing_name(uni_temp.st_data);
	}

	//setup what kind of particle system we need
	//set a few defaults?
	//p_ps->set_visual_follow(this);

	p_ps->dx_create();
}


//create a generic Cpicmeter object
void func_CreateEntPicMeter(CScriptPassVars *p_vars)
{

	//create new cpic
	CPicMeter *p_ps = new CPicMeter();
	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, cls_tglo.p_thing_list->add_new_thing(p_ps),
		0, NULL);
		//set the name also
    uni uni_temp(0);	
	uni_temp.set_us(p_vars->a_vars[0].GetWChar());
	if (strlen(uni_temp.st_data) >= C_MAX_THING_NAME_LENGTH)
	{
		log_error("%s is too long a thing name. %d is the max.", uni_temp.st_data,C_MAX_THING_NAME_LENGTH );
	} else
	{
		p_ps->set_thing_name(uni_temp.st_data);
	}

	//setup what kind of particle system we need
	//set a few defaults?
	//p_ps->set_visual_follow(this);
    p_ps->InitMeter(p_vars->a_vars[1].GetInt());
    p_ps->SetMeterNoLerp(p_vars->a_vars[2].GetFloat());
    p_ps->dx_create();
}


void func_EntPicSetTrigger(CScriptPassVars *p_vars)
{

	CPic * p_meter = (CPic*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_meter)
	{
		log_error("EntPicSetTrigger> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
    p_meter->SetTrigger(p_vars->a_vars[1].GetInt(),
        p_vars->a_vars[2].GetFloat(),
        p_vars->a_vars[3].GetInt(),
        uni(p_vars->a_vars[4].GetWChar()).to_st()
        );
	
    
}


//create a generic Cpic object
void func_EntSetSprite(CScriptPassVars *p_vars)
{

	
	CVisualMulti * p_pic = (CVisualMulti*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_pic)
	{
		log_error("EntSetSprite> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
    //now load the sprite we're supposed to
	
    p_pic->SetMipMap(1); //force the texture to use only 1 mipmap level
    
    p_pic->ReleaseResourceIfWeHaveOne(); 
    p_pic->init_visual_sprite(uni(p_vars->a_vars[1].GetWChar()).to_st(),uni(p_vars->a_vars[1].GetWChar()).to_st());
//  p_pic->set_sprite_layer(-10); //background
    //p_pic->set_xy(0, 0);
    p_pic->dx_create();

}





//create a generic Cpic object
void func_EntSetLayer(CScriptPassVars *p_vars)
{
	CVisualMulti * p_pic = (CVisualMulti*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_pic)
	{
		log_error("EntSetLayer> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
  //check for a conversation element

	if (
		(p_pic->get_type() == C_THING_TEXTBOX)
		|| (p_pic->get_type() == C_THING_TEXTBOX_CHOICE)
		)
	{
		//special later code for textbox controls, they will set the layers of their
		//children themselves
		((CTextBox*)p_pic)->SetLayer(p_vars->a_vars[1].GetInt());
		return;
	}



	if (p_pic->get_type() == C_THING_CPIC)  
  {
	  //special layer settings for sprites

	((CPic*)p_pic)->set_sprite_layer(p_vars->a_vars[1].GetInt()); //background

  } else
  {
      //set the layer normally
	  p_pic->set_sort(p_vars->a_vars[1].GetInt()); //background
  }

}


//create a generic Cpic object
void func_CreateDialog(CScriptPassVars *p_vars)
{
	//create text box
	CTextBox *p_ps = new CTextBox();
	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, cls_tglo.p_thing_list->add_new_thing(p_ps),
		0, NULL);

	
	//set the name also
    uni uni_temp(0);	
	uni_temp.set_us(p_vars->a_vars[0].GetWChar());
	if (strlen(uni_temp.st_data) >= C_MAX_THING_NAME_LENGTH)
	{
		log_error("%s is too long a thing name. %d is the max.", uni_temp.st_data,C_MAX_THING_NAME_LENGTH );
	} else
	{
		p_ps->set_thing_name(uni_temp.st_data);
	}


	
	p_ps->dx_create();
}


void func_DialogClear(CScriptPassVars *p_vars)
{
	CTextBox * p_dialog = (CTextBox*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_dialog)
	{
		log_error("DialogClear> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}

    p_dialog->ClearText();
}

void func_DialogAdd(CScriptPassVars *p_vars)
{
	CTextBox * p_dialog = (CTextBox*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_dialog)
	{
		log_error("DialogAdd> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}

    p_dialog->AddText(p_vars->a_vars[1].GetWChar());
}

void func_DialogSet (CScriptPassVars *p_vars)
{
	CTextBox * p_dialog = (CTextBox*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_dialog)
	{
		log_error("DialogSet> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}

    p_dialog->SetText(p_vars->a_vars[1].GetWChar());
}



void func_FontSet(CScriptPassVars *p_vars)
{
	if (cls_tglo.p_font)
	{
	
	cls_tglo.p_font->SetFontName(p_vars->a_vars[0].GetInt(),
        uni(p_vars->a_vars[1].GetWChar()).to_st(), 
       uni(p_vars->a_vars[2].GetWChar()).to_st(),
	p_vars->a_vars[3].GetInt());
	} else
	{
		log_error("FontSet> Can't change font, font system not initted at all yet.");
	}
}


void func_DialogWaitForOk(CScriptPassVars *p_vars)
{

	CTextBox * p_dialog = (CTextBox*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_dialog)
	{
		log_error("DialogWaitForOk> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
 
	//tell the textbox to run the script when OK is clicked

	//tell the script to wait forever
	p_dialog->SetScriptCallbackOnOk(p_vars->i_script);
	p_vars->return_var.SetVar(C_SCRIPT_VAR_FORCE_WAIT, 200000000, 0, NULL);
}




//create a generic Cpic object
void func_CreateDialogChoice(CScriptPassVars *p_vars)
{
	//create text box
	CTextChoice*p_ps = new CTextChoice();
	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, cls_tglo.p_thing_list->add_new_thing(p_ps),
		0, NULL);

		//set the name also
    uni uni_temp(0);	
	uni_temp.set_us(p_vars->a_vars[0].GetWChar());
	if (strlen(uni_temp.st_data) >= C_MAX_THING_NAME_LENGTH)
	{
		log_error("%s is too long a thing name. %d is the max.", uni_temp.st_data,C_MAX_THING_NAME_LENGTH );
	} else
	{
		p_ps->set_thing_name(uni_temp.st_data);
	}

	
	p_ps->dx_create();
}

//create a generic Cpic object
void func_CreateConversation(CScriptPassVars *p_vars)
{
	//create text box
	CConversation*p_ps = new CConversation();
	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, cls_tglo.p_thing_list->add_new_thing(p_ps),
		0, NULL);

     uni uni_temp(0);	
	uni_temp.set_us(p_vars->a_vars[0].GetWChar());
	if (strlen(uni_temp.st_data) >= C_MAX_THING_NAME_LENGTH)
	{
		log_error("%s is too long a thing name. %d is the max.", uni_temp.st_data,C_MAX_THING_NAME_LENGTH );
	} else
	{
		p_ps->set_thing_name(uni_temp.st_data);
	}



}

void func_ConversationGetDialog(CScriptPassVars *p_vars)
{

	CConversation *p_con  = (CConversation*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_con)
	{
		log_error("ConversationGetDialog> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}

	
	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_con->GetDialogId(), 0, NULL );
}


void func_ConversationGetChoice(CScriptPassVars *p_vars)
{

	CConversation *p_con  = (CConversation*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_con)
	{
		log_error("ConversationGetChoice> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
	
	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_con->GetChoiceId(), 0, NULL );
}

void func_DialogChoiceAdd(CScriptPassVars *p_vars)
{

	CTextChoice *p_choice  = (CTextChoice*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_choice)
	{
		log_error("DialogChoiceAdd> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
	p_choice->AddChoice(p_vars->a_vars[1].GetInt(), p_vars->a_vars[2].GetWChar());
	 
}

void func_ConversationSetPic(CScriptPassVars *p_vars)
{

	CConversation *p_con  = (CConversation*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_con)
	{
		log_error("ConversationSetPic> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
	p_con->SetPanelPic(uni(p_vars->a_vars[1].GetWChar()).to_st());
	 
}

void func_DialogChoiceWait(CScriptPassVars *p_vars)
{

	CTextChoice *p_choice  = (CTextChoice*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_choice)
	{
		log_error("DialogChoiceWait> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
	p_choice->SetScriptCallbackOnChoice(p_vars->i_script);
	p_vars->return_var.SetVar(C_SCRIPT_VAR_FORCE_WAIT, 200000000, 0, NULL);
 
}

void func_DialogChoiceClear(CScriptPassVars *p_vars)
{
	CTextChoice * p_dialog = (CTextChoice*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_dialog)
	{
		log_error("DialogChoiceClear> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}

    p_dialog->ClearChoices();
}


//Note, 1 means the first option, 2 means the second. (1 based, not 0!!)  This has nothing to do with what you set the return code
//to be when setting up the choices.  (this seems wrongly design to me now, but I can't change it, it's used
//in many places.  Perhaps add another way later to set by return id we'd given the choices...

void func_DialogChoiceSetCurrent (CScriptPassVars *p_vars)
{
		CTextChoice * p_dialog = (CTextChoice*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_dialog)
	{
		log_error("DialogChoiceSetCurrent> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}

    p_dialog->SetCurrentSelection(p_vars->a_vars[1].GetInt());

}

  

//make an ent and run a custom script.  Doesn't have to be a visual, could be anything.

void func_CreateEntScript(CScriptPassVars *p_vars)
{
	CEntItem * p_script = new CEntItem();
	cls_tglo.p_thing_list->add_new_thing(p_script);
	p_script->InitScript(uni(p_vars->a_vars[1].GetWChar()).to_st());

	//set the name also
    uni uni_temp(0);	
	uni_temp.set_us(p_vars->a_vars[0].GetWChar());
	if (strlen(uni_temp.st_data) >= C_MAX_THING_NAME_LENGTH)
	{
		log_error("%s is too long a thing name. %d is the max.", uni_temp.st_data,C_MAX_THING_NAME_LENGTH );
	} else
	{
		p_script->set_thing_name(uni_temp.st_data);
	}

	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_script->get_id(), 0, NULL );
}	




void func_ZoneLoad(CScriptPassVars *p_vars)
{

	if (cls_tglo.p_map)
	{
	
		if (uni(p_vars->a_vars[0].GetWChar()).to_st()[0] == 0)
		{
			//they tried to load a blank map?  What the?  Ok, let's blank out the zone.
           //let's also clear all texture references
           cls_tglo.p_manager->UnloadAllNonDynamic(C_RESOURCE_TEXTURE);
			cls_tglo.p_map->generate_map(1,1,5,3);
			cls_tglo.p_map->send_map_to_terrain();
	    } else
		{
		

#ifdef _DEMO
//make sure it's one of the four approved levels
            
            char st_crap[256];
            strcpy(st_crap, uni(p_vars->a_vars[0].GetWChar()).to_st());
            static int i_ok;
            i_ok = 0;

#ifdef _TLM_

            if (stricmp(st_crap, "data\\graveyard.zon") == 0) i_ok = 1; 
            if (stricmp(st_crap, "data\\lawn.zon") == 0) i_ok = 1; 
            if (stricmp(st_crap, "data\\park.zon") == 0) i_ok = 1; 
            if (stricmp(st_crap, "data\\rollingrocks.zon") == 0) i_ok = 1; 
#endif           
            if (i_ok == 0)
            {
                //can't load this
                return;
            }
#endif

		cls_tglo.p_map->load_zone(uni(p_vars->a_vars[0].GetWChar()).to_st());
			//clear_objects_from_map(&cls_things, ::C_THING_ENT_GENERIC);
			load_objects_for_map(cls_tglo.p_thing_list, uni(p_vars->a_vars[0].GetWChar()).to_st());
		}
	}	else
	{
		log_error("Can't load zone, map not initted.");
	}
	

}

void func_CreateEntWeather(CScriptPassVars *p_vars)
{

    assert(cls_tglo.p_thing_list->get_thing_by_name("Weather") == NULL && "Weather was already initted somewhere.");
    
    CWeather * p_sky = new CWeather();
	cls_tglo.p_thing_list->add_new_thing(p_sky);

	//set the name also
    uni uni_temp(0);	
	uni_temp.set_us(p_vars->a_vars[0].GetWChar());
	if (strlen(uni_temp.st_data) >= C_MAX_THING_NAME_LENGTH)
	{
		log_error("%s is too long a thing name. %d is the max.", uni_temp.st_data,C_MAX_THING_NAME_LENGTH );
	} else
	{
		p_sky->set_thing_name(uni_temp.st_data);
		p_sky->Init();
	}

	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT,  p_sky->get_id(), 0, NULL );
}
	 


void func_PostQuitMessage(CScriptPassVars *p_vars)
{
  		PostMessage(cls_tglo.p_d3dapp->m_hWnd, WM_CLOSE, 0,0);
		
}

 void func_SetPauseLevel(CScriptPassVars *p_vars)
{
  //set a pause level command
  cls_tglo.SetPauseLevel(p_vars->a_vars[0].GetInt());
}



void func_GetPauseLevel(CScriptPassVars *p_vars)
{
  //set a pause level command
  	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT,  cls_tglo.GetPauseLevel(), 0, NULL );

}


void func_EntSetPauseLevel(CScriptPassVars *p_vars)
{
    if (cls_tglo.p_thing_list->b_deleting_list) return; 
    
    CThing *p_thing  = cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_thing)
	{
		log_error("EntSetPauseLevel> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}

	p_thing->SetPauseLevel(p_vars->a_vars[1].GetInt());
}


void func_EntGetPauseLevel(CScriptPassVars *p_vars)
{
	CThing *p_thing  = cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_thing)
	{
		log_error("EntGetPauseLevel> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}

	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_thing->GetPauseLevel(), 0, NULL );
}  


void func_CreateEntMouse(CScriptPassVars *p_vars)
{
	//create text box
	CMouse *p_mouse = new CMouse();
	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, cls_tglo.p_thing_list->add_new_thing(p_mouse),
		0, NULL);

	
	//set the name also
    uni uni_temp(0);	
	uni_temp.set_us(p_vars->a_vars[0].GetWChar());
	if (strlen(uni_temp.st_data) >= C_MAX_THING_NAME_LENGTH)
	{
		log_error("%s is too long a thing name. %d is the max.", uni_temp.st_data,C_MAX_THING_NAME_LENGTH );
	} else
	{
		p_mouse->set_thing_name(uni_temp.st_data);
	}


	
	p_mouse->dx_create();
}


void func_EntSetIgnoreKill(CScriptPassVars *p_vars)
{
	CVisualMulti *p_thing  = (CVisualMulti*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_thing)
	{
		log_error("EntSetIgnoreKill> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}

	p_thing->SetIgnoreKillMessage (p_vars->a_vars[1].GetInt()!=0);
}

void func_EntSetXY(CScriptPassVars *p_vars)
{
	CVisualMulti *p_thing  = (CVisualMulti*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_thing)
	{
		log_error("EntSetXY> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
    if ( (p_thing->get_type() == C_THING_TEXTBOX)
        || (p_thing->get_type() == C_THING_TEXTBOX_CHOICE))
    {
      //uh oh. You can't do this.  But with this hack you can!


	((CTextBox*)p_thing)->set_pos_x(p_vars->a_vars[1].GetFloat());
    ((CTextBox*)p_thing)->set_pos_z(p_vars->a_vars[2].GetFloat());
     ((CTextBox*)p_thing)->UpdatePositions();


    }   else
    {
    
	p_thing->set_xy(p_vars->a_vars[1].GetFloat(), p_vars->a_vars[2].GetFloat());
    }
}


void func_CreateEntButton(CScriptPassVars *p_vars)
{
	//create text box
	CPic *p_button = new CPic();
	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, cls_tglo.p_thing_list->add_new_thing(p_button),
		0, NULL);

	
	//set the name also
    uni uni_temp(0);	
	uni_temp.set_us(p_vars->a_vars[0].GetWChar());
	if (strlen(uni_temp.st_data) >= C_MAX_THING_NAME_LENGTH)
	{
		log_error("%s is too long a thing name. %d is the max.", uni_temp.st_data,C_MAX_THING_NAME_LENGTH );
	} else
	{
		p_button->set_thing_name(uni_temp.st_data);
	}


	//setup it up like a button

	p_button->InitAsButton(uni(p_vars->a_vars[1].GetWChar()).to_st(),
		uni(p_vars->a_vars[2].GetWChar()).to_st());
	p_button->set_clickable(true);
	//set default click sound...

	p_button->SetClickSound("sound\\click.wav");
	
	uni uni_script(0);
	uni_script.set_us(p_vars->a_vars[3].GetWChar());

	//set a click script?
	if (uni_script.st_data[0] != 0)
	{
		p_button->SetClickScript(uni_script.st_data);
	}

	p_button->dx_create();
}


void func_EntSetButtonHighlight(CScriptPassVars *p_vars)
{
    CPic *p_thing  = (CPic*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
    if (!p_thing)
    {
        log_error("EntSetButtonHighlight> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
        return;
    }
    
    p_thing->SetButtonHighlight(uni(p_vars->a_vars[1].GetWChar()).to_st());
    
}

void func_EntSetButtonHighlightSound(CScriptPassVars *p_vars)
{
    CPic *p_thing  = (CPic*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
    if (!p_thing)
    {
        log_error("EntSetButtonHighlightSound> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
        return;
    }
    
    p_thing->SetHighlightSound(uni(p_vars->a_vars[1].GetWChar()).to_st());
    
}

void func_EntKillAllPics(CScriptPassVars *p_vars)
{
  clear_objects_from_map(cls_tglo.p_thing_list, ::C_THING_CPIC);
}


void func_EntReplaceTexture(CScriptPassVars *p_vars)
{
	//create text box
	CVisualMulti *p_thing  = (CVisualMulti*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_thing)
	{
		log_error("EntReplaceTexture> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}


	//let's issue the replace texture command
	p_thing->ReplaceTexture(uni(p_vars->a_vars[1].GetWChar()).to_st(), 
		uni(p_vars->a_vars[2].GetWChar()).to_st(), uni(p_vars->a_vars[3].GetWChar()).to_st());


}


void func_EntSetDirection(CScriptPassVars *p_vars)
{
	CVisual *p_thing  = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_thing)
	{
		log_error("EntSetDirection> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
    if (p_thing->get_type() == C_THING_CAMERA)
	{
	  //special handling for the camera
		((CCamera*)p_thing)->setAngleY(p_vars->a_vars[1].GetFloat());
			((CCamera*)p_thing)->SetTargetDirection(p_vars->a_vars[1].GetFloat());

	}	else
	{
	//turn the real direction
	((CEnt*)p_thing)->set_dir_in_rad(p_vars->a_vars[1].GetFloat());
	//turn our desired direction.  Without doing this it will just snap back.
	((CEnt*)p_thing)->set_direction_rad(p_vars->a_vars[1].GetFloat());
	}
} 


void func_EntGetDirection(CScriptPassVars *p_vars)
{
	CVisual *p_thing  = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_thing)
	{
		log_error("EntGetDirection> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}

    //do I need this here?  not sure
	/*
	if (p_thing->get_type() == C_THING_CAMERA)
	{
	  //special handling for the camera
		((CCamera*)p_thing)->setAngleY(p_vars->a_vars[1].GetFloat());
			((CCamera*)p_thing)->SetTargetDirection(p_vars->a_vars[1].GetFloat());

	}	else
	  */
	{
	  float f_return = p_thing->get_dir_in_rad();
	p_vars->return_var.SetVar(C_SCRIPT_VAR_FLOAT, *(int*) &f_return,0, NULL);
	}
} 


void func_EntSetTilt(CScriptPassVars *p_vars)
{
	CVisual *p_thing  = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_thing)
	{
		log_error("EntSetTilt> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
       if (p_thing->get_type() == C_THING_CAMERA)
	{
	  //special handling for the camera
		((CCamera*)p_thing)->setAngleX(p_vars->a_vars[1].GetFloat());
 		((CCamera*)p_thing)->SetTargetTilt(p_vars->a_vars[1].GetFloat());
           p_thing->set_tilt(p_vars->a_vars[1].GetFloat());
	   }	else
	{

	       
           p_thing->set_tilt(p_vars->a_vars[1].GetFloat());
	}
}  


    
void func_EntGetTilt(CScriptPassVars *p_vars)
{
	  float f_return;
	CVisual *p_thing  = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_thing)
	{
		log_error("EntGetTilt> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
       if (p_thing->get_type() == C_THING_CAMERA)
	{
	  //special handling for the camera
	       //why do I need this *-1?  maybe I did something weird with the camera
           //so it ended up tilting the opposite of normal Ent's...
           f_return = ((CCamera*)p_thing)->get_tilt()*-1;
    	p_vars->return_var.SetVar(C_SCRIPT_VAR_FLOAT, *(int*) &f_return,0, NULL);
  
       }	else
	{
       f_return = p_thing->get_tilt();
    	p_vars->return_var.SetVar(C_SCRIPT_VAR_FLOAT, *(int*) &f_return,0, NULL);
	}
}  




void func_EntSetTargetTilt(CScriptPassVars *p_vars)
{
    CVisual *p_thing  = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
    if (!p_thing)
    {
        log_error("EntSetTargetTilt> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
        return;
    }
    if (p_thing->get_type() == C_THING_CAMERA)
    {
        //special handling for the camera
        ((CCamera*)p_thing)->SetTargetTilt(p_vars->a_vars[1].GetFloat());
    }	else
        if (p_thing->get_type() == C_THING_ENT_GENERIC)
        {
         ((CEnt*)p_thing)->set_tilt_rad(p_vars->a_vars[1].GetFloat());
            
        }
        else
        {
            log_error("EntSetTargetTilt> Don't know how to tilt this object.");
            //p_thing->set_tilt(p_vars->a_vars[1].GetFloat());
        }
}  

 void func_EntSetTargetDirection(CScriptPassVars *p_vars)
{
	CVisual *p_thing  = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_thing)
	{
		log_error("EntSetTargetDirection> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
       if (p_thing->get_type() == C_THING_CAMERA)
	{
	  //special handling for the camera
		((CCamera*)p_thing)->SetTargetDirection(p_vars->a_vars[1].GetFloat());
	}
	   
	   else if (p_thing->get_type() == C_THING_ENT_GENERIC)
	   {
		((CEnt*)p_thing)->set_direction_rad(p_vars->a_vars[1].GetFloat());
	    

	   } 

	else 
	{
	  log_error("EntSetTargetDirection> Don't know how to turn this object.");
	//p_thing->set_tilt(p_vars->a_vars[1].GetFloat());
	}
} 
 

 void func_EntSetTargetPosition(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CVisual * p_temp = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntSetTargetPosition> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.
      if (p_temp->get_type() == C_THING_CAMERA)
	{
	  //special handling for the camera
		((CCamera*)p_temp)->SetTargetPosition(p_vars->a_vars[1].GetVector3());
	}	else
	{
	  log_error("EntSetTargetPosition> Don't know how to set this object.");
	//p_thing->set_tilt(p_vars->a_vars[1].GetFloat());
	}

}

void func_EntWeatherFlash(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CWeather * p_temp = (CWeather*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntWeatherFlash> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
    p_temp->SetRandomFlash(p_vars->a_vars[1].GetInt());

}


void func_EntWeatherFade(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CWeather * p_temp = (CWeather*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntWeatherFade> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
    p_temp->LoadAndFadeToWeather(uni(p_vars->a_vars[1].GetWChar()).to_st(), p_vars->a_vars[2].GetInt(),
        p_vars->a_vars[3].GetFloat());
}

  void func_BindKeyToScript(CScriptPassVars *p_vars)
{
 cls_tglo.p_cls_input->AddAliasBindScript(uni(p_vars->a_vars[0].GetWChar()).to_st(), uni(p_vars->a_vars[1].GetWChar()).to_st(), p_vars->a_vars[2].GetInt());
}


void func_Sound3DPlay(CScriptPassVars *p_vars)
{

   //Create a 3d sound entity to handle this sound, it will autodie when the sound is no more
   CThingSound *p_sound = new CThingSound();
   cls_tglo.p_thing_list->add_new_thing(p_sound);
  

   //we're actually passing an unsigned int to an int but hey, we don't want to add a new type to our
   //scripting system so this will just have to be good enough.
    hSoundInstance h_sound;

 	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 
    h_sound = p_sound->Init3DSound(p_vars->a_vars[0].GetVector3(), uni(p_vars->a_vars[1].GetWChar()).to_st())
    , 0, NULL );
   	if (h_sound == C_SOUND_NONE)
    {
        //failed to load sound.  something is wrong. kill it
        p_sound->b_delete_flag = true;
        return;
    }
    
    
  p_sound->SetDistance(25, 0); //always hear it loud at medium range
	//actually play it
	p_sound->Play(p_vars->a_vars[2].GetInt());

}

void func_SoundEntPlay(CScriptPassVars *p_vars)
{

    if (!cls_tglo.p_sound) return; //sound not initted, probably in the editor
    
    CVisual * p_temp = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("SoundEntPlay> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}

   //Create a 3d sound entity to handle this sound, it will autodie when the sound is no more
   CThingSound *p_sound = new CThingSound();
   cls_tglo.p_thing_list->add_new_thing(p_sound);
  

   //we're actually passing an unsigned int to an int but hey, we don't want to add a new type to our
   //scripting system so this will just have to be good enough.

    int h_sound = p_sound->Init3DSound(&p_temp->getObjectPosition(), uni(p_vars->a_vars[1].GetWChar()).to_st());
	//actually play it
	p_sound->SetDistance(25, 0); //always hear it loud at medium range
	p_sound->Play(1);
	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, h_sound, 0, NULL );

  

}


void func_SoundMusicStop(CScriptPassVars *p_vars)
{
   if (cls_tglo.p_sound)
   {
	   cls_tglo.p_sound->StopMusic();
   }
  
}


void func_SoundKill(CScriptPassVars *p_vars)
{
   if (cls_tglo.p_sound)
   {
	  // cls_tglo.p_sound->SoundInstanceStop(p_vars->a_vars[0].GetInt());


       cls_tglo.p_sound->SoundInstanceKill(p_vars->a_vars[0].GetInt());
   }
  
}



void func_SoundClearCache(CScriptPassVars *p_vars)
{
   if (cls_tglo.p_sound)
   {
	 
       cls_tglo.p_sound->ClearCache();
   }
  
}


void func_ShowMessage(CScriptPassVars *p_vars)
{
   
    CMessage *p_mess = (CMessage*)cls_tglo.p_thing_list->get_thing_by_name("CMESSAGE");

    if (!p_mess)
    {
        log_error("CMessage handler not initted.  This needs to be done in the program, not the script.");
        return;
    }

   p_mess->SetMessageByText(p_vars->a_vars[0].GetWChar());
}



void func_SoundPreload(CScriptPassVars *p_vars)
{
    
   GetApp()->p_sound->PreloadSound(uni(p_vars->a_vars[0].GetWChar()).st_data);
}


void func_Sound3DPreload(CScriptPassVars *p_vars)
{
    
    GetApp()->p_sound->Preload3DSound(uni(p_vars->a_vars[0].GetWChar()).st_data);
}

void func_DialogSetStyleDefault(CScriptPassVars *p_vars)
{
	//create text box
	CTextBox *p_text = (CTextBox*) cls_tglo.p_thing_list->get_thing_by_id(p_vars->a_vars[0].GetInt());

	if (!p_text)
	{
	  log_error("DialogSetStyleDefault got a bad id.");
	  return;
	}

	D3DXVECTOR2 pt_pos;

	pt_pos.x = 144;
	pt_pos.y = 188;

	//adjust for screensize
	AdjustPointPosition(&pt_pos, 800, 600);

  p_text->set_pos_x(pt_pos.x);
  p_text->set_pos_y(pt_pos.y);

  
  // p_text->SetFontColor(D3DCOLOR_ARGB(255,255,255,0));
  //setup how we want the font to act

  //size of the text surface, follow ^2 limitation.
  D3DXVECTOR2 pt_size = D3DXVECTOR2(512, 256);

  //offset of our text as comparing to the main background
  D3DXVECTOR2 pt_offset = D3DXVECTOR2(20,15);

  //if we don't want to use the whole text surface, set this to limit it
  //D3DXVECTOR2 pt_limit = D3DXVECTOR2(512, 256);
  D3DXVECTOR2 pt_limit = D3DXVECTOR2(440, 200);

  p_text->SetFontId(2);
  p_text->InitTextBox(pt_size, pt_offset, pt_limit);
  p_text->InitBG("interface\\textbox.dds");
  
  //p_text->InitBGBlank(pt_size, D3DCOLOR_ARGB(140,0,0,0));

  //let's also add up and down scroll buttons

  p_text->AddUpScroll(D3DXVECTOR2(462, 17), "interface\\arrow_up.dds", "interface\\arrow_up_2.dds");
  p_text->AddDownScroll(D3DXVECTOR2(462, 180), "interface\\arrow_down.dds", "interface\\arrow_down_2.dds");

  //we'll add the OK button, but note that it may or may not actually get used, depending
  p_text->AddOkButton(D3DXVECTOR2(216, 165), "interface\\dialog_ok.dds", "interface\\dialog_ok_2.dds");

  //that's fine and dandy.  But should we also set a click sound effect? if it's
  //a choice box then yes.

  if (p_text->get_type() == C_THING_TEXTBOX_CHOICE)
  {
	  //yes it is.
//	  ((CTextChoice*)p_text)->SetClickSound("sound\\click.wav");

  }
   p_text->UpdatePositions();

  p_text->dx_create();

}




