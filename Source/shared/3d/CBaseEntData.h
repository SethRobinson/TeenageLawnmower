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

#include "CEntData.h"

const int C_DRAW_MODE_NORMAL = 0;
const int C_DRAW_MODE_FOLLOW_Y = 1; //face player by turning on the Y axis
const int C_DRAW_MODE_FOLLOW_ALL = 2; //always face player


class CBaseEntData
{
public:
	CBaseEntData();
	~CBaseEntData();
	//base info, looked up by every item of this type
	char st_name[60];
	int i_index;
	char st_visual[255];								
	float f_temp;
	int i_draw_mode;
	D3DXVECTOR3 vec_scale; //base size mod applied to all of this type
	bool b_follow_ground_mode;
	//defaults to be copied to the item when created
	CEntData *p_ent_default;
	int i_ent_size; //remember how big it was
 	bool b_enable_lighting;
	char st_script[255];
	void CBaseEntData::kill();
    bool save(FILE *fp);
	bool CBaseEntData::load(FILE *fp);


};