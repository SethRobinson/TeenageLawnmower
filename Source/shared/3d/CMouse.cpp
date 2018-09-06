/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CMouse.h"
#include "CCamera.h"
#include "CThing_util.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMouse::CMouse()
{
  	//if we don't change this it's going to be assumed it's a Cpic
	
	//init mouse stuff
  this->set_thing_name("Mouse");
  this->set_sprite_layer(100); //insure it is run first  
  m_i_click_delay = 500;
  m_b_wait = false;

  //move mouse to last known position
  assert(cls_tglo.p_cls_input && "Shouldn't be initted already?");
  set_pos_x(float(cls_tglo.p_cls_input->GetMouseX()));
  set_pos_y(float(cls_tglo.p_cls_input->GetMouseY()));
}

CMouse::~CMouse()
{
  //deconstruct

}

void CMouse::ResetClickTimer()
{
	m_b_wait = true;
	m_timer_delay.set_interval(m_i_click_delay);
}

        

void CMouse::dx_restore()
{
    set_no_draw(false);
 
    CPic::dx_restore();
}

void CMouse::think()
{
    
   
    if (!GetApp()->p_d3dapp->IsWindowed())
    {
        //limit mouse to screen coordinates
        fforce_range(&m_vObjectPosition.x, 0, float(cls_tglo.get_screen_width()));
        fforce_range(&m_vObjectPosition.y, 0, float(cls_tglo.get_screen_height()));
        set_no_draw(false);
     
    } else
    {
        set_no_draw(true);
     }
	//update position
	
    float i_hold_x = get_pos_x();
    float i_hold_y = get_pos_y();

    if (cls_tglo.p_cls_input->GetMouseExclusive())
    {
     i_hold_x += cls_tglo.p_cls_input->get_mouse_change_x(); 
     i_hold_y += cls_tglo.p_cls_input->get_mouse_change_y(); 
    } else
    {
        i_hold_x = cls_tglo.p_cls_input->GetMouseX();
        i_hold_y = cls_tglo.p_cls_input->GetMouseY();
    }
    
   if (cls_tglo.GetMouseControlledByJoystick())
   {
   
     //allow joystick to also control arrow

    if (cls_tglo.p_cls_input->GetAnyJoystickDir(C_INPUT_JOY_UP))
    {
        cls_tglo.p_cls_input->SetVirtualKey(C_INPUT_FORWARD, 1);
    }

    if (cls_tglo.p_cls_input->GetAnyJoystickDir(C_INPUT_JOY_DOWN))
    {
        cls_tglo.p_cls_input->SetVirtualKey(C_INPUT_BACKWARD, 1);
    }


    if (cls_tglo.p_cls_input->GetAnyJoystickDir(C_INPUT_JOY_LEFT))
    {
        cls_tglo.p_cls_input->SetVirtualKey(C_INPUT_LEFT, 1);
    }

    if (cls_tglo.p_cls_input->GetAnyJoystickDir(C_INPUT_JOY_RIGHT))
    {
        cls_tglo.p_cls_input->SetVirtualKey(C_INPUT_RIGHT, 1);
    }

 

        float f_joy_speed = 270.0f;
        if (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_RIGHT))
        {
        
            i_hold_x += cls_tglo.time()*f_joy_speed;
           

        }
        if (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_LEFT))
        {
          i_hold_x -= cls_tglo.time()*f_joy_speed;
        }
   
        
         if 
             (
             (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_BACKWARD))
             || 
             (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_BACKWARD_SECONDARY) )
         )
         {
          i_hold_y += cls_tglo.time()*f_joy_speed;
        }

         if (
             (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_FORWARD))
             || (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_FORWARD_SECONDARY))

             )
        {
          i_hold_y -= cls_tglo.time()*f_joy_speed;
        }
       }

   
        
        set_xy(i_hold_x, i_hold_y);

        if (!cls_tglo.p_cls_input->GetMouseExclusive())
        {
            //SetCursorPos(m_vObjectPosition.x, m_vObjectPosition.y);
        }

        
        /*	
   	if (m_b_wait)
	{
		if (m_timer_delay.interval_reached())
		{
			m_b_wait = false; //click away, guys!
		} else
		{
			//ignore hold down clicks for now
		
		}
	}
	
  	if (!m_b_wait)
	{
		//convert hold downs into clicks if we need to
		if (cls_tglo.p_cls_input->is_mouse_down(C_MOUSE_LEFT))
		{
			cls_tglo.p_cls_input->set_mouse_tapped(C_MOUSE_LEFT);
			ResetClickTimer();
		    log_msg("Forced click.");    
		}
	}
	 */


	//was a mouse button clicked?
	if (
        (cls_tglo.p_cls_input->is_mouse_tapped(::C_MOUSE_LEFT))
         || (cls_tglo.p_cls_input->is_key_tapped(DIK_RETURN))
       ||(cls_tglo.p_cls_input->is_key_tapped(DIK_RCONTROL))
       ||(cls_tglo.p_cls_input->is_key_tapped(DIK_SPACE))
       ||(
       
     cls_tglo.GetMouseControlledByJoystick() &&
     cls_tglo.p_cls_input->GetAnyJoystickButtonTapped(0)
       
       
       )
        )
	{
//		log_msg("Processing click.");
		//send a left click message
        D3DXVECTOR3 vec_point_a, vec_point_b;
	
	    //get close point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_a, int(get_pos_x()),
		int(get_pos_y()), 0.001f); //close to screen
		
		//get far point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_b, int(get_pos_x()),
		int(get_pos_y()), 1000); //close to screen

		//the last zero is to say "no minimum distance required"
		CThing *p_thing;
	
		float f_dist = 0; //return
		
		D3DXVECTOR2 vec_mouse = D3DXVECTOR2(get_pos_x(), get_pos_y());
	   // log_msg("Doing raypic.");
		if (p_thing = ray_pick_objects(p_my_thing_list, &vec_point_a, &vec_point_b, -1, &f_dist, 0, NULL, &vec_mouse))
		{
			//the -1 means pick all
		    
			 log_msg("Clicked thing.");
		}


	}
 

//in any case, update the global mouse position pointer
    cls_tglo.p_cls_input->SetMouseX(int(get_pos_x()));
    cls_tglo.p_cls_input->SetMouseY(int(get_pos_y()));
    
	CPic::think();
}