/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



//All base entities are stored in libs
#pragma once
#pragma warning (disable:4786)


#include <vector>
#include <map>
#include <iostream>
#include "CBaseEntData.h"

using namespace std;

//make things more readable

class CCategory;

typedef vector<CBaseEntData*> vec_base;
typedef vec_base::iterator itor_base;

typedef map<unsigned int, CCategory*> map_base;
typedef map_base::value_type val_base;
typedef map_base::iterator itor_map_base;


//info about this category,  the map is of these basically
class CCategory
{
public:
    char st_cat_name[80];
    char st_filename[80];
    int i_ent_count;
    int i_size_of_cbase;
    vec_base *items;
    unsigned ui_class_id;
};


class CEntLib
{
    
    
public:
    int get_ent_id_from_string(unsigned int ui_class_id, char st_item_name[80]);
    int add_item(unsigned int i_class_id, CBaseEntData *p_new_base);	
    void delete_sub_class(unsigned int i_class_id);
    unsigned int add_class(char st_name[80], char st_fname[80]);
    CCategory * get_all_categories();
    CBaseEntData * get_all_ents_in_this_cat(unsigned int ui_class_id);
    CBaseEntData * get_base_item(unsigned int ui_class_id, int i_item);
    void save_category(unsigned int ui_class_id);
    void load_category_wildcard(char st_wildcard[]);
    unsigned int load_category(char st_filename[]);
    unsigned int get_class_id_from_string(char st_fname[80]);
    bool class_exists(unsigned int ui_class);
    char * GetCategoryName(unsigned int ui_class);
    
    CEntLib();
    ~CEntLib();
    void destroy_all();
    map_base base;
    
private:
    itor_map_base scan_itor;
    bool b_scan_active;
    int i_scan_item_temp;
    int m_i_max_categories_demo_can_load;
  
};