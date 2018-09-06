/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CEntHuman.h"

#include "CManager.h"
#include "CMap.h"
#include "..\\CInput.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEntHuman::CEntHuman()
{
//	this->init_visual("model\\test\\ferarri.x", ::C_RESOURCE_PMESH);

//	this->init_visual_cal_instance("model\\cally\\cally.cfg");
	this->init_visual_cal_instance("model\\skeleton\\skeleton.cfg");
//	this->init_visual_cal_instance("model\\paladin\\paladin.cfg");
//	this->init_visual_cal_instance("model\\zeb\\zeb.cfg");
	
	set_desired_velocity(6);
   	setObjectPosition(D3DXVECTOR3(frandom(cls_tglo.p_map->get_size_x()*cls_tglo.p_map->get_block_size()),
	10,frandom(cls_tglo.p_map->get_size_y()*cls_tglo.p_map->get_block_size())));
	
	//
	//setObjectPosition(D3DXVECTOR3(17,0,60));
	
	this->set_follow_ground_tilt(false); //rotate to match the ground angle
  	this->set_direction_rad(D3DX_PI);
	f_bounce_power = 0.25f;
   f_traction = 0.50f;
   f_ground_friction = 0.002f;
  this->set_scale(0.2f, 0.2f, 0.2f);
   set_thing_name("Human");
 
  // this->set_no_lighting(true);
	
}


void CEntHuman::think()
{
	if (cls_tglo.p_cls_input->is_key_tapped(DIK_E))
	{
	  this->vec_vel.y += cls_tglo.time()*10;
	}
	
		if (cls_tglo.p_cls_input->is_key_tapped(DIK_E))
	{
	  set_desired_velocity(10);
	  this->set_direction_rad(f_dir);
	}

	if (cls_tglo.p_cls_input->is_key_tapped(DIK_T))
	{
	  set_desired_velocity(0);
	}
 		if (cls_tglo.p_cls_input->is_key_down(DIK_Q))
	{
	  this->set_direction_rad(f_dir-(cls_tglo.time()*3));
	}

	if (cls_tglo.p_cls_input->is_key_down(DIK_W))
	{
	  this->set_direction_rad(f_dir+(cls_tglo.time()*3));
	}



  
	if (timer_die.get_elapsed_milliseconds() > 30000)
	{
	  this->b_delete_flag = true; //die
	}
  	 
	

	CEnt::think(); //do movement
}



