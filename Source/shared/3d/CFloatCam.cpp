#include "CFloatCam.h"
#include "CThingList.h"
#include "CEnt.h"
#include "..\\vectormath.h"
#include "..\\CInput.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFloatCam::CFloatCam()
{
  this->set_sort(-1000); //make sure we get processed early, right after the CCamera object
  this->set_type(C_THING_CONTROL);
  set_thing_name("CFloatCam");
}


void CFloatCam::think()
{
 
 //if (!cls_tglo.p_d3dapp->m_bHasFocus) return;
 if (!cls_tglo.p_camera)
 {
   log_error("Camera not initted yet, can't move!");
   return;
 }


    FLOAT fSpeed        = 23.0f*cls_tglo.time();
    FLOAT fAngularSpeed = 2.0f*cls_tglo.time();

	if (fSpeed > 2.15f) fSpeed = 2.15f;
	if (fAngularSpeed > 0.05f) fAngularSpeed = 0.05f;

    // De-accelerate the camera movement (for smooth motion)
    cls_tglo.p_camera->decelerate( (1-cls_tglo.time()) *0.90f );
   //cls_tglo.p_camera->decelerate(0);
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
	
	 /*
	if (cls_tglo.p_cls_input->is_mouse_down(C_MOUSE_LEFT))
	{
	  	D3DXVECTOR3 vec_point_a, vec_point_b;

		//get close point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_a, cls_tglo.p_cls_input->i_mouse_x,
			cls_tglo.p_cls_input->i_mouse_y, 0.001f); //close to screen
		
		//get far point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_b, cls_tglo.p_cls_input->i_mouse_x,
		cls_tglo.p_cls_input->i_mouse_y, 1000); //close to screen
	
	
	   	D3DXVECTOR3 vec_ground;
		D3DXPLANE plane_ground;
		if (cls_tglo.p_terrain->find_ground_from_ray(&vec_point_a, &vec_point_b, &vec_ground, &plane_ground))
		{
	//	 char st_temp[255];
	//	  sprintf(st_temp, "TAPPED: Tiger placed at %s", print_vector(vec_ground));
	//	  log_msg(st_temp);

	  //create tiger
			
			CMesh *p_mesh = new CMesh();
		  this->p_my_thing_list->add_new_thing(p_mesh);
		  p_mesh->setObjectPosition(vec_ground);
		  p_mesh->init_mesh(NULL);
		  
		} else
		{

		 log_msg("Failed to find ground.");
		}
	} 
	 */
    //stick us on the ground

	
//	cls_tglo.p_camera->set_pos_y(1+cls_tglo.p_terrain->get_height(
	//	cls_tglo.p_camera->get_pos_x(), cls_tglo.p_camera->get_pos_z() ));
	  
	// transform the updated camera information.
   
	
	cls_tglo.p_camera->transform( fSpeed, fAngularSpeed );


}