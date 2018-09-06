/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CResourceAnim.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//let all MD2's share the same system mem vert buffer
LPDIRECT3DVERTEXBUFFER8 g_vert_buff = NULL; //hold a buffer we can use over and over every frame
int g_i_md2_vert_ref = 0; //no one using it yet


LPDIRECT3DVERTEXBUFFER8 g_rtm_vert_buff = NULL; //hold a buffer we can use over and over every frame
int g_i_rtm_vert_ref = 0; //no one using it yet


CResourceAnim::CResourceAnim()
{
 	m_fObjectRadius = 0;
	m_vObjectCenter = D3DXVECTOR3(0,0,0);
		
    vec_bb_min = D3DXVECTOR3(0,0,0); //bounding box
    vec_bb_max = D3DXVECTOR3(0,0,0);
  	

}


CResourceAnim::~CResourceAnim()
{
  
}


void CResourceAnim::dx_create()
{

}

void CResourceAnim::dx_restore()
{

}
void CResourceAnim::dx_invalidate()
{

}

void CResourceAnim::dx_kill()
{

}
