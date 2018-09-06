/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CManager.h"
#include "..\\CTextParse.h"
#include "CResourceMD2.h"
#include "CResourceRTM.h" //robinson technolgies model format

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CManager::CManager()
{
	i_dynamic_id_counter = ::C_MANAGER_DYNAMIC_START_ID;
	this->set_sort(-3000); //make sure we get processed first
	b_scan_active = false;
	b_deleting_all = false;
	set_thing_name("Resource Manager");
}


void CManager::delete_all()
{
	//we allow incorrect refs in this case
	b_deleting_all = true;
	
	for (itor_manager itor = map.begin(); itor != map.end();)
	{
	   //delete everything
		//first delete the class the pointer was pointing at
		SAFE_DELETE(itor->second);
		map.erase(itor++);
	}
	b_deleting_all = false;
   
}





int CManager::delete_ref(int i_index)
{
	static int i_return = 0;
	
	if (b_deleting_all) return 0; //ignore refs at this stage, we're deleting
	//everything anyway

	i_return = map[i_index]->delete_ref();
	
//	ASSERT(i_return >= 0);
	if (i_return == 0)
	{
	  //nobody is using this resource, completely remove it unless
		delete map[i_index];
		map.erase(i_index);

	}
	return i_return;
}

int CManager::add_ref(int i_index)
{
  	
	return map[i_index]->add_ref();
}

bool CManager::load_if_needed(int i_index)
{
  //make sure this resource is loaded
	  if (!map[i_index]->b_loaded)
	  {
	    //load it up!
		map[i_index]->dx_create();
		return true;
	  }

	return false; //didn't reload anything
}


CManager::~CManager()
{
	delete_all();
	cls_tglo.p_manager = NULL;
}

void CManager::think()
{


}

void CManager::dx_create()
{
  for (itor_manager itor = map.begin(); itor != map.end(); ++itor)
  itor->second->dx_create();
}


void CManager::dx_restore()
{
  for (itor_manager itor = map.begin(); itor != map.end(); ++itor)
  itor->second->dx_restore();
}

void CManager::dx_invalidate()
{

  for (itor_manager itor = map.begin(); itor != map.end(); ++itor)
  itor->second->dx_invalidate();
}

void CManager::dx_kill()
{

  for (itor_manager itor = map.begin(); itor != map.end(); ++itor)
  itor->second->dx_kill();
}



//call this in a while loop to walk through the resource tree, you can filter
//for one kind of resource or just do C_RESOURCE_NONE to grab them all
//not multithread able!!!
bool CManager::scan_resources(const int i_type_to_scan, const int i_sub_type_to_scan, char p_st_name[], int *p_i_id)
{
   if (!b_scan_active)
   {
	 scan_itor = map.begin();
	 b_scan_active = true;
   }
again:

   if (scan_itor == map.end())
   {
	 b_scan_active = false;
	 return false; //all done
   }

   if (i_type_to_scan != ::C_RESOURCE_NONE)
   if (scan_itor->second->i_resource_type != i_type_to_scan)
   {
	 //we don't want this one.
	   ++scan_itor;
	   goto again;
   }

   if (i_sub_type_to_scan != ::C_RESOURCE_NONE)
   {
      if (scan_itor->second->i_resource_sub_type != i_sub_type_to_scan)
   {
	 //we don't want this one.
	   ++scan_itor;
	   goto again;
   }



   }

   //found one we want to return
   strcpy(p_st_name, scan_itor->second->GetName());
   *p_i_id = scan_itor->first;
  
   //inc the item
   ++scan_itor;
   return true; //keep scanning
}

void CManager::list_contents()
{
  log_msg("Listing resources...");
  char st_type[100];
  char st_loaded[40];
  for (itor_manager itor = map.begin(); itor != map.end(); ++itor)
  {
      if (itor->second->i_resource_type == ::C_RESOURCE_TEXTURE)
		  strcpy(st_type, "texture"); else
       if (itor->second->i_resource_type == ::C_RESOURCE_PMESH)
		  strcpy(st_type, "x mesh"); else
       if (itor->second->i_resource_type == ::C_RESOURCE_3DS)
		  strcpy(st_type, "3ds mesh"); else
	  if (itor->second->i_resource_type == ::C_RESOURCE_SPHERE)
		  strcpy(st_type, "Sphere"); else
	  if (itor->second->i_resource_type == ::C_RESOURCE_SPRITE)
		  strcpy(st_type, "Sprite"); else
	  if (itor->second->i_resource_type == ::C_RESOURCE_CAL_CORE)
		  strcpy(st_type, "CalCore"); else
	  if (itor->second->i_resource_type == ::C_RESOURCE_CAL_RENDER)
		  strcpy(st_type, "Cal Renderer"); else

	  if (itor->second->i_resource_type == ::C_RESOURCE_MD2)
		  strcpy(st_type, "MD2"); else
		  
  if (itor->second->i_resource_type == ::C_RESOURCE_RTM)
		  strcpy(st_type, "RTSoft Model"); else
		  
		  strcpy(st_type, "unknown");
	  st_loaded[0] = 0;
	  if (itor->second->b_loaded) sprintf(st_loaded, "(loaded) (%d refs)", itor->second->get_refs());
	  
	  log_msg("%d (%s) - Name: %s %s", itor->first, st_type, itor->second->GetName(), st_loaded);
  }

}

void CManager::add_resource_by_string(int i_index, int i_resource_type, int i_resource_sub_type,  char st_resource[256])
{
 
  if (map[i_index] != NULL)
  {
    log_error("Resource %s is overwriting resource %s!",st_resource, map[i_index]->GetName());
//    ASSERT(0);
  }

  CResource *res;
  
  if (i_resource_type == ::C_RESOURCE_TEXTURE)
  {
      res = new CResourceTex;
  } else
  if (i_resource_type == ::C_RESOURCE_PMESH)
  {
      res = new CResourcePMesh;
  } else
  if (i_resource_type == ::C_RESOURCE_3DS)
  {
      res = new CResource3DS;
  } else

  if (i_resource_type == ::C_RESOURCE_MD2)
  {
      res = new CResourceMD2;
  } else
 if (i_resource_type == ::C_RESOURCE_RTM)
  {
      res = new CResourceRTM;
  } else
	  
	  if (i_resource_type == ::C_RESOURCE_SPHERE)
  {
      res = new CResourceSphere;
  } else
if (i_resource_type == ::C_RESOURCE_SPRITE)
  {
      res = new CResourceSprite;
  } else
	#ifdef _CAL3D  

	  if (i_resource_type == ::C_RESOURCE_CAL_CORE)
  {
	
	res = new CResourceCalCore;
  } else 
	  if (i_resource_type == ::C_RESOURCE_CAL_RENDER)
  {
      res = new CResourceCalRender;
  } else
	#endif

 {
    log_error("Unknown resource type.");
//	ASSERT(0);
    return;
  };

  strcpy(res->GetName(), st_resource);
  res->i_last_used = cls_tglo.get_game_age();
  res->i_resource_type = i_resource_type;
  res->i_resource_sub_type = i_resource_sub_type;
  //add a ref so it doesn't get deleted automatically
 // res->add_ref();    //let have 0 refs, don't worry, it won't get deleted because it's not dynamic.
 
  //add to map
  map[i_index] = res;
//  log_msg("Resource %s addeded by string.",res->GetName());
}

int CManager::find_resource_by_type_and_string(int i_resource_type, char st_resource[256])
{
  //cycle through all resources until we locate this one, slow, but shouldn't happen much
   for (itor_manager itor = map.begin(); itor != map.end(); ++itor)
   {
	 if (itor->second->i_resource_type == i_resource_type)
		 if (stricmp(itor->second->GetName(), st_resource) == 0) return itor->first;
   }

   //none found
   return ::C_RESOURCE_NONE;
}

//send this "" if you don't want to check for a previous version
bool CManager::does_resource_exist(int i_id)
{
   if (map[i_id] == NULL)
   {
	 //resource doesn't exist
	   map.erase(i_id);
	   return false;
   }
 
 return true;  
}

int CManager::get_dynamic_resource_by_string(int i_resource_type, char st_resource[256])
{
 
//	log_msg("asking for %s, a %d.", st_resource, i_resource_type);
	
	//does a resource of this type and string already exist?
 	if (st_resource[0] != 0)
	{
	int i_return = find_resource_by_type_and_string(i_resource_type, st_resource);
	
  if (i_return != C_RESOURCE_NONE)
  {
	  map[i_return]->add_ref(); //let them know it's being used somewhere
	  return i_return; //already have this resource in memory somewhere
  }
	}

  //log_msg("Adding new resource (%s).", st_resource);
  //guess we have to create it
  int i_id = ++i_dynamic_id_counter;


   CResource *res;
  
  if (i_resource_type == ::C_RESOURCE_TEXTURE)
  {
      res = new CResourceTex;
  } else
 if (i_resource_type == ::C_RESOURCE_PMESH)
  {
      res = new CResourcePMesh;
  } else
  if (i_resource_type == ::C_RESOURCE_3DS)
  {
      res = new CResource3DS;
  } else
  if (i_resource_type == ::C_RESOURCE_SPHERE)
  {
      res = new CResourceSphere;
  } else
 if (i_resource_type == ::C_RESOURCE_SPRITE)
  {
      res = new CResourceSprite;
  } else
 	#ifdef _CAL3D  

if (i_resource_type == ::C_RESOURCE_CAL_CORE)
  {
      res = new CResourceCalCore;
  } else
	  if (i_resource_type == ::C_RESOURCE_CAL_RENDER)
  {
      res = new CResourceCalRender;
  } else
#endif
 if (i_resource_type == ::C_RESOURCE_MD2)
  {
      res = new CResourceMD2;
  } else
  if (i_resource_type == ::C_RESOURCE_RTM)
  {
      res = new CResourceRTM;
  } else





	  {
    log_error("Unknown resource type.");
//	ASSERT(0);
    return ::C_RESOURCE_NONE;
  };

  strcpy(res->GetName(), st_resource);
  res->i_last_used = cls_tglo.get_game_age();
  res->i_resource_type = i_resource_type;
  res->b_dynamic = true;
  //add to map
  map[i_id] = res;
  add_ref(i_id); //let them know it's being used somewhere
  return i_id; //tell them what we gave it
}

/*
CResource * CManager::get(int i_index)
{
    if (map[i_index] == NULL)
	{
		log_error("Resource %d is blank!",i_index);
	    map.erase(i_index);
		ASSERT(0);
		return 0;
	}
 
 	map[i_index]->i_last_used = cls_tglo.get_game_age();
	return map[i_index];
}

*/

char * CManager::get_string(int i_index)
{
       if (map[i_index] == NULL)
	   {
		   log_error("Resource %d is blank!",i_index);
	     //  ASSERT(0);
	       map.erase(i_index);
		   return 0;
	   }

 	map[i_index]->i_last_used = cls_tglo.get_game_age();
	return map[i_index]->GetName();
}



bool CManager::load_resource_list(char *p_st_file)
{
 //process a text file and look for texture we want to add to our map
   //log_msg("Loading risk texts from risks.txt.");
   CTextParse cls_parse;
  // cls_parse.set_path(st_data_path);
   
   char st_real_dir[256];

   strcpy(st_real_dir, p_st_file);

   if (!exist(st_real_dir))
   {
       log_msg("%s missing from mod dir, using base dir instead.", p_st_file);
       sprintf(st_real_dir, "%s\\%s", GetExeDir(), p_st_file);
   }

#ifdef _DEMO
   //extra security
      sprintf(st_real_dir, "%s\\%s", GetExeDir(), p_st_file); //don't allow mods at all
 

#endif

   if (!cls_parse.open(st_real_dir))
   {
	 log_error("Couldn't open resource list %s.",st_real_dir);
	 return false;
   }
   
	//ready to read actual stuff!
	char *p_buffer;
 	char st_crap[256];
	char st_temp[256];
		while(cls_parse.get_next_line(&p_buffer))
		{
	 	strcpy(st_crap, cls_parse.get_word(1));
	
		if (strcmp(st_crap, "set_texture") == 0)
		{
		   //clear vars/set defaults
			strcpy(st_temp, cls_parse.get_word(3));
			this->add_resource_by_string(atol(cls_parse.get_word(2)),
			   C_RESOURCE_TEXTURE, C_RESOURCE_SUB_LANDSCAPE, st_temp);
		} else
		{
		  //nothing we know
		}

	}


 return true;
}

void CManager::DeleteAllNonDynamic(int i_type_to_unload)
{
    for (itor_manager itor = map.begin(); itor != map.end();)
    {
        if (itor->second->i_resource_type == i_type_to_unload)
        {
            if (!itor->second->b_dynamic)
            {
                
                while (itor->second->get_refs() > 0)
                {
                    itor->second->delete_ref();
                }
                
                itor->second->dx_invalidate(); //unload it
            
            	SAFE_DELETE(itor->second);
   		       //modified 5-7-2004 to compile with stlport
                map.erase(itor++);
               continue;
            
            }
            
        }
        ++itor;
    }


}


void CManager::UnloadAllNonDynamic(int i_type_to_unload)
{
    //cycle through the resources and unload all of this kind
    
    for (itor_manager itor = map.begin(); itor != map.end(); ++itor)
    {
        if (itor->second->i_resource_type == i_type_to_unload)
        {
            if (!itor->second->b_dynamic)
            {
                
                while (itor->second->get_refs() > 0)
                {
                    itor->second->delete_ref();
                }
                
                itor->second->dx_invalidate(); //unload it
            }
            
        }
        
    }
    
}




