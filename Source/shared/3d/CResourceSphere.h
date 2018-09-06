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

#include "..\\all.h"
#include "CThing_globals.h"
#include "CResource.h"
#include "assert.h"
#include "Vertex3D.h"
#include "Material.h"

//handles textures
//Whether you want to draw a hemisphere, or a full sphere.
enum SPHERE_ATTRIBUTES { FULL_SPHERE, HEMI_SPHERE };

class CResourceSphere: public CResource
{
private:
 LPDIRECT3DTEXTURE8 p_resource; //the texture


public:

void CResourceSphere::Create( float x_scale, 
					 float y_scale, float z_scale, UINT num_segments, UINT num_rings, 
					 float tex_scale, bool hemisphere, D3DCOLOR dcolor );

	 
	
 virtual void dx_create();
 virtual void dx_restore();
 virtual void dx_invalidate();
 
 void SetSkyTexture(char *p_st){strcpy(szTextureName, p_st);}
 void CResourceSphere::dx_draw_sky();
  	virtual void dx_draw();
  	void GenerateSphere( float fXScale, float fYScale, float fZScale, float tex_scale, 
						 LPVERTEX3D* p_Vertices, UINT* p_NumVertices, bool hemisphere, 
						 D3DCOLOR dcolor ); 


 CResourceSphere();
 virtual ~CResourceSphere();
 bool sphere_has_been_generated(){if (m_uVertices != 0) return true; else return false;};

private:
    UINT		m_uNumRings;  //The number of rings.
    UINT		m_uNumSegments;  //The number of Segments.
	UINT		m_uVertices; //Number of vertices.

	LPVERTEX3D	m_pSphereVertices;
	LPDIRECT3DVERTEXBUFFER8 m_pVBSphere;
	char szTextureName[512];   
	int i_tex_id;
	Material mat; //the material we want to use.  Important!


};	 


