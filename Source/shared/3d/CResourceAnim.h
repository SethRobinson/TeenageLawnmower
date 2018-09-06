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

#include "CResource.h"

const int C_ANIM_INTERPOLATE_MODE_FORWARD = 0;
const int C_ANIM_INTERPOLATE_MODE_BACKWARDS = 1;
const int C_ANIM_INTERPOLATE_MODE_NONE = 2;	//faster

const int C_ANIM_MAX_VERT = 5000; //maximum number of verts per frame for our scratch buffer 
const int C_ANIM_RTM_MAX_VERT = 10000;

extern LPDIRECT3DVERTEXBUFFER8 g_vert_buff;
extern int g_i_md2_vert_ref;

extern LPDIRECT3DVERTEXBUFFER8 g_rtm_vert_buff;
extern int g_i_rtm_vert_ref;


class CResourceAnim : public CResource
{
public:
    virtual void dx_create();
  virtual void dx_invalidate();
  virtual void dx_restore();
  virtual void dx_kill();
  CResourceAnim();
  virtual ~CResourceAnim();

    D3DXVECTOR3 vec_bb_min; //bounding box
	D3DXVECTOR3 vec_bb_max;
	D3DXVECTOR3         m_vObjectCenter;        // Center of bounding sphere of object
    FLOAT               m_fObjectRadius;        // Radius of bounding sphere of object

	virtual int Draw(int i_anim, int i_frame, float t, DWORD dw_color, int i_interpolate_mode) {return 0;};
	virtual int GetFramesInAnim(int i_anim) {return 0;};
	virtual bool InitMesh() {return true;};
	virtual int GetAnimByName(char *st_anim) {return 0;};
};
