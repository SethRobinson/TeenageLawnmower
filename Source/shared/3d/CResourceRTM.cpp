/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CRTM.h" //robinson technologies model format
#include "CResourceRTM.h"
#include "d3dhelperfuncs.h"
#include "CThing_util.h"
#include "d3dapp.h"
#include <DXErr8.h>


#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CResourceRTM::CResourceRTM()
{
	if (g_i_rtm_vert_ref == 0) g_rtm_vert_buff = NULL;
	g_i_rtm_vert_ref++;
    m_mat.SetAmbient(0.9f,0.9f,0.9f,1);
    
}


CResourceRTM::~CResourceRTM()
{
   dx_invalidate();

	g_i_rtm_vert_ref--;
	if (g_i_rtm_vert_ref == 0) 
	{
		SAFE_RELEASE(g_rtm_vert_buff);
	}
}

bool CResourceRTM::InitMesh()
{
 if (m_model.m_i_total_frames == 0)
  {
 
      char st_fname[256];
      strcpy(st_fname, GetName());
      if (!exist(st_fname))
      {
          sprintf(st_fname, "%s%s", GetExeDir(), GetName());
      }


  	  m_model.LoadRTM(st_fname);

	  if (m_model.GetFrameCount() < 1)
	  {
		  log_error("RTM %s has no frames.  It's not going to be that useful I would guess.", st_fname);
		  return false;
	  }
	

	  //copy our bounding box info into the real thing
	  this->m_vObjectCenter = m_model.m_vec_sphere_center;
	  this->m_fObjectRadius = m_model.m_vec_sphere_radius;

	//let's also compute some general information about this model
 /*	 
	 HRESULT hr;
	//we are letting Flexporter do this for us, no longer need it in here
	int i_frame_to_compute_with = 0;
	hr = D3DXComputeBoundingSphere( m_model.GetPointerToFrameVerts(i_frame_to_compute_with),
		m_model.GetVertCountOfFrame(i_frame_to_compute_with),   D3DFVF_XYZ,  &m_vObjectCenter, &m_fObjectRadius );

    if (FAILED(hr)) log_error("Error computing bounding sphere for %s (%s)", GetName(), DXGetErrorString8(hr));
	 //m_fObjectRadius /= 2;	//seems too big.  here is a hack to make it "feel" right.
	//also let's get the bounding box
	
    hr = D3DXComputeBoundingBox(m_model.GetPointerToFrameVerts(i_frame_to_compute_with), 
						 m_model.GetVertCountOfFrame(i_frame_to_compute_with),
                                    D3DFVF_XYZ, &this->vec_bb_min, &vec_bb_max);
    if (FAILED(hr)) log_error("Error computing bounding box for %s (%s)", GetName(), DXGetErrorString8(hr));
  */

	  //Radius is already correct.  However, bounding box isn't computed yet.  We should
	  //eventually move this into the max exporter.

	} else
	{

		//already loaded
	}
	
	return true; //success
}


void CResourceRTM::dx_invalidate()
{
 	  //free everything
    SAFE_RELEASE(g_rtm_vert_buff);
	b_loaded = false;
} 


void CResourceRTM::dx_create()
{
	if (get_refs() < 1) return; //nobody needs us, why should we load?
   if (g_rtm_vert_buff) return; //destroy if it's loaded

   //let's build our index buffer.
  HRESULT hr;
 //  log_msg("Total faces for %s is %d.  %d tex verts.", GetName(), m_i_total_faces, m_3DModel->pObject[0].numTexVertex);
   //can't figure out a way to use this index buffer with the data as I have it.  The texture index is different from
   //the vertex index and I don't want to resort them on the fly...

   /*
   if( FAILED(hr= cls_tglo.p_d3dDevice->CreateIndexBuffer( m_i_total_faces*3* sizeof(WORD),
	                                             0, D3DFMT_INDEX16,
		                                         D3DPOOL_MANAGED, 
												 &m_index_buff ) ) )
	{
	    log_error("MD2 Index (%s)", DXGetErrorString8(hr));
		return;
	}
 
    //actually copy the faces in
	WORD*		pIndices  = NULL;
   	m_index_buff->Lock(0, 0, (BYTE**)&pIndices, 0);
	//loop through and move them in.  I could do a memcpy if I really wanted to change around the structures, but
	//for an average of 800 faces on a one time copy? no.
	
	
	for (int i=0; i < m_i_total_faces; i++)
	{
		pIndices[i*3] = m_3DModel->pObject[0].pFaces[i].vertIndex[0];
		pIndices[(i*3)+1] = m_3DModel->pObject[0].pFaces[i].vertIndex[1];
		pIndices[(i*3)+2] = m_3DModel->pObject[0].pFaces[i].vertIndex[2];
	    
	}

  	m_index_buff->Unlock();
   	*/
	
	//now do a vertice buffer.  We'll be writing over it every frame for each md2, we kind of have to for the
    //interpolation.  You got a better idea?
	
   	
    int	dwSizeOfVertices = C_ANIM_RTM_MAX_VERT * sizeof(VERTEX_XYZ_NORMAL_TEX1);
   	// Create the vertex buffer
	if( FAILED(hr= cls_tglo.p_d3dDevice->CreateVertexBuffer (	dwSizeOfVertices, 0,
													D3DFVF_XYZ_NORMAL_TEX1, 
													D3DPOOL_SYSTEMMEM, 
													&g_rtm_vert_buff ) ) )
	{
 	    log_error("CResourceRTM (%s)", DXGetErrorString8(hr));
		return;
	}

		//we'll copy data in at a later date.  It's kind of a scratch buffer for us.
   
   //MD2_MAX_VERTICES
}

  
void CResourceRTM::dx_restore()
{
 if (!g_rtm_vert_buff)  dx_create();

}



int CResourceRTM::Draw(int i_anim, int i_frame, float t, DWORD dw_color, int i_interpolate_mode)
{
    if (!g_rtm_vert_buff)  dx_create();
  
	if ( m_model.GetFrameCount() < 1)
		{
			log_error("Model %s has no frames of animation!", GetName());
			return 0;
		}
   	if (i_anim > m_model.GetAnimations()-1)
	{
		return 0; //anim is out of range.
	}



	//copy info into our thing
	static VERTEX_XYZ_NORMAL_TEX1	*pVertices = NULL;
	
	g_rtm_vert_buff->Lock(0, 0, (BYTE**)&pVertices, 0);
   	
	int i_vertex_count = 0;


	// Here we grab the current animation that we are on from our model's animation list
	CRTMAnim *pAnim = m_model.GetPointerToAnim(i_anim);

	//add the real index of the frame.  this way they can always say "0" for the first frame of that
	//anim.
	i_frame += pAnim->i_start;

	// This gives us the current frame we are on.  We mod the current frame plus
	// 1 by the current animations end frame to make sure the next frame is valid.
	// If the next frame is past our end frame, then we go back to zero.  We check this next.
	static int nextFrame;
	if (i_interpolate_mode == C_ANIM_INTERPOLATE_MODE_FORWARD)
	{
		nextFrame = ( (i_frame) + 1);
		if (nextFrame > pAnim->i_end) nextFrame = pAnim->i_start;
	}
	else
	if (i_interpolate_mode == C_ANIM_INTERPOLATE_MODE_BACKWARDS)
	{
		nextFrame = ( (i_frame) - 1);
		if (nextFrame > pAnim->i_start) nextFrame = pAnim->i_end;
	}  else
	{
		log_error("Unsupported interpolation mode");
		return 0;
	}

	// If the next frame is zero, that means that we need to start the animation over.
	// To do this, we set nextFrame to the starting frame of this animation.

	// Get the current key frame we are on
	CRTMFrame *pFrame =	 m_model.GetPointerToFrame(i_frame);

	// Get the next key frame we are interpolating too
	CRTMFrame *pNextFrame =  m_model.GetPointerToFrame(nextFrame);


		// Go through all of the faces (polygons) of the current frame and draw them
		for(int j = 0; j < m_model.m_l_faces; j++)
		{
				  // Pass in the texture coordinate for this vertex
					pVertices[j].tu = m_model.m_p_a_tex[j].x;
					pVertices[j].tv = m_model.m_p_a_tex[j].y;
	
				// Store the current and next frame's vertex
				   static D3DXVECTOR3 vPoint1;
				   static D3DXVECTOR3 vPoint2;

				vPoint1	= pFrame->p_verts[ m_model.m_p_a_faces[j]];
				vPoint2 = pNextFrame->p_verts[m_model.m_p_a_faces[j]];
				
				// By using the equation: p(t) = p0 + t(p1 - p0), with a time t
				// passed in, we create a new vertex that is closer to the next key frame.
		
				pVertices[j].position.x = vPoint1.x + t * (vPoint2.x - vPoint1.x);
				pVertices[j].position.y =  vPoint1.y + t * (vPoint2.y - vPoint1.y), // Find the interpolated Y
				pVertices[j].position.z = vPoint1.z + t * (vPoint2.z - vPoint1.z);// Find the interpolated Z
		
				//set the normal
			pVertices[j].normal = m_model.m_p_a_normals[j];

		}
	  	g_rtm_vert_buff->Unlock();

	cls_tglo.p_d3dDevice->SetVertexShader (D3DFVF_XYZ_NORMAL_TEX1);
	cls_tglo.p_d3dDevice->SetStreamSource (0, g_rtm_vert_buff, sizeof(VERTEX_XYZ_NORMAL_TEX1));

    
    
     static D3DMATERIAL8 mat_orig;
    cls_tglo.p_d3dDevice->GetMaterial(&mat_orig);
    m_mat.Use(cls_tglo.p_d3dDevice);
    
    //set any special render modes
     bool b_tint_active = false;
 

     if (dw_color != D3DCOLOR_ARGB(255,255,255,255))
  {
	  //we need to enable color alpha/tinting support

	  	//transparency test
			cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	    	cls_tglo.p_d3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, dw_color);
			cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
			cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);


			cls_tglo.p_d3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT);
			cls_tglo.p_d3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	    
		    cls_tglo.p_d3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	
			b_tint_active = true;

            cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
            cls_tglo.p_d3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
            cls_tglo.p_d3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
            
            
            //let's force only 2 bit alpha basically, stops the bilinear filtering from
            //blurring around the edges.
         	if( cls_tglo.p_d3dapp->m_d3dCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL )
            {   
            cls_tglo.p_d3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
            cls_tglo.p_d3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER );
            cls_tglo.p_d3dDevice->SetRenderState(D3DRS_ALPHAREF,  0x08); // tolerance
            }
				
     } 
    
  
    
    
    cls_tglo.p_d3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 
										0, m_model.m_l_faces/3); // number of primitives
  if (b_tint_active)
  {
      //turn off the tinting stuff  
      //	  cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
      //	  cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
      cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1,   D3DTA_TEXTURE );
      cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2,   D3DTA_DIFFUSE );
      cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,   D3DTA_TEXTURE );
      cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2,   D3DTA_DIFFUSE );
      cls_tglo.p_d3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
      cls_tglo.p_d3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR,D3DCOLOR_ARGB(255,255,255,255));
  }
	
 cls_tglo.p_d3dDevice->SetMaterial(&mat_orig);
			return 0; //normal
} 

int CResourceRTM::GetFramesInAnim(int i_anim)
{
	if (i_anim >= m_model.GetAnimations())
	{
		log_error("Anim %d is out of range in RTM file.", i_anim);
		return 0;
	}
	
	return (*m_model.m_p_a_anim)[i_anim].i_length;
}

int CResourceRTM::GetAnimByName(	char *st_anim)
{
	return m_model.GetAnimIndexByName(st_anim);
}


//code to use to just test out the rendering stuff fast:

	  
   /*
		//test out the vertices
        cls_tglo.p_d3dDevice->SetVertexShader(D3DFVF_XYZ_NORMAL_TEX1);
		VERTEX_XYZ_NORMAL_TEX1 a_vert[3];
	
		
		for (int i=0; i <m_model.m_l_faces; i += 3 )
		{
		
			a_vert[0].position = (*m_model.m_p_a_frame)[0]->p_verts[m_model.m_p_a_faces[i]];
			a_vert[1].position = (*m_model.m_p_a_frame)[0]->p_verts[m_model.m_p_a_faces[i+1]];
		    a_vert[2].position = (*m_model.m_p_a_frame)[0]->p_verts[m_model.m_p_a_faces[i+2]];
		
				
			a_vert[0].tu =  m_model.m_p_a_tex[i].x;
			a_vert[0].tv = m_model.m_p_a_tex[i].y;

			a_vert[1].tu = m_model.m_p_a_tex[i+1].x;
			a_vert[1].tv = m_model.m_p_a_tex[i+1].y;

			a_vert[2].tu =  m_model.m_p_a_tex[i+2].x;
			a_vert[2].tv =  m_model.m_p_a_tex[i+2].y;


			//add normal info
			if (m_model.m_p_a_normals > 0)
			{
			
			a_vert[0].normal = m_model.m_p_a_normals[i];
			a_vert[2].normal = m_model.m_p_a_normals[i+1];
			a_vert[1].normal = m_model.m_p_a_normals[i+2];

			} else
			{
				log_msg("No normals");
			}


		cls_tglo.p_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, 
		&a_vert, sizeof(VERTEX_XYZ_NORMAL_TEX1));
		
		}   
		
		  	*/


