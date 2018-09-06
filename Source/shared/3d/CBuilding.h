/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CVisual.h"

const int C_WALL_MAPPING_TILE = 0;
const int C_WALL_MAPPING_STRETCH = 1;



class CMapZone;


const DWORD D3DFVF_BUILD3D = ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | 
							D3DFVF_TEX1);

class CBuildVertex
{
public:
    //our ultra small vertex format, we don't need textures or normals
    
    D3DXVECTOR3 vec_pos;
    D3DXVECTOR3 vec_normal;
    DWORD dw_color; //The color
    D3DXVECTOR2 vec_tex;
    
};


//just a place to compile important information on each tile, such as the textures

const int C_BUILDING_TEX_NORTH = 0;
const int C_BUILDING_TEX_WEST = 1;
const int C_BUILDING_TEX_EAST = 2;
const int C_BUILDING_TEX_SOUTH = 3;
const int C_BUILDING_TEX_TOP = 4;

const int C_BUILDING_TEX_PER_WALL = 5;


class CBuildingTile
{
public:
    LPDIRECT3DTEXTURE8 tex[C_BUILDING_TEX_PER_WALL];
    void CBuildingTile::refresh_texture(int i_side);
    bool b_wall_active[C_BUILDING_TEX_PER_WALL];
    int i_x, i_z;
    
};

class CRoofTile
{
public:
    LPDIRECT3DTEXTURE8 tex;
    void CRoofTile::refresh_texture();
    int i_x, i_z;
    
};


class CBuilding : public CVisual
{
    
public:
    
    void set_sub_zone_id(int i_new){i_sub_zone = i_new;};
    
    CBuilding();
    virtual ~CBuilding();
    
    void build_walls();
    void build_all_wall_quads();
    void build_wall_quads(int i_buf_wall, int i_x, int i_z);
    CMapZone *p_map;
    void dx_invalidate_walls();
    void dx_create_walls();
    void update_wall_vertex_data();
    void update_roof_vertex_data();
    void update_wall_tile(int i_x, int i_z);
    
    virtual void dx_draw();
    void dx_draw_walls();
    void dx_draw_roof();
    
    virtual void dx_create();
    virtual void dx_invalidate();
    virtual void dx_restore();
    
    void build_virt_quad_from_two_verts(CBuildVertex *p_vert, float f_height, int i_side);
    void set_quad_normal(CBuildVertex *p_vert, float nx, float ny, float nz);
    void compute_wall_visibility(int i_wall_count, int i_x, int i_z);
    void set_texture_info( int i_buf_wall, int i_x, int i_z);
    bool does_ray_hit_wall(int i_wall_to_check, D3DXVECTOR3 *p_point_a, D3DXVECTOR3 *p_point_b, int *i_out_wall_part,
        D3DXVECTOR3 *vec_out_hit, float *p_f_out_dist);
    void get_x_and_y_from_wall_array_num(int i_wall_num, int *p_x, int *p_y);
    void build_all_roof_tiles();
    void create_roof_quad(int i_roof);
    bool does_ray_hit_roof(int i_roof_to_check, D3DXVECTOR3 *p_point_a, D3DXVECTOR3 *p_point_b, int *i_out_wall_part,
        D3DXVECTOR3 *vec_out_hit, float *p_f_out_dist);
    void get_x_and_y_from_roof_array_num(int i_roof_num, int *p_x, int *p_y);
    
    int i_vertex_bytes;
    LPDIRECT3DVERTEXBUFFER8 p_vertex_buffer;
    int					     i_vertex_count;
    CBuildVertex				*p_local_verts;
    int i_walls;
    int i_sub_zone;
    int i_sides_per_wall;
    CBuildingTile *p_building_tile_info;
    
    //for the roof
    LPDIRECT3DVERTEXBUFFER8 p_vertex_buffer_roof;
    CBuildVertex *p_local_verts_roof;
    int i_roof_tiles;
    int i_vertex_count_roof;
    int i_vertex_bytes_roof;
    CRoofTile *p_building_roof_tile_info;
    
};