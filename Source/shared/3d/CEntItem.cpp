/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CEntItem.h"
#include "CManager.h"
#include "CMap.h"
#include "CEntLib.h"
#include "CMessageHeader.h"
#include "file_utils.h"
#include "..\\CScriptEngine.h"
#include "script_utils.h"
#include "cthing_util.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEntItem::CEntItem()
{
    
    
    //  place_on_ground();
	p_ent = NULL;
    this->set_type(C_THING_ENT_GENERIC);
	SetSave(false); //by default we won't save stuff 
	p_base = NULL; //our template
    m_i_script = C_SCRIPT_NONE; //no script attached
	m_b_run_script = false;
	SetOnHitScript(false);
	SetOnTouchScript(false);  
    SetOnTouchWallScript(false);
    SetOnTouchGroundScript(false);
    SetOnTouchCeilingScript(false);
//	f_bounce_power = 0.25f;
  	//maybe I should separate skidding up/down..
   	f_accel = 0.3f;	  
//	SetHitWallBehavior(C_HITWALL_REVERSE);
	SetTurnLerp(4); //so the editor feels right

}

void CEntItem::SetFloatingMode(	bool b_new)
{
	b_floating = b_new;
	if (p_ent) p_ent->b_floating = b_new;
}

void CEntItem::ShutDownScript()
{
    	if (m_i_script != ::C_SCRIPT_NONE)
	{
		 	cls_tglo.p_script_engine->RunScriptFunction(m_i_script, "ONKILL");
	
			//now actually remove the script from memoty
			cls_tglo.p_script_engine->UnloadScript(m_i_script);
			m_i_script = C_SCRIPT_NONE;
	}
	

}


CEntItem::~CEntItem()
{
    ShutDownScript();
	//kill any memory that we need to
	SAFE_DELETE(p_ent);

}
void CEntItem::update_ent_data()
{
	p_ent->vec_pos = this->m_vObjectPosition;
	p_ent->f_dir = this->getAngleY();

	//if THINK is being used on this item then it needs this mod...
//	 if (b_follow_ground_tilt) //later I decided all items should do a think
	 {
		p_ent->f_dir = (f_dir_goal);
	 }

	
	if (p_base)
	{
	
	p_ent->vec_scale.x = f_scale_x /vec_original_base_size.x;
	p_ent->vec_scale.y = this->f_scale_y / vec_original_base_size.y;
	p_ent->vec_scale.z = this->f_scale_z/ vec_original_base_size.z;


	}	else
	{

		//
	p_ent->vec_scale.x = this->f_scale_x;
	p_ent->vec_scale.y = this->f_scale_y;
	p_ent->vec_scale.z = this->f_scale_z;
	}
}

void CEntItem::SetRelativeScale(D3DXVECTOR3 vec_scale)
{
	if (!p_base)
	{
	
	set_scale(vec_scale.x, vec_scale.y, vec_scale.z);
    BuildRadius(vec_scale.x);

	} else
	{

	   set_scale(p_base->vec_scale.x * vec_scale.x,p_base->vec_scale.y *  vec_scale.y, 
		p_base->vec_scale.z * vec_scale.z);
	
	  //also change our f_radius to match our new scale
	   BuildRadius(p_base->vec_scale.x * vec_scale.x);
	}
}

D3DXVECTOR3 CEntItem::GetRelativeScale()
{

    D3DXVECTOR3 vec_scale = D3DXVECTOR3(get_scale_x(), get_scale_y(), get_scale_z());
   
    
     //remove base scale from the equation to isolate the real scale

    vec_scale.x /= p_base->vec_scale.x;
    vec_scale.y /= p_base->vec_scale.y;
    vec_scale.z /= p_base->vec_scale.z;
    return vec_scale;
}




void CEntItem::SetRelativeScaleUniform(float f_scale)
{
	if (!p_base)
	{
	
	set_scale_uniform(f_scale);
   BuildRadius(f_scale);

	} else
	{
	   set_scale_uniform(p_base->vec_scale.y * f_scale);
	   BuildRadius(p_base->vec_scale.x * f_scale);
	}
}


void CEntItem::receive_message(CMessageHeader *p_header, void *p_message)
{

   if (p_header->i_message == ::C_THING_MSG_TOUCHED_WALL)
	{
     if (m_b_on_wall_touch_script)
     {
        cls_tglo.p_script_engine->RunScriptFunction(m_i_script, "ONTOUCHWALL");
     }
    return;
   }

    if (p_header->i_message == ::C_THING_MSG_TOUCHED_CEILING)
	{
     if (m_b_on_ceiling_touch_script)
     {
        cls_tglo.p_script_engine->RunScriptFunction(m_i_script, "ONTOUCHCEILING");
     }
    return;
   }

   if (p_header->i_message == ::C_THING_MSG_TOUCHED_GROUND)
	{
     if (m_b_on_ground_touch_script)
     {
        cls_tglo.p_script_engine->RunScriptFunction(m_i_script, "ONTOUCHGROUND");
     }
    return;
   }
   
    if (p_header->i_message == ::C_THING_MSG_KILL)
	{
	  //we've been instructed to kill ourself
	 if (GetPauseLevel() <= cls_tglo.GetPauseLevel())
	 {
	 
	 this->b_delete_flag = true;
	  return;
	 } else
	 {
		// log_msg("Didn't delete %s, too much priority.", get_thing_name());
	 }
	}

    if (p_header->i_message == C_THING_MSG_MOVE)
    {
        
        CMessageMove *p_move = (CMessageMove*)p_message;
       //LogMsg("%s got message from %s", this->get_thing_name(), p_move->p_me->get_thing_name());

        if (GetHardness() == C_ENT_HARDNESS_NORMAL)
       {
          p_move->p_blocked_by = this; //let them know we blocked them
       }
     
         if (!m_b_on_hit_script && !m_b_on_touch_script) return; //don't need to know if we got hit
      
        
        if (!m_b_on_touch_script)
        {
            if (p_move->f_damage <= 0) return; //we only want to know if we actually got swung at
        }
        
        if ( (m_i_visual_type == C_RESOURCE_PMESH) || (this->m_i_visual_type == C_RESOURCE_MD2) ||
            (this->m_i_visual_type == C_RESOURCE_RTM) )
        {
           
            //should we do an additional AABB check?

            if (e_collision_type == C_COLLISION_BOX)
            {
                if (p_move->p_me)
                {
                    //grab info from the object
                    if (p_move->p_me->m_i_visual_type == ::C_RESOURCE_PMESH)
                        if (m_i_visual_type == ::C_RESOURCE_PMESH)
                        {
                            CResourcePMesh *p_mesh = (CResourcePMesh*)GetResourcePointer();
                            CResourcePMesh *p_his_mesh = (CResourcePMesh*)p_move->p_me->GetResourcePointer();
                           
                            D3DXVECTOR3 bb_min, bb_max;
                            D3DXVECTOR3 his_bb_max, his_bb_min;
                            D3DXMATRIX mat_temp;

                            D3DXMatrixScaling(&mat_temp, f_scale_x, f_scale_y, f_scale_z);
                        	D3DXMatrixMultiply( &mat_temp, &mat_temp, &m_matTrans );

                           	D3DXVec3TransformCoord( &bb_min,  &p_mesh->vec_bb_min,&mat_temp);
                           	D3DXVec3TransformCoord( &bb_max,  &p_mesh->vec_bb_max,&mat_temp);

                            D3DXMatrixScaling(&mat_temp, p_move->p_me->get_scale_x(), p_move->p_me->get_scale_y(), p_move->p_me->get_scale_z());
                        	D3DXMatrixMultiply( &mat_temp, &mat_temp, &p_move->p_me->m_matTrans );

                           	D3DXVec3TransformCoord( &his_bb_min,  &p_his_mesh->vec_bb_min, &mat_temp);
                           	D3DXVec3TransformCoord( &his_bb_max,  &p_his_mesh->vec_bb_max, &mat_temp);

                           if (!AABBIntersect(bb_min, bb_max, his_bb_min, his_bb_max))
                           {
                               //rects not overlapping
                               return;
                           }
                        }
                }
            }
               
                  //log_msg("Collision with %s.", this->get_thing_name());
                //run the script
                if (m_b_on_hit_script)
                    if (p_move->f_damage > 0)
                    {
                   	    if (p_move->p_me)
                        cls_tglo.p_script_engine->GetScript(0)->GetVar(g_i_touched_by_id)->SetInt(p_move->p_me->get_id());
		      
                        cls_tglo.p_script_engine->RunScriptFunction(m_i_script, "ONHIT");
                    }

                    if (m_b_on_touch_script)
                    {
                       if (p_move->p_me)
                        cls_tglo.p_script_engine->GetScript(0)->GetVar(g_i_touched_by_id)->SetInt(p_move->p_me->get_id());
		      

                        cls_tglo.p_script_engine->RunScriptFunction(m_i_script, "ONTOUCH");
                    }
          
           
            
        }
        return;
    }
    
	if (p_header->i_message == C_THING_MSG_SAVE_GAME)
	{
        if (m_b_save)
        {
        
        update_ent_data(); //copy any changes we've made to it
  	  CMessageFile *p_file = (CMessageFile*)p_message;
	  //write id and header
	   write_type(this->get_type(), p_file->fp);
	   //write this item/ent and all attached
	   this->p_ent->save(p_file->fp);
        }
	}

	if (p_header->i_message == ::C_THING_RAY_PICK)
	{
  	  CMessageRayPick *p_ray = (CMessageRayPick*)p_message;

  	  CVisualMulti::ray_pick(p_ray); //send it the whole pack, why not
	  return;
	}

  
}

unsigned int CEntItem::get_class_id()
{
  if (!p_ent)
  {
	  return 0; //no class id yet
  }

  return (p_ent->i_class_id);

}

int CEntItem::get_item_id()
{
  if (!p_ent)
  {
	  return 0; //no item id yet
  }

  return (p_ent->i_id);

}
void CEntItem::RunScriptFunction(char *p_st_func_name)
{
    if (m_i_script == C_SCRIPT_NONE)
    {
        log_error("Um, %s doesn't have a script loaded, why would it run %s?", get_thing_name(), p_st_func_name);
        return;
    }
    _strupr(p_st_func_name);
	cls_tglo.p_script_engine->RunScriptFunction(m_i_script, p_st_func_name);
}


void CEntItem::setup_item_from_cent(CEntData *p_new_ent)
{
   if (p_new_ent != NULL)
   {
	p_ent = p_new_ent;	//save this pointer.  it has been newed before coming here, we have to
	//delete it later when we're done.
   } else
   {
	 //reinitting what we already have
	   if (!p_ent)
	   {
		 log_error("Can't reinit item, item not set!");
		 return;
	   }

   }
  
   //if a script was active, kill it.

   ShutDownScript();


   //force some items to get rechecked/refreshed
   SetAlphaSort(C_VISUAL_ALPHA_SORT_UNKNOWN);
  this->set_translate_asap(false); //this is actually misnamed, it's more like "translate every frame with the
  //dx_draw_prepare command?  
  	m_i_draw_mode = C_DRAW_MODE_NORMAL;
   	m_dw_vertex_color = D3DCOLOR_XRGB(255,255,255); //default to all color, no translucent
  
  //turn off any fading we were doing, or coloring
    SetShadowMode(C_VISUAL_SHADOW_NONE);
    SetBlink(false);
    SetSmartFade(0,0); //disable smart fade

 	//first see if this base item even exists
     if (!cls_tglo.p_ent_lib->class_exists(p_ent->i_class_id) ||
         !cls_tglo.p_ent_lib->get_base_item(p_ent->i_class_id, p_ent->i_id))
	 {
		 //big problem.  We don't know what kind of class this is.

         
         //isolate the problem

         if (!cls_tglo.p_ent_lib->class_exists(p_ent->i_class_id) )
         {
         
         log_error("Unable to locate class_id %u, skipping item %s.",p_ent->i_class_id, p_ent->st_name);
         } else
         {

         log_error("Item index %d not found inside of class %s. skipping item %s.",p_ent->i_id, cls_tglo.p_ent_lib->GetCategoryName(p_ent->i_class_id), p_ent->st_name);

         }
	  
         
         
         
         init_visual_sprite("texture\\unknown.dds", "texture\\unknown.dds"); //generic thing to show for now
		 	this->set_scale(
		12*p_ent->vec_scale.x,
		12*p_ent->vec_scale.y,
		12*p_ent->vec_scale.z
		);
    set_no_lighting(true);
	 m_i_draw_mode = C_DRAW_MODE_FOLLOW_Y;
   

	 }	else
	 {
   
    //get base info about this item
	p_base = cls_tglo.p_ent_lib->get_base_item(p_ent->i_class_id, p_ent->i_id);
	
	if  (file_extension_is(p_base->st_visual, ".x")) 
	{
		init_visual(p_base->st_visual,C_RESOURCE_PMESH);
		b_update_matrix = true;
	}	else
   if  (file_extension_is(p_base->st_visual, ".md2")) 
	{
		init_visual(p_base->st_visual,C_RESOURCE_MD2);
		b_update_matrix = true;
	}	else
    if  (file_extension_is(p_base->st_visual, ".rtm")) 
	{
		init_visual(p_base->st_visual,C_RESOURCE_RTM);
		b_update_matrix = true;
	}	else
	{
		//assume a sprite of some type
		init_visual_sprite(p_base->st_visual,p_base->st_visual);
	}
	
    if (p_base->b_enable_lighting)
   this->set_no_lighting(false); else set_no_lighting(true);

   b_follow_ground_tilt = p_base->b_follow_ground_mode;
  

	 this->SetRelativeScale(p_ent->vec_scale);
	  vec_original_base_size = p_base->vec_scale;

	 //should we attach a script?
//	  log_msg("Set draw mode to %d. Y aligned is %d.", p_base->i_draw_mode, C_DRAW_MODE_FOLLOW_Y);
	  m_i_draw_mode = p_base->i_draw_mode;
	 
	 }


	set_desired_velocity(0.0f);
	//let's also place it in a random position
	 b_floating = p_ent->b_floating;
     if (b_floating)
     {
         //let's turn off physics by default, if it's floating we probably don't want physics to
         //move it with gravity or force it above ground.
         m_i_physics = C_ENT_PHYSICS_NONE;
     }
  	setObjectPosition(p_ent->vec_pos);
	//	set_follow_ground_tilt(p_ent->b);
   set_direction_rad(p_ent->f_dir);  //anything doing physics will check this
   this->f_dir_goal = p_ent->f_dir;
   	f_dir = p_ent->f_dir;
   setAngleY(p_ent->f_dir);	 //if not doing physics, this will set the rotation too
   f_ground_friction = 0.06f; //very low friction
   f_bounce_power = 0.10f;  
   set_thing_name(p_ent->st_name);
   SetAlphaSort(C_VISUAL_ALPHA_SORT_UNKNOWN); //recheck for alpha
    if (p_base && cls_tglo.p_script_engine) //only run scripts if the script engine is activated.. ie, not in the editor
   {
	   if (p_base->st_script[0])
       {
            InitScript(p_base->st_script);
  	   }
   }

     b_update_matrix = true;
	
	 
	 //set it a random color
	  //VertexColor(D3DCOLOR_ARGB(255,random_range(0,255), random_range(0, 255), random_range(0, 255)));
     
    //  m_b_skip_draw_of_next_frame = true;
	// CEnt::think();

}

//returns the script ID just in case you want it for something

int CEntItem::InitScript(char *st_script_file)
{

				m_i_script = cls_tglo.p_script_engine->CreateScript();
				//let's add any script specific variables it needs
				int i_temp_var = cls_tglo.p_script_engine->GetScript(m_i_script)->AddVar("i_my_id", ::C_SCRIPT_VAR_INT);
				cls_tglo.p_script_engine->GetScript(m_i_script)->GetVar(i_temp_var)->SetInt(this->get_id());

	
			int i_result = cls_tglo.p_script_engine->load_and_compile_script(st_script_file, m_i_script);
			if (i_result != ::C_SCRIPT_NONE)
			{
				//successfull load

					//we'll run our oncreate and main on the next tick.  this gives a chance
				//to the script/process that created us to set some defaults.
					//always run the OnLoad right after making it
			  //  m_b_run_script = true;
                 cls_tglo.p_script_engine->RunScriptFunction(m_i_script, "ONCREATE");
			cls_tglo.p_script_engine->RunScriptFunction(m_i_script, "MAIN");

			//	this->set_no_draw(true); //only draw after the script has been run for the first time
			} else
			{
				//not succesfull.  Kill the script we had created earlier.  Error message wil be
				//handled by the CScriptEngine.
				cls_tglo.p_script_engine->UnloadScript(m_i_script);
				m_i_script = C_SCRIPT_NONE;
			}
			 return m_i_script;

}


void CEntItem::think()
{
    
    if (b_delete_flag)
  {
	  //we've been marked for deletion, let's not actually do anything
	  return;
  }

  	 if (GetPauseLevel() < cls_tglo.GetPauseLevel())
	 {
       //only higher priotity things can run right now
		return;
	 }
  
     CEnt::think();


  if (m_i_script != ::C_SCRIPT_NONE)
	{
		if (m_b_run_script)
		{
		     m_b_run_script = false;
		//	set_no_draw(false);  //hoefully we didn't need this for TLM.. I'm now using
             //m_b_skip_render_of_next_frame, cleaner system, won't lose script changes
			
		}  else
		{
            //Update our script each frame.  This generally will do
			//nothing, basically a way to check for script timers and such.
			cls_tglo.p_script_engine->GetScript(m_i_script)->UpdateCallback();
		}	   
	}	
		
		CVisualMulti::process_drag();
		
		if ( (!p_base) )
		{
			//no p_base 
			setup_billboard_matrix();
			CheckForAlphaSort();
			//this might have alpha channel stuff, so let's make sure
			return;
		}
		
		if (p_base->i_draw_mode == C_DRAW_MODE_NORMAL)
		{
			if (b_update_matrix) 
			{
				if ( /*(b_follow_ground_tilt) && */(!b_floating) )
				{
				  CEnt::think(); //physics
					
				} else
                {

                //					translateMovement();
                    CEnt::think(); //physics

                }
			}
			
		} else
		{
			
			if (p_base->i_draw_mode == C_DRAW_MODE_FOLLOW_Y)
			{
				//always look at the cam method
				setup_billboard_matrix();
			} else
				
				if (p_base->i_draw_mode == C_DRAW_MODE_FOLLOW_ALL)
					
				{
					//always look at the cam method
					setup_billboard_matrix_simple();
				}
		}
		//should we sort?
		CheckForAlphaSort();
		
	
}

