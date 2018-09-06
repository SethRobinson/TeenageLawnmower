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
#include "CResourceSprite.h" //steal it's vert info
//forwards for speed


#include "CManager.h" 
#include "CResourceAnim.h"
#include "CRTM.h"
//MD2_MAX_VERTICES is defined in md2.h

class CResourceRTM: public CResourceAnim
{
private:
  CRTM m_model;
  Material m_mat; //material we'll use to render this model type.
  

public:
 virtual void dx_create();
 virtual void dx_restore();
 virtual void dx_invalidate();

 virtual bool InitMesh();
 CResourceRTM();
 virtual ~CResourceRTM();
 virtual int CResourceRTM::Draw(int i_anim, int i_frame, float t, DWORD dw_color, int i_interpolate_mode);
 virtual int GetFramesInAnim(int i_anim);
 virtual int GetAnimByName(char *st_anim);
 

};	 

