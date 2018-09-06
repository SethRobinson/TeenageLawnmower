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
#include "mbstring.h"

class CEntItem; //foward for speed
class CThingList; //forward for speed

bool save_objects_for_map(CThingList *p_things, const char st_incoming_name[256]);
bool load_objects_for_map(CThingList *p_things, const char st_incoming_name[256]);
void write_type(int i_type, FILE *fp);
bool clear_objects_from_map(CThingList *p_things, int i_type);
void reinit_objects_on_map(CThingList *p_things, int i_class_id);
CEntItem * scan_for_item_type(CThingList *p_things, unsigned int i_class_id, int i_id);
