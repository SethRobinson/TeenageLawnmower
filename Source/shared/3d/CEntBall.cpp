/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CEntBall.h"

#include "CManager.h"
#include "CMap.h"
#include "..\\CInput.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEntBall::CEntBall()
{
	 this->init_visual_sphere("Little Sphere", 1,1,1,20,20,4, FULL_SPHERE,
	D3DCOLOR_XRGB( 255, 255, 255 ), "texture\\skydome\\skydome6.dds");
//	 this->init_visual_sphere("Little Sphere", 0.25f,0.25f,0.25f,20,20,4, FULL_SPHERE,
//	D3DCOLOR_XRGB( 255, 255, 255 ));
	set_desired_velocity(0.0f);
	//let's also place it in a random position
  	setObjectPosition(D3DXVECTOR3(frandom(cls_tglo.p_map->get_size_x()*cls_tglo.p_map->get_block_size()),
		10,frandom(cls_tglo.p_map->get_size_y()*cls_tglo.p_map->get_block_size())));
   b_roll = true;
   set_follow_ground_tilt(false);
  set_direction_rad(0); 
  f_ground_friction = 0.0006f; //very low friction
   f_bounce_power = 0.60f;  
  set_thing_name("Ball");
  set_tilt_lerp(1.0); //no tilt interpolation
}


void CEntBall::think()
{
	if (cls_tglo.p_cls_input->is_key_tapped(DIK_E))
	{
	  this->vec_vel.y += cls_tglo.time()*10;
	}

	
	if (timer_die.get_elapsed_milliseconds() > 30000)
	{
	  this->b_delete_flag = true; //die
	}
  
	CEnt::think(); //do movement
}

