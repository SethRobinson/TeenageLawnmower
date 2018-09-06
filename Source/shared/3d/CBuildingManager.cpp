/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CBuildingManager.h"
#include "CBuilding.h"
#include "CThingList.h"
#include "CMap.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBuildingManager::CBuildingManager()
{
  	i_count = 0; //how many buildings we know about
	cls_tglo.p_building_manager = this;
    this->p_a_building = NULL;
	this->set_thing_name("Building manager");  
	set_sort(-5);
}


CBuildingManager::~CBuildingManager()
{
  cls_tglo.p_building_manager = NULL;
  clear_all();
}


void CBuildingManager::clear_all()
{
     //release all buildings
	 for (int i= 0; i < i_count; i++)
	 {
	  	 p_a_building[i]->dx_invalidate();
		 p_a_building[i]->b_delete_flag = true; //bye
	 }
	
	 SAFE_FREE(p_a_building);
     i_count = 0;
}

void CBuildingManager::add_building()
{
//  log_msg("CBuildingManager is adding a new building");
  	i_count++;
	int i_building = i_count-1; //zero based
	
	//create new memory or add to existing
	p_a_building = (CBuilding**)realloc(p_a_building, sizeof(CBuilding*)*i_count);

	p_a_building[i_building] = new CBuilding();
	p_my_thing_list->add_new_thing(p_a_building[i_building]);
	p_a_building[i_building]->set_sub_zone_id(i_building);
	p_a_building[i_building]->dx_create();
	p_a_building[i_building]->set_sort(3);

}




void CBuildingManager::update_block_of_tiles(int i_x, int i_y)
{
 
	//let's not do this, too slow

	return;

	int i_tiles_per_block = cls_tglo.p_map->i_tiles_per_block;	
  i_x *= i_tiles_per_block;
  i_y *= i_tiles_per_block;
  int i_save_zone = -1;

  for (int x = 0; x < i_tiles_per_block; x++)
  {
	  for (int y = 0; y < i_tiles_per_block; y++)
	  {
		  if (cls_tglo.p_map->get_tile_pointer(i_x, i_y)->i_sub_zone != -1)
		  {
			  //it is on a subzone.  But is it a wall?
		  }
		  if (cls_tglo.p_map->get_tile_pointer(i_x, i_y)->bits_texture.get_bit(::C_MT_WALL))
		  {
			  if (i_save_zone == -1) i_save_zone = cls_tglo.p_map->get_tile_pointer(i_x, i_y)->i_sub_zone;
			  
			  if (i_save_zone != cls_tglo.p_map->get_tile_pointer(i_x, i_y)->i_sub_zone)
			  {
				  //we switched sub zones?
				  i_save_zone = cls_tglo.p_map->get_tile_pointer(i_x, i_y)->i_sub_zone;
				  
			  } else
			  {
				  i_save_zone = cls_tglo.p_map->get_tile_pointer(i_x, i_y)->i_sub_zone;
			  }
			   //update area

			  	cls_tglo.p_building_manager->p_a_building[i_save_zone]->update_wall_tile(i_x, i_y);  

		  }
		  
	  }
  }


	
}


//a building's number is it's sub zone number

CBuilding * CBuildingManager::get(int i_building)
{
  				
  return p_a_building[i_building];
}


  bool CBuildingManager::find_wall_from_ray(D3DXVECTOR3 *vec_point_a, D3DXVECTOR3
	  *vec_point_b, int *p_i_tile_x, int *p_i_tile_y, int *p_i_wall, D3DXVECTOR3 *out_vec_hit)
  {
	float f_last_dist = 2000000000;
	float f_dist = 0;
	static D3DXVECTOR3 vec_temp;
	//check each  building for collision
	D3DXVECTOR3 p_temp_hit, vec_last_hit;
	int i_temp_wall_part, i_save_wall_num, i_save_building, i_save_wall_part;

	for (int i=0; i < this->i_count; i++)
	{
		for (int k=0; k < this->p_a_building[i]->i_walls; k++)
		{
			if (p_a_building[i]->does_ray_hit_wall(k, vec_point_a, vec_point_b, &i_temp_wall_part, &p_temp_hit, &f_dist))
			{
				if (f_dist < f_last_dist)
				{
					//this is the closest collision so far, save it
					f_last_dist = f_dist;
					i_save_building = i;
					i_save_wall_num = k;
					i_save_wall_part = i_temp_wall_part;
					vec_last_hit = p_temp_hit;
				}
			}
			
		}
	  
        
        //let's also scan this buildings roof
        for (int p=0; p <p_a_building[i]->i_roof_tiles; p++)
        {
            
            if (p_a_building[i]->does_ray_hit_roof(p, vec_point_a, vec_point_b, &i_temp_wall_part, &p_temp_hit, &f_dist))
            {
                if (f_dist < f_last_dist)
                {
                    //this is the closest collision so far, save it
                    f_last_dist = f_dist;
                    i_save_building = i;
                    i_save_wall_num = p;
                    i_save_wall_part = i_temp_wall_part;
                    vec_last_hit = p_temp_hit;
                }
            }
            
        }
        
    }
    
    

   if (f_last_dist < 2000000000)
   {
	   *out_vec_hit = vec_last_hit;
	   *p_i_wall = i_save_wall_part;

		//special case for TOP
	   if (*p_i_wall == ::C_BUILDING_TEX_TOP)
	   {
	    this->p_a_building[i_save_building]->get_x_and_y_from_roof_array_num(i_save_wall_num,
		   p_i_tile_x, p_i_tile_y);


	   } else
	   {

	   //convert wall array num into it's x and y
	   this->p_a_building[i_save_building]->get_x_and_y_from_wall_array_num(i_save_wall_num,
		   p_i_tile_x, p_i_tile_y);
	   }
	   return true;
   }

    return false;
  }