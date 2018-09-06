/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CLightning.h"
#include "CThing_globals.h"
#include "D3DHelperFuncs.h"
#include "CCamera.h"
#include "CManager.h"
#include "d3dapp.h"
#include "CWeather.h"    
#include "CThingList.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLightning::CLightning()
{
	//init stuff
	m_vec_start = D3DXVECTOR3(0, 60, 0);
	m_vec_end = D3DXVECTOR3(0, 5, 0);
	m_i_line_segments = 0;
	m_p_a_segs = NULL;
	m_i_tex_id = C_RESOURCE_NONE;
	m_p_tex = NULL;

    m_f_size = 0.40f; //lightning width
	LoadOrChangeTexture("model\\lightning.dds");
	CalculateLightningPath();
	m_timer_recalc.set_interval(150);
    SetFollowTarget(-1);
    
}


CLightning::~CLightning()
{
   //if we had a texture let's release our reference
	if (m_i_tex_id)
	{
	 	if (cls_tglo.p_manager)
		cls_tglo.p_manager->delete_ref(m_i_tex_id);
 	}
   SAFE_DELETE(m_p_a_segs);
 
}


void CLightning::LoadOrChangeTexture(char *st_filename)
{
	if (m_i_tex_id != C_RESOURCE_NONE)
	{
		//release our reference count on the old one
		cls_tglo.p_manager->delete_ref(m_i_tex_id);
        m_i_tex_id = C_RESOURCE_NONE;
	}

	//get or create new texture
	m_i_tex_id = cls_tglo.p_manager->get_dynamic_resource_by_string(C_RESOURCE_TEXTURE, st_filename);
    m_p_tex = NULL;
       
}


void CLightning::CalculateLightningPath()
{
   int i_first = 0;
  int i_last = m_i_line_segments-1;

  
  //add the final coord
 
  float f_dist = D3DXVec3Length(&(m_vec_start-m_vec_end));

  float f_radius = f_dist/30;

  m_i_line_segments = int(f_dist / 2.5f);

  SAFE_DELETE_ARRAY(m_p_a_segs);
  m_p_a_segs = new D3DXVECTOR3[m_i_line_segments];

  //set first and last
  m_p_a_segs[0] = m_vec_start;
  m_p_a_segs[m_i_line_segments-1] = m_vec_end;

  float f_height;
  //build the rest of the line
	int i;
  for (i=1; i < m_i_line_segments-1; i++)
  {
	  D3DXVec3Lerp(&m_p_a_segs[i], &m_vec_start, &m_vec_end, (float(i) / m_i_line_segments));

	  f_height = 0;
	  if (random(3) == 1)
	  {
	    //give it a little upward jerk
	  	 f_height += frandom( (f_dist / m_i_line_segments)/2) + (f_dist / m_i_line_segments)/2;
	  }

	  m_p_a_segs[i] +=  D3DXVECTOR3(frandom_range(-f_radius, f_radius), f_height, frandom_range(-f_radius, f_radius));
  }

 
  //add a bias to one side
  float f_bias_amount = f_dist / 3;
  f_bias_amount *= frandom_range(0.70f, 0.130f);
  float f_angle = frandom(D3DX_PI*2);
  
  D3DXVECTOR3 vec_bias = D3DXVECTOR3(cos(f_angle) * f_bias_amount, 0, sin(f_angle) * f_bias_amount);

  for (i=1; i < m_i_line_segments-1; i++)
  {
     m_p_a_segs[i] += (vec_bias * sin(   (D3DX_PI *  ((float(i)/m_i_line_segments)))));
  }

}

void CLightning::RefreshTextureInfo()
{
	if (m_p_tex) return;

	//well, no texture pointer exists so let's refresh it.

	if (!cls_tglo.IsRunning()) return;  //don't refresh it yet
	
	//cache out where the pointer is for speed
	CResourceTex* p_res = (CResourceTex*) cls_tglo.p_manager->get(m_i_tex_id);
	p_res->dx_create(); //load if needed
	m_p_tex = p_res->get_texture();

}
void CLightning::dx_restore()
{
	m_p_tex = NULL; //for the tex to get reloaded
}



void CLightning::dx_draw()
{
 
	if (!m_p_a_segs)
	{
		log_error("Lightning path not initted yet, can't draw.");
		return;
	}

	RefreshTextureInfo();
	if (m_timer_recalc.interval_reached())
	{
	    if (m_i_follow_target != -1)
        {
            //update target
            CVisual *p_visual = (CVisual*)p_my_thing_list->get_thing_by_id(m_i_follow_target);
            if (!p_visual)
            {
                log_msg("Lightning unable to find target.");
            }   else
            {
                SetEndPoint(&p_visual->m_vObjectPosition);
            }

        }
        CalculateLightningPath();
    }

	//	log_msg("Drawing lightning.");
	D3DXMATRIX mat_view;
	D3DXMATRIX mat_proj;

	//point them to the real info
    cls_tglo.p_d3dDevice->GetTransform(D3DTS_PROJECTION, &mat_proj );
    cls_tglo.p_d3dDevice->GetTransform( D3DTS_VIEW, &mat_view );

    
   static VERTEX_XYZ_TEX1 verts[4];

   //go through and draw each section
   verts[0].tu = 0.0f;
   verts[0].tv = 1.0f;

   verts[1].tu = 0.0f;
   verts[1].tv = 0.0f;

   verts[2].tu = 1.0f;
   verts[2].tv = 1.0f;

   verts[3].tu = 1.0f;
   verts[3].tv = 0.0f;

   
   //prepare render states

    cls_tglo.p_d3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
  
   	cls_tglo.p_d3dDevice->SetTexture(0, m_p_tex);

	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	// Enable alpha testing (skips pixels with less than a certain alpha.)

	if( cls_tglo.p_d3dapp->m_d3dCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL )
	{
		cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
		cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x08 );
		cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	}
	
    cls_tglo.p_d3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_NONE);
				
   
   for (int i=0; i < m_i_line_segments-1; i++)
   {
	   
	   ComputeScreenQuad(cls_tglo.p_camera->m_inverse_view, mat_view, mat_proj, (byte*)&verts, NULL, sizeof(VERTEX_XYZ_TEX1),
		   m_p_a_segs[i], m_p_a_segs[i+1], m_f_size, false);
	   
	   //great, now draw them.
	   
	   cls_tglo.p_d3dDevice->SetVertexShader(D3DFVF_XYZ_TEX1);
	   cls_tglo.p_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, 
		   &verts, sizeof(VERTEX_XYZ_TEX1));
	   
   }
   cls_tglo.p_d3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW);
  cls_tglo.p_d3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
   cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,    FALSE );
   cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
			
}

