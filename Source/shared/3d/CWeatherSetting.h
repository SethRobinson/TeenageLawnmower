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

#include "..\\all.h"
#include "d3dx8math.h"

const int C_SKYBOX_TOP = 0;
const int C_SKYBOX_BOTTOM = 1;
const int C_SKYBOX_LEFT = 2;
const int C_SKYBOX_RIGHT = 3;
const int C_SKYBOX_FRONT = 4;
const int C_SKYBOX_BACK = 5;

const int C_SKYBOX_SIDES = 6;


//note:  add new values to the END only!! Then it will always be backwards compatible.

class CWeatherSetting
{
public:
	CWeatherSetting(); //init
	bool Save(char *p_st_fname);
	bool Load(char *p_st_fname);

	//skybox
	char a_st_tex[C_SKYBOX_SIDES][256];
	float f_skybox_height_offset;
	float f_skybox_side_size;
	

	//lighting
	D3DXVECTOR3 vec_light_dir; //where the main light is pointing
	D3DXVECTOR3 vec_light_color; //main light color
	D3DXVECTOR3 vec_ambient_light; //where the main light is pointing

   //fog
	D3DXVECTOR3 vec_fog_color; 
	float f_fog_start;
	float f_fog_end;
	
	//skydome
	char st_skydome_tex[256]; 
	float f_skydome_height_offset;
	D3DXVECTOR3 vec_skydome_size;
	bool b_skydome_disable_lighting;
    char st_script[256]; //optional script filename to load.  Leave NULL for none.

};