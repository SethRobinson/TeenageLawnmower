/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CTextChoice.h"
#include "CPic.h"
#include "CThingFont.h"
#include "CResourceTex.h"
#include "..\\uni.h"	 
#include "Cthing_util.h"
#include "..\\CScriptEngine.h"
#include "..\\sound\\ISoundManager.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define C_TEXT_DEFAULT_CHOOSE_DELAY_MS 200
#define C_TEXT_DEFAULT_FLASH_DELAY_MS 60


//***************************** The small CHOICE class *****************************


int CChoice::CalculateLinesUsed(int i_font, int i_width)
  {
  	i_lines_used = cls_tglo.p_font->ComputeHowManyLines(i_font, ws_choice,  i_width);
//	log_msg("used %d lines for %s.", i_lines_used, uni(ws_choice).to_st());
	return i_lines_used;
  }


//*************************** THE MAIN Choice class, derived from CTextBox *********

CTextChoice::CTextChoice()
{
//   m_dw_sel_color = D3DCOLOR_ARGB(255, 188,169,5);
//	m_dw_alt_color = D3DCOLOR_ARGB(255,200,190,190);
//	m_dw_color = D3DCOLOR_ARGB(255,255,255,255);

	AutoScrollNeighbor(NULL);
	m_dw_sel_color = D3DCOLOR_ARGB(255,255,255,255);
	m_dw_alt_color = D3DCOLOR_ARGB(255,200,190,190);
	m_dw_color = D3DCOLOR_ARGB(255,200,190,190);
    m_b_flash_highlighted = false;
    m_i_timer_flash_ms = C_TEXT_DEFAULT_FLASH_DELAY_MS;
       
	m_i_final_sel = -1;
	m_i_final_return = -1;
	m_i_timer = C_TEXT_DEFAULT_CHOOSE_DELAY_MS;
	m_st_move_sound[0] = 0;
   	strcpy(m_st_move_sound, "sound\\click3.wav");
	set_type(C_THING_TEXTBOX_CHOICE);
    //m_b_skip_draw_of_next_frame = true; this is done in it's parent class
   ClearChoices();

}

CTextChoice::~CTextChoice()
{
	ClearChoices();
}




void CTextChoice::SetMoveSound(char *st_name)
{
	strcpy(m_st_move_sound, st_name);

}

void CTextChoice::ClearChoices()
{
	for (unsigned int i=0; i < m_vec_choice.size(); i++ )
	{
		SAFE_DELETE(m_vec_choice[i]);
	}

	m_vec_choice.clear();
	m_i_cur_sel = 0;	  
	m_i_cur_line = 0; //the line we start with
	m_b_got_initial_click = false;
}

void CTextChoice::AddChoice(int i_return, WCHAR *p_wchar)
{
	m_vec_choice.push_back(NULL);
	int i_index = m_vec_choice.size()-1;
	m_vec_choice[i_index] = new CChoice;

	//fill it's values
	m_vec_choice[i_index]->i_return = i_return;
	wcscpy(m_vec_choice[i_index]->ws_choice, p_wchar);
	b_update_text = true; //force a visual update
	m_vec_choice[i_index]->CalculateLinesUsed(m_i_font, int(m_pt_text_limit.x));

}  

void CTextChoice::SetScriptCallbackOnChoice(int i_script)
{

	m_i_script = i_script;

}

void CTextChoice::UpdateText(bool b_scroll_to_end)
{
  
	b_update_text = false;
	//let's write some text on it for now
    CResourceTex * p_tex = m_p_pic_text->GetSpriteTex();
	
    if (!p_tex)
    {
#ifdef _DEBUG
 log_error("UpdateTex SpriteTex of %s is null.", get_thing_name());
#endif
        return;
    }
    m_i_display_lines = int(m_pt_text_limit.y / float(cls_tglo.p_font->GetRealFontHeight(m_i_font)));
	cls_tglo.p_font->SetPenOptions(m_i_font, m_dw_options);
	RECT rect_box;
	int i_lines_drawn_so_far = 0;
	int i_font_height = cls_tglo.p_font->GetRealFontHeight(m_i_font);

	//let's clear our text palette first
	p_tex->FillWithColor(D3DCOLOR_ARGB(0,0,0,0));

	//while we're here, let's go ahead and calculate our total lines
	m_i_total_lines = 0;

   	for (unsigned int j=0; j < m_vec_choice.size(); j++)
	{
	  m_i_total_lines += m_vec_choice[j]->i_lines_used;

	}
	
	bool b_alt_color = false;
	
	for (unsigned int i=m_i_cur_line; i < m_vec_choice.size(); i++)
	{
		//do have room to write the whole option?	
		if ( (i_lines_drawn_so_far +  m_vec_choice[i]->i_lines_used) > m_i_display_lines)
		{
			//can't fit it all, abort.
			break;
		}
		
		//	log_msg("Drawing %d lines, can only display %d though.", m_i_total_lines, m_i_display_lines);
		SetRect(&rect_box, 0,i_lines_drawn_so_far * i_font_height ,int(m_pt_text_limit.x), int(m_pt_text_limit.y));
		//if all 0's were sent it would just set it to the texture width/height.
		
		//set the font color
		
		if (i == m_i_final_sel)
		{
			//a selection has been made and this is it.  Let's color it our special color.

            //so it flashes..

            if (m_b_flash_highlighted)
            cls_tglo.p_font->SetPenColor(m_i_font, m_dw_sel_color); else
                cls_tglo.p_font->SetPenColor(m_i_font, m_dw_color);
            
		}	else
		{
			
			
			if (!b_alt_color)
			{
				
				cls_tglo.p_font->SetPenColor(m_i_font, m_dw_color);
			} else
			{
				cls_tglo.p_font->SetPenColor(m_i_font, m_dw_alt_color);
			}
	
		  if (i == m_i_cur_sel && m_i_final_sel == -1)
		  {
			  //this only happens with arrow key control	
			  cls_tglo.p_font->SetPenColor(m_i_font, m_dw_sel_color);

		  }
		
		}
		 b_alt_color = !b_alt_color;
		p_tex->RenderText(m_i_font, &rect_box, (WCHAR*)&m_vec_choice[i]->ws_choice);
		i_lines_drawn_so_far += m_vec_choice[i]->i_lines_used;
	}
	
	

	
	UpdateTextPosition();
   
}


void CTextChoice::UpdateScrollDownPosition()
{
    if (!m_p_pic_down) return;

    m_p_pic_down->set_xy(get_pos_x()+m_pt_down_offset.x, get_pos_y() + m_pt_down_offset.y);
	if (m_i_cur_line + ConvertLinesToChoices(m_i_cur_line, m_i_display_lines) < int(m_vec_choice.size()))
	{
		//this button should be active.
		m_p_pic_down->SetBlink(true);
        m_p_pic_down->VertexColor(D3DCOLOR_ARGB(255,255,255,255));
	}	else
	{
		m_p_pic_down->SetBlink(false);
        m_p_pic_down->VertexColor(m_dw_inactive_color);
	}

	m_p_pic_down->set_sprite_layer(m_i_layer+2);

}

void CTextChoice::UpdateScrollUpPosition()
{
    if (!m_p_pic_up) return;

	m_p_pic_up->set_xy(get_pos_x()+m_pt_up_offset.x, get_pos_y() + m_pt_up_offset.y);

	if (m_i_cur_line > 0)
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


void CTextChoice::UpdateTextPosition()
{
	if (m_p_pic_text)
	{
	
	m_p_pic_text->set_xy(get_pos_x()+m_pt_text_offset.x, get_pos_y() + m_pt_text_offset.y);
	m_p_pic_text->set_sprite_layer(m_i_layer+1);
	m_p_pic_text->set_clickable(true); //so they can click on the answer
   	}
	
	if (m_p_pic_bg)
	{
		
        m_p_pic_bg->set_xy(get_pos_x(), get_pos_y());
		m_p_pic_bg->set_sprite_layer(m_i_layer);
	}
		
	UpdateScrollDownPosition();
    UpdateScrollUpPosition();

}


void CTextChoice::UpdatePositions()
{
	UpdateTextPosition();
}


int CTextChoice::ConvertLinesToChoices(int i_start, int i_lines_to_count)
{
    int i_lines = 0;
	int j;
	for (j=i_start; j < int(m_vec_choice.size()); j++)
	{
	  i_lines += m_vec_choice[j]->i_lines_used;

	  if (i_lines > i_lines_to_count)
	  {
		  //uh oh, this choice pushes us over the limit.  Go back.
		  return j;
	  }
	}

return j;
}

  int CTextChoice::ConvertLinesToChoicesBackwards(int i_start, int i_lines_to_count)
{
    int i_lines = 0;
	int j;
	int i_choices = 0;

	if (m_vec_choice.size() == 0) return 0; //no choices yet
	if (i_start == 0) return 0; //um, it's unlikely we can count lines backwards from 0

	for (j=i_start; j >= 0; j--)
	{
	  i_lines += m_vec_choice[j]->i_lines_used;
	  i_choices++;
	  if (i_lines > i_lines_to_count)
	  {
		  //uh oh, this choice pushes us over the limit.  Go back.
		  return i_choices-1;
	  }
	}

return i_choices;
}


int CTextChoice::GetLinesFromChoices(int i_choices)
{

     int i_lines = 0;
	int j;
	for (j=0; (j < int(m_vec_choice.size())) && (j < i_choices); j++)
	{
	  i_lines += m_vec_choice[j]->i_lines_used;
	}
	return i_lines;
}

int CTextChoice::GetChoiceFromLine(int i_line)
{
	int i_line_total = 0;
  
	int i = 0;
	for (i=0; i < int(m_vec_choice.size()); i++)
	{
		i_line_total += m_vec_choice[i]->i_lines_used;

	    if (i_line_total >= i_line)
		{
		  //we got it.
			break;
		}
	}


   return i; //this is the choice!
}

int CTextChoice::GetLastLineListedNow()
{
	
	int i_lines = 0;
	for (int j=0; (j < int(m_vec_choice.size())) && (j < m_i_cur_line); j++)
	{
		i_lines += m_vec_choice[j]->i_lines_used;
	}
	
	//ok, that's all the lines BEFORE the screen we're seeing, now let's add the lines currently on the
	//screen.
	
	int i_screen_lines = 0;
	
   	for (unsigned int i=m_i_cur_line; i < m_vec_choice.size(); i++)
	{
		i_screen_lines += m_vec_choice[i]->i_lines_used;
		
		if (i_screen_lines > m_i_display_lines)
		{
			//whups, went too far
			i_screen_lines -=  m_vec_choice[i]->i_lines_used;
			break;
		}
	}
	
	return i_lines+i_screen_lines;
}

int CTextChoice::ComputeClick(D3DXVECTOR2 *p_pt)
{
  
	if (p_pt->x > m_pt_text_limit.x)
	{
		//they clicked too far to the right, ignore this.
		return -1; //
	}

	//did we click too far low on the current group of selections?
	int i_click_line = int(p_pt->y) + (GetLinesFromChoices(m_i_cur_line) * cls_tglo.p_font->GetRealFontHeight(m_i_font));
	
	i_click_line /=  cls_tglo.p_font->GetRealFontHeight(m_i_font);
	
	i_click_line++;

	if (i_click_line > GetLastLineListedNow())
	{
		//they clicked below the last option, ignore it
		return -1;
	}


	//get logical click point
//	log_msg("You actually clicked line %d.", i_click_line);
//	log_msg("Got clicked at %s.", print_point(m_p_pic_text->GetLastHitPoint()));

	//ok, we now know exactly what "line" they clicked, but we need to convert this into what "choice" they clicked.

	int i_choice = GetChoiceFromLine(i_click_line);
//	log_msg("Hurray!  Return id is %d.",m_vec_choice[i_choice]->i_return );
	SetChoiceClick(i_choice);

  	return m_i_final_return;
}
  
void CTextChoice::SetChoiceClick(int i_choice)
{
	//play a sound?
	if (m_st_click_sound[0] != 0)
	{
		cls_tglo.p_sound->Play(CDir(m_st_click_sound));
	}
	m_i_final_sel = i_choice;
	m_i_final_return = m_vec_choice[i_choice]->i_return; 
    m_b_got_initial_click = true;
	//set everything to draw all cool
	m_timer_choice.set_interval(m_i_timer);
	m_timer_flash.set_interval(m_i_timer_flash_ms);
    m_b_flash_highlighted = true;
	//clear stuff from the upper dialog window
	UpdateText(false);
	//yes, a selection was made.  disable this.
	if (m_p_pic_text)
	m_p_pic_text->set_clickable(false);
		
}


bool CTextChoice::HasBeenClicked()
{
	  if (m_b_got_initial_click)
	  {
		  m_b_got_initial_click = false;
		  return true;
	  }
 return false;
}

//note, this is 1 based, not 0.
void CTextChoice::SetCurrentSelection(int i_choice)
{
    //cycle through them all and find the choice that matches this return value

    for (unsigned int i=0; i < m_vec_choice.size(); i++)
    {
        if (m_vec_choice[i]->i_return == i_choice)
        {
          m_i_cur_sel = i_choice-1;
          b_update_text = true;
          return;
        }
    }

}

void CTextChoice::ProcessManualDownClick()
{
  	int i_orig = m_i_cur_sel;
	
	m_i_cur_sel += 1;

   //note, m_i_cur_sel is 1 based, not 0
   if (m_i_cur_sel >= int(m_vec_choice.size()))
	{
		m_i_cur_sel = 	m_vec_choice.size()-1;
	}

   if (i_orig != m_i_cur_sel)
   {
	   //a change was actually made
	   if (m_st_click_sound[0] != 0)
	   {
		   cls_tglo.p_sound->Play(CDir(m_st_move_sound));
	   }
	   
	   b_update_text = true;
   }
  
   
   if (m_i_cur_sel >= ConvertLinesToChoices(m_i_cur_line, m_i_display_lines))
  {
      //our select is off the screen, let's scroll down.
       m_i_cur_line += ConvertLinesToChoices(m_i_cur_line, m_i_display_lines);
				//update the text
				this->UpdateText(false);

  }

}

void CTextChoice::ProcessManualUpClick()
{
  	int i_orig = m_i_cur_sel;
	m_i_cur_sel -= 1;
  
   if (m_i_cur_sel < 0) m_i_cur_sel = 0;

   if (i_orig != m_i_cur_sel)
   {
	   //a change was actually made
	   if (m_st_click_sound[0] != 0)
	   {
		   cls_tglo.p_sound->Play(CDir(m_st_move_sound));
	   }
	   
	   b_update_text = true;
   }

   if (m_i_cur_sel < m_i_cur_line)
   {
   
   m_i_cur_line -= ConvertLinesToChoicesBackwards(m_i_cur_line-1, m_i_display_lines);
			   if (m_i_cur_line < 0) m_i_cur_line = 0; //if the text had changed we may have needed this
			   //update the text
			   this->UpdateText(false);
   }
}

void CTextChoice::ProcessManualChoiceClick()
{
	//they want to choose.  Can they?
	if ( (m_i_cur_sel > -1) 
		&& (m_i_cur_sel < int(m_vec_choice.size()))
		)
	{
	
        if (m_p_textbox_neighbor)  
          {
              if (m_p_textbox_neighbor->CanScrollDown())
              {
              
                m_p_textbox_neighbor->ScrollDown();
                return;
              }
          }

        
        //ok, sure
		this->SetChoiceClick(m_i_cur_sel);
	}

}

void CTextChoice::think()
{
 if (GetPauseLevel() < cls_tglo.GetPauseLevel()) return; //game paused for us


if (b_delete_flag) return; //we're gonna die, don't do anything	



	if (b_update_text)
	{
		UpdateText(false);
	}

    if (m_b_skip_draw_of_next_frame)
 {
     //don't process input until the next frame
     m_b_skip_draw_of_next_frame = false;
     return;
 }


	if (m_i_final_return != -1)
	{
		//they clicked and we're.. waiting?
		if (m_timer_choice.interval_reached())
		{
		  
		   int i_return = m_i_final_return;
		   m_i_final_return = -1;
		   m_i_final_sel = -1;
		   m_i_cur_sel = -1;
		   m_p_pic_text->set_clickable(true);
		  this->UpdateText(false);

		  //ok, callback wherever we were supposed too...
		  	 if (m_i_script != C_SCRIPT_NONE)
			 {

				 //we need to set a local var in the script too.
				int i_temp_var = cls_tglo.p_script_engine->GetScript(0)->FindVarByName("G_I_LAST_CHOICE");
				cls_tglo.p_script_engine->GetScript(0)->GetVar(i_temp_var)->SetInt(i_return);
		        cls_tglo.p_script_engine->GetScript(m_i_script)->StartUpAfterWait();
				 //let's call the script back
			 }

		} else
		{
			//still waiting.  Make it blink
            
            if (m_timer_flash.interval_reached())
            {
                m_b_flash_highlighted = !m_b_flash_highlighted;
                
                this->UpdateText(false);
            }
			return;
		}

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
			if (m_i_cur_line + ConvertLinesToChoices(m_i_cur_line, m_i_display_lines) < int(m_vec_choice.size()))
			{
				//yes, we can scroll down
				m_i_cur_line += ConvertLinesToChoices(m_i_cur_line, m_i_display_lines);
				//update the text
				this->UpdateText(false);
			}
		}
	}

	if (m_p_pic_up)
	{
		if (m_p_pic_up->got_clicked())
		{
		   if (m_i_cur_line > 0) 
		   {
			   //yes, we can scroll down
			   m_i_cur_line -= ConvertLinesToChoicesBackwards(m_i_cur_line-1, m_i_display_lines);
			   if (m_i_cur_line < 0) m_i_cur_line = 0; //if the text had changed we may have needed this
			   //update the text
			   this->UpdateText(false);
		   }
		}
	}

   //ah yes, but did our text get clicked...
	if (m_p_pic_text)
	{
		if (m_p_pic_text->got_clicked())
		{
		  
          
          m_p_pic_text->set_clicked(false); //we're done with you now
		  ComputeClick(m_p_pic_text->GetLastHitPoint());
		
		}

	}
    if (m_i_final_return == -1)
    {
        
      if (
          (cls_tglo.p_cls_input->is_key_tapped(DIK_DOWN))
          ||
          (cls_tglo.p_cls_input->GetAnyJoystickDirTapped(C_INPUT_JOY_DOWN))
          ) ProcessManualDownClick();



     if (
         (cls_tglo.p_cls_input->is_key_tapped(DIK_UP))
         ||
          (cls_tglo.p_cls_input->GetAnyJoystickDirTapped(C_INPUT_JOY_UP))

         ) ProcessManualUpClick();
         
        if (cls_tglo.p_cls_input->is_key_tapped(DIK_RETURN))
        {
            cls_tglo.p_cls_input->set_key_not_tapped(DIK_RETURN);
            ProcessManualChoiceClick();
        } else
        if (cls_tglo.p_cls_input->is_key_tapped(DIK_RCONTROL))
        {
         
            cls_tglo.p_cls_input->set_key_not_tapped(DIK_RCONTROL);
            ProcessManualChoiceClick();
        }  else
        if (cls_tglo.p_cls_input->is_key_tapped(DIK_SPACE))
        {
            cls_tglo.p_cls_input->set_key_not_tapped(DIK_SPACE);
            ProcessManualChoiceClick();
        } else
        
        if (cls_tglo.p_cls_input->GetAnyJoystickButtonTapped(0))
        {
            cls_tglo.p_cls_input->SetAllJoystickButtonsOff();
            ProcessManualChoiceClick();
        }
    }
}


void CTextChoice::AutoScrollNeighbor(CTextBox *p_new)
{
    m_p_textbox_neighbor = p_new;
}
