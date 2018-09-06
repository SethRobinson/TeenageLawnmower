#include "CSCamera.h"
#include "CThing_globals.h"   
#include "CCamera.h"
#include "CEnt.h"
CSCamera::CSCamera()
{
    set_thing_name("CSCamera");
    
    
    SetupOrthogonalView();
    set_sort(500000);
}   


void CSCamera::SetupOrthogonalView()
{
      cls_tglo.p_camera->set_isometric_view();
    //let's orient it better
    
   	cls_tglo.p_camera->setObjectPosition(D3DXVECTOR3(20,20,-5));

    cls_tglo.p_camera->setAngleY(0);
    cls_tglo.p_camera->setAngleX(0);
    cls_tglo.p_camera->setAngleZ(0); //look down
    cls_tglo.p_camera->set_iso_zoom(20);

}

void CSCamera::dx_restore()
{
   
    SetupOrthogonalView();
}


void CSCamera::think()
{
   
    
    
    FLOAT fElapsedTime;

    if( cls_tglo.time() > 0.0f )
        fElapsedTime = cls_tglo.time();
    else
        fElapsedTime = 0.05f;

    FLOAT fSpeed        = 6.0f*fElapsedTime*(cls_tglo.p_camera->get_iso_zoom()*0.20f);
    FLOAT fAngularSpeed = 3.0f*fElapsedTime;

    // De-accelerate the camera movement (for smooth motion)
    cls_tglo.p_camera->decelerate( 0.9f );

    /*
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
      */
	



   fSpeed        = 10.0f*fElapsedTime;

   //cls_tglo.p_camera->transform( fSpeed, fAngularSpeed );

   cls_tglo.p_camera->transform( 0, 0 );


   if (cls_tglo.p_player)
   {
   cls_tglo.p_camera->m_vObjectPosition.x = (cls_tglo.p_player->m_vObjectPosition.x);
   cls_tglo.p_camera->m_vObjectPosition.y = (cls_tglo.p_player->m_vObjectPosition.y);


 //  cls_tglo.p_camera->setObjectPosition(cls_tglo.p_player->getObjectPosition());
   }
     

   if (cls_tglo.p_camera->get_pos_y() <17) cls_tglo.p_camera->set_pos_y(17);

}