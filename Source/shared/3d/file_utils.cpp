/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "file_utils.h"
#include "CThing_globals.h"
#include "CMessageHeader.h"
#include "CThingList.h"
#include "..\\all.h"
#include "CEntItem.h"
#include "CEntData.h"


bool save_objects_for_map(CThingList *p_things, const char st_incoming_name[256])
{

	  char st_name[256];
	  strcpy(st_name, st_incoming_name);
			
	//also save all items, monsters, people and entities
	 CMessageHeader msg;
	 msg.i_message = C_THING_MSG_SAVE_GAME;
	 CMessageFile custom; //the custom message
	 
	 change_file_extension(st_name, "obj"); 
	 log_msg("Saving zone objects as %s", st_name);
	 
	 custom.fp = fopen(st_name, "wb");
	 
	 if (!custom.fp)
	 {
	   log_error("Unable to open %s.",st_name);
	   return false;
	 }

	 
	 p_things->broadcast_message(&msg, (void*)&custom, -1); //the second parm is for special instructions, we have none
     //the -1 is to say "save for everything, not a certain kind of object    
	 //all done, close the file handle
	 fclose(custom.fp);
	 return true;
}


void write_type(int i_type, FILE *fp)
{
  fwrite(&i_type, sizeof(int), 1, fp);  
}


bool clear_objects_from_map(CThingList *p_things, int i_type)
{
 	 CMessageHeader msg;
	 msg.i_message = C_THING_MSG_KILL;
  
  	 p_things->broadcast_message(&msg, NULL, i_type); //the second parm is for special instructions, we have none
   	
	 //now, the object have been marked for deletion yes, but to actually 
	 //delete them and have the resources freed we need to do this.

	 //turns out that is a bad idea.  What is this gets called during an item's
	 //think() command?  

	 //p_things->process_delete_requests();

     return true;
}





void reinit_objects_on_map(CThingList *p_things, int i_class_id)
{
   
	CThing *p_thing;
	while (p_thing = p_things->get_next())
   {
		if (p_thing->get_type() == ::C_THING_ENT_GENERIC)
		{
		  if ( ((CEntItem*)p_thing)->get_class_id() == unsigned int (i_class_id))
		  {
		    
			  	 ((CEntItem*)p_thing)->update_ent_data();
		
			  //re-init this item without losing any of its data
			 ((CEntItem*)p_thing)->setup_item_from_cent(NULL); //by sending
			 //null we tell it to just re-init itself
		    //also tell them to load themself if we're currently running
			 //if the item is already loaded it is smart enough to not
			 //reload the jpg or whatever
			 if (cls_tglo.IsRunning())
			 {
			 ((CEntItem*)p_thing)->dx_create();  

			 }

		  
		  }


		}

   }


}


//always do a While(!=null) on this, you need to scan through everything until NULL

CEntItem * scan_for_item_type( CThingList *p_things, unsigned int i_class_id, int i_id)
{
   	log_msg("Looking for class %u, item %d..", i_class_id,  i_id);

	CThing *p_thing = NULL;
	
	while (p_thing = p_things->get_next())
	{
		if (p_thing->get_type() == ::C_THING_ENT_GENERIC)
		{
			if ( ((CEntItem*)p_thing)->get_class_id() == i_class_id)
			{
				//correct class, but is it the correct item id too?
				if (((CEntItem*)p_thing)->get_item_id() == i_id)
				{
					//reset get next for next guy
					cls_tglo.p_thing_list->reset_get_next();
					//found ya.
					return (CEntItem*)p_thing;
				}
				
			}
			
		}
		
	}

	//all done
	return NULL;
}



bool load_objects_for_map(CThingList *p_things, const char st_incoming_name[256])
{

  //load all objects from this file
   
	//first modify the filename
	  char st_name[256];
	  strcpy(st_name, st_incoming_name);
	 change_file_extension(st_name, "obj"); 
	
     if (!exist(st_name))
     {
         char st_hold[256];
         strcpy(st_hold, st_name);
         //um, this must be a mod.  try main dir
         sprintf(st_name, "%s%s", GetExeDir(), st_hold);
     }


//     log_msg("loading items as %s", st_name);
	 
	//first open the file

	 FILE *fp;
	 fp = fopen(st_name, "rb");
	 if (!fp)
	 {
	   log_error("Unable to open %s.",st_name);
	   return false;
	 }

	 
	 int i_type = 0; //type of item to load
	 
	 CEntItem *p_ent = NULL;
	 CEntData * p_item_data = NULL;
	 
	 //loop to load all items
	 while (fread(&i_type, sizeof(int), 1, fp) > 0)
	 {
	   //we loaded something, hopefully it's an int telling us what kind of
	   //item to load
 	   switch(i_type)
	   {
	   case C_THING_ENT_GENERIC:
		   //load this item
		p_ent = new CEntItem();	//an ent item is an entity build/owning an item
		p_things->add_new_thing(p_ent);
		p_item_data = new CEntData();
		p_item_data->load(fp);
		
        p_ent->setup_item_from_cent( p_item_data);
	  //since we loaded it, let's assume we'll want to save it too when the time comes
        p_ent->SetSave(true);
        //preload it
		if (cls_tglo.IsRunning())
		p_ent->dx_create();
	   break;

	   default:
		   log_error("Don't know how to handle item %d, skipping it.",i_type);
	   }

	 }


	 //all done, close the file handle
	 fclose(fp);
	 return true;


}
