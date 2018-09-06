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

#include "CVisual.h"

#include "d3dhelperfuncs.h"
#include "CWeatherSetting.h"

class CSkyBox: public CVisual
{
public:
	  CSkyBox();
	  ~CSkyBox();
    void CSkyBox::InitTextures(char *p_left, char *p_right, char *p_back, char *p_front, char *p_top, char *p_bottom);

  	virtual void dx_draw();
	void SetWallSize(float f_size){m_fSize = f_size;}
protected:

	
	enum BoxFace {
		Top = 0,
			Bottom,
			Left,
			Right,
			Front,
			Back
	};
	
	
	float m_fSize;
	int m_i_texture_width; //let us know how to build the quads
    int a_tex_id[C_SKYBOX_SIDES];
	LPDIRECT3DTEXTURE8 a_p_tex[C_SKYBOX_SIDES]; //direct link to our textures, for speed
	void CSkyBox::ForceTextureRefresh();
};

