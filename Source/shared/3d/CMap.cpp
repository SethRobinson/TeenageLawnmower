/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CMap.h"
#include "CTerrain.h"
#include "CCamera.h"
#include "CSubManager.h"
#include "CBuildingManager.h"
#include "CBuilding.h"
#include "CTileExtra.h"
#include "..\\CFRect.h"

#define GET_BLOCK(x, y) x+(y*i_size_x)

//just some random seeds for my CTerrain generation.
//generates very small random values. (we're talking less than 1)
#define RANDMASK RAND_MAX
#define MRANDOM (float)(rand() & (RANDMASK))/(RANDMASK)
#define SRANDOM ((MRANDOM) * 2) - 1


CMapZone::CMapZone()
{
  ZeroMemory(this, sizeof(CMapZone));
  //set some defaults
  strcpy(st_name, "A cool level?");
  strcpy(st_author, "Seth A. Robinson");
  strcpy(st_email, "seth@rtsoft.com");
  strcpy(st_description, "A new level.");
  
  init_gravity();
} 

CMapZone::~CMapZone()
{
  kill_map(); //make sure everything is released

}


void CMapZone::init_gravity()
{
    f_gravity = C_F_GRAVITY;
    if (f_gravity_override != 0)
    {
        f_gravity = f_gravity_override;
    }
    
}

void CMapZone::set_block_size(float f_new)
{

    f_block_size = f_new;
}  

void CMapZone::SetTilesPerBlock(int i_new)
{

    i_tiles_per_block = i_new;
}

void CMapZone::SetRectWithTile(CFRect * p_rect, float f_padding, int i_x, int i_y)
{
    float f_tile_world_size = (f_block_size/ float(i_tiles_per_block));
    //f_padding *= 0;
      //f_padding = 0;

    p_rect->top = (i_y*f_tile_world_size)-f_padding;
    p_rect->left = (i_x*f_tile_world_size)-f_padding;

    p_rect->right = ((i_x*f_tile_world_size)+ f_tile_world_size) + f_padding;
    p_rect->bottom = ((i_y*f_tile_world_size)+ f_tile_world_size) + f_padding;

}

void CheckRectForCollision(CFRect &rect,  float f_x, float f_y, float *p_f_best_dist, int *p_i_best_result)
{

    //first, see if the point is inside the rect.
    static D3DXVECTOR2 l_a, l_b; //line start and end
    static D3DXVECTOR2 c; //point to measure from

    static float f_dot; 
    static float f_line_length;
    static float f_ratio;
    static D3DXVECTOR2 p; //point on the line we are closest to
    static float f_final_dist;

    c = D3DXVECTOR2(f_x,f_y);

    
    if (rect.PointIsInside(f_x, f_y))
   {
       //check each line closeness to point

        
        //top line
        l_a  = D3DXVECTOR2(rect.left, rect.top);
        l_b  = D3DXVECTOR2(rect.right, rect.top);

        //get dot product
        f_dot = ((c.x-l_a.x) * (l_b.x-l_a.x)) + ((c.y - l_a.y) * (l_b.y - l_a.y));
        f_line_length = D3DXVec2Length(&D3DXVECTOR2(l_a-l_b));
        f_ratio = f_dot/(f_line_length*f_line_length);


        //now we know how far on the line is the closest point.
      
        p.x = l_a.x + (f_ratio * (l_b.x-l_a.x));
        p.y = l_a.y + (f_ratio * (l_b.y-l_a.y));
        
        float f_final_dist = D3DXVec2Length(&D3DXVECTOR2(p - c));
        
        if (f_final_dist < *p_f_best_dist)
        {
            *p_f_best_dist = f_final_dist;
            *p_i_best_result = C_COLLISION_Y_POS;
        
        }
          //bottom line
        l_a  = D3DXVECTOR2(rect.left, rect.bottom);
        l_b  = D3DXVECTOR2(rect.right, rect.bottom);

        //get dot product
        f_dot = ((c.x-l_a.x) * (l_b.x-l_a.x)) + ((c.y - l_a.y) * (l_b.y - l_a.y));
        f_line_length = D3DXVec2Length(&D3DXVECTOR2(l_a-l_b));
         f_ratio = f_dot/(f_line_length*f_line_length);

    
        //now we know how far on the line is the closest point.
      
        p.x = l_a.x + (f_ratio * (l_b.x-l_a.x));
        p.y = l_a.y + (f_ratio * (l_b.y-l_a.y));
     
        f_final_dist = D3DXVec2Length(&D3DXVECTOR2(p - c));
        
        if (f_final_dist < *p_f_best_dist)
        {
            *p_f_best_dist = f_final_dist;
            *p_i_best_result = C_COLLISION_Y_NEG;
        
        }

                  //left
        l_a  = D3DXVECTOR2(rect.left, rect.top);
        l_b  = D3DXVECTOR2(rect.left, rect.bottom);

        //get dot product
        f_dot = ((c.x-l_a.x) * (l_b.x-l_a.x)) + ((c.y - l_a.y) * (l_b.y - l_a.y));
        f_line_length = D3DXVec2Length(&D3DXVECTOR2(l_a-l_b));
        f_ratio = f_dot/(f_line_length*f_line_length);

    
        //now we know how far on the line is the closest point.
     
        p.x = l_a.x + (f_ratio * (l_b.x-l_a.x));
        p.y = l_a.y + (f_ratio * (l_b.y-l_a.y));
        
        f_final_dist = D3DXVec2Length(&D3DXVECTOR2(p - c));
        
        if (f_final_dist < *p_f_best_dist)
        {
            *p_f_best_dist = f_final_dist;
            *p_i_best_result = C_COLLISION_X_POS;
        
        }
                 //right
        l_a  = D3DXVECTOR2(rect.right, rect.top);
        l_b  = D3DXVECTOR2(rect.right, rect.bottom);

        //get dot product
         f_dot = ((c.x-l_a.x) * (l_b.x-l_a.x)) + ((c.y - l_a.y) * (l_b.y - l_a.y));
         f_line_length = D3DXVec2Length(&D3DXVECTOR2(l_a-l_b));
         f_ratio = f_dot/(f_line_length*f_line_length);

    
        //now we know how far on the line is the closest point.
       
        p.x = l_a.x + (f_ratio * (l_b.x-l_a.x));
        p.y = l_a.y + (f_ratio * (l_b.y-l_a.y));
        
        f_final_dist = D3DXVec2Length(&D3DXVECTOR2(p - c));
        
        if (f_final_dist < *p_f_best_dist)
        {
            *p_f_best_dist = f_final_dist;
            *p_i_best_result = C_COLLISION_X_NEG;
        
        }


   }   else
   {
       //failed test
       return;
   }

}


int CMapZone::SphereToTileCheck(D3DXVECTOR3 *p_vec_pos, float f_rad)
{

   	int i_x = this->get_tile_x(p_vec_pos->x);
  	int i_y = this->get_tile_y(p_vec_pos->z);


  //are we standing on a hard tile right now?  if so, fail the test in any case.
  if (get_tile_pointer(i_x, i_y)->bits_texture.get_bit(::C_MT_BLOCKED)) return C_COLLISION_BOTH;
  float f_tile_world_size = (f_block_size/ float(i_tiles_per_block));

  CFRect rect_tile;

  float f_best_dist = 99999;
  int i_best_result = 0; //none so far
 
  //check the nine surrounding tiles


 
  if (get_tile_pointer(i_x, i_y-1)->bits_texture.get_bit(::C_MT_BLOCKED)) 
  {
      SetRectWithTile(&rect_tile, f_rad, i_x, i_y-1);
      CheckRectForCollision(rect_tile, p_vec_pos->x, p_vec_pos->z, &f_best_dist, &i_best_result);
  }

  
   if (get_tile_pointer(i_x, i_y+1)->bits_texture.get_bit(::C_MT_BLOCKED)) 
  {
      SetRectWithTile(&rect_tile, f_rad, i_x, i_y+1);
      CheckRectForCollision(rect_tile, p_vec_pos->x, p_vec_pos->z, &f_best_dist, &i_best_result);
  }

   if (get_tile_pointer(i_x+1, i_y)->bits_texture.get_bit(::C_MT_BLOCKED)) 
  {
      SetRectWithTile(&rect_tile, f_rad, i_x+1, i_y);
      CheckRectForCollision(rect_tile, p_vec_pos->x, p_vec_pos->z, &f_best_dist, &i_best_result);
  }


   if (get_tile_pointer(i_x-1, i_y)->bits_texture.get_bit(::C_MT_BLOCKED)) 
  {
      SetRectWithTile(&rect_tile, f_rad, i_x-1, i_y);
      CheckRectForCollision(rect_tile, p_vec_pos->x, p_vec_pos->z, &f_best_dist, &i_best_result);
  }


   //diags

   if (get_tile_pointer(i_x-1, i_y-1)->bits_texture.get_bit(::C_MT_BLOCKED)) 
  {
      SetRectWithTile(&rect_tile, f_rad, i_x-1, i_y-1);
      CheckRectForCollision(rect_tile, p_vec_pos->x, p_vec_pos->z, &f_best_dist, &i_best_result);
  }


   if (get_tile_pointer(i_x+1, i_y-1)->bits_texture.get_bit(::C_MT_BLOCKED)) 
  {
      SetRectWithTile(&rect_tile, f_rad, i_x+1, i_y-1);
      CheckRectForCollision(rect_tile, p_vec_pos->x, p_vec_pos->z, &f_best_dist, &i_best_result);
  }


      if (get_tile_pointer(i_x+1, i_y+1)->bits_texture.get_bit(::C_MT_BLOCKED)) 
  {
      SetRectWithTile(&rect_tile, f_rad, i_x+1, i_y+1);
      CheckRectForCollision(rect_tile, p_vec_pos->x, p_vec_pos->z, &f_best_dist, &i_best_result);
  }

      if (get_tile_pointer(i_x-1, i_y+1)->bits_texture.get_bit(::C_MT_BLOCKED)) 
  {
      SetRectWithTile(&rect_tile, f_rad, i_x-1, i_y+1);
      CheckRectForCollision(rect_tile, p_vec_pos->x, p_vec_pos->z, &f_best_dist, &i_best_result);
  }
 /*
 
     if (i_best_result != 0)
   {
       char st_crap[255];
       if (i_best_result == C_COLLISION_X)
       {
       
       sprintf(st_crap, "Closest line is X with %.3f", f_best_dist );
       } else
       {
                  sprintf(st_crap, "Closest line is Y with %.3f", f_best_dist );

       }
       log_msg(st_crap);
   }
    */
  
   return i_best_result; //no collision
}



bool CMapZone::tile_is_hard_world(float f_x, float f_y)
{
  	int i_x = this->get_tile_x(f_x);
  	int i_y = this->get_tile_y(f_y);

	if (!is_valid_tile(i_x, i_y)) return false;
	return (this->get_tile_pointer(i_x, i_y)->bits_texture.get_bit(::C_MT_BLOCKED));
}

void CMapZone::send_map_to_terrain()
{
	if (cls_tglo.p_terrain)
	{
	
	//this will create the real map terrain from our data
	cls_tglo.p_terrain->set_map_source(this);
	cls_tglo.p_terrain->dx_restore();
	}
}


bool CMapZone::is_valid_block(int x, int y)
{
 if ( (x < 0) || (y < 0) ) return false;
 if (x >= this->i_size_x) return false;
 if (y >= this->i_size_y) return false;

 //block is valid if we get here
 return true;
}


bool CMapZone::is_valid_tile(int x, int y)
{
 if ( (x < 0) || (y < 0) ) return false;
 if (x >= this->i_tile_x) return false;
 if (y >= this->i_tile_y) return false;

 //tile is valid if we get here
 return true;
}

int CMapZone::get_tile_x(float f_x)
{
  float f_div = this->f_block_size/i_tiles_per_block;

  return int((f_x / f_div));
}

int CMapZone::get_tile_y(float f_y)
{
  float f_div = this->f_block_size/i_tiles_per_block;

  return int((f_y / f_div));
}

CMapTile * CMapZone::get_tile_pointer(int i_x, int i_y)
{
  return &p_tile[ i_x+(i_y*i_tile_x)];
}
 CTileExtra * CMapZone::get_tile_extra_pointer(int i_x, int i_y)
{
  return &p_tile_extra[ i_x+(i_y*i_tile_x)];
}



float CMapZone::GetWorldHeightIncludingWalls(float f_x, float f_y, float f_height_of_blocked_tiles)
{

	int i_x = this->get_tile_x(f_x);
  	int i_y = this->get_tile_y(f_y);

	if (!is_valid_tile(i_x, i_y)) return 0;

  CMapTile *p_tile = get_tile_pointer(i_x, i_y);
  
  if (!p_tile->bits_texture.get_bit(::C_MT_WALL))
   {
	  if (p_tile->bits_texture.get_bit(C_MT_BLOCKED))
      {
          //blocked, nothing can pass, ever
         return f_height_of_blocked_tiles;  
      }
      
      //no wall.  just return the regular height.
	   return cls_tglo.p_terrain->get_height(f_x, f_y);
   } 
   
   //if we got here there was a wall.  Let's add it's height to the world height and return that.

  CSubZone *p_sub_zone = &p_sub_manager->p_a_sub_zone[p_tile->i_sub_zone];

  if ( (p_sub_zone->i_ceil_type == C_SUBZONE_CEIL_FIXED)
      || (p_sub_zone->i_ceil_type == C_SUBZONE_CEIL_NONE)
      )
  {
    return (p_sub_zone->f_wall_height);
  }

   if (p_sub_zone->i_ceil_type == C_SUBZONE_CEIL_RELATIVE)
   {
    return (cls_tglo.p_terrain->get_height(f_x, f_y) + 	p_sub_zone->f_wall_height);
   }
  //default, walls are off
  return cls_tglo.p_terrain->get_height(f_x, f_y);
 }

 float CMapZone::GetCeilingHeight(float f_x, float f_y)
{
    static const float C_DEFAULT_HEIGHT = 10000.0f;
	int i_x = this->get_tile_x(f_x);
  	int i_y = this->get_tile_y(f_y);

	if (!is_valid_tile(i_x, i_y)) return C_DEFAULT_HEIGHT;

  CMapTile *p_tile = get_tile_pointer(i_x, i_y);
  
  if (p_tile->i_sub_zone == -1)
   {
	   //no wall.  just return the regular height.
	   return C_DEFAULT_HEIGHT;
   } 
   
   //we are on a sub zone

  CSubZone *p_sub_zone = &p_sub_manager->p_a_sub_zone[p_tile->i_sub_zone];
  if (p_sub_zone->i_cealing_draw != C_SUBZONE_ROOF_NORMAL) return C_DEFAULT_HEIGHT;
  
  if (p_sub_zone->i_ceil_type == C_SUBZONE_CEIL_FIXED)
  {
    return (p_sub_zone->f_wall_height);
  }

   if (p_sub_zone->i_ceil_type == C_SUBZONE_CEIL_RELATIVE)
   {
    return (cls_tglo.p_terrain->get_height(f_x, f_y) + 	p_sub_zone->f_wall_height);
   }

   //default, walls are off

   return C_DEFAULT_HEIGHT;

 }

/*

 Convert world cordinate to the a map block - return the FIRST block
 of texture we're on.
  */
int CMapZone::get_base_tile_block_x(float f_x)
{
 int i_blocks_per_tex = cls_tglo.p_terrain->get_blocks_per_texture();

 int i_x = this->get_block_x(f_x); //get what it would be normally

 //now figure out where this texture started
 i_x = ( (i_x/i_blocks_per_tex)*i_blocks_per_tex);
 return i_x;
}

int CMapZone::get_base_tile_block_y(float f_y)
{
 int i_blocks_per_tex = cls_tglo.p_terrain->get_blocks_per_texture();

 int i_y = this->get_block_y(f_y); //get what it would be normally

 //now figure out where this teyture started
 i_y = ( (i_y/i_blocks_per_tex)*i_blocks_per_tex);
 return i_y;
}


bool CMapZone::generate_map(int i_new_x, int i_new_y, float f_new_block_size, float f_new_tiles_per_block)
{
  kill_map(); //free map memory
	
  i_size_x = i_new_x;
  i_size_y = i_new_y;
  f_block_size = f_new_block_size;
  this->i_tiles_per_block = int(f_new_tiles_per_block);

  //The number of vertices is simply the area of the grid.
  i_vertex_count =  (i_size_x+1) * (i_size_y+1);

    //allocate memory for the block info structure
    p_block = new CMapBlock[i_size_x*i_size_y];
	//clear it
	ZeroMemory(p_block, i_size_x*i_size_y*sizeof(CMapBlock));
	setup_tile_memory();

	//fill part of it
   /*
	for (int o=0; o < 5; o++)
	{
		for (int k=0; k < i_tile_y; k++)
		{
		p_tile[o+(k*i_tile_x)].bits_texture.set_bit(::C_MT_BLOCKED,1);
		}

	}
	 
	 */

  	//Allocate memory for the height buffer.
	p_height_buffer = new FLOAT[i_vertex_count];
	
	for (int x=0; x < i_size_x; x++)
		for (int y=0; y <  i_size_y; y++)
		{
		  this->p_block[GET_BLOCK(x,y)].i_tex_id = 1; //random(2)+1;
		}

	//set all the initial heights to zero. because we're modifying a flat plane.
	memset(p_height_buffer, 0, i_vertex_count*sizeof(FLOAT));

	if (p_sub_manager)
	{
	  log_error("ERROR: sub manager shouldn't be initted yet");
	} 
	
	//create sub zone system, init with zero sub zones
	p_sub_manager = new CSubManager();

   log_msg("Map generated.");
  //created new struct ok
  return true;
}

void CMapZone::setup_tile_memory()
{
    if (i_tiles_per_block == 0) i_tiles_per_block = 3; //for upgrading old maps
	//allocate memory for the tile info structure
	i_tile_x = i_size_x*i_tiles_per_block;
	i_tile_y = i_size_y*i_tiles_per_block;

	i_tile_num = i_tile_x*i_tile_y;


	p_tile_extra = new CTileExtra[i_tile_num];
	ZeroMemory(p_tile_extra, i_tile_num*sizeof(CTileExtra));



	p_tile = new CMapTile[i_tile_num]; //save map tile data
	//clear it
	ZeroMemory(p_tile, i_tile_num*sizeof(CMapTile));

	//set defaults

	  for (int ty=0; ty < i_tile_y; ty++)
	  {
		for (int tx=0; tx < i_tile_x; tx++)
		{
          p_tile[tx +(ty*i_tile_x)].i_sub_zone = -1;

		  p_tile_extra[tx +(ty*i_tile_x)].set_defaults();
		  p_tile_extra[tx +(ty*i_tile_x)].i_x = tx;
		  p_tile_extra[tx +(ty*i_tile_x)].i_y = ty;

	  }

	}


}


//this controls what tiles should have "extra info" saved about them.  
//our base tile is always saved, but some tiles need extra stuff but we
//dont' want to save ALL of them or our data file will be 20 megs..

bool CMapZone::tile_extra_should_be_saved(int i_x, int i_y)
{
  if (this->p_tile[i_x+(i_y*i_tile_x)].i_sub_zone != -1)
  {
	
	  //it's a sub zone, save it!
	  return true;

	  /*
	  if (p_tile[i_x+(i_y*i_tile_x)].bits_texture.get_bit(::C_MT_WALL))
	  {
	   return true;
	  }
	  */

  }
return false;
}

void CMapZone::kill_map()
{
  
  SAFE_DELETE_ARRAY(p_height_buffer);
  SAFE_DELETE_ARRAY(p_block);
  SAFE_DELETE_ARRAY(p_tile);
  SAFE_DELETE_ARRAY(p_tile_extra);
  SAFE_DELETE(p_sub_manager);
}


void CMapZone::create_random_height(FLOAT CTerrainHeight, FLOAT heightScale, 
							   FLOAT frequency, FLOAT freqScale, int numPasses)
{
  	//Depending on how many iterations, the CTerrain will look more realistic..
    for( int passCounter = 0; passCounter < numPasses; passCounter++ )
	{
		generateHeightValues( CTerrainHeight, frequency );
		frequency += freqScale;
		CTerrainHeight *= heightScale;
	}

}


/******************************************************************************
	Name:	generateHeightValues
	Desc:	It iterates a over an array with varying random values and interpolates
			them to give us neat amplitude and wavelengths.

	Param1: FLOAT heightScale	- The scaling of the height of the interpolation.
	Param2: FLOAT frequency		- The frequency in the number of bumps on the CTerrain.

	Author:	Neil Haran.
	Date:	August 30, 2001
	History:
 *****************************************************************************/
void CMapZone::generateHeightValues( FLOAT heightScale, FLOAT frequency )
{
	//some constants for ease of coding.
	int NUM_VERT_LINES = i_size_x + 1;
	int NUM_HORZ_LINES = i_size_y + 1;
    


	//The max frequency, 2 extra is needed for smoothing of spline.
	//This is used for smooth spline interpolation
	int frequencyMax = int(frequency + 2);

	//pixels per spline segment
	FLOAT fDeltaX = NUM_VERT_LINES / (frequency-1), 
				fDeltaZ = NUM_HORZ_LINES / (frequency-1);

	//for immediate calculation with the spline knots.
	FLOAT *zknots = new FLOAT[frequencyMax];

	// the generated values - to be equally spread across the buffer.
	FLOAT *values = new FLOAT[frequencyMax*frequencyMax];

	// horizontal lines through knots
	FLOAT *phorzList = new FLOAT[frequencyMax * NUM_VERT_LINES];

	// The height buffer to store the generated height values.
	FLOAT *heightBuffer = new FLOAT[i_vertex_count];

	memset(heightBuffer, 0, i_vertex_count*sizeof(FLOAT));

	int rows = 0;

	// a helper counter for the indexing into our buffers.
	int nCounter = 0;

	//generate CTerrain knots.
	//they are basically random points.
	for( rows = 0; rows < frequencyMax; rows++ )
	{
		for( int columns = 0; columns < frequencyMax; columns++ )
		{
			//just adding random knot values to the array.
			values[nCounter++] = SRANDOM;
		}
	}

	nCounter = 0; //resetting the counter after its last use.

	// interpolate all horizontal lines using the knots
	int freqCounter;
	for( freqCounter = 0; freqCounter < frequencyMax; freqCounter++ )
	{
		FLOAT* knots = &values[freqCounter * frequencyMax];
		FLOAT xKnot = 0;
		for( int columns = 0; columns < NUM_VERT_LINES; columns++ )
		{
			phorzList[nCounter++] = interpolateSpline( xKnot/fDeltaX, 4, knots );
			xKnot += 1;
			//This is a little check to make sure that the height is not greater
			//than the max height for this iteration.
			if( xKnot >= fDeltaX )
			{
				xKnot -= fDeltaX;
				knots++;
			}
		}
	}

	nCounter = 0; //resetting the counter after its last use.

	// interpolate all vertical lines using the knots
	for( rows = 0; rows < NUM_VERT_LINES; rows++ )
	{
		FLOAT zKnot = 0;
		FLOAT* knots = zknots;
		
		// build the knot list
		for( freqCounter = 0; freqCounter < frequencyMax; freqCounter++ )
		{
			//getting all the knots (random points) from the current row.
			knots[freqCounter] = phorzList[freqCounter * NUM_VERT_LINES + rows];
		}
		for( int columns = 0; columns < NUM_HORZ_LINES; columns++ )
		{
			//adds the value of the interpolated knot to the height buffer.
			heightBuffer[nCounter++] = interpolateSpline( zKnot/fDeltaZ, 4, knots ) * heightScale;
			zKnot++;
			//This is a little check to make sure that the height is not greater
			//than the max height for this iteration.
			if( zKnot >= fDeltaZ )
			{
				zKnot -= fDeltaZ;
				knots++;
			}
		}
	}

	nCounter = 0; //resetting the counter from its last use.
	// update the height buffer with all the values from the local height buffer.
	//basically adds the new iteration of height values to the old ones... 
	
	for( rows = 0; rows < NUM_HORZ_LINES; rows++ )
	{
		for( int columns = 0; columns < NUM_VERT_LINES; columns++ )
		{
			p_height_buffer[nCounter] += heightBuffer[nCounter];
			nCounter++;
		}
	}

	//deletion of our buffers.
	SAFE_DELETE_ARRAY( values );
	SAFE_DELETE_ARRAY( zknots );
	SAFE_DELETE_ARRAY( phorzList );
	SAFE_DELETE_ARRAY( heightBuffer );
}

/******************************************************************************
	Name:	interpolateSpline
	Desc:	It basically interpolates four points (knots) and generates a spline 
			and returns that interpolated value.
	Author:	Neil Haran
			Math was Taken directly out of Darwyn Peachey's chapter in Texturing & 
			Modeling: A Procedural Approach. I just wrote the code.
	Date:	August 30, 2001
	History:
 *****************************************************************************/
FLOAT CMapZone::interpolateSpline( float x, int numKnots, float *knot )
{
	// basis matrix for spline interpolation
	D3DXMATRIX matSpline = D3DXMATRIX( -0.5, 1.5,-1.5, 0.5,
										1.0,-2.5, 2.0,-0.5,
									   -0.5, 0.0, 0.5, 0.0,
										0.0, 1.0, 0.0, 0.0 );

	int span = int(x);
	int numSpans = numKnots - 3;
	float cCubic[4];	// coefficients of the cubic

	if( numSpans < 1) return 0; //if its illegal value.. return out.

	// find the appropriate 4-point span of the spline
	x = (x < 0 ? 0 : (x > 1 ? 1 : x)) * numSpans;

	if( span >= numKnots - 3 ) span = numKnots - 3;

	x -= span;
	knot += span;

	// evaluate the span cubic at x using horner's rule
	cCubic[0] = matSpline._41 * knot[0] + matSpline._42 * knot[1] +
				matSpline._43 * knot[2] + matSpline._44 * knot[3];

	cCubic[1] = matSpline._31 * knot[0] + matSpline._32 * knot[1] +
				matSpline._33 * knot[2] + matSpline._34 * knot[3];

	cCubic[2] = matSpline._21 * knot[0] + matSpline._22 * knot[1] +
				matSpline._23 * knot[2] + matSpline._24 * knot[3];

	cCubic[3] = matSpline._11 * knot[0] + matSpline._12 * knot[1] +
				matSpline._13 * knot[2] + matSpline._14 * knot[3];

	//This is basically the value at the interpolation points...
	return ( ( cCubic[3] * x + cCubic[2] ) * x + cCubic[1] ) * x + cCubic[0];
}

//find the closest vertex and set it.
void CMapZone::set_vertex_height(float f_x, float f_y, float f_height)
{
   //let's figure out where the closest vertex is

	//the vertex buffer is a bunch of floats in a giant array.  
	f_x = f_x / (this->f_block_size);
	f_y = f_y / (this->f_block_size);
	f_x += 0.5;
	f_y += 0.5;
	int i_x = int(f_x);
	int i_y = int(f_y);

	this->p_height_buffer[i_x+(i_y*(i_size_x+1))] = f_height;
 
   	cls_tglo.p_terrain->update_vertex_height(i_x, i_y);

	//this->send_map_to_terrain();
}

float CMapZone::mod_vertex_height(float f_x, float f_y, float f_height)
{
   //let's figure out where the closest vertex is

	//the vertex buffer is a bunch of floats in a giant array.  
	f_x = f_x / (this->f_block_size);
	f_y = f_y / (this->f_block_size);
	f_x += 0.5;
	f_y += 0.5;
	int i_x = int(f_x);
	int i_y = int(f_y);

	this->p_height_buffer[i_x+(i_y*(i_size_x+1))] += f_height;
 
   	cls_tglo.p_terrain->update_vertex_height(i_x, i_y);
	
	return p_height_buffer[i_x+(i_y*(i_size_x+1))];
	//this->send_map_to_terrain();
}



void CMapZone::ScaleVerts(float f_scale)
{

    for (int x = 0;  x < this->get_size_x(); x++)
    {
        for (int y = 0; y < this->get_size_y(); y++)
        {
         p_height_buffer[x+(y*(get_size_x()))] = float(p_height_buffer[x+(y*(get_size_x()))]) * f_scale;
        }

    }

}


void CMapZone::save_zone(char st_fname[256])
{

//  log_msg("Saving from within CMAP.");

  //first fill in the version and size info
  this->f_version = cls_tglo.f_version;
  this->i_data_size = sizeof(CMapZone);
  this->i_map_block_data_size = sizeof(CMapBlock);
  this->i_tile_data_size = sizeof(CMapTile);
  
//  log_msg("Saving Zone Header... %d bytes.",i_data_size);
  FILE * fp;
     if ( (fp=fopen(st_fname, "wb")) == NULL)
	 {
	  log_error("Error saving %s.  Out of HD space or something?", st_fname);
	  return;
	 }

	 if (fwrite(this, sizeof(CMapZone), 1, fp) <= 0)
	 {
		log_error("Error writing map headerto %s.", st_fname);
		fclose(fp);
		return;
	 }
 	
	 
	 //however, we are not finished.  Let's now write the height buffer of the map to the file
	 //the vertex's are actually shared between blocks, this is why I don't store this in each
	 //cmapblock object. (this is the height vertex only, the x and z are computed)

	 if (fwrite(this->p_height_buffer, (i_size_x+1) * (i_size_y+1)*sizeof(float), 1, fp) <= 0)
	 {
		log_error("Error writing height buffer to %s.", st_fname);
		fclose(fp);
		return;
	 }
 
	 //now let's write each map block

	 if (fwrite(this->p_block, i_size_x*i_size_y*sizeof(CMapBlock), 1, fp) <= 0)
	 {
		log_error("Error writing map blocks to %s.", st_fname);
		fclose(fp);
		return;
	 }
	 
	 //write all our tiles

	 if (fwrite(this->p_tile, i_tile_num*sizeof(CMapTile), 1, fp) <= 0)
	 {
		log_error("Error writing tiles to %s.", st_fname);
		fclose(fp);
		return;
	 }

	 //save building defination info
	 this->p_sub_manager->save(fp); //let it save it's own stuff
//	 log_msg("Save finished.");
	fclose(fp);

}


bool CMapZone::load_zone(char * p_st_fname)
{


    char st_fname[256];
    strcpy(st_fname, p_st_fname);

    if (!exist(st_fname))
    {
        sprintf(st_fname, "%s%s", GetExeDir(), p_st_fname);
    }


//  log_msg("Reading from within CMAP.");
  FILE * fp;
  if ( (fp=fopen(st_fname, "rb")) == NULL)
	{
      log_error("Unable to open zone %s.",st_fname);
	  	this->send_map_to_terrain();  //five the terrain something

	  return false; //no error
	}

  //release all current map info we have now
  kill_map();

    //first load a small piece, this will tell us how big it is and how big the CMapBlock's are

    //load header
	CMapLoader sys_loader;

	if (fread(&sys_loader, sizeof(sys_loader), 1, fp) <= 0)
	{
	  log_error("Couldn't read %s header.  Corrupted file?  Delete it maybe.",
		  st_fname);
	  fclose(fp);
	  return false;
	}

	//clear ourself
	 ZeroMemory(this, sizeof(CMapZone));
 

 //load data over ourselves, tricky but it works


	char st_crap[255];
	sprintf(st_crap, "Loading Zone %s. (version %.3f)",st_fname, sys_loader.f_version);
	log_msg(st_crap);

	//Load real thing.
	if (fseek(fp, 0, SEEK_SET ))
	{
	 log_error("Unable to perform seek on sys.dat file.");
	}
	if (!fread(this, sys_loader.i_data_size, 1,fp))
	{
	 log_error("Error with fread loading zone file.");
	}

	//kill some old bad pointers
	this->p_sub_manager = NULL;

	//that's fine.  But now we need to load the height buffer
	//first init it, there is a bad pointer there right now.
	i_vertex_count =  (i_size_x+1) * (i_size_y+1);

    //allocate memory for the block info structure
    p_block = new CMapBlock[i_size_x*i_size_y];

	//clear it, in case new stuff has been added at least it will be set to zero
	ZeroMemory(p_block, i_size_x*i_size_y*sizeof(CMapBlock));

  	//Allocate memory for the height buffer.
	p_height_buffer = new FLOAT[i_vertex_count];

	//now let's load the height buffer
	if (!fread(this->p_height_buffer, i_vertex_count*sizeof(float), 1,fp))
	{
	 log_error("Error with fread loading height buffer in zone file.");
	}

	setup_tile_memory();

	//two ways to load, first the quick way
	if (sizeof(CMapBlock) == this->i_map_block_data_size)
	{
		//size is the same, load the fast way
		if (!fread(this->p_block, i_size_x*i_size_y*sizeof(CMapBlock), 1,fp))
		{
			log_error("Error with fread loading block info in zone file.");
		}
	} else
	{
	 	log_msg("Converting map data to new format.");
		//map data has changed, load the really slow way, block by block
	   for (int i=0; i < i_size_x*i_size_y; i++)
	   {
		fread(&p_block[i], this->i_map_block_data_size, 1,fp);
	   }
	}
	

	//load tiles


		//two ways to load, first the quick way
	if (sizeof(CMapTile) == this->i_tile_data_size)
	{
		//size is the same, load the fast way
		if (!fread(this->p_tile, i_tile_num*sizeof(CMapTile), 1,fp))
		{
			log_error("Error with fread loading tile data in zone file.");
		}
	} else
	{
	 	log_msg("Converting tile data to new format.");
		//map data has changed, load the really slow way, block by block
	   for (int i=0; i < i_tile_num; i++)
	   {
		fread(&p_tile[i], this->i_tile_data_size, 1,fp);
	   }
	}

	//handle sub zone definations

	if (p_sub_manager)
	{
	  log_error("ERROR: sub manager shouldn't be initted yet");
	} 
	
	//create sub zone system, init with zero sub zones
	p_sub_manager = new CSubManager();

	p_sub_manager->load(fp);

	fclose(fp);

	//now actually put the map to the screen jelly bean
	this->send_map_to_terrain();
	init_gravity();
  
	//load all building walls now
	for (int o = 0; o < this->p_sub_manager->get_count(); o++)
	{
	  cls_tglo.p_building_manager->get(o)->build_walls();
	  cls_tglo.p_building_manager->get(o)->dx_create();
	  
	}
	cls_tglo.SetIgnoreNextFrameSmooth(true);

	return true;
}




