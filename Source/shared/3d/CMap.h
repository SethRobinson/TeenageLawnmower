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

#define GET_BLOCK_SMART(x, y) x+(y*cls_tglo.p_map->get_size_x())

#include "CThing_globals.h"
#include "CMapBlock.h"
#include "CMapTile.h"
class CFRect;
const int C_COLLISION_X = 1;
const int C_COLLISION_Y = 2;
const int C_COLLISION_X_POS = 3;
const int C_COLLISION_X_NEG = 4;
const int C_COLLISION_Y_POS = 5;
const int C_COLLISION_Y_NEG = 6;
const int C_COLLISION_BOTH = 7;


class CTileExtra;


class CSubManager; //forward

const float C_F_GRAVITY = 0.25f;
//the loader class helps us work with changing file sizes during development
class CMapLoader
{
public:
  float f_version;
  int i_data_size;
};

//Data structure for a zone

class CMapZone
{
  float f_version;
  int i_data_size;
  	
  int i_size_x; //how many X blocks
  int i_size_y; //how many Y blocks
  float f_block_size; //size of blocks in world cords
  int i_map_block_data_size;

public:

	//Accessors
	void SetRectWithTile(CFRect * p_rect, float f_padding, int i_x, int i_y);

    int get_size_x(){return i_size_x;};
	int get_size_y(){return i_size_y;};
	float get_block_size(){return f_block_size;};
    void set_block_size(float f_new);
    float get_gravity() {return f_gravity;};
	int get_block_x(float x) {if (x < 0) return -1; return int((x)/int(f_block_size));};
	int get_block_y(float y) {if (y < 0) return -1; return int((y)/int(f_block_size));};
	bool is_valid_block(int x, int y);
    bool CMapZone::tile_is_hard_world(float f_x, float f_y);
    float CMapZone::GetWorldHeightIncludingWalls(float f_x, float f_y, float f_height_of_blocked_tiles);
	int get_base_tile_block_x(float x);
	int get_base_tile_block_y(float y);
	void init_gravity();
    int get_tile_x(float f_x);
	int get_tile_y(float f_y);
    CMapTile * get_tile_pointer(int i_x, int i_y);
    CTileExtra * get_tile_extra_pointer(int i_x, int i_y);
	bool tile_extra_should_be_saved(int i_x, int i_y);
	bool is_valid_tile(int x, int y);
    int SphereToTileCheck(D3DXVECTOR3 *p_vec_pos, float f_rad);
	 //will return 10000 if there is no ceiling in this position
	 float GetCeilingHeight(float f_x, float f_y);
     int GetTilesPerBlock(){return i_tiles_per_block;}
	 void ScaleVerts(float f_scale);
     bool is_valid_position(float x, float y)
	{
	  if (x < 0) return false;
	  if (x > float(i_size_x)*f_block_size) return false;

	  if (y < 0) return false;
	  if (y > float(i_size_y)*f_block_size) return false;

	  //it's good
	  return true;
	};
	
	void CMapZone::send_map_to_terrain();
	
  CMapZone();
  ~CMapZone();
 bool CMapZone::generate_map(int i_new_x, int i_new_y, float f_new_block_size, float f_new_tiles_per_block);
 void CMapZone::create_random_height(FLOAT CTerrainHeight, FLOAT heightScale, 
							   FLOAT frequency, FLOAT freqScale, int numPasses);
  void CMapZone::generateHeightValues( FLOAT heightScale, FLOAT frequency );
  FLOAT CMapZone::interpolateSpline( float x, int numKnots, float *knot );
  CMapBlock * get_pointer_to_block(int i_x, int i_y){return &p_block[i_x+(i_y*i_size_x)];};
  void CMapZone::kill_map(); 
  void CMapZone::set_vertex_height(float f_x, float f_y, float f_height);
  float CMapZone::mod_vertex_height(float f_x, float f_y, float f_height);
  void CMapZone::save_zone(char st_fname[256]);
  bool CMapZone::load_zone(char * p_st_fname);
  void CMapZone::setup_tile_memory();
  void CMapZone::SetTilesPerBlock(int i_new);

  //vars
  
	float *p_height_buffer;  //array of each vertex's height
	                         //which makes it 2 bigger than we have blocks
	CMapBlock *p_block; //array of all our map info blocks
	int i_vertex_count;
	char st_name[50];
	char st_author[80];
	char st_email[80];
	char st_description[80];
	D3DXVECTOR3 vec_camera_pos; //remember where the camera was
	D3DXVECTOR3 vec_camera_look;
	float f_gravity_override; //force the gravity of this map, 0 for default
	float f_gravity;
	int i_tiles_per_block; //3 means 3X3 per block
	CMapTile *p_tile; //array of all our tiles
	int i_tile_num;
	int i_tile_x; //how many tiles there are in the X axis
	int i_tile_y; //how many tiles on the Y axis
	CSubManager *p_sub_manager; //handles the sub zone properties
    int i_tile_data_size; //size of ONE CMapTile object
	CTileExtra *p_tile_extra; //extra info we don't save
};