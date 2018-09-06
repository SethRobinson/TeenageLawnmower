/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CSphere.h"
#include "CManager.h"
#include "d3dutil.h"
#include <DXErr8.h>

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSphere::CSphere()
{
   //defaults
  p_mesh = NULL;

}

void CSphere::init_sphere(float f_radius_new, UINT ui_slices_new, UINT ui_stack_new)
{
 
	f_radius = f_radius_new;
	ui_slices = ui_slices_new;
	ui_stack = ui_stack_new;

}

CSphere::~CSphere()
{
  	//kill in case we have not already
	dx_kill();

}


void CSphere::set_sphere_color(float r, float g, float b)
{
  D3DUtil_InitMaterial( mtrl, r, g, b );
}

void CSphere::dx_draw()
{
   	if (b_no_lighting) cls_tglo.p_d3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	cls_tglo.p_d3dDevice->SetMaterial( &mtrl );

	
	set_world_transform();

	
	
	p_mesh->DrawSubset(0);
   	if (b_no_lighting) cls_tglo.p_d3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	CVisual::dx_draw();
}


void CSphere::think()
{

	CVisual::think();
}


void CSphere::dx_restore()
{
	CVisual::dx_restore();

}

void CSphere::dx_create()
{
 
   dx_kill();  //kill if required	
	
   HRESULT hr;

  if( FAILED( hr=  D3DXCreateSphere( cls_tglo.p_d3dDevice, f_radius,ui_slices,ui_stack, &p_mesh, NULL)))
  {
	  	    log_error("CreateSphere (%s)", DXGetErrorString8(hr));

	 return;
 }
  CVisual::dx_create();

  //place it
  this->translateMovement();
}

void CSphere::dx_kill()
{

    //free everything
   SAFE_RELEASE(p_mesh);
   CVisual::dx_kill();
}