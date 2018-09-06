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
#include "CResourceAnim.h"
//forwards for speed

struct t3DModel;

#include "CManager.h" 

//handle quake2 model format popuarly known as .MD2

//MD2_MAX_VERTICES is defined in md2.h

class CResourceMD2: public CResourceAnim
{
private:
 t3DModel *m_3DModel;	// This holds the 3D Model info that we load in
 
 int m_i_total_faces; //to keep track of our index buffer size
public:
 virtual void dx_create();
 virtual void dx_restore();
 virtual void dx_invalidate();

 virtual bool InitMesh();
 CResourceMD2();
 virtual ~CResourceMD2();
 virtual int CResourceMD2::Draw(int i_anim, int i_frame, float t, DWORD dw_color, int i_interpolate_mode);
 virtual int GetFramesInAnim(int i_anim);
 virtual int GetAnimByName(char *st_anim);


};	 

