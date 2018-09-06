/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



//derived from CLinkedList, look ma, I'm subclassing!
//this class keeps track of all the "things" in the game. This could be a 
//tree, a deer or a menu element.  Things can be created and destroyed.

#include "CThingList.h"
#include "CPic.h"

CThingList::CThingList()
{
  i_things_created = 0;
  que_step = NULL;
  b_reset_next = true;
}

CThingList::~CThingList()
{
  

}

void CThingList::reset_get_next()
{
	b_reset_next = true;
}

CThing * CThingList::get_next()
{
  if (b_reset_next) 
  {
	  this->get_reference_to_first(&que_step);
	  b_reset_next = false;
  }
  else
  que_step = que_step->next;
 
  if (!que_step) 
  {
	  reset_get_next();
      return NULL;
  }
  return que_step->p_cls_thing;
}


//returns things deleted
int CThingList::delete_things_by_type(int i_type_to_delete)
{
	CThing *p_temp;
   int i_things_deleted = 0;
	while (p_temp = get_next())
	{
		if (p_temp->get_type() == i_type_to_delete)
		{
			p_temp->b_delete_flag = true;
		   i_things_deleted++;
		}
	}
  return i_things_deleted;
}

//returns things deleted
//if -1, count all
int CThingList::get_thing_count(int i_type_to_count)
{
	CThing *p_temp;
   int i_things_counted = 0;
	while (p_temp = get_next())
	{
		if (p_temp->b_delete_flag == false) //don't want to count people who are about to get deleted
		if (p_temp->get_type() == i_type_to_count)
		{
		   i_things_counted++;
		}
	}
  return i_things_counted;
}

int CThingList::add_new_thing(CThing *cls_new_thing)
{
  //int id
  i_things_created++;
	
  CLinkedList_packet *que_temp = new CLinkedList_packet;
  if (!que_temp)
  {
	log_error("Error creating new thing.");
	return -1;
  }
  //new node added.  Assign it this thing.
  que_temp->p_cls_thing = cls_new_thing; //all done.
  que_temp->p_cls_thing->set_id(i_things_created);
  que_temp->p_cls_thing->set_my_thing_list(this);

  //insert packet into list, while sorting with the CThing's i_zsort.

  insert_with_zsort(que_temp);

  return i_things_created;
}

void CThingList::insert_with_zsort(CLinkedList_packet *que_new)
{
  //ok, let's figure out where to put this in.  If an object has a "b_needs_sorting" flag,
  //we can safely ignore it because it will be sorted later.

   CLinkedList_packet *p_packet = NULL;
   CLinkedList_packet *p_parent_packet = NULL;
   get_reference_to_first(&p_packet);
   
   //new packet will be sorted, kill it's sort request if it had one
   que_new->p_cls_thing->b_sort_request = false;
   //makes sure this new packet doesn't bring any friends
   que_new->next = NULL;
   //special case for if none exist at all
   if (p_packet == NULL)
   {

	 first = que_new;
	 //Msg("Sort: Created list");
	 return;
   }

   while (p_packet)
   {
	   
	   if ( (p_packet->p_cls_thing->get_sort() >= que_new->p_cls_thing->get_sort()) && (p_packet->p_cls_thing->b_sort_request == false) )
	   {
		   //we should be inserted before this guy.
		   /*
		   if (p_parent_packet && p_packet->p_cls_thing->get_type() == C_THING_CPIC)
		   {
		   
		   log_msg("Ok, %d is higher than %d (%s), go back to after %d.", p_packet->p_cls_thing->get_sort(),
			 que_new->p_cls_thing->get_sort(), ((CPic*)que_new->p_cls_thing)->st_filename, p_parent_packet->p_cls_thing->get_sort());
		   
		   }

  */
			  if (!p_parent_packet)
		   {
			 //special case, we're inserting before the first guy
			 first = que_new;
			 que_new->next = p_packet;
	//		 Msg("Sort:  Put a guy (%s) (%d) in the front",  que_new->p_cls_thing->get_thing_name(), que_new->p_cls_thing->get_sort());
			 return;
		   }
	
	   	   //insert before a guy somewhere in the middle of the last
		   p_parent_packet->next = que_new;
		   que_new->next = p_packet;
	//	   Msg("Sort: Added a guy (%s) (%d)somewhere in the middle of the list", que_new->p_cls_thing->get_thing_name(),  que_new->p_cls_thing->get_sort());
		   return;

	   }
	   
	   p_parent_packet = p_packet; //remember parent
	   p_packet = p_packet->next;
   }

   //well, we've gone through the list and nothing has been inserted so I guess we're at
   //the end.

   p_parent_packet->next = que_new;
//   Msg("Sort: Guy (%s) (%d) added to end.", que_new->p_cls_thing->get_thing_name(),  que_new->p_cls_thing->get_sort());

}

void CThingList::think()
{
   CLinkedList_packet *p_packet;

    //done thinking? Let's delete those who need it.
   get_reference_to_first(&p_packet);

   try_again:
 
   while (p_packet)
   {

   	 //does this need to be deleted from the list?
	 if (p_packet->p_cls_thing->b_delete_flag)
	 {
	   //delete it
	  	 CLinkedList_packet *p_temp = p_packet->next;
		 remove_node(p_packet);
		 p_packet = p_temp;
		 goto try_again;

	 }
	   
	 p_packet = p_packet->next;
   }

   
   get_reference_to_first(&p_packet);


   while (p_packet)
   {

	 p_packet->p_cls_thing->think();
	 p_packet = p_packet->next;
   }


  


}


//this is also done in the thing() function, so you should only call
//this if you want to delete without thinking.  Like, to erase stuff right
//before you load a new map.

void CThingList::process_delete_requests()
{
   CLinkedList_packet *p_packet;

    //done thinking? Let's delete those who need it.
   get_reference_to_first(&p_packet);

   try_again:
 
   while (p_packet)
   {

   	 //does this need to be deleted from the list?
	 if (p_packet->p_cls_thing->b_delete_flag)
	 {
	   //delete it
	  	 CLinkedList_packet *p_temp = p_packet->next;
		 remove_node(p_packet);
		 p_packet = p_temp;
		 goto try_again;

	 }
	   
	 p_packet = p_packet->next;
   }

}


void CThingList::dx_draw()
{
   CLinkedList_packet *p_packet;
   get_reference_to_first(&p_packet);

   while (p_packet)
   {
	 p_packet->p_cls_thing->dx_draw();
	 p_packet = p_packet->next;
   }


}


void CThingList::dx_restore()
{
   CLinkedList_packet *p_packet;
   get_reference_to_first(&p_packet);

   while (p_packet)
   {
	 p_packet->p_cls_thing->dx_restore();
	 p_packet = p_packet->next;
   }


}

void CThingList::dx_create()
{
   CLinkedList_packet *p_packet;
   get_reference_to_first(&p_packet);

   while (p_packet)
   {
	 p_packet->p_cls_thing->dx_create();
	 p_packet = p_packet->next;
   }


}


void CThingList::dx_kill()
{
   CLinkedList_packet *p_packet;
   get_reference_to_first(&p_packet);

   while (p_packet)
   {
	 p_packet->p_cls_thing->dx_kill();
	 p_packet = p_packet->next;
   }


}

 void CThingList::dx_invalidate()
{
   CLinkedList_packet *p_packet;
   get_reference_to_first(&p_packet);

   while (p_packet)
   {
	 p_packet->p_cls_thing->dx_invalidate();
	 p_packet = p_packet->next;
   }


}

int CThingList::get_sort_of_highest_thing()
{

   int i_sort = -1;
   CLinkedList_packet *p_packet;
   get_reference_to_first(&p_packet);

   //get the very last thing in the list 
   	   while (p_packet)
	   {
		   if (p_packet->p_cls_thing->get_sort() > i_sort)
			i_sort = p_packet->p_cls_thing->get_sort();
		   p_packet = p_packet->next;
	   }

    return i_sort;
}



void CThingList::zsort()
{
  //sort things out
  CLinkedList_packet *p_packet = NULL;
  CLinkedList_packet *p_parent_packet = NULL;
  get_reference_to_first(&p_packet);
  
   while (p_packet)
   {
	   if (p_packet->p_cls_thing->requires_sort())
	   {
		 //remove from list then put him back in a sorted way
		   if (!p_parent_packet)
		   {
			 //special case, this was the first in the list
			 first = p_packet->next;
    		 insert_with_zsort(p_packet);
  		     p_parent_packet = NULL;
			 get_reference_to_first(&p_packet);
			 continue;
		   } else
		   {
			   p_parent_packet->next = p_packet->next;
		   }

	    //it's been removed.  Put it back sorted.
		   insert_with_zsort(p_packet);
		   //go back to where we were
		   
		   p_packet = p_parent_packet;
		   //go back to the top of the while
		   continue;
	   }
	   
	   p_parent_packet = p_packet;
	   p_packet = p_packet->next;
   }

  //done with sort

}

CThing * CThingList::get_thing_by_id(int i_request_id)
{
   CLinkedList_packet *p_packet;
   get_reference_to_first(&p_packet);

   while (p_packet)
   {
	
	   if (p_packet->p_cls_thing->get_id() == i_request_id)
	   {
		 //this is the guy we want.
		 return p_packet->p_cls_thing;
	   }
	   
	   p_packet = p_packet->next;
   }
  //couldn't find 'em.

   return NULL;

}

 CThing * CThingList::get_thing_by_name(char *p_st_name)
{
   CLinkedList_packet *p_packet;
   get_reference_to_first(&p_packet);

   while (p_packet)
   {
	
	   if (stricmp(p_packet->p_cls_thing->get_thing_name(), p_st_name) == 0)
	   {
		 //this is the guy we want.
		 return p_packet->p_cls_thing;
	   }
	   
	   p_packet = p_packet->next;
   }
  //couldn't find 'em.

   return NULL;

}

void CThingList::list_all_things()
{
   char st_name[32];	
   char st_final[255];
   CLinkedList_packet *p_packet;
   get_reference_to_first(&p_packet);

   while (p_packet)
   {
		st_name[0] = 0;
		strcpy(st_name, p_packet->p_cls_thing->get_thing_name());
		if (st_name[0] == 0)
		{
		 strcpy(st_name, "Unknown");
		}
	    sprintf(st_final, "%s at sort level %d",st_name,
			p_packet->p_cls_thing->get_sort());
		log_msg(st_final);
    p_packet = p_packet->next;

   }

}


//send a message to everybody or a group of things.  Sending -1 as the type to
//send to means send to everybody.  p_custom is a custom struct pointer.
void CThingList::broadcast_message(CMessageHeader *p_msg, void * p_custom, int i_type_to_send_to)
{
		
   CLinkedList_packet *p_packet;
   get_reference_to_first(&p_packet);

   while (p_packet)
	{
	if ( (i_type_to_send_to == -1) || (i_type_to_send_to == p_packet->p_cls_thing->get_type()) )
	   p_packet->p_cls_thing->receive_message(p_msg, p_custom);
	 p_packet = p_packet->next;

	}

}


bool CThingList::SendMessageByName(CMessageHeader *p_msg, void * p_custom, char *p_name)
{
   CThing *p_thing = this->get_thing_by_name(p_name);
   if (p_thing)
   {
       p_thing->receive_message(p_msg, p_custom);
       return true;
   }   else
   {
       log_msg("Unable to send message to thing %s.", p_name);
   }

   return false; //couldn't send it
}

