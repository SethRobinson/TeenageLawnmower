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
#include "CSettings.h"
#include "CLightManager.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void func_MovePointByDirection(CScriptPassVars *p_vars);
void func_MovePointByDirAndTilt(CScriptPassVars *p_vars);
void func_Reset3DDevice(CScriptPassVars *p_vars);
void func_GetVSync(CScriptPassVars *p_vars);
void func_SetVSync(CScriptPassVars *p_vars);
void func_ToggleWindowedMode(CScriptPassVars *p_vars);
void func_SetMouseJoystickControl(CScriptPassVars *p_vars);
void func_GetDisableJoystick(CScriptPassVars *p_vars);
void func_SetDisableJoystick(CScriptPassVars *p_vars);
void func_LightAdd(CScriptPassVars *p_vars);
void func_LightRemove(CScriptPassVars *p_vars);



void add_global_functions_to_script_0_3d()
{
	if (!cls_tglo.p_script_engine)
	{
		log_error("Scripting engine not initted, can't add 3d functions.");
	}
						 
	CScriptPassVars *p_vars;

	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_VECTOR3); //input vector
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //input angle
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_FLOAT); //distance to offsetby
	p_vars->return_var.SetType(C_SCRIPT_VAR_VECTOR3); //return vector
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("MovePointByDirection", func_MovePointByDirection, p_vars);

    p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_VECTOR3); //input vector
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //input angle
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_FLOAT); //tilt
	p_vars->a_vars[3].SetType(C_SCRIPT_VAR_FLOAT); //distance to offsetby
	p_vars->return_var.SetType(C_SCRIPT_VAR_VECTOR3); //return vector
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("MovePointByDirAndTilt", func_MovePointByDirAndTilt, p_vars);


    p_vars = new CScriptPassVars();
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("Reset3DDevice", func_Reset3DDevice  , p_vars);

    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //return vector
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("GetVSync", func_GetVSync  , p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //input angle
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SetVSync", func_SetVSync  , p_vars);
 
    p_vars = new CScriptPassVars();
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("ToggleWindowedMode", func_ToggleWindowedMode, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //input true/false
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SetMouseJoystickControl", func_SetMouseJoystickControl  , p_vars);

   p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //return vector
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("GetDisableJoystick", func_GetDisableJoystick  , p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //input angle
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SetDisableJoystick", func_SetDisableJoystick  , p_vars);
 
    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_VECTOR3); //pos
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_VECTOR3); //color
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_FLOAT); //range
	p_vars->a_vars[3].SetType(C_SCRIPT_VAR_FLOAT); //distance trigger
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //return light index

    cls_tglo.p_script_engine->GetScript(0)->AddFunction("LightAdd", func_LightAdd  , p_vars);
 

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //light to remove
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("LightRemove", func_LightRemove  , p_vars);
 


}



void func_MovePointByDirection(CScriptPassVars *p_vars)
{

    D3DXVECTOR3 vec_point = *p_vars->a_vars[0].GetVector3();

    vec_point.z += float(cos(p_vars->a_vars[1].GetFloat()))*p_vars->a_vars[2].GetFloat();
    vec_point.x += float(sin(p_vars->a_vars[1].GetFloat()))*p_vars->a_vars[2].GetFloat();
 
	p_vars->return_var.SetVar(C_SCRIPT_VAR_VECTOR3,0,0,vec_point);

}

 void func_MovePointByDirAndTilt(CScriptPassVars *p_vars)
{

    D3DXVECTOR3 vec_point = *p_vars->a_vars[0].GetVector3();

    float f_distance = p_vars->a_vars[3].GetFloat();
    float f_tilt = p_vars->a_vars[2].GetFloat();
    f_tilt *= 0.75f;
    float f_dir =  p_vars->a_vars[1].GetFloat();
    vec_point.y += f_distance * f_tilt;
    vec_point.z += float(cos(f_dir))*f_distance*(1-float(fabs(f_tilt)));
    vec_point.x += float(sin(f_dir))*f_distance*(1-float(fabs(f_tilt)));
   //now go back and limit the x and z
	p_vars->return_var.SetVar(C_SCRIPT_VAR_VECTOR3,0,0,vec_point);

}


 void func_Reset3DDevice(CScriptPassVars *p_vars)
{
    cls_tglo.p_d3dapp->ReinitializeTheDevice();
}

 void func_ToggleWindowedMode(CScriptPassVars *p_vars)
{
    cls_tglo.p_d3dapp->ToggleFullscreen();
}


void func_GetVSync(CScriptPassVars *p_vars)
{
  if (cls_tglo.p_settings)
    {
   
      p_vars->return_var.SetVar(C_SCRIPT_VAR_INT,cls_tglo.p_settings->GetVSync(), 0, NULL );
  } else
  {
       log_error("p_settings not initted in tglo.");

  }
}

void func_SetVSync(CScriptPassVars *p_vars)
{
    if (cls_tglo.p_settings)
    {
    
    cls_tglo.p_settings->SetVSync(p_vars->a_vars[0].GetInt());
    cls_tglo.p_settings->SetVideoSettingsFromConfig(cls_tglo.p_d3dapp);
    } else
    {
      log_error("p_settings not initted in tglo.");
    }
}

 
void func_GetDisableJoystick(CScriptPassVars *p_vars)
{
  if (cls_tglo.p_settings)
    {
   
      p_vars->return_var.SetVar(C_SCRIPT_VAR_INT,cls_tglo.p_settings->GetDisableJoystick(), 0, NULL );
  } else
  {
       log_error("p_settings not initted in tglo.");
  }
}

void func_SetDisableJoystick(CScriptPassVars *p_vars)
{
    if (cls_tglo.p_settings)
    {
    
    cls_tglo.p_settings->SetDisableJoystick(p_vars->a_vars[0].GetInt());
    cls_tglo.p_cls_input->SetDisableJoystick(p_vars->a_vars[0].GetInt());
    } else
    {
      log_error("p_settings not initted in tglo.");
    }
}



void func_SetMouseJoystickControl(CScriptPassVars *p_vars)
{
   cls_tglo.SetMouseControlledByJoystick(p_vars->a_vars[0].GetInt());
}


void func_LightAdd(CScriptPassVars *p_vars)
{
    if (!cls_tglo.p_light_manager)
    {
        log_error("Unable to use LightAdd, light manager not initted yet.");
        return;
    }
    
    
    int i_light =  cls_tglo.p_light_manager->AddLight(p_vars->a_vars[0].GetVector3(), p_vars->a_vars[1].GetVector3(),
        p_vars->a_vars[2].GetFloat(), p_vars->a_vars[3].GetFloat());
        
        p_vars->return_var.SetVar(C_SCRIPT_VAR_INT,i_light,0,NULL);
}

void func_LightRemove(CScriptPassVars *p_vars)
{
    if (!cls_tglo.p_light_manager)
    {
        log_error("Unable to use LightRemove, light manager not initted yet.");
        return;
    }
    
    cls_tglo.p_light_manager->RemoveLight(p_vars->a_vars[0].GetInt());
}


