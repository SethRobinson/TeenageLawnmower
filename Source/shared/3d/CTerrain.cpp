/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


//Based on code by Neil Haran 
 
#include "CTerrain.h"
#include "..\\vectormath.h"
#include "CThing_util.h"
#include "material.h"
#include "CThing_globals.h"
#include "CGridTile.h"
#include "CBuildingManager.h"
#include "CMessageHeader.h"
#include "CCamera.h"
#include "CThingList.h"
#include <DXErr8.h>

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/******************************************************************************
	Construction/Destruction
*****************************************************************************/

#define GET_BLOCK(x, y) x+(y*p_map->get_size_x())

void terrain_block_info::refresh_texture(int i_tex)
{
 
  if (cls_tglo.p_manager->does_resource_exist(i_tex) == false)
  {
   log_error("Texture id %d does not exist.  Check your .ini files.",i_tex);
   
    if (!cls_tglo.p_manager->get(i_tex))
    {
       cls_tglo.p_manager->add_resource_by_string(1, C_RESOURCE_TEXTURE, C_RESOURCE_SUB_LANDSCAPE,
           "unknown.dds");

    }
    this->tex = ((CResourceTex*)cls_tglo.p_manager->get(1))->get_texture();
    
	return;
  }
  if (((CResourceTex*)cls_tglo.p_manager->get(i_tex))->get_texture() == NULL)
  {
    //add ref and load it
	((CResourceTex*)cls_tglo.p_manager->get(i_tex))->add_ref();
	((CResourceTex*)cls_tglo.p_manager->get(i_tex))->dx_create();
  }

  this->tex = ((CResourceTex*)cls_tglo.p_manager->get(i_tex))->get_texture();

}


void terrain_block_info::refresh_overlay_texture(int i_tex)
{
  if (i_tex == 0)
  {
	  i_tex = 207; //force this texture which must be defined as a transparent texture.  This way
	  //I don't have to mess with switching the render stages to be compatible with gf2 cards
//	  p_overlay_tex = NULL;
//	  return;

  }
  if (cls_tglo.p_manager->does_resource_exist(i_tex) == false)
  {
   log_error("Overlay texture id %d does not exist.  Check your .ini files.",i_tex);
    if (!cls_tglo.p_manager->get(207))
    {

     cls_tglo.p_manager->add_resource_by_string(207, C_RESOURCE_TEXTURE, C_RESOURCE_SUB_LANDSCAPE,
           "transparent.dds");


    }
    this->p_overlay_tex = ((CResourceTex*)cls_tglo.p_manager->get(207))->get_texture();
	return;
  }
  if (((CResourceTex*)cls_tglo.p_manager->get(i_tex))->get_texture() == NULL)
  {
    //add ref and load it
	((CResourceTex*)cls_tglo.p_manager->get(i_tex))->add_ref();
	((CResourceTex*)cls_tglo.p_manager->get(i_tex))->dx_create();
  }

  p_overlay_tex  = ((CResourceTex*)cls_tglo.p_manager->get(i_tex))->get_texture();

}

CTerrain::CTerrain()
{
    m_pVBCTerrain		= NULL;
    m_pIBCTerrain		= NULL;
    m_pTerrainVertices  = NULL;
    m_dwSizeOfVertices	= 0;
    m_iVertexCount		= 0;
    m_iTriangleCount	= 0;
    p_binfo = NULL;
    i_blocks_per_texture = 3; //each texture will take a 3X3 grid
    p_map = NULL;
    b_show_grid = false;
    set_thing_name("Terrain");
    set_sort(-5);
    
}

CTerrain::~CTerrain()
{
  		SAFE_RELEASE(m_pVBCTerrain); 
        SAFE_RELEASE(m_pIBCTerrain); 
        SAFE_DELETE_ARRAY( m_pTerrainVertices );
        SAFE_DELETE_ARRAY( p_binfo); 
        
}

void CTerrain::receive_message(CMessageHeader *p_header, void *p_message)
{
    if (p_header->i_message == C_THING_MSG_GENERIC)
    {
        if ( ((CMessageGeneric*)p_message)->i_message_command == C_MESSAGE_COMMAND_TOGGLE_WIREFRAME)
        {
           b_show_grid = !b_show_grid;

        }
    }
}
	

void CTerrain::think()
{
  if (cls_tglo.p_cls_input->is_key_tapped(DIK_1))
  {
    //toggle wireframe overlay
	  b_show_grid = !b_show_grid;
  }

};

void CTerrain::update_terrain_block_from_map(int i_x, int i_y)
{
	//make sure our texture pointer is current
	p_binfo[GET_BLOCK(i_x, i_y)].refresh_texture(p_map->p_block[GET_BLOCK(i_x,i_y)].i_tex_id);
	p_binfo[GET_BLOCK(i_x, i_y)].refresh_overlay_texture(p_map->p_block[GET_BLOCK(i_x,i_y)].i_overlay_tex_id);

	//recreate the quad with data from the map.  Skip normal and height calcs though
	this->update_block_information(i_x, i_y);
	

}

void CTerrain::set_map_source(CMapZone *p_new_map)
{
   dx_invalidate(); //kill old data if we had any
   	SAFE_DELETE_ARRAY( m_pTerrainVertices );
	SAFE_DELETE_ARRAY( p_binfo);
   p_map = p_new_map; //set our source of info
  init_terrain_buffers();
   //Generate the CTerrain.
   GenerateCTerrain();
//setup map info
   p_binfo = new terrain_block_info[p_map->get_size_x() * p_map->get_size_y()];

}

void CTerrain::get_tris_from_block(int i_block_x, int i_block_y, D3DXVECTOR3 *tri, D3DXVECTOR3 *tri2)
{

   	//let' grab the verts from the vert array.  A 3X3 block grid is actually 4X4 grid of
	//verts, it looks like this:

	/*

	0  1  2  3
	4  5  6  7
	8  9  10 11
	12 13 14 15


    Vert tri's are clockwise, like 0, 1, 5 and 1, 5,  4.

  */
   
	int i_location = (i_block_x+(i_block_y*(p_map->get_size_x())))*6;


	tri[0] = m_pTerrainVertices[i_location].get_pos();

	
	tri[1] = m_pTerrainVertices[i_location+1].get_pos();
	tri[2] = m_pTerrainVertices[i_location+2].get_pos();	

	//now for getting the second triangle
	
	tri2[0] = tri[0]; //they share this vert
	tri2[1] = tri[2]; //they share this vert too
	tri2[2] = m_pTerrainVertices[i_location+5].get_pos();	


}


float CTerrain::get_ground_and_y(float x, float z, D3DXVECTOR3 *vec_ground)
{

   	int i_block_x = p_map->get_block_x(x);
	int i_block_y = p_map->get_block_y(z);
	
	if ( (i_block_x < 0) || (i_block_x >= p_map->get_size_x())) return 0;
	if ( (i_block_y < 0) || (i_block_y >= p_map->get_size_y())) return 0;

	
	D3DXVECTOR3 tri[3]; //first triangle
	D3DXVECTOR3 tri2[3]; //second triangle

	get_tris_from_block(i_block_x, i_block_y, &tri[0], &tri2[0]);


	//we now have our triangles, let's hit them with a ray

	D3DXPLANE plane;	
   	D3DXVECTOR3 vec_answer;

	D3DXPlaneFromPoints(&plane,&tri[0],&tri[1],&tri[2]);
   
	//get plane info
	vec_ground->x = plane.a;
	vec_ground->y = plane.b;
	vec_ground->z = plane.c;

	D3DXVECTOR3 rayNear, rayFar;
	rayNear = D3DXVECTOR3(x, 200, z);
	rayFar = D3DXVECTOR3(x, -1, z);
	
    if (D3DXPlaneIntersectLine(&vec_answer,&plane,&rayNear,&rayFar))
	{
		//don't check Y
		tri[0].y = vec_answer.y;
		tri[1].y = vec_answer.y;
		tri[2].y = vec_answer.y;
	
		if (CheckPointInTriangle(vec_answer, tri[0], tri[1], tri[2]))
		{
			//found it
			//	log_msg("Found vec");
			return vec_answer.y;
		}
	}
	//still looking, check the other triangle

	D3DXPlaneFromPoints(&plane,&tri2[0],&tri2[1],&tri2[2]);
	//get plane info
	vec_ground->x = plane.a;
	vec_ground->y = plane.b;
	vec_ground->z = plane.c;

	 
	D3DXPlaneIntersectLine(&vec_answer,&plane,&rayNear,&rayFar);

	//let's assume we got the correct answer

	/*
		if (!CheckPointInTriangle(vec_answer, tri2[0], tri2[1], tri2[2]))
	{
	   log_msg("Error, couldn't find the point in the triangle!");
		} else
		{
		 log_msg("Found vec 2 ok");

		}
	  */
    	
	return vec_answer.y;



}

float CTerrain::get_height(float x, float z)
{
	//get the first triangle
//	return 0; //temp

	int i_block_x = p_map->get_block_x(x);
	int i_block_y = p_map->get_block_y(z);

	//special cases
	if (i_block_x ==  p_map->get_size_x())
	{
	  i_block_x -= 1;
	}

	
	if (i_block_y ==  p_map->get_size_y())
	{
	  i_block_y -= 1;
	}
	  

	if ( (i_block_x < 0) || (i_block_x >= p_map->get_size_x()))
	{
		return 0;
	}
	if ( (i_block_y < 0) || (i_block_y >= p_map->get_size_y()))
	{
		return 0;
	}

	
	D3DXVECTOR3 tri[3]; //first triangle
	D3DXVECTOR3 tri2[3]; //second triangle

	get_tris_from_block(i_block_x, i_block_y, &tri[0], &tri2[0]);


	//we now have our triangles, let's hit them with a ray

	D3DXPLANE plane;	
   	D3DXVECTOR3 vec_answer;

	D3DXPlaneFromPoints(&plane,&tri[0],&tri[1],&tri[2]);
   	
	D3DXVECTOR3 rayNear, rayFar;
	rayNear = D3DXVECTOR3(x, 200, z);
	rayFar = D3DXVECTOR3(x, -200, z);
	
    if (D3DXPlaneIntersectLine(&vec_answer,&plane,&rayNear,&rayFar))
	{
	
		//don't check Y
		tri[0].y = vec_answer.y;
		tri[1].y = vec_answer.y;
		tri[2].y = vec_answer.y;



		if (CheckPointInTriangle(vec_answer, tri[0], tri[1], tri[2]))
		{
			//found it
			//	log_msg("Found vec");
			return vec_answer.y;
		}
	}
	//still looking, check the other triangle

	D3DXPlaneFromPoints(&plane,&tri2[0],&tri2[1],&tri2[2]);
	 
	D3DXPlaneIntersectLine(&vec_answer,&plane,&rayNear,&rayFar);

	//let's assume we got the correct answer
	/* 
	  	tri2[0].y = vec_answer.y;
		tri2[1].y = vec_answer.y;
		tri2[2].y = vec_answer.y;

		if (!CheckPointInTriangle(vec_answer, tri2[0], tri2[1], tri2[2]))
	{
	    	
			log_msg("Error, couldn't find the point in the triangle!");
		} else
		{
		// log_msg("Found vec 2 ok");

		}
	  */
    	
	return vec_answer.y;
}

/*
  vec_a is the point in space
  vec_b is the DIRECTION vector

  */
//bool CTerrain::get_normal


bool CTerrain::find_ground_from_ray(D3DXVECTOR3 *p_vec_a,D3DXVECTOR3 *p_vec_b, D3DXVECTOR3 *p_vec_out_pos,
									D3DXPLANE *p_out_plane)
{
  	 D3DXVECTOR3 tri[3]; //first triangle
     D3DXVECTOR3 tri2[3]; //second triangle
  

	 D3DXVECTOR3 vec_dir = D3DXVECTOR3(0,-1,0);
	 // vec_mag = *p_vec_a - *p_vec_a;
	//D3DXVec3Normalize(&vec_dir, &vec_mag);
	 float f_closest_so_far = 10000000000;
	 float f_dist, f_scan_dist, f_scan_final;
	 static D3DXVECTOR3 vec_save_pos; //save the best so far here
	 static D3DXVECTOR3 vec_temp;
	 static D3DXPLANE plane_save;
	 int i_reject = 0;

	 D3DXVECTOR3 vec_point, vec_line_a, vec_line_b, vec_closest_point_from_line;
	 vec_line_a = *p_vec_a;
	 vec_line_b = *p_vec_b;
	 vec_line_a.y = 0;
	 vec_line_b.y = 0;
	 float f_dist_save = 0;
	float C_F_MAX_DISTANCE = p_map->get_block_size() * p_map->get_block_size()*2;   //if a block is more than this distance SQRT'ed from 
	 //our line we won't process it.
	 static char st_crap[100];
	 //let's cycle through every map block we have
	for (int x = 0; x < this->p_map->get_size_x(); x++)
	{

		for (int y = 0; y < this->p_map->get_size_y(); y++)
		{
	
			//first, does the line cross this tile?
			
			vec_point.x = x*this->p_map->get_block_size();
			vec_point.y = 0; //it's a 2d check
			vec_point.z = y*this->p_map->get_block_size();

		 	vec_closest_point_from_line = closestPointOnLine(vec_line_a, vec_line_b, vec_point);
			
	        vec_temp = (vec_point - vec_closest_point_from_line);
		
			f_scan_dist = vec_temp.x*vec_temp.x+vec_temp.y*vec_temp.y+vec_temp.z*vec_temp.z;
		//	sprintf(st_crap, "Dist is %.3f",f_dist);
		//	log_msg(st_crap);
			if (f_scan_dist < C_F_MAX_DISTANCE)
			{
				
				//did our line bump this tri?
				get_tris_from_block(x, y, &tri[0], &tri2[0]);
				D3DXPlaneFromPoints(p_out_plane,&tri[0],&tri[1],&tri[2]);
				
				if (D3DXPlaneIntersectLine(p_vec_out_pos,p_out_plane,p_vec_a,p_vec_b))
				{
					//	log_msg("Point on plane: %s", print_vector(*p_vec_out_pos));
					
					if (IntersectTriangle(p_vec_out_pos, *p_vec_out_pos, vec_dir, tri[0], tri[1], tri[2]))
					{
						//get distance
					     vec_temp = (*p_vec_a - *p_vec_out_pos);
						f_dist = pow(vec_temp.x, 2)+pow(vec_temp.y, 2)+pow(vec_temp.z, 2);
	
						if (f_dist < f_closest_so_far)
						{
							//it's good, mark it.
							vec_save_pos = *p_vec_out_pos;
							plane_save = *p_out_plane;
							f_closest_so_far = f_dist;
							f_scan_final = f_scan_dist;
						}
						
					} 
				}  
				
				//check other tri
				D3DXPlaneFromPoints(p_out_plane,&tri2[0],&tri2[1],&tri2[2]);
				if (D3DXPlaneIntersectLine(p_vec_out_pos,p_out_plane,p_vec_a,p_vec_b))
				{
					
					if (IntersectTriangle(p_vec_out_pos, *p_vec_out_pos, vec_dir, tri2[0], tri2[1], tri2[2]))
					{
					   vec_temp = (*p_vec_a - *p_vec_out_pos);
						f_dist = pow(vec_temp.x, 2)+pow(vec_temp.y, 2)+pow(vec_temp.z, 2);
	
						if (f_dist < f_closest_so_far)
						{
							//it's good, mark it.
							vec_save_pos = *p_vec_out_pos;
							plane_save = *p_out_plane;
							f_closest_so_far = f_dist;
								f_scan_final = f_scan_dist;

						}
						//	return true;
					} 
					
				}
			}  else
			{
			//	i_reject++;
				// log_msg("Skipping.");
			}
			
			
		}
	}
   if (f_closest_so_far != 10000000000)
   {
	 //found a good one
		//		sprintf(st_crap, "Final dist is %.3f. Tagged %s - Rejected: %d",f_scan_final,
		//			print_vector(vec_save_pos), i_reject);
		//	log_msg(st_crap);
      *p_vec_out_pos = vec_save_pos;
	  *p_out_plane = plane_save;
	return true;				
   } else
   {
		//		sprintf(st_crap, "FAILED: Final dist is %.3f",f_closest_so_far);
		//	log_msg(st_crap);


   }

   return false; //found nothing
}



/******************************************************************************
	Name:	GenerateCTerrain
	Desc:	Generates the Vertices and indices for the CTerrain.
	Param1: FLOAT CTerrainHeight - The max height value for the CTerrain.
	Param2: FLOAT heightScale	- The scaling of the height of the interpolation.
	Param3: FLOAT frequency		- The frequency in the number of bumps on the CTerrain.
	Param4: FLOAT freqScale		- The scaling of the frequency.
	Param5: int numPasses		- The number of iterations to run through.

 *****************************************************************************/
D3DXVECTOR3 CTerrain::get_normal(int X, int Z)
{
 
 static D3DXVECTOR3 Normal;	
 static int Heightmap_Width;
 static int Heightmap_Length;

Heightmap_Width = p_map->get_size_x() + 1;
Heightmap_Length = p_map->get_size_y() + 1;

//return (D3DXVECTOR3(1,0,0));

if (X >= Heightmap_Width) return Normal;
if (Z >= Heightmap_Length) return Normal;

//ASSERT(X < Heightmap_Width);
//ASSERT(Z < Heightmap_Length);

 // heights of the elements immediately surrounding the one at X,Z.
 float north, south, left, right;
 // 2 * world-coord space between height samples
 float space_between_height_samples_X2 = 2 * p_map->get_block_size() / 4;

 // heights of elements just outside the heightmap are taken from nearest inside hmap.

 if(Z == 0)
  north = GetElementHeight(X, Z);
 else
  north = GetElementHeight(X, Z - 1);

 if(Z == Heightmap_Length - 1)
  south = GetElementHeight(X, Z);
 else
  south = GetElementHeight(X, Z + 1);

 if(X == 0)
  left = GetElementHeight(X, Z);
 else
  left = GetElementHeight(X - 1, Z);

 if(X == Heightmap_Width - 1)
  right = GetElementHeight(X, Z);
 else
  right = GetElementHeight(X + 1, Z);

 Normal.z = 0 - (right - left)/(float)space_between_height_samples_X2;
 Normal.x = 0 - (south - north)/(float)space_between_height_samples_X2;
 Normal.y = 1;

 static D3DXVECTOR3 vec_final;
D3DXVec3Normalize(&vec_final, &Normal);
 return vec_final;
}


void CTerrain::init_terrain_buffers()
{
  
	//we're going to store 6 verts for every quad, vertex indices be damned
	m_iVertexCount = p_map->get_size_x()*p_map->get_size_y()*6;
	m_dwSizeOfVertices = m_iVertexCount * sizeof(VERTEX3D);

	//First allocate memory for the vertices.
	m_pTerrainVertices = new VERTEX3D[m_iVertexCount];
}


void CTerrain::set_normals_for_block(int i_x, int i_y)
{

 //set the normals for the 6 verts in this block
 int i_base = i_x*6+(i_y*p_map->get_size_x()*6);

 m_pTerrainVertices[i_base].setNormal(  this->get_normal(i_x, i_y));
 m_pTerrainVertices[i_base+1].setNormal(  this->get_normal(i_x, i_y+1));
 m_pTerrainVertices[i_base+2].setNormal(  this->get_normal(i_x+1, i_y+1));
 m_pTerrainVertices[i_base+5].setNormal( this->get_normal(i_x+1, i_y));
 
 
 //copy the ones that are the same
 m_pTerrainVertices[i_base+3].setNormal( m_pTerrainVertices[i_base].getNormal());
 m_pTerrainVertices[i_base+4].setNormal( m_pTerrainVertices[i_base+2].getNormal());

}

void CTerrain::GenerateCTerrain()
{
	//now let's build the vertex information.. we'll go 4 in a row for each
	//quad, left to right, starting at the highest Y.
 	for (int i_y = 0; i_y < p_map->get_size_y(); i_y++)
		{
		  	for(int i_x = 0; i_x < p_map->get_size_x(); i_x++)
			{
	    	update_block_information(i_x, i_y);
		 //great, let's fill in the normals for these 4 verts
	     
			set_normals_for_block(i_x, i_y);
			}
	}

 

}

//copy the vertex data, update texturing uv - for one block (two tris that make the quad, six vertexes)

void CTerrain::update_block_information(int i_x, int i_y)
{
	static int i_count;
	static int v = 0;
	static float tu, tv, tu2, tv2;
 	 
	
	i_count = i_x*6+(i_y*p_map->get_size_x()*6);
	     
	        v = 0;
			m_pTerrainVertices[i_count+v].setX( i_x * p_map->get_block_size());
			m_pTerrainVertices[i_count+v].setY( p_map->p_height_buffer[i_x + ( (i_y) * (p_map->get_size_x()+1))]);
			m_pTerrainVertices[i_count+v].setZ( (i_y * p_map->get_block_size()));
			m_pTerrainVertices[i_count+v].setColor( D3DCOLOR_XRGB( 255,255, 255 ) );
			//second vert
			v = 1;
			m_pTerrainVertices[i_count+v].setX( (i_x * p_map->get_block_size()));
			m_pTerrainVertices[i_count+v].setY( p_map->p_height_buffer[i_x + ((i_y+1) * (p_map->get_size_x()+1))]);
			m_pTerrainVertices[i_count+v].setZ( (i_y * p_map->get_block_size() ) + p_map->get_block_size() );
			m_pTerrainVertices[i_count+v].setColor( D3DCOLOR_XRGB( 255,255, 255 ) );
		    //third
   			v = 2;
			m_pTerrainVertices[i_count+v].setX( (i_x * p_map->get_block_size())+p_map->get_block_size());
			m_pTerrainVertices[i_count+v].setY( p_map->p_height_buffer[i_x+1 + ((i_y+1) * (p_map->get_size_x()+1))]);
			m_pTerrainVertices[i_count+v].setZ( (i_y * p_map->get_block_size())+p_map->get_block_size());
			m_pTerrainVertices[i_count+v].setColor( D3DCOLOR_XRGB( 255,255, 255 ) );
 		
		    //skipping two verts, we'll copy them later as they're the same as the first two.  Doing last vert of
			//second tri..
			v = 5; //final vert
			m_pTerrainVertices[i_count+v].setX( (i_x * p_map->get_block_size())+p_map->get_block_size());
			m_pTerrainVertices[i_count+v].setY( p_map->p_height_buffer[(i_x+1) + ((i_y) * (p_map->get_size_x()+1))]);
 		    m_pTerrainVertices[i_count+v].setZ( (i_y * p_map->get_block_size())  );
			m_pTerrainVertices[i_count+v].setColor( D3DCOLOR_XRGB( 255,255, 255 ) );
			m_pTerrainVertices[i_count+v].setTexCoords( (float) tu, (float) tv );


		  	tu = float(i_y) / float(i_blocks_per_texture);
			tv = float(i_x) / float(i_blocks_per_texture);
		
			double i_full_number;
			//mod it down
			tu = tu2 = float(modf(tu, &i_full_number));
			tv = tv2 = float(modf(tv, &i_full_number));

			static float f_y_offset, f_y_offset2;
			static float f_x_offset, f_x_offset2;

			f_y_offset = f_y_offset2 = (1/float(i_blocks_per_texture)); 
            f_x_offset = f_x_offset2 = (1/float(i_blocks_per_texture)); 
             
         		//now set the other tex coords (for overlays)
           if (p_map->p_block[GET_BLOCK(i_x,i_y)].bits_texture.get_bit(::C_MB_BITS_TEX_FACE_MAP))
            {
              //force the texture to fill up one tiny block, instead of 9
                tu2 = 0;
                tv2 = 0;
               f_x_offset2 = 1;
               f_y_offset2 = 1;
            }
        
     
            if (p_map->p_block[GET_BLOCK(i_x,i_y)].bits_texture.get_bit(::C_MB_BITS_TEX_FLIP_X))
			{
				
				//for Y mirroring:
				tu = 1.0f-tu; //Y
				f_y_offset *= (-1); //reverse it
			}
			
           	if (p_map->p_block[GET_BLOCK(i_x,i_y)].bits_texture.get_bit(::C_MB_BITS_TEX_FLIP_Z))
			{
				
				//for X mirroring:
				tv = 1.0f-tv; //X
				f_x_offset *= (-1); //reverse it
			} 
		
			m_pTerrainVertices[i_count].setTexCoords( tu,tv);
			m_pTerrainVertices[i_count+1].setTexCoords(tu+f_y_offset, tv);
			m_pTerrainVertices[i_count+2].setTexCoords( tu +f_y_offset, tv +f_x_offset);
			m_pTerrainVertices[i_count+5].setTexCoords( tu, tv+f_x_offset);

		
			
			//now set the other tex coords (for overlays)
           if (p_map->p_block[GET_BLOCK(i_x,i_y)].bits_texture.get_bit(::C_MB_BITS_TEX2_FACE_MAP))
            {
              //force the texture to fill up one tiny block, instead of 9
                tu2 = 0;
                tv2 = 0;
               f_x_offset2 = 1;
               f_y_offset2 = 1;
            }
        
				if (p_map->p_block[GET_BLOCK(i_x,i_y)].bits_texture.get_bit(::C_MB_BITS_TEX2_FLIP_X))
			{
				
				//for Y mirroring:
				tu2 = 1.0f-tu2; //Y
				f_y_offset2 *= (-1); //reverse it
			}
			
           	if (p_map->p_block[GET_BLOCK(i_x,i_y)].bits_texture.get_bit(::C_MB_BITS_TEX2_FLIP_Z))
			{
				
				//for X mirroring:
				tv2 = 1.0f-tv2; //X
				f_x_offset2 *= (-1); //reverse it
			} 
		
			m_pTerrainVertices[i_count].setTexCoords2( tu2,tv2);
			m_pTerrainVertices[i_count+1].setTexCoords2(tu2+f_y_offset2, tv2);
			m_pTerrainVertices[i_count+2].setTexCoords2( tu2 +f_y_offset2, tv2 +f_x_offset2);
			m_pTerrainVertices[i_count+5].setTexCoords2( tu2, tv2+f_x_offset2);




			//copy the first two verts directly
			memcpy(&m_pTerrainVertices[i_count+3] , &m_pTerrainVertices[i_count+0], sizeof(VERTEX3D)); //save vert really
			memcpy(&m_pTerrainVertices[i_count+4] , &m_pTerrainVertices[i_count+2], sizeof(VERTEX3D)); //save vert really
}

bool CTerrain::is_valid_vertex(int x, int y)
{
 	if ( (x < 0) | (y < 0)) return false;
	if (x > p_map->get_size_x()) return false;
	if (y > p_map->get_size_y()) return false;

	return true;

}

/*
	Without doing this you'll never see any changes made to a blocks settings.
  */
void CTerrain::update_block_in_vertex_buffer(int x, int y)
{
  	int i_vert_in_buf = x*6+(y*p_map->get_size_x()*6);
    
	HRESULT hr;
		// Lock the Vertex Buffer
		LPVERTEX3D pVertices = NULL;
		if (FAILED(hr = m_pVBCTerrain->Lock(0, 0, (BYTE**)&pVertices, 0)))
		{
		  log_error("Unable to lock m_pVBCTerrain in CTerrain.cpp"); 
  		  	 log_error("CTerrain (%s)", DXGetErrorString8(hr));

	  return;
		}

	   	memcpy(&pVertices[i_vert_in_buf], &m_pTerrainVertices[i_vert_in_buf], sizeof(VERTEX3D)*6);
	
	    m_pVBCTerrain->Unlock();

}


void CTerrain::update_vertex_height(int x, int y)
{
 	int i_vertex = x+(y*(p_map->get_size_x() + 1));
	int i_vert_in_buf = x*6+(y*p_map->get_size_x()*6);

//	m_pTerrainVertices[i_vert_in_buf].setY( p_map->p_height_buffer[i_vertex] );
	
    //now let's actually update the vertex buffer, this might have been on the video card so this
	//might be a slow process as Directx moves it back or whatever it does
	   HRESULT hr;
		// Lock the Vertex Buffer
		LPVERTEX3D pVertices = NULL;
		if (FAILED(hr = m_pVBCTerrain->Lock(0, 0, (BYTE**)&pVertices, 0)))
		{
		  log_error("Unable to lock m_pVBCTerrain in CTerrain.cpp"); 
  		  log_error("CTerrain (%s)", DXGetErrorString8(hr));

	  return;
		}
	
		//copy the one we changes (this is really slow, we're copying all six verts then we're copying
		//all the neighbors even though only the Y and normals could have changed...
   //let's also update the surrounding vertexs as the lighting may have changed
	
		int i_vertex_temp;

		int i_temp_x;
		int i_temp_y;

		//update the 16 blocks surrounded this vertex
		for (i_temp_x = x-2; i_temp_x < x+2; i_temp_x++)
			for (i_temp_y = y-2; i_temp_y < y+2; i_temp_y++)

		if (p_map->is_valid_block(i_temp_x, i_temp_y))
		{
			i_vertex_temp = (i_temp_x)*6+((i_temp_y)*p_map->get_size_x()*6);
			this->update_block_information(i_temp_x,i_temp_y);
			memcpy(&pVertices[i_vertex_temp], &m_pTerrainVertices[i_vertex_temp], sizeof(VERTEX3D)*6);
		}
		 
	 

		for (i_temp_x = x-2; i_temp_x < x+2; i_temp_x++)
			for (i_temp_y = y-2; i_temp_y < y+2; i_temp_y++)
				if (p_map->is_valid_block(i_temp_x, i_temp_y))
					
				{
				i_vertex_temp = (i_temp_x)*6+((i_temp_y)*p_map->get_size_x()*6);
			this->set_normals_for_block(i_temp_x, i_temp_y);
			memcpy(&pVertices[i_vertex_temp], &m_pTerrainVertices[i_vertex_temp], sizeof(VERTEX3D)*6);
	
		
					if (cls_tglo.p_grid_tile)
					{
						
						//let's also update our grid thingie
						cls_tglo.p_grid_tile->update_block_of_tiles(i_temp_x, i_temp_y);
					}

					if (cls_tglo.p_building_manager)
					{
					   //let's also update the effected tiles for building heights
						cls_tglo.p_building_manager->update_block_of_tiles(i_temp_x, i_temp_y);

					}
				}

  m_pVBCTerrain->Unlock();


}


/******************************************************************************
	Name:	BuildVertexBuffer
	Desc:	Fills up the vertex and index buffers with their respective info, 
			and sets what texture is to be used for mapping.
	Param1: const char*	szTextureName - The Texture for the CTerrain.
	History:
 *****************************************************************************/
void CTerrain::dx_create()
{
	this->dx_invalidate();

	HRESULT hr;
	if( m_pVBCTerrain != NULL )
	{
		SAFE_RELEASE(m_pVBCTerrain);
	}

 	
	// Create the vertex buffer, and then copy the vertices into the vertex buffer.
	if( FAILED(hr =  cls_tglo.p_d3dDevice->CreateVertexBuffer (	m_dwSizeOfVertices, D3DUSAGE_WRITEONLY,
													D3DFVF_VERTEX3D, 
													D3DPOOL_DEFAULT, 
													&m_pVBCTerrain ) ) )
	{
	
	 log_error("CTerrain CreateVertexBuffer (%s)", DXGetErrorString8(hr));

	    return;
	}

	// Lock the Vertex Buffer
	VOID* pVertices = NULL;
	if (FAILED(hr = m_pVBCTerrain->Lock(0, 0, (BYTE**)&pVertices, 0)))
	{
	  log_error("Unable to lock m_pVBCTerrain in CTerrain.cpp"); 
 	 log_error("CTerrain CreateVertexBuffer (%s)", DXGetErrorString8(hr));


	  return;
	}
	memcpy( pVertices, m_pTerrainVertices, m_dwSizeOfVertices );
	m_pVBCTerrain->Unlock();
	
	//great, now let's refresh the texture cache
   for (int x=0; x < p_map->get_size_x(); x++)
	   for (int y=0; y < p_map->get_size_y(); y++)
	   {
		 p_binfo[GET_BLOCK(x,y)].refresh_texture(p_map->p_block[GET_BLOCK(x,y)].i_tex_id);
   		 p_binfo[GET_BLOCK(x,y)].refresh_overlay_texture(p_map->p_block[GET_BLOCK(x,y)].i_overlay_tex_id);


	   }
  

	return;
}


/******************************************************************************
	Name:	Render
	Desc:	Renders the CTerrain.
	History:
 *****************************************************************************/
void CTerrain::dx_draw()
{
	
	if (m_iVertexCount <= 6) return; //no need to draw a map this small, let's assume it's a blank map

	// Reset the world transformation matrix
	this->set_world_transform();
   	dx_draw_prepare();
	if (!m_pVBCTerrain)
	{
		return; //something wrong
	}
	
	static float fBias = -0.5;
	cls_tglo.p_d3dDevice->SetTextureStageState(0, D3DTSS_MIPMAPLODBIAS, *((LPDWORD) (&fBias)));
	cls_tglo.p_d3dDevice->SetTextureStageState(1, D3DTSS_MIPMAPLODBIAS, *((LPDWORD) (&fBias)));
								   
	cls_tglo.p_d3dDevice->SetVertexShader (D3DFVF_VERTEX3D);
	cls_tglo.p_d3dDevice->SetStreamSource (0, m_pVBCTerrain, sizeof(VERTEX3D));
  //cls_tglo.p_d3dDevice->SetRenderState( D3DRS_CULLMODE,     D3DCULL_CCW );

	//set the tiles texture, weather we need to change it or not
//	cls_tglo.p_d3dDevice->SetTexture( 0, p_binfo[int(i*0.5)].tex);
 /*
	triple stage design
  
	Stage     texture  colorop                alpha op
	------------------------------------ -----------------
		0     grass    mod with diffuse		 sel1/diffuse (to use in next texture stage)
		1     flower   mod with diffuse , then replace alphaed areas with tex 0
		2     flower   mod wi3th diffuse , then replace alphaed areas with tex 0
*/	

 	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
 	cls_tglo.p_d3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
	cls_tglo.p_d3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
   cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,    D3DTOP_SELECTARG1 );
   
   cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,   D3DTA_TEXTURE );
   cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2,   D3DTA_DIFFUSE );
 	cls_tglo.p_d3dDevice->SetTextureStageState(1, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
	cls_tglo.p_d3dDevice->SetTextureStageState(1, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
   
    cls_tglo.p_d3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);

	cls_tglo.p_d3dDevice->SetTextureStageState(1,D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
	cls_tglo.p_d3dDevice->SetTextureStageState(1,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	cls_tglo.p_d3dDevice->SetTextureStageState(1,D3DTSS_COLORARG2,D3DTA_CURRENT);
  	cls_tglo.p_d3dDevice->SetTextureStageState(2,D3DTSS_COLOROP, D3DTOP_MODULATE);
	cls_tglo.p_d3dDevice->SetTextureStageState(2,D3DTSS_COLORARG1,D3DTA_CURRENT);
	cls_tglo.p_d3dDevice->SetTextureStageState(2,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
   //let's use a third texture stage
    cls_tglo.p_d3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
    cls_tglo.p_d3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
    cls_tglo.p_d3dDevice->SetTextureStageState( 1, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );


    cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,    D3DTOP_DISABLE );
    
    int i_blocks = p_map->get_size_x() * p_map->get_size_y();
    
    
    if (cls_tglo.p_thing_list->GetCullingDistance())
    {	
        
        
        D3DXVECTOR2 vec_cam (float(p_map->get_block_x(cls_tglo.p_camera->get_pos_x())), float(p_map->get_block_y(cls_tglo.p_camera->get_pos_z())));
        
        int i_padding = int(cls_tglo.p_thing_list->GetCullingDistance()/p_map->get_block_size());
        int i_start_x = int(vec_cam.x) - i_padding;
        if (i_start_x < 0) i_start_x = 0;
        
        int i_end_x = int(vec_cam.x) + i_padding;
        if (i_end_x > p_map->get_size_x()) i_end_x =  p_map->get_size_x();
        
        int i_line_to_start_on = int(vec_cam.y) - i_padding;
        if (i_line_to_start_on < 0) i_line_to_start_on = 0;
        int i_line_to_end_on = int(vec_cam.y) + i_padding;
        if (i_line_to_end_on > p_map->get_size_y()) i_line_to_end_on = p_map->get_size_y();
        
        //wow, lot of work.  optimize later.  now actually draw it
        int x;
        int i_finish;
        for (int i=i_line_to_start_on; i < i_line_to_end_on; i++)
        {
            x = i*p_map->get_size_x();
            i_finish = x + i_end_x;
            x += i_start_x;
            while (x < i_finish)
            {
                cls_tglo.p_d3dDevice->SetTexture( 0, p_binfo[x].tex);
                cls_tglo.p_d3dDevice->SetTexture( 1, p_binfo[x].p_overlay_tex);
                cls_tglo.p_d3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 
                    x*6, 2); // number of primitives
                
                x++; //advance to next block
            }
            
        }
          
    } else
    {
        //brute force
        for (int i=0; i < i_blocks; ++i)
        {
            
            //do we need to change the texture here?
            cls_tglo.p_d3dDevice->SetTexture( 0, p_binfo[i].tex);
            cls_tglo.p_d3dDevice->SetTexture( 1, p_binfo[i].p_overlay_tex);
            cls_tglo.p_d3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 
                i*6, 2); // number of primitives
        }
    }
    
    cls_tglo.p_d3dDevice->SetTextureStageState(2,D3DTSS_COLOROP, D3DTOP_DISABLE);
    cls_tglo.p_d3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
    cls_tglo.p_d3dDevice->SetTextureStageState(1, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
    cls_tglo.p_d3dDevice->SetTextureStageState(1, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
    cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,    D3DTOP_MODULATE );
    
    if (b_show_grid)
    {
        //also draw a grid over it
        
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
        cls_tglo.p_d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,  D3DTOP_DISABLE);
        this->set_pos_y(0.01f); //draw the grid slightly higher so we can see it better
        this->set_world_transform();
        
        cls_tglo.p_d3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 
            0, i_blocks*2); // number of primitives
        
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID  );
        cls_tglo.p_d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,  D3DTOP_MODULATE);
        this->set_pos_y(0.0f);
    }
    
    /*
    cls_tglo.p_d3dDevice->SetVertexShader (D3DFVF_VERTEX3D);
    cls_tglo.p_d3dDevice->SetStreamSource (0, m_pVBCTerrain, sizeof(VERTEX3D));
    cls_tglo.p_d3dDevice->SetIndices( m_pIBCTerrain, 0 );
    
      cls_tglo.p_d3dDevice->SetTexture( 0, m_pGroundTexture );
      
        cls_tglo.p_d3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
        0, 
        m_iVertexCount,  // number of vertices
        0, 
        m_iTriangleCount); // number of primitives
    */
    // Turn off culling, so we see the front and back of the Terrain.
    // cls_tglo.p_d3dDevice->SetRenderState( D3DRS_CULLMODE,     D3DCULL_NONE );
    cls_tglo.p_d3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
    cls_tglo.p_d3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
   	
    cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,    D3DTOP_MODULATE );
    //   cls_tglo.p_d3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,    D3DTOP_MODULATE  );
    
    
    CVisual::dx_draw();
}

/******************************************************************************
Name:	Restore
Desc:	Just sends a signal to build the buffers to draw the CTerrain and
grid.
Author:	Neil Haran
Date:	August 20, 2001
History:
 *****************************************************************************/
void CTerrain::dx_restore() 
{ 
	if (!cls_tglo.p_d3dDevice) return; //d3d not initted yet, can't restore
	dx_create();
	CVisual::dx_restore();
}

/******************************************************************************
	Name:	Invalidate
	Desc:	Releases the devices of this class.
	History:
 *****************************************************************************/

void CTerrain::dx_invalidate()
{
 	SAFE_RELEASE(m_pVBCTerrain); 
	SAFE_RELEASE(m_pIBCTerrain); 
	CVisual::dx_invalidate();
}

void CTerrain::dx_kill() 
{ 
 	dx_invalidate();
	CVisual::dx_kill();
}
