/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CBuilding.h"
#include "CMapTile.h"
#include "CMap.h"
#include "CTerrain.h"	 
#include "CSubManager.h"
#include "CTileExtra.h"
#include "CThing_util.h"
#include <DXErr8.h>

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CBuildingTile::refresh_texture(int i_side)
{
 
	static int i_tex;

	i_tex = (cls_tglo.p_map->get_tile_extra_pointer(i_x, i_z))->i_wall_tex[i_side];
	if (!cls_tglo.p_manager->does_resource_exist(i_tex))
	{
	  log_error("Building tile referenced a texture with %d, it has not been loaded!",i_tex);
	 i_tex = 1; //force grass
	}
	if (((CResourceTex*)cls_tglo.p_manager->get(i_tex))->get_texture() == NULL)
  {
    //add ref and load it
	((CResourceTex*)cls_tglo.p_manager->get(i_tex))->add_ref();
	((CResourceTex*)cls_tglo.p_manager->get(i_tex))->dx_create();
  }

  this->tex[i_side] = ((CResourceTex*)cls_tglo.p_manager->get(i_tex))->get_texture();
}


void CRoofTile::refresh_texture()
{
 
	static int i_tex;
	if (!cls_tglo.p_map->is_valid_tile(i_x, i_z))
	{
	  log_error("Tile %d:%d is not valid, can't refresh texture.",i_x, i_z);
	  

	} else
	{
	i_tex = (cls_tglo.p_map->get_tile_extra_pointer(i_x, i_z))->i_wall_tex[C_BUILDING_TEX_TOP];
	if (!cls_tglo.p_manager->does_resource_exist(i_tex))
	{
	  log_error("Building roof tile referenced a texture with %d, it has not been loaded!",i_tex);
		 i_tex = 1; //force grass
}
	
	if (((CResourceTex*)cls_tglo.p_manager->get(i_tex))->get_texture() == NULL)
  {
    //add ref and load it
	((CResourceTex*)cls_tglo.p_manager->get(i_tex))->add_ref();
	((CResourceTex*)cls_tglo.p_manager->get(i_tex))->dx_create();
  }

	}
  this->tex = ((CResourceTex*)cls_tglo.p_manager->get(i_tex))->get_texture();
}


CBuilding::CBuilding()
{
    i_vertex_bytes = 0;
    p_building_tile_info = NULL;
    p_vertex_buffer= NULL;
    p_local_verts = NULL;
    i_walls = 0;
    i_vertex_count = 0;
    i_sub_zone = -1; //don't know our sub zone yet
    i_sides_per_wall = 4;
    
    p_vertex_buffer_roof = NULL;
    p_local_verts_roof = NULL;
    i_roof_tiles = 0;
    i_vertex_bytes_roof = 0;
    p_building_roof_tile_info = NULL;
    
    if (!cls_tglo.p_map)
    {
        log_error("CBuilding can't set p_map var!");
        
    }
    p_map = cls_tglo.p_map;
    
    set_thing_name("Zone");
    
    set_sort(0);
    SetForceDoubleSidedTextures(true);

    // this->set_no_lighting(true);
}

CBuilding::~CBuilding()
{
    SAFE_DELETE_ARRAY(p_building_tile_info);
    SAFE_DELETE_ARRAY(this->p_local_verts);

    SAFE_DELETE_ARRAY(p_building_roof_tile_info);
	SAFE_DELETE_ARRAY(p_local_verts_roof);
}


void CBuilding::get_x_and_y_from_wall_array_num(int i_wall_num, int *p_x, int *p_y)
{
  *p_x = this->p_building_tile_info[i_wall_num].i_x;
  *p_y = this->p_building_tile_info[i_wall_num].i_z;
}

void CBuilding::get_x_and_y_from_roof_array_num(int i_roof_num, int *p_x, int *p_y)
{
  *p_x = this->p_building_roof_tile_info[i_roof_num].i_x;
  *p_y = this->p_building_roof_tile_info[i_roof_num].i_z;
}


void CBuilding::update_wall_tile(int i_x, int i_z)
{

	 log_msg("Updating wall %d, %d", i_x, i_z);

}
void CBuilding::build_walls()
{
  //how many walls are there?

  CMapTile *p_tile;
  i_walls = 0;
  i_roof_tiles = 0;
 
  
  //first check for the draw nothing flag if this is set then we're probably only
  //using these tile definition to control hardness or areas.
  if (cls_tglo.p_map->p_sub_manager->p_a_sub_zone[this->i_sub_zone].i_cealing_draw != C_SUBZONE_ROOF_DRAW_NOTHING)
  {
      
      
      for (int z = 0; z < cls_tglo.p_map->i_tile_y; z++)
      {
          for (int x = 0; x < cls_tglo.p_map->i_tile_x; x++)
          {
              p_tile = cls_tglo.p_map->get_tile_pointer(x, z);
              
              if (p_tile->i_sub_zone == i_sub_zone)
              {
                  //this is on our turf.  But is it a wall?
                  if (p_tile->bits_texture.get_bit(::C_MT_WALL))
                  {
                      
                      i_walls++;
                      if (cls_tglo.p_map->p_sub_manager->p_a_sub_zone[this->i_sub_zone].i_cealing_draw != C_SUBZONE_ROOF_NONE)
                      {
                          //also going to draw a roof tile for this guy later
                          i_roof_tiles++;
                      }
                  }	else
                  {
                      
                      //it's in our subzone but it isn't a wall
                      if (cls_tglo.p_map->p_sub_manager->p_a_sub_zone[this->i_sub_zone].i_cealing_draw == C_SUBZONE_ROOF_NORMAL)
                      {
                          //also going to draw a roof tile here
                          i_roof_tiles++;
                      }
                      
                      
                  }
             
               //either way, let's invalidate the roof tile index
                  ((CTileExtra*)cls_tglo.p_map->get_tile_extra_pointer(x, z))->i_roof_index = -1;
              
              }
          }  
      }
  }
 //  log_msg("Found %d walls.", i_walls);
  
  //init memory for our general info buffer
  SAFE_DELETE_ARRAY(p_building_tile_info);
  p_building_tile_info = new CBuildingTile[i_walls];

   SAFE_DELETE_ARRAY(p_building_roof_tile_info);
  p_building_roof_tile_info = new CRoofTile[i_roof_tiles];



  //free mem if we need to
  SAFE_DELETE_ARRAY(this->p_local_verts);

  //init memory for our local copy of the wall verts
  this->i_vertex_count = 6*i_sides_per_wall*i_walls; //we're going to make 5 sided cubes, no need for a bottom
  this->i_vertex_bytes = i_vertex_count * sizeof(CBuildVertex);

  p_local_verts = new CBuildVertex[i_vertex_count];
  

  SAFE_DELETE_ARRAY(this->p_local_verts_roof);
  this->i_vertex_count_roof = 6*this->i_roof_tiles;
  this->i_vertex_bytes_roof = i_vertex_count_roof* sizeof(CBuildVertex);

  p_local_verts_roof = new CBuildVertex[i_vertex_count_roof];


  build_all_wall_quads();
  build_all_roof_tiles();

  dx_invalidate_walls(); //make sure we don't keep drawing the old outdated walls
}

void CBuilding::compute_wall_visibility(int i_wall_count, int i_x, int i_z)
{
  //we don't want to draw walls that can't be seen, so let's look at our neighbors...
	
	if ( (i_x+1 < cls_tglo.p_map->i_tile_x) //check to make sure it's a valid position
	&&  ( ((CMapTile*)cls_tglo.p_map->get_tile_pointer(i_x+1, i_z))->bits_texture.get_bit(::C_MT_WALL)) 
	&& (cls_tglo.p_map->get_tile_pointer(i_x+1, i_z)->i_sub_zone == this->i_sub_zone)
	)
	{
	  //wall is blocked off
		p_building_tile_info[i_wall_count].b_wall_active[::C_BUILDING_TEX_EAST] = false;
	} else 	p_building_tile_info[i_wall_count].b_wall_active[::C_BUILDING_TEX_EAST] = true;

	
	if ( (i_x-1 > -1) //check to make sure it's a valid position
	&&  ( ((CMapTile*)cls_tglo.p_map->get_tile_pointer(i_x-1, i_z))->bits_texture.get_bit(::C_MT_WALL))
		&& (cls_tglo.p_map->get_tile_pointer(i_x-1, i_z)->i_sub_zone == this->i_sub_zone)

	)
	{
	  //wall is blocked off
		p_building_tile_info[i_wall_count].b_wall_active[::C_BUILDING_TEX_WEST] = false;
	} else 	p_building_tile_info[i_wall_count].b_wall_active[::C_BUILDING_TEX_WEST] = true;

	if ( (i_z-1 > -1) //check to make sure it's a valid position
	&&  ( ((CMapTile*)cls_tglo.p_map->get_tile_pointer(i_x, i_z-1))->bits_texture.get_bit(::C_MT_WALL))
		&& (cls_tglo.p_map->get_tile_pointer(i_x, i_z-1)->i_sub_zone == this->i_sub_zone)

	)
	{
	  //wall is blocked off
		p_building_tile_info[i_wall_count].b_wall_active[::C_BUILDING_TEX_SOUTH] = false;
	} else 	p_building_tile_info[i_wall_count].b_wall_active[::C_BUILDING_TEX_SOUTH] = true;

	if ( (i_z+1 < cls_tglo.p_map->i_tile_y) //check to make sure it's a valid position
	&&  ( ((CMapTile*)cls_tglo.p_map->get_tile_pointer(i_x, i_z+1))->bits_texture.get_bit(::C_MT_WALL))
		&& (cls_tglo.p_map->get_tile_pointer(i_x, i_z+1)->i_sub_zone == this->i_sub_zone)

	)
	{
	  //wall is blocked off
		p_building_tile_info[i_wall_count].b_wall_active[::C_BUILDING_TEX_NORTH] = false;
	} else 	p_building_tile_info[i_wall_count].b_wall_active[::C_BUILDING_TEX_NORTH] = true;

}

void CBuilding::build_all_wall_quads()
{
	CMapTile *p_tile;
	int i_wall_count = 0;  
	int i_temp_roof = 0;
	
    //if we're set to draw nothing, we can skip this step

    
    if (cls_tglo.p_map->p_sub_manager->p_a_sub_zone[this->i_sub_zone].i_cealing_draw != C_SUBZONE_ROOF_DRAW_NOTHING)
    {			
    
    for (int z = 0; z < cls_tglo.p_map->i_tile_y; z++)
	{
		for (int x = 0; x < cls_tglo.p_map->i_tile_x; x++)
		{
			p_tile = cls_tglo.p_map->get_tile_pointer(x, z);
			
			if (p_tile->i_sub_zone == i_sub_zone)
			{
				//this is on our turf.  But is it a wall?
				if (p_tile->bits_texture.get_bit(::C_MT_WALL))
				{
				//remember location for array to x/y lookups
					this->p_building_tile_info[i_wall_count].i_x = x;
					this->p_building_tile_info[i_wall_count].i_z = z;
					
					//add this wall to our info
					build_wall_quads(i_wall_count, x, z);
					compute_wall_visibility(i_wall_count, x, z);
					
					if (cls_tglo.p_map->p_sub_manager->p_a_sub_zone[this->i_sub_zone].i_cealing_draw != C_SUBZONE_ROOF_NONE)
					{
					    //for lookups with the roof
						this->p_building_roof_tile_info[i_temp_roof].i_x = x;
					    this->p_building_roof_tile_info[i_temp_roof].i_z = z;
						i_temp_roof++;
					}
					
					i_wall_count++;
				} else
				{
				  //no wall here but it is our subzone

				 	if (cls_tglo.p_map->p_sub_manager->p_a_sub_zone[this->i_sub_zone].i_cealing_draw == C_SUBZONE_ROOF_NORMAL)
					{
					    //for lookups with the roof
						this->p_building_roof_tile_info[i_temp_roof].i_x = x;
					    this->p_building_roof_tile_info[i_temp_roof].i_z = z;
						i_temp_roof++;
					}


				}
				
			}
			
		}  
	}
	
    }
	
}

void CBuilding::create_roof_quad(int i_roof)
{
  	bool b_relative_height = false;
	if (cls_tglo.p_map->p_sub_manager->p_a_sub_zone[i_sub_zone].i_ceil_type == ::C_SUBZONE_CEIL_RELATIVE)
	{
	 b_relative_height = true;
	}
	int p=i_roof*6;
  	float f_height =  cls_tglo.p_map->p_sub_manager->p_a_sub_zone[i_sub_zone].f_wall_height;

	float f_spacer = p_map->get_block_size() / float(p_map->i_tiles_per_block);
    float f_pos_x = float(p_building_roof_tile_info[i_roof].i_x)*f_spacer;
    float f_pos_z = float(p_building_roof_tile_info[i_roof].i_z)*f_spacer;
  

	p_local_verts_roof[p].vec_pos.x = f_pos_x;
	p_local_verts_roof[p].vec_pos.z = f_pos_z;
	if (b_relative_height)
	{
		p_local_verts_roof[p].vec_pos.y = f_height + cls_tglo.p_terrain->get_height(p_local_verts_roof[p].vec_pos.x, p_local_verts_roof[p].vec_pos.z);//height
	} else
	{
	 p_local_verts_roof[p].vec_pos.y = f_height;
	}

	p++;


	p_local_verts_roof[p].vec_pos.x = f_pos_x; 
	p_local_verts_roof[p].vec_pos.z = f_pos_z + f_spacer; 
	if (b_relative_height)
	{
		p_local_verts_roof[p].vec_pos.y = f_height + cls_tglo.p_terrain->get_height(p_local_verts_roof[p].vec_pos.x, p_local_verts_roof[p].vec_pos.z);//height
	} else
	{
	 p_local_verts_roof[p].vec_pos.y = f_height;
	}

	p++;

	p_local_verts_roof[p].vec_pos.x = f_pos_x + f_spacer; 
	p_local_verts_roof[p].vec_pos.z = f_pos_z + f_spacer; 
	if (b_relative_height)
	{
		p_local_verts_roof[p].vec_pos.y = f_height + cls_tglo.p_terrain->get_height(p_local_verts_roof[p].vec_pos.x, p_local_verts_roof[p].vec_pos.z);//height
	} else
	{
	 p_local_verts_roof[p].vec_pos.y = f_height;
	}

	p++;


	//build the other tri
   	
  	int i_v =i_roof*6;
	//copy the first two verts	
	p_local_verts_roof[p].vec_pos = p_local_verts_roof[i_v].vec_pos;
	p++;
	p_local_verts_roof[p].vec_pos = p_local_verts_roof[i_v+2].vec_pos;
	p++;
   	p_local_verts_roof[p].vec_pos.x = f_pos_x + f_spacer; 
	p_local_verts_roof[p].vec_pos.z = f_pos_z; 
	if (b_relative_height)
	{
		p_local_verts_roof[p].vec_pos.y = f_height + cls_tglo.p_terrain->get_height(p_local_verts_roof[p].vec_pos.x, p_local_verts_roof[p].vec_pos.z);//height
	} else
	{
	 p_local_verts_roof[p].vec_pos.y = f_height;
	}


	
 	const DWORD dw_color =  D3DCOLOR_RGBA( 255,255, 255,0 ); //white

	//now handle the normals
	for (int h=i_v; h < (i_v+6); h++)
	{
	p_local_verts_roof[h].vec_normal = g_vec_down;
	p_local_verts_roof[h].dw_color = dw_color;
	}




  	static float f_xm, f_ym;

		f_xm = 	cls_tglo.p_map->p_sub_manager->p_a_sub_zone[this->i_sub_zone].f_roof_tex_x;
		f_ym = 	cls_tglo.p_map->p_sub_manager->p_a_sub_zone[this->i_sub_zone].f_roof_tex_y;
	
	//handle texture coordinates

		for (int i=i_v; i < i_v+6; i++)
		{
		p_local_verts_roof[i].vec_tex.x = p_local_verts_roof[i].vec_pos.x*f_xm;
		p_local_verts_roof[i].vec_tex.y = p_local_verts_roof[i].vec_pos.z*f_ym;
		}




}

void CBuilding::build_all_roof_tiles()
{
  //the walls have been built, but not let's build the roof tiles if we need to

   if (i_roof_tiles == 0) return; //no roof tiles to be drawn
  for (int i=0; i < this->i_roof_tiles; i++)
  {
 cls_tglo.p_map->get_tile_extra_pointer(this->p_building_roof_tile_info[i].i_x, 
		 this->p_building_roof_tile_info[i].i_z)->i_roof_index = i;
	 create_roof_quad(i);
	 //set lookup
	
  }

  


}

void flip_sign(float *p_float)
{
  *p_float = 0-*p_float;

}


void CBuilding::build_virt_quad_from_two_verts(CBuildVertex *p_vert, float f_height, int i_side)
{

	bool b_relative_height = false;
	if (cls_tglo.p_map->p_sub_manager->p_a_sub_zone[i_sub_zone].i_ceil_type == ::C_SUBZONE_CEIL_RELATIVE)
	{
	 b_relative_height = true;
	}
	

	
	
	//tri one create vertex 1
	p_vert[1] = p_vert[0];
	if (b_relative_height)
	p_vert[1].vec_pos.y += f_height; 
	else 
	p_vert[1].vec_pos.y = f_height; 

	//tri one, create vertex 2
	p_vert[2] = p_vert[5];
	if (b_relative_height)
	p_vert[2].vec_pos.y += f_height;
	else 
	p_vert[2].vec_pos.y = f_height; 

	//tri two, create vertex 1
	p_vert[3] = p_vert[0];
	p_vert[4] = p_vert[2];

	//set texture coords



	if (cls_tglo.p_map->p_sub_manager->p_a_sub_zone[this->i_sub_zone].i_wall_tex_style == ::C_SUBZONE_WALL_TEX_LOCAL)
	{
	//face mapping
	p_vert[0].vec_tex.x = 0;
	p_vert[0].vec_tex.y = 0;

	p_vert[1].vec_tex.x = 0;
	p_vert[1].vec_tex.y = cls_tglo.p_map->p_sub_manager->p_a_sub_zone[i_sub_zone].f_wall_tex_y;

	p_vert[2].vec_tex.x = cls_tglo.p_map->p_sub_manager->p_a_sub_zone[i_sub_zone].f_wall_tex_x;
	p_vert[2].vec_tex.y = cls_tglo.p_map->p_sub_manager->p_a_sub_zone[i_sub_zone].f_wall_tex_y;
	
	p_vert[3].vec_tex.x = 0;
	p_vert[3].vec_tex.y = 0;

	p_vert[4].vec_tex.x =  cls_tglo.p_map->p_sub_manager->p_a_sub_zone[i_sub_zone].f_wall_tex_x;
	p_vert[4].vec_tex.y = cls_tglo.p_map->p_sub_manager->p_a_sub_zone[i_sub_zone].f_wall_tex_y;

	p_vert[5].vec_tex.x =  cls_tglo.p_map->p_sub_manager->p_a_sub_zone[i_sub_zone].f_wall_tex_x;
	p_vert[5].vec_tex.y = 0;
	} 


 	if (cls_tglo.p_map->p_sub_manager->p_a_sub_zone[this->i_sub_zone].i_wall_tex_style == ::C_SUBZONE_WALL_TEX_WORLD)
	{
		static float f_x, f_y;
		static float f_xm, f_ym;

		f_xm = 	cls_tglo.p_map->p_sub_manager->p_a_sub_zone[this->i_sub_zone].f_wall_tex_x;
		f_ym = 	cls_tglo.p_map->p_sub_manager->p_a_sub_zone[this->i_sub_zone].f_wall_tex_y;
		//mapping is based on world information
		if ( (i_side == ::C_BUILDING_TEX_NORTH || (i_side == ::C_BUILDING_TEX_SOUTH) ))
		{
		  	p_vert[0].vec_tex.x = p_vert[0].vec_pos.x * f_xm;
			p_vert[0].vec_tex.y = p_vert[0].vec_pos.y * f_ym;


		  	p_vert[1].vec_tex.x = p_vert[1].vec_pos.x * f_xm;

			p_vert[1].vec_tex.y = p_vert[1].vec_pos.y * f_ym;

		  	p_vert[2].vec_tex.x = p_vert[2].vec_pos.x * f_xm;

			p_vert[2].vec_tex.y = p_vert[2].vec_pos.y * f_ym;

		 
			//final vert

			p_vert[5].vec_tex.x = p_vert[5].vec_pos.x * f_xm;

			p_vert[5].vec_tex.y = p_vert[5].vec_pos.y * f_ym;


		    //reverse tex

			flip_sign(&p_vert[0].vec_tex.x);
			flip_sign(&p_vert[0].vec_tex.y);
			flip_sign(&p_vert[1].vec_tex.x);
			flip_sign(&p_vert[1].vec_tex.y);
			flip_sign(&p_vert[2].vec_tex.x);
			flip_sign(&p_vert[2].vec_tex.y);
			flip_sign(&p_vert[5].vec_tex.x);
			flip_sign(&p_vert[5].vec_tex.y);

			 	//copies of other verts

			p_vert[3].vec_tex.x = p_vert[0].vec_tex.x;
		  	p_vert[3].vec_tex.y = p_vert[0].vec_tex.y;
			p_vert[4].vec_tex.x = p_vert[2].vec_tex.x;
		  	p_vert[4].vec_tex.y = p_vert[2].vec_tex.y;


		} else
		{

			//must be an east/west wall

			p_vert[0].vec_tex.x = p_vert[0].vec_pos.z * f_xm;
			p_vert[0].vec_tex.y = p_vert[0].vec_pos.y * f_ym;
		  	p_vert[1].vec_tex.x = p_vert[1].vec_pos.z * f_xm;
			p_vert[1].vec_tex.y = p_vert[1].vec_pos.y * f_ym;
		  	p_vert[2].vec_tex.x = p_vert[2].vec_pos.z * f_xm;
			p_vert[2].vec_tex.y = p_vert[2].vec_pos.y * f_ym;
		  
			//final vert

			p_vert[5].vec_tex.x = p_vert[5].vec_pos.z * f_xm;

			p_vert[5].vec_tex.y = p_vert[5].vec_pos.y * f_ym;

			flip_sign(&p_vert[0].vec_tex.x);
			flip_sign(&p_vert[0].vec_tex.y);
			flip_sign(&p_vert[1].vec_tex.x);
			flip_sign(&p_vert[1].vec_tex.y);
			flip_sign(&p_vert[2].vec_tex.x);
			flip_sign(&p_vert[2].vec_tex.y);
			flip_sign(&p_vert[5].vec_tex.x);
			flip_sign(&p_vert[5].vec_tex.y);

				//copies of other verts

			p_vert[3].vec_tex.x = p_vert[0].vec_tex.x;
		  	p_vert[3].vec_tex.y = p_vert[0].vec_tex.y;
			p_vert[4].vec_tex.x = p_vert[2].vec_tex.x;
		  	p_vert[4].vec_tex.y = p_vert[2].vec_tex.y;

		}
	} 


		 //compute lighting

	DWORD dw_color =  D3DCOLOR_RGBA( 255,255, 255,0 ); //white
	
	for (int i=0; i < 6; i++)
	{
		p_vert[i].dw_color = dw_color;
		//map coords to worldspace
	//	p_vert[i].vec_tex.x = p_vert[i].vec_pos.x/3;
	//	p_vert[i].vec_tex.y = p_vert[i].vec_pos.y/3;

	}


}

void CBuilding::set_quad_normal(CBuildVertex *p_vert, float nx, float ny, float nz)
{
  p_vert[0].vec_normal.x = nx;
  p_vert[0].vec_normal.z = nz;
  p_vert[0].vec_normal.y = ny;

  p_vert[1].vec_normal.x = nx;
  p_vert[1].vec_normal.z = nz;
  p_vert[1].vec_normal.y = ny;

  p_vert[2].vec_normal.x = nx;
  p_vert[2].vec_normal.z = nz;
  p_vert[2].vec_normal.y = ny;

  p_vert[3].vec_normal.x = nx;
  p_vert[3].vec_normal.z = nz;
  p_vert[3].vec_normal.y = ny;

  p_vert[4].vec_normal.x = nx;
  p_vert[4].vec_normal.z = nz;
  p_vert[4].vec_normal.y = ny;

  p_vert[5].vec_normal.x = nx;
  p_vert[5].vec_normal.z = nz;
  p_vert[5].vec_normal.y = ny;

}


void CBuilding::build_wall_quads(int i_buf_wall, int i_x, int i_z)
{
	int p = i_buf_wall*6*i_sides_per_wall; //find our place in the array
  
	float f_spacer = p_map->get_block_size() / float(p_map->i_tiles_per_block);
    float f_pos_x = float(i_x)*f_spacer;
    float f_pos_z = float(i_z)*f_spacer;
    float f_height = cls_tglo.p_map->p_sub_manager->p_a_sub_zone[i_sub_zone].f_wall_height;
	int i_tex = 2;

	int i_mapping_style = C_WALL_MAPPING_TILE;

	//do north wall first, set up the two bottom points at correct positions

	p_local_verts[p].vec_pos.x = f_pos_x;
	p_local_verts[p].vec_pos.z = f_pos_z+f_spacer;
	p_local_verts[p].vec_pos.y = cls_tglo.p_terrain->get_height(p_local_verts[p].vec_pos.x, p_local_verts[p].vec_pos.z);//height

	p_local_verts[p+5].vec_pos.x = f_pos_x+f_spacer;
	p_local_verts[p+5].vec_pos.z = f_pos_z+f_spacer;
	p_local_verts[p+5].vec_pos.y = cls_tglo.p_terrain->get_height(p_local_verts[p+5].vec_pos.x, p_local_verts[p+5].vec_pos.z);//height

	//auto-build the rest of this quad
	build_virt_quad_from_two_verts(&p_local_verts[p], f_height, C_BUILDING_TEX_NORTH);

	set_quad_normal(&p_local_verts[p], 0,1,1);

	//do west wall
	p += 6; //advance to next wall piece

	p_local_verts[p].vec_pos.x = f_pos_x;
	p_local_verts[p].vec_pos.z = f_pos_z;
	p_local_verts[p].vec_pos.y = cls_tglo.p_terrain->get_height(p_local_verts[p].vec_pos.x, p_local_verts[p].vec_pos.z);//height

	p_local_verts[p+5].vec_pos.x = f_pos_x;
	p_local_verts[p+5].vec_pos.z = f_pos_z+f_spacer;
	p_local_verts[p+5].vec_pos.y = cls_tglo.p_terrain->get_height(p_local_verts[p+5].vec_pos.x, p_local_verts[p+5].vec_pos.z);//height

 	build_virt_quad_from_two_verts(&p_local_verts[p], f_height, C_BUILDING_TEX_WEST);

	set_quad_normal(&p_local_verts[p], -1,1,0);

	//east wall
   
	p += 6; //advance to next wall piece

	p_local_verts[p].vec_pos.x = f_pos_x+f_spacer;
	p_local_verts[p].vec_pos.z = f_pos_z;
	p_local_verts[p].vec_pos.y = cls_tglo.p_terrain->get_height(p_local_verts[p].vec_pos.x, p_local_verts[p].vec_pos.z);//height

	p_local_verts[p+5].vec_pos.x = f_pos_x+f_spacer;
	p_local_verts[p+5].vec_pos.z = f_pos_z+f_spacer;
	p_local_verts[p+5].vec_pos.y = cls_tglo.p_terrain->get_height(p_local_verts[p+5].vec_pos.x, p_local_verts[p+5].vec_pos.z);//height

	build_virt_quad_from_two_verts(&p_local_verts[p], f_height, C_BUILDING_TEX_EAST);

  	set_quad_normal(&p_local_verts[p], 1,1,0);


  	//south wall
   
	p += 6; //advance to next wall piece

	p_local_verts[p].vec_pos.x = f_pos_x;
	p_local_verts[p].vec_pos.z = f_pos_z;
	p_local_verts[p].vec_pos.y = cls_tglo.p_terrain->get_height(p_local_verts[p].vec_pos.x, p_local_verts[p].vec_pos.z);//height

	p_local_verts[p+5].vec_pos.x = f_pos_x+f_spacer;
	p_local_verts[p+5].vec_pos.z = f_pos_z;
	p_local_verts[p+5].vec_pos.y = cls_tglo.p_terrain->get_height(p_local_verts[p+5].vec_pos.x, p_local_verts[p+5].vec_pos.z);//height

 	build_virt_quad_from_two_verts(&p_local_verts[p], f_height, C_BUILDING_TEX_SOUTH);

  	set_quad_normal(&p_local_verts[p], 0,1,-1);

	//set our number
	cls_tglo.p_map->p_tile_extra[i_x+(i_z*cls_tglo.p_map->i_tile_x)].i_wall = i_buf_wall;
}


void CBuilding::dx_create_walls()
{
  dx_invalidate_walls();

	HRESULT hr;


  //create our wall vertex buffer
  if (i_walls > 0)
  {

 	if( FAILED(hr =  cls_tglo.p_d3dDevice->CreateVertexBuffer (	i_vertex_bytes, D3DUSAGE_WRITEONLY,
													D3DFVF_BUILD3D, 
													D3DPOOL_DEFAULT, 
													&p_vertex_buffer ) ) )
	{
	 	    log_error("CBuilding: CreateVertexBuffer (%s)", DXGetErrorString8(hr));

	    return;
	}


	update_wall_vertex_data();

	for ( int i_buf_wall =0; i_buf_wall < i_walls; i_buf_wall++)
	{
		for (int i=0; i < this->i_sides_per_wall; i++)
		{
			this->p_building_tile_info[i_buf_wall].refresh_texture(i);
		}
	}
  }
	//also create roof tiles
    if (i_roof_tiles > 0)
  {


  	if( FAILED(hr =  cls_tglo.p_d3dDevice->CreateVertexBuffer (	i_vertex_bytes_roof, D3DUSAGE_WRITEONLY,
													D3DFVF_BUILD3D, 
													D3DPOOL_DEFAULT, 
													&p_vertex_buffer_roof ) ) )
	{
	
	 	    log_error("CBuilding: CreateVertexBuffer (%s)", DXGetErrorString8(hr));
	    return;
	}


	update_roof_vertex_data();

	for ( int i_buf_roof =0; i_buf_roof < i_roof_tiles; i_buf_roof++)
	{
		this->p_building_roof_tile_info[i_buf_roof].refresh_texture();
	}
 	}

}		  

void CBuilding::dx_invalidate_walls()
{
  SAFE_RELEASE(p_vertex_buffer);
  SAFE_RELEASE(p_vertex_buffer_roof);
}


void CBuilding::update_wall_vertex_data()
{

	//unlock vertex buffer and copy our vertex data into it
		HRESULT hr;
		// Lock the Vertex Buffer
		CBuildVertex *pVertices = NULL;
		if (FAILED(hr = p_vertex_buffer->Lock(0, 0, (BYTE**)&pVertices, 0)))
		{
		  log_error("Unable to lock p_vertex_buffer in CBuilding.cpp"); 
  	 	    log_error("CBuilding: UpdateWallVertex (%s)", DXGetErrorString8(hr));
		  return;
		}

	   	memcpy(pVertices, p_local_verts, i_vertex_bytes);
	    p_vertex_buffer->Unlock();

}


void CBuilding::update_roof_vertex_data()
{

	//unlock vertex buffer and copy our vertex data into it
		HRESULT hr;
		// Lock the Vertex Buffer
		CBuildVertex *pVertices = NULL;
		if (FAILED(hr = p_vertex_buffer_roof->Lock(0, 0, (BYTE**)&pVertices, 0)))
		{
		  log_error("Unable to lock p_vertex_buffer_roof in CBuilding.cpp (%s)", DXGetErrorString8(hr)); 
		  return;
		}

	   	memcpy(pVertices, p_local_verts_roof, i_vertex_bytes_roof);
	    p_vertex_buffer_roof->Unlock();

}


void CBuilding::dx_draw()
{
  	//it's possible we are deleted but getting one last draw command anyway...
	
    if (p_vertex_buffer)
    {
        

        dx_draw_prepare();
        set_world_transform();
        dx_draw_walls();
        dx_draw_roof();
        
        CVisual::dx_draw();
    }
    
}

void CBuilding::dx_restore()
{
   if ( (!p_vertex_buffer) && (!p_vertex_buffer_roof)) 
	{
		dx_create_walls();
	}
   
}

void CBuilding::dx_create()
{
	if (!cls_tglo.p_d3dDevice) return; //d3d doesn't exist yet

	//only create the walls if they don't exist
	if ( (!p_vertex_buffer) && (!p_vertex_buffer_roof)) 
	{
		dx_create_walls();
	}

}

void CBuilding::dx_invalidate()
{
 dx_invalidate_walls();

}

void CBuilding::dx_draw_walls()
{

	cls_tglo.p_d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
//	cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    
//	cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
//	  
//	   cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	   cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	
	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	//let's force only 2 bit alpha basically, stops the bilinear filtering from
	//blurring around the edges.
	cls_tglo.p_d3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	cls_tglo.p_d3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER );
	cls_tglo.p_d3dDevice->SetRenderState(D3DRS_ALPHAREF,  128); // tolerance
	cls_tglo.p_d3dDevice->SetVertexShader (D3DFVF_BUILD3D);
	cls_tglo.p_d3dDevice->SetStreamSource (0, p_vertex_buffer, sizeof(CBuildVertex));
  
	static int i_cur_position;
	//first draw each wall
	for (int w=0; w < i_walls; w++)
	{
	 	i_cur_position = w*24;	//only works with 4 sided walls!

		//should we draw the north wall?

		if (this->p_building_tile_info[w].b_wall_active[::C_BUILDING_TEX_NORTH])
		{
		//set texture for north wall
		cls_tglo.p_d3dDevice->SetTexture( 0,p_building_tile_info[w].tex[C_BUILDING_TEX_NORTH]);
	
		//draw
		cls_tglo.p_d3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, i_cur_position, 2); // number of primitives
		i_cur_position += 6;
		} else i_cur_position += 6; //skip wall

		if (this->p_building_tile_info[w].b_wall_active[::C_BUILDING_TEX_WEST])
		{
		//set texture for west wall
	  		cls_tglo.p_d3dDevice->SetTexture( 0,p_building_tile_info[w].tex[C_BUILDING_TEX_WEST]);

		//draw
		cls_tglo.p_d3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, i_cur_position, 2); // number of primitives
		i_cur_position += 6;
		} else i_cur_position += 6; //skip wall

		if (this->p_building_tile_info[w].b_wall_active[::C_BUILDING_TEX_EAST])
		{
		//set texture for east wall
	  	  		cls_tglo.p_d3dDevice->SetTexture( 0,p_building_tile_info[w].tex[C_BUILDING_TEX_EAST]);

		//draw
		cls_tglo.p_d3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, i_cur_position, 2); // number of primitives
		i_cur_position += 6;
		} else i_cur_position += 6; //skip wall

		if (this->p_building_tile_info[w].b_wall_active[::C_BUILDING_TEX_SOUTH])
		{
		//set texture for south wall
   	  		cls_tglo.p_d3dDevice->SetTexture( 0,p_building_tile_info[w].tex[C_BUILDING_TEX_SOUTH]);

		//draw
		cls_tglo.p_d3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, i_cur_position, 2); // number of primitives
		i_cur_position += 6;
		} else i_cur_position += 6; //skip wall
	
	}
	cls_tglo.p_d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,    FALSE );
	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
	cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
			
}

void CBuilding::dx_draw_roof()
{
  	 
	cls_tglo.p_d3dDevice->SetStreamSource (0, p_vertex_buffer_roof, sizeof(CBuildVertex));
  
	static int i_cur_position;
	//first draw each wall
	for (int w=0; w < i_roof_tiles; w++)
	{
	i_cur_position = w*6;	//draw the two tris for the roof
		//draw
		//set texture for roof
	cls_tglo.p_d3dDevice->SetTexture( 0,p_building_roof_tile_info[w].tex);
	cls_tglo.p_d3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, i_cur_position, 2); // number of primitives
	}

}

	bool CBuilding::does_ray_hit_wall(int i_wall_to_check, D3DXVECTOR3 *p_point_a, D3DXVECTOR3 *p_point_b, int *i_out_wall_part,
		D3DXVECTOR3 *vec_out_hit, float *p_f_out_dist)
	{
		//check this wall for hits
		
		static float f_dist;
		static float f_last_dist;
		
		f_last_dist = 200000000;
		
		static D3DXPLANE plane;
		//where is the real vertex information for this wall?
		int p = i_wall_to_check*6*i_sides_per_wall; //find our place in the array
		
		static D3DXVECTOR3 tri[3];
		static D3DXVECTOR3 vec_dir, vec_temp;
		
		vec_dir = (*p_point_a - *p_point_b);
		static bool b_hit;
		
		for (int i=0; i < i_sides_per_wall; i++)
		{
			if (this->p_building_tile_info[i_wall_to_check].b_wall_active[i])
			{
				b_hit = false;
				
				//check tri 1
				tri[0] = p_local_verts[p+(6*i)].vec_pos;
				tri[1] = p_local_verts[p+(6*i)+1].vec_pos;
				tri[2] = p_local_verts[p+(6*i)+2].vec_pos;
				
				if (IntersectTriangle(vec_out_hit, *p_point_a, vec_dir, tri[0], tri[1], tri[2]))
				{
					D3DXPlaneFromPoints(&plane,&tri[0],&tri[1],&tri[2]);
					D3DXPlaneIntersectLine(vec_out_hit,&plane,p_point_a,p_point_b);
					//only count this if it is front of us
					float f_theta = get_theta_from_x_and_z(p_point_a->x-vec_out_hit->x,
						p_point_a->z-vec_out_hit->z);
					
					float f_theta_facing =  get_theta_from_x_and_z(vec_dir.x,
						vec_dir.z);
				
					
					if (thetas_are_facing_same_way(f_theta, f_theta_facing, SETH_PI_OVER2))
						{
							b_hit = true;
						}

					
				}
				
				if (!b_hit)
				{
					
					//check tri 2
					tri[0] = p_local_verts[p+(6*i)+3].vec_pos;
					tri[1] = p_local_verts[p+(6*i)+4].vec_pos;
					tri[2] = p_local_verts[p+(6*i)+5].vec_pos;
					
					if (IntersectTriangle(vec_out_hit, *p_point_a, vec_dir, tri[0], tri[1], tri[2]))
					{
						D3DXPlaneFromPoints(&plane,&tri[0],&tri[1],&tri[2]);
						D3DXPlaneIntersectLine(vec_out_hit,&plane,p_point_a,p_point_b);
						//only count this if it is front of us
						float f_theta = get_theta_from_x_and_z(p_point_a->x-vec_out_hit->x,
							p_point_a->z-vec_out_hit->z);
						
						float f_theta_facing =  get_theta_from_x_and_z(vec_dir.x,
							vec_dir.z);
				
		if (thetas_are_facing_same_way(f_theta, f_theta_facing, SETH_PI_OVER2))
						{
							b_hit = true;
						}
						
					}
				}
					
					if (b_hit)
					{
						//return hit info
						//figure out exact spot we hit it
						D3DXPlaneFromPoints(&plane,&tri[0],&tri[1],&tri[2]);
						D3DXPlaneIntersectLine(vec_out_hit,&plane,p_point_a,p_point_b);
						vec_temp = (*p_point_a - *vec_out_hit);	
						f_dist = pow(vec_temp.x, 2)+pow(vec_temp.y, 2)+pow(vec_temp.z, 2);
						if (f_dist < f_last_dist)
						{
							//store it
							*i_out_wall_part = i;
							*p_f_out_dist = f_dist;
							f_last_dist = f_dist;
						}
						
					}
					
				}
			}
			
			
			//did we get a hit?
			if (f_last_dist != 200000000) return true; //yes, so the return stuff has already been set
			
			return false;
		}



bool CBuilding::does_ray_hit_roof(int i_roof_to_check, D3DXVECTOR3 *p_point_a, D3DXVECTOR3 *p_point_b, int *i_out_wall_part,
		D3DXVECTOR3 *vec_out_hit, float *p_f_out_dist)
{
		int p = i_roof_to_check*6; //find our place in the array
		static D3DXVECTOR3 tri[3];
		static D3DXVECTOR3 vec_dir, vec_temp;
	   	static float f_dist;
		static D3DXPLANE plane;
		vec_dir = (*p_point_a - *p_point_b);
		static bool b_hit;
		
		b_hit = false;
		//does the ray go through this wall?
	 	tri[0] = p_local_verts_roof[p].vec_pos;
		tri[1] = p_local_verts_roof[p+1].vec_pos;
		tri[2] = p_local_verts_roof[p+2].vec_pos;
				
				if (IntersectTriangle(vec_out_hit, *p_point_a, vec_dir, tri[0], tri[1], tri[2]))
				{
					D3DXPlaneFromPoints(&plane,&tri[0],&tri[1],&tri[2]);
					D3DXPlaneIntersectLine(vec_out_hit,&plane,p_point_a,p_point_b);
					//only count this if it is front of us
					float f_theta = get_theta_from_x_and_z(p_point_a->x-vec_out_hit->x,
						p_point_a->z-vec_out_hit->z);
					
					float f_theta_facing =  get_theta_from_x_and_z(vec_dir.x,
						vec_dir.z);
				
					
					if (thetas_are_facing_same_way(f_theta, f_theta_facing, SETH_PI_OVER2))
						{
							b_hit = true;
						}

				}

				if (!b_hit)
				{
					//still looking for a collision
					tri[0] = p_local_verts_roof[p+3].vec_pos;
					tri[1] = p_local_verts_roof[p+4].vec_pos;
					tri[2] = p_local_verts_roof[p+5].vec_pos;
					
					if (IntersectTriangle(vec_out_hit, *p_point_a, vec_dir, tri[0], tri[1], tri[2]))
					{
						D3DXPlaneFromPoints(&plane,&tri[0],&tri[1],&tri[2]);
						D3DXPlaneIntersectLine(vec_out_hit,&plane,p_point_a,p_point_b);
						//only count this if it is front of us
						float f_theta = get_theta_from_x_and_z(p_point_a->x-vec_out_hit->x,
							p_point_a->z-vec_out_hit->z);
						
						float f_theta_facing =  get_theta_from_x_and_z(vec_dir.x,
							vec_dir.z);
						
						
						if (thetas_are_facing_same_way(f_theta, f_theta_facing, SETH_PI_OVER2))
						{
							b_hit = true;
						}
						
					}
					
				}
				

				if (b_hit)
				{
					vec_temp = (*p_point_a - *vec_out_hit);	
					f_dist = pow(vec_temp.x, 2)+pow(vec_temp.y, 2)+pow(vec_temp.z, 2);
					
					*i_out_wall_part = C_BUILDING_TEX_TOP;
					*p_f_out_dist = f_dist;
					return true;
				}
 return false;
}
