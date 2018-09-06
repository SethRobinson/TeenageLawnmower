/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CEntPlayer.h"
#include "CThing_Util.h"


#include "CManager.h"
#include "CMap.h" 
#include "CCamera.h"
#include "..\\CInput.h"
#include "CTerrain.h"


#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEntPlayer::CEntPlayer()
{

	BuildRadius(2);

    //this->init_visual("model\\ent\\tarzan.rtm", ::C_RESOURCE_RTM);
	//this->SetAnimTexture("model\\ent\\tarzan_skin2.dds");

 //   this->init_visual("model\\ent\\deko.rtm", ::C_RESOURCE_RTM);
//	this->SetAnimTexture("model\\ent\\deko_skin3.dds");
	//fix rotation problem

	D3DXMatrixRotationY( &m_mat_base, D3DX_PI);
	D3DXMATRIX p_mat;
	D3DXMatrixRotationX( &p_mat, D3DX_PI/2);
	D3DXMatrixMultiply(&m_mat_base,  &m_mat_base, &p_mat);


	set_desired_velocity(0);
   	setObjectPosition(D3DXVECTOR3(frandom(cls_tglo.p_map->get_size_x()*cls_tglo.p_map->get_block_size()),
		10,frandom(cls_tglo.p_map->get_size_y()*cls_tglo.p_map->get_block_size())));
	this->set_follow_ground_tilt(true); //rotate to match the ground angle
   f_bounce_power = 0.20f;
   f_traction = 0.2f;
   f_ground_friction = 0.008f;
   b_follow_ground_tilt = false;
   set_thing_name("Player");
 	m_f_max_speed = 6;
   
   m_i_view = 0; //standard 1st person view
    
   f_accel = 0.5f;	
   f_dir_lerp = 0.40f;  //instant
   this->set_tilt_lerp(0.40f); //extra smooth because a camera is attached
   this->set_type(C_THING_CONTROL);
 
     f_tilt_goal = 0.1f;
   fforce_range(&f_tilt_goal, -1, 1);

   set_tilt_rad(f_tilt_goal);

   SetPlayerView();
   
   this->SetAnimByIndex(0, 0); //start the run animation
   this->SetAnimPause(true);  
   
 
}

CEntPlayer::~CEntPlayer()
{
 	
	if (cls_tglo.p_camera)
	{
	  cls_tglo.p_camera->set_visual_follow(NULL); //no longer follow us, we're done
	}
}


void CEntPlayer::SetPlayerView()
{
   
	cls_tglo.p_camera->set_visual_follow(this); //camera will follow us
    
	CThing *p_was_in_follow_mode = cls_tglo.p_camera->get_visual_follow();

	cls_tglo.p_camera->SetVisualLook(this); //follow us

	if (m_i_view == 0)
	{
		cls_tglo.p_camera->set_visual_follow_offset(D3DXVECTOR3(-0.2f,3,0));
   cls_tglo.p_camera->SetVisualLook(false); //follow us
		this->b_no_render_next_frame = true;
	   if (p_was_in_follow_mode)
	   {
		   //set a waiting period befre switching from third person view
	   		cls_tglo.p_camera->set_third_person_view(true);
   			cls_tglo.p_camera->TurnOffThirdPersonViewByTimer(200);
	   } else
	   {
		   		cls_tglo.p_camera->set_third_person_view(false);
   
	   }
 	}

	if (m_i_view == 1)
	{
		
		cls_tglo.p_camera->set_visual_follow_offset(D3DXVECTOR3(-1,2,0.03f));
		cls_tglo.p_camera->set_third_person_view(true);
	    cls_tglo.p_camera->SetVisualLook(false); //follow us
	
		//numbers added to current ground height of course
		m_f_camera_height_max = 2;
		m_f_camera_height_min = 3;

		this->b_no_render_next_frame = false;
	}
	
 
 if (m_i_view == 2)
	{
		cls_tglo.p_camera->set_visual_follow_offset(D3DXVECTOR3(-10,7,0.20f));
		cls_tglo.p_camera->set_third_person_view(true);
		this->b_no_render_next_frame = false;
		m_f_camera_height_max = 20;
		m_f_camera_height_min = 1;
	}
	
		if (m_i_view == 3)
	{
		cls_tglo.p_camera->set_visual_follow_offset(D3DXVECTOR3(-40,25,0.60f));
		cls_tglo.p_camera->set_third_person_view(true);
		this->b_no_render_next_frame = false;
		m_f_camera_height_max = 50;
	    m_f_camera_height_min = 1;
	}

       float f_base_sense = 6; //what can be considered "normal distance speed"
		m_f_camera_mouse_sensitivity =  (m_f_camera_height_max - m_f_camera_height_min)/f_base_sense;
}

void CEntPlayer::think()
{
  if (GetPauseLevel() < cls_tglo.GetPauseLevel()) return; //game paused for us
	//apply mouse movement

    float f_mouse_sens_mod_h = 1.0;
    float f_mouse_sens_mod_v = 1.0;
    float f_strafe_power = 0.50f;
  
    float f_mouse_sensitivity_h = 150.0f * f_mouse_sens_mod_h;
    float f_mouse_sensitivity_v = 150.0f * f_mouse_sens_mod_v;

	
	//change views?
	if (cls_tglo.p_cls_input->is_key_tapped(DIK_TAB))
	{
		m_i_view++;
		if (m_i_view == 4) m_i_view = 0;
		SetPlayerView();
	}

	if (cls_tglo.p_cls_input->get_mouse_change_x() != 0)
	{
		this->set_direction_rad(f_dir_goal+(float(cls_tglo.p_cls_input->get_mouse_change_x())/f_mouse_sensitivity_h));
	}
	
 if (b_no_render_next_frame)
 {
 
	if (cls_tglo.p_cls_input->get_mouse_change_y() != 0)
	{
		set_tilt_rad(f_tilt_goal + (float(cls_tglo.p_cls_input->get_mouse_change_y())/f_mouse_sensitivity_v));
	
//	 char st_crap[100];
//	 sprintf(st_crap, "Tilt is %f, goal is %f.",f_tilt, f_tilt_goal);
// log_msg(st_crap);
		fforce_range(&f_tilt_goal, -1, 1);
	}
	
 } else
 {
   //camera is in 3rd person view, mouse tilts should affect camera, not the player anymore.
   f_tilt = f_tilt_goal =0;
   cls_tglo.p_camera->vec_follow_offset.y += ((float(cls_tglo.p_cls_input->get_mouse_change_y())/f_mouse_sensitivity_v))
	     * m_f_camera_mouse_sensitivity;
  fforce_range(&cls_tglo.p_camera->vec_follow_offset.y, m_f_camera_height_min, m_f_camera_height_max);

 }
	if (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_JUMP))
	{
	 	 
		 //only jump if we're on the ground
		if (get_pos_y() < f_ground_height+0.01f)
		{
 
		 this->vec_vel.y += cls_tglo.time()*10;
		}
	}

	bool b_moved = false;

	if (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_FORWARD))
	{
	  set_desired_velocity(m_f_max_speed);
	  fforce_range(&f_target_vel, -f_max_vel, f_max_vel);
	  b_moved = true;
	}

	if (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_BACKWARD))
	{
	  set_desired_velocity(-m_f_max_speed);
	  fforce_range(&f_target_vel, -f_max_vel, f_max_vel);
	  b_moved = true;
	}

		if (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_LEFT))
	{
	  this->set_direction_rad(f_dir_goal-cls_tglo.time()*3);
	  b_moved = true;
	}

	if (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_RIGHT))
	{
	  b_moved = true;
	  this->set_direction_rad(f_dir_goal+cls_tglo.time()*3);
	}

	if (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_STRAFE_RIGHT))
	{
	
		 D3DXVECTOR3 p_temp;
		 get_x_and_z_from_radian(&p_temp, f_dir+SETH_PI_OVER2);
		 vec_accel.x += p_temp.z *cls_tglo.time()*f_strafe_power; 
		 vec_accel.z += p_temp.x *cls_tglo.time()*f_strafe_power; 
	  b_moved = true;
	}

	if (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_STRAFE_LEFT))
	{
	
		 D3DXVECTOR3 p_temp;
		 get_x_and_z_from_radian(&p_temp, f_dir-SETH_PI_OVER2);
		 vec_accel.x += p_temp.z *cls_tglo.time()*f_strafe_power; 
		 vec_accel.z += p_temp.x *cls_tglo.time()*f_strafe_power; 
	  b_moved = true;
	}

    
    if (!b_moved)
	{
	  //stop movement
		set_desired_velocity(0);
		this->SetAnimByIndex(1, 0); //pause. w e don't have an idle yet so..
        this->SetAnimPause(true);
	}  else
	{
		this->SetAnimByIndex(0, -1); //start the run animation
        this->SetAnimPause(false);
	}

 

	CEnt::think(); //do movement
}

