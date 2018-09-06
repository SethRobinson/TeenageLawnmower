/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#pragma once
#include "CLinkedList.h"
#include "CThing.h"

class CMessageHeader; //forward for speed



class CThingList : public CLinkedList
{
int i_things_created;

//vars for automatting stepping through the list externally
CLinkedList_packet *que_step;
bool b_reset_next;


public:
  CThingList();
  ~CThingList();
  int add_new_thing(CThing *cls_new_thing); //add new thing to the list and return it's ID
  void think();
  void dx_draw();
  void zsort(); //move the list around for painters algorithm
  CThing * get_thing_by_id(int i_request_id);
  void CThingList::insert_with_zsort(CLinkedList_packet *que_new);
int CThingList::get_sort_of_highest_thing(); 
void CThingList::reset_get_next();
CThing * CThingList::get_next();
 void dx_restore(); //restore our directx stuff
 void dx_create(); //reload our directx stuff
 void dx_kill(); //di init our directx stuff
 void CThingList::dx_invalidate(); //invalidate
 int CThingList::delete_things_by_type(int i_type_to_delete);
 void CThingList::list_all_things();
 int CThingList::get_thing_count(int i_type_to_count);
 void CThingList::broadcast_message(CMessageHeader *p_msg, void * p_custom, int i_type_to_send_to);
 void CThingList::process_delete_requests();
 CThing * CThingList::get_thing_by_name(char *p_st_name);
  bool CThingList::SendMessageByName(CMessageHeader *p_msg, void * p_custom, char *p_name);

};