//******************************************
// copyright 2002	Seth A. Robinson

// This file is NOT public domain and may not be shared, distributed or used
// without specific permission in any way.

// seth@rtsoft.com - www.rtsoft.com
//******************************************

#pragma once

#include "CInput.h"
#include "all.h"
#include "3d\\CThingList.h"
#include "3d\\CThing_globals.h"
#include "3d\\CMap.h"
#include "CPlayer.h"
#include <iostream>
#include <string>
#include <vector>
#include "3d\\CSettings.h"

void log_msg(LPCSTR lpFormat, ...);
void log_error(LPCSTR lpFormat, ...);
void switch_to_play_cam();


class app_globals
{
public:
   	
	app_globals();

	~app_globals()
	{
	  	SAFE_DELETE(p_player);
	}

    float GetVersion(){return f_version;}
    
    char st_version_long[100];
	char st_version[16];
	char st_name[100]; //name of product
	bool b_log_to_disk;
	char st_command_line[256];
	CPlayer *p_player; //stores save game data and info for player
 	 bool b_disable_sound;
     bool b_disable_joystick;

   void process_command_line(char *p_st_command_line);
  	float f_version;


private:
  

};



extern app_globals app_glo;

//handle DX8 input
extern CInput cls_input;

extern CMapZone cls_map; 
//handle map data (but not the drawing part)

//handle creation of all objects
extern CThingList cls_things;

extern ISoundManager *p_cls_sound;
extern CSettings g_settings;
