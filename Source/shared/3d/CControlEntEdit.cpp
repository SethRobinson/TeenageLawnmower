#include "CControlEntEdit.h"
#include "CEntItem.h"
#include "CCamera.h"
#include "CTerrain.h"
#include "CThingList.h"
#include "CEntLib.h"
#include "CEntData.h"
#include "CThing_util.h"

CControlEntEdit::CControlEntEdit()
{
   this->set_sort(-1000); //make sure we get processed early, right after the CCamera object
   this->set_type(C_THING_EDIT);
}  

void CControlEntEdit::think()
{
	if (i_mode == C_ENT_EDIT_MODE_CREATE) process_create();
	if (i_mode == C_ENT_EDIT_MODE_DELETE) process_delete();
	if (i_mode == C_ENT_EDIT_MODE_MOVE) process_move();
	if (i_mode == C_ENT_EDIT_MODE_EDIT) process_edit();


}



void CControlEntEdit::process_create()
{
 	if (cls_tglo.p_cls_input->is_mouse_tapped(::C_MOUSE_LEFT))
	{
	  	D3DXVECTOR3 vec_point_a, vec_point_b;
	    //get close point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_a, cls_tglo.p_cls_input->GetMouseX(),
		cls_tglo.p_cls_input->GetMouseY(), 0.001f); //close to screen
		
		//get far point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_b, cls_tglo.p_cls_input->GetMouseX(),
		cls_tglo.p_cls_input->GetMouseY(), 1000); //close to screen

		D3DXVECTOR3 vec_ground;
		D3DXPLANE plane_ground;
		if (!cls_tglo.p_terrain->find_ground_from_ray(&vec_point_a, &vec_point_b, &vec_ground, &plane_ground))
		{
		  //didn't touch anything
			return;
		}

		if ( (ui_cur_category == 0) || (i_cur_item_index < 0) )
		{
			log_msg("No item selected, can't create it.");
			return;
		}
		
		CEntItem *p_ent = new CEntItem();	//an ent item is an entity build/owning an item
		this->p_my_thing_list->add_new_thing(p_ent);
        p_ent->SetSave(true); //save with map objects
		CEntData *p_item_data = NULL; //this will hold the actual item data that gets loaded/saved
		(*cls_tglo.p_ent_lib->get_base_item(ui_cur_category, i_cur_item_index)).p_ent_default->clone(&p_item_data);


		p_item_data->vec_pos = vec_ground;

		p_ent->setup_item_from_cent( p_item_data);
		
		//preload it
		p_ent->dx_create();

		//set custom defaults for it
		
//	    p_ent->setObjectPosition(vec_ground);
//		log_msg("Created thing.");
	}

}


void CControlEntEdit::process_delete()
{
 	if (cls_tglo.p_cls_input->is_mouse_tapped(::C_MOUSE_LEFT))
	{
	  	D3DXVECTOR3 vec_point_a, vec_point_b;
	    //get close point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_a, cls_tglo.p_cls_input->GetMouseX(),
		cls_tglo.p_cls_input->GetMouseY(), 0.001f); //close to screen
		
		//get far point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_b, cls_tglo.p_cls_input->GetMouseX(),
		cls_tglo.p_cls_input->GetMouseY(), 1000); //close to screen

		//Thingfind_entity_from_ray

		float f_dist;

		
		//the last zero is to say "no minimum distance required"
		CThing *p_thing;
		D3DXVECTOR2 vec_mouse = D3DXVECTOR2(float(cls_tglo.p_cls_input->GetMouseX()), float(cls_tglo.p_cls_input->GetMouseY()));
		if (p_thing = ray_pick_objects(p_my_thing_list, &vec_point_a, &vec_point_b, -1, &f_dist, 0, NULL, &vec_mouse))
		{
			//the -1 means pick all
		   	//found 
			
			 p_thing->b_delete_flag = true; //it will get deleted on the next call
			 log_msg("Deleted thing.");

		}
	
	}

}

void CControlEntEdit::process_move()
{
 	if (cls_tglo.p_cls_input->is_mouse_tapped(::C_MOUSE_LEFT))
	{
	  	D3DXVECTOR3 vec_point_a, vec_point_b;
	    //get close point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_a, cls_tglo.p_cls_input->GetMouseX(),
		cls_tglo.p_cls_input->GetMouseY(), 0.001f); //close to screen
		
		//get far point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_b, cls_tglo.p_cls_input->GetMouseX(),
		cls_tglo.p_cls_input->GetMouseY(), 1000); //close to screen

		//Thingfind_entity_from_ray

		float f_dist;

		
		//the last zero is to say "no minimum distance required"
		CThing *p_thing;
		D3DXVECTOR2 vec_mouse = D3DXVECTOR2(cls_tglo.p_cls_input->GetMouseX(), cls_tglo.p_cls_input->GetMouseY());
		if (p_thing = ray_pick_objects(p_my_thing_list, &vec_point_a, &vec_point_b, -1, &f_dist, 0, NULL, &vec_mouse))
		{
			//the -1 means pick all
		   	//found 
			 //mark it as dragging
			 
			((CVisualMulti*)p_thing)->set_drag_mode(true);
			log_msg("Moving %s.",p_thing->get_thing_name());
		}
	
	}

}


void CControlEntEdit::process_edit()
{
 	if (cls_tglo.p_cls_input->is_mouse_tapped(::C_MOUSE_LEFT))
	{
	  	D3DXVECTOR3 vec_point_a, vec_point_b;
	    //get close point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_a, cls_tglo.p_cls_input->GetMouseX(),
		cls_tglo.p_cls_input->GetMouseY(), 0.001f); //close to screen
		
		//get far point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_b, cls_tglo.p_cls_input->GetMouseX(),
		cls_tglo.p_cls_input->GetMouseY(), 1000); //close to screen

		//Thingfind_entity_from_ray

		float f_dist;

		
		//the last zero is to say "no minimum distance required"
		CThing *p_thing;
		D3DXVECTOR2 vec_mouse = D3DXVECTOR2(cls_tglo.p_cls_input->GetMouseX(), cls_tglo.p_cls_input->GetMouseY());
		if (p_thing = ray_pick_objects(p_my_thing_list, &vec_point_a, &vec_point_b, -1, &f_dist, 0, NULL, &vec_mouse))
		{
			//the -1 means pick all
		   	//found 
		    //edit this thing
			//put thing in our last clicked buffer to be extracted by whoever
			//called us

			p_last_thing = p_thing;
		
			//do the callback
			if (!func_ret)
			{
			  log_error("No function pointer set yet for edit thing callback!");
			  return;
			} else
			{
			func_ret();
			}


		}	else
		{
		  p_last_thing = NULL; //couldn't find anything

		}
	
	}

}

void CControlEntEdit::set_edit_callback(void (*func)())
{
  this->func_ret = func;
}

