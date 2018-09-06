/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



//This class holds the base template information for every item, npc and monster.
#pragma once
#pragma warning (disable:4786)

#include <list> //stl linked list
#include "..\\all.h"
#include "CEntPerson.h"
#include "d3dx8math.h" //just for the d3dxvector3 class


using namespace std;

//make things more readable

class CEntData;

typedef list<CEntData*> list_ent;
typedef list_ent::iterator itor_ent;



const int C_ENT_MODE_ITEM = 0;
const int C_ENT_MODE_PERSON = 1;


class CEntData
{
public:
  CEntData();
  ~CEntData();
  int c_ent_mode;  
  unsigned int i_class_id; //based on the filename of the template item we're based on
  int i_id; //id of the template we're based on	 (basically array index)
  char st_name[40];
  D3DXVECTOR3 vec_pos;
  D3DXVECTOR3 vec_scale; //modifier
  float f_dir; //way we're facing

  // CEntItem item; //non null if an item
  CEntPerson *p_person; //non null if a person/NPC/monster
  list_ent * p_list_attached; //items can be attached to this guy
  int i_attached; //how many items are attached
  bool b_floating; //if yes, we are not restricted to  the ground
  
  bool CEntData::save(FILE *fp);
  bool CEntData::load(FILE *fp);
  void CEntData::kill();
  bool CEntData::attach_entity(CEntData *p_ent);
  CEntData * CEntData::get_all_attached_entities();
  bool CEntData::clone(CEntData **p_ent);
  bool remove_entity(int i_index_in_linked_list);

};

