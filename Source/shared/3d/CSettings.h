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
#ifndef _NO_TLM_GLOBALS
#include "CThing_globals.h"
#else
#include "..\all.h"
#endif

//File to handle loading and saving resolution, graphic settings and keybindings. We
//don't want to save it with our save game or if they load an old setting it will
//be screwy.
class CD3DApplication;

class CSettings
{

public:
    CSettings(); //set defaults
    bool Save(char *p_st_fname, bool b_force_base_dir);
  bool  Load(char *p_st_fname, bool b_force_base_dir);
  void ResetKey(); //invalidate any registration data
    void CSettings::SetKey(const char *p_name, const char *p_key);
    bool CSettings::KeyInfoExists();
    char * GetName(){return m_st_name;}
    char * GetKey(){return m_st_key;}
    void SetVideoSettingsFromConfig(CD3DApplication *p_d3dapp);
    bool GetVSync(){return m_b_vsync;}
    void SetVSync(bool b_new){m_b_vsync = b_new;}
    bool GetDisableJoystick(){return m_b_disable_joystick;}
    void SetDisableJoystick(bool b_new){m_b_disable_joystick = b_new;}
    long GetScreenX(){return m_point_window.x;}
    long GetScreenY(){return m_point_window.y;}
    void SetWindowed(bool b_new){m_b_windowed = b_new;}
    bool GetWindowed(){return m_b_windowed;}

private:
    
    bool m_b_windowed;
    POINT m_point_window;
    int m_i_desired_bits; //16 or 32 bits etc
    char m_st_name[80];
    char m_st_key[100];
    bool m_b_vsync; //usually false
    bool m_b_disable_joystick;
    char buffer[200]; //for future stuff

};

