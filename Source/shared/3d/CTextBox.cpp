/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CTextBox.h"
#include "CPic.h"
#include "CThingList.h"
#include "CResourceTex.h"
#include "CThingFont.h"
#include "..\\uni.h"
#include "CMessageHeader.h"
#include "..\\CScriptEngine.h"
#include "..\\sound\\ISoundManager.h"
#include "CThing_Util.h"
#include "ent_utils.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTextBox::CTextBox()
{
   m_p_pic_bg = NULL;
   m_p_pic_text = NULL;
   m_p_wchar = NULL;
   m_p_pic_down = NULL;
   m_p_pic_up = NULL;
   m_p_pic_ok = NULL;
   
   m_dw_options = DT_LEFT|DT_WORDBREAK;
   m_dw_color = D3DCOLOR_ARGB(255,255,255,255);
   m_i_total_lines = 0; //how many lines of text we have based on this font size.
   m_i_font = 1; //default font we should use.
   m_i_display_lines = 0; //how many we can show at once
   m_i_cur_line = 1;
   set_type(C_THING_TEXTBOX);
   m_b_page_up_dik = DIK_PGUP;
   m_b_page_down_dik = DIK_PGDN;
   set_thing_name("CTextBox");
   this->set_sort(-2); //get processed early
   b_update_text = false;
   m_i_layer = 8;
   m_i_script = C_SCRIPT_NONE; 
   m_dw_inactive_color = D3DCOLOR_ARGB(0,255,255,255);
   m_st_click_sound[0] = 0;
   strcpy(m_st_click_sound, "sound\\click2.wav");
   m_f_fade_speed = 1;
   m_i_fade_control = C_TEXTBOX_NO_FADE;
   m_f_fade_alpha = 1;
   m_b_skip_draw_of_next_frame = true;
   m_b_wait_to_click = false; 
   m_i_res_adjust = C_RES_ADJUST_NONE;
   m_f_res_adjust_save_x = 0;
   m_f_res_adjust_save_y = 0;	   
  
}

CTextBox::~CTextBox()
{
    ClearText();
    
    //kill any pics that were associated with this
    
    
    if (m_p_pic_down)
    {
        KillByAddress(p_my_thing_list,  m_p_pic_down);
    }
    
    if (m_p_pic_up)
    {
        
        KillByAddress(p_my_thing_list,  m_p_pic_up);
        
    }
    if (m_p_pic_text) 
    {
        KillByAddress(p_my_thing_list,   m_p_pic_text);
        
    }
    if (m_p_pic_bg)
    {
        KillByAddress(p_my_thing_list,   m_p_pic_bg);
        
    }
    if (m_p_pic_ok)
    {
        KillByAddress(p_my_thing_list,  m_p_pic_ok);
        
    }
    
}

void CTextBox::SetFontColor(DWORD dw_color)
{
	m_dw_color = dw_color;
}

void CTextBox::SetClickSound(char *st_name)
{
	strcpy(m_st_click_sound, st_name);

}

void CTextBox::SetFadeUp(float f_speed)
{
    m_i_fade_control = C_TEXTBOX_UP;
    m_f_fade_speed = f_speed; 
    m_f_fade_alpha = 0; //default to 0

   D3DXCOLOR col = D3DCOLOR_COLORVALUE(1,1,1,m_f_fade_alpha);

  if (m_p_pic_text)
    {
        m_p_pic_text->VertexColor(col);
    }

    if (m_p_pic_ok)
    {
        m_p_pic_ok->VertexColor(col);
    }
   if (m_p_pic_bg)
    {
      m_p_pic_bg->VertexColor(col);
    }

}


void CTextBox::SetFadeDown(float f_speed)
{
    m_i_fade_control = C_TEXTBOX_DOWN;
    m_f_fade_speed = f_speed;
}


void CTextBox::dx_create()
{
	b_update_text = true; //we'll update next frame which will give our surfaces a chance
	//to actually get created.

}  

void CTextBox::dx_restore()
{

	b_update_text = true; //we'll update next frame which will give our surfaces a chance
	//to actually get created.
   	if (m_i_res_adjust != C_RES_ADJUST_NONE)
    {
        SetResAdjust(m_i_res_adjust, m_i_res_correct_base_x, m_i_res_correct_base_y);
        set_xy(m_f_res_adjust_save_x, m_f_res_adjust_save_y);
    }

}


void CTextBox::SetScriptCallbackOnOk(int i_script)
{
	if (m_p_pic_ok)
	{
 	    m_p_pic_ok->set_clickable(true);
		m_p_pic_ok->set_no_draw(false); //show it
	}

	m_i_script = i_script;
}

int CTextBox::AddDownScroll(D3DXVECTOR2 pt_pos, char st_up[256], char st_down[256])
{
 
  m_pt_down_offset = pt_pos;
  
  m_p_pic_down = new CPic();
  p_my_thing_list->add_new_thing(m_p_pic_down);
  m_p_pic_down->InitAsButton(st_up, st_down);
  m_p_pic_down->set_sprite_layer(m_i_layer+2);
  m_p_pic_down->set_clickable(true);
  m_p_pic_down->dx_create();
  m_p_pic_down->SetPauseLevel(GetPauseLevel());
  UpdateScrollDownPosition();
  return m_p_pic_down->get_id();
}


int CTextBox::AddUpScroll(D3DXVECTOR2 pt_pos, char st_up[256], char st_down[256])
{
 
  m_pt_up_offset = pt_pos;
  
  m_p_pic_up = new CPic();
  p_my_thing_list->add_new_thing(m_p_pic_up);
  m_p_pic_up->InitAsButton(st_up, st_down);
  m_p_pic_up->set_sprite_layer(m_i_layer+2);
  m_p_pic_up->set_clickable(true);
  m_p_pic_up->dx_create();
  m_p_pic_up->SetPauseLevel(GetPauseLevel());

  UpdateScrollUpPosition();
  return m_p_pic_up->get_id();
}

int CTextBox::AddOkButton(D3DXVECTOR2 pt_pos, const char* p_st_up, const char* p_st_down)
{
 
  m_pt_ok_offset = pt_pos;
  
  m_p_pic_ok = new CPic();
  p_my_thing_list->add_new_thing(m_p_pic_ok);
  m_p_pic_ok->InitAsButton(p_st_up, p_st_down);
  m_p_pic_ok->set_sprite_layer(m_i_layer+2);
  m_p_pic_ok->set_clickable(false);
//  m_p_pic_ok->SetButtonHighlight(p_st_down);
  m_p_pic_ok->dx_create();
  UpdateScrollUpPosition();
   m_p_pic_ok->SetPauseLevel(GetPauseLevel());

  m_p_pic_ok->set_no_draw(true); //hide it unless needed later
  return m_p_pic_ok->get_id();
}


void CTextBox::ClearText()
{
	SAFE_FREE(m_p_wchar);
	m_i_cur_line = 1; //if we're scrolled down, move er back up
    b_update_text = true;
}



void CTextBox::UpdateScrollDownPosition()
{
    if (!m_p_pic_down) return;

    m_p_pic_down->set_xy(get_pos_x()+m_pt_down_offset.x, get_pos_y() + m_pt_down_offset.y);
	if (m_i_cur_line+ m_i_display_lines  <  m_i_total_lines)
	{
		//this button should be active.
		m_p_pic_down->VertexColor(D3DCOLOR_ARGB(255,255,255,255));
        m_p_pic_down->SetBlink(true);
    }	else
	{
        m_p_pic_down->SetBlink(false);
		m_p_pic_down->VertexColor(m_dw_inactive_color);
	}

	m_p_pic_down->set_sprite_layer(m_i_layer+2);

}

void CTextBox::UpdateScrollUpPosition()
{
    if (!m_p_pic_up) return;

	m_p_pic_up->set_xy(get_pos_x()+m_pt_up_offset.x, get_pos_y() + m_pt_up_offset.y);

	if (m_i_cur_line > 1)
	{
		m_p_pic_up->SetBlink(true);
        m_p_pic_up->VertexColor(D3DCOLOR_ARGB(255,255,255,255));
	}	else
	{
		m_p_pic_up->SetBlink(false);
        m_p_pic_up->VertexColor(m_dw_inactive_color);
	}
		m_p_pic_up->set_sprite_layer(m_i_layer+2);

}


void CTextBox::UpdateOkPosition()
{
    if (!m_p_pic_ok) return;
	m_p_pic_ok->set_xy(get_pos_x()+m_pt_ok_offset.x, get_pos_y() + m_pt_ok_offset.y);
}


void CTextBox::set_xy(float f_x, float f_y)
{
    m_vObjectPosition.x = f_x;
    m_vObjectPosition.y = f_y;
    
    //add any special offsets, these are used for interface items so they won't look too
    //funky at different resolutions
    
    if (m_i_res_adjust == C_RES_ADJUST_CENTER_BOTTOM) //center and bottom of screen
    {
        
        //save originals
        m_f_res_adjust_save_x = m_vObjectPosition.x;
        m_f_res_adjust_save_y = m_vObjectPosition.y;
        
        if (cls_tglo.get_screen_width() != m_i_res_correct_base_x)
        {
            //adjust x
            
            m_vObjectPosition.x += (cls_tglo.get_screen_width() - m_i_res_correct_base_x) / 2;
            
            m_vObjectPosition.y += 	(cls_tglo.get_screen_height() - m_i_res_correct_base_y);
            
        }
    }  else
        
        if (m_i_res_adjust == C_RES_ADJUST_NORMAL) //center 
        {
            
            //save originals
            m_f_res_adjust_save_x = m_vObjectPosition.x;
            m_f_res_adjust_save_y = m_vObjectPosition.y;
            
            if (cls_tglo.get_screen_width() != m_i_res_correct_base_x)
            {
                //adjust x
                
                m_vObjectPosition.x += (cls_tglo.get_screen_width() - m_i_res_correct_base_x) / 2;
                
                m_vObjectPosition.y += 	(cls_tglo.get_screen_height() - m_i_res_correct_base_y) /2;
                
            }
        } else
            if (m_i_res_adjust == C_RES_ADJUST_FULL) //fit screen
            {
                
                //save originals
                m_f_res_adjust_save_x = m_vObjectPosition.x;
                m_f_res_adjust_save_y = m_vObjectPosition.y;
                
                if (cls_tglo.get_screen_width() != m_i_res_correct_base_x)
                {
                    //adjust x
                    
                    
                    m_vObjectPosition.x = (f_x / float(m_i_res_correct_base_x)) * cls_tglo.get_screen_width();
                    
                    m_vObjectPosition.y = (f_y / float(m_i_res_correct_base_y)) * cls_tglo.get_screen_height();
                    
                }
            } 
            
            
            
}


void CTextBox::SetResAdjust(int i_type, int i_base_x, int i_base_y)
{
    
    assert(i_type < C_RES_ADJUST_COUNT && "Unknown type of res adjust");
    if (m_i_res_adjust == C_RES_ADJUST_NONE)
    {
        //first time, imprint current x/y as original
        //save originals
        m_f_res_adjust_save_x = m_vObjectPosition.x;
        m_f_res_adjust_save_y = m_vObjectPosition.y;
        
    }
    
    m_i_res_adjust = i_type;
    m_i_res_correct_base_x = i_base_x;
    m_i_res_correct_base_y = i_base_y;
    
    switch (i_type)
    {
        
    case C_RES_ADJUST_FULL:
        break;
        
    default:;
        
          
    }
    
    //activate it
    set_xy(m_f_res_adjust_save_x, m_f_res_adjust_save_y);
    
}


void CTextBox::UpdateTextPosition()
{
	if (m_p_pic_text)
	{
		
		m_p_pic_text->set_xy(get_pos_x()+m_pt_text_offset.x, get_pos_y() + m_pt_text_offset.y);
		m_p_pic_text->set_sprite_layer(m_i_layer+1);
   	}
	
	if (m_p_pic_bg)
	{
		m_p_pic_bg->set_xy(get_pos_x(), get_pos_y());
		m_p_pic_bg->set_sprite_layer(m_i_layer);
	}
	
	UpdateScrollDownPosition();
    UpdateScrollUpPosition();
	UpdateOkPosition();
}


void CTextBox::UpdatePositions()
{
	UpdateTextPosition();
}


void CTextBox::SetLayer(	int i_layer)
{
	m_i_layer = i_layer; 
	UpdateTextPosition();
}

//returns id of text for some unknown reason
int CTextBox::InitTextBox(D3DXVECTOR2 pt_size, D3DXVECTOR2 pt_offset, D3DXVECTOR2 pt_limit)
{
	//create a blank resource and assign a pic to it
	m_pt_text_offset = pt_offset;
	m_pt_text_limit = pt_limit;
	m_p_pic_text = new CPic();
	int i_id = p_my_thing_list->add_new_thing(m_p_pic_text);
	//first create a new unique resource name
	char st_text[32];
	sprintf(st_text, "TBTEXT%d", get_id());
	m_p_pic_text->init_visual_sprite_blank(st_text, pt_size, D3DCOLOR_ARGB(0,0,0,0));
	m_p_pic_text->set_sprite_layer(m_i_layer+1);
     m_p_pic_text->SetPauseLevel(GetPauseLevel());

    UpdateTextPosition();
	//go ahead and load it if we are currently running
	if (cls_tglo.IsRunning())
	{
		m_p_pic_text->dx_create();
	} else
	{
		log_error("Shouldn't init a text box when the engine isn't running yet.. :/");
	}
	
	return i_id;
}

void CTextBox::AddText(WCHAR *p_wchar)
{
	int i_orig_size = 0;
	if (m_p_wchar) i_orig_size =  wcslen(m_p_wchar);
	int i_new_size = wcslen(p_wchar);
	m_p_wchar = (WCHAR*)realloc(m_p_wchar, (i_orig_size+i_new_size+1)*2);	//the +1 is for the last NULL
	//now copy the new stuff into it
    memcpy(&m_p_wchar[i_orig_size], p_wchar, (i_new_size*2));
	m_p_wchar[i_orig_size+i_new_size] = 0; //ending null
	b_update_text = true;
}					 


void CTextBox::UpdateText(bool b_scroll_to_end)
{

	b_update_text = false;
	if (!m_p_pic_text) return;
    
	//let's write our options
    CResourceTex * p_tex = m_p_pic_text->GetSpriteTex();
    
	cls_tglo.p_font->SetPenColor(m_i_font, m_dw_color);
	cls_tglo.p_font->SetPenOptions(m_i_font, m_dw_options);
	m_i_total_lines = cls_tglo.p_font->ComputeHowManyLines(m_i_font, m_p_wchar, int(m_pt_text_limit.x));
	m_i_display_lines = int(m_pt_text_limit.y / float(cls_tglo.p_font->GetRealFontHeight(m_i_font)));

	if (b_scroll_to_end)
	{
		//let's scroll down
		m_i_cur_line = m_i_total_lines-m_i_display_lines;
		if (m_i_cur_line < 1) m_i_cur_line = 1;
	}
	
	//	log_msg("Drawing %d lines, can only display %d though.", m_i_total_lines, m_i_display_lines);
	RECT rect_box;
	SetRect(&rect_box, 0,0,int(m_pt_text_limit.x), int(m_pt_text_limit.y));
	//if all 0's were sent it would just set it to the texture width/height.

	//let's clear our text palette first
	p_tex->FillWithColor(D3DCOLOR_ARGB(0,0,0,0));

	//ok, which character should we start drawing with?
	int i_start_char = cls_tglo.p_font->ComputeStartingChar(m_i_font, m_p_wchar, int(m_pt_text_limit.x), m_i_cur_line);
	
	//set the font color
	 if (m_p_wchar)
     {
     
	        p_tex->RenderText(m_i_font, &rect_box, &m_p_wchar[i_start_char]);
     }
    UpdateTextPosition();

}

void CTextBox::AddTextA(char *p_text)
{
	AddText(uni(p_text).us_data);
}


void CTextBox::SetText(WCHAR *p_wchar)
{
    ClearText();
	AddText(p_wchar);
}

void CTextBox::SetTextA(char *p_text)
{
    ClearText();
	AddText(uni(p_text).us_data);
}

int CTextBox::InitBG(char st_name[256])
{
    //kill the BG if it's already loaded
    
    if (m_p_pic_bg)
        {
          	m_p_pic_bg->b_delete_flag = true;
            m_p_pic_bg->set_no_draw(true);
            m_p_pic_bg = NULL;

        }


    if (st_name[0] == 0)
    {
        //they don't want a BG at all.  Let's clear it if there is one.
        return 0;
    } 
    
    //make kind of a generic BG for our text
   	m_p_pic_bg = new CPic();
	int i_id = p_my_thing_list->add_new_thing(m_p_pic_bg);
	m_p_pic_bg->init_visual_sprite(st_name, st_name);
	m_p_pic_bg->set_sprite_layer(m_i_layer);
	m_p_pic_bg->set_xy(get_pos_x(), get_pos_y());
       m_p_pic_bg->SetPauseLevel(GetPauseLevel());


    if (cls_tglo.IsRunning())
	{
		m_p_pic_bg->dx_create();
	}
		return i_id;
}

int CTextBox::InitBGBlank(D3DXVECTOR2 pt_size, DWORD dw_color)
{
	//make BG out of a solid color
   	m_p_pic_bg = new CPic();
	int i_id = p_my_thing_list->add_new_thing(m_p_pic_bg);

	char st_sprite_name[256];
	sprintf(st_sprite_name, "PicBG%d", m_p_pic_bg->get_id());
	m_p_pic_bg->init_visual_sprite_blank(st_sprite_name, pt_size, dw_color);
	m_p_pic_bg->set_sprite_layer(m_i_layer);
         m_p_pic_bg->SetPauseLevel(GetPauseLevel());

	m_p_pic_bg->set_xy(get_pos_x(), get_pos_y());

	if (cls_tglo.IsRunning())
	{
		m_p_pic_bg->dx_create();
	}

	return i_id;

}
	   

void CTextBox::receive_message(CMessageHeader *p_header, void *p_message)
{

	if (p_header->i_message == ::C_THING_MSG_KILL)
	{
        //if our pause level is higher than the killing process we won't be affected
        
        if (GetPauseLevel() <= cls_tglo.GetPauseLevel())
    {
       //we've been instructed to kill ourself
	  this->b_delete_flag = true;
	  return;
    }
	}

}

void CTextBox::ProcessFades()
{
    
    if (m_i_fade_control == C_TEXTBOX_UP)
    {
    
        m_f_fade_alpha += cls_tglo.time()*m_f_fade_speed;
        if (m_f_fade_alpha >= 1.0f) 
        {
            m_f_fade_alpha = 1;
            m_i_fade_control = C_TEXTBOX_NO_FADE;
        }
    }
  
    if (m_i_fade_control == C_TEXTBOX_DOWN)
    {
    
        m_f_fade_alpha -= cls_tglo.time()*m_f_fade_speed;
        if (m_f_fade_alpha <= 0.0f) 
        {
            m_f_fade_alpha = 0;
            m_i_fade_control = C_TEXTBOX_NO_FADE;
        }
    }
    
  
    D3DXCOLOR col = D3DCOLOR_COLORVALUE(1,1,1,m_f_fade_alpha);

    //set everything to that alpha

    if (m_p_pic_text)
    {
        m_p_pic_text->VertexColor(col);
    }

    if (m_p_pic_ok)
    {
        m_p_pic_ok->VertexColor(col);
    }
   if (m_p_pic_bg)
    {
      m_p_pic_bg->VertexColor(col);
    }
  
}


void CTextBox::think()
{
 if (GetPauseLevel() < cls_tglo.GetPauseLevel()) return; //game paused for us
 if (b_delete_flag) return; //scheduled to die, don't think here

 if (m_b_skip_draw_of_next_frame)
 {
     //don't process input until the next frame
     m_b_skip_draw_of_next_frame = false;
     return;
 }

  if (m_i_fade_control != C_TEXTBOX_NO_FADE)
     {
         ProcessFades();
     }
  
    
    if (b_update_text)
	{
		UpdateText(false);
	}

    
    if (m_b_wait_to_click)
    {
        if (m_timer_wait_to_click.interval_reached())
        {

       if (m_i_script != C_SCRIPT_NONE)
			 {
				 
				 
				 //we don't need to set any script vars, just run it
				 cls_tglo.p_script_engine->GetScript(m_i_script)->StartUpAfterWait();
				 //let's call the script back
			 }
       m_b_wait_to_click = false;
        }

      return;
    }
    
    
    if (m_b_page_down_dik != 0)
	{
	  if (cls_tglo.p_cls_input->is_key_tapped(m_b_page_down_dik))
	  {
		if (m_p_pic_down)
		{
		   m_p_pic_down->set_clicked(true);
		}	else
		{
			log_error("Got a custom page move command, but no up/down icons exist to set.");
		}
	  }
	}

	 if (m_b_page_up_dik != 0)
	{
	  if (cls_tglo.p_cls_input->is_key_tapped(m_b_page_up_dik))
	  {
		if (m_p_pic_up)
		{
		   m_p_pic_up->set_clicked(true);
		}	else
		{
			log_error("Got a custom page move command, but no up/down icons exist to set.");
		}
	  }
	}

	//respond to button clicks and such here
	if (m_p_pic_down)
	{
		if (m_p_pic_down->got_clicked())
		{
		   if (CanScrollDown()) 
		   {
			   //yes, we can scroll down
			  ScrollDown();
		   }
		}
	}

	if (m_p_pic_up)
	{
		if (m_p_pic_up->got_clicked())
		{
		   if (m_i_cur_line > 1) 
		   {
			   //yes, we can scroll down
			   m_i_cur_line -= m_i_display_lines;
			   if (m_i_cur_line < 1) m_i_cur_line = 1; //if the text had changed we may have needed this
			   //update the text
			   this->UpdateText(false);
		   }
		}
	}

    
   
	 if (m_p_pic_ok && !m_p_pic_ok->get_no_draw())
	 {
		 if (
			 (m_p_pic_ok->got_clicked()) ||
			 (cls_tglo.p_cls_input->is_key_tapped(DIK_RETURN)) ||
             	 (cls_tglo.p_cls_input->is_key_tapped(DIK_SPACE)) ||
          	 (cls_tglo.p_cls_input->is_key_tapped(DIK_RCONTROL))
             ||  (cls_tglo.p_cls_input->GetAnyJoystickButton(0))
			 )
		 {
	       
             if (cls_tglo.p_cls_input->is_key_tapped(DIK_RETURN))
             {
                 cls_tglo.p_cls_input->set_key_not_tapped(DIK_RETURN);
             }
             if (cls_tglo.p_cls_input->is_key_tapped(DIK_RCONTROL))
             {
                 cls_tglo.p_cls_input->set_key_not_tapped(DIK_RCONTROL);
             }
             if (cls_tglo.p_cls_input->is_key_tapped(DIK_SPACE))
             {
                 cls_tglo.p_cls_input->set_key_not_tapped(DIK_SPACE);
             }
             
             
             cls_tglo.p_cls_input->SetAllJoystickButtonsOff();
             
             m_b_wait_to_click = true;
             
             //even though we won't do it yet, play the click sound now
             //play a sound?
             if (m_st_click_sound[0] != 0)
             {
                 cls_tglo.p_sound->Play(CDir(m_st_click_sound));
             }


             m_timer_wait_to_click.set_interval(200);

             m_p_pic_ok->set_clicked(true); //change the graphic

		 }
	 }
   


}


bool CTextBox::CanScrollDown()
{
    return (m_i_cur_line+m_i_display_lines < m_i_total_lines);
}  

void CTextBox::ScrollDown()
{
     m_i_cur_line += m_i_display_lines;
			   //update the text
			   this->UpdateText(false);
}
