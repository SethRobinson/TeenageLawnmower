/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CThingStats.h"
#include "CThing_globals.h"
#include "CThingFont.h"
#include "CCamera.h"
#include "CMap.h"
#include "CTerrain.h"
#include "d3dapp.h"
#include "CThingList.h"
#include "..\\sound\\ISoundManager.h"
#include "CGridTile.h"    
#include "..\\CScriptEngine.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CThingStats::CThingStats()
{
  this->set_sort(20000000); //make sure we're processed last
  byte_key = 0;
  b_draw_active = false; //off by default
  set_thing_name("Stats");
  m_b_show_fps = false;
  SetShowSimpleFPS(false);



 #ifdef _DEBUG
  m_b_show_fps = true;

 #endif
}
void CThingStats::think()
{
 if (byte_key)
	  if (cls_tglo.p_cls_input->is_key_tapped(byte_key))
	  {
	    b_draw_active = !b_draw_active;

	  }

	
  	if (cls_tglo.p_cls_input->is_key_tapped(DIK_F11))   
{
	//write some debug info
	    if (cls_tglo.p_sound)
        {
        
        log_msg("Currently there are %d sound instances running.",cls_tglo.p_sound->GetSoundsCurrentlyPlaying());
        } else
        {
            log_msg("Sound not initted, can't list sounce resources.");
        }

        if (cls_tglo.p_script_engine)
        {
        
        log_msg("Currently %d scripts running.", cls_tglo.p_script_engine->GetScriptsLoaded());
        cls_tglo.p_script_engine->LogAllScriptsLoaded();
        
        }




}

	if (cls_tglo.p_cls_input->is_key_tapped(DIK_F9))   
{
	//write some debug info
	cls_tglo.p_manager->list_contents();
}

if (cls_tglo.p_cls_input->is_key_tapped(DIK_F8))   
{
	//write some debug info
	p_my_thing_list->list_all_things();
}


//only allow this from the game, the editor has it's own way of turning on the
//tile grid


if (cls_tglo.GetProgram() == C_GLOBAL_GAME)
{
    if (cls_tglo.p_cls_input->is_key_tapped(DIK_H))
    {
        if (cls_tglo.p_cls_input->is_key_down(DIK_LCONTROL))
        {
            
            
            CThing *p_ent = p_my_thing_list->get_thing_by_name("TILEGRID");
            
            if (!p_ent)
            {
                
                //install new one
                CGridTile *p_tile = new CGridTile();
                p_my_thing_list->add_new_thing(p_tile);
                p_tile->dx_create();
                
            }   else
            {
                //kill it
                p_ent->b_delete_flag = true;
            }
        }
        
    }
}
if (
	(cls_tglo.p_cls_input->is_key_down(DIK_LCONTROL))  //left control  
	 &&
	(cls_tglo.p_cls_input->is_key_tapped(DIK_F))   
	)
{
   m_b_show_fps = !m_b_show_fps;
}

 if (cls_tglo.GetProgram() == C_GLOBAL_EDITOR)
 if (cls_tglo.p_cls_input->is_key_tapped(DIK_P))
 {
    D3DXVECTOR3 vec_cam = cls_tglo.p_camera->getObjectPosition();


     if (cls_tglo.p_cls_input->is_key_down(DIK_LSHIFT))
     {

         //use TARGET commands because shift is held down
       	 //print out some info for easy cut and paste
   	log_msg("int id_cam = EntGetByName(\"Main Camera\");");
	char st_temp[256];
		
	sprintf(st_temp, "vec_cam = MakeVector3(%.3f, %.3f, %.3f);",
	  vec_cam.x, vec_cam.y, vec_cam.z );
	log_msg(st_temp);
	log_msg("EntSetTargetPosition(id_cam, vec_cam);");

	float f_angle = cls_tglo.p_camera->getAngleZ();

    f_angle = fmod(f_angle, D3DX_PI*2);
	sprintf(st_temp, "EntSetTargetDirection(id_cam, %.3f);",f_angle );
	log_msg(st_temp);

	sprintf(st_temp, "EntSetTargetTilt(id_cam, %.3f);",cls_tglo.p_camera->getAngleY());
	log_msg(st_temp);


     }   else
     {
         //normal move

      	 //print out some info for easy cut and paste
   	log_msg("int id_cam = EntGetByName(\"Main Camera\");");
	char st_temp[256];
		
	sprintf(st_temp, "vec_cam = MakeVector3(%.3f, %.3f, %.3f);",
	  vec_cam.x, vec_cam.y, vec_cam.z );
	log_msg(st_temp);
	log_msg("EntSetPosition(id_cam, vec_cam);");

	float f_angle = cls_tglo.p_camera->getAngleZ();

    f_angle = fmod(f_angle, D3DX_PI*2);
	sprintf(st_temp, "EntSetDirection(id_cam, %.3f);",f_angle );
	log_msg(st_temp);

	sprintf(st_temp, "EntSetTilt(id_cam, %.3f);",cls_tglo.p_camera->getAngleY());
	log_msg(st_temp);

     }

     
     
    


 }
}

void CThingStats::dx_draw()
{
	
	
	D3DCOLOR fontColor        = D3DCOLOR_ARGB(255,255,255,0);
    D3DCOLOR fontWarningColor = D3DCOLOR_ARGB(255,0,255,255);
    TCHAR szMsg[MAX_PATH] = TEXT("");

    // Output display stats
    FLOAT fNextLine = 40.0f; 
   	if (m_b_show_fps)
	{
     if (!m_b_show_simple_fps)
     {
  
    lstrcpy( szMsg, cls_tglo.p_d3dapp->m_strDeviceStats );
    fNextLine -= 20.0f;
  	cls_tglo.p_font->DrawFastText(0, 2, fNextLine, fontColor,szMsg);
    
    lstrcpy( szMsg, cls_tglo.p_d3dapp->m_strFrameStats );
    fNextLine -= 20.0f;
 	cls_tglo.p_font->DrawFastText(0, 2, fNextLine, fontColor,szMsg);
    
      fNextLine = (FLOAT) cls_tglo.p_d3dapp->m_d3dsdBackBuffer.Height; 
     	D3DXVECTOR3 vec_cam = cls_tglo.p_camera->getObjectPosition();


     
     	sprintf(szMsg, "Pos is X: %.3f Y: %.3f Z: %.3f",
					vec_cam.x, vec_cam.y, vec_cam.z);
				fNextLine -= 20.0f;
				cls_tglo.p_font->DrawFastText(0, 2, fNextLine, fontColor,szMsg);
	

     
     } else
     {
       fNextLine = 21.0f;   //simple fps...
         sprintf(szMsg, "FPS: %.2f", cls_tglo.p_d3dapp->m_fFPS);
         fNextLine -= 20.0f;
  	     cls_tglo.p_font->DrawFastText(0, 2, fNextLine, fontColor,szMsg);
    
     
     
     
     }

    // Output statistics & help
    fNextLine = (FLOAT) cls_tglo.p_d3dapp->m_d3dsdBackBuffer.Height; 
	}
   // wsprintf( szMsg, TEXT("Arrow keys: Up=%d Down=%d Left=%d Right=%d"), 
   //           m_UserInput.bRotateUp, m_UserInput.bRotateDown, m_UserInput.bRotateLeft, m_UserInput.bRotateRight );
   // fNextLine -= 20.0f; m_pFont->DrawText( 2, fNextLine, fontColor, szMsg );

   // lstrcpy( szMsg, TEXT("Use the arrow keys and S,D,F and V to move around.") );
   // fNextLine -= 20.0f; cls_tglo.p_font->p_font->DrawText( 2, fNextLine, fontColor, szMsg );

  //  lstrcpy( szMsg, TEXT("Press 'F2' to configure display, number keys to change options.") );
  //  fNextLine -= 20.0f; cls_tglo.p_font->p_font->DrawText( 2, fNextLine, fontColor, szMsg );
	
	
	D3DXVECTOR3 vec_cam = cls_tglo.p_camera->getObjectPosition();

if (cls_tglo.GetProgram() == C_GLOBAL_EDITOR)
	{	
		if (cls_tglo.p_map)
		{
			int block_x = cls_tglo.p_map->get_block_x(vec_cam.x);
			int block_z = cls_tglo.p_map->get_block_y(vec_cam.z);
			if (cls_tglo.p_terrain)
			{
				sprintf(szMsg, "Pos is X: %.3f Y: %.3f Z: %.3f - (Block %d:%d) - Height is %.3f.",
					vec_cam.x, vec_cam.y, vec_cam.z, block_x, block_z, cls_tglo.p_terrain->get_height(vec_cam.x,vec_cam.z));
				fNextLine -= 20.0f;
				cls_tglo.p_font->DrawFastText(0, 2, fNextLine, fontColor,szMsg);
	
				
				//also show orientation
				
				
				sprintf(szMsg, "Roll: %.3f Tilt: %.3f Direction: %.3f", 
					cls_tglo.p_camera->getAngleX(), cls_tglo.p_camera->getAngleY(), cls_tglo.p_camera->getAngleZ());
				fNextLine -= 20.0f; 
				 	cls_tglo.p_font->DrawFastText(0, 2, fNextLine, fontColor,szMsg);

				
				
				
			}
		}
	}
	
	//	cls_tglo.p_camera->set_pos_y(cls_tglo.p_terrain->get_height(vec_cam.x,vec_cam.z));





}


void CThingStats::activate_draw_toggle(byte dik_key) 
{
  byte_key = dik_key;
}



