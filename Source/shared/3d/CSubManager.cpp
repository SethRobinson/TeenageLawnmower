/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CSubManager.h"
#include "..\\all.h"
#include "CThing_globals.h"
#include "CBuildingManager.h"
#include "CMap.h"
#include "CMapTile.h"
#include "CTileExtra.h"

CSubManager::CSubManager()
{
    ZeroMemory(this, sizeof(CSubManager));
 
}


CSubManager::~CSubManager()
{
  		if (cls_tglo.p_building_manager)
	{
	  cls_tglo.p_building_manager->clear_all();
	}

	SAFE_FREE(p_a_sub_zone);
}


int CSubManager::GetSubZoneByName(	char *st_name)
{
   for (int i=0; i < this->i_count; i++)
   {
	  if (stricmp(st_name, this->p_a_sub_zone[i].st_name) == 0)
	  {
		  //match made
		  return i;
	  }
   }

   //no match found.
   return -1;

}

int CSubManager::add_sub_zone()
{
  	//add one
	i_count++;
	
	//create new memory or add to existing
	p_a_sub_zone =  (CSubZone*)realloc(p_a_sub_zone, sizeof(CSubZone)*i_count);

	//that's great.  Now create an entity to house the house
	if (cls_tglo.p_building_manager)
	{
	  cls_tglo.p_building_manager->add_building();
	} else
	{
	  log_error("p_building_manager doens't exist, can't create building");

	}

    return i_count-1; //the first record is at 0
}

int CSubManager::count_extra_tiles_actually_used()
{
  int i_tiles = 0;
  

  for (int ty=0; ty < cls_tglo.p_map->i_tile_y; ty++)
  {
	  for (int tx=0; tx < cls_tglo.p_map->i_tile_x; tx++)
	  {
		  
		  if (cls_tglo.p_map->tile_extra_should_be_saved(tx,ty))
		  {
		   i_tiles++;
		  }
		  
	  }
	  
  }

return i_tiles;

}



int CSubManager::count_extra_tiles_used_in_zone(int i_zone)
{
  int i_tiles = 0;
  

  for (int ty=0; ty < cls_tglo.p_map->i_tile_y; ty++)
  {
	  for (int tx=0; tx < cls_tglo.p_map->i_tile_x; tx++)
	  {
		  
		  if (cls_tglo.p_map->get_tile_pointer(tx,ty)->i_sub_zone == i_zone)
		  {
		   
              i_tiles++;
		  }
		  
	  }
	  
  }
//  log_msg("Counted %d tiles used in zone %d.", i_tiles, i_zone);

return i_tiles;

}
void CSubManager::save(FILE *fp)
{

	int i_size_of_us = sizeof(CSubManager);
	i_size_of_csub_zone = sizeof(CSubZone);

	this->i_extra_data_size = sizeof(CTileExtra);
	this->i_extra_tile_count = count_extra_tiles_actually_used();

	//if we have zones, how many are in each one?

	for (int i=0; i < i_count; i++)
	{
		this->p_a_sub_zone[i].i_zone_squares = count_extra_tiles_used_in_zone(i);
	}

	 



	
	//first write our data
	fwrite(&i_size_of_us, sizeof(int), 1, fp);

	//next write this record
	
	if (fwrite(this, sizeof(CSubManager), 1, fp) <= 0)
	 {
		log_error("Error writing sub Manager info to file");
		fclose(fp);
		return;
	 }
    if (i_count > 0)
    {
    
	if (fwrite(p_a_sub_zone, i_count*sizeof(CSubZone), 1, fp) <= 0)
	 {
		log_error("Error writing sub zones to file");
		fclose(fp);
		return;
	 }

    }



	//now save any special used tiles we needed

	//create a temp space to hold all of them
	CTileExtra *p_pe = NULL;

	p_pe = (CTileExtra*) malloc(this->i_extra_tile_count*this->i_extra_data_size);

	if (!p_pe)
	{
	  log_error("Unable to allocate memory to save tiles. ");
	  return;
	}

  
	//copy all the correct tiles into our new array
int i_tiles = 0;	
	 for (int ty=0; ty < cls_tglo.p_map->i_tile_y; ty++)
  {
	  for (int tx=0; tx < cls_tglo.p_map->i_tile_x; tx++)
	  {
		  
		  if (cls_tglo.p_map->tile_extra_should_be_saved(tx, ty))
		  {
		  	  //copy this tile, clearing any bad data

			 memcpy(&p_pe[i_tiles], &cls_tglo.p_map->p_tile_extra[tx +(ty*cls_tglo.p_map->i_tile_x)],
				 this->i_extra_data_size);
              p_pe[i_tiles].i_roof_index = -1;
             i_tiles++; 
		  }
		  
	  }
	  
  }


	 if (i_extra_tile_count > 0)
     {
     
     if (fwrite(p_pe, this->i_extra_data_size*this->i_extra_tile_count, 1, fp) <= 0)
	 {
		log_error("Error writing extra tile data to file");
		SAFE_FREE(p_pe);
		fclose(fp);
		return;
	 }
     }

	//free mem we're done with
	SAFE_FREE(p_pe);


}

void CSubManager::load(FILE *fp)
{
  
    
    //old record size let's us realize if the file size has changed so we
  //can load a smarter (but slower) way if needed
  if (this->i_count != 0) log_error("CSubManager should have 0 count now.");

  int i_our_size = 0;
  
  //first load the size we're supposed to be
  if (!fread(&i_our_size, sizeof(int), 1, fp))
  {
	  log_error("Couldn't read CSubManager size info");
	  return;
  }

  //got size, load us
  if (!fread(this, i_our_size, 1, fp))
  {
	  log_error("Couldn't read CSubManager info");
	  return;
  }

  //reset out pointers (they still have old data in them from the load)
  this->p_a_sub_zone = NULL; //not initted yet

  //init memory for sub zones
  p_a_sub_zone =  (CSubZone*)realloc(p_a_sub_zone, sizeof(CSubZone)*i_count);
  ZeroMemory(p_a_sub_zone, i_count*sizeof(CSubZone));
  //clear it so new stuff will at least default to 0

  //great, we have memory but it is blank...


  if (sizeof(CSubZone) == this->i_size_of_csub_zone)
	{
		//size is the same, load the fast way
	  if (i_count > 0)
      {
      
      if (!fread(p_a_sub_zone, i_count*sizeof(CSubZone), 1,fp))
		{
			log_error("Error with fread loading sub zone data in zone file.");
		}
      }
	} else
	{
	 	log_msg("Converting tile data to new format.");
		//map data has changed, load the really slow way, block by block
	   for (int i=0; i < i_count; i++)
	   {
		fread(&p_a_sub_zone[i], this->i_size_of_csub_zone, 1,fp);
	   }
	}


	
	//now load all our extra tile info to overlay over our extra info stuff...

	//create some temp memory to work with it
   	CTileExtra *p_pe = NULL;

	p_pe = (CTileExtra*) malloc(this->i_extra_tile_count*sizeof(CTileExtra));

	if (!p_pe)
	{
	  log_error("Unable to allocate memory to load extra tile info. ");
	  return;
	}


   if (sizeof(CTileExtra) == this->i_extra_data_size)
	{
	if ( i_extra_tile_count > 0)
    {
    
       //size is the same, load the fast way
		if (!fread(p_pe, i_extra_tile_count*i_extra_data_size, 1,fp))
		{
			log_error("Error with fread loading sub zone data in zone file.");
		}
    }

	} else
	{
	 	log_msg("Converting extra tile data to new format.");
		//map data has changed, load the really slow way, block by block
	  	memset(p_pe, 0, i_extra_tile_count * sizeof(CTileExtra)); //default should be 0 for any new
		//stuff added to the class
		for (int i=0; i < this->i_extra_tile_count; i++)
	   {
		fread(&p_pe[i], i_extra_data_size, 1,fp);
	   }
	}

	//now copy everything we loaded to it's correct place

	for (int h=0; h < this->i_extra_tile_count; h++)
	{
	  memcpy(cls_tglo.p_map->get_tile_extra_pointer(p_pe[h].i_x, p_pe[h].i_y), 
		  &p_pe[h], sizeof(CTileExtra));

	}

   	//free mem we're done with
	SAFE_FREE(p_pe);


  //clear our building info

	if (cls_tglo.p_building_manager)
	{
	  cls_tglo.p_building_manager->clear_all();
	} else
	{
	  log_error("p_building_manager doens't exist, can't create building");
	}


  //build each building

	for ( int i=0; i < i_count; i++)
	{
		//that's great.  Now create an entity to house the house
		  cls_tglo.p_building_manager->add_building();
	}

}

 