/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CResource3DS.h"
//#include "3dsimpexp.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CResource3DS::CResource3DS()
{
 	//defaults
    m_pPMeshes           = NULL;
    m_cPMeshes           = 0;
    m_pPMeshFull         = NULL;
    m_dwNumMaterials     = 0L;
    m_mtrlMeshMaterials  = NULL;
	m_fObjectRadius = 0;
	m_vObjectCenter = D3DXVECTOR3(0,0,0);
	m_texture_id_array = NULL;
	
	
vec_bb_min = D3DXVECTOR3(0,0,0); //bounding box
vec_bb_max = D3DXVECTOR3(0,0,0);


}

CResource3DS::~CResource3DS()
{
  dx_invalidate();
}


void CResource3DS::dx_create()
{
 /*   	
	if (get_refs() < 1) return; //nobody needs us, why should we load?
	log_msg("Loading 3ds file %s.", this->GetName());
    import3ds mesh;
	
 	mesh.import(GetName(), 1);
  */
  }


void CResource3DS::dx_invalidate()
{
 
}


void CResource3DS::dx_restore()
{
 }


void CResource3DS::draw(bool b_lod_active, int i_lod)
{

}