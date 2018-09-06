#pragma once

#include "all.h"
#include "CThing_globals.h"
#include "CResource.h"
#include "assert.h"
#include "material.h"

#include "..\..\cal3d\cal3d\src\cal3d\cal3d.h"

class CCalControl; //forward dec

//some vertex information


const DWORD D3DFVF_CAL3D = ( D3DFVF_XYZ );

const DWORD D3DFVF_CAL3D_NORM = ( D3DFVF_NORMAL );

const DWORD D3DFVF_CAL3D_TEX = ( D3DFVF_TEX0 );



class CResourceCalRender: public CResource
{
private:
  float m_scale;
public:

 virtual void dx_create();
 virtual void dx_restore();
 virtual void dx_invalidate();
 virtual void dx_kill();

 CResourceCalRender();
 virtual ~CResourceCalRender();

 void CResourceCalRender::render_cal(CCalControl *p_controller, CalModel *p_cal_model);


	LPDIRECT3DVERTEXBUFFER8 p_vertex_buffer;
	WORD						i_max_verts;
	LPDIRECT3DINDEXBUFFER8	p_index_buffer;
	WORD						i_max_indexes;

	
   	LPDIRECT3DVERTEXBUFFER8 p_vertex_buffer_normals;
   	LPDIRECT3DVERTEXBUFFER8 p_vertex_buffer_tex;
	Material mat;
	
	DWORD p_vertex_shader;
};	 


