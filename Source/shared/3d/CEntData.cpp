/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CEntData.h"


//globals used

itor_ent scan_itor; 
bool b_scan_active = false;
 


CEntData::CEntData()
{
 memset(this, 0, sizeof(CEntData));
 vec_scale.x = vec_scale.y = vec_scale.z = 1;
 strcpy(this->st_name, "Unnamed");
}

CEntData::~CEntData()
{
 	kill();
}

void CEntData::kill()
{
 	//free any memory if we need to
	if (p_person)
	{
	 SAFE_DELETE(p_person);
	}

	if (p_list_attached)
	{
	   //items are attached to us, we need to kill each one and deinit the list
			for (itor_ent itor = this->p_list_attached->begin(); itor != this->p_list_attached->end();)
	{
		delete (*itor);
		  
		p_list_attached->erase(itor++);
	}

		//kill the list
		SAFE_DELETE(this->p_list_attached);

	}

}
//pass this a pointer to something you newed() already and it will take it over

bool CEntData::attach_entity(CEntData *p_ent)
{
	
  //first, do we even have the list initted?
	if (!this->p_list_attached)
	{
	  p_list_attached = new list_ent;
	  if (!this->p_list_attached)
	  {
	    log_error("Unable to init memory for new item");
		return false;
	  }
	}

	//actually add the item
	p_list_attached->push_back(p_ent);
	this->i_attached = p_list_attached->size();
	return true;	
}


bool CEntData::remove_entity(int i_index_in_linked_list)
{
    int i_counter = 0;
	if ( (i_index_in_linked_list < 0) || 
		i_index_in_linked_list > int(p_list_attached->size()))
	{
		log_error("Can't delete entity from linked list, %d is OUT of range.", i_index_in_linked_list);
			return false;
	}
	
	


	
	for (itor_ent itor = this->p_list_attached->begin(); itor != this->p_list_attached->end();)
	{
		if (i_counter == i_index_in_linked_list)
		{
		  //this is the one we want to remove/delete
		  	delete (*itor); //delete the real object
			itor = p_list_attached->erase(itor); //remove it from our list
			this->i_attached = p_list_attached->size();
		
			if (i_attached == 0)
			{
			  //let's also delete this whole linked list struct,
				//we don't need it
				SAFE_DELETE(p_list_attached);

			}
			return true;
		}

		i_counter++;
		itor++;	 //go to next
	}

 log_error("Unable to find entity to remove from entity %s.", st_name);  
 return false;
}



CEntData * CEntData::get_all_attached_entities()
{
  if (!b_scan_active)
  {
  	  if (!p_list_attached)
	  {
	    //no items are attached, return null
		  return NULL;
	  }
	  scan_itor = p_list_attached->begin();
	  b_scan_active = true;
 	 
	 //special case for first one
  	  log_msg("Returning item %s..", (*scan_itor)->st_name);
	  return (*scan_itor);
  
  }

  ++scan_itor;

  if (scan_itor == p_list_attached->end())
   {
	 b_scan_active = false;
	 return NULL; //all done
   }

  
	  log_msg("Returning item %s..", (*scan_itor)->st_name);
  return (*scan_itor);
}



bool CEntData::save(FILE *fp)
{

 //first write the size of the record to allow dynamic changes later 
 int i_size = sizeof(CEntData);
//log_msg("Saving item %s.. (%d bytes)", this->st_name, i_size);
 fwrite(&i_size, sizeof(int), 1, fp);

 if (fwrite(this, i_size, 1, fp) <= 0)
	{
		log_error("Error writing template %s.",st_name);
		return false;
	}

 //write any attached items too

 //if we have "person/monster" info let's save it
 if (this->p_person	)
 {

	 int i_size_person = sizeof(CEntPerson);
	 log_msg("Saving extra person data .. (%d bytes)", this->st_name, i_size_person);
	 fwrite(&i_size_person, sizeof(int), 1, fp);
	 //save it's size and it
	 fwrite(this->p_person, i_size_person, 1, fp);
	 {
		 log_error("Error writing template %s.",st_name);
		 return false;
	 }
	 
 }
 
  if (this->i_attached > 0)
  {
    //let's also save the items that are attached to this one
	if (!p_list_attached)
	{
	  log_error("We should be saving %d items but the p_list_attached struct is null?", i_attached);
	  return false;
	}

    //save each item that is attached
    for (itor_ent itor = this->p_list_attached->begin(); itor != this->p_list_attached->end();)
	{
		//recursively save this attached item and any items attached to it..
		log_msg("Saving attached thing %s.",(*itor)->st_name);
		(*itor)->save(fp);
		itor++;	 //go to next
	}



  }


return true;
}




bool CEntData::load(FILE *fp)
{
//	log_msg("Loading item..");
	
	//first write the size of the record to allow dynamic changes later 
	int i_size;
	fread(&i_size, sizeof(int), 1, fp);
	
	if (fread(this, i_size, 1, fp) <= 0)
	{
		log_error("Error reading item");
		return false;
	}
	
	//read any attached items too
	if (this->p_person)
	{
		//there is a junk pointer in this, we must have people data associated with this item
		
		int i_size_person;
		fread(&i_size_person, sizeof(int), 1, fp);
		p_person = new CEntPerson();
		
		if (fread(p_person, i_size_person, 1, fp) <= 0)
		{
			log_error("Error reading person data in item %s.",st_name);
			return false;
		}
		
	}

	if (this->i_attached > 0)
	{
	  	//clear our garbage pointer	to the stl linked list
		this->p_list_attached = NULL;
		
	   int i_attached_temp = i_attached; //IMPORTANT, i_attached gets set everytime
	   //we add something, we need to this to remember how many things we need to
	   //load.

		//we also have to load attached items
	   for (int i=0; i < i_attached_temp; i++)
	   {
		  CEntData *p_new = new CEntData();	
          p_new->load(fp);
		  //now attach it to us
		  this->attach_entity(p_new);
	   }
	}

  
	return false;
}

//this accepts a pointer to a pointer to a CEntData that should be null.  We init it and
//copy everything from this item into it, making a perfect clone.


bool CEntData::clone(CEntData **p_ent)
{
 	
  //First init the main struct
	
  *p_ent = new CEntData();

  CEntData *p_new = (*p_ent); //makes it a little easier for me to deal with


  memcpy(p_new, this, sizeof(CEntData)); 

  //now we have to fix any bad pointers that we copied and init other memory structs that we
  //rely on

  	if (this->p_person)
	{
		//there is a junk pointer in this, we must have people data associated with this item
		p_new->p_person = new CEntPerson();
		memcpy(p_new->p_person, p_person, sizeof(CEntPerson));
	} else
	{
	  //no person data struct yet, leave it null
	}


	//are any other items attached to this item?  If so, we have to copy them too.

	if (this->p_list_attached)
	{
      //yes there are items.  First let's init our stl linked list for the items.
	  p_new->p_list_attached = NULL; //kill the bad pointer that was there

	  //now let's clone each item into it
	
	  for (itor_ent itor = this->p_list_attached->begin(); itor != this->p_list_attached->end();)
	  {
    
		  CEntData *p_new_item = NULL;
		  (*itor)->clone(&p_new_item);
		  p_new->attach_entity(p_new_item);
	  }

	} else
	{
	  //no items are attached, we're done.
	}



 return true;
}




