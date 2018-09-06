/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CResourceMD2.h"
#include "md2.h"
#include "d3dhelperfuncs.h"
#include "d3dapp.h"
#include <DXErr8.h>


#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CResourceMD2::CResourceMD2()
{
	m_3DModel = NULL;
	if (g_i_md2_vert_ref == 0) g_vert_buff = NULL;
	g_i_md2_vert_ref++;
}


CResourceMD2::~CResourceMD2()
{
   dx_invalidate();

	for(int i = 0; i < m_3DModel->numOfObjects; i++)
	{
			// Free the faces, normals, vertices, and texture coordinates.
		if(m_3DModel->pObject[i].pFaces)		delete [] m_3DModel->pObject[i].pFaces;
		if(m_3DModel->pObject[i].pNormals)	delete [] m_3DModel->pObject[i].pNormals;
		if(m_3DModel->pObject[i].pVerts)		delete [] m_3DModel->pObject[i].pVerts;
		if(m_3DModel->pObject[i].pTexVerts)	delete [] m_3DModel->pObject[i].pTexVerts;
	}
	
    SAFE_DELETE(m_3DModel);
	g_i_md2_vert_ref--;
	if (g_i_md2_vert_ref == 0) 
	{
		SAFE_RELEASE(g_vert_buff);
	}
}

bool CResourceMD2::InitMesh()
{
    
    if (!m_3DModel)
    {
        m_3DModel = new t3DModel();
        CLoadMD2 m_LoadMd2;
        
        char st_fname[256];
        strcpy(st_fname, GetName());
        if (!exist(st_fname))
        {
            sprintf(st_fname, "%s%s", GetExeDir(), GetName());
        }
        
        
        //load the mesh from our name
        if (!m_LoadMd2.ImportMD2(m_3DModel, st_fname, ""))
        {
            ///error loading, it told it
            return false;
        }
        
        
        //let's also compute some general information about this model
        HRESULT hr;
        
        int i_anim_to_compute_with = 0;
        hr = D3DXComputeBoundingSphere( m_3DModel->pObject[i_anim_to_compute_with].pVerts,
            m_3DModel->pObject[i_anim_to_compute_with].numOfVerts,
            D3DFVF_XYZ,  &m_vObjectCenter, &m_fObjectRadius );
        
        if (FAILED(hr)) log_error("Error computing bounding sphere for %s (%s)", GetName(), DXGetErrorString8(hr));
        //m_fObjectRadius /= 2;	//seems too big.  here is a hack to make it "feel" right.
        //also let's get the bounding box
        
        hr = D3DXComputeBoundingBox( m_3DModel->pObject[i_anim_to_compute_with].pVerts, 
            m_3DModel->pObject[i_anim_to_compute_with].numOfVerts,
            D3DFVF_XYZ, &this->vec_bb_min, &vec_bb_max);
        if (FAILED(hr)) log_error("Error computing bounding box for %s (%s)", GetName(), DXGetErrorString8(hr));
        
    } else
    {
        
        //already loaded
    }
    
    return true; //success
}


void CResourceMD2::dx_invalidate()
{
 	  //free everything
    SAFE_RELEASE(g_vert_buff);
	b_loaded = false;
} 


void CResourceMD2::dx_create()
{
	if (get_refs() < 1) return; //nobody needs us, why should we load?
    if (g_vert_buff) return; //destroy if it's loaded
 	if(m_3DModel->pObject.size() <= 0) return;


   //let's build our index buffer.
  HRESULT hr;
   m_i_total_faces =  m_3DModel->pObject[0].numOfFaces; //we can assume this.

   log_msg("Total faces for %s is %d.  %d tex verts.", GetName(), m_i_total_faces, m_3DModel->pObject[0].numTexVertex);
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
	
   	
    int	dwSizeOfVertices = C_ANIM_MAX_VERT * sizeof(VERTEX_XYZ_NORMAL_TEX1);
   	// Create the vertex buffer
	if( FAILED(hr= cls_tglo.p_d3dDevice->CreateVertexBuffer (	dwSizeOfVertices, 0,
													D3DFVF_XYZ_NORMAL_TEX1, 
													D3DPOOL_SYSTEMMEM, 
													&g_vert_buff ) ) )
	{
 	    log_error("MD2 (%s)", DXGetErrorString8(hr));
		return;
	}

		//we'll copy data in at a later date.  It's kind of a scratch buffer for us.
   
   //MD2_MAX_VERTICES
}

  
void CResourceMD2::dx_restore()
{
 if (!g_vert_buff)  dx_create();

}



int CResourceMD2::Draw(int i_anim, int i_frame, float t, DWORD dw_color, int i_interpolate_mode)
{
    //fix base anim

	
	
	if (!g_vert_buff)  dx_create();
	// Make sure we have valid objects just in case. (size() is in the vector class)
	if(m_3DModel->pObject.size() <= 0) return -1;

	//copy info into our thing
	static VERTEX_XYZ_NORMAL_TEX1	*pVertices = NULL;
	
	g_vert_buff->Lock(0, 0, (BYTE**)&pVertices, 0);
   	
	int i_vertex_count = 0;

	// Here we grab the current animation that we are on from our model's animation list
	tAnimationInfo *pAnim = &(m_3DModel->pAnimations[i_anim]);
   
	//adjust frame for offset (we got sent a 0 based one)
	i_frame += pAnim->startFrame;

	// This gives us the current frame we are on.  We mod the current frame plus
	// 1 by the current animations end frame to make sure the next frame is valid.
	// If the next frame is past our end frame, then we go back to zero.  We check this next.
	static int nextFrame;
	if (i_interpolate_mode == C_ANIM_INTERPOLATE_MODE_FORWARD)
	{
		nextFrame = ( (i_frame) + 1) % pAnim->endFrame;
	}
	else
	if (i_interpolate_mode == C_ANIM_INTERPOLATE_MODE_BACKWARDS)
	{
	
	nextFrame = ( (i_frame) - 1) % pAnim->endFrame;
	}  else
	{
		log_error("Unsupported interpolation mode");
		return 0;
	}

	// If the next frame is zero, that means that we need to start the animation over.
	// To do this, we set nextFrame to the starting frame of this animation.
	if(nextFrame == 0) 
	{
		nextFrame =  pAnim->startFrame;
	    if (nextFrame == -1)
		{
			//it's only a 1 frame animation? woah that sucks
			nextFrame = 0;
		}
	}
	
	// Get the current key frame we are on
	t3DObject *pFrame =	 &m_3DModel->pObject[i_frame];

	// Get the next key frame we are interpolating too
	t3DObject *pNextFrame =  &m_3DModel->pObject[nextFrame];

	// Get the first key frame so we have an address to the texture and face information
	t3DObject *pFirstFrame = &m_3DModel->pObject[0];


		// Go through all of the faces (polygons) of the current frame and draw them
		for(int j = 0; j < pFrame->numOfFaces; j++)
		{
			// Go through each corner of the triangle and draw it.
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				// Get the index for each point of the face
				int vertIndex = pFirstFrame->pFaces[j].vertIndex[whichVertex];

				// Get the index for each texture coordinate for this face
				int texIndex  = pFirstFrame->pFaces[j].coordIndex[whichVertex];
						
				// Make sure there was a UVW map applied to the object.  Notice that
				// we use the first frame to check if we have texture coordinates because
				// none of the other frames hold this information, just the first by design.
			
					// Pass in the texture coordinate for this vertex
					pVertices[i_vertex_count].tu = pFirstFrame->pTexVerts[ texIndex ].x;
					pVertices[i_vertex_count].tv = pFirstFrame->pTexVerts[ texIndex ].y;
			        if (pFirstFrame->pNormals)
                    {
                    
                    pVertices[i_vertex_count].normal.x = -pFirstFrame->pNormals[vertIndex].x;
			        pVertices[i_vertex_count].normal.y = -pFirstFrame->pNormals[vertIndex].y;
			        pVertices[i_vertex_count].normal.z = -pFirstFrame->pNormals[vertIndex].z;
                    
                     //pVertices[i_vertex_count].normal.x = 1;
                     //pVertices[i_vertex_count].normal.y = 0;
                     //pVertices[i_vertex_count].normal.z = 0;
                    }

				// Now we get to the interpolation part! (*Bites his nails*)
				// Below, we first store the vertex we are working on for the current
				// frame and the frame we are interpolating too.  Next, we use the
				// linear interpolation equation to smoothly transition from one
				// key frame to the next.
			
				// Store the current and next frame's vertex
				CVector3 vPoint1 = pFrame->pVerts[ vertIndex ];
				CVector3 vPoint2 = pNextFrame->pVerts[ vertIndex ];
				
				// By using the equation: p(t) = p0 + t(p1 - p0), with a time t
				// passed in, we create a new vertex that is closer to the next key frame.
		
				pVertices[i_vertex_count].position.x = vPoint1.x + t * (vPoint2.x - vPoint1.x);
				pVertices[i_vertex_count].position.y =  vPoint1.y + t * (vPoint2.y - vPoint1.y), // Find the interpolated Y
				pVertices[i_vertex_count].position.z = vPoint1.z + t * (vPoint2.z - vPoint1.z);// Find the interpolated Z
		
		  	 	i_vertex_count++;
			  }
 			
		}
	  	g_vert_buff->Unlock();

	cls_tglo.p_d3dDevice->SetVertexShader (D3DFVF_XYZ_NORMAL_TEX1);
	cls_tglo.p_d3dDevice->SetStreamSource (0, g_vert_buff, sizeof(VERTEX_XYZ_NORMAL_TEX1));
	
    
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
										0, i_vertex_count/3); // number of primitives

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
	

    return 0; //normal
} 

int CResourceMD2::GetFramesInAnim(int i_anim)
{
	if (i_anim >= m_3DModel->numOfAnimations)
	{
		log_error("Anim %d out of range.", i_anim);
		return 0;
	}
	return m_3DModel->pAnimations[i_anim].endFrame - m_3DModel->pAnimations[i_anim].startFrame;
}

int CResourceMD2::GetAnimByName(char *st_anim)
{
	 //loop through our animation names and see if we match one.

	for (int i=0; i < m_3DModel->numOfAnimations; i++)
	{
		if (stricmp(m_3DModel->pAnimations[i].strName, st_anim) == 0)
		{
			//found match
			return i;
		}
	}
   log_msg("Unable to find animation %s in MD2.", st_anim);
   return 0;
}         


