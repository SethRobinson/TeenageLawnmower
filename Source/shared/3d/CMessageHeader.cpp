/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */




#include "CMessageHeader.h"
#include "CThing_globals.h"
#include "d3dapp.h"
#include "CThingList.h"
#include "CThing_util.h"
#include "CManager.h"
#include "CEntBall.h"        
#include "CGridTile.h" 
#include "CBuildingManager.h"
#include "CBuilding.h"        
#include "CSubManager.h"

//this let's us send complicated engine messages via regular ol windows
//messaging.  This is useful for the editor to communicate with the engine
//in a threadsafe manner.
void HandleSystemMessage(CEngineMessage * p_message)
{
    if (p_message->m_i_message_type = C_THING_MSG_GENERIC)
    {
        CMessageGeneric * p_mg = (CMessageGeneric*)p_message->m_p_message;
        //handle messages here
        if (p_mg->i_message_command == C_MESSAGE_COMMAND_DELETE_THINGS_BY_TYPE)
        {
            cls_tglo.p_thing_list->delete_things_by_type(p_mg->i_thing_type);

        } else
    
        if (p_mg->i_message_command == C_MESSAGE_COMMAND_LIST_ALL_THINGS)
        {
            cls_tglo.p_manager->p_my_thing_list->list_all_things();	

        } else
          if (p_mg->i_message_command == C_MESSAGE_COMMAND_LIST_ALL_RESOURCES)
        {
            cls_tglo.p_manager->list_contents();

        } else
      
            if (p_mg->i_message_command == C_MESSAGE_COMMAND_REBUILD_ALL_WALLS)
            {
                
                if (cls_tglo.p_building_manager)
                {
                    //load all building walls now
                    for (int o = 0; o < cls_tglo.p_map->p_sub_manager->get_count(); o++)
                    {
                        cls_tglo.p_building_manager->get(o)->build_walls();
                        cls_tglo.p_building_manager->get(o)->dx_create();
                        
                    }
                    
                    

	} else
	{
	  log_error("Can't build anything, p_building_manager is null.");
	}
  


        } else
      

            if (p_mg->i_message_command == C_MESSAGE_COMMAND_CREATE_SOME_SPHERES)
            {
                CEntBall *p_mesh;
                for (int p=0; p < 20; p++)
                {
                    p_mesh = new CEntBall();
                    cls_tglo.p_manager->p_my_thing_list->add_new_thing(p_mesh);
                    p_mesh->dx_create();
                }
                
            } else
                
              if (p_mg->i_message_command == C_MESSAGE_COMMAND_SWITCH_TO_PLAYER_WALK_CAM)
                {
                   switch_to_player_walk_cam();
     }  else
         
         if (p_mg->i_message_command == C_MESSAGE_COMMAND_SWITCH_TO_TOP_VIEW_CAM)
         {
             switch_to_top_view_cam();
         } else
             if (p_mg->i_message_command == C_MESSAGE_COMMAND_REBUILD_WALLS)
             {
                 //graphically update it too
                 if (!cls_tglo.p_building_manager) 
                 {
                     log_error("Why isn't p_building_manager initted? (CMessagerHeader)");
                    
                 } else
                 {
                    
                     cls_tglo.p_building_manager->get(p_mg->i_building)->build_walls();
                     cls_tglo.p_building_manager->get(p_mg->i_building)->dx_create();
                 }
                 
             } else
                            
    if (p_mg->i_message_command == C_MESSAGE_COMMAND_SWITCH_TO_FREE_CAM)
     {
       switch_to_free_cam();
        } else

       if (p_mg->i_message_command == C_MESSAGE_COMMAND_SWITCH_TO_PLAY_CAM)
        {
            switch_to_play_cam();
        } else

         if (p_mg->i_message_command == C_MESSAGE_COMMAND_RESET_LIGHT_AND_FOG)
        {
            set_reset_light_and_fog();
        } else
         if (p_mg->i_message_command == C_MESSAGE_COMMAND_RANDOM_FOG)
        {
            set_random_fog();
        } else
         if (p_mg->i_message_command == C_MESSAGE_COMMAND_INIT_ENGINE_FROM_PATH)
        {
            init_engine_from_base_path(true);
        } else


         if (p_mg->i_message_command == C_MESSAGE_COMMAND_RANDOM_SUN)
        {
            set_random_sun();
        } else
       
            if (p_mg->i_message_command == C_MESSAGE_COMMAND_TERRAFORM_MAP)
            {
            cls_tglo.p_map->create_random_height(p_mg->f_height, p_mg->f_height_scale,
                    p_mg->f_hill_freq,p_mg->f_hill_scale, p_mg->i_run_times);
                cls_tglo.p_map->send_map_to_terrain();
            }   else
                
        if (p_mg->i_message_command == C_MESSAGE_COMMAND_REINIT_MAP)
        {

            if (p_mg->f_scale != 1)
            {
                
                //let's scale the height vertices while we're ad it
                cls_tglo.p_map->ScaleVerts(p_mg->f_scale);
            }
            cls_tglo.p_map->send_map_to_terrain();
   
        }   else
            if (p_mg->i_message_command == C_MESSAGE_COMMAND_FORCE_GRID_TILES_ON)
            {

                //let's show the tiles if we need to
                if (!cls_tglo.p_grid_tile)
                {
                    CGridTile *p_grid = new CGridTile();
                    cls_tglo.p_thing_list->add_new_thing(p_grid);
                    p_grid->dx_create();
                }

            }   else


         if (p_mg->i_message_command == C_MESSAGE_COMMAND_TOGGLE_LAND_WIREFRAME)
        {
            set_landscape_wire_frame_toggle();
        } else
        
            if (p_mg->i_message_command == C_MESSAGE_COMMAND_TOGGLE_SHOW_TILE)
            {
                if (cls_tglo.p_thing_list->get_thing_count(C_THING_TILE) > 0)
                {
                    //it exists, just kill it
                    cls_tglo.p_thing_list->delete_things_by_type(C_THING_TILE);
                     
                } else
                {
                    //it doesn't exist yet
                    //install new one
                    CGridTile *p_tile = new CGridTile();
                    cls_tglo.p_thing_list->add_new_thing(p_tile);
                    p_tile->dx_create();
                }
                
            } else
     
         
         {
            log_msg("Unable to handle this kind of CMessageGeneric as a system message.");
        }

    }   else
    {
        log_msg("Unable to handle this type of system message.");
    }

}


void ProcessWindowsEngineMessage(DWORD dw_message, CEngineMessage * p_message)
{
//   log_msg("Got message.");

   //process it
   if (p_message->m_b_system_message)
   {
       //special handling, not send to an object.
       HandleSystemMessage(p_message);
   }   else
   {
      
       CMessageHeader header;
       header.i_message = p_message->m_i_message_type;
       
       if (p_message->m_st_entity_name[0] == 0)
       {
           cls_tglo.p_thing_list->broadcast_message(&header, p_message->m_p_message,
               p_message->m_i_entity_type);
           
       } else
       {
           //name isn't null, send by name.
           cls_tglo.p_thing_list->SendMessageByName(&header, p_message->m_p_message, p_message->m_st_entity_name);
       }
   }


   //free the memory
   SAFE_DELETE(p_message->m_p_message); //delete the custom sub message
   SAFE_DELETE(p_message); //delete the overall message structure.  these were newed() from
   //the caller.

}


void SendWindowsEngineMessage(CEngineMessage *p_message)
{
    //let's do it!
    if (cls_tglo.p_d3dapp)
    SendMessage(cls_tglo.p_d3dapp->m_hWnd, WM_ENGINE_MESSAGE, 0, (long)p_message);

}


void SendSimpleWindowsEngineMessage(int i_command, int i_parm)
{

    CEngineMessage *p_new = new CEngineMessage();
    p_new->m_i_message_type = C_THING_MSG_GENERIC;
    CMessageGeneric *p_mg = new CMessageGeneric();
    p_new->m_p_message = p_mg;
    p_new->m_b_system_message = true;
    p_mg->i_message_command = i_command;
    p_mg->i_thing_type = i_parm;

    SendWindowsEngineMessage(p_new);


}



