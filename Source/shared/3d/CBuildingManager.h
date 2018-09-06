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

#include "CThing.h"
#include "CThing_globals.h"

class CBuilding;

class CBuildingManager : public CThing
{

public:
   int i_count;


  CBuildingManager();
  virtual ~CBuildingManager();
  void CBuildingManager::clear_all();
  void CBuildingManager::update_block_of_tiles(int i_x, int i_y);

  void CBuildingManager::add_building();
  CBuilding * CBuildingManager::get(int i_building);
  bool CBuildingManager::find_wall_from_ray(D3DXVECTOR3 *vec_point_a, D3DXVECTOR3
	  *vec_point_b, int *p_i_tile_x, int *p_i_tile_y, int *p_i_wall, D3DXVECTOR3 *out_vec_hit);
  CBuilding **p_a_building;

};

