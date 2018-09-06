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
#include <list>
#include <iostream>
#include "CThing.h"

using namespace std;

typedef list<CThing*> ListCThing;
typedef ListCThing::iterator CThingItor;  
class CMessageHeader; //forward for speed

#include "Spheretree\\spherepack.h"
#include "Spheretree\\pool.h"


class CThingDrawHandler : public CThing
{
public:
    void CThingDrawHandler::RangeTestCallback(const Vector3d<float> &p,float distance,SpherePack *sphere,ViewState state);
    
};

class CThingList
{
    
public:
    
    bool b_deleting_list; //true when deleting the whole list
    
    CThingList();
    ~CThingList();
    int add_new_thing(CThing *cls_new_thing); //add new thing to the list and return it's ID
    void think();
    void dx_draw();
    void zsort(); //move the list around for painters algorithm
    CThing * get_thing_by_id(int i_request_id);
    int get_sort_of_highest_thing(); 
    void reset_get_next();
    CThing * get_next();
    void dx_restore(); //restore our directx stuff
    void dx_create(); //reload our directx stuff
    void dx_kill(); //di init our directx stuff
    void dx_invalidate(); //invalidate
    int delete_things_by_type(int i_type_to_delete);
    void list_all_things();
    int get_thing_count(int i_type_to_count);
    void broadcast_message(CMessageHeader *p_msg, void * p_custom, int i_type_to_send_to);
    void process_delete_requests();
    CThing * get_thing_by_name(char *p_st_name);
    bool SendMessageByName(CMessageHeader *p_msg, void * p_custom, char *p_name);
    void SetCullingDistance(float f_new){
        m_f_culling_distance = f_new; m_f_culling_distance_sq = m_f_culling_distance * m_f_culling_distance;}
    float GetCullingDistance(){return m_f_culling_distance;}
    float GetCullingDistanceSQ(){return m_f_culling_distance_sq;}
    void delete_list();
   
    ListCThing m_object_list; //all our objects are on this
    SpherePackFactory* GetSphereTree(){return m_p_sphere_tree;};
    
private:
    
    int m_i_things_created;
    bool m_b_reset_next;
    CThingItor m_itor; //for outside sources to go through the list
    SpherePackFactory      *m_p_sphere_tree;
    float m_f_culling_distance;
    float m_f_culling_distance_sq; //squared for speed
    CThingDrawHandler m_draw_handler;
};


