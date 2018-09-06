/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CEntLib.h"
#include "CThing_globals.h"
#include "..\\all.h"

CEntLib::CEntLib()
{
  //init defaults
   b_scan_active = false;	
   m_i_max_categories_demo_can_load = 13;    //default for TLM

}

CEntLib::~CEntLib()
{
  destroy_all();
  cls_tglo.p_ent_lib = NULL; //we don't exist
}
	
void CEntLib::destroy_all()
{
    	for (itor_map_base itor = base.begin(); itor != base.end();)
	{
	 	//the deconstructer for the vecter would probably do this for me but I might want to
	    //do something special later so..
		delete_sub_class(itor->first);
		delete itor->second; //kill the category struct now that's basically empty anyway
		base.erase(itor++);
	}
  
    
}


void CEntLib::save_category(unsigned int ui_class_id)
{
   CCategory *p_cat =  base[ui_class_id];

   if (p_cat == NULL)
 {
   log_error("Class ID %u doesn't exist yet, can't save it.",ui_class_id);
    base.erase(ui_class_id);
   return;
 }

  //log_msg("Saving category %s (id %u)",base[ui_class_id]->st_filename, base[ui_class_id]->ui_class_id);

   //open file
  char st_temp[255];
  sprintf(st_temp, "data\\%s", p_cat->st_filename);
  FILE * fp;
  if ( (fp=fopen(st_temp, "wb")) == NULL)
	{
      log_error("Unable to write category file %s.",p_cat->st_filename);
	  return; //no error
	}

    p_cat->i_ent_count = p_cat->items->size();
  	int i_size_of_ccat = sizeof(CCategory);
    p_cat->i_size_of_cbase = sizeof(CBaseEntData);
	
	//first right the size of this struct
  	fwrite(&i_size_of_ccat, sizeof(int), 1, fp);

	//now write the CCat
	if (fwrite(p_cat, i_size_of_ccat, 1, fp) <= 0)
	{
	  log_error("Error writing to category %s.",p_cat->st_filename);
	}

	//next write out all our little vectors
	for (int i=0; i < p_cat->i_ent_count; i++)
	{
	(*p_cat->items)[i]->save(fp); //let the template save itself
		//ok, we just wrote the base item description, but now let's write any 
		//objects connecting to the item recursively.  
	}
	//we're done?
	fclose(fp);

}


unsigned int CEntLib::load_category(char st_filename[])
{
   //open file
  char st_temp[255];
  sprintf(st_temp, "%s",st_filename);
  FILE * fp;
  if ( (fp=fopen(st_temp, "rb")) == NULL)
	{
      log_error("Unable to read category file %s.",st_filename);
	  return 0; //no error
	}

   	int i_size_of_ccat = 0;
    //read size of struct
	//first right the size of this struct
  	fread(&i_size_of_ccat, sizeof(int), 1, fp);

	CCategory *p_cat;
	
	p_cat = new CCategory();


	memset(p_cat, 0, sizeof(CCategory));
	//now read the CCat struct
	if (fread(p_cat, i_size_of_ccat, 1, fp) <= 0)
	{
	  log_error("Error reading category %s.",st_filename);
	}


    //ok, let's find out if this cat already exists...
    if (class_exists(p_cat->ui_class_id))
    {
    //    log_error("Error, ignoring entity file %s, because class ID %u already exists.",
    //      p_cat->st_cat_name, p_cat->ui_class_id);
        delete p_cat;
       fclose(fp);
       return 0;
    }


    base[p_cat->ui_class_id] = p_cat; //save pointer to our new category
 
	//now init the vecter struct, it has a garbage pointer in it right now
	p_cat->items = new vec_base();

	//next load all our little vectors (ent descriptions)

	CBaseEntData *p_base_temp = NULL;
	
	for (int i=0; i < p_cat->i_ent_count; i++)
	{
		
		 p_base_temp = new CBaseEntData();
		
		 p_base_temp->load(fp);
		 
		//ok, we loaded the item, now let's attache it to the list...
	   this->add_item(p_cat->ui_class_id, p_base_temp);
	}
	//we're done?
	fclose(fp);
//	log_msg("Loaded %s. (class id %u) (Category: %s with %d items)",p_cat->st_filename,
//		p_cat->ui_class_id, p_cat->st_cat_name, p_cat->i_ent_count);
  
	return p_cat->ui_class_id;
}


CCategory * CEntLib::get_all_categories()
{
  	CCategory *p_cat_temp = NULL;
	
	if (!b_scan_active)
   {
	 scan_itor = base.begin();
	 b_scan_active = true;
   }

   if (scan_itor == base.end())
   {
	 b_scan_active = false;
	 return NULL; //all done
   }

   //found one we want to return
   p_cat_temp = scan_itor->second;
  
   //inc the item
   ++scan_itor;
   return p_cat_temp; //keep scanning

}

CBaseEntData * CEntLib::get_all_ents_in_this_cat(unsigned int ui_class_id)
{
  if (!b_scan_active)
  {
    i_scan_item_temp = 0;
     b_scan_active = true;
  }

  if (i_scan_item_temp == int(base[ui_class_id]->items->size()))
  {
    //all done
	  b_scan_active = false;
	  return NULL;
  }
  i_scan_item_temp++;

  return (*base[ui_class_id]->items)[i_scan_item_temp-1];
}


//no checking is done here to make sure the class exsts, so be carefull
char * CEntLib::GetCategoryName(unsigned int ui_class)
{
    return (char*)&(base[ui_class])->st_cat_name;
}


CBaseEntData * CEntLib::get_base_item(unsigned int ui_class_id, int i_item)
{
   if ( i_item >=  int(base[ui_class_id]->items->size()))
   {
       //number too high
       log_error("Item %d does not exist in item category %s.", i_item, 
           (base[ui_class_id])->st_cat_name);
       return NULL;
   }
    
  // log_msg("Loading %s from %s.","Something",(base[ui_class_id])->st_cat_name );

    return (*base[ui_class_id]->items)[i_item];
}


bool CEntLib::class_exists(unsigned int ui_class)
{
   return base.find(ui_class) != base.end();
}

void CEntLib::delete_sub_class(unsigned int i_class_id)
{
//  	log_msg("Clearing class %u.", i_class_id);

	//clear all of them
	for (int i=0; i < int(base[i_class_id]->items->size()); i++)
	{
	  SAFE_DELETE ((*base[i_class_id]->items)[i]);
	}
	
	base[i_class_id]->items->clear();

	//now delete it
	SAFE_DELETE(base[i_class_id]->items);

}


//Give it a pointer to the item base you want to add, CAREFUL, this pointer
//will be saved, so make sure you new'd it in advance and whatnot.

int CEntLib::add_item(unsigned int i_class_id, CBaseEntData *p_new_base)
{
 
 if (base[i_class_id] == NULL)
 {
   log_error("Class ID %u doesn't exist yet, can't add base item.",i_class_id);
   SAFE_DELETE( p_new_base);
   base.erase(i_class_id);
   return 0;
 }

 //add item to the correct class ID
 base[i_class_id]->items->push_back(p_new_base);
 int i_index = (base[i_class_id]->items->size())-1;
 p_new_base->i_index = i_index;
 
 //set some defaults for the item so it knows who it's dad is
 p_new_base->p_ent_default->i_class_id = i_class_id;
 p_new_base->p_ent_default->i_id = i_index;
 

// log_msg("Creating item %d. ",i_index);
 return i_index;
}
	 

unsigned int CEntLib::get_class_id_from_string(char st_fname[80])
{
  //	log_msg("Computing checksum for %s.",st_fname);
	return compute_checksum_from_string(st_fname);
}


int CEntLib::get_ent_id_from_string(unsigned int ui_class_id, char st_item_name[80])
{
	//cycle through all ents in this cat and return the one that matches the
	//string.

	for (int i=0; i < int(base[ui_class_id]->items->size()); i++)
	{
		if (stricmp((*base[ui_class_id]->items)[i]->st_name, st_item_name) == 0)
		{
			//match found
			return i;
		}
	}


	//error
	return -1;
}

unsigned int CEntLib::add_class(char st_name[80], char st_fname[80])
{
 
	
 unsigned int i_class_id = get_class_id_from_string(st_name);
 
 if (base[i_class_id] != NULL)
 {
   log_error("Class ID %u already exists.",i_class_id);
    return 0;
 }
  
  base[i_class_id] = new CCategory();
  base[i_class_id]->items = new vec_base;
  strcpy(base[i_class_id]->st_cat_name, st_name);
  strcpy(base[i_class_id]->st_filename, st_fname);
  base[i_class_id]->ui_class_id = i_class_id;
  return i_class_id;

}


void CEntLib::load_category_wildcard(char st_wildcard[])
{
	 char st_temp[256];																		  
  	

    
  	 int i_counter = 0;
 i_counter = 0;  
    
    //be careful, this command changes the dir so we'll be in data\\ until the last
	//call moves it back.
  while (get_files_with_wildard("data\\", st_wildcard, (char*)&st_temp))
	 {
	
         i_counter++;


#if defined(_DEMO) == 0 || defined(_DEBUG)
        this->load_category(st_temp);
#else
 
        //demo version, can only load 13.
       
        if (i_counter <= m_i_max_categories_demo_can_load)
         {
              this->load_category(st_temp);
         } else
         {
            extern int g_i_demo_hacked;
             g_i_demo_hacked = 1;

         }

#endif

	 }

  #ifndef _DEMO
 //now load info from main dir if we are using a mod
     if (cls_tglo.mod.m_st_path[0] != 0)
     {
        
         char st_dir[256];
          sprintf(st_dir, "%s\\data\\",GetExeDir());
         
         while (get_files_with_wildard(st_dir, st_wildcard, (char*)&st_temp))
         {
             this->load_category(st_temp);
         }
        
     }

#endif

}
