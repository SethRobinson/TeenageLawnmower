/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CControlPlayer.h"
#include "..\\CInput.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CControlPlayer::CControlPlayer()
{
  this->set_sort(-1000); //make sure we get processed early, right after the CCamera object
  this->set_type(C_THING_CONTROL);
  set_thing_name("Player Control");
}


void CControlPlayer::think()
{
 
 //if (!cls_tglo.p_d3dapp->m_bHasFocus) return;
 if (!cls_tglo.p_camera)
 {
   log_error("Camera not initted yet, can't move!");
   return;
 }

 FLOAT fElapsedTime;

    if( cls_tglo.time() > 0.0f )
        fElapsedTime = cls_tglo.time();
    else
        fElapsedTime = 0.05f;

    FLOAT fSpeed        = 10.0f*fElapsedTime;
    FLOAT fAngularSpeed = 3.0f*fElapsedTime;

    // De-accelerate the camera movement (for smooth motion)
    cls_tglo.p_camera->decelerate( 0.9f );

    // Process keyboard input
    if( cls_tglo.p_cls_input->is_key_down(DIK_D) )    cls_tglo.p_camera->slideRight( fSpeed );
    if( cls_tglo.p_cls_input->is_key_down(DIK_S) )     cls_tglo.p_camera->slideLeft( fSpeed );
    if( cls_tglo.p_cls_input->is_key_down(DIK_SPACE) )      cls_tglo.p_camera->slideUp( fSpeed );
    if( cls_tglo.p_cls_input->is_key_down(DIK_R) ) cls_tglo.p_camera->slideDown( fSpeed );
    if( cls_tglo.p_cls_input->is_key_down(DIK_F) )       cls_tglo.p_camera->moveForward( fSpeed );
    if( cls_tglo.p_cls_input->is_key_down(DIK_V) )     cls_tglo.p_camera->moveBackward( fSpeed );
    if( cls_tglo.p_cls_input->is_key_down(DIK_RIGHT) )    cls_tglo.p_camera->turnRight( fSpeed );
    if( cls_tglo.p_cls_input->is_key_down(DIK_LEFT) )     cls_tglo.p_camera->turnLeft( fSpeed );
    if( cls_tglo.p_cls_input->is_key_down(DIK_DOWN) )      cls_tglo.p_camera->turnDown( fSpeed );
    if( cls_tglo.p_cls_input->is_key_down(DIK_UP) )     cls_tglo.p_camera->turnUp( fSpeed );

    //stick us on the ground
  
	if (cls_tglo.p_terrain)
	{
		cls_tglo.p_camera->set_pos_y(2.5f+cls_tglo.p_terrain->get_height(
			cls_tglo.p_camera->get_pos_x(), cls_tglo.p_camera->get_pos_z() ));
	}
	
	// transform the updated camera information.
  	cls_tglo.p_camera->transform( fSpeed, fAngularSpeed );

}