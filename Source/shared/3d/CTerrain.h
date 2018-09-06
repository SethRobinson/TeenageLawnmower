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
#include "CVisual.h"
#include "Vertex3D.h"
#include "CManager.h"
#include "CMap.h"

//this struct holds data about map blocks, we can get pointers and things in advance
//to make drawing faster

class terrain_block_info
{
  friend CTerrain;
  LPDIRECT3DTEXTURE8 tex;
  LPDIRECT3DTEXTURE8 p_overlay_tex;
  public:
  
	  void terrain_block_info::refresh_overlay_texture(int i_tex);
	  void	refresh_texture(int i_tex);
};


class CTerrain : public CVisual 
{
public:
	CTerrain();
	virtual ~CTerrain();
	//accessors
	inline float GetElementHeight(int x, int y) { return p_map->p_height_buffer[(y * (p_map->get_size_x() + 1)) + x];};
	//inline float get_height_vertex(int x, int y){return m_pTerrainVertices[(y * (p_map->get_size_x() + 1)) + x].getY();};
	void CTerrain::init_terrain_buffers();
	void CTerrain::get_tris_from_block(int i_block_x, int i_block_y, D3DXVECTOR3 *tri, D3DXVECTOR3 *tri2);
   bool CTerrain::find_ground_from_ray(D3DXVECTOR3 *p_vec_a,D3DXVECTOR3 *p_vec_b, D3DXVECTOR3 *p_vec_out_pos,
									D3DXPLANE *p_out_plane);
	bool CTerrain::is_valid_vertex(int x, int y);
	void CTerrain::set_normals_for_block(int i_x, int i_y);
	void CTerrain::update_block_in_vertex_buffer(int x, int y);
	float CTerrain::get_ground_and_y(float x, float z, D3DXVECTOR3 *vec_ground);
	//non accessors
 	void CTerrain::update_terrain_block_from_map(int i_x, int i_y);

	virtual void dx_restore();
	virtual void dx_kill();
	virtual void dx_draw();
	virtual void dx_create();
	virtual void dx_invalidate();
	virtual void think();
    float CTerrain::get_height(float x, float z);
  void set_map_source(CMapZone *p_new_map);
  void CTerrain::update_vertex_height(int x, int y);
  int get_blocks_per_texture() {return i_blocks_per_texture;};
  void set_blocks_per_texture(int i_new) {i_blocks_per_texture = i_new;};
  void CTerrain::update_block_information(int i_x, int i_y);
  virtual void receive_message(CMessageHeader *p_header, void *p_message);
	
protected:
	void GenerateCTerrain();
	D3DXVECTOR3 CTerrain::get_normal(int X, int Z);

	LPDIRECT3DVERTEXBUFFER8 m_pVBCTerrain;
	DWORD					m_dwSizeOfVertices;
	LPDIRECT3DINDEXBUFFER8	m_pIBCTerrain;
	LPVERTEX3D				m_pTerrainVertices;
	int						m_iVertexCount;
	int						m_iTriangleCount;				  
	CMapZone *p_map;
  	terrain_block_info *p_binfo; //terrain block info
	int i_blocks_per_texture;
    bool b_show_grid; //also draw a grid on top of it
};




