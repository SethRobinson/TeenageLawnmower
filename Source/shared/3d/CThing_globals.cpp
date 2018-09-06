/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CThing_globals.h"
#include "CFileLib\CFileLib.h"


#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

float C_F_LOGIC_UPDATE_DELAY = 0.01f; //100 times a second

CThing_globals cls_tglo;
CThing_globals * GetApp(){return &cls_tglo;}  //another way to access these globals, for Seth's
//sanity because many other engines I work with use this



CThing_globals::CThing_globals()
{
    
    //let's switch to the dir we were run from
    switch_to_current_dir();   
    
    p_d3dDevice = NULL;
    p_d3dapp = NULL;
    p_cls_input = NULL;
    p_terrain = NULL;
    p_manager = NULL;
    p_map = NULL; 
    p_font = NULL;
    p_grid_tile = NULL;
    f_version = 0.01f;
    p_player = NULL;
    p_sound = NULL;
    p_script_engine = NULL;
    p_thing_list = NULL;
    p_console = NULL;
    p_weather = NULL;
    m_i_pause_level = 0;
    p_csmap = NULL;
    p_light_manager = NULL;
    m_i_program = C_GLOBAL_EDITOR;
    i_need_wm_messages = 0;
    p_csrender = 0; //only used for 2d stuff
    p_settings = 0;
    m_mouse_controlled_by_joystick = false;
    reset();
    m_p_file_lib = new CFileLib;
}

CThing_globals::~CThing_globals()
{
    SAFE_DELETE(m_p_file_lib);

}


void CThing_globals::reset()
{
    i_mesh_detail_mod = 10;
    p_camera = NULL;
    f_time_to_use = 0;
    i_game_update = 0;
    b_running = false;
    b_ignore_next_smooth_frame = false;
    m_f_delta = 0;   
 
}


void CThing_globals::game_time_update(float f_time_passed)
{
   // static char crap[50];

   // sprintf(crap, "%.3f", f_time_passed);
   // LogMsg(crap);
   
    f_time_to_use += f_time_passed;
    m_f_delta = f_time_passed;
    

}



int CThing_globals::get_screen_width()
{
	return p_d3dapp->m_d3dsdBackBuffer.Width;
}
int CThing_globals::get_screen_height()
{
	return p_d3dapp->m_d3dsdBackBuffer.Height;
}	


bool AdjustPointPosition(D3DXVECTOR2 *p_pt_pos, int i_x, int i_y)
{
	if (i_x == cls_tglo.get_screen_width()) if (i_y == cls_tglo.get_screen_height())
	{
	  //um, no adjustment is required, the screen is the one we planned for
		return false;
	}


	//change is required.
	float f_adjust_x = float((cls_tglo.get_screen_width()-i_x) / 2);
	float f_adjust_y = float((cls_tglo.get_screen_height()-i_y) / 2);
	p_pt_pos->x += f_adjust_x;
	p_pt_pos->y += f_adjust_y;

    return true;
}   


//our interpolated delta.  if you want the real one, use return p_d3dapp->m_fElapsedTime;
float CThing_globals::GetDelta()
{
    {return m_f_delta;}  //time the last frame took

}

