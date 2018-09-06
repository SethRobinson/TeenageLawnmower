/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#pragma once

#include "..\all.h"
#include <windows.h>

#include <D3DX8.h>
#include "DXUtil.h"
#include "..\\CInput.h"
#include "CMod.h"
#include "d3dapp.h"

class ISoundManager; //interface for our sound wrapper class


enum
{
  C_GLOBAL_EDITOR = 0,
      C_GLOBAL_GAME

};

extern float C_F_LOGIC_UPDATE_DELAY;
const int WM_ENGINE_MESSAGE = WM_USER+3001;


class CD3DApplication;  //will be declared later
class CCamera; //will be declared later
class CTerrain; //will be declared later
class CManager; //will be declared later
class CMapZone; 
class CThingFont;
class CInput;
class CGridTile;
class CBuildingManager;
class CEntLib;
class CEnt;
class CScriptEngine;
class CThingList;
class CConsole;
class CWeather;
class CSMap;
class CSettings;
class CLightManager;
class CFileLib;

class CSRender;

class CThing_globals
{

public:

    CThing_globals();  
    ~CThing_globals ();

    void reset();
    
    int get_screen_width();
    int get_screen_height();
    void game_time_update(float f_time_passed);
    char * GetRootDir(){return GetExeDir();}
    int get_game_age(){return i_game_update;};

    float CThing_globals::game_time_get()
    {
        if (C_F_LOGIC_UPDATE_DELAY > f_time_to_use) return 0; //no update needed
        i_game_update++;
        f_time_to_use -= C_F_LOGIC_UPDATE_DELAY;
        return C_F_LOGIC_UPDATE_DELAY;
    }

    int GetProgram(){return m_i_program;}
    void set_simulation_speed(float f_new_speed)
    {
        // if (f_new_speed < 0.01f) f_new_speed = 0.01f; //never update more than
        //100 times per second, just not needed...
        // if (f_new_speed > 0.1f) f_new_speed = 0.1f; //never update less than 10 times a second
        
        //actually set it
        C_F_LOGIC_UPDATE_DELAY = f_new_speed;
    };
    void SetIgnoreNextFrameSmooth(bool b_set)
    {
        if (b_set)
        {
            f_time_to_use = C_F_LOGIC_UPDATE_DELAY+1; //only do one tick
        }
        b_ignore_next_smooth_frame = b_set;
    }
    
    bool ResetSmoothing() {  return b_ignore_next_smooth_frame;}
    void  SetPauseLevel(int i_pause_level) { m_i_pause_level = i_pause_level; }
    inline float time(){return C_F_LOGIC_UPDATE_DELAY;};
    inline bool IsRunning(){return b_running;};
    void set_running(bool b_new){b_running = b_new;};
    int GetPauseLevel(){return m_i_pause_level;};
    bool GetMouseControlledByJoystick(){return m_mouse_controlled_by_joystick;}
    void SetMouseControlledByJoystick(bool b_new) {m_mouse_controlled_by_joystick = b_new;}
    float GetDelta();
    void SetDelta(float f_new){m_f_delta = 0;}
    CFileLib * GetFileLib(){return m_p_file_lib;}
    CFL * GetCFL(){return m_p_file_lib->GetCFL();}
   //these are basically globals, here for speed

   LPDIRECT3DDEVICE8  p_d3dDevice;
   CD3DApplication *p_d3dapp;
   CInput *p_cls_input;
   CCamera *p_camera;
   CTerrain *p_terrain;
   CManager *p_manager;
   CMapZone *p_map;
   CEntLib *p_ent_lib; //holds base items/npc to be used as templates
   CGridTile *p_grid_tile; //sub zone/tile grid
   CThingFont *p_font;
   CBuildingManager *p_building_manager;
   CEnt *p_player;
   CScriptEngine * p_script_engine;
   ISoundManager *p_sound; //sound manager
   int i_mesh_detail_mod;
   float f_version;
   bool b_running; //true if adapter is initted and running
   CThingList * p_thing_list; //set this ASAP.
   CConsole * p_console; //used for outputting debug stuff
   int m_i_pause_level;
   int m_i_program;  //is this the editor or the game?
   bool b_ignore_next_smooth_frame;
   CWeather * p_weather; //if not NULl we have weather handler active
   int i_need_wm_messages; //if != 0 we'll send WM_CHAR messages to Input windows
   CSMap *p_csmap; //only used for a mario like tile map
   CSRender *p_csrender; //only used for 2d
   CMod mod;
   CSettings *p_settings; //default video card settings, and reginfo in  the case of TLM
   bool m_mouse_controlled_by_joystick;
   CLightManager *p_light_manager;
   char m_st_root_dir[MAX_PATH]; //our game root folder where the exe is in.
   
private:
    
    //frame independent game timing stuff
    int i_game_update; //what update we're on now
    float f_time_to_use; //how much time we have to spend
    float m_f_delta;
    CFileLib * m_p_file_lib;
  

};

bool AdjustPointPosition(D3DXVECTOR2 *p_pt_pos, int i_x, int i_y);


extern CThing_globals cls_tglo;

CThing_globals * GetApp();

