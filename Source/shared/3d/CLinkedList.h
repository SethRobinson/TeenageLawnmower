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
#include "CThing.h"

//each message contains this
//class CThing;

class CLinkedList_packet
{
friend class CLinkedList;

public:
	
	CLinkedList_packet *next;
	CThing *p_cls_thing;
	
public:
 
	
	
  CLinkedList_packet()
 {
   next = NULL;
   p_cls_thing = NULL;
 }


 ~CLinkedList_packet()
 {
  	 //free memory used
	 //free our children, will recursively free everybody
	if (p_cls_thing) delete p_cls_thing;
    if (next) delete next;
 }


};

//class to keep track of the link list, pop and push things and such
class CLinkedList
{
protected:
 CLinkedList_packet *first; //every linked list needs a first one
 static int i_count;
public:
 CLinkedList(); //construct
 ~CLinkedList(); //deconstruct
CLinkedList_packet * CLinkedList::add_to_end();
void delete_list();
bool get_reference_to_first(CLinkedList_packet **que_temp);
bool delete_first();
bool remove_node(CLinkedList_packet *p_remove);
bool b_deleting_list;


};

