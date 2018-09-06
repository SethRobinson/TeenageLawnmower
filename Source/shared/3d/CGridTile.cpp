/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CGridTile.h"
#include "CTerrain.h"
#include "d3dapp.h"
#include <DXErr8.h>

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CGridTile::CGridTile()
{
	for (int i=0; i < ::C_TILE_GRID_VBS; i++)
	{
		p_vertex_buffer[i] = NULL;
	}
  p_map = NULL;
  i_vertexes = 0;
  p_vertex = NULL;
 this->set_no_lighting(true);
  
 set_sort(-3);
 this->set_pos_y(0.01f); //make it a little higher
// this->translateMovement();
 //this->set_fill_mode(D3DFILL_WIREFRAME);
 this->set_type(::C_THING_TILE);
 cls_tglo.p_grid_tile = this; //remember our location in memory
 this->m_i_zone_highlight = -1; //that means no highlight
 set_thing_name("TILEGRID");
}




CGridTile::~CGridTile()
{
 
	if (cls_tglo.p_grid_tile == this) cls_tglo.p_grid_tile = NULL;
	dx_invalidate();
	delete_local_vertex_info();
	

}

void CGridTile::delete_local_vertex_info()
{
  SAFE_DELETE_ARRAY(p_vertex);

}

void CGridTile::SetZoneHighlight(int i_zone)
{

	//this might be called from the editor, if so, que it up to do
    //later

    
    if (m_i_zone_highlight != i_zone)
	{
	
      m_b_update_zone_highlights = true;
 	
  	  m_i_zone_highlight = i_zone; //cause one group of blocks to look different
	}
}


void CGridTile::dx_create()
{
  //create the grid's vertex buffer
  	if (!cls_tglo.p_map)
	{
	  log_error("CGridTile failed, no map exists yet.");
	  return;
	}
	if (p_vertex_buffer[0])
	{
	  log_error("Vertex buffer shouldn't be used here");
	}

	p_map = cls_tglo.p_map; //remember the map, makes it easier

	i_vertexes = (p_map->i_tile_x)*(p_map->i_tile_y)*6; //using strips is SLOWER, gonna do it this way
 	delete_local_vertex_info();
	create_vertex_information();
	create_vertex_buffer();
}

float CGridTile::get_tile_height_at_point(float f_x, float f_z)
{
  

	return cls_tglo.p_terrain->get_height(f_x, f_z);
}

void CGridTile::create_vertex_info_for_tile(int i_x, int i_z)
{
  //figure out where the first vertex is in the giant array
  int p = (i_x*6)+((i_z*(p_map->i_tile_x))*6); //(this is the real position)

  float f_spacer = p_map->get_block_size() / float(p_map->i_tiles_per_block);
  float f_pos_x = float(i_x)*f_spacer;
  float f_pos_z = float(i_z)*f_spacer;


  p_vertex[p].vec_pos.x = f_pos_x;
  p_vertex[p].vec_pos.z = f_pos_z;
  p_vertex[p].vec_pos.y = get_tile_height_at_point(p_vertex[p].vec_pos.x, p_vertex[p].vec_pos.z);//height
  //p_vertex[p].dw_color = D3DCOLOR_XRGB( 255,255, 255 ); //white

  p += 1;
  p_vertex[p].vec_pos.x = f_pos_x;
  p_vertex[p].vec_pos.z = f_pos_z+f_spacer;
  p_vertex[p].vec_pos.y = get_tile_height_at_point(p_vertex[p].vec_pos.x, p_vertex[p].vec_pos.z);//height
//  p_vertex[p].dw_color = D3DCOLOR_XRGB( 255,255, 255 ); //white

  p += 1;
  p_vertex[p].vec_pos.x = f_pos_x+f_spacer;
  p_vertex[p].vec_pos.z = f_pos_z+f_spacer;
  p_vertex[p].vec_pos.y = get_tile_height_at_point(p_vertex[p].vec_pos.x, p_vertex[p].vec_pos.z);//height
  //p_vertex[p].dw_color = D3DCOLOR_XRGB( 255,255, 255 ); //white


  //second tri

  p += 1;
  p_vertex[p].vec_pos = p_vertex[p-3].vec_pos;
 // p_vertex[p].dw_color = p_vertex[p-3].dw_color;

  p += 1;
  p_vertex[p].vec_pos = p_vertex[p-2].vec_pos;
 // p_vertex[p].dw_color = p_vertex[p-2].dw_color;

  p += 1;
  p_vertex[p].vec_pos.x = f_pos_x+f_spacer;
  p_vertex[p].vec_pos.z = f_pos_z;
  p_vertex[p].vec_pos.y = get_tile_height_at_point(p_vertex[p].vec_pos.x, p_vertex[p].vec_pos.z);//height
//  p_vertex[p].dw_color = D3DCOLOR_XRGB( 255,255, 255 ); //white


  p -= 5;
  
  static DWORD dw_temp;

  int i_r = 0;
  int i_g = 0;
  int i_b = 0;
  int i_a = 0; //alpha

  CMapTile *p_temp = cls_tglo.p_map->get_tile_pointer(i_x, i_z);



  if (p_temp->bits_texture.get_bit(::C_MT_BLOCKED))
  {
    i_a = 60;
	i_r = 255;
  }
  

  if (p_temp->i_sub_zone != -1)
  {
    i_a = 60;
	i_b = 255;
  }
 if (p_temp->i_sub_zone == m_i_zone_highlight)
 {
	 //make this block really stand out, we were told to highlight this kind
	 i_a = 80;
	 i_g = 70;

 }
  
  
    dw_temp = D3DCOLOR_RGBA( i_r,i_g, i_b,i_a ); //red


  for (int c=0; c < 6; c++)
  {
   p_vertex[p+c].dw_color = dw_temp;
 
  }


}

void CGridTile::create_vertex_information()
{
   //init our local vertex info holder
   	p_vertex = new CGridVertex[i_vertexes];

	//clear them
	ZeroMemory(p_vertex,i_vertexes*sizeof(CGridVertex));

 	//start top left, which is max Z and min X.  Go right on row X row by row

	for (int i_z = 0; i_z < (p_map->i_tile_y); i_z++)
	{
	  for (int i_x = 0; i_x < (p_map->i_tile_x); i_x++)
	  {
        create_vertex_info_for_tile(i_x, i_z);
	  }
	}

}


void CGridTile::UpdateAllTiles()
{

   	for (int i_z = 0; i_z < (p_map->i_tile_y); i_z++)
	{
	  for (int i_x = 0; i_x < (p_map->i_tile_x); i_x++)
	  {
        create_vertex_info_for_tile(i_x, i_z);
	    update_tile_in_vertex_buffer(i_x, i_z);

	  }
	}

}

void CGridTile::update_tile(int x, int y)
{
  	//first lock the vertex buffer
   if (this == NULL) return; //we weren't initted yet
   this->create_vertex_info_for_tile(x, y); //update our local copy

   //copy it to the real vertex buffer that is probably on the video card
   update_tile_in_vertex_buffer(x, y);

}

void CGridTile::update_block_of_tiles(int i_x, int i_y)
{
 
   int i_tiles_per_block = cls_tglo.p_map->i_tiles_per_block;	
  i_x *= i_tiles_per_block;
  i_y *= i_tiles_per_block;

  for (int x = 0; x < i_tiles_per_block; x++)
  {
    for (int y = 0; y < i_tiles_per_block; y++)
	{
		update_tile(i_x+x, i_y+y);
	}
  }

  
}


void CGridTile::update_tile_in_vertex_buffer(int x, int y)
{
    
	int i_vert_in_buf = (x*6)+(y*p_map->i_tile_x*6); //(this is the real position)
    int i_vert_in_local_buf = i_vert_in_buf;

	//figure out which vertex buffer it's in
	int i_buf = i_vert_in_buf/i_max_verts_per_buf;
	
	i_vert_in_buf -= (i_buf*i_max_verts_per_buf);


	HRESULT hr;
		// Lock the Vertex Buffer
		CGridVertex *pVertices = NULL;
		if (FAILED(hr = p_vertex_buffer[i_buf]->Lock(0, 0, (BYTE**)&pVertices, 0)))
		{
		  log_error("Unable to lock p_vertex_buffer[i_buf] in CGridTile.cpp"); 
  	      log_error("CGridTile (%s)", DXGetErrorString8(hr));
		  return;
		}

	   	memcpy(&pVertices[i_vert_in_buf], &p_vertex[i_vert_in_local_buf], sizeof(CGridVertex)*6);
	
	    p_vertex_buffer[i_buf]->Unlock();

}


void CGridTile::create_vertex_buffer()
{
	HRESULT hr;
	// Create the vertex buffer, and then copy the vertices into the vertex buffer.
	
  	 i_max_verts_per_buf = cls_tglo.p_d3dapp->m_d3dCaps.MaxPrimitiveCount;


	 if (i_max_verts_per_buf > 63600) i_max_verts_per_buf = 63600; //divisable by 6
	 
	 //compute size needed
	

 	 int i_verts_left = this->i_vertexes; //how many total we have to draw
	 int i = 0;
	 i_vbs_used = 0;
	 int i_local_vb_offset = 0;
	 while (i_verts_left > 0)
	 {
		 this->i_vbs_used++;
		 
		 int i_vb_size = i_verts_left;
		 if (i_vb_size > this->i_max_verts_per_buf) i_vb_size = i_max_verts_per_buf;

		 i_verts_left -= i_vb_size;
		 
		 
		 if( FAILED(hr =  cls_tglo.p_d3dDevice->CreateVertexBuffer (	i_vb_size*sizeof(CGridVertex), D3DUSAGE_WRITEONLY,
			 D3DFVF_TILE_GRID, 
			 D3DPOOL_DEFAULT, 
			 &p_vertex_buffer[i] ) ) )
		 {
			 log_error( "CreateVertexBuffer error in CGridTile", hr );
			 return;
		 }
		 
		 // Lock the Vertex Buffer
		 VOID* pVertices = NULL;
		 if (FAILED(hr = p_vertex_buffer[i]->Lock(0, 0, (BYTE**)&pVertices, 0)))
		 {
			 log_error("Unable to lock p_vertex_buffer in CGridTile.cpp"); 
		    log_error("CGridTile (%s)", DXGetErrorString8(hr));
		 return;
		 }
		 
		 //let's copy the vertex data in
		 
		 memcpy( pVertices, p_vertex+i_local_vb_offset, i_vb_size*(sizeof(CGridVertex)));
		 //put vertex information in
		 
		 p_vertex_buffer[i]->Unlock();
		 i_local_vb_offset += i_vb_size;
		 i++;
     }	
	
//	log_msg("A total of %d VB's were used for tile grid.", i_vbs_used);
}


void CGridTile::dx_invalidate()
{
  //kill the grids vertex buffer
	for (int i=0; i < ::C_TILE_GRID_VBS; i++)
	{
		SAFE_RELEASE(p_vertex_buffer[i]);
	}
}

void CGridTile::dx_restore()
{
   //whatever
 dx_invalidate();
 create_vertex_buffer();

} 

void CGridTile::think()
{

    if (m_b_update_zone_highlights)
    {
        m_b_update_zone_highlights = false;
        UpdateAllTiles();
    }
}

void CGridTile::dx_draw()
{
  //draw the tile grid overlay
 	this->set_world_transform();
   	dx_draw_prepare();
 
	//render the first part
  cls_tglo.p_d3dDevice->SetTexture( 0, NULL);
  cls_tglo.p_d3dDevice->SetVertexShader (D3DFVF_TILE_GRID);
 

  //draw everything we need to
  int i_verts_to_draw = 0;
  int i_verts_left = this->i_vertexes; //how many total we have to draw
	
 //cls_tglo.p_d3dDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
 //cls_tglo.p_d3dDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE); //this will kill transparency
 cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
 
 cls_tglo.p_d3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
 cls_tglo.p_d3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

  for (int i= 0; i < this->i_vbs_used; i++)
  {
	 int i_vb_size = i_verts_left;
     if (i_vb_size > this->i_max_verts_per_buf) i_vb_size = i_max_verts_per_buf;

	 i_verts_left -= i_vb_size;

    cls_tglo.p_d3dDevice->SetStreamSource (0, p_vertex_buffer[i], sizeof(CGridVertex));
  	cls_tglo.p_d3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 
	0, i_vb_size/3); // number of primitives

  }
  cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
  

  CVisual::dx_draw();
	

}

void CGridTile::dx_kill()
{
  //completely killing the dx device
  

}