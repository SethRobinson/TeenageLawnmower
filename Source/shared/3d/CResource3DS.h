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


#include "CManager.h" 

//handles textures

class CResource3DS: public CResource
{
private:
    TCHAR               str_file[512]; // Filename of mesh
    TCHAR               str_path[512]; // Reletive path of mesh
    LPD3DXPMESH*        m_pPMeshes;          
    LPD3DXPMESH         m_pPMeshFull;          
    DWORD               m_cPMeshes;
    DWORD               m_iPMeshCur;

    D3DMATERIAL8*       m_mtrlMeshMaterials;
    int*                 m_texture_id_array;
	DWORD               m_dwNumMaterials;       // Number of materials
   

public:
 	D3DXVECTOR3 vec_bb_min; //bounding box
	D3DXVECTOR3 vec_bb_max;
	D3DXVECTOR3         m_vObjectCenter;        // Center of bounding sphere of object
    FLOAT               m_fObjectRadius;        // Radius of bounding sphere of object



 virtual void dx_create();
 virtual void dx_restore();
 virtual void dx_invalidate();
 void CResource3DS::draw(bool b_lod_active, int i_lod);

 CResource3DS();
 virtual ~CResource3DS();
};	 

