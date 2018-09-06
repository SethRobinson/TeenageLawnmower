//******************************************
// copyright 2002	Seth A. Robinson

// This file is NOT public domain and may not be shared, distributed or used
// without specific permission in any way.

// seth@rtsoft.com - www.rtsoft.com
//******************************************

#include "mow_script_utils.h"
#include "3d\\CThing_Globals.h"
#include "3d\\CEntItem.h"
#include "CScriptEngine.h"
#include "3d\\CEntLib.h"
#include "3d\\CThingList.h"
#include "uni.h"
#include "3d\\CVisual.h"
#include "3d\\CMap.h"
#include "3d\\CSubManager.h"
#include "3d\\CTerrain.h"
#include "3d\\CParticleEmitter.h"
#include "sound\\ISoundManager.h"


#include "3d\\CPic.h"
#include "3d\\CTextBox.h"
#include "3d\\CTextChoice.h"
#include "3d\\CManager.h"
#include "3d\\CCamera.h"
#include "3d\\Script_Utils.h"
#include "3d\\CThing_util.h"

#include "CEntMower.h"
#include "CMowStats.h"
#include "CGlobals.h"
#include "CWeedKiller.h"
#include "3d\\d3dapp.h"
#include "3d\\CThingFont.h" 
#include "3d\\CMod.h"
#include "std_all.h"
#include <direct.h> //for chdir

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//create a generic Cpic object
void func_DialogSetStyleMow(CScriptPassVars *p_vars)
{
	//create text box
	CTextBox *p_text = (CTextBox*) cls_tglo.p_thing_list->get_thing_by_id(p_vars->a_vars[0].GetInt());

	if (!p_text)
	{
	  log_error("DialogSetStyleMow got a bad id.");
	  return;
	}

	D3DXVECTOR2 pt_pos;

	pt_pos.x = 178;
	pt_pos.y = 188;

	//adjust for screensize
//	AdjustPointPosition(&pt_pos, 800, 600);

  p_text->set_pos_x(pt_pos.x);
  p_text->set_pos_y(pt_pos.y);

  p_text->SetResAdjust(C_RES_ADJUST_NORMAL, 800, 600);
  
  // p_text->SetFontColor(D3DCOLOR_ARGB(255,255,255,0));
  //setup how we want the font to act

  //size of the text surface, follow ^2 limitation.
  D3DXVECTOR2 pt_size = D3DXVECTOR2(512, 256);

  //offset of our text as comparing to the main background
  D3DXVECTOR2 pt_offset = D3DXVECTOR2(20,15);

  //if we don't want to use the whole text surface, set this to limit it
  //D3DXVECTOR2 pt_limit = D3DXVECTOR2(512, 256);
  D3DXVECTOR2 pt_limit = D3DXVECTOR2(395, 178);

  p_text->SetFontId(2);
  p_text->InitTextBox(pt_size, pt_offset, pt_limit);
  p_text->InitBG("interface\\textbox.dds");
  
  //p_text->InitBGBlank(pt_size, D3DCOLOR_ARGB(140,0,0,0));

  //let's also add up and down scroll buttons

  p_text->AddUpScroll(D3DXVECTOR2(421, 17), "interface\\arrow_up.dds", "interface\\arrow_up_2.dds");
  p_text->AddDownScroll(D3DXVECTOR2(421, 166), "interface\\arrow_down.dds", "interface\\arrow_down_2.dds");

  //we'll add the OK button, but note that it may or may not actually get used, depending
  p_text->AddOkButton(D3DXVECTOR2(200, 155), "interface\\dialog_ok.dds", "interface\\dialog_ok_2.dds");

  //that's fine and dandy.  But should we also set a click sound effect? if it's
  //a choice box then yes.

  if (p_text->get_type() == C_THING_TEXTBOX_CHOICE)
  {
	  //yes it is.
//	  ((CTextChoice*)p_text)->SetClickSound("sound\\click.wav");

  }
 
  //let's move the whole thing a bit
  
  p_text->UpdatePositions();

  p_text->dx_create();

}


void func_DialogSetStyleLoad(CScriptPassVars *p_vars)
{
	//create text box
	CTextBox *p_text = (CTextBox*) cls_tglo.p_thing_list->get_thing_by_id(p_vars->a_vars[0].GetInt());

	if (!p_text)
	{
	  log_error("DialogSetStyleLoad got a bad id.");
	  return;
	}

	D3DXVECTOR2 pt_pos;

	pt_pos.x = 178;
	pt_pos.y = 188;

	//adjust for screensize
//	AdjustPointPosition(&pt_pos, 800, 600);

  p_text->set_pos_x(pt_pos.x);
  p_text->set_pos_y(pt_pos.y);

  p_text->SetResAdjust(C_RES_ADJUST_NORMAL, 800, 600);
 
  // p_text->SetFontColor(D3DCOLOR_ARGB(255,255,255,0));
  //setup how we want the font to act

  //size of the text surface, follow ^2 limitation.
  D3DXVECTOR2 pt_size = D3DXVECTOR2(512, 256);

  //offset of our text as comparing to the main background
  D3DXVECTOR2 pt_offset = D3DXVECTOR2(20,30);

  //if we don't want to use the whole text surface, set this to limit it
  //D3DXVECTOR2 pt_limit = D3DXVECTOR2(512, 256);
  D3DXVECTOR2 pt_limit = D3DXVECTOR2(395, 166);

  p_text->SetFontId(2);
  p_text->InitTextBox(pt_size, pt_offset, pt_limit);
  p_text->InitBG("interface\\textbox.dds");
  
  //p_text->InitBGBlank(pt_size, D3DCOLOR_ARGB(140,0,0,0));

  //let's also add up and down scroll buttons

  p_text->AddUpScroll(D3DXVECTOR2(421, 17), "interface\\arrow_up.dds", "interface\\arrow_up_2.dds");
  p_text->AddDownScroll(D3DXVECTOR2(421, 166), "interface\\arrow_down.dds", "interface\\arrow_down_2.dds");

  //we'll add the OK button, but note that it may or may not actually get used, depending
  p_text->AddOkButton(D3DXVECTOR2(200, 155), "interface\\dialog_ok.dds", "interface\\dialog_ok_2.dds");

  //that's fine and dandy.  But should we also set a click sound effect? if it's
  //a choice box then yes.

  if (p_text->get_type() == C_THING_TEXTBOX_CHOICE)
  {
	  //yes it is.
//	  ((CTextChoice*)p_text)->SetClickSound("sound\\click.wav");

  }
 
  //let's move the whole thing a bit
  
  p_text->UpdatePositions();

  p_text->dx_create();

}


void func_CreateEntMower(CScriptPassVars *p_vars)
{
	CEntMower * p_script = new CEntMower();
	cls_tglo.p_thing_list->add_new_thing(p_script);

	//set the name also
    uni uni_temp(0);	
	uni_temp.set_us(p_vars->a_vars[0].GetWChar());
	if (strlen(uni_temp.st_data) >= C_MAX_THING_NAME_LENGTH)
	{
		log_error("%s is too long a thing name. %d is the max.", uni_temp.st_data,C_MAX_THING_NAME_LENGTH );
	} else
	{
		p_script->set_thing_name(uni_temp.st_data);
		//let's also enable smoke and grass clippings
		p_script->enable_smoke();
		p_script->EnableClippings("script\\grass.pss"); //particle system we want

		if (cls_tglo.IsRunning()) p_script->dx_create();
	}

	p_vars->return_var.SetVar(C_SCRIPT_VAR_INT,  p_script->get_id(), 0, NULL );
}



 void func_SetMowerClippings(CScriptPassVars *p_vars)
{

//demo has no need for this
#ifndef _DEMO 
  ((CEntMower*)cls_tglo.p_player)->EnableClippings(uni(p_vars->a_vars[0].GetWChar()).to_st());

#endif

}



 void func_EntMowerSetPower(CScriptPassVars *p_vars)
{
	CEntMower *p_ent  = (CEntMower*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntMowerSetPower> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
  p_ent->SetPower(p_vars->a_vars[1].GetInt() !=0);

}

 
  void func_EntMowerSetSpeed(CScriptPassVars *p_vars)
{
	CEntMower *p_ent  = (CEntMower*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntMowerSetSpeed> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
  p_ent->SetMowerSpeed(p_vars->a_vars[1].GetFloat());

}

 
 void func_EntMowerSetHealth(CScriptPassVars *p_vars)
{
	CEntMower *p_ent  = (CEntMower*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_ent)
	{
		log_error("EntMowerSetPower> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}
   
  p_ent->SetHealth(p_vars->a_vars[1].GetFloat());

}

void func_MoneySet(CScriptPassVars *p_vars)
{
   app_glo.p_player->SetMoney(p_vars->a_vars[0].GetInt());
}

void func_MoneyMod(CScriptPassVars *p_vars)
{
   app_glo.p_player->ModifyMoney(p_vars->a_vars[0].GetInt());
   p_vars->return_var.SetVar(C_SCRIPT_VAR_INT,  app_glo.p_player->GetMoney(), 0, NULL);
}

void func_MoneyGet(CScriptPassVars *p_vars)
{
   p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, app_glo.p_player->GetMoney(), 0, NULL);
}


void func_ReputationSet(CScriptPassVars *p_vars)
{
   app_glo.p_player->SetReputation(p_vars->a_vars[0].GetFloat());
}

void func_ReputationMod(CScriptPassVars *p_vars)
{
   app_glo.p_player->ModifyReputation(p_vars->a_vars[0].GetFloat());
   float f_rep = app_glo.p_player->GetReputation();
   p_vars->return_var.SetVar(C_SCRIPT_VAR_FLOAT,  *(int*) &f_rep, 0, NULL);
}


void func_GetVersionFloat(CScriptPassVars *p_vars)
{
   p_vars->return_var.SetVar(C_SCRIPT_VAR_FLOAT,  *(int*) &app_glo.f_version, 0, NULL);
}

void func_EntPicBrandVersion(CScriptPassVars *p_vars)
{

  	CPic *p_main  = (CPic*)cls_tglo.p_thing_list->get_thing_by_id( p_vars->a_vars[0].GetInt());
	if (!p_main)
	{
		log_error("EntPicBrandVersion> first parm %d is invalid.", p_vars->a_vars[0].GetInt());
		return;
	}

	//just make a new pic and overlay it

    CPic * p_pic_text = new CPic();
	int i_id = cls_tglo.p_thing_list->add_new_thing(p_pic_text);
	//first create a new unique resource name
    D3DXVECTOR2 pt_size;
    pt_size.x = 128;
    pt_size.y = 64;
    p_pic_text->init_visual_sprite_blank("PICVersion", pt_size, D3DCOLOR_ARGB(0,0,0,0));
   	p_pic_text->set_xy(672, 574);
   	p_pic_text->set_sprite_layer(40);
    //go ahead and load it if we are currently running
	if (cls_tglo.IsRunning())
	{
		p_pic_text->dx_create();
	} else
	{
		log_error("Shouldn't init a text box when the engine isn't running yet.. :/");
		return;
	}
	CResourceTex * p_tex = p_pic_text->GetSpriteTex();
	cls_tglo.p_font->SetPenColor(0, D3DCOLOR_ARGB(227,255,255,255));
	cls_tglo.p_font->SetPenOptions(0, DT_CENTER | DT_WORDBREAK ) ;

	RECT rect_box;
	SetRect(&rect_box, 0,0,int(pt_size.x),int(pt_size.y));
	uni u_temp(app_glo.st_version);
	p_tex->FillWithColor(D3DCOLOR_ARGB(0,0,0,0));


	p_tex->RenderText(0, &rect_box, u_temp.us_data);

	p_pic_text->SetResAdjust(C_RES_ADJUST_FULL, 800, 600);
	//also, let's add the version number to the bitmap

	/*
	if (p_main)
    {
    log_msg("Found main!");
    CResourceTex * p_tex = p_main->GetSpriteTex();
    
      if (p_tex)
      {
      //translucent
      cls_tglo.p_font->SetPenColor(0, D3DCOLOR_ARGB(255,255,255,255));
      cls_tglo.p_font->SetPenOptions(0, DT_CENTER | DT_WORDBREAK ) ;
      //cls_tglo.p_font->SetFontHeight(0, 32);
      
        RECT rect_temp;
        SetRect(&rect_temp, 0,550,0,0);
        p_tex->RenderText(0, &rect_temp, uni(app_glo.st_version_long).us_data);
        
          }
          } else
          {
          log_msg("Failed to find main.");
          }
          
            //cls_tglo.p_font->SetFontHeight(0, 16);
    */

}

void func_ReputationGet(CScriptPassVars *p_vars)
{
    float f_rep = app_glo.p_player->GetReputation();
   p_vars->return_var.SetVar(C_SCRIPT_VAR_FLOAT, *(int*) &f_rep, 0, NULL);
}

void func_SaveGame(CScriptPassVars *p_vars)
{
  	app_glo.p_player->SetSaveDescription(uni(p_vars->a_vars[0].GetWChar()).to_st());
	app_glo.p_player->Save(uni(p_vars->a_vars[1].GetWChar()).to_st());
}


void func_LoadGame(CScriptPassVars *p_vars)
{
  
    
    app_glo.p_player->Load(uni(p_vars->a_vars[0].GetWChar()).to_st());
}

void func_SetMowedTile(CScriptPassVars *p_vars)
{
#ifndef _DEMO
    if (!cls_tglo.p_player)
     {
         log_error("Player not initted right now, can't set mowed tile.");
         return;
     }
    
    ((CEntMower*)cls_tglo.p_player)->SetMowedTile(p_vars->a_vars[0].GetInt());
#endif

}


void func_GetMowedTile(CScriptPassVars *p_vars)
{

    if (!cls_tglo.p_player)
    {
        log_error("Player not initted right now, can't set mowed tile.");
        return;
    }
    
    

    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, ((CEntMower*)cls_tglo.p_player)->GetMowedTile(), 0, NULL);
   
}



void func_MowSetDrunk(CScriptPassVars *p_vars)
{
  
  #ifndef _DEMO
  if (!cls_tglo.p_player)
     {
         log_error("Player not initted right now, can't set drunk level.");
         return;
     }
 
    ((CEntMower*)cls_tglo.p_player)->SetDrunk(p_vars->a_vars[0].GetInt()); 
    #endif

}

  void func_MowSetHeightBlowUp(CScriptPassVars *p_vars)
{
  
    if (!cls_tglo.p_player)
     {
         log_error("Player not initted right now, can't set height blow up stuff.");
         return;
     }
 
    ((CEntMower*)cls_tglo.p_player)->SetHeightBlowUp(p_vars->a_vars[0].GetInt()!=0,p_vars->a_vars[1].GetFloat() );
}

void func_GetSaveDescription(CScriptPassVars *p_vars)
{
    char st_temp[256];
    app_glo.p_player->LoadDescription(st_temp, uni(p_vars->a_vars[0].GetWChar()).to_st());
    uni uni_temp(0);
    uni_temp.set(st_temp);
    p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni_temp.us_data);

 }  

//must be called until it returns 0!


void func_GetModCount(CScriptPassVars *p_vars)
{
    char st_temp[255];																		  
    int i_counter = 0;
    while ( get_files_with_wildard("mod\\", "*.", (char*)&st_temp))
    {
        
        i_counter++;
    }
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, i_counter, 0, NULL);
    
    
}


void func_MowSection(CScriptPassVars *p_vars)
{
    CWeedKiller *p_thing = new CWeedKiller;
    p_thing->Init(p_vars->a_vars[0].GetVector3(), p_vars->a_vars[1].GetInt());
    cls_tglo.p_thing_list->add_new_thing(p_thing);
}

void func_ModRun(CScriptPassVars *p_vars)
{
   
#ifdef _DEMO
    log_error("Demo cannot run mods.");

#else

    if (p_vars->a_vars[0].GetInt() == 0)
    {
        //first save the keybindings in case changes were made in a mod
        g_settings.Save("defaults.dat", true); //load again tis time wit cls_input initted.
        //they actually want to UNLOAD the mod
        cls_tglo.p_cls_input->ResetKeyBindings();
        cls_tglo.p_cls_input->AddDefaultKeyBindings();
        g_settings.Load("defaults.dat", true); //load again tis time wit cls_input initted.
        cls_tglo.mod.Clear();
        //move to the mod dir
        chdir(GetExeDir());
        //we need to kill everything and load the mod
        //reload/setup  a few things
        cls_tglo.p_script_engine->InitGlobalScript();
        add_global_functions_to_script_0(); //add the 3d engine specific functions to the scripting engine
        add_global_functions_to_script_0_mow(); //add our custom functions/vars to the scripting engine
        init_engine_from_base_path(false);
        return;
    }
    

    //get info on the mod we need
      //figure out which dir it was
      char st_temp[255];																		  
	 int i_counter = 0;
     
     while ( get_files_with_wildard("mod\\", "*.", (char*)&st_temp))
     {
         i_counter++;
         if (i_counter == p_vars->a_vars[0].GetInt())
         {
             //this is it.
             if (!cls_tglo.mod.LoadModInfoByDir(st_temp))
             {
                 log_error("Error loading mod info for dir %s.", st_temp);
             } else
             {
                 sprintf(cls_tglo.mod.m_st_path, "mod\\%s",st_temp); //remember the dir
             }
         }
     }
 
     if (cls_tglo.mod.m_st_path[0] == 0)
     {
        return; //there was an error loading the mod info, the error will have already been shown
     }
 
     //clear all old keybindings

     cls_tglo.p_cls_input->ResetKeyBindings();
     cls_tglo.p_cls_input->AddDefaultKeyBindings();

     g_settings.Load("defaults.dat", true); //load again tis time wit cls_input initted.
   
    // need to figure out what's going on with why the first story doesn'T start'

     //move to the mod dir
     chdir(cls_tglo.mod.m_st_path);
    //we need to kill everything and load the mod
     //reload/setup  a few things
     
     cls_tglo.p_script_engine->InitGlobalScript();
    
    add_global_functions_to_script_0(); //add the 3d engine specific functions to the scripting engine
	add_global_functions_to_script_0_mow(); //add our custom functions/vars to the scripting engine

   init_engine_from_base_path(false);
#endif 
}

     


void func_GetModName(CScriptPassVars *p_vars)
{
    //figure out which dir it was
      char st_temp[255];																		  
	 int i_counter = 0;
     CMod mod;
     while ( get_files_with_wildard("mod\\", "*.", (char*)&st_temp))
    {
      i_counter++;
      if (i_counter == p_vars->a_vars[0].GetInt())
      {
          //this is it.
          if (!mod.LoadModInfoByDir(st_temp))
          {
              log_error("Error loading mod info for dir %s.", st_temp);
          }
      }

    }
 
    uni uni_temp(0);
    uni_temp.set(mod.m_st_name);
    p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni_temp.us_data);
 }


 void func_LaunchRTSoftSite(CScriptPassVars *p_vars)
{
   
      winall_create_url_file("www.rtsoft.com");
      int result = (int)ShellExecute(NULL,"open","temp.url", NULL,NULL, SW_MAXIMIZE);
      ProcessError(NULL , result, "temp.url","www.rtsoft.com"); 
          
      //stop the game from running, script had better of shutdown
      cls_tglo.p_d3dapp->Pause(true);

}  

 


 void func_LaunchPurchaseSite(CScriptPassVars *p_vars)
{
   
     
     
     winall_create_url_file("www.rtsoft.com/tlm/purchase.shtml");
      int result = (int)ShellExecute(NULL,"open","temp.url", NULL,NULL, SW_MAXIMIZE);
      ProcessError(NULL , result, "temp.url","www.rtsoft.com/tlm/purchase.shtml"); 
      
     

/*
    //AFFILIATE LINKS FOR 5-STAR
      winall_create_url_file("www.regnow.com/softsell/nph-softsell.cgi?item=7932-1&affiliate=8328");
      int result = (int)ShellExecute(NULL,"open","temp.url", NULL,NULL, SW_MAXIMIZE);
      ProcessError(NULL , result, "temp.url","www.regnow.com/softsell/nph-softsell.cgi?item=7932-1&affiliate=8328"); 
  
  */
  /*
    //AFFILIATE LINKS FOR LAWNCARE>COM
      winall_create_url_file("www.regnow.com/softsell/nph-softsell.cgi?item=7932-1&affiliate=17386");
      int result = (int)ShellExecute(NULL,"open","temp.url", NULL,NULL, SW_MAXIMIZE);
      ProcessError(NULL , result, "temp.url","www.regnow.com/softsell/nph-softsell.cgi?item=7932-1&affiliate=17386"); 
    */


      //stop the game from running, script had better of shutdown
      cls_tglo.p_d3dapp->Pause(true);

}  

void add_global_functions_to_script_0_mow()
{
	if (!cls_tglo.p_script_engine)
	{
		log_error("Scripting engine not initted, can't add mow functions.");
	}
	
	CScriptPassVars *p_vars;
	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id of a dialog
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("DialogSetStyleMow", func_DialogSetStyleMow, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id of a dialog
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("DialogSetStyleLoad", func_DialogSetStyleLoad, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //id of a dialog
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);  //id of a dialog
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("CreateEntMower", func_CreateEntMower, p_vars);


	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of cpic with timer going
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT); //call when this gets hit
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntMowerSetPower", func_EntMowerSetPower, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of cpic with timer going
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //call when this gets hit
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntMowerSetHealth", func_EntMowerSetHealth, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of mower
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //how much speed, -1 for default
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntMowerSetSpeed", func_EntMowerSetSpeed, p_vars);


    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of mow progress
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("MoneySet", func_MoneySet, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //id of mow progress
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("MoneyGet", func_MoneyGet, p_vars);

    p_vars = new CScriptPassVars();
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("LaunchRTSoftSite", func_LaunchRTSoftSite, p_vars);

    p_vars = new CScriptPassVars();
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("LaunchPurchaseSite", func_LaunchPurchaseSite, p_vars);


    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //id of mow progress
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of mow progress
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("MoneyMod", func_MoneyMod , p_vars);


	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_FLOAT); //id of mow progress
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("ReputationSet", func_ReputationSet, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_FLOAT); //id of mow progress
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("ReputationGet", func_ReputationGet, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_FLOAT); //id of mow progress
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_FLOAT); //id of mow progress
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("ReputationMod", func_ReputationMod , p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); ////description of save
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR); //dir and filename
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SaveGame", func_SaveGame , p_vars);

     p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); ////description of save
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("LoadGame", func_LoadGame , p_vars);

       p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //Filename of save
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR); //description of save, "returns Empty" if none
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("GetSaveDescription", func_GetSaveDescription , p_vars);


    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of mow progress
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SetMowedTile", func_SetMowedTile, p_vars);

    p_vars = new CScriptPassVars();
    p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //id of mow progress
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("GetMowedTile", func_GetMowedTile, p_vars);
    


        p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR); //id of mow progress
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("SetMowerClippings", func_SetMowerClippings, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //id of mow progress
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("MowSetDrunk", func_MowSetDrunk, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //1 to turn it on, 0 for off
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //height to explode at, usually negative
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("MowSetHeightBlowUp", func_MowSetHeightBlowUp, p_vars);


    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_VECTOR3); //position
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT); //diameter 
    cls_tglo.p_script_engine->GetScript(0)->AddFunction("MowSection", func_MowSection, p_vars);
    
    

     p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //1 to turn it on, 0 for off
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("EntPicBrandVersion", func_EntPicBrandVersion, p_vars);

     p_vars = new CScriptPassVars();
    	p_vars->return_var.SetType(C_SCRIPT_VAR_FLOAT); //id of mow progress

    cls_tglo.p_script_engine->GetScript(0)->AddFunction("GetVersionFloat", func_GetVersionFloat, p_vars);

    p_vars = new CScriptPassVars();
    	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //id of mow progress
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("GetModCount", func_GetModCount, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //Filename of save
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR); //description of save, "returns Empty" if none
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("GetModName", func_GetModName , p_vars);

    p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT); //Filename of save
	cls_tglo.p_script_engine->GetScript(0)->AddFunction("ModRun", func_ModRun , p_vars);

   
}


	



