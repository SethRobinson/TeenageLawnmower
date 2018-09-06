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
#pragma warning (disable:4786)
#include <vector>
using namespace std;

#include "CManager.h" 

//handles textures
//We need to be able to handle multiple "texture settings" for the same mesh, for instance, we may
//want to use the same house model with 4 different textures on the same level, but we don't want
//to load the .x file for times.  
//We will use an stl vector to push new texture settings into an index.  The index # is for speed,
//but to figre out if we're modifying an existing texture setting or need to make a new one we go
//by english names.  

//Texture setting 0 is the original, named "ORIGINAL" always.  This should never be changed.


class CTextureInfo
{

public:
	 char st_tex[256]; //original texture name, if any
	 int i_tex; // remember the tex's id
	 bool b_alpha;
	 LPDIRECT3DTEXTURE8 p_tex; //actual texture handle
	 CTextureInfo()
	 {
		 st_tex[0] = 0;
		 i_tex = C_RESOURCE_NONE; //no texture
	
		 Reset();
	 }

     void Reset()
	 {
 		 //force it to get recached
		 p_tex = NULL; //no tex handle
		 b_alpha = false;
	 }  
     void ReleaseTextureReference()
     {
       Reset();
       i_tex = C_RESOURCE_NONE;
     }

};

class CTexSetting
{

public:
   char st_name[32]; //name of this texture setting
   CTextureInfo*       m_texture_id_array;
  
   CTexSetting(int i_tex_count)
   {
	   st_name[0] = 0; //clear it out
	   m_texture_id_array = new CTextureInfo[i_tex_count];
   }

   ~CTexSetting()
   {
	   SAFE_DELETE_ARRAY(m_texture_id_array);
   }

};


typedef vector<CTexSetting*> TexInfoVec;

class CResourcePMesh: public CResource
{

public:
 	D3DXVECTOR3 vec_bb_min; //bounding box
	D3DXVECTOR3 vec_bb_max;
	D3DXVECTOR3         m_vObjectCenter;        // Center of bounding sphere of object
    FLOAT               m_fObjectRadius;        // Radius of bounding sphere of object

bool CResourcePMesh::AccurateRayCheck(D3DXVECTOR3 *p_vec_origin, D3DXVECTOR3 *p_vec_dir, D3DXVECTOR3 *p_out_hit);


 virtual void dx_create();
 virtual void dx_restore();
 virtual void dx_invalidate();
void CResourcePMesh::draw(bool b_lod_active, int i_lod, int i_texture_set, DWORD dw_color, int i_render_extra);
bool CResourcePMesh::HasAlpha(int i_texture_set);
int CResourcePMesh::ReplaceTexture(char *st_tex_system, char *st_old, char *st_new);

 CResourcePMesh();
 virtual ~CResourcePMesh();

private:
    TCHAR               str_path[512]; // Relative path of mesh
    LPD3DXPMESH*        m_pPMeshes;          
    LPD3DXPMESH         m_pPMeshFull;          
    DWORD               m_cPMeshes;
    DWORD               m_iPMeshCur;

    D3DMATERIAL8*       m_mtrlMeshMaterials;
 	DWORD               m_dwNumMaterials;       // Number of materials
   	TexInfoVec m_vec_texinfo; //resizable array of texture settings for sharing models with different tex
   void CResourcePMesh::InitTexture(int i_tex_index, int i_index, bool b_load_now);
   bool CResourcePMesh::HasAlphaSpecific(int i_texture_set, int i_tex);
};	 


