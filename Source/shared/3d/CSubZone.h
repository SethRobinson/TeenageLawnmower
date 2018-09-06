/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


//Information about any sub zones in this map


#pragma once


const int C_SUBZONE_CEIL_NONE = 0;
const int C_SUBZONE_CEIL_FIXED = 1;
const int C_SUBZONE_CEIL_RELATIVE = 2;	 

const int C_SUBZONE_WALL_TEX_WORLD = 0;
const int C_SUBZONE_WALL_TEX_LOCAL = 1;

const int C_SUBZONE_ROOF_TEX_WORLD = 0;
const int C_SUBZONE_ROOF_TEX_LOCAL = 1;

const int C_SUBZONE_ROOF_NORMAL = 0;
const int C_SUBZONE_ROOF_WALLS_ONLY = 1;
const int C_SUBZONE_ROOF_NONE = 2;
const int C_SUBZONE_ROOF_DRAW_NOTHING = 3;


class CSubZone
{
public:
	char st_name[50]; //name of the zone
	int i_ceal_id; //not used
	int i_ceil_type; 
	float f_wall_height;
	int i_wall_tex_style;
	float f_wall_tex_x;
	float f_wall_tex_y;
	int i_cealing_draw; //do we draw the whole cealing or what?
	float f_roof_tex_x;
	float f_roof_tex_y;
	int i_roof_tex_style;
	int i_zone_squares; //figured out at load time, NOT accurate while editting!
};

