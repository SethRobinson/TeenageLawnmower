/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



//CPic is multipurpose class capable of buttons, 2d pics and overlaying text to the screen. 
//They are based on the CVisualMulti class so it would be easy to add support for showing 3d
//objects in a 2d mode.



#include "CPic.h"
#include "CResource.h"
#include "CResourceSprite.h"
#include "CMessageHeader.h"
#include "..\\sound\\ISoundManager.h"
#include "CThing_util.h"	   
#include "..\\CScriptEngine.h"
#include "CThingFont.h"
#include "CEntItem.h"
#include "CThingList.h"
#include "CCamera.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define C_DEFAULT_CLICK_FLASH_SPEED_MS 100
#define C_DEFAULT_CLICK_DELAY_MS 480 

CPic::CPic()
{
   	m_st_sound[0] = 0;
    m_st_highlight_sound[0] = 0;
    m_i_click_delay = C_DEFAULT_CLICK_DELAY_MS;
    m_i_click_flash_speed_ms = C_DEFAULT_CLICK_FLASH_SPEED_MS;
    m_i_pic_clicked_id = C_RESOURCE_NONE;
    m_i_pic_normal_id = C_RESOURCE_NONE;
    m_i_pic_highlight_id = C_RESOURCE_NONE;
    m_b_button_mouse_over = false;
    
    m_b_button_mode = false;
    m_b_button_down = false; //start in normal position if being used as a button	
    m_script_trigger_ent_id = -1;
    
    m_i_trigger_type = C_METER_TRIGGER_SAME_OR_HIGHER;
    
    //we're a 2d thingie, aw yeah
    m_i_script = C_SCRIPT_NONE;
    SetMipMap(1); //force the texture to use only 1 mipmap level
    
    set_thing_name("CPic");
    b_is_cpic = true;
    update_2d_matrix();
    this->set_type(C_THING_CPIC);
    set_sprite_layer(-1);
    m_b_run_script = false; //help us run the script only once on its think()
    m_i_fast_font_index = -1; //no font, disables a special "draw fast text" mode
    m_dw_font_color = D3DCOLOR_ARGB(255,255,255,0); //default color
    m_i_callback_script = C_SCRIPT_NONE;
    m_i_timer_mode = C_PIC_TEXT_TIMER_MODE_NONE;
    SetCPicVisualEffect(C_CPIC_VISUAL_EFFECT_NONE);
    SetAlphaSort(C_VISUAL_ALPHA_SORT_NO);

}	

CPic::~CPic()
{
 
    //let's remove our own refs if we're a complicated button
	if (m_b_button_mode)
	{
	   
        if (m_i_pic_clicked_id != C_RESOURCE_NONE)
		cls_tglo.p_manager->delete_ref(m_i_pic_clicked_id);
		if (m_i_pic_normal_id != C_RESOURCE_NONE)
		cls_tglo.p_manager->delete_ref(m_i_pic_normal_id);
		m_i_visual_id = C_RESOURCE_NONE; 
	}

    //if a script exists let's kill it
	if (m_i_script != ::C_SCRIPT_NONE)
	{
		 	cls_tglo.p_script_engine->RunScriptFunction(m_i_script, "ONKILL");
	
			//now actually remove the script from memoty
			cls_tglo.p_script_engine->UnloadScript(m_i_script);
			m_i_script = C_SCRIPT_NONE;
	}
}

//render text over the sprite graphic itself

void CPic::RenderText(int i_font, WCHAR *p_ws_text, int i_x, int i_y, bool b_clear_first)
{
    CResourceTex * p_tex = GetSpriteTex();
    
    if (!p_tex)
    {
        log_error("Can't render text to CPic %s, no texture assigned to it yet.",
            this->get_thing_name());
        return;
    }
    //D3DCOLOR_ARGB(a,r,g,b)) 
    D3DCOLOR old_color = cls_tglo.p_font->GetPenColor(i_font);
    
    cls_tglo.p_font->SetPenColor(i_font, m_dw_font_color);
    //cls_tglo.p_font->SetPenOptions(i_font, DT_CENTER | DT_WORDBREAK ) ;
    
   
    if (b_clear_first)
    {
        p_tex->FillWithColor(D3DCOLOR_ARGB(0,0,0,0));
    }
    
    RECT rect_box;
    SetRect(&rect_box, i_x,i_y,0,0);

    p_tex->RenderText(i_font, &rect_box, p_ws_text);
    
    //put the old color back
    cls_tglo.p_font->SetPenColor(i_font, old_color);
}


void CPic::SetupFastTextRender(int i_fast_font_index,  const char *p_st_text)
{
    assert(strlen(p_st_text) < C_PIC_MAX_INPUT_SIZE && "Passed too big a text thing.");
    m_i_fast_font_index = i_fast_font_index; //if not -1, enables text mode
	strcpy(m_st_text, p_st_text);
	m_i_timer_mode = C_PIC_TEXT_TIMER_MODE_NONE; //disable timer crap
}

float CPic::EntGetTime()
{
	return m_f_cur_time;
}

void CPic::EntSetTime(float f_time)
{
	m_f_cur_time = f_time;

	//udate it visually
	TextFormat();
}

int CPic::SetButtonGraphic(int i_id)
{
	//set ourselves to  the correct sprite
	if (i_id == 0)
	{
		//special case, no visual id so just don't draw us
		this->set_no_draw(true);
	   return 0;
	}
	m_i_visual_id = i_id;
	this->set_no_draw(false);
    return 0;
}
void CPic::TextFormat()
{
    
    if (m_i_timer_mode == C_PIC_TEXT_TIMER_MODE_INT_CENTER)
    {
        //we should add centering...
        sprintf(m_st_text, "%.0f", m_f_cur_time); 
    }
    
    if (m_i_timer_mode == C_PIC_TEXT_TIMER_MODE_CLOCK)
    {
        //format it like a clock
        int i_min = int(m_f_cur_time/60);
        int i_sec = int(m_f_cur_time - (i_min*60));
        
        
        if (i_sec < 10)
        {
            //add leading 0
          sprintf(m_st_text, "%d:0%d", i_min, i_sec);
        }   else
        {
          sprintf(m_st_text, "%d:%d", i_min, i_sec);
        }

    }
}

void CPic::SetTrigger(int i_trigger_type, float f_trigger, int id_ent, const char *p_st_function)
{
    m_i_trigger_type = i_trigger_type;
    m_f_script_trigger = f_trigger;
	m_script_trigger_ent_id = id_ent;
	strcpy(m_st_script_trigger_function, p_st_function);
}

void CPic::EntSetFastTimer(int i_mode, float f_start, float f_time_mod, int i_timer)
{
	this->m_f_cur_time = f_start;
	m_f_time_mod = f_time_mod;
	m_i_timer_mode = i_mode;
	m_i_text_wait = i_timer;
	
	TextFormat(); //format the number
	m_timer_text.reset();
	m_timer_text.set_interval(m_i_text_wait);
}

void CPic::InitAsButton(const char *p_st_normal, const char *p_st_clicked)
{
   	m_b_button_mode = true;
	
	//this is a bit cooky, but we're going to init both sprites and save the resulting visual id's
	char st_name[80];
	sprintf(st_name, "BUTTON%dclick", get_id());

	if (init_visual_sprite(st_name, p_st_clicked))
	{
	   m_i_pic_clicked_id = m_i_visual_id;
	   if (cls_tglo.IsRunning()) cls_tglo.p_manager->load_if_needed(m_i_visual_id);
	}
	
	
	if (p_st_normal[0] == 0)
	{
		//the pic is only shown when clicked, there is no "normal" image
	    //initially don't show us then
		this->set_no_draw(true);
   		
	}	else
	{
        sprintf(st_name, "BUTTON%dnorm", get_id());
        
		if (init_visual_sprite(st_name, p_st_normal))
		{
			m_i_pic_normal_id = m_i_visual_id;
			if (cls_tglo.IsRunning()) cls_tglo.p_manager->load_if_needed(m_i_visual_id);
		}
	}
}


bool CPic::SetButtonHighlight(const char *p_st_fname)
{
   assert(m_b_button_mode && "Should only use this after setting InitAsAButton");
    
    char st_name[128];
    sprintf(st_name, "BUTTON%dhlite", get_id());
    bool b_success = true;
    bool b_no_draw = this->get_no_draw();

    //replace our image with a new one, just to init it correctly.  Then switch back to the old we had.
    int i_temp_visual = m_i_visual_id;
    
    if (init_visual_sprite(st_name, p_st_fname))
    {
        m_i_pic_highlight_id = m_i_visual_id;
        if (cls_tglo.IsRunning()) cls_tglo.p_manager->load_if_needed(m_i_visual_id);
        m_i_visual_id = i_temp_visual;
    } else
    {
        LogError("Error initting %s.", p_st_fname);
        b_success = false;
    }
    
    SetButtonGraphic(m_i_visual_id);
    set_no_draw(b_no_draw);
    
    return b_success;
}


void CPic::set_sprite_layer(int i_layer)
{
  set_sort(200000+i_layer);
 // log_msg("CPIC %s set layer to %d.", get_thing_name(), 100000+i_layer);
 
}


void CPic::SetClickSound(char *p_st_sound)
{
	strcpy(m_st_sound, p_st_sound); 
}

void CPic::SetHighlightSound(char *p_st_sound)
{
    strcpy(m_st_highlight_sound, p_st_sound); 
}

int CPic::SetClickScript(char *p_st_script)
{

			m_i_script = cls_tglo.p_script_engine->CreateScript();
				//let's add any script specific variables it needs
				int i_temp_var = cls_tglo.p_script_engine->GetScript(m_i_script)->AddVar("i_my_id", ::C_SCRIPT_VAR_INT);
				cls_tglo.p_script_engine->GetScript(m_i_script)->GetVar(i_temp_var)->SetInt(this->get_id());

	
			int i_result = cls_tglo.p_script_engine->load_and_compile_script(p_st_script, m_i_script);
			if (i_result != ::C_SCRIPT_NONE)
			{
				//successfull load
			m_b_run_script = true;
			} else
			{
				//not succesfull.  Kill the script we had created earlier.  Error message wil be
				//handled by the CScriptEngine.
				cls_tglo.p_script_engine->UnloadScript(m_i_script);
				m_i_script = C_SCRIPT_NONE;
			}
	 return m_i_script;
}

void CPic::SetCallbackOnTimer(int i_script, float f_time)
{
	m_f_callback_time = f_time;
	m_i_callback_script = i_script;

}
void CPic::think()
{
    CheckForAlphaSort();
    CVisual::think();
     if (GetPauseLevel() < cls_tglo.GetPauseLevel()) return; //game paused for us

    if (m_b_run_script)
	{
		m_b_run_script = false;
		cls_tglo.p_script_engine->RunScriptFunction(m_i_script, "ONCREATE");
		cls_tglo.p_script_engine->RunScriptFunction(m_i_script, "MAIN");
	}


	//process text stuff if we have to
	if (m_i_timer_mode != C_PIC_TEXT_TIMER_MODE_NONE)
	{
	  if (m_timer_text.interval_reached())
	  {
		  //process change to number and re-format it
		   m_f_cur_time += m_f_time_mod;
		   TextFormat();
	       if (m_i_callback_script != C_SCRIPT_NONE)
		   {
			   if (m_f_cur_time == m_f_callback_time) 
			   cls_tglo.p_script_engine->GetScript(m_i_callback_script)->StartUpAfterWait();
				
		   }
	 
      
      
      
      	if (m_script_trigger_ent_id != -1)
			{
				//did we hit a trigger we need to?
			
              if (m_i_trigger_type == C_METER_TRIGGER_SAME_OR_HIGHER)
              {
              
              if (m_f_cur_time >= m_f_script_trigger)
				{
					//yes!  
					int i_temp = m_script_trigger_ent_id;
					m_script_trigger_ent_id = -1; //don't run this again
					
                    m_i_timer_mode = C_PIC_TEXT_TIMER_MODE_NONE;

					//run the script we're supposed to
					CEntItem *p_ent = (CEntItem*) p_my_thing_list->get_thing_by_id(i_temp);
					if (p_ent)
					{
					    p_ent->RunScriptFunction(m_st_script_trigger_function);

					}	else
					{
						log_error("CPic (%s), can't run trigger.  ent doesn't exist.", get_thing_name());
					}

                    

				}
            
              } else
               if (m_i_trigger_type == C_METER_TRIGGER_SAME_OR_LOWER)
              {
              
              if (m_f_cur_time <= m_f_script_trigger)
				{
					//yes!  
					int i_temp = m_script_trigger_ent_id;
					m_script_trigger_ent_id = -1; //don't run this again
                    m_i_timer_mode = C_PIC_TEXT_TIMER_MODE_NONE;
	
					//run the script we're supposed to
					CEntItem *p_ent = (CEntItem*) p_my_thing_list->get_thing_by_id(i_temp);
					if (p_ent)
					{
					    p_ent->RunScriptFunction(m_st_script_trigger_function);

					}	else
					{
						log_error("CPic (%s), can't run trigger.  ent doesn't exist.", get_thing_name());
					}


				}
            
              } else

              {
                  log_error("%s is using an unknown trigger type (%d)", get_thing_name(),
                      m_i_trigger_type);
              }
			}
    
      }

	}
	

	if (m_b_button_mode)
  {
	  //we handle out own clicks
        
        if (this->got_clicked())
        {
            
            if (m_st_sound[0]) cls_tglo.p_sound->Play(CDir(m_st_sound));
            this->set_clicked(false);
            m_timer_click.set_interval(m_i_click_delay);
            m_timer_click_flash_speed.set_interval(m_i_click_flash_speed_ms); //how fast the button flashes during the click delay
            m_b_button_down = true;
            SetButtonGraphic(m_i_pic_clicked_id);
            m_b_blink_highlighted = true;
        } else
        {
            if (m_b_button_down)
            {
                if (m_timer_click.interval_reached())
                {
                    //but done being clicked, let's actually run the script we're supposed to.
                    if (m_i_script != C_SCRIPT_NONE)	  
                    {
                        cls_tglo.p_script_engine->RunScriptFunction(m_i_script, "ONCLICK");
                    }
                    
                    m_b_button_down = false;
                    m_b_button_mouse_over = false; //make sure this gets reset
                    SetButtonGraphic(m_i_pic_normal_id);
                    return;
                } else
                {
                    //flash the button if applicable
                    if (m_timer_click_flash_speed.interval_reached())
                    {

                        if (m_b_blink_highlighted)
                        {
                            //switch to unhighlighted
                             SetButtonGraphic(m_i_pic_normal_id);
                        } else
                        {
                            //switch back to highlighted
                            SetButtonGraphic(m_i_pic_clicked_id);
                        }
                        m_b_blink_highlighted = !m_b_blink_highlighted;
                    }


                }
            }
            
            //actually, if the mouse is over us, use the mouse over pic if applicable
            
            if (!m_b_button_down && m_i_pic_highlight_id != C_RESOURCE_NONE)
            {
                //see if our status has changed..
                if (m_b_button_mouse_over)
                {
                    //did we exit the area?
                    if (!InsideRect2D(cls_tglo.p_cls_input->GetMouseX(), cls_tglo.p_cls_input->GetMouseY()))
                    {
                        //yes, turn off the highlight pic
                        SetButtonGraphic(m_i_pic_normal_id);
                        m_b_button_mouse_over = false;
                    }
                } else
                {
                    //did we enter the button's rect area?

                    if (InsideRect2D(cls_tglo.p_cls_input->GetMouseX(), cls_tglo.p_cls_input->GetMouseY()))
                    {
                        if (m_st_highlight_sound[0]) cls_tglo.p_sound->Play(CDir(m_st_highlight_sound));
                        //yes, turn on the highlight pic
                        SetButtonGraphic(m_i_pic_highlight_id);
                        m_b_button_mouse_over = true;
                    }
                }
                
            }
            
        }

  }


    


}



void CPic::dx_restore()
{
//    log_msg("Restoring cpic %s.", get_thing_name());
	CVisualMulti::dx_restore();
}

void CPic::receive_message(CMessageHeader *p_header, void *p_message)
{
    
    if (p_header->i_message == ::C_THING_MSG_KILL)
    {
        //we've been instructed to kill ourself
        if (cls_tglo.GetPauseLevel() < GetPauseLevel()) return; //ignore it!
        if (!GetIgnoreKillMessage())
        {
            this->b_delete_flag = true;
        }
        return;
    }
    
    if (p_header->i_message == ::C_THING_RAY_PICK)
    {
        if (GetPauseLevel() < cls_tglo.GetPauseLevel()) return; //game paused for us
        
        if (b_clickable)
        {
            CMessageRayPick *p_ray = (CMessageRayPick*)p_message;
            
            //log_msg("Ray pick on %s", this->get_thing_name());
            //CVisualMulti::ray_pick(p_ray); //send it the whole pack, why not
            if (m_i_visual_type == ::C_RESOURCE_SPRITE)
            {
                hit_test_sprite(p_ray);
                return;
            }
        }
        return;
    }
}

void CPic::hit_test_sprite(CMessageRayPick *p_ray)
{
   //assume we did hit, and check it
 	D3DXVECTOR3 vec_hit = D3DXVECTOR3(p_ray->p_vec_mouse->x, p_ray->p_vec_mouse->y, 0);

	if ( ((CResourceSprite*)GetResourcePointer())->alpha_test_2d(&m_objectMatrix, e_centering, &vec_hit))
	{
	  //it's a hit
		//log_msg("Hit %s.",this->get_thing_name());
		this->set_clicked(true);
		this->m_pt_last_hit.x = vec_hit.x - get_pos_x();
		this->m_pt_last_hit.y = vec_hit.y - get_pos_y();
	}
};

void CPic::GetRect(RECT * p_rect_out)
{
    assert(!"Not ready yet!");
    
/*

    CResourceSprite *p_sprite = (CResourceSprite*)cls_tglo.p_manager->get(i_visual_id);
    assert(i_visual_id && "Can't hittest a bad image, asshole!");
    p_sprite->GetTransformedRect(&m_objectMatrix, p_rect_out);
   */

}

bool CPic::InsideRect2D(int i_x, int i_y)
{
    
    D3DXVECTOR3 vec_hit = D3DXVECTOR3(float(i_x),float(i_y), 0);
    return ((CResourceSprite*)GetResourcePointer())->alpha_test_2d(&m_objectMatrix, e_centering, &vec_hit);
    
}

void CPic::dx_draw()
{
     //our own draw function(s)
 
  	if ( b_no_render_next_frame)
	{
	  return; //don't draw it this time
	}

    ProcessBlink();

    if (m_i_fast_font_index != -1)
    {
        //draw a text message to the screen
        cls_tglo.p_font->DrawFastText(m_i_fast_font_index, this->get_pos_x(),
            get_pos_y(), m_dw_font_color, m_st_text);
    }
    

	
	  	if (m_i_visual_type == C_RESOURCE_SPRITE)
		{

            assert(GetCPicVisualEffect() == C_CPIC_VISUAL_EFFECT_NONE && "Visual effects aren't supported for this type yet");
            CVisual::dx_draw_prepare();
		  
            CResourceSprite *p_sprite = (CResourceSprite*)cls_tglo.p_manager->get(m_i_visual_id);
		    if (p_sprite)
            {
            
                 
                p_sprite->dx_draw_blit(&m_objectMatrix, e_centering, GetVertexColor(), C_METER_NONE, 0, 1,1);
                
            } else
            {
                //log_msg("Error, CPic %s grapghi)
            }
			 	CVisual::dx_draw();
		} else
            if (m_i_visual_type == C_RESOURCE_PMESH)
            {
               //SetZDisable(true);
             
                D3DXVECTOR3 vec_pos, vec_temp;
                vec_temp = getObjectPosition();
                
                //get screen position
                cls_tglo.p_camera->get_screen_to_world_point(&vec_pos, get_pos_x(), get_pos_y(), 0.7f); //close to screen
                setObjectPosition(vec_pos);
              
               //setAngleY(getAngleY()+(1*cls_tglo.GetDelta()));
             
               //setAngleY(sin(float(cls_tglo.get_game_age())/20.f)/10);
                switch(m_i_cpic_visual_effect)
                {
                case C_CPIC_VISUAL_EFFECT_BOB:
                    set_tilt(sin(float(cls_tglo.get_game_age())/17.f)/10);
                    break;
                    
                default:  ;
                };
           
              setup_billboard_matrix_simple();
              //   set_translate_asap(false); //we're handling it ourself
//              cls_tglo.p_d3dDevice->SetRenderState( D3DRS_CULLMODE,     D3DCULL_CCW );
              
               CVisualMulti::dx_draw();
               //cls_tglo.p_d3dDevice->SetRenderState( D3DRS_CULLMODE,     D3DCULL_NONE );
               

                //Move it back
                setObjectPosition(vec_temp);

            }   else
            if (m_i_visual_type == C_RESOURCE_NONE)
            {
                
            } else

        {

            assert(!"Don't know how to handle this kind of visual in CPic yet.");
        }
	

}



