/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



//Draws a LOT of little boxes on the map

#include "CVisual.h"
#include "CThing_globals.h"
#include "CMap.h"

//special tile vertex information

const int C_TILE_GRID_VBS = 9; //can use up to 9 vertex buffers for this

//very simple, we're just going to overlay color

const DWORD D3DFVF_TILE_GRID = ( D3DFVF_XYZ | D3DFVF_DIFFUSE );

class CGridVertex
{
public:
  //our ultra small vertex format, we don't need textures or normals
	
  D3DXVECTOR3 vec_pos;
  DWORD dw_color; //The color

};

class CGridTile : public CVisual
{
public:
	CGridTile();
    virtual ~CGridTile();
   	virtual void dx_draw();
	virtual void dx_kill();
	virtual void dx_restore();
	virtual void dx_invalidate();
   	virtual void dx_create();
	virtual void think();
    void CGridTile::delete_local_vertex_info();

	void CGridTile::create_vertex_buffer();
	void CGridTile::create_vertex_information();
	void CGridTile::create_vertex_info_for_tile(int i_x, int i_z);
	float CGridTile::get_tile_height_at_point(float f_x, float f_z);
    void CGridTile::update_tile(int x, int y);
    void CGridTile::update_tile_in_vertex_buffer(int x, int y);
   void CGridTile::update_block_of_tiles(int i_x, int i_y);
   void CGridTile::SetZoneHighlight(int i_zone);
   void CGridTile::UpdateAllTiles();
private:

  	LPDIRECT3DVERTEXBUFFER8 p_vertex_buffer[C_TILE_GRID_VBS]; //use up to nine vertex buffers to support 100X100
	//landscape, this is pretty much the max with most cards 64,000 per VB limit and I don't want to
	//do multiple VB's with CTerrain right now
    CMapZone *p_map; //remember where the map is
	int i_vertexes; //how many total verts we need
	CGridVertex *p_vertex; //our local copy of the vertex buffer, better than trying to write it
	//directly to the VB, easier to catch bugs.
    int i_vbs_used; //how many vertex buffers we're using right now
	int i_max_verts_per_buf;
	int m_i_zone_highlight;
    bool m_b_update_zone_highlights;
};
