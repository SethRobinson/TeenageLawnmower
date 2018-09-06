/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


//	History:    Originally by Neil Haran, modified to fit in my class
 
#include "CResourceSphere.h"
#include "CManager.h" 
#include <DXErr8.h>

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/******************************************************************************
	Construction/Destruction
 *****************************************************************************/

CResourceSphere::CResourceSphere()
{
m_uVertices = 0;
m_uNumRings = 0;
m_uNumSegments = 0;

	m_pVBSphere = NULL;
	strcpy(this->szTextureName, "texture\\sky.dds");
	i_tex_id = C_RESOURCE_NONE;
	 p_resource = NULL;
	mat.SetPower(22);
	mat.SetSpecular(1,1,1,1);

}

CResourceSphere::~CResourceSphere()
{
	if (i_tex_id != ::C_RESOURCE_NONE)
	cls_tglo.p_manager->delete_ref(i_tex_id);

	if(m_pSphereVertices != NULL) 
	{
        SAFE_DELETE_ARRAY( m_pSphereVertices );
	}
	SAFE_RELEASE( m_pVBSphere );
	
}


/******************************************************************************
	Name:	GenerateSphere
	Desc:	Generates the Vertices and indices for the Sphere.

	Param1: float x_scale		   - The scaling of the sphere in the X axis.
	Param2: float y_scale	       - The scaling of the sphere in the Y axis.
	Param3: float y_scale		   - The scaling of the sphere in the Z axis.

	Param4:	float tex_scale		   - The scaling of the texture on the sphere.
	Param5:	LPVERTEX3D* p_Vertices - A pointer to a container for the vertices.

	Param6: UINT* p_NumVertices	   - A pointer to store the number of vertices.

	Param7: bool hemisphere		   - The option of having a hemisphere instead
									 of a full sphere.
	Param8: D3DCOLOR dcolor		   - The diffuse color component of the sphere.


 *****************************************************************************/
void CResourceSphere::GenerateSphere( float fXScale, float fYScale, float fZScale, float tex_scale, 
							 LPVERTEX3D* p_Vertices, UINT* p_NumVertices, bool hemisphere, 
							 D3DCOLOR dcolor )
{
	float fRingAngle;
	float fSegAngle;
	float TWOPI;
	float fTScale;
	DWORD ring;
	DWORD seg;

	LPVERTEX3D pVertices;
        
	//the angle at which the rings of the sphere are positioned around 
	//the latitude lines of the sphere.
	fRingAngle = (D3DX_PI / m_uNumRings);
	//The angle at which the segments are marked on each circle.
	fSegAngle  = (2.0f * D3DX_PI / m_uNumSegments);
	TWOPI = (D3DX_PI * 2.0f);
	fTScale = 4.0f; //texture scale factor

	//check to draw only half-sphere (hemisphere)
	//Since we are dealing with drawing two halves of a sphere, it seems logical
	//to provide this option.
	if(hemisphere) 
	{
		m_uNumRings = m_uNumRings / 2;
	}

    //alloc memory for the sphere
    pVertices = new Vertex3D[2 * m_uNumRings * (m_uNumSegments+1) * sizeof(VERTEX3D)];

	// 2 times ( because we have TWO Hemispheres ) the number of rings, 
	// times the number of segments... 
    *p_NumVertices = 2 * m_uNumRings * (m_uNumSegments+1);
    *p_Vertices = pVertices;

    //generate the group of rings for the sphere
	//credit: The sphere math for the vertices, was taken from
	//the bump earth example..

	//Modifications:	1) Added my own texture information.
	//					2) Added my own custom features like the hemisphere.
	//					3) Modified scaling of the vertices.

    for( ring = 0; ring < m_uNumRings; ring++ )
    {    
        float r0 = (float)sin( (ring + 0) * fRingAngle );
        float r1 = (float)sin( (ring + 1) * fRingAngle );
        float z0 = (float)cos( (ring + 0) * fRingAngle );
        float z1 = (float)cos( (ring + 1) * fRingAngle );
        //generate the group of segments for the current ring

        for( seg = 0; seg < (m_uNumSegments+1); seg++ )
        {
            float x0 =  r0 * (float)sin( seg * fSegAngle );
            float y0 =  r0 * (float)cos( seg * fSegAngle );
            float x1 =  r1 * (float)sin( seg * fSegAngle );
            float y1 =  r1 * (float)cos( seg * fSegAngle );
            
			// Add two vertices to the strip which makes up the sphere
            pVertices->setX( (fXScale * x0) );
            pVertices->setY( (fYScale * y0) );
            pVertices->setZ( (fZScale * z0) );
            pVertices->setNormal( x0, y0, z0 );
            pVertices->setColor( dcolor );

			if(ring < m_uNumRings/2 || hemisphere) 
			{  
				// Mirror the texture over the top and bottom
				//Basically it runs through each segment and in a circular fashion and
				//generates values for u and v based on the range from 0 to 1.
				// if u is 0, v is 1.. they intersect at 0.5... 
				// Because as sin approaches 90 degrees, it gets closer to 1 and cos gets
				// closer to 0.
				pVertices->setTexCoords( (float)((0.5f + 0.5f*(sin(-(float)seg*TWOPI/m_uNumSegments)*
										 (float)(ring+0)/(float)m_uNumRings/2.0f))/tex_scale) * 
										 fTScale, 
										 
										 (float)((0.5f + 0.5f*(cos(-(float)seg*TWOPI/m_uNumSegments)*
										 (float)(ring+0)/(float)m_uNumRings/2.0f))/tex_scale) * 
										 fTScale );
			} 
			else 
			{
				//Doing exactly backwards here.
				pVertices->setTexCoords( (float)((0.5f + 0.5f*(sin(-(float)seg*TWOPI/m_uNumSegments)*
										 (float)(m_uNumRings-(ring+0))/(float)m_uNumRings/2.0f))/tex_scale) * 
										 fTScale,

										 (float)((0.5f + 0.5f*(cos(-(float)seg*TWOPI/m_uNumSegments)*
										 (float)(m_uNumRings-(ring+0))/(float)m_uNumRings/2.0f))/tex_scale) * 
										 fTScale );
			}

            *pVertices++;
            pVertices->setX( (fXScale * x1) );
            pVertices->setY( (fYScale * y1) );
            pVertices->setZ( (fZScale * z1) );
            pVertices->setNormal( x1, y1, z1 );
            pVertices->setColor( dcolor );

			if(ring < m_uNumRings/2 || hemisphere) 
			{  
				// Mirror the texture over the top and bottom.
				pVertices->setTexCoords( (float)((0.5f + 0.5f*(sin(-(float)seg*TWOPI/m_uNumSegments)*
										 (float)(ring+1)/(float)m_uNumRings/2.0f))/tex_scale) * 
										 fTScale, 
										 
										 (float)((0.5f + 0.5f*(cos(-(float)seg*TWOPI/m_uNumSegments)*
										 (float)(ring+1)/(float)m_uNumRings/2.0f))/tex_scale) * 
										 fTScale );
			} 
			else 
			{
				pVertices->setTexCoords( (float)((0.5f + 0.5f*(sin(-(float)seg*TWOPI/m_uNumSegments)*
										 (float)(m_uNumRings-(ring+1))/(float)m_uNumRings/2.0f))/tex_scale) * 
										 fTScale,
										 
										 (float)((0.5f + 0.5f*(cos(-(float)seg*TWOPI/m_uNumSegments)*
										 (float)(m_uNumRings-(ring+1))/(float)m_uNumRings/2.0f))/tex_scale) * 
										 fTScale );
			}

            *pVertices++;           
        }
    }
}

/******************************************************************************
	Name:	 Create
	Desc:	 Initializes the size and shape of the Sphere.
	Param1:  float xCoord		- The coordinate of the sphere in the X axis.
	Param2:  float yCoord		- The coordinate of the sphere in the Y axis.
	Param3:  float zCoord		- The coordinate of the sphere in the Z axis.

	Param4:  float x_scale		- The scaling of the sphere in the X axis.
	Param5:  float y_scale		- The scaling of the sphere in the Y axis.
	Param6:  float y_scale		- The scaling of the sphere in the Z axis.
	
	Param7:  UINT num_segments	- The number of segments in the sphere.
	Param8:  UINT num_rings		- The number of rings in the sphere.

	Param9:	 float tex_scale	- The scaling of the texture on the sphere.
	Param10: bool hemisphere	- The option of having a hemisphere instead of a
								  full sphere.

	Param11: D3DCOLOR dcolor	- The diffuse color component of the sphere.
	

 *****************************************************************************/
void CResourceSphere::Create( float x_scale, 
					 float y_scale, float z_scale, UINT num_segments, UINT num_rings, 
					 float tex_scale, bool hemisphere, D3DCOLOR dcolor )
{

	//the sphere will have an equal number of rings and segments.
	m_uNumRings = num_rings;
	m_uNumSegments = num_segments;

	//Generate the sphere data
	GenerateSphere( x_scale, y_scale, z_scale, tex_scale, &m_pSphereVertices, 
					&m_uVertices, hemisphere, dcolor );

   
}

void CResourceSphere::dx_create()
{
	if (get_refs() < 1) return; //nobody needs us, why should we load?


	if (m_pVBSphere) return;
	
	HRESULT hr;

	// Create the vertex buffer, and then copy the vertices into the vertex buffer.
	if( FAILED(hr = cls_tglo.p_d3dDevice->CreateVertexBuffer (	m_uVertices * sizeof(VERTEX3D), D3DUSAGE_WRITEONLY,
													D3DFVF_VERTEX3D, 
													D3DPOOL_DEFAULT, 
													&m_pVBSphere ) ) )
	{
		  	    log_error("CSphere (%s)", DXGetErrorString8(hr));

	  return;
	}
  
	/*
	// Load up the texture
	if( FAILED( hr =D3DXCreateTextureFromFile( cls_tglo.p_d3dDevice, szTextureName, &m_pTexture ) ) )
	{
	  DXTRACE_ERR_NOMSGBOX( "CGrid", hr );
	  return;
	}
	*/
  i_tex_id = cls_tglo.p_manager->get_dynamic_resource_by_string(C_RESOURCE_TEXTURE, 
	  szTextureName);

    //load it if it hasn't been
   cls_tglo.p_manager->load_if_needed(i_tex_id);
	

	// Lock the Vertex Buffer
	VOID* pVertices = NULL;
	m_pVBSphere->Lock(0, 0, (BYTE**)&pVertices, 0);
	memcpy( pVertices, m_pSphereVertices, m_uVertices * sizeof(VERTEX3D) );
	m_pVBSphere->Unlock();
	b_loaded = true;
}

/******************************************************************************
	Name:	Render
	Desc:	Renders the sphere.

 *****************************************************************************/
void CResourceSphere::dx_draw()
{
	// Reset the world transformation matrix
  	cls_tglo.p_d3dDevice->SetVertexShader (D3DFVF_VERTEX3D);
	cls_tglo.p_d3dDevice->SetStreamSource (0, m_pVBSphere, sizeof(VERTEX3D));
	mat.Use(cls_tglo.p_d3dDevice);
	// Initialize the Texture stage.
		CResourceTex *p_tex = (CResourceTex*)cls_tglo.p_manager->get(i_tex_id);
        cls_tglo.p_d3dDevice->SetTexture( 0, p_tex->get_texture());
              
	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_SPECULARENABLE, true );
	// Draw sphere... we subtract two because it is a traingle STRIP! and 
	// the first and last vertices make up 2 triangles! when there are 
	// only 4 vertices used!
	cls_tglo.p_d3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, m_uVertices - 2 );

 			cls_tglo.p_d3dDevice->SetRenderState( D3DRS_SPECULARENABLE, false );

  // cls_tglo.p_d3dDevice->SetMaterial(NULL);
}




void CResourceSphere::dx_draw_sky()
{
	// Reset the world transformation matrix
  	cls_tglo.p_d3dDevice->SetVertexShader (D3DFVF_VERTEX3D);
	cls_tglo.p_d3dDevice->SetStreamSource (0, m_pVBSphere, sizeof(VERTEX3D));
	mat.Use(cls_tglo.p_d3dDevice);

 //  cls_tglo.p_d3dDevice->SetRenderState( D3DRS_CULLMODE,     D3DCULL_CCW );
	
	//make sure we're going full speed here
//	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
  
	// Initialize the Texture stage.
		CResourceTex *p_tex = (CResourceTex*)cls_tglo.p_manager->get(i_tex_id);
        cls_tglo.p_d3dDevice->SetTexture( 0, p_tex->get_texture());
              
	cls_tglo.p_d3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, m_uVertices - 2 );

  //cls_tglo.p_d3dDevice->SetRenderState( D3DRS_CULLMODE,     D3DCULL_NONE ); //default is no culling
 
}



void CResourceSphere::dx_restore()
{
	
	dx_create(); 
}



void CResourceSphere::dx_invalidate()
{
	SAFE_RELEASE(m_pVBSphere);
		b_loaded = false;

}



