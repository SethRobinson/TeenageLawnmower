/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CPicMeter.h"  
#include "CManager.h"
#include "CThing_util.h"
#include "CEntItem.h"
#include "CThingList.h"
#include "CThingFont.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CPicMeter::CPicMeter()
{
    m_i_meter_style = C_METER_LEFT_BASED;
    m_f_cur = 1;
    m_f_visual_cur = 0;
    m_f_meter_lerp = 2.8f;
    m_script_trigger_ent_id = -1;

    m_i_trigger_type = C_METER_TRIGGER_SAME_OR_HIGHER;

}


void CPicMeter::InitMeter(int i_meter_style)
{
    m_i_meter_style = i_meter_style;
}

//instantly set the meter without actually fading to it
void CPicMeter::SetMeterNoLerp(float f_percent)
{
   m_f_cur = m_f_visual_cur = f_percent;
}



void CPicMeter::SetMeterMod(float f_mod)
{
   m_f_cur += f_mod;
   
   //force it to be within bounds
   if (m_f_cur < 0) m_f_cur = 0;
   if (m_f_cur > 1) m_f_cur = 1;
}



void CPicMeter::dx_draw()
{

    //our own draw function(s)
    //do our own draw code
  	if (b_no_render_next_frame)
	{
	  return; //don't draw it this time
	}
  
    
    //we do this whole set float with target thing to avoid the "forever ALMOST there .. problem
    //that happens while lerping)
    set_float_with_target(&m_f_visual_cur, m_f_cur,m_f_meter_lerp*(cls_tglo.time()*0.01f) );
    
    m_f_visual_cur = lerp_float(m_f_visual_cur, m_f_cur, m_f_meter_lerp*cls_tglo.time());

 
     	if (b_no_render_next_frame)
	{
	  return; //don't draw it this time
	}

 	if (m_i_fast_font_index != -1)
	{
	    static char st_space[2];
        st_space[0] = 0;
        if (m_f_visual_cur < 1)
        {
          st_space[0] = ' ';
          st_space[1] = 0;
        }

        if (m_f_visual_cur < 0.10f)
        {
           //thow a leading 0 in there too
        sprintf(m_st_text, "%s0%d", st_space, int(m_f_visual_cur*100));

        }   else
        {
        
        sprintf(m_st_text, "%s%d",st_space, int(m_f_visual_cur*100));
        }
        
        
        //draw a text message to the screen
		cls_tglo.p_font->DrawFastText(m_i_fast_font_index, this->get_pos_x(),
			get_pos_y(), m_dw_font_color, m_st_text);

	}
	


	CVisual::dx_draw_prepare();

	  	if (m_i_visual_type == C_RESOURCE_SPRITE)
		{
			CResourceSprite *p_sprite = (CResourceSprite*)cls_tglo.p_manager->get(m_i_visual_id);
			p_sprite->dx_draw_blit(&m_objectMatrix, e_centering, GetVertexColor(), m_i_meter_style, m_f_visual_cur ,
                 f_scale_x, f_scale_y);
			
		}
	
	CVisual::dx_draw();

}


void CPicMeter::think()
{
        if (GetPauseLevel() < cls_tglo.GetPauseLevel()) return; //game paused for us


    	if (m_script_trigger_ent_id != -1)
			{
				//did we hit a trigger we need to?
			
              if (m_i_trigger_type == C_METER_TRIGGER_SAME_OR_HIGHER)
              {
              
              if (m_f_visual_cur >= m_f_script_trigger)
				{
					//yes!  
					int i_temp = m_script_trigger_ent_id;
					m_script_trigger_ent_id = -1; //don't run this again
					
					//run the script we're supposed to
					CEntItem *p_ent = (CEntItem*) p_my_thing_list->get_thing_by_id(i_temp);
					if (p_ent)
					{
					    p_ent->RunScriptFunction(m_st_script_trigger_function);

					}	else
					{
						log_error("CPicMeter (%s), can't run trigger.  ent doesn't exist.", get_thing_name());
					}


				}
            
              } else
               if (m_i_trigger_type == C_METER_TRIGGER_SAME_OR_LOWER)
              {
              
              if (m_f_visual_cur <= m_f_script_trigger)
				{
					//yes!  
					int i_temp = m_script_trigger_ent_id;
					m_script_trigger_ent_id = -1; //don't run this again
					
					//run the script we're supposed to
					CEntItem *p_ent = (CEntItem*) p_my_thing_list->get_thing_by_id(i_temp);
					if (p_ent)
					{
					    p_ent->RunScriptFunction(m_st_script_trigger_function);

					}	else
					{
						log_error("CPicMeter (%s), can't run trigger.  ent doesn't exist.", get_thing_name());
					}


				}
            
              } else

              {
                  log_error("%s is using an unknown trigger type (%d)", get_thing_name(),
                      m_i_trigger_type);
              }
			}


    CPic::think();
}