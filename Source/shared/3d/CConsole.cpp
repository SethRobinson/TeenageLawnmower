/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */




#include "CConsole.h"
#include "..\\CInput.h"
#include "CThing_globals.h"
#include "CThingList.h"
#include "CTextBox.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CConsole::CConsole()
{
	m_i_console_id = -1;
    this->set_thing_name("Console");
	set_type(C_THING_CONSOLE);
	set_sort(1);
	cls_tglo.p_console = this;
	m_i_start_line = 0;
	m_i_new_line = 1;
	m_b_refresh = false;
	strcpy(m_a_text[0], "(Log Started)\r\n");

}


CConsole::~CConsole()
{
	cls_tglo.p_console = NULL;
}


void CConsole::dx_kill()
{
	//resetting everything.. our console won't survive this.
	CloseConsole();
}

void CConsole::InitConsole()
{

  CTextBox *p_text = new CTextBox();

  p_my_thing_list->add_new_thing(p_text);
  p_text->SetPauseLevel(300); //can't be erased with a kill all pics command
  m_i_console_id = p_text->get_id();
  p_text->set_pos_x(20);
  p_text->set_pos_y(50);
  p_text->SetFontColor(D3DCOLOR_ARGB(255,255,255,255));
  //setup how we want the font to act


  D3DXVECTOR2 pt_size, pt_offset, pt_limit;
 
  int i_size_x = 512;
  int i_size_y = 256;

  //size of the text surface, follow ^2 limitation.
  pt_size.x = float(i_size_x);
  pt_size.y = float(i_size_y);
  
  //offset of our text as comparing to the main background
  pt_offset.x = 15;
  pt_offset.y = 7;
  
  //if we don't want to use the whole text surface, set this to limit it
  pt_limit.x = 295;
  pt_limit.y = 188;

  p_text->SetLayer(80); //make sure it's above our other sprites
  p_text->InitTextBox(pt_size, pt_offset, pt_limit);
 // p_text->InitBGBlank(pt_size, D3DCOLOR_ARGB(140,0,0,0));
   p_text->InitBG("interface\\textbox.dds");
   p_text->SetFontId(0); //use a font nobody will screw with

  //let's also add up and down scroll buttons
  D3DXVECTOR2 pt_pos2 = D3DXVECTOR2(421,  17);  
  p_text->AddUpScroll(pt_pos2, "interface\\arrow_up.dds", "interface\\arrow_up_2.dds");

    D3DXVECTOR2 pt_pos = D3DXVECTOR2(421, 166);  
  p_text->AddDownScroll(pt_pos, "interface\\arrow_down.dds", "interface\\arrow_down_2.dds");


  m_b_refresh = true;
  p_text->SetPageUpKey(DIK_PGUP);
  p_text->SetPageDownKey(DIK_PGDN);

}

void CConsole::AddText(char *p_text)
{
	
	//first add it to our internal copy
	strcpy(m_a_text[m_i_new_line], p_text);

	m_i_new_line++;

	if (m_i_new_line == C_CONSOLE_LINES)
	{
		m_i_new_line = 0; //wrap around
	}

	if (m_i_new_line == m_i_start_line) 
	{
		m_i_start_line++;
		if (m_i_start_line == C_CONSOLE_LINES) m_i_start_line = 0; //wrap around
	}
	
	m_b_refresh = true; //udate later during our think()
}

void CConsole::DrawConsole()
{

	if (!cls_tglo.IsRunning()) return; //can't draw now
	m_b_refresh = false;
		
	if (m_i_console_id != -1)
	{
		CTextBox *p_con = (CTextBox*) p_my_thing_list->get_thing_by_id(m_i_console_id);
		
		if (p_con)
		{
			p_con->ClearText();

			//loop through and give all the info
		
			for (int i = m_i_start_line; (i < C_CONSOLE_LINES); i++)
			{
				
				p_con->AddTextA(m_a_text[i]);

			  	if (m_i_new_line == i+1)
				{
					//we're done here.
					break;
				}
			
			}

			if ( m_i_start_line > m_i_new_line)
			{
				//go back and get the ones that wrapped around too
				for (int j = 0; j < m_i_new_line; j++ )
				{
					p_con->AddTextA(m_a_text[j]);
				}

			}
		
			p_con->UpdateText(true); //the true means we'll show the end, not the start
		}
		
	}


}

void CConsole::CloseConsole()
{
   if (m_i_console_id == -1) return; //already closed
	   
   CThing *p_con = p_my_thing_list->get_thing_by_id(m_i_console_id);

   if (p_con)
   {
	   p_con->b_delete_flag = true; //bye bye
   }
  
   m_i_console_id = -1;
}


void CConsole::think()
{
   //should we turn on the console?

	if (cls_tglo.p_cls_input->is_key_tapped(DIK_GRAVE))
	{
		if (
			(m_i_console_id == -1) 
			|| (!p_my_thing_list->get_thing_by_id(m_i_console_id))
			)
		{
		    InitConsole(); 
		//	log_msg("Console Opened.");
		} else
		{
			//close console
		    CloseConsole();
		//	log_msg("Console Closed.");
		}
	}

	if (m_i_console_id != -1)
	{
		
		if (m_b_refresh)
		{
			DrawConsole();
		}
	}
   
}
