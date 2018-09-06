/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "Script_utils_ent.h"
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
#include "CThing_util.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void func_EntSetBlink(CScriptPassVars *p_vars);


void func_EntFace(CScriptPassVars *p_vars);
void func_EntFaceAway(CScriptPassVars *p_vars);
void func_EntDistance(CScriptPassVars *p_vars);
void func_EntSetResAdjust(CScriptPassVars *p_vars);
void func_EntSetDefaultResAdjust(CScriptPassVars *p_vars);

void func_EntSetFastText(CScriptPassVars *p_vars);
void func_EntSetPicTimer(CScriptPassVars *p_vars);
void func_EntGetPicTime(CScriptPassVars *p_vars);
void func_EntWeatherLoad(CScriptPassVars *p_vars);
void func_WaitForPicTimer(CScriptPassVars *p_vars);
void func_EntRunFunction(CScriptPassVars *p_vars);
void func_CameraSetFollow(CScriptPassVars *p_vars);
void func_EntKillAllItems(CScriptPassVars *p_vars);
void func_EntKillByName(CScriptPassVars *p_vars);
void func_IsPositionValid(CScriptPassVars *p_vars);

void func_AddFastFont(CScriptPassVars *p_vars);
void func_EntSetPicTextColor(CScriptPassVars *p_vars);
void func_EntSetRadiusMod(CScriptPassVars *p_vars);
void func_EntSetSphereTreeMod(CScriptPassVars *p_vars);

void func_EntSetSmartFade(CScriptPassVars *p_vars);
void func_EntSetAnimTexture(CScriptPassVars *p_vars);
void func_EntSetSpeed(CScriptPassVars *p_vars);
void func_EntGetSpeed(CScriptPassVars *p_vars);
void func_EntSetMaxSpeed(CScriptPassVars *p_vars);
void func_EntGetMaxSpeed(CScriptPassVars *p_vars);

void func_EntSetPicTime(CScriptPassVars *p_vars);
void func_EntSetColor(CScriptPassVars *p_vars);
void func_EntSetColorFade(CScriptPassVars *p_vars);
void func_EntSetTurnSpeed(CScriptPassVars *p_vars);
void func_EntSetAnimByName(CScriptPassVars *p_vars);
void func_EntSetModel(CScriptPassVars *p_vars);
void func_EntRotateBaseFrameYX(CScriptPassVars *p_vars);
void func_EntSetAnimSpeedMod(CScriptPassVars *p_vars);
void func_EntSetDrawOffset(CScriptPassVars *p_vars);
void func_GetHost(CScriptPassVars *p_vars);
void func_EntSetName(CScriptPassVars *p_vars);
void func_EntGetName(CScriptPassVars *p_vars);
void func_EntSetWallBounce(CScriptPassVars *p_vars);
void func_CreateScriptFunction(CScriptPassVars *p_vars);
void func_CameraSetLerp(CScriptPassVars *p_vars);
void func_CameraSetDirLerp(CScriptPassVars *p_vars);
void func_EntForceMod(CScriptPassVars *p_vars);
void func_EntForceGet(CScriptPassVars *p_vars);
void func_EntForceSet(CScriptPassVars *p_vars);
void func_GetGroundHeight(CScriptPassVars *p_vars);
void func_EntSetSpecial(CScriptPassVars *p_vars);
void func_EntSetCollisionType(CScriptPassVars *p_vars);
void func_EntSetBounceSound(CScriptPassVars *p_vars);
void func_EntSetAcceleration(CScriptPassVars *p_vars);
void func_EntVelocityGet(CScriptPassVars *p_vars);
void func_EntPicRenderText(CScriptPassVars *p_vars);
void func_EntSetGravityType(CScriptPassVars *p_vars);
void func_EntSetForceType(CScriptPassVars *p_vars);
void func_SetOnTouchGround(CScriptPassVars *p_vars);
void func_SetOnTouchWall(CScriptPassVars *p_vars);
void func_EntSetHardness(CScriptPassVars *p_vars);
void func_EntSetBroadcastType(CScriptPassVars *p_vars);
void func_EntSetTouchDamage(CScriptPassVars *p_vars);
void func_SetOnTouchCeiling(CScriptPassVars *p_vars);
void func_SetOnTouchAll(CScriptPassVars *p_vars);
void func_EntSetMovement(CScriptPassVars *p_vars); //current movement in vector form (without added force)
void func_EntGetMovement(CScriptPassVars *p_vars); //current movement in vector form (without added force)
void func_EntSetCentering(CScriptPassVars *p_vars);
void func_EntSetPicVisualEffect(CScriptPassVars *p_vars);
void func_EntSetDrawTwoSided(CScriptPassVars *p_vars);

void SetupScriptEntStuff()
{
	CScriptPassVars *p_vars;

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent to do this to
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //ent of who they should face
	
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntFace", func_EntFace, p_vars);


	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent to do this to
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //ent of who they should face
	
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntFaceAway", func_EntFaceAway, p_vars);


	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent to do this to
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //ent of who they should face
	p_vars->return_var.SetType(C_SCRIPT_VAR_FLOAT);
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntDistance", func_EntDistance, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent to do this to
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //erad mod, 1.0 for none
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetRadiusMod", func_EntSetRadiusMod, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent to do this to
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //erad mod, 1.0 for none
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetSphereTreeMod", func_EntSetSphereTreeMod, p_vars);



    	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_FLOAT); //x
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //z
	p_vars->return_var.SetType(C_SCRIPT_VAR_FLOAT);
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("GetGroundHeight", func_GetGroundHeight, p_vars);



	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //type
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT); //base x
	p_vars->a_vars[3].SetType(C_SCRIPT_VAR_INT); //base y
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetResAdjust", func_EntSetResAdjust, p_vars);


    //like above, but uses the game's default x/y.  the reason I prefer to use this is
    //so I can change the menu for a game without touching all the scripts
    
    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //type
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntDefaultResAdjust", func_EntSetDefaultResAdjust, p_vars);
    
	//only works with CPic's
	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //font index
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_WCHAR); //msg
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetFastText", func_EntSetFastText, p_vars);

   	//only works with CPic's
	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //starting num
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_FLOAT); //change
	p_vars->a_vars[3].SetType(C_SCRIPT_VAR_INT); //milliseconds between doing changes/update
	p_vars->a_vars[4].SetType(C_SCRIPT_VAR_INT); //mode.  1 normal, 2 for clock
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetPicTimer", func_EntSetPicTimer, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_FORCE_WAIT);  //reserved?
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of cpic with timer going
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //call when this gets hit
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("WaitForPicTimer", func_WaitForPicTimer, p_vars);


	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of cpic with timer going
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CameraSetFollow", func_CameraSetFollow, p_vars);

   	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_FLOAT); //id of cpic with timer going
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CameraSetLerp", func_CameraSetLerp, p_vars);

   	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_FLOAT); //id of cpic with timer going
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CameraSetDirLerp", func_CameraSetDirLerp, p_vars);

	p_vars = new CScriptPassVars();
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntKillAllItems", func_EntKillAllItems, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of cpic with timer going
	p_vars->return_var.SetType(C_SCRIPT_VAR_FLOAT);	   //time
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntGetPicTime", func_EntGetPicTime, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of cpic with timer going
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //time
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetPicTime", func_EntSetPicTime, p_vars);


   	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //ent id of who to kill
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntKillByName", func_EntKillByName, p_vars);
    
    p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id 
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //ent id 
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_FLOAT); //ent id 
	p_vars->a_vars[3].SetType(C_SCRIPT_VAR_FLOAT); //ent id 
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntForceMod", func_EntForceMod, p_vars);


    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id 
	p_vars->return_var.SetType(C_SCRIPT_VAR_VECTOR3); //ent id 
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntForceGet", func_EntForceGet, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id 
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_VECTOR3); //ent id 
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntForceSet", func_EntForceSet, p_vars);


    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id 
	p_vars->return_var.SetType(C_SCRIPT_VAR_VECTOR3); //ent id 
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntGetMovement", func_EntGetMovement, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id 
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_VECTOR3); //ent id 
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetMovement", func_EntSetMovement, p_vars);



    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id 
	p_vars->return_var.SetType(C_SCRIPT_VAR_FLOAT); //current overall speed.  (dir is not given)
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntVelocityGet", func_EntVelocityGet, p_vars);
    
     
    p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //name of func

	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntRunFunction", func_EntRunFunction, p_vars);
 
    
        p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //script filename
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //function name
 	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //return of int, probably invalid though as the script will kill itself

	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CreateScriptFunction", func_CreateScriptFunction, p_vars);
 

         p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_VECTOR3); //position to check, y is ignored
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //zone name. ALL for don't check, null for only non-zones ok.
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT); //If 1, block must not be hard.
 	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //1 is area is ok

	cls_tglo.p_script_engine->GetScript(0)->AddFunction("IsPositionValid", func_IsPositionValid, p_vars);
 


    p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //font name
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //font filename or "" for none


    p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT); //size of font
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //font index

	cls_tglo.p_script_engine->GetScript(0)->AddFunction("AddFastFont", func_AddFastFont, p_vars);

	
	
	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of pic based ent
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //r
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT); //g
	p_vars->a_vars[3].SetType(C_SCRIPT_VAR_INT); //b
	p_vars->a_vars[4].SetType(C_SCRIPT_VAR_INT); //alpha

	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetPicTextColor", func_EntSetPicTextColor, p_vars);


	
	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //font name
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //size of font

	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntWeatherLoad", func_EntWeatherLoad, p_vars);


	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of pic based ent
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //r
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT); //g
	p_vars->a_vars[3].SetType(C_SCRIPT_VAR_INT); //b
	p_vars->a_vars[4].SetType(C_SCRIPT_VAR_INT); //alpha

	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetColor", func_EntSetColor, p_vars);

    p_vars = new CScriptPassVars();
    
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of pic based ent
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //r
    p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT); //g
    p_vars->a_vars[3].SetType(C_SCRIPT_VAR_INT); //b
    p_vars->a_vars[4].SetType(C_SCRIPT_VAR_INT); //alpha
    p_vars->a_vars[5].SetType(C_SCRIPT_VAR_INT); //speed
    
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetColorFade", func_EntSetColorFade, p_vars);


	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //modifer for close fade
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_FLOAT); //mod for far fade
	
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetSmartFade", func_EntSetSmartFade, p_vars);

  	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //texture name
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetAnimTexture", func_EntSetAnimTexture, p_vars);

  	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //texture name
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetSpeed", func_EntSetSpeed, p_vars);

    p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->return_var.SetType(C_SCRIPT_VAR_FLOAT); //texture name
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntGetSpeed", func_EntGetSpeed, p_vars);

   	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //0 for box, 1 for spherical.. box actually uses sphere collision now.
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetCollisionType", func_EntSetCollisionType, p_vars);

   	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //id of special parm, 0 for ROLL
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_FLOAT); //amount to set it.  1 is normal, 0 is disabled.
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetSpecial", func_EntSetSpecial, p_vars);

  


    p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //texture name
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetMaxSpeed", func_EntSetMaxSpeed, p_vars);


      
    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id 
	p_vars->return_var.SetType(C_SCRIPT_VAR_FLOAT); //max speed we're allowed to go
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntGetMaxSpeed", func_EntGetMaxSpeed, p_vars);
 

	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //texture name
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetTurnSpeed", func_EntSetTurnSpeed, p_vars);
 

 	p_vars = new CScriptPassVars();
 	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //anim name (must be < 12!)
  	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT); //id of frame they want, or -1 to "smartset"
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetAnimByName", func_EntSetAnimByName, p_vars);

   	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //model name
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetModel", func_EntSetModel, p_vars);


 	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //Y rotation
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_FLOAT); //X rotation
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntRotateBaseFrameYX", func_EntRotateBaseFrameYX, p_vars);

   	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //speed, 1 is no change
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetAnimSpeedMod", func_EntSetAnimSpeedMod, p_vars);


   	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_VECTOR3); //offset
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetDrawOffset", func_EntSetDrawOffset, p_vars);

   	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //accel speed. 0.3 is normal?
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetSkid", func_EntSetAcceleration, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //gravity type
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_FLOAT); //amount, will be the new gravity or a multiplier for the world grav (or not used)
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetGravityType", func_EntSetGravityType, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //force type
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetForceType", func_EntSetForceType, p_vars);



   	p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); 
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("GetHost", func_GetHost, p_vars);

 	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //name
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetName", func_EntSetName, p_vars);

 	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR); //name
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntGetName", func_EntGetName, p_vars);


    	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of int
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //texture name
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetWallBounce", func_EntSetWallBounce, p_vars);

     	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of ent
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //filename
	
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_FLOAT); //bounce strength required to play sound
	p_vars->a_vars[3].SetType(C_SCRIPT_VAR_INT); //minimum time between playing bounce sounds
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetBounceSound", func_EntSetBounceSound, p_vars);


    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of pic
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //id of font to use
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_WCHAR); //strng
	p_vars->a_vars[3].SetType(C_SCRIPT_VAR_INT); //x
	p_vars->a_vars[4].SetType(C_SCRIPT_VAR_INT); //y
	p_vars->a_vars[5].SetType(C_SCRIPT_VAR_INT); //clear first? yes/no

	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntPicRenderText", func_EntPicRenderText , p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //should be a bool but...
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SetOnTouchWall", func_SetOnTouchWall, p_vars);

    
        p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //should be a bool but...
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SetOnTouchCeiling", func_SetOnTouchCeiling, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //should be a bool but...
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SetOnTouchAll", func_SetOnTouchAll, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //should be a bool but...
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT); //speed of blink, not used
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetBlink", func_EntSetBlink, p_vars);


    p_vars = new CScriptPassVars();    
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //should be a bool but...
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SetOnTouchGround", func_SetOnTouchGround, p_vars);
  
    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //shardness type
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetHardness", func_EntSetHardness, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //shardness type
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetBroadcastType", func_EntSetBroadcastType, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //how much damage
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetTouchDamage", func_EntSetTouchDamage, p_vars);
   
    
    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //centering mode to use
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetCentering", func_EntSetCentering, p_vars);
   
    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //fade target (from 1 to 255)

    cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetCentering", func_EntSetCentering, p_vars);
  

    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetPicVisualEffect", func_EntSetPicVisualEffect, p_vars);
    
    p_vars = new CScriptPassVars();                 
    //adjust culling mode per entity if needed
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //true false
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetDrawTwoSided", func_EntSetDrawTwoSided, p_vars);
    

}



 void func_EntFace(CScriptPassVars *p_vars)
{
	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntFace> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   

	CVisual *p_target  = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[1].GetInt());
	if (!p_target)
	{
		log_error("EntFace> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}

	//ok, we have the two people, let's figure out the angle between them

   float f_dir = get_theta_from_x_and_z(p_target->get_pos_x()-p_ent->get_pos_x(),
   p_target->get_pos_z()-p_ent->get_pos_z() );
   p_ent->set_direction_rad(f_dir);

} 



  void func_EntFaceAway(CScriptPassVars *p_vars)
{
	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntFaceAway> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   

	CVisual *p_target  = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[1].GetInt());
	if (!p_target)
	{
		log_error("EntFaceAway> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}

	//ok, we have the two people, let's figure out the angle between them

   float f_dir = get_theta_from_x_and_z(p_target->get_pos_x()-p_ent->get_pos_x(),
   p_target->get_pos_z()-p_ent->get_pos_z() );
   p_ent->set_direction_rad(f_dir+D3DX_PI); //look away from them

} 

 void func_EntSetBounceSound(CScriptPassVars *p_vars)
{
	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetBounceSound> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
    p_ent->SetBounceSound(uni(p_vars->a_vars[1].GetWChar()).to_st(), 
         p_vars->a_vars[2].GetFloat(), 
         p_vars->a_vars[3].GetInt());
	
} 


 void func_EntDistance(CScriptPassVars *p_vars)
{
	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntDistance> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
	CVisual *p_target  = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[1].GetInt());
	if (!p_target)
	{
		log_error("EntDistance> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}

	//ok, we have the two people, let's figure out the angle between them

   float f_dist = Fast_Distance_3D(p_target->get_pos_x()-p_ent->get_pos_x(),
  p_target->get_pos_y()-p_ent->get_pos_y(),
  p_target->get_pos_z()-p_ent->get_pos_z());

   p_vars->return_var.SetVar(C_SCRIPT_VAR_FLOAT, *(int*) &f_dist, 0, NULL);

} 

 void func_GetGroundHeight(CScriptPassVars *p_vars)
{                                                                                 
   float f_dist = cls_tglo.p_terrain->get_height(p_vars->a_vars[0].GetFloat() , p_vars->a_vars[1].GetFloat());
   p_vars->return_var.SetVar(C_SCRIPT_VAR_FLOAT, *(int*) &f_dist, 0, NULL);
}


  void func_EntForceMod(CScriptPassVars *p_vars)
{
	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntForceMod> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
    //modify this ents force
    static D3DXVECTOR3 vec_temp;

    vec_temp.x = p_vars->a_vars[1].GetFloat() * cls_tglo.time();
    vec_temp.y = p_vars->a_vars[2].GetFloat()* cls_tglo.time();
    vec_temp.z = p_vars->a_vars[3].GetFloat()* cls_tglo.time();

    p_ent->ModifyAccel(&vec_temp);

} 

  void func_EntForceGet(CScriptPassVars *p_vars)
  {
      CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
      if (!p_ent)
      {
          log_error("EntForceGet> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
          return;
      }
      
      p_vars->return_var.SetVar(C_SCRIPT_VAR_VECTOR3,0,0, p_ent->GetForce());
      
  } 
  
 
   void func_EntForceSet(CScriptPassVars *p_vars)
   {
       CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
       if (!p_ent)
       {
           log_error("EntForceGet> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
           return;
       }
       p_ent->SetAccel(p_vars->a_vars[1].GetVector3());
   } 

   
   void func_EntSetMovement(CScriptPassVars *p_vars)
  {
      CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
      if (!p_ent)
      {
          log_error("EntSetMovement> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
          return;
      }
      
      p_vars->return_var.SetVar(C_SCRIPT_VAR_VECTOR3,0,0, p_ent->GetMovement());
      
  } 
  
 
   void func_EntGetMovement(CScriptPassVars *p_vars)
   {
       CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
       if (!p_ent)
       {
           log_error("EntGetMovement> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
           return;
       }
       p_ent->SetMovement(p_vars->a_vars[1].GetVector3());
   } 



      void func_EntSetForceType(CScriptPassVars *p_vars)
{
	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetForceType> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
       p_ent->SetForceType(p_vars->a_vars[1].GetInt());
} 

    void func_EntSetGravityType(CScriptPassVars *p_vars)
{
	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetGravityType> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
       p_ent->SetGravityMode(p_vars->a_vars[1].GetInt(), p_vars->a_vars[2].GetFloat());
} 
  
     void func_EntSetRadiusMod(CScriptPassVars *p_vars)
{
	CVisualMulti *p_ent = (CVisualMulti*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetRadiusMod> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
    p_ent->SetRadiusMod(p_vars->a_vars[1].GetFloat());
} 
  void func_EntSetSphereTreeMod(CScriptPassVars *p_vars)
{
	CVisualMulti *p_ent = (CVisualMulti*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetSphereTreeMod> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
       p_ent->SetSphereTreeMod(p_vars->a_vars[1].GetFloat());
} 

  void func_EntSetResAdjust(CScriptPassVars *p_vars)
  {
      CVisualMulti *p_ent  = (CVisualMulti*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
      if (!p_ent)
      {
          log_error("EntSetResAdjust> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
          return;
      }
      assert(p_ent->get_type() != C_THING_TEXTBOX && "You can't res adjust this kind of thing unless subclassed off of CVisualMulti.");
      
      p_ent->SetResAdjust(p_vars->a_vars[1].GetInt(), p_vars->a_vars[2].GetInt(), p_vars->a_vars[3].GetInt());
      
  } 

  void func_EntSetDefaultResAdjust(CScriptPassVars *p_vars)
  {
      CVisualMulti *p_ent  = (CVisualMulti*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
      if (!p_ent)
      {
          log_error("EntSetDefaultResAdjust> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
          return;
      }
      
      assert(p_ent->get_type() != C_THING_TEXTBOX && "You can't res adjust this kind of thing unless subclassed off of CVisualMulti.");
      //move the xy to cls_tglo and make an accesser if I ever need it changed

      p_ent->SetResAdjust(p_vars->a_vars[1].GetInt(), 800,600);
      
  } 

void func_EntSetFastText(CScriptPassVars *p_vars)
{
	CPic *p_ent  = (CPic*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetFastText> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
  p_ent->SetupFastTextRender(p_vars->a_vars[1].GetInt(),
	  uni(p_vars->a_vars[2].GetWChar()).to_st());
} 


 void func_EntPicRenderText(CScriptPassVars *p_vars)
  {
    	CPic *p_ent  = (CPic*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntPicRenderText> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}


    p_ent->RenderText(p_vars->a_vars[1].GetInt(),
        p_vars->a_vars[2].GetWChar(),
        p_vars->a_vars[3].GetInt(),
        p_vars->a_vars[4].GetInt(),
        p_vars->a_vars[5].GetInt() != 0);



  }



void func_EntSetPicTimer(CScriptPassVars *p_vars)
{
	CPic *p_ent  = (CPic*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetPicTimer> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
  p_ent->EntSetFastTimer( p_vars->a_vars[4].GetInt(), 
  p_vars->a_vars[1].GetFloat(), p_vars->a_vars[2].GetFloat(), p_vars->a_vars[3].GetInt());

}


void func_EntSetPicTime(CScriptPassVars *p_vars)
{
	CPic *p_ent  = (CPic*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetPicTimer> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
  p_ent->EntSetTime(p_vars->a_vars[1].GetFloat());

}


void func_EntGetPicTime(CScriptPassVars *p_vars)
{
	CPic *p_ent  = (CPic*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetPicTimer> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
    float f_return =p_ent->EntGetTime();
    p_vars->return_var.SetVar(C_SCRIPT_VAR_FLOAT, *(int*) &f_return, 0, NULL);


}


void func_EntVelocityGet(CScriptPassVars *p_vars)
{
	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntVelocityGet> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
    float f_return = D3DXVec3Length(p_ent->GetForce()); //get the length of the vector

    p_vars->return_var.SetVar(C_SCRIPT_VAR_FLOAT, *(int*) &f_return, 0, NULL);

}


void func_EntWeatherLoad(CScriptPassVars *p_vars)
{
	CWeather *p_ent  = (CWeather*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntWeatherLoadr> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
	 p_ent->LoadWeather(uni(p_vars->a_vars[1].GetWChar()).to_st());

}

void func_WaitForPicTimer(CScriptPassVars *p_vars)
{

	CPic * p_cpic = (CPic*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_cpic)
	{
		log_error("DialogWaitForOk> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
 
	//tell the textbox to run the script when OK is clicked

	//tell the script to wait forever
	p_cpic->SetCallbackOnTimer(p_vars->i_script, p_vars->a_vars[1].GetFloat());
	p_vars->return_var.SetVar(C_SCRIPT_VAR_FORCE_WAIT, 200000000, 0, NULL);
}




void func_EntRunFunction(CScriptPassVars *p_vars)
{

	CEntItem * p_cpic = (CEntItem*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_cpic)
	{
		log_error("EntRunFunction> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
    if (p_vars->a_vars[1].GetWChar()==NULL)
    {
        log_error("EntRunFunction - Cannot run %s's script, was sent NULL for the func name.", p_cpic->get_thing_name());
        return;
    }
	p_cpic->RunScriptFunction(uni(p_vars->a_vars[1].GetWChar()).to_st());
}


void func_CreateScriptFunction(CScriptPassVars *p_vars)
{

	CEntItem * p_script = new CEntItem();
	cls_tglo.p_thing_list->add_new_thing(p_script);
	if (p_script->InitScript(uni(p_vars->a_vars[0].GetWChar()).to_st()) == C_SCRIPT_NONE)
    {
        //error loading script, forget the whole thing
        p_script->b_delete_flag = true;
	    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, -1, 0, NULL );
        return;

    }

	//set the name also
	p_script->set_thing_name("FUNCTION");

	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_script->get_id(), 0, NULL );
  	p_script->RunScriptFunction(uni(p_vars->a_vars[1].GetWChar()).to_st());

}


void func_CameraSetFollow(CScriptPassVars *p_vars)
{

	if (p_vars->i_script, p_vars->a_vars[0].GetInt() == 0)
	{
		//don't want to follow anybody
		cls_tglo.p_camera->set_visual_follow(NULL);
		return;
	}

	CVisual * p_cpic = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_cpic)
	{
		log_error("CameraSetFollow> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
 
	//tell the textbox to run the script when OK is clicked
	cls_tglo.p_camera->set_visual_follow(p_cpic);
    
}

void func_CameraSetLerp(CScriptPassVars *p_vars)
{
	cls_tglo.p_camera->SetFreeMoveLerp(p_vars->a_vars[0].GetFloat());
}

void func_CameraSetDirLerp(CScriptPassVars *p_vars)
{
	cls_tglo.p_camera->SetFreeTurnLerp(p_vars->a_vars[0].GetFloat());
}


void func_EntKillAllItems(CScriptPassVars *p_vars)
{
  clear_objects_from_map(cls_tglo.p_thing_list, ::C_THING_ENT_GENERIC);
}



void func_EntKillByName(CScriptPassVars *p_vars)
{
	
    //if we are doing a final, FINAL shut down let's not let them do this search
    if (cls_tglo.p_thing_list->b_deleting_list)
    {

        return;
    }

    
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
			 p_thing->b_delete_flag = true;
			return;
			}
		}
	}
	
	//found jack and or crap

}

void func_AddFastFont(CScriptPassVars *p_vars)
{
	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 
	        cls_tglo.p_font->AddFastFont(uni(p_vars->a_vars[0].GetWChar()).to_st(),
            uni(p_vars->a_vars[1].GetWChar()).to_st() ,

            p_vars->a_vars[2].GetInt(), D3DFONT_BOLD), //standard, used internally in the engine
			0, NULL);
   //actually force the font to get created now?
//	if (cls_tglo.IsRunning()) cls_tglo.p_font->dx_create();

}
	 
void func_EntSetPicTextColor(CScriptPassVars *p_vars)
{

	CPic *p_ent  = (CPic*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetPicTextColor> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	p_ent->SetTextColor(D3DCOLOR_RGBA(
		p_vars->a_vars[1].GetInt(),
		p_vars->a_vars[2].GetInt(),
		p_vars->a_vars[3].GetInt(),
		p_vars->a_vars[4].GetInt()
		)
		);

}

void func_EntSetColor(CScriptPassVars *p_vars)
{

	CVisualMulti *p_ent  = (CVisualMulti*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetColor> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	p_ent->VertexColor(D3DCOLOR_RGBA(
		p_vars->a_vars[1].GetInt(),
		p_vars->a_vars[2].GetInt(),
		p_vars->a_vars[3].GetInt(),
		p_vars->a_vars[4].GetInt(),
		)
		);

}     

void func_EntSetColorFade(CScriptPassVars *p_vars)
{
    
    CVisualMulti *p_ent  = (CVisualMulti*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
    if (!p_ent)
    {
        log_error("EntSetColor> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
        return;
    }
   	p_ent->SetColorFadeTo(p_vars->a_vars[1].GetInt(),
        p_vars->a_vars[2].GetInt(),
        p_vars->a_vars[3].GetInt(),
        p_vars->a_vars[4].GetInt(),p_vars->a_vars[5].GetInt() );
    
}


void func_EntSetSmartFade(CScriptPassVars *p_vars)
{

	CVisualMulti *p_ent  = (CVisualMulti*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetSmartFade> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	p_ent->SetSmartFade(p_vars->a_vars[1].GetFloat(), p_vars->a_vars[2].GetFloat());

}

void func_EntSetAnimTexture(CScriptPassVars *p_vars)
{

	CVisualMulti *p_ent  = (CVisualMulti*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntMD2SetTexture> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
	p_ent->SetAnimTexture(uni(p_vars->a_vars[1].GetWChar()).to_st());
}

 void func_EntSetSpeed(CScriptPassVars *p_vars)
{

	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("func_EntSetSpeed> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
	p_ent->set_desired_velocity(p_vars->a_vars[1].GetFloat());
}

 
  void func_EntGetSpeed(CScriptPassVars *p_vars)
{

	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("func_EntGetSpeed> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
     //for more accuracy the real velocity should probably be figured from the movement vector
     float f_return =p_ent->get_desired_velocity();
     p_vars->return_var.SetVar(C_SCRIPT_VAR_FLOAT, *(int*) &f_return, 0, NULL);
}


  void func_EntGetMaxSpeed(CScriptPassVars *p_vars)
{

	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("func_EntGetMaxSpeed> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
     //for more accuracy the real velocity should probably be figured from the movement vector
     float f_return =p_ent->GetMaxSpeed();
     p_vars->return_var.SetVar(C_SCRIPT_VAR_FLOAT, *(int*) &f_return, 0, NULL);
}



 
 void func_EntSetAcceleration(CScriptPassVars *p_vars)
{

	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("func_EntSetSkid> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
	p_ent->SetAcceleration(p_vars->a_vars[1].GetFloat());
}

  void func_EntSetSpecial(CScriptPassVars *p_vars)
{

	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("func_EntSetSpecial> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	if (p_vars->a_vars[1].GetInt() == 0)
    {
        //ROLL setting
	  
        if (p_vars->a_vars[2].GetFloat() == 0)
        {
            p_ent->SetRoll(false);
        }   else
        {
            p_ent->SetRoll(true);
            p_ent->SetRollMod(p_vars->a_vars[2].GetFloat());
        }
    } else
    {
        log_error("EntSetSpecial - Type %d is an unknown property.  Right now there is only type 0 for roll.",
            p_vars->a_vars[1].GetInt());
    }
}



void func_EntSetMaxSpeed(CScriptPassVars *p_vars)
{

	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("func_EntSetMaxSpeed> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
	p_ent->SetMaxSpeed(p_vars->a_vars[1].GetFloat());
}
	
 void func_EntSetTurnSpeed(CScriptPassVars *p_vars)
{

	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetTurnSpeed> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
	p_ent->SetTurnLerp(p_vars->a_vars[1].GetFloat());
}

void func_EntSetAnimByName(CScriptPassVars *p_vars)
{

	CVisualMulti *p_ent  = (CVisualMulti*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetAnimByName> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
	p_ent->SetAnimByName(uni(p_vars->a_vars[1].GetWChar()).to_st(), p_vars->a_vars[2].GetInt());
}


void func_EntSetModel(CScriptPassVars *p_vars)
{

	CVisualMulti *p_ent  = (CVisualMulti*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetModel> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
	uni u_model(p_vars->a_vars[1].GetWChar());

		if  (file_extension_is(u_model.st_data, ".x")) 
	{
		p_ent->init_visual(u_model.st_data,C_RESOURCE_PMESH);
	}	else
   if  (file_extension_is(u_model.st_data, ".md2")) 
	{
		p_ent->init_visual(u_model.st_data,C_RESOURCE_MD2);
	}	else
    if  (file_extension_is(u_model.st_data, ".rtm")) 
	{
		p_ent->init_visual(u_model.st_data,C_RESOURCE_RTM);
	}	else
	{
		//assume a sprite of some type
		p_ent->init_visual_sprite(u_model.st_data,u_model.st_data);
	}
    p_ent->dx_create();

}


 void func_EntRotateBaseFrameYX(CScriptPassVars *p_vars)
{

	CVisual *p_ent  = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("func_EntRotateBaseFrame> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
	p_ent->RotateBaseFrameYX(p_vars->a_vars[1].GetFloat(), p_vars->a_vars[2].GetFloat());
}
  

  void func_EntSetAnimSpeedMod(CScriptPassVars *p_vars)
{

	CVisualMulti *p_ent  = (CVisualMulti *)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetAnimSpeedMod> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
	p_ent->SetSpeedMod(p_vars->a_vars[1].GetFloat());
}


   void func_EntSetCollisionType(CScriptPassVars *p_vars)
{

	CVisualMulti *p_ent  = (CVisualMulti *)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetCollisionType> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
	p_ent->SetCollisionType(p_vars->a_vars[1].GetInt());
}

  
  //note, only Y is going to work right atm, unless I want to do the expensive rotation math for the rest
  void func_EntSetDrawOffset(CScriptPassVars *p_vars)
{

	CVisual *p_ent  = (CVisual *)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntSetDrawOffset(> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
	p_ent->set_visual_follow_offset(*p_vars->a_vars[1].GetVector3());
}


  //note, only Y is going to work right atm, unless I want to do the expensive rotation math for the rest
  void func_GetHost(CScriptPassVars *p_vars)
{
 p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, cls_tglo.GetProgram(), 0, NULL);
}


   void func_EntSetName(CScriptPassVars *p_vars)
{

	CThing *p_ent  = (CThing *)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("func_EntSetName> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
	p_ent->set_thing_name(uni(p_vars->a_vars[1].GetWChar()).to_st());
}

 void func_EntGetName(CScriptPassVars *p_vars)
{

	CThing *p_ent  = (CThing *)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("func_GntSetName> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
	
    uni u_name(p_ent->get_thing_name());

    p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, u_name.us_data);
 
 }

   void func_EntSetWallBounce(CScriptPassVars *p_vars)
{

	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("func_EntSetWallBounce> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
	p_ent->SetHitWallBehavior(p_vars->a_vars[1].GetInt());
}


   void func_EntSetBlink(CScriptPassVars *p_vars)
{

	CEnt *p_ent  = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("func_EntSetBlink> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   	
	p_ent->SetBlink(p_vars->a_vars[1].GetInt());
    //ignore blink speed for now

}

   
   //check a world coordinate to see if it matches a zone or is hard.  (either can be set independtly,
   //if asked for zone "ALL" it will ignore the zone part
   void func_IsPositionValid(CScriptPassVars *p_vars)
{
  
	//first find the sub zone by string..
		
       int i_sub_zone;
       uni uni_zone(p_vars->a_vars[1].GetWChar());
       
       if (strlen(uni_zone.st_data) == 0)
       {
           i_sub_zone = -1; //default
       }
       
       if (stricmp(uni_zone.st_data, "ALL") == 0)
       {
           i_sub_zone = -2; //internal code for ALL
       } else
       {
           
           i_sub_zone = cls_tglo.p_map->p_sub_manager->GetSubZoneByName(uni_zone.st_data);
           
           if (i_sub_zone == -1)
           {
               log_error("IsPositionValid: unable to locate sub zone %s.", uni_zone.st_data);
                p_vars->return_var.SetVar(C_SCRIPT_VAR_INT,0,0, NULL);

               return; //didn't set any vars
           }
           
       }
       D3DXVECTOR3 vec_pos = *p_vars->a_vars[0].GetVector3();
	 
    if (i_sub_zone != -2)
    {
        
        if (
            
            i_sub_zone != 
 cls_tglo.p_map->get_tile_pointer(cls_tglo.p_map->get_tile_x(vec_pos.x),
 cls_tglo.p_map->get_tile_y(vec_pos.z))->i_sub_zone
            
            
            )
        {
            //not the subzone they wanted
            
            p_vars->return_var.SetVar(C_SCRIPT_VAR_INT,0,0, NULL);
            return;	
        }
        
    }
   
    if (p_vars->a_vars[2].GetInt() == 1)
    {
       //they also want to make sure the ground isn't on a hard tile
        if (cls_tglo.p_map->tile_is_hard_world(vec_pos.x, vec_pos.z))
        {
             p_vars->return_var.SetVar(C_SCRIPT_VAR_INT,0,0, NULL);
            return;	

        }

    }
 

  //passed all tests, good area.
 	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT,1,0, NULL);

}



   void func_SetOnTouchAll(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CEntItem * p_temp = (CEntItem*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("SetOnTouchAll> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.

	p_temp->SetOnTouchCeilingScript(p_vars->a_vars[1].GetInt()!=0);
    p_temp->SetOnTouchWallScript(p_vars->a_vars[1].GetInt()!=0);
    p_temp->SetOnTouchGroundScript(p_vars->a_vars[1].GetInt()!=0);
    p_temp->SetOnTouchScript(p_vars->a_vars[1].GetInt()!=0);


}


void func_SetOnTouchWall(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CEntItem * p_temp = (CEntItem*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("SetOnTouchWall> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.

	p_temp->SetOnTouchWallScript(p_vars->a_vars[1].GetInt()!=0);

}


void func_SetOnTouchCeiling(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CEntItem * p_temp = (CEntItem*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("SetOnTouchCeiling> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.

	p_temp->SetOnTouchCeilingScript(p_vars->a_vars[1].GetInt()!=0);

}



void func_SetOnTouchGround(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CEntItem * p_temp = (CEntItem*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("SetOnTouchGround> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.

	p_temp->SetOnTouchGroundScript(p_vars->a_vars[1].GetInt()!=0);

}



 void func_EntSetHardness(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CEnt * p_temp = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntSetHardness> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.

	p_temp->SetHardness(p_vars->a_vars[1].GetInt());

}

void func_EntSetBroadcastType(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CEnt * p_temp = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntSetBroadcastType> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.

	p_temp->SetBroadCastType(p_vars->a_vars[1].GetInt());

}   

void func_EntSetTouchDamage(CScriptPassVars *p_vars)
{
	//move the ent to the correct spot
	//this is kind of a slow look up by id.  Maybe do something about this later...
	CEnt * p_temp = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_temp)
	{
		log_error("EntSetTouchDamage> Int %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
     //let's set the position and also force an update.

	p_temp->SetTouchDamage(p_vars->a_vars[1].GetInt());

}

void func_EntSetCentering(CScriptPassVars *p_vars)
{
    //move the ent to the correct spot
    //this is kind of a slow look up by id.  Maybe do something about this later...
    CEnt * p_temp = (CEnt*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
    if (!p_temp)
    {
        log_error("EntSetCentering> Int %d is invalid.", p_vars->a_vars[0].GetInt());
        return;
    }
    //let's set the position and also force an update.
    
    p_temp->SetCentering(p_vars->a_vars[1].GetInt());
    
}

void func_EntSetPicVisualEffect(CScriptPassVars *p_vars)
{
    //move the ent to the correct spot
    //this is kind of a slow look up by id.  Maybe do something about this later...
    CPic * p_temp = (CPic*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
    if (!p_temp)
    {
        log_error("EntSetPicVisualType> Int %d is invalid.", p_vars->a_vars[0].GetInt());
        return;
    }
    
    assert(p_temp->get_type() == C_THING_CPIC && "This doesn't work with this object type");
    //let's set the position and also force an update.
    p_temp->SetCPicVisualEffect(p_vars->a_vars[1].GetInt());
    
}      

void func_EntSetDrawTwoSided(CScriptPassVars *p_vars)
{
    //move the ent to the correct spot
    //this is kind of a slow look up by id.  Maybe do something about this later...
    CVisual * p_temp = (CVisual*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
    if (!p_temp)
    {
        log_error("EntSetDrawTwoSided> Int %d is invalid.", p_vars->a_vars[0].GetInt());
        return;
    }
    //let's set the position and also force an update.
    
    p_temp->SetForceDoubleSidedTextures(p_vars->a_vars[1].GetInt());
    
}


