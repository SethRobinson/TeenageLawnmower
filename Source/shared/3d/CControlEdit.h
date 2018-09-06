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

#include "CThing.h"
#include "CCamera.h"
#include "CTerrain.h"

const int C_BRUSH_TYPE_PAINT = 0;
const int C_BRUSH_TYPE_LANDSCAPE = 1;

const int C_BRUSH_MOD = 0;
const int C_BRUSH_SET = 1;
const int C_BRUSH_SMOOTH = 2;


const int C_BRUSH_PAINT_LAND = 0;
const int C_BRUSH_PAINT_WALLS = 1;


class CControlEdit : public CThing
{

private:
	int i_current_brush;
	int i_brush_type;
	int i_brush_sub_mode;
	float f_strength;	 
	bool m_b_overlay;
 	void CControlEdit::process_landscape();
 	void CControlEdit::process_paint();
	float f_tool_setting;
	bool b_paste_full; //only paste in full 3X3 grids?
    unsigned char uc_texture_mode;
	unsigned char uc_texture_effect;
	unsigned char uc_texture_shrink;
public:
	void set_overlay_mode(bool b_overlay){m_b_overlay = b_overlay;}
	void set_current_brush(int i_new){i_current_brush = i_new;};
	void set_brush_type(int i_new){i_brush_type = i_new;};
	void set_brush_sub_mode(int i_new){i_brush_sub_mode = i_new;};
    void set_tool_setting(float f_new){f_tool_setting = f_new;};
	void set_paste_full(bool b_new){b_paste_full = b_new;};
	void set_texture_mode(unsigned char uc_new){uc_texture_mode = uc_new;};
	void CControlEdit::process_paint_walls();

	CControlEdit(); //init   
  virtual void think();

};