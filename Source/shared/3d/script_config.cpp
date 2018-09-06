/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "Script_config.h"                                                       
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
#include "CThingInput.h"
#include "CPicMeter.h"
#include "CPicInput.h"
#include "d3dres.h"
#include "CSettings.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void func_VKeySet(CScriptPassVars *p_vars);


void func_VKeyToString(CScriptPassVars *p_vars);
void func_WaitForKey(CScriptPassVars *p_vars);
void func_EntMeterMod (CScriptPassVars *p_vars);
void func_EntMeterGet (CScriptPassVars *p_vars);
void func_EntMeterSet (CScriptPassVars *p_vars);
void func_CreatePicInput (CScriptPassVars *p_vars);
void func_EntSetPicInputTrigger(CScriptPassVars *p_vars);
void func_EntSetPicInputSound(CScriptPassVars *p_vars);

void func_EntKillAll(CScriptPassVars *p_vars);
void func_DialogSetFont (CScriptPassVars *p_vars);
void func_EntSetFadeUp (CScriptPassVars *p_vars);
void func_EntSetFadeDown (CScriptPassVars *p_vars);
void func_ResetFrameTimer(CScriptPassVars *p_vars);
void func_DialogSetBG(CScriptPassVars *p_vars);
void func_OpenConfigureVideo(CScriptPassVars *p_vars);
void func_GetScreenX(CScriptPassVars *p_vars);
void func_GetScreenY(CScriptPassVars *p_vars);
void func_SetRenderOptions(CScriptPassVars *p_vars);
void func_GetDemoMode(CScriptPassVars *p_vars);


void add_global_functions_to_script_0_config()
{
	if (!cls_tglo.p_script_engine)
	{
		log_error("Scripting engine not initted, can't add 3d functions.");
	}
						 
	CScriptPassVars *p_vars;

	p_vars = new CScriptPassVars();

	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //ent name
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //default text we want
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT); //Style. 0 for normal string input
	p_vars->a_vars[3].SetType(C_SCRIPT_VAR_INT); //font id
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR); //ent id we created
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CreatePicInput", func_CreatePicInput, p_vars);

   	
    
    
    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //ent id
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR); //position
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("VKeyToString", func_VKeyToString, p_vars);
    
     p_vars = new CScriptPassVars();
	 cls_tglo.p_script_engine->GetScript(0)->AddFunction("ResetFrameTimer", func_ResetFrameTimer, p_vars);
   
     p_vars = new CScriptPassVars();
     cls_tglo.p_script_engine->GetScript(0)->AddFunction("OpenConfigureVideo", func_OpenConfigureVideo, p_vars);

     p_vars = new CScriptPassVars();
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntKillAll", func_EntKillAll, p_vars);
    
    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //dialog id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //new font id
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("DialogSetFont", func_DialogSetFont, p_vars);
  
    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //dialog id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //background filename, "" for none
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("DialogSetBG", func_DialogSetBG, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //dialog id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //fade speed
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetFadeUp", func_EntSetFadeUp, p_vars);


    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //dialog id (only support textboxes now)
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //fade speed
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetFadeDown", func_EntSetFadeDown, p_vars);


    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_FORCE_WAIT);  //reserved?
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //type of key needed.  0 for all, 1 for 1 key only? (no alt/shift etc)
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("WaitForKey", func_WaitForKey, p_vars);


    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //vkey name
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //keys to use in text form 
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("VKeySet", func_VKeySet, p_vars);


    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of meter
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //amount to mod with
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntMeterMod", func_EntMeterMod, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of meter
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //amount to set to
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntMeterSet", func_EntMeterSet, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of meter
	p_vars->return_var.SetType(C_SCRIPT_VAR_FLOAT); //amount t
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntMeterGet", func_EntMeterGet, p_vars);


    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //script to run
    p_vars->a_vars[2].SetType(C_SCRIPT_VAR_WCHAR); //script function to run
    
    
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetPicInputTrigger", func_EntSetPicInputTrigger, p_vars);
  
    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //ent id
     p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //Sound effect to play
     cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntSetPicInputSound", func_EntSetPicInputSound, p_vars);
    
 

    p_vars = new CScriptPassVars();
    p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //position
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("GetScreenX", func_GetScreenX, p_vars);
 
    p_vars = new CScriptPassVars();
    p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //position
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("GetDemoMode", func_GetDemoMode, p_vars);

    p_vars = new CScriptPassVars();
    p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //position
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("GetScreenY", func_GetScreenY, p_vars);
  
    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //clear screen each frame 1 is yes
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //Clear z buffer, 1 is yes
    
    p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT); //r (if clear screen is enabled these are used)
    p_vars->a_vars[3].SetType(C_SCRIPT_VAR_INT); //g
    p_vars->a_vars[4].SetType(C_SCRIPT_VAR_INT); //b
    

    cls_tglo.p_script_engine->GetScript(0)->AddFunction("SetRenderOptions", func_SetRenderOptions, p_vars);
   
}



void func_EntSetPicInputTrigger(CScriptPassVars *p_vars)
{
    //we use CPic's set trigger, but actually the trigger will be in CPicInput.


	CPic * p_meter = (CPic*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_meter)
	{
		log_error("EntPicInputSetTrigger> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
       p_meter->SetTrigger(0, 0,
        p_vars->a_vars[1].GetInt(),
        uni(p_vars->a_vars[2].GetWChar()).to_st()
       );
    
}

void func_EntSetPicInputSound(CScriptPassVars *p_vars)
{
    
    CPicInput * p_ent = (CPicInput*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
    if (!p_ent)
    {
        log_error("EntPicInputSetSound> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
        return;
    }
    
   p_ent->SetInputSoundEffect(uni(p_vars->a_vars[1].GetWChar()).to_st());
    
}



void func_VKeyToString(CScriptPassVars *p_vars)
{
    char st_temp[256];


    cls_tglo.p_cls_input->GetTextFromVirtualKey((char*)&st_temp, uni(p_vars->a_vars[0].GetWChar()).to_st());
    uni uni_temp(0);
    uni_temp.set(st_temp);
    p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni_temp.us_data);
 }   




void func_WaitForKey(CScriptPassVars *p_vars)
{

    //create input wait entity
    CThingInput *p_thing = new CThingInput();
	cls_tglo.p_thing_list->add_new_thing(p_thing);
    //tell the script to wait forever
	p_thing->SetScriptCallbackOnKeypress(p_vars->i_script, p_vars->a_vars[0].GetInt());
	p_vars->return_var.SetVar(C_SCRIPT_VAR_FORCE_WAIT, 200000000, 0, NULL);
}





void func_VKeySet(CScriptPassVars *p_vars)
{

   cls_tglo.p_cls_input->AddAliasBindVirtualKeyByString(uni(p_vars->a_vars[0].GetWChar()).to_st(),
       uni(p_vars->a_vars[1].GetWChar()).to_st());

}


void func_EntMeterMod (CScriptPassVars *p_vars)
{
	CPicMeter * p_meter = (CPicMeter*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_meter)
	{
		log_error("EntMeterMod> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}

    p_meter->SetMeterMod(p_vars->a_vars[1].GetFloat());
}

void func_EntMeterGet (CScriptPassVars *p_vars)
{
	CPicMeter * p_meter = (CPicMeter*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_meter)
	{
		log_error("EntMeterGet> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
    float f_return = p_meter->GetMeter();

  	p_vars->return_var.SetVar(C_SCRIPT_VAR_FLOAT, *(int*) &f_return,0, NULL);

}


void func_EntMeterSet (CScriptPassVars *p_vars)
{
	CPicMeter * p_meter = (CPicMeter*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_meter)
	{
		log_error("EntMeterSet> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
  
    
    p_meter->SetMeter(p_vars->a_vars[1].GetFloat());

}

void func_DialogSetFont (CScriptPassVars *p_vars)
{
	CTextBox * p_box = (CTextBox*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_box)
	{
		log_error("DialogSetFont> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
  
    
    p_box->SetFontId(p_vars->a_vars[1].GetInt());

}


void func_CreatePicInput (CScriptPassVars *p_vars)
{
    CPicInput * p_input  = new CPicInput();


   	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, cls_tglo.p_thing_list->add_new_thing(p_input),
		0, NULL);
		//set the name also
    uni uni_temp(0);	
	uni_temp.set_us(p_vars->a_vars[0].GetWChar());
	if (strlen(uni_temp.st_data) >= C_MAX_THING_NAME_LENGTH)
	{
		log_error("%s is too long a thing name. %d is the max.", uni_temp.st_data,C_MAX_THING_NAME_LENGTH );
	} else
	{
		p_input->set_thing_name(uni_temp.st_data);
	}

    //set the defaults for input

    p_input->SetupFastTextRender(p_vars->a_vars[3].GetInt(), uni(p_vars->a_vars[1].GetWChar()).to_st());
    p_input->set_sprite_layer(10); //make it above other things by default

    
    p_input->dx_create();
}



void func_EntKillAll (CScriptPassVars *p_vars)
{
    //let's kill everything


  clear_objects_from_map(cls_tglo.p_thing_list, ::C_THING_ENT_GENERIC);
  clear_objects_from_map(cls_tglo.p_thing_list, C_THING_TEXTBOX);
  clear_objects_from_map(cls_tglo.p_thing_list, C_THING_TEXTBOX_CHOICE);

    clear_objects_from_map(cls_tglo.p_thing_list, ::C_THING_CPIC);

}   


void func_EntSetFadeUp (CScriptPassVars *p_vars)
{
	CThing * p_box = (CThing*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_box)
	{
		log_error("EntSetFadeUp> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
  
    
    if (
        (p_box->get_type() == C_THING_TEXTBOX) ||
        (p_box->get_type() == C_THING_TEXTBOX_CHOICE)
        )
    {
       ((CTextBox*) p_box)->SetFadeUp(p_vars->a_vars[1].GetFloat());
    }
    
    
    else
    {
       log_error("Don't know how to fade with this kind of object.  Support not implemented yet.");
    }


}

void func_EntSetFadeDown (CScriptPassVars *p_vars)
{
	CThing * p_box = (CThing*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_box)
	{
		log_error("EntSetFadeDown> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
  
    
    if (
        (p_box->get_type() == C_THING_TEXTBOX) ||
        (p_box->get_type() == C_THING_TEXTBOX_CHOICE)
        )
    {
       ((CTextBox*) p_box)->SetFadeDown(p_vars->a_vars[1].GetFloat());
    }
    
    
    else
    {
       log_error("Don't know how to fade with this kind of object.  Support not implemented yet.");
    }


}

void func_ResetFrameTimer(CScriptPassVars *p_vars)
{
    cls_tglo.SetIgnoreNextFrameSmooth(true);
   
}


void func_OpenConfigureVideo(CScriptPassVars *p_vars)
{
    //Open up that dorky choose dialog thing
    SendMessage(cls_tglo.p_d3dapp->m_hWnd, WM_COMMAND, IDM_CHANGEDEVICE, 0);
}

void func_DialogSetBG(CScriptPassVars *p_vars)
{
	CTextBox * p_box = (CTextBox*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_box)
	{
		log_error("DialogSetBG> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
  
    
    p_box->InitBG(uni(p_vars->a_vars[1].GetWChar()).to_st());

}

void func_GetScreenX(CScriptPassVars *p_vars)
{
    p_vars->return_var.SetVar(::C_SCRIPT_VAR_INT, cls_tglo.p_settings->GetScreenX(), 0, NULL);
}

void func_GetDemoMode(CScriptPassVars *p_vars)
{
#ifdef _DEMO
    p_vars->return_var.SetVar(::C_SCRIPT_VAR_INT, 1, 0, NULL);

#else
    p_vars->return_var.SetVar(::C_SCRIPT_VAR_INT, 0, 0, NULL);

#endif
}


void func_GetScreenY(CScriptPassVars *p_vars)
{
    p_vars->return_var.SetVar(::C_SCRIPT_VAR_INT, cls_tglo.p_settings->GetScreenY(), 0, NULL);
}

void func_SetRenderOptions(CScriptPassVars *p_vars)
{
    if (p_vars->a_vars[0].GetInt() == 0)
    {
      //don't clear the screen each frame
      cls_tglo.p_d3dapp->RemoveClearFlag(D3DCLEAR_TARGET);
    } else
    {
        cls_tglo.p_d3dapp->AddClearFlag(D3DCLEAR_TARGET);
    }

    if (p_vars->a_vars[1].GetInt() == 0)
    {
        //don't clear the screen each frame
        cls_tglo.p_d3dapp->RemoveClearFlag(D3DCLEAR_ZBUFFER);
    } else
    {
        cls_tglo.p_d3dapp->AddClearFlag(D3DCLEAR_ZBUFFER);
    }

    //TODO:  Handle BG color


}

