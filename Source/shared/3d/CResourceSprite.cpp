/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CResourceSprite.h"
#include "CThing_globals.h"
#include "d3dapp.h"
#include "CPic.h"   //this really only to get the C_METER defines

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const float C_F_SPRITE_RESOLUTION = 400;

CResourceSprite::CResourceSprite()
{
	//defaults
    m_fObjectRadius = 0;
	m_vObjectCenter = D3DXVECTOR3(0,0,0);
	
	vec_bb_min = D3DXVECTOR3(0,0,0); //bounding box
	vec_bb_max = D3DXVECTOR3(0,0,0);
	i_tex_id = ::C_RESOURCE_NONE;
    st_texture[0] = 0;	
	m_pTreeVB      = NULL;
	m_i_mip_map = 0;
	
}

CResourceSprite::~CResourceSprite()
{
  	if (i_tex_id != ::C_RESOURCE_NONE)
	{
		cls_tglo.p_manager->delete_ref(i_tex_id);
	}

	dx_invalidate();
}

bool CResourceSprite::HasAlpha()
{

	CResourceTex *p_tex = (CResourceTex*)cls_tglo.p_manager->get(i_tex_id);
   
	return (p_tex->HasAlpha());
}

bool CResourceSprite::alpha_test(float f_x, float f_y)
{
   if (i_tex_id != 0)
   {

	CResourceTex *p_tex = (CResourceTex*)cls_tglo.p_manager->get(i_tex_id);
   
	//get the real offsets

	f_x *= C_F_SPRITE_RESOLUTION;
	f_y *= C_F_SPRITE_RESOLUTION;
	

	f_x += (float(p_tex->get_width())/2);
	
	f_x = (float(p_tex->get_width())-f_x);
	f_y = (float(p_tex->get_height())-f_y); 

	return(p_tex->alpha_test(int(f_x), int(f_y)));
   } else
   {
	 log_error("Can't do alpha test, texture is null");
   }

   return true;
}

void CResourceSprite::setup_sprite(const char p_st_tex[])
{
  strcpy((char*)&st_texture, p_st_tex);
}

void CResourceSprite::RefreshTexture()
{

	
	if (i_tex_id != ::C_RESOURCE_NONE)
	{
		cls_tglo.p_manager->delete_ref(i_tex_id);
	}

	dx_invalidate();
	this->i_tex_id = C_RESOURCE_NONE; //this will force it to reload by string name rather than
	//the faster ID lookup.  
	dx_create();
}


int CResourceSprite::setup_sprite_create_new(D3DXVECTOR2 pt_size, DWORD dw_color)
{
   //let's make a brand spanking new texture and init it to be this size.
	this->i_tex_id = cls_tglo.p_manager->get_dynamic_resource_by_string(C_RESOURCE_TEXTURE, GetName());
	//ok, we have our handle let's actually create it, first get the pointer directly to it
	CResourceTex *p_tex = (CResourceTex*)cls_tglo.p_manager->get(i_tex_id);
	p_tex->CreateTexture(pt_size, dw_color);
	setup_sprite(GetName()); //give the texture the identical name that we have.  don't worry, because
	//it's marked as a C_RESOURCE_TEXTURE we won't get confused with it. (we're a C_RESOURCE_SPRITE, not a texture)
	return i_tex_id;
}

void CResourceSprite::dx_create()
{
    if (this->b_loaded) return; //already loaded	
    if (get_refs() < 1) return; //nobody needs us, why should we load?
    if (m_pTreeVB) this->dx_invalidate();
    //make sure our texture is already loaded first
    //	log_msg("%s is asking for tex %s.", GetName(), st_texture) ;
    
    if (i_tex_id == C_RESOURCE_NONE)
    {
        i_tex_id = cls_tglo.p_manager->get_dynamic_resource_by_string(C_RESOURCE_TEXTURE, 
            st_texture);
    } 
    
    
    CResourceTex *p_tex = (CResourceTex*)cls_tglo.p_manager->get(i_tex_id);
    p_tex->SetMipMaps(m_i_mip_map);
    //load it if it hasn't been
    cls_tglo.p_manager->load_if_needed(i_tex_id);
    cls_tglo.p_d3dDevice->SetTexture( 0, p_tex->get_texture());
    
    float fHeight = float(p_tex->get_height())/C_F_SPRITE_RESOLUTION;
    float fWidth = float(p_tex->get_width())/C_F_SPRITE_RESOLUTION;
    
    
    //vertex color of sprite
    DWORD dwColor = D3DCOLOR_RGBA( 255, 255, 255,255 );
    
    v[0].p     = D3DXVECTOR3(-(fWidth/2), 0, 0.0f );
    v[0].color = dwColor;
    v[0].tu    = 1.0f;   v[0].tv = 1.0f;
    v[1].p     = D3DXVECTOR3(-(fWidth/2), fHeight, 0.0f  );
    v[1].color = dwColor;
    v[1].tu    = 1.0f;   v[1].tv = 0.0f;
    v[2].p     = D3DXVECTOR3( fWidth/2, 0, 0.0f  );
    v[2].color = dwColor;
    v[2].tu    = 0.0f;   v[2].tv = 1.0f;
    v[3].p     = D3DXVECTOR3( fWidth/2, fHeight, 0.0f  );
    v[3].color = dwColor;
    v[3].tu    = 0.0f;   v[3].tv = 0.0f;
    
	   //set normals
	   v[0].n = D3DXVECTOR3(0,0,1);
       v[1].n = D3DXVECTOR3(0,0,1);
       v[2].n = D3DXVECTOR3(0,0,1);
       v[3].n = D3DXVECTOR3(0,0,1);
       
       
       //copy this info into the vertex buffer
       
       // Create a quad for rendering each tree
       if( FAILED( cls_tglo.p_d3dDevice->CreateVertexBuffer( 4*sizeof(TREEVERTEX),
           D3DUSAGE_WRITEONLY, D3DFVF_TREEVERTEX,
           D3DPOOL_MANAGED, &m_pTreeVB ) ) )
       {
           log_error("Error creating vertex buffer for sprite %s.", GetName());
           return;
       }
       
	      // Copy tree mesh data into vertexbuffer
       TREEVERTEX* v_buff;
       m_pTreeVB->Lock( 0, 0, (BYTE**)&v_buff, 0 );
       DWORD dwOffset = 0; //for later speed ups...
       memcpy( &v_buff[dwOffset], v, 4*sizeof(TREEVERTEX) );
       m_pTreeVB->Unlock();
       b_loaded = true;
}


void CResourceSprite::dx_invalidate()
{
   SAFE_RELEASE(m_pTreeVB);
	b_loaded = false;
}


void CResourceSprite::dx_restore()
{
	dx_create();
}


void CResourceSprite::dx_draw()
{
  // Set diffuse blending for alpha set in vertices.
	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
    cls_tglo.p_d3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    cls_tglo.p_d3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
  
    // Enable alpha testing (skips pixels with less than a certain alpha.)
    if( cls_tglo.p_d3dapp->m_d3dCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL )
    {
        /*
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x08 );
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
          */
    
         cls_tglo.p_d3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	    cls_tglo.p_d3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER );
		cls_tglo.p_d3dDevice->SetRenderState(D3DRS_ALPHAREF,  128); // tolerance
    }

    // Loop through and render all trees
    cls_tglo.p_d3dDevice->SetStreamSource( 0, m_pTreeVB, sizeof(TREEVERTEX) );
    cls_tglo.p_d3dDevice->SetVertexShader( D3DFVF_TREEVERTEX );
 	
	// Set the tree texture
    
    //load texture if it isn't already
	
	CResourceTex *p_tex = (CResourceTex*)cls_tglo.p_manager->get(i_tex_id);
        cls_tglo.p_d3dDevice->SetTexture( 0, p_tex->get_texture());
   
        // Render the billboard
        cls_tglo.p_d3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
   	
		cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,    FALSE );
		cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
	 //   cls_tglo.p_d3dDevice->SetRenderState( D3DRS_LIGHTING,    TRUE );
   
	   cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	   cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
 }




bool CResourceSprite::alpha_test_2d(D3DXMATRIX *p_matrix, int e_centering, D3DXVECTOR3 *p_hit)
{
   	
	//this could be optimized SO MUCH.. but I'll wait until later.
	
	float left, right, top, bottom;		   
   	CResourceTex *p_tex = (CResourceTex*)cls_tglo.p_manager->get(i_tex_id);
	 
	if (e_centering == ::C_CENTERING_UPPER_LEFT)
	{
		left = 0;
		top = 0;
		//get the true original size
		right = float(p_tex->m_source_image_info.Width);
		bottom = float(p_tex->m_source_image_info.Height);
	} else
    if (e_centering == ::C_CENTERING_CENTER)
	{
		left =  -float((p_tex->m_source_image_info.Width/2));
		top =  -float((p_tex->m_source_image_info.Height/2));
		//get the true original size
		right = left * -1;
		bottom = top * -1;
	} else
        if (e_centering == ::C_CENTERING_BOTTOM_MIDDLE)
        {
            left =  -float((p_tex->m_source_image_info.Width/2));
            top =  -float((p_tex->m_source_image_info.Height));
            //get the true original size
            right = left * -1;
            bottom = 0;
        } else
	{
			log_error("Error, %d is an undefined centering mode.", e_centering);
            assert(0);
   	}
    D3DXVECTOR3 verts[4];
  	
	 float z = 0; //depth
     float rhw= 1.0f/(z*990.0f+0.0f);

	verts[0] = D3DXVECTOR3(left-0.5f,  top-0.5f,    z);
	verts[1] = D3DXVECTOR3(right+0.5f, top-0.5f,    z);
	verts[2] = D3DXVECTOR3(right+0.5f, bottom+0.5f,  z);
    verts[3] = D3DXVECTOR3(left-0.5f,  bottom+0.5f, z);
	
	 for (int i=0; i < 4; i++)
	 {
	  //apply the transform matrix to these 4 points for special effects and translation
	 	D3DXVec3TransformCoord(&verts[i], &verts[i], p_matrix);
	 }

     //this should be modified to be a real poly test.. maybe, if I ever use rotation on gui items

		if (p_hit->x < 	verts[0].x) return false;
		if (p_hit->x > 	verts[1].x) return false;
		if (p_hit->y <  verts[1].y) return false;
		if (p_hit->y > 	verts[2].y) return false;

		return true;
}

void CResourceSprite::dx_draw_blit(D3DXMATRIX *p_matrix, int e_centering, DWORD dw_color,
                                   int i_meter_mode, float f_meter_amount,  float f_scale_x, float f_scale_y)
{
  
    float left, right, top, bottom;		   
   	
    CResourceTex *p_tex = (CResourceTex*)cls_tglo.p_manager->get(i_tex_id);
    //is it loaded?
    
    if (!p_tex) return;
    
    if (e_centering == ::C_CENTERING_UPPER_LEFT)
    {
        left = 0;
        top = 0;
        //get the true original size
        right = p_tex->m_source_image_info.Width*f_scale_x;
        bottom = p_tex->m_source_image_info.Height*f_scale_y;
    } else
        if (e_centering == ::C_CENTERING_CENTER)
        {
            left = -float(( (p_tex->m_source_image_info.Width*f_scale_x)/2));
            top = -float(((p_tex->m_source_image_info.Height*f_scale_y)/2));
            //get the true original size
            right = left * -1;
            bottom = top * -1;
        } else
            if (e_centering == ::C_CENTERING_BOTTOM_MIDDLE)
            {
                left =  -float((p_tex->m_source_image_info.Width*f_scale_x/2));
                top =  -float((p_tex->m_source_image_info.Height*f_scale_y));
                //get the true original size
                right = left * -1;
                bottom = 0;
            } else
                
          {
                log_error("Error, %d is an undefined centering mode.", e_centering);
            assert(0);
        }
        
        float z = 0.10f; //depth
          
        D3DTLVERTEX verts[4];
        
        verts[0].vec_pos = D3DXVECTOR3(left-0.5f,  top-0.5f,    z);
        verts[0].tu = 0.0f;
        verts[0].tv = 0.0f;
        
        verts[1].vec_pos = D3DXVECTOR3(right-0.5f, top-0.5f,    z);
        verts[1].tu = 1.0f;
        verts[1].tv = 0.0f;
        
        verts[2].vec_pos = D3DXVECTOR3(right-0.5f, bottom-0.5f,  z);
        verts[2].tu = 1.0f;
        verts[2].tv = 1.0f;
        
        verts[3].vec_pos = D3DXVECTOR3(left-0.5f,  bottom-0.5f, z);
        verts[3].tu = 0.0f;
        verts[3].tv = 1.0f;
        
        //before we do the transform, let's do any mods we need to for the meter code

        if (i_meter_mode == C_METER_LEFT_BASED)
        {
           verts[1].vec_pos.x  = left+  ((right-left)* f_meter_amount);
           verts[2].vec_pos.x  = verts[1].vec_pos.x;

           //comment this out to STRETCH the texture
           //also adjust the textures
           verts[1].tu *= f_meter_amount;
           verts[2].tu = verts[1].tu;

        }
 
        for (int i=0; i < 4; i++)
        {
            verts[i].rhw = 10;  //this is a little funky, but it works and we don't really need Z buffer placement
            verts[i].color = dw_color;
            
            //apply the transform matrix to these 4 points for special effects and translation
            D3DXVec3TransformCoord(&verts[i].vec_pos, &verts[i].vec_pos, p_matrix);
            
        }
          cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
          cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
         
        
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
        
        // set the texture
        cls_tglo.p_d3dDevice->SetTexture( 0, p_tex->get_texture());
        cls_tglo.p_d3dDevice->SetVertexShader(D3DFVF_TLVERTEX);
        // draw the rectangle
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_LIGHTING,  FALSE );
        
        // Enable alpha testing (skips pixels with less than a certain alpha.)
        if( cls_tglo.p_d3dapp->m_d3dCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL )
        {
            cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
            cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x00 );
            cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
        }
     
     
        cls_tglo.p_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,verts,sizeof(D3DTLVERTEX));

            
         cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
         cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
        
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,    FALSE );
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_LIGHTING,    TRUE );
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
        
}



