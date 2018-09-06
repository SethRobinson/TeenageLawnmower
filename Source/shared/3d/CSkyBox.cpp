/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CSkyBox.h"
#include "CCamera.h"
#include "CManager.h"
#include "CThingList.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSkyBox::CSkyBox()
{
	log_msg("Initting skybox.");

		for (int i=0; i < C_SKYBOX_SIDES; i++)
	{
	  	a_tex_id[i] = 1; //default, grass
	}

	 //get the texture's we want to use
     m_fSize = 10.0f;
	 set_sort(-10); //draw first
//	 set_visual_follow(cls_tglo.p_camera);
     set_translate_asap(true);
    ForceTextureRefresh();
}


CSkyBox::~CSkyBox()
{
	//release the textures
	if (!this->p_my_thing_list->b_deleting_list)
	{
	  ForceTextureRefresh();

	}
}

void CSkyBox::InitTextures(char *p_left, char *p_right, char *p_back, char *p_front, char *p_top, char *p_bottom)
{
    ForceTextureRefresh();
   
    a_tex_id[Left] = cls_tglo.p_manager->get_dynamic_resource_by_string(C_RESOURCE_TEXTURE, p_left);
    a_tex_id[Right] = cls_tglo.p_manager->get_dynamic_resource_by_string(C_RESOURCE_TEXTURE, p_right);
    a_tex_id[Back] = cls_tglo.p_manager->get_dynamic_resource_by_string(C_RESOURCE_TEXTURE, p_back);
    a_tex_id[Front] = cls_tglo.p_manager->get_dynamic_resource_by_string(C_RESOURCE_TEXTURE, p_front);

    a_tex_id[Top] = cls_tglo.p_manager->get_dynamic_resource_by_string(C_RESOURCE_TEXTURE, p_top);
	a_tex_id[Bottom] = cls_tglo.p_manager->get_dynamic_resource_by_string(C_RESOURCE_TEXTURE, p_bottom);

}


void CSkyBox::ForceTextureRefresh()
{


	for (int i=0; i < C_SKYBOX_SIDES; i++)
	{
		if (a_tex_id[i] > 1)
		{
		   //looks like we had actually used this, let's release our claim to it
		    cls_tglo.p_manager->delete_ref(a_tex_id[i]);
			a_p_tex[i] = NULL;
		}
	    
		a_tex_id[i] = 1; //default, grass
		a_p_tex[i] = NULL;
	}

}




void CSkyBox::dx_draw()
{
   	//setup our special skybox render modes
	
	    cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
		cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
		cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		
	//	cls_tglo.p_d3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	//	cls_tglo.p_d3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
 
  	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
    cls_tglo.p_d3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE);
	
	//draw the six sides

  //cache the textures?
  CResourceTex *p_res_tex_temp = NULL;
	for (int i=0; i < C_SKYBOX_SIDES; i++)
	{
		if (!a_p_tex[i])
		{
			//cache the pointer for speed
		   	cls_tglo.p_manager->load_if_needed(a_tex_id[i]);
			p_res_tex_temp = ((CResourceTex*)cls_tglo.p_manager->get(a_tex_id[i]));
			a_p_tex[i] = p_res_tex_temp->get_texture();
		  
			if (i == 0)
		   {
		   	  //grab this info from one of them, assume they are all the same size.  why wouldn't they be?
		      m_i_texture_width = p_res_tex_temp->get_width();
		   }
		}

	}


 static float f;
 static VERTEX_XYZ_DIFFUSE_TEX1 vert[4];
 static D3DXMATRIX mat, mat_hold;
 this->set_world_transform();
  
// cls_tglo.p_d3dDevice->GetTransform(D3DTS_VIEW, &mat); //move to middle of camera

 mat_hold = mat = cls_tglo.p_camera->getObjectMatrix();
 mat._41 = mat._42 = mat._43 = 0.0f;
 cls_tglo.p_d3dDevice->SetTransform(D3DTS_VIEW, &mat); //move to middle of camera
 								
  f = 0.5f / (float)(m_i_texture_width);

  vert[0].tu = 0.0f + f; vert[0].tv = 0.0f + f;
  vert[1].tu = 0.0f + f; vert[1].tv = 1.0f - f;
  vert[2].tu = 1.0f - f; vert[2].tv = 0.0f + f;
  vert[3].tu = 1.0f - f; vert[3].tv = 1.0f - f;

  cls_tglo.p_d3dDevice->SetVertexShader(D3DFVF_XYZ_DIFFUSE_TEX1);
  f = m_fSize * 0.5f;


  // left (negative x)
  vert[0].position = D3DXVECTOR3(-f,  f,  f);
  vert[1].position = D3DXVECTOR3(-f, -f,  f);
  vert[2].position = D3DXVECTOR3(-f,  f, -f);
  vert[3].position = D3DXVECTOR3(-f, -f, -f);

cls_tglo.p_d3dDevice->SetTexture(0, a_p_tex[Left]);
  cls_tglo.p_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (LPVOID) vert, sizeof(VERTEX_XYZ_DIFFUSE_TEX1));


  // right (positive x)
  vert[0].position = D3DXVECTOR3( f,  f, -f);
  vert[1].position = D3DXVECTOR3( f, -f, -f);
  vert[2].position = D3DXVECTOR3( f,  f,  f);
  vert[3].position = D3DXVECTOR3( f, -f,  f);

  cls_tglo.p_d3dDevice->SetTexture(0, a_p_tex[Right]);
  cls_tglo.p_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (LPVOID) vert, sizeof(VERTEX_XYZ_DIFFUSE_TEX1));


  // down (negative y)
  vert[1].position = D3DXVECTOR3(-f, -f, -f);
  vert[3].position = D3DXVECTOR3(-f, -f,  f);
  vert[0].position = D3DXVECTOR3( f, -f, -f);
  vert[2].position = D3DXVECTOR3( f, -f,  f);

  cls_tglo.p_d3dDevice->SetTexture(0, a_p_tex[Bottom]);
  cls_tglo.p_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (LPVOID) vert, sizeof(VERTEX_XYZ_DIFFUSE_TEX1));

  
  // up (positive y)
  vert[0].position = D3DXVECTOR3(-f,  f,  f);
  vert[1].position = D3DXVECTOR3(-f,  f, -f);
  vert[2].position = D3DXVECTOR3( f,  f,  f);
  vert[3].position = D3DXVECTOR3( f,  f, -f);

  cls_tglo.p_d3dDevice->SetTexture(0, a_p_tex[Top]);
  cls_tglo.p_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (LPVOID) vert, sizeof(VERTEX_XYZ_DIFFUSE_TEX1));

  // back (negative z)
  vert[0].position = D3DXVECTOR3(-f,  f, -f);
  vert[1].position = D3DXVECTOR3(-f, -f, -f);
  vert[2].position = D3DXVECTOR3( f,  f, -f);
  vert[3].position = D3DXVECTOR3( f, -f, -f);

  cls_tglo.p_d3dDevice->SetTexture(0, a_p_tex[Back]);
  cls_tglo.p_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (LPVOID) vert, sizeof(VERTEX_XYZ_DIFFUSE_TEX1));

  // front (positive z)
  vert[0].position = D3DXVECTOR3( f,  f,  f);
  vert[1].position = D3DXVECTOR3( f, -f,  f);
  vert[2].position = D3DXVECTOR3(-f,  f,  f);
  vert[3].position = D3DXVECTOR3(-f, -f,  f);

  cls_tglo.p_d3dDevice->SetTexture(0, a_p_tex[Front]);
  cls_tglo.p_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (LPVOID) vert, sizeof(VERTEX_XYZ_DIFFUSE_TEX1));
 	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
    cls_tglo.p_d3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE);

	   cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	
	//move it back to where it was
	cls_tglo.p_d3dDevice->SetTransform(D3DTS_VIEW, &mat_hold); //move back

}