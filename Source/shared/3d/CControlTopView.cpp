#include "CControlTopView.h"
#include "CThingList.h"
#include "CEnt.h"
#include "..\\vectormath.h"
#include "..\\CInput.h"


#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CControlTopView::CControlTopView()
{
  this->set_sort(-1000); //make sure we get processed early, right after the CCamera object
  this->set_type(C_THING_CONTROL);
  set_thing_name("CControlTopView"); //can't be more than 19 chars!

  //setup camera
  if (cls_tglo.p_camera)
  {
 
    cls_tglo.p_camera->toggle_isometric_view();
 
  } else log_error("CCOntrolTopView unable to set camera, it's null.");
 

}

CControlTopView::~CControlTopView()
{
   //turn off isometric mode
  if (cls_tglo.p_camera)
  {
  cls_tglo.p_camera->toggle_isometric_view();
  } else log_error("CCOntrolTopView unable to set camera, it's null.");
 

}

void CControlTopView::think()
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

    FLOAT fSpeed        = 6.0f*fElapsedTime*(cls_tglo.p_camera->get_iso_zoom()*0.20f);
    FLOAT fAngularSpeed = 3.0f*fElapsedTime;

    // De-accelerate the camera movement (for smooth motion)
    cls_tglo.p_camera->decelerate( 0.9f );

    // Process keyboard input
    if( cls_tglo.p_cls_input->is_key_down(DIK_D) )   cls_tglo.p_camera->slideRight( fSpeed );
    if( cls_tglo.p_cls_input->is_key_down(DIK_S) )  cls_tglo.p_camera->slideLeft( fSpeed );
    if( cls_tglo.p_cls_input->is_key_down(DIK_SPACE) )      cls_tglo.p_camera->slideUp( fSpeed );
    if( cls_tglo.p_cls_input->is_key_down(DIK_R) ) cls_tglo.p_camera->slideDown( fSpeed );


	//might as well let them use the arrow keys too
	if( cls_tglo.p_cls_input->is_key_down(DIK_RIGHT) ) cls_tglo.p_camera->slideRight( fSpeed );

    if( cls_tglo.p_cls_input->is_key_down(DIK_LEFT) )    cls_tglo.p_camera->slideLeft( fSpeed );

    if( cls_tglo.p_cls_input->is_key_down(DIK_DOWN) )   cls_tglo.p_camera->slideDown( fSpeed );

    if( cls_tglo.p_cls_input->is_key_down(DIK_UP) )    cls_tglo.p_camera->slideUp( fSpeed );

	
	
	    if( cls_tglo.p_cls_input->is_key_down(DIK_V) ) 
	{
		cls_tglo.p_camera->set_iso_zoom(cls_tglo.p_camera->get_iso_zoom()+(fSpeed));
	}
    if( cls_tglo.p_cls_input->is_key_down(DIK_F) )
	{
		if (cls_tglo.p_camera->get_iso_zoom() > 0.03f) //don't go into reverse	
		cls_tglo.p_camera->set_iso_zoom(cls_tglo.p_camera->get_iso_zoom()-(fSpeed));

	}

	
	/*
	  */
   fSpeed        = 10.0f*fElapsedTime;

	cls_tglo.p_camera->transform( fSpeed, fAngularSpeed );


}