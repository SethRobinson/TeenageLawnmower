/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CVisualSky.h"
#include "CManager.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CVisualSky::CVisualSky()
{
 
	this->set_visual_follow((CVisual*)cls_tglo.p_camera);
	this->set_no_lighting(true);
   set_thing_name("Sky Sphere");
   set_sort(-20);
   SetZDisable(true);
   
}




//custom rendering

void CVisualSky::Init(char *p_st_tex)
{
	char st_sky_name[256];
	sprintf(st_sky_name, "Sky%d", get_id());
 //  set_visual_follow_offset(D3DXVECTOR3(0,-5,0));
	//rotate the sphere around..
	 //	setAngleX(-D3DX_PI/2);
	 //  set_tilt(-D3DX_PI/2);
	translateMovement();
  
		init_visual("texture\\skydome\\skydome.x", C_RESOURCE_PMESH);
		CVisualMulti::dx_create();
		ReplaceTexture("Sky", "skydome6", p_st_tex); //doing this means force a texture upon this model without
		//loading the original
		/*
		init_visual_sphere(st_sky_name, 400,400,400,8,8,2, FULL_SPHERE,
		D3DCOLOR_XRGB( 255, 255, 255 ), p_st_tex);
		//resort us	, otherwise we'll get sorted with the other spheres in  the link list..
		  */ 

	set_sort(-10);
	
}

void CVisualSky::UpdateTexture(char *p_st_tex)
{
   	  
  log_error("CVisualSky: UpdateTexture not implemented yet.  Seth is a lazy ass.");
}




void CVisualSky::dx_draw()
{
    
    CResourceSphere *p_sphere = (CResourceSphere*)cls_tglo.p_manager->get(m_i_visual_id);
    cls_tglo.p_d3dDevice->SetRenderState( D3DRS_CULLMODE,     D3DCULL_NONE );
    CVisualMulti::dx_draw();
    cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
    cls_tglo.p_d3dDevice->SetRenderState( D3DRS_CULLMODE,     D3DCULL_CCW );
       
}