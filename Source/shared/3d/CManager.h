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
#pragma warning (disable:4786)

#include "CThing.h"
#include "CResource.h"
#include "CThing_globals.h"
#include "CResourceTex.h"
#include "CResourcePMesh.h"
#include "CResource3DS.h"
#include "CResourceSphere.h"
#include "CResourceSprite.h"

#ifdef _CAL3D
#include "CResourceCalCore.h"
#include "CResourceCalRender.h"
#endif

//#include "CResourceAnim.h" //both MD2 and RTM loading are based on this

#include <map>
#include <algorithm>

			 
typedef std::map<int, CResource*> map_manager;
typedef map_manager::value_type val_manager;
typedef map_manager::iterator itor_manager;

//the resource class can handle resources two ways - preset ID's and dynamically
//created ID's such as a texture that will get loaded automatically by a model.
//If two DIFFERENT models use the same texture, this system will only load ONE
//texture.  (if the filenames are the same)

//id's 1 through 9999 are for me to use, after that the engine can use it as he
//wishes.

const int C_MANAGER_DYNAMIC_START_ID = 10000;


class CManager : public CThing
{
  int i_dynamic_id_counter; //so we know where to put dynamic resources
  map_manager map;
  itor_manager scan_itor;
  bool b_scan_active;
  bool b_deleting_all;

public:
 
void CManager::add_resource_by_string(int i_index, int i_resource_type, int i_resource_sub_type,  char st_resource[256]);
   int CManager::get_dynamic_resource_by_string(int i_resource_type, char st_resource[256]);
	int CManager::find_resource_by_type_and_string(int i_resource_type, char st_resource[256]);
	int CManager::delete_ref(int i_index);
	int CManager::add_ref(int i_index);
   bool CManager::load_resource_list(char *p_st_file);
   bool CManager::scan_resources(const int i_type_to_scan, const int i_sub_type_to_scan, char p_st_name[], int *p_i_id);
bool CManager::load_if_needed(int i_index);
bool CManager::does_resource_exist(int i_id);
void CManager::UnloadAllNonDynamic(int i_type_to_unload);
void CManager::DeleteAllNonDynamic(int i_type_to_unload);

//	CResource * get(int i_index);
   
  CResource * get(int i_index){return map[i_index];};
  char * get_string(int i_index);
  void CManager::list_contents();
  void CManager::delete_all();
  CManager();
  virtual ~CManager();
  virtual void dx_create();
  virtual void dx_invalidate();
  virtual void dx_restore();
  virtual void dx_kill();
  virtual void think();
};