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
#include "CThing_globals.h"

//holds our array of all our sub zone info and helps us manipulate them

#include "CSubZone.h"

class CSubManager
{
public:
	CSubManager();

    ~CSubManager();
   void CSubManager::save(FILE *fp);

  int get_count(){return i_count;};
    CSubZone *p_a_sub_zone; //our sub zone array to hold them all
	int add_sub_zone(); //returns the index of the new sub zone
   	void CSubManager::load(FILE *fp);
   int CSubManager::count_extra_tiles_actually_used();
   int GetSubZoneByName(char *st_name);
private:
  int i_count; //how many sub zones exist
  int i_size_of_csub_zone;
  int i_extra_tile_count;
  int i_extra_data_size;
  int CSubManager::count_extra_tiles_used_in_zone(int i_zone);

};