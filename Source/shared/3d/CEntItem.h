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

#include "CEnt.h"
#include "..\\CTimer.h"



class CEntData;
class CBaseEntData;

class CEntItem : public CEnt
{
public:
	CEntData * get_entity_pointer(){return p_ent;};
  CEntItem();
  virtual void think();
  unsigned int CEntItem::get_class_id();
  void CEntItem::setup_item_from_cent(CEntData *p_new_ent);
  virtual ~CEntItem();
  virtual void receive_message(CMessageHeader *p_header, void *p_message);
  int CEntItem::get_item_id();
  D3DXVECTOR3 CEntItem::GetRelativeScale();
  void CEntItem::update_ent_data();
  void CEntItem::SetRelativeScale(D3DXVECTOR3 vec_scale);
  void CEntItem::SetRelativeScaleUniform(float f_scale);
  void SetOnHitScript(bool b_on_hit_script){m_b_on_hit_script = b_on_hit_script;}
  void SetOnTouchScript(bool b_on_touch_script){m_b_on_touch_script = b_on_touch_script;}
  void SetOnTouchWallScript(bool b_on_wall_touch_script){m_b_on_wall_touch_script = b_on_wall_touch_script;}
  void SetOnTouchGroundScript(bool b_on_ground_touch_script){m_b_on_ground_touch_script = b_on_ground_touch_script;}
  void SetOnTouchCeilingScript(bool b_on_ceiling_touch_script){m_b_on_ceiling_touch_script = b_on_ceiling_touch_script;}
  void SetFloatingMode(bool b_new);
  int CEntItem::InitScript(char *st_script_file);
  void CEntItem::RunScriptFunction(char *p_st_func_name);
  void CEntItem::SetSave(bool b_new){m_b_save = b_new;} //true to save with items



private:

void CEntItem::ShutDownScript();
    
    int m_i_script; //if != C_SCRIPT_NONE there is a valid script attached to this entity
  int i_alpha_sort;
  bool m_b_run_script;
  bool m_b_on_hit_script; //if true we'll run the on hit handler when needed
  bool m_b_on_touch_script; //if true we'll run the on hit handler when needed
  bool m_b_on_wall_touch_script; //if true we'll run the on hit handler when needed
  bool m_b_on_ground_touch_script; //if true we'll run the on hit handler when needed
  bool m_b_on_ceiling_touch_script; //if true we'll run the on hit handler when needed


  
  CTimer timer_die;
 CEntData *p_ent; //remember the data struct that made us
 bool m_b_save; //should we save to disk when told to or is this ent just crap
 CBaseEntData *p_base; //what we're based on
 D3DXVECTOR3 vec_original_base_size; //if the base size changes dynamically, we need this
 //to properly convert our size to the new one.
 //that should disappear?

};