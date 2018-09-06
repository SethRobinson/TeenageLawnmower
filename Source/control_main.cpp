//******************************************
// copyright 2002	Seth A. Robinson

// This file is NOT public domain and may not be shared, distributed or used
// without specific permission in any way.

// seth@rtsoft.com - www.rtsoft.com
//******************************************

#include "control_main.h"
#include "CGlobals.h"
#include "Test4.h"
#include "3d\\CThingFont.h"
#include "3d\\CEntItem.h"
#include "3d\\CMessage.h"
#include "3d\\CPic.h"
#include "3d\\CResourceTex.h"
#include "3d\\CThingList.h"
#include "uni.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


bool g_b_init_was_run = false;
//called when switching resolutions, after stuff is reloaded.


void control_main_restore()
{

}


void control_main_init()
{
  
  
  //run the init script
  if (!g_b_init_was_run)
  {
 	  CMessage *p_mess = new CMessage();
      cls_things.add_new_thing(p_mess); //that's all there is to it
      p_mess->SetPauseLevel(500); //it and any pics it makes cannot be deleted by normal means
      
      //one time runnings, but after our graphic engine is going.
    cls_tglo.p_font->SetFontHeight(0, 16);
	CEntItem *p_start_script = new CEntItem();
 	cls_things.add_new_thing(p_start_script);
	p_start_script->InitScript("script\\init.c");
	g_b_init_was_run = true;
   }
 
	
}

void control_main_kill()
{
  log_msg("Killing main");
}

void control_main_think()
{
   
}


void control_main_switch()
{
	switch_control(control_main_init, control_main_kill, control_main_think, control_main_restore);
}


