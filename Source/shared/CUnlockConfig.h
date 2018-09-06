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

#include "all.h"


//File to handle loading and saving resolution, graphic settings and keybindings. We
//don't want to save it with our save game or if they load an old setting it will
//be screwy.


class CConfig
{

public:
    CConfig(); //set defaults
    bool Save(bool b_force_directory); //usually true
    bool Load(bool b_force_directory); //usually true
    void ResetKey(); //invalidate any registration data
    void CConfig::SetKey(const char *p_name, const char *p_key);
    bool CConfig::KeyInfoExists();
    char * GetName(){return m_st_name;}
    char * GetKey(){return m_st_key;}
    
private:
    char buf1[15];
    char m_st_name[80];
    char m_st_key[100];
    char buffer[200]; //for future stuff

};

