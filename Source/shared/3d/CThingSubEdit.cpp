#include "CThingSubEdit.h"
#include "CThingList.h"
#include "..\\CInput.h"
#include "CGridTile.h"	
#include "CBuildingManager.h"
#include "CBuilding.h"
#include "CTileExtra.h"

											  
CThingSubEdit::CThingSubEdit()
{
  this->set_sort(-1000); //make sure we get processed early, right after the CCamera object
  i_default_tex = 1; 
  this->set_type(C_THING_EDIT);
}


void CThingSubEdit::think()
{
 
 if (!cls_tglo.p_camera)
 {
   log_error("Camera not initted yet, can't process!");
   return;
 }

 if (this->i_edit_mode == ::C_EDIT_SUB_HARDNESS)
 process_zone_edit();
 if (this->i_edit_mode == ::C_EDIT_SUB_SUB_ZONE)
	 process_sub_zone_edit();
 if (this->i_edit_mode == ::C_EDIT_WALL)
	 process_edit_wall();

}  

void CThingSubEdit::set_edit_mode(int i_new)
{
  i_edit_mode = i_new;
}

void CThingSubEdit::set_sub_zone(int i_new)
{
	i_sub_zone = i_new;

	if (cls_tglo.p_grid_tile)
	{
		//let this kind of zone get highlighted
		cls_tglo.p_grid_tile->SetZoneHighlight(i_sub_zone);
	}

}

void CThingSubEdit::process_sub_zone_edit()
{
	if (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_LEFT) ||
		cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_RIGHT))
	{
		if (this->i_sub_zone == -1)
		{
		  log_msg("No sub zone selected, create one first.");
		  return;
		}
		
		
		//one of the mouse buttons was pressed
		D3DXVECTOR3 vec_point_a, vec_point_b;
		//get close point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_a, cls_tglo.p_cls_input->GetMouseX(),
			cls_tglo.p_cls_input->GetMouseY(), 0.001f); //close to screen
		
		//get far point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_b, cls_tglo.p_cls_input->GetMouseX(),
			cls_tglo.p_cls_input->GetMouseY(), 1000); //far from screen
		
		D3DXVECTOR3 vec_ground;
		D3DXPLANE plane_ground;
		if (!cls_tglo.p_terrain->find_ground_from_ray(&vec_point_a, &vec_point_b, &vec_ground, &plane_ground))
		{
			//didn't touch anything
			return;
		}
		
		int i_x;
		int i_y;

		CMapTile *p_temp;
			 
		if (this->b_block_mode)
		{
		  //update nine tiles, not just one

		 	i_x = (cls_tglo.p_map->get_block_x(vec_ground.x)*cls_tglo.p_map->i_tiles_per_block);
			i_y = cls_tglo.p_map->get_block_y(vec_ground.z)*cls_tglo.p_map->i_tiles_per_block;

			//draw 3 to the right and up (well, down the Z..)

			for (int x=i_x; x < i_x+3; x++)
			{
			  for (int y=i_y; y < i_y+3; y++)
			  {

			   				//let's mark this tile as the current SubZone
					p_temp = cls_tglo.p_map->get_tile_pointer(x, y);
					if (p_temp)
					{
					   
						if (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_LEFT))
						{
						  p_temp->i_sub_zone = this->i_sub_zone;
						} 

						if (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_RIGHT))
						{
						  p_temp->i_sub_zone = -1; //none
						} 
						if (cls_tglo.p_grid_tile)
						cls_tglo.p_grid_tile->update_tile(x, y);

					}




			  }

			}



		} else
		{

		i_x = cls_tglo.p_map->get_tile_x(vec_ground.x); //convert world to tile coords
		i_y = cls_tglo.p_map->get_tile_y(vec_ground.z); //convert world to tile coords		

		


		//let's mark this tile as the current SubZone
		p_temp = cls_tglo.p_map->get_tile_pointer(i_x, i_y);
		if (p_temp)
		{
		   
			if (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_LEFT))
			{
			  p_temp->i_sub_zone = this->i_sub_zone;
			} 

			if (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_RIGHT))
			{
			  p_temp->i_sub_zone = -1; //none
			} 
	
				if (cls_tglo.p_grid_tile)
					cls_tglo.p_grid_tile->update_tile(i_x, i_y);

		}
		}
		
		
	}
}

void CThingSubEdit::process_zone_edit()
{
 if (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_LEFT))
	{
	 
		D3DXVECTOR3 vec_point_a, vec_point_b;
	    //get close point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_a, cls_tglo.p_cls_input->GetMouseX(),
		cls_tglo.p_cls_input->GetMouseY(), 0.001f); //close to screen
		
		//get far point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_b, cls_tglo.p_cls_input->GetMouseX(),
		cls_tglo.p_cls_input->GetMouseY(), 1000); //far from screen

		D3DXVECTOR3 vec_ground;
		D3DXPLANE plane_ground;
		if (!cls_tglo.p_terrain->find_ground_from_ray(&vec_point_a, &vec_point_b, &vec_ground, &plane_ground))
		{
		  //didn't touch anything
			return;
		}

	 	int i_x;
		int i_y;

		CMapTile *p_temp;
			 
		if (this->b_block_mode)
		{
		  //update nine tiles, not just one

		 	i_x = (cls_tglo.p_map->get_block_x(vec_ground.x)*cls_tglo.p_map->i_tiles_per_block);
			i_y = cls_tglo.p_map->get_block_y(vec_ground.z)*cls_tglo.p_map->i_tiles_per_block;

			//draw 3 to the right and up (well, down the Z..)

			for (int x=i_x; x < i_x+3; x++)
			{
			  for (int y=i_y; y < i_y+3; y++)
			  {

			   		p_temp = cls_tglo.p_map->get_tile_pointer(x, y);
					if (p_temp)
					{
						if (p_temp->bits_texture.get_bit(C_MT_BLOCKED))
						{
							
						} else
						{
							p_temp->bits_texture.set_bit(C_MT_BLOCKED, 1);
							if (cls_tglo.p_grid_tile)
							{
								//update it visually too
								cls_tglo.p_grid_tile->update_tile(x, y);
							}
							
						}
					}
					
					
			  }
			  
			}



		} else
		{
		
			i_x = cls_tglo.p_map->get_tile_x(vec_ground.x); //convert world to tile coords  
			i_y = cls_tglo.p_map->get_tile_y(vec_ground.z); //convert world to tile coords	
			p_temp = cls_tglo.p_map->get_tile_pointer(i_x, i_y);
			if (p_temp)
			{
				if (p_temp->bits_texture.get_bit(C_MT_BLOCKED))
				{
					
				} else
				{
					p_temp->bits_texture.set_bit(C_MT_BLOCKED, 1);
					if (cls_tglo.p_grid_tile)
					{
						//update it visually too
						cls_tglo.p_grid_tile->update_tile(i_x, i_y);
					}
					
				}
			}
	
		}
	}

	 	 

   if (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_RIGHT))
	{
		D3DXVECTOR3 vec_point_a, vec_point_b;
	    //get close point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_a, cls_tglo.p_cls_input->GetMouseX(),
		cls_tglo.p_cls_input->GetMouseY(), 0.001f); //close to screen
		
		//get far point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_b, cls_tglo.p_cls_input->GetMouseX(),
		cls_tglo.p_cls_input->GetMouseY(), 1000); //far from screen

		D3DXVECTOR3 vec_ground;
		D3DXPLANE plane_ground;
		if (!cls_tglo.p_terrain->find_ground_from_ray(&vec_point_a, &vec_point_b, &vec_ground, &plane_ground))
		{
		  //didn't touch anything
			return;
		}
			int i_x;
		int i_y;

		CMapTile *p_temp;
		
		if (this->b_block_mode)
		{
			//update nine tiles, not just one
			
			i_x = (cls_tglo.p_map->get_block_x(vec_ground.x)*cls_tglo.p_map->i_tiles_per_block);
			i_y = cls_tglo.p_map->get_block_y(vec_ground.z)*cls_tglo.p_map->i_tiles_per_block;
			
			//draw 3 to the right and up (well, down the Z..)
			
			for (int x=i_x; x < i_x+3; x++)
			{
				for (int y=i_y; y < i_y+3; y++)
				{
					
					p_temp = cls_tglo.p_map->get_tile_pointer(x, y);
					if (p_temp)
					{
						if (p_temp->bits_texture.get_bit(C_MT_BLOCKED))
						{
							p_temp->bits_texture.set_bit(C_MT_BLOCKED, 0);
							if (cls_tglo.p_grid_tile)
							{
								//update it visually too
								cls_tglo.p_grid_tile->update_tile(x, y);
							}
							
						} else
						{
							
							
						}
						
					}
				
				}
				
			}
			
			
			
		} else
		{
			
			i_x = cls_tglo.p_map->get_tile_x(vec_ground.x); //convert world to tile coords
			i_y = cls_tglo.p_map->get_tile_y(vec_ground.z); //convert world to tile coords		
			
			//let's mark this tile as the current SubZone
			p_temp = cls_tglo.p_map->get_tile_pointer(i_x, i_y);
			if (p_temp)
			{
				if (p_temp->bits_texture.get_bit(C_MT_BLOCKED))
				{
					p_temp->bits_texture.set_bit(C_MT_BLOCKED, 0);
					if (cls_tglo.p_grid_tile)
					{
						//update it visually too
						cls_tglo.p_grid_tile->update_tile(i_x, i_y);
					}
					
				} else
				{
					
					
				}
			}
			
		}
   }
   
   


}



void CThingSubEdit::process_edit_wall()
{
    bool b_update_map = false;
    int i_sub_zone_hold = -1;
            
    if (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_LEFT) ||
		cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_RIGHT))
	{
		
		//one of the mouse buttons was pressed
		D3DXVECTOR3 vec_point_a, vec_point_b;
		//get close point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_a, cls_tglo.p_cls_input->GetMouseX(),
			cls_tglo.p_cls_input->GetMouseY(), 0.001f); //close to screen
		
		//get far point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_b, cls_tglo.p_cls_input->GetMouseX(),
			cls_tglo.p_cls_input->GetMouseY(), 1000); //far from screen
		
		D3DXVECTOR3 vec_ground;
		D3DXPLANE plane_ground;
		if (!cls_tglo.p_terrain->find_ground_from_ray(&vec_point_a, &vec_point_b, &vec_ground, &plane_ground))
		{
			//didn't touch anything
			return;
		}
		
		int i_x = cls_tglo.p_map->get_tile_x(vec_ground.x); //convert world to tile coords
		int i_y = cls_tglo.p_map->get_tile_y(vec_ground.z); //convert world to tile coords		
		
		//let's mark this tile as the current SubZone
		CMapTile *p_temp;
	
        
        
        if (this->b_block_mode)
        {
            //update nine tiles, not just one
            
            i_x = (cls_tglo.p_map->get_block_x(vec_ground.x)*cls_tglo.p_map->i_tiles_per_block);
            i_y = cls_tglo.p_map->get_block_y(vec_ground.z)*cls_tglo.p_map->i_tiles_per_block;
            
            //draw 3 to the right and up (well, down the Z..)
            
            for (int x=i_x; x < i_x+3; x++)
            {
                for (int y=i_y; y < i_y+3; y++)
                {
                    
                    //let's mark this tile as the current SubZone
                    p_temp = cls_tglo.p_map->get_tile_pointer(x, y);
                    if (p_temp)
                    {
                        
                        if (p_temp->i_sub_zone == -1)
                        {
                            log_msg("Can only create walls on sub zones.");
                            return;
                            
                        }
                        i_sub_zone_hold = p_temp->i_sub_zone;
                        bool b_store = p_temp->bits_texture.get_bit(C_MT_WALL);
                        
                        
                        if (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_LEFT))
                        {
                            p_temp->bits_texture.set_bit(::C_MT_WALL, 1);
                            if (p_temp->bits_texture.get_bit(C_MT_WALL) != b_store)
                            {
                                //actually added a wall, let's set the defaults for it
                                cls_tglo.p_map->get_tile_extra_pointer(x, y)->i_wall_tex[::C_BUILDING_TEX_NORTH] = i_default_tex;
                                cls_tglo.p_map->get_tile_extra_pointer(x, y)->i_wall_tex[::C_BUILDING_TEX_WEST] = i_default_tex;
                                cls_tglo.p_map->get_tile_extra_pointer(x, y)->i_wall_tex[::C_BUILDING_TEX_EAST] = i_default_tex;
                                cls_tglo.p_map->get_tile_extra_pointer(x, y)->i_wall_tex[::C_BUILDING_TEX_SOUTH] = i_default_tex;
                                //set it to hard by default
                                p_temp->bits_texture.set_bit(C_MT_BLOCKED, 1);
                                cls_tglo.p_grid_tile->update_tile(x, y);
                               
                                b_update_map =	true; //rebuild required
              
                            }
                            
                        } 
                        
                        
                        if (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_RIGHT))
                        {
                            p_temp->bits_texture.set_bit(::C_MT_WALL, 0);
                            if (p_temp->bits_texture.get_bit(C_MT_WALL) != b_store)
                            {
                                
                                p_temp->bits_texture.set_bit(C_MT_BLOCKED, 0);
                                cls_tglo.p_grid_tile->update_tile(x, y);
                                b_update_map =	true; //rebuild required
                                
                            }
                            
                            
                        } 
                        
                        
                        
                        
                    }
                    
                }
                
            }      
                
		}  else
    
        { 
        
        p_temp = cls_tglo.p_map->get_tile_pointer(i_x, i_y);
		if (p_temp)
		{
		
			if (p_temp->i_sub_zone == -1)
			{
			  LogMsg("Can only create walls on sub zones.");
			  return;

			}
		    i_sub_zone_hold = p_temp->i_sub_zone;
            bool b_store = p_temp->bits_texture.get_bit(C_MT_WALL);
			
			if (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_LEFT))
			{
			
                p_temp->bits_texture.set_bit(::C_MT_WALL, 1);
                if (p_temp->bits_texture.get_bit(C_MT_WALL) != b_store)
                {
                    //actually added a wall, let's set the defaults for it
                    cls_tglo.p_map->get_tile_extra_pointer(i_x, i_y)->i_wall_tex[::C_BUILDING_TEX_NORTH] = i_default_tex;
                    cls_tglo.p_map->get_tile_extra_pointer(i_x, i_y)->i_wall_tex[::C_BUILDING_TEX_WEST] = i_default_tex;
                    cls_tglo.p_map->get_tile_extra_pointer(i_x, i_y)->i_wall_tex[::C_BUILDING_TEX_EAST] = i_default_tex;
                    cls_tglo.p_map->get_tile_extra_pointer(i_x, i_y)->i_wall_tex[::C_BUILDING_TEX_SOUTH] = i_default_tex;
                    //set it to hard by default
                    p_temp->bits_texture.set_bit(C_MT_BLOCKED, 1);
                    cls_tglo.p_grid_tile->update_tile(i_x, i_y);
                    b_update_map = true; //redraw!
                    
                }
                
			} 

            if (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_RIGHT))
            {
                p_temp->bits_texture.set_bit(::C_MT_WALL, 0);
                if (p_temp->bits_texture.get_bit(C_MT_WALL) != b_store)
                {
                    
                    p_temp->bits_texture.set_bit(C_MT_BLOCKED, 0);
                    cls_tglo.p_grid_tile->update_tile(i_x, i_y);
                    b_update_map = true; //redraw!
              }
                
            } 
            
            
            
			//cls_tglo.p_grid_tile->update_tile(i_x, i_y);

		}
		
        }
		
	}


	if (b_update_map)
			if (cls_tglo.p_building_manager)
			{
			  cls_tglo.p_building_manager->get(i_sub_zone_hold)->build_walls();
			  cls_tglo.p_building_manager->get(i_sub_zone_hold)->dx_create();
			  
			  //force update
			}


}




