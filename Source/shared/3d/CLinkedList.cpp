/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CLinkedList.h"

CLinkedList::CLinkedList()
{
  //init everything
	this->first = NULL;
	b_deleting_list = false;
}

CLinkedList::~CLinkedList()
{
  //delete everything
	//Msg("Deleting list.");
	delete_list();
}


void CLinkedList::delete_list()
{
	b_deleting_list = true;

	if (first) delete first;
	//this will kill the list and it's children, using
	//recursion behavior in the CLinkedList_packet class.
	first = NULL;
	b_deleting_list = false;
	
}

CLinkedList_packet * CLinkedList::add_to_end()
{
	//add a new packet to the end of the list
	
	CLinkedList_packet *que_temp, *que_new;
	que_temp = first;

	//create new packet, fill in values
	que_new = new CLinkedList_packet; //create it
	if (!que_new) return NULL; //couldn't initialize
	//find end of list.  Could keep track of end later for speed, would
	//be easy to add.

	//test for special cases
	if (!que_temp)
	{
	  	  //add first one
	 first = que_new;
	 //Msg("Added first one.");
	 return que_new;
	}

	while (que_temp->next)
	que_temp = que_temp->next;

	//ok, we're here, add the new guy.

	que_temp->next = que_new;
//	log_msg("Added one.");

  return que_new;
}

bool CLinkedList::get_reference_to_first(CLinkedList_packet **que_temp)
{
 if (!first) 
 {
  	 *que_temp = NULL;
	 return false;
 }
 //return reference to the first item in the stack
 *que_temp = first;
 return true; 														  
}

bool CLinkedList::delete_first()
{
   //delete the first item in the stack
	if (!first) return false; //can't delete anything, nothing exists
	
	CLinkedList_packet *que_temp = first;
	first = first->next;

	//now actually delete the old first
	que_temp->next = NULL; //don't let the recursive deletion chain kick in
	delete que_temp;

	return true;
}

bool CLinkedList::remove_node(CLinkedList_packet * p_remove)
{
 	CLinkedList_packet *que_temp = p_remove;
	if (que_temp == first)
	{
	  //first thing on list, special case
	  first = que_temp->next;
	  
	  que_temp->next = NULL; //don't start a crazy chain reaction
	  delete que_temp;
	  //Msg("Remove first thing from list.");
	  return true;
	}

  //we know this packet is not the first, we need to find out who his parent is.

  que_temp = first;

  while (que_temp)
  {
    if (que_temp->next == p_remove)
	{
	  //this is his parent - skip the guy we are deleting
	  que_temp->next = p_remove->next;

	  p_remove->next = NULL; //don't start a crazy chain reaction
	  delete p_remove;
//	  Msg("Removed a guy from the middle of a list");
	  return true;
	}
   //cycle to next object
	que_temp = que_temp->next;
  
  }

 //unable to find parent
  log_msg("Unable to find parent");
  return false;

}

