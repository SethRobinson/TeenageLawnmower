/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CControlEdit.h"
#include "CEntBall.h"
#include "CThingList.h"
#include "..\\CInput.h"	   
#include "CBuildingManager.h"
#include "CThing_util.h"
#include "CTileExtra.h"
#include "CBuilding.h"

CControlEdit::CControlEdit()
{
  this->set_sort(-1000); //make sure we get processed early, right after the CCamera object
  this->set_type(C_THING_EDIT);
  f_strength = 1;
  this->i_brush_type = ::C_BRUSH_TYPE_PAINT;
  this->i_current_brush = ::C_RESOURCE_NONE;
  f_tool_setting = 0.1f;
  b_paste_full = true;
  i_brush_sub_mode = 0;
  this->uc_texture_mode = ::C_MB_TEXTURE_MODE_NONE;
}


void CControlEdit::think()
{
 
 //if (!cls_tglo.p_d3dapp->m_bHasFocus) return;
 if (!cls_tglo.p_camera)
 {
   log_error("Camera not initted yet, can't process!");
   return;
 }

 if (i_brush_type == ::C_BRUSH_TYPE_PAINT)
	 process_paint(); else

 if (i_brush_type == ::C_BRUSH_TYPE_LANDSCAPE)
	 process_landscape(); else
 {
   log_error("Brush type not set in CControlEdit.");
 }


 //misc hotkeys

 if (cls_tglo.p_cls_input->is_key_down(DIK_4))
 {
   //make some random balls
   CEntBall *p_temp = new CEntBall();
   this->p_my_thing_list->add_new_thing(p_temp);
   p_temp->dx_create();
 }


  
}  

void CControlEdit::process_landscape()
{
 	if (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_LEFT))
	{
		 	D3DXVECTOR3 vec_point_a, vec_point_b;
	    //get close point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_a, cls_tglo.p_cls_input->GetMouseX(),
		cls_tglo.p_cls_input->GetMouseY(), 0.001f); //close to screen
		
		//get far point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_b, cls_tglo.p_cls_input->GetMouseX(),
		cls_tglo.p_cls_input->GetMouseY(), 1000); //close to screen

		D3DXVECTOR3 vec_ground;
		D3DXPLANE plane_ground;
		if (!cls_tglo.p_terrain->find_ground_from_ray(&vec_point_a, &vec_point_b, &vec_ground, &plane_ground))
		{
		  //didn't touch anything
			return;
		}

		if (this->i_current_brush == ::C_BRUSH_MOD)
		{
		  //let's raise this part of the landscape
			cls_tglo.p_map->mod_vertex_height(vec_ground.x, vec_ground.z, f_tool_setting);
		}
	
	   	if (this->i_current_brush == ::C_BRUSH_SET)
		{
		  //let's raise this part of the landscape
			cls_tglo.p_map->set_vertex_height(vec_ground.x, vec_ground.z, f_tool_setting);
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
		cls_tglo.p_cls_input->GetMouseY(), 1000); //close to screen

		D3DXVECTOR3 vec_ground;
		D3DXPLANE plane_ground;
		if (!cls_tglo.p_terrain->find_ground_from_ray(&vec_point_a, &vec_point_b, &vec_ground, &plane_ground))
		{
		  //didn't touch anything
			return;
		}

		if (this->i_current_brush == ::C_BRUSH_MOD)
		{
		  //let's raise this part of the landscape
			cls_tglo.p_map->mod_vertex_height(vec_ground.x, vec_ground.z, -f_tool_setting); //subtract
		}
	
	}

}

void CControlEdit::process_paint_walls()
{

  if ( (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_LEFT)) ||
	  (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_RIGHT)) )
  {

	 	  D3DXVECTOR3 vec_point_a, vec_point_b;
	  //get close point
	  cls_tglo.p_camera->get_screen_to_world_point(&vec_point_a, cls_tglo.p_cls_input->GetMouseX(),
		  cls_tglo.p_cls_input->GetMouseY(), 0.001f); //close to screen
	  
	  //get far point
	  cls_tglo.p_camera->get_screen_to_world_point(&vec_point_b, cls_tglo.p_cls_input->GetMouseX(),
		  cls_tglo.p_cls_input->GetMouseY(), 1000); //close to screen
	  
	  //we have our ray.  Did it hit a wall?

	  //recurse through every wall in the world...
	  int i_x, i_y;	  //wall is on this tile
	  int i_wall_part; //section of the wall we hit
	  D3DXVECTOR3 vec_hit; //exact place we hit

	  if (cls_tglo.p_building_manager->find_wall_from_ray(&vec_point_a, &vec_point_b, &i_x, &i_y,
		  &i_wall_part, &vec_hit))
	  {
	   // log_msg("hit wall piece %d at %d, %d at %s",i_wall_part, i_x, i_y, print_vector(vec_hit) );
	     
	    CTileExtra * p_tile_e = cls_tglo.p_map->get_tile_extra_pointer(i_x, i_y);
		
		if (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_RIGHT))
		{
		  //let's just grab the color and leave
		  i_current_brush = p_tile_e->i_wall_tex[i_wall_part];
		  return;
		}
		
		int i_building =  cls_tglo.p_map->get_tile_pointer(i_x, i_y)->i_sub_zone;

		if (i_wall_part == ::C_BUILDING_TEX_TOP)
		{
		  //slightly different procedure to update the roof tile color
			if (this->b_paste_full)
		{
		  //update nine tiles, not just one
			float f_tile_size = cls_tglo.p_map->get_block_size() / cls_tglo.p_map->i_tiles_per_block;

		 	i_x = (cls_tglo.p_map->get_block_x(i_x*f_tile_size)*cls_tglo.p_map->i_tiles_per_block);
			i_y = cls_tglo.p_map->get_block_y(i_y*f_tile_size)*cls_tglo.p_map->i_tiles_per_block;

			//draw 3 to the right and up (well, down the Z..)

			for (int x=i_x; x < i_x+3; x++)
			{
			  for (int y=i_y; y < i_y+3; y++)
			  {
				  p_tile_e = cls_tglo.p_map->get_tile_extra_pointer(x, y);
                  if (cls_tglo.p_map->get_tile_pointer(x,y)->i_sub_zone == i_building)
                  {
                      if (p_tile_e->i_roof_index != -1)
                      {
                          
                          p_tile_e->i_wall_tex[i_wall_part] = i_current_brush;
                          cls_tglo.p_building_manager->p_a_building[i_building]->p_building_roof_tile_info[p_tile_e->i_roof_index].refresh_texture();
                      }
                      
                  }
                  
              }
			}
			} else
			{
			 
			 //only update the one we clicked	
			  p_tile_e->i_wall_tex[i_wall_part] = i_current_brush;
		     cls_tglo.p_building_manager->p_a_building[i_building]->p_building_roof_tile_info[p_tile_e->i_roof_index].refresh_texture();
			}
		
		} else
		{
		p_tile_e->i_wall_tex[i_wall_part] = i_current_brush;
		cls_tglo.p_building_manager->p_a_building[i_building]->p_building_tile_info[
			p_tile_e->i_wall].refresh_texture(i_wall_part);
		}

	  }
  }

}

void CControlEdit::process_paint()
{
 // if (i_current_brush == ::C_RESOURCE_NONE) return; //no brush has
 //been chosen yet
 
  if (this->i_brush_sub_mode == ::C_BRUSH_PAINT_WALLS)
  {
    process_paint_walls();
    return;
  }

 //process any input that has occured

  if ( (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_LEFT))
	  || (cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_RIGHT))

	  )
  {
	  //let's paint the landscape here
	  D3DXVECTOR3 vec_point_a, vec_point_b;
	  //get close point
	  cls_tglo.p_camera->get_screen_to_world_point(&vec_point_a, cls_tglo.p_cls_input->GetMouseX(),
		  cls_tglo.p_cls_input->GetMouseY(), 0.001f); //close to screen
	  
	  //get far point
	  cls_tglo.p_camera->get_screen_to_world_point(&vec_point_b, cls_tglo.p_cls_input->GetMouseX(),
		  cls_tglo.p_cls_input->GetMouseY(), 1000); //close to screen
	  
	  D3DXVECTOR3 vec_ground;
	  D3DXPLANE plane_ground;
	  if (cls_tglo.p_terrain->find_ground_from_ray(&vec_point_a, &vec_point_b, &vec_ground, &plane_ground))
	  {
		if ( cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_RIGHT))
		{
		 //right mouse button is down, let's make the texture they clicked the active
		 //texture
		 
			CMapBlock *p_temp;
			int i_x = cls_tglo.p_map->get_block_x(vec_ground.x);
			int i_z = cls_tglo.p_map->get_block_y(vec_ground.z);
			p_temp = cls_tglo.p_map->get_pointer_to_block(i_x, i_z);
			
			if (m_b_overlay)
			{
				i_current_brush = p_temp->i_overlay_tex_id;
			} else
			{
			
			i_current_brush = p_temp->i_tex_id;
			}
		  return;
		}
			 
		  if (b_paste_full)
		  {
			  //update whole tile area	  
			  CMapBlock *p_temp;
			  int i_start_x = cls_tglo.p_map->get_base_tile_block_x(vec_ground.x);
			  int i_start_z = cls_tglo.p_map->get_base_tile_block_y(vec_ground.z);
			  
			  int i_blocks_per_tex = cls_tglo.p_terrain->get_blocks_per_texture();
			  
			  for (int i_x = i_start_x; i_x < i_start_x+i_blocks_per_tex; i_x++)
				  for (int i_z = i_start_z; i_z < i_start_z+i_blocks_per_tex; i_z++)
				  {
					  if (cls_tglo.p_map->is_valid_block(i_x, i_z))
					  {
						  p_temp = cls_tglo.p_map->get_pointer_to_block(i_x, i_z);
						  if (m_b_overlay)
						  p_temp->i_overlay_tex_id = this->i_current_brush; else
						  p_temp->i_tex_id = this->i_current_brush;
						  
						  //sorry this is so ugly, I switched to saving bits later and
						  //didn't want to change my gui around.. just lazy I guess.

						  if (uc_texture_mode != C_MB_TEXTURE_MODE_NONE)
						  {
							 if (uc_texture_mode == C_MB_TEXTURE_MODE_FLIP_X_AND_Z)
							 {
							  if (m_b_overlay)
							  {
							  p_temp->bits_texture.set_bit(C_MB_BITS_TEX2_FLIP_X, true);
							  p_temp->bits_texture.set_bit(C_MB_BITS_TEX2_FLIP_Z, true);
							  } else
							  {
							  
							  p_temp->bits_texture.set_bit(C_MB_BITS_TEX_FLIP_X, true);
							  p_temp->bits_texture.set_bit(C_MB_BITS_TEX_FLIP_Z, true);
							  }
							 } else
							 {
								 if (uc_texture_mode == C_MB_TEXTURE_MODE_FLIP_X)
								 {
									 if (m_b_overlay)
									 {
										 p_temp->bits_texture.set_bit(C_MB_BITS_TEX2_FLIP_X, true);
									 } else
									 {
								 		 p_temp->bits_texture.set_bit(C_MB_BITS_TEX_FLIP_X, true);
								
									 }
								 } else
								 {
									 if (m_b_overlay)
									 {
										 p_temp->bits_texture.set_bit(C_MB_BITS_TEX2_FLIP_Z, true);
									 }
									 {
										 p_temp->bits_texture.set_bit(C_MB_BITS_TEX_FLIP_Z, true);
									 }
								 }
							 }
							 
						  } else
						  {
							  //no tile flipping
							  if (m_b_overlay)
							  {
								  
								  p_temp->bits_texture.set_bit(C_MB_BITS_TEX2_FLIP_X, false);
								  p_temp->bits_texture.set_bit(C_MB_BITS_TEX2_FLIP_Z, false);
								  
							  } else
							  {
								  p_temp->bits_texture.set_bit(C_MB_BITS_TEX_FLIP_X, false);
								  p_temp->bits_texture.set_bit(C_MB_BITS_TEX_FLIP_Z, false);
								  
								  
							  }
						  }
						  
						  cls_tglo.p_terrain->update_terrain_block_from_map(i_x, i_z);
						  cls_tglo.p_terrain->update_block_in_vertex_buffer(i_x, i_z);
					  }
				  }
			  
		  } else
		  {
			  //update one small area
			  CMapBlock *p_temp;
			  int i_x = cls_tglo.p_map->get_block_x(vec_ground.x);
			  int i_z = cls_tglo.p_map->get_block_y(vec_ground.z);
			  
			  p_temp = cls_tglo.p_map->get_pointer_to_block(i_x, i_z);
			 
			  if (m_b_overlay)
			  {
			  
				  //hold it.  is this texture really transparent?  


			  p_temp->i_overlay_tex_id = this->i_current_brush;
			  }
			  else
			  {
			  
			  p_temp->i_tex_id = this->i_current_brush;
			  }
			  
			  if (uc_texture_mode != C_MB_TEXTURE_MODE_NONE)
						  {
							 if (uc_texture_mode == C_MB_TEXTURE_MODE_FLIP_X_AND_Z)
							 {
							  p_temp->bits_texture.set_bit(C_MB_BITS_TEX_FLIP_X, true);
							  p_temp->bits_texture.set_bit(C_MB_BITS_TEX_FLIP_Z, true);
							 } else
							 {
								 if (uc_texture_mode == C_MB_TEXTURE_MODE_FLIP_X)
								 {
							     p_temp->bits_texture.set_bit(C_MB_BITS_TEX_FLIP_X, true);
							
								 } else
								 {
							  	  p_temp->bits_texture.set_bit(C_MB_BITS_TEX_FLIP_Z, true);
								 }
							 }

						  } else
						  {
						    //no tile flipping
							  p_temp->bits_texture.set_bit(C_MB_BITS_TEX_FLIP_X, false);
							  p_temp->bits_texture.set_bit(C_MB_BITS_TEX_FLIP_Z, false);

						  }
						  
					
			  cls_tglo.p_terrain->update_terrain_block_from_map(i_x, i_z);
			  cls_tglo.p_terrain->update_block_in_vertex_buffer(i_x, i_z);

		  }
	  }
 }		



}