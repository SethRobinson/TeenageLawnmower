/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "ent_utils.h"
#include "CEntItem.h"
#include "CThing_globals.h"
#include "CEntLib.h" 
#include "CThingList.h"
#include "CThingSound.h"
#include "CParticleEmitter.h"			 
#include "CMap.h"
#include "CTerrain.h"
#include "CThing_util.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void Sound2DPlay(char *st_file)
{
  int h_handle = cls_tglo.p_sound->SoundInstanceCreateAndLoad(CDir(st_file));
  cls_tglo.p_sound->SoundInstancePlay(h_handle, 1);
}

//creates an entity and adds it to the thing list and returns it's id
int CreateEntityFromTemplate(char st_class[256], char st_item[256])
{

        //locate the template they want to use by string name
	    unsigned int  ui_class = cls_tglo.p_ent_lib->get_class_id_from_string(st_class);
		if (!cls_tglo.p_ent_lib->class_exists(ui_class))
		{
			log_error("CreateEntityFromTemplate> No class of %s found.",st_class);
			return -1;
		}

		//locate the item index by name too
		int i_ent_id = cls_tglo.p_ent_lib->get_ent_id_from_string(ui_class, st_item);

		if (i_ent_id == -1)
		{
			log_error("CreateEntityFromTemplate> No entity with a name of %s exists in class %s.",
				st_class , st_item);
			return -1;
		
		
		}
  
		CEntItem *p_ent = new CEntItem();	//an ent item is an entity build/owning an item
		cls_tglo.p_thing_list->add_new_thing(p_ent);

		CEntData *p_item_data = NULL; //this will hold the actual item data that gets loaded/saved
		(*cls_tglo.p_ent_lib->get_base_item(ui_class, i_ent_id)).p_ent_default->clone(&p_item_data);
		p_ent->setup_item_from_cent(p_item_data);
		//preload it
		p_ent->dx_create();
		
//		log_msg("Created thing %d, %d.", ui_class, i_ent_id);

        return p_ent->get_id();
} 

hSoundInstance Sound3DPlay(D3DXVECTOR3 *p_vec_pos, char *st_file, int i_repeat)
{
  if (!cls_tglo.p_sound) return C_SOUND_NONE;
 //Create a 3d sound entity to handle this sound, it will autodie when the sound is no more
   CThingSound *p_sound = new CThingSound();
   cls_tglo.p_thing_list->add_new_thing(p_sound);
 
   //we're actually passing an unsigned int to an int but hey, we don't want to add a new type to our
   //scripting system so this will just have to be good enough.

 	hSoundInstance h_sound = p_sound->Init3DSound(p_vec_pos, st_file);
    h_sound = p_sound->Init3DSound(p_vec_pos, st_file);
	//actually play it
	if (h_sound == C_SOUND_NONE)
    {
        //failed big time, kill the object
        p_sound->b_delete_flag = true;
        assert(0);
    } else
    {
        p_sound->Play(i_repeat);
    }
	return h_sound;
}


void EntKillConnectedPS(CThingList *p_thinglist, CThing *p_thing)
{
    //cycle through, find all particle systems and kill the ones attached to this
    //entity
    if (!p_thinglist->b_deleting_list)
    {
        for (CThingItor itor=p_thinglist->m_object_list.begin(); itor != p_thinglist->m_object_list.end(); ++itor)
        {
            if ((*itor)->get_type() == C_THING_PARTICLE_SYSTEM)
            {
                 ( (CParticleEmitter*)(*itor))->set_visual_follow(NULL); 
            (*itor)->b_delete_flag = true; //bye
            }
        }
      
    }
   
}


bool ValidateCollisionAccurate(D3DXVECTOR3 *p_vec_force, D3DXVECTOR3 *p_vec_orig, float f_speed, float f_spacer)
{
    //check movement against surrounding tiles

   bool b_all_clear = true;
    //first check X
for (int i=0; i < 10; i++)    
{


    if (p_vec_force->x != 0)
    {
        
        D3DXVECTOR3 vec_temp_pos = (*p_vec_orig);
        vec_temp_pos.x += f_speed * cls_tglo.time()*p_vec_force->x;
    //    vec_temp_pos.z += f_speed * cls_tglo.time()* p_vec_force->z;
        int i_result; 
        if (i_result = cls_tglo.p_map->SphereToTileCheck(&vec_temp_pos, f_spacer))
        {
           if (i_result == C_COLLISION_BOTH)
            {
               p_vec_force->x = 0; //disable this axis of movement and recompute
                p_vec_force->z = 0; //disable this axis of movement and recompute
             }  
            if (i_result == C_COLLISION_X_POS)
            {
                 if (p_vec_force->x >0) p_vec_force->x = 0; //disable this axis of movement and recompute
            } 
            else 
                if (i_result == C_COLLISION_X_NEG)
                {
                     if (p_vec_force->x < 0) p_vec_force->x = 0; //disable this axis of movement and recompute
            //hit something
        b_all_clear = false;
        continue; //try again
      }
                
        }                  
        
      
    }
      if (p_vec_force->z != 0)
        {
            
            D3DXVECTOR3 vec_temp_pos = (*p_vec_orig);
   //    vec_temp_pos.x += f_speed * cls_tglo.time()*p_vec_force->x;
        vec_temp_pos.z += f_speed * cls_tglo.time()* p_vec_force->z;
            int i_result; 
            if (i_result = cls_tglo.p_map->SphereToTileCheck(&vec_temp_pos, f_spacer))
            {
               if (i_result == C_COLLISION_BOTH)
            {
                p_vec_force->x = 0; //disable this axis of movement and recompute
                p_vec_force->z = 0; //disable this axis of movement and recompute
            }  
            
                if (i_result == C_COLLISION_Y_POS)
                {
                    if (p_vec_force->z >0) p_vec_force->z = 0; //disable this axis of movement and recompute
                } 
                else 
                    if (i_result == C_COLLISION_Y_NEG)
                    {
                        if (p_vec_force->z < 0) p_vec_force->z = 0; //disable this axis of movement and recompute
                    }
             //hit something
           b_all_clear = false;
           continue; //try again
       
            }         
       
       }


    //if we got here we're ok
    break;
}
 return b_all_clear;

}



bool ValidateCollision(D3DXVECTOR3 *p_vec_force, D3DXVECTOR3 *p_vec_orig, float f_speed, float f_spacer)
{
 
  float f_height_of_blocked = 1000;  
  //check the 4 tiles close to us
  D3DXVECTOR3 vec_pos_hold;
  D3DXVECTOR3 *vec_position;
  bool b_all_clear = true;
  
  for (int i = 0; i < 10; i++) //insure we can never lock up on a bad collision test
  {
  vec_pos_hold = *p_vec_orig;
  vec_position = &vec_pos_hold;
  //apply change to our temp var
  *vec_position += ( (*p_vec_force)*(cls_tglo.time()*f_speed));

      


  //maybe I should make sure nothing starts at 0 to force all checks?  like do 0.00001 if it's zero to start
//  if (p_vec_force->x ==0) p_vec_force->x = 0.0001f;
//  if (p_vec_force->z ==0) p_vec_force->z = 0.0001f;

  if (p_vec_force->x < 0)
  {
	  if (cls_tglo.p_map->tile_is_hard_world(vec_position->x-f_spacer, vec_position->z))
	  {
		   if (cls_tglo.p_map->GetWorldHeightIncludingWalls(vec_position->x-f_spacer,vec_position->z, f_height_of_blocked) >= vec_position->y)
		  {
		
          //hit wall. Let's not allow them to move in this dir
		  p_vec_force->x = 0;
		  //restart test
		  b_all_clear = false;
		  continue;
          }
	  }
  }
  
  //east
  if (p_vec_force->x > 0)
  {
      if (cls_tglo.p_map->tile_is_hard_world(vec_position->x+f_spacer, vec_position->z))
      {
          if (cls_tglo.p_map->GetWorldHeightIncludingWalls(vec_position->x+f_spacer,vec_position->z, f_height_of_blocked) >= vec_position->y)
          {
              
              //hit wall
              p_vec_force->x = 0;
              b_all_clear = false;
              continue;
          }
      }
  }
  
  if (p_vec_force->z > 0)
   {
	   if (cls_tglo.p_map->tile_is_hard_world(vec_position->x, vec_position->z+f_spacer))
	   {
	      if (cls_tglo.p_map->GetWorldHeightIncludingWalls(vec_position->x,vec_position->z+f_spacer, f_height_of_blocked) >= vec_position->y)
          {
          
              //hit wall to the north
		   p_vec_force->z = 0;
		  b_all_clear = false;
		   continue;
          }
	   }
	   
	   
   }
   
   //south

  if (p_vec_force->z < 0)
  {
      if (cls_tglo.p_map->tile_is_hard_world(vec_position->x, vec_position->z-f_spacer))
      {
          if (cls_tglo.p_map->GetWorldHeightIncludingWalls(vec_position->x,vec_position->z-f_spacer, f_height_of_blocked) >= vec_position->y)
          {
              //hit wall 
              p_vec_force->z = 0;
              b_all_clear = false;
              continue;
          }
      }
  }

 
   //one last check.. is the tile we are currently inside of hard?  If so we're f***ed.  It's
   //funny I use astricks even though I'm the only person who should ever be reading this code.
   //haha.  ok, moments over, back to work.
  
   if (cls_tglo.p_map->tile_is_hard_world(vec_position->x, vec_position->z))
   {
        if (cls_tglo.p_map->GetWorldHeightIncludingWalls(vec_position->x,vec_position->z,f_height_of_blocked) >= vec_position->y)
        {
        
            //  log_msg("REAL HARD.");
       b_all_clear = false;
		   p_vec_force->z = 0;
		   p_vec_force->x = 0;
        }
   }
    
   
   //if we got here all is cool
	break;
   }
 
  //throw in a quick check to see if they stepped off the valid map
  
  if (!cls_tglo.p_map->is_valid_position(vec_position->x, vec_position->z))
  {
	       p_vec_force->z = 0;
		   p_vec_force->x = 0;
         b_all_clear = false;
  }
	  return  b_all_clear;
	
}


//doesn't check the height of walls, good for the player in TLM so he won't fly over walls when lightninged
bool ValidateCollisionSimple(D3DXVECTOR3 *p_vec_force, D3DXVECTOR3 *p_vec_orig, float f_speed, float f_spacer)
{
 
  //check the 4 tiles close to us
  D3DXVECTOR3 vec_pos_hold;
  D3DXVECTOR3 *vec_position;
  bool b_all_clear = true;
  
  for (int i = 0; i < 10; i++) //insure we can never lock up on a bad collision test
  {
  vec_pos_hold = *p_vec_orig;
  vec_position = &vec_pos_hold;
  //apply change to our temp var
  *vec_position += ( (*p_vec_force)*(cls_tglo.time()*f_speed));
    

  if (p_vec_force->x < 0)
  {
	  if (cls_tglo.p_map->tile_is_hard_world(vec_position->x-f_spacer, vec_position->z))
	  {
	      //hit wall. Let's not allow them to move in this dir
		  p_vec_force->x = 0;
		  //restart test
		  b_all_clear = false;
		  continue;
  	  }
  }
  
  //east
  if (p_vec_force->x > 0)
  {
      if (cls_tglo.p_map->tile_is_hard_world(vec_position->x+f_spacer, vec_position->z))
      {
              //hit wall
              p_vec_force->x = 0;
              b_all_clear = false;
              continue;
      }
  }
  
  if (p_vec_force->z > 0)
   {
	   if (cls_tglo.p_map->tile_is_hard_world(vec_position->x, vec_position->z+f_spacer))
	   {
            //hit wall to the north
		   p_vec_force->z = 0;
		  b_all_clear = false;
		   continue;
	   }
	   
	   
   }
   
   //south

  if (p_vec_force->z < 0)
  {
      if (cls_tglo.p_map->tile_is_hard_world(vec_position->x, vec_position->z-f_spacer))
      {
              //hit wall 
              p_vec_force->z = 0;
              b_all_clear = false;
              continue;
      }
  }

 
   //one last check.. is the tile we are currently inside of hard?  If so we're f***ed.  It's
   //funny I use astricks even though I'm the only person who should ever be reading this code.
   //haha.  ok, moments over, back to work.
  
   if (cls_tglo.p_map->tile_is_hard_world(vec_position->x, vec_position->z))
   {
             //  log_msg("REAL HARD.");
       b_all_clear = false;
		   p_vec_force->z = 0;
		   p_vec_force->x = 0;
   }
    
   
   //if we got here all is cool
	break;
   }
 
  //throw in a quick check to see if they stepped off the valid map
  
  if (!cls_tglo.p_map->is_valid_position(vec_position->x, vec_position->z))
  {
	       p_vec_force->z = 0;
		   p_vec_force->x = 0;
         b_all_clear = false;
  }
	  return  b_all_clear;
	
}


//code for the camera.  basically the same as above but takes a start position and a requested displacement
//amount rather than a vector force.

bool ValidateCollisionCamera(D3DXVECTOR3 *p_vec_displacement, D3DXVECTOR3 *p_vec_orig,  float f_spacer)
{
  //check the 4 tiles close to us
  float const f_height_of_blocked = 0;

  static D3DXVECTOR3 vec_pos_hold;
  vec_pos_hold = *p_vec_orig;
  D3DXVECTOR3 *vec_position;
  vec_position = &vec_pos_hold;
  
  
  for (int i = 0; i < 10; i++) //insure we can never lock up on a bad collision test
  {
   *vec_position = *p_vec_orig;
  //apply change to our temp var
  *vec_position += (*p_vec_displacement);
  
  if (p_vec_displacement->x < 0)
  {
	  if (cls_tglo.p_map->tile_is_hard_world(vec_position->x-f_spacer, vec_position->z))
	  {
		  //hit wall. Let's not allow them to move in this dir

  		  if (cls_tglo.p_map->GetWorldHeightIncludingWalls(vec_position->x-f_spacer,vec_position->z, f_height_of_blocked) >= vec_position->y)
		  {
			  p_vec_displacement->x = 0;
			  continue;
		  }
	  }
  }
  
  //east
  if (p_vec_displacement->x > 0)
  {
	  if (cls_tglo.p_map->tile_is_hard_world(vec_position->x+f_spacer, vec_position->z))
	  {
  		  if (cls_tglo.p_map->GetWorldHeightIncludingWalls(vec_position->x+f_spacer,vec_position->z, f_height_of_blocked) >= vec_position->y)
		  {
			  p_vec_displacement->x = 0;
			  continue;
		  }
	  }
  }
  
  if (p_vec_displacement->z > 0)
   {
	   if (cls_tglo.p_map->tile_is_hard_world(vec_position->x, vec_position->z+f_spacer))
	   {
		   //hit wall to the north
		 if (cls_tglo.p_map->GetWorldHeightIncludingWalls(vec_position->x,vec_position->z+f_spacer, f_height_of_blocked) >= vec_position->y)
		  {
			  p_vec_displacement->z = 0;
			  continue;
		  }

	   }
	   
	   
   }
   
   //south

   if (p_vec_displacement->z < 0)
   {
	   if (cls_tglo.p_map->tile_is_hard_world(vec_position->x, vec_position->z-f_spacer))
	   {
			if (cls_tglo.p_map->GetWorldHeightIncludingWalls(vec_position->x,vec_position->z-f_spacer, f_height_of_blocked) >= vec_position->y)
		  {
			  p_vec_displacement->z = 0;
			  continue;
		  }
	   }
   }
	//if we got here all is cool
   return true;
   }

 //if we got here there was an error	

  log_msg("Camera got locked up.") ;
  return false;
	
}


//safe way to kill something if you really aren't sure if the pointer is valid,
//used by textboxes to catch errors that scripts make.
void KillByAddress(CThingList *p_thinglist, CThing *p_thing)
{
    
    //cycle through, find all particle systems and kill the ones attached to this
    //entity
    if (!p_thinglist->b_deleting_list)
    {
        for (CThingItor itor=p_thinglist->m_object_list.begin(); itor != p_thinglist->m_object_list.end(); ++itor)
        {
            
            if ((*itor) == p_thing)
            {
                (*itor)->b_delete_flag = true; //bye
            }
        }
        
    }
    
}


