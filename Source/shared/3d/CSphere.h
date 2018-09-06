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



class CSphere : public CVisual
{
protected:
 	ID3DXMesh* p_mesh;
    D3DMATERIAL8 mtrl;	
	float f_radius;
	UINT ui_slices;
	UINT ui_stack;


public:
 	virtual void dx_draw();
	virtual void think();
	virtual void dx_restore();
	virtual void dx_create();
	virtual void dx_kill();
 	void CSphere::init_sphere(float f_radius_new, UINT ui_slices_new, UINT ui_stack_new); //init sphere defaults
    void CSphere::set_sphere_color(float r, float g, float b);
	CSphere();
	virtual ~CSphere();

};