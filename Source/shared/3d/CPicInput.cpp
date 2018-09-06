#include "CPicInput.h"
#include "CMessageHeader.h"
#include "CThingFont.h"
#include "CResourceSprite.h"
#include "..\\CScriptEngine.h"
#include "..\\Uni.h"
#include "CEntItem.h"
#include "CThingList.h"
#include "..\\Sound\\ISoundManager.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPicInput::CPicInput()
{
    
    //we will be needing the global win messages sent to us
    cls_tglo.i_need_wm_messages++;
    set_type(C_THING_INPUT);
    m_i_max_string_size = 16;
    timer_blink.set_interval(500);
    b_cursor_blink_on = true;
    m_st_sound[0] = 0;

}

void CPicInput::SetInputSoundEffect(const char *p_st_sound)
{
    strcpy(m_st_sound, p_st_sound);
}

CPicInput::~CPicInput()
{
        cls_tglo.i_need_wm_messages--; //remove our reference

}

void CPicInput::ProcessAsciiKey(char c_key)
{
  
   
    if (c_key == 13)
    {
     	if (m_script_trigger_ent_id != -1)
			{
					int i_temp = m_script_trigger_ent_id;
					m_script_trigger_ent_id = -1; //don't run this again
					
                    //set the string to what we want
                     int i_temp_var = cls_tglo.p_script_engine->GetScript(0)->FindVarByName("G_ST_LAST_STRING");
                     cls_tglo.p_script_engine->GetScript(0)->GetVar(i_temp_var)->SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni(m_st_text).us_data);
     

					//run the script we're supposed to
					CEntItem *p_ent = (CEntItem*) p_my_thing_list->get_thing_by_id(i_temp);
					if (p_ent)
					{
					    p_ent->RunScriptFunction(m_st_script_trigger_function);

					}	else
					{
						log_error("CPicInput (%s), can't run trigger.  ent doesn't exist.", get_thing_name());
					}

                    //kill this object
                    b_delete_flag = true;
			}
            
 
        
        //pressed ENTER
        return;
    }
    if (m_st_sound[0]) cls_tglo.p_sound->Play(m_st_sound);
    
 
    if (c_key == 27)
    {
        //pressed escape
        return;
    }
    
    if (c_key == 8)
    {
        //they pressed backspace
        int i_length = strlen(m_st_text);
        if (i_length > 0)
        {
            
            m_st_text[i_length-1] = 0; //truncate it
        }
        
    } else


       //must be a regular key
   {
       int i_length = strlen(m_st_text);

       if (i_length < m_i_max_string_size-1)
       {
           m_st_text[i_length] = c_key;
           m_st_text[i_length+1] = 0; //add the null

       }

   }



}

void CPicInput::receive_message(CMessageHeader *p_header, void *p_message)
{
    
    if (p_header->i_message == C_THING_MSG_INPUT)
    {
        if (GetPauseLevel() < cls_tglo.GetPauseLevel()) return; //game paused for us
        
        CMessageInput *p_ray = (CMessageInput*)p_message;
        //process this keystroke
        ProcessAsciiKey(p_ray->c_key);

        return;
    }
    
   CPic::receive_message(p_header, p_message);
}



void CPicInput::think()
{
   //ok, was a key hit or whatever?

   


}


//override the draw

void CPicInput::dx_draw()
{
     //our own draw function(s)
    //do our own draw code
  	if (b_no_render_next_frame)
	{
	  return; //don't draw it this time
	}

    
 	if (m_i_fast_font_index != -1)
	{
		//draw a text message to the screen
	
      if (timer_blink.interval_reached())
      {
          b_cursor_blink_on = !b_cursor_blink_on;
      }
       int i_length;        
      if (b_cursor_blink_on)
      {
          //append a little blinky on the end, we'll take it off after drawing
          i_length = strlen(m_st_text);
         m_st_text[i_length] = '|';
         m_st_text[i_length+1] = 0;
      }
      cls_tglo.p_font->DrawFastText(m_i_fast_font_index, this->get_pos_x(),
			get_pos_y(), m_dw_font_color, m_st_text);

      if (b_cursor_blink_on)
      {
          //remove that thingie we just put there by truncating the string
          m_st_text[i_length] = 0;

      }

	}
	

	CVisual::dx_draw_prepare();

	  	if (m_i_visual_type == C_RESOURCE_SPRITE)
		{
			CResourceSprite *p_sprite = (CResourceSprite*)cls_tglo.p_manager->get(m_i_visual_id);
			p_sprite->dx_draw_blit(&m_objectMatrix, e_centering, GetVertexColor(), C_METER_NONE, 0,
                 f_scale_x, f_scale_y);
			
		}
	
	CVisual::dx_draw();

}



