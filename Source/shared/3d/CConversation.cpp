/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CConversation.h"
#include "..\\CInput.h"
#include "CThing_globals.h"
#include "CThingList.h"
#include "CTextBox.h"
#include "CPic.h"		   
#include "CTextChoice.h"
#include "CManager.h"
#include "ent_utils.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CConversation::CConversation()
{
   this->set_thing_name("Conversation");
	set_sort(-3);

	m_p_pic_panel = NULL;
	m_p_tb_dialog = NULL;
	m_p_tb_choice = NULL;
    
	m_pt_panel.x = 589;
	m_pt_panel.y = 22;

	//global position, use this to move everything at once
	m_pt_pos.x = 0;
	m_pt_pos.y = 0;
	m_i_layer = 20;

	//no change will be made

	/*
      //these were the default when tarzan was made.. 
      m_pt_dialog.x = 144;
	m_pt_dialog.y = 65;

	m_pt_choice.x = 202;
	m_pt_choice.y = 397;
	  */
    
    m_pt_dialog.x = 184;
	m_pt_dialog.y = 43;

	m_pt_choice.x = 207;
	m_pt_choice.y = 310;
    
    set_type(C_THING_TEXTBOX); //actually it's not a textbox, it controls one.  but hey,
	//we do this so it gets erased before other stuff upon a quit exit
    m_i_res_adjust = C_RES_ADJUST_NONE;
    m_f_res_adjust_save_x = 0;
    m_f_res_adjust_save_y = 0;	   

    SetResAdjust(C_RES_ADJUST_NORMAL, 800, 600); //default for TLM, don't change
	UpdatePositions();
}


CConversation::~CConversation()
{
   
	
   if (m_p_pic_panel) 
   {
       KillByAddress(p_my_thing_list, m_p_pic_panel);
   }
   if (m_p_tb_dialog)
   {
       KillByAddress(p_my_thing_list, m_p_tb_dialog);
   }
   if (m_p_tb_choice) 
   {
       KillByAddress(p_my_thing_list, m_p_tb_choice);
   }





}


void CConversation::SetResAdjust(int i_type, int i_base_x, int i_base_y)
{
    
    assert(i_type < C_RES_ADJUST_COUNT && "Unknown type of res adjust");
    if (m_i_res_adjust == C_RES_ADJUST_NONE)
    {
        //first time, imprint current x/y as original
        //save originals
        m_f_res_adjust_save_x = m_pt_pos.x;
        m_f_res_adjust_save_y = m_pt_pos.y;
        
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



void CConversation::set_xy(float f_x, float f_y)
{
    m_pt_pos.x = f_x;
    m_pt_pos.y = f_y;
    
    //add any special offsets, these are used for interface items so they won't look too
    //funky at different resolutions
    
    if (m_i_res_adjust == C_RES_ADJUST_CENTER_BOTTOM) //center and bottom of screen
    {
        
        //save originals
        m_f_res_adjust_save_x = m_pt_pos.x;
        m_f_res_adjust_save_y = m_pt_pos.y;
        
        if (cls_tglo.get_screen_width() != m_i_res_correct_base_x)
        {
            //adjust x
            
            m_pt_pos.x += (cls_tglo.get_screen_width() - m_i_res_correct_base_x) / 2;
            
            m_pt_pos.y += 	(cls_tglo.get_screen_height() - m_i_res_correct_base_y);
            
        }
    }  else
        
        if (m_i_res_adjust == C_RES_ADJUST_NORMAL) //center 
        {
            
            //save originals
            m_f_res_adjust_save_x = m_pt_pos.x;
            m_f_res_adjust_save_y = m_pt_pos.y;
            
            if (cls_tglo.get_screen_width() != m_i_res_correct_base_x)
            {
                //adjust x
                
                m_pt_pos.x += (cls_tglo.get_screen_width() - m_i_res_correct_base_x) / 2;
                
                m_pt_pos.y += 	(cls_tglo.get_screen_height() - m_i_res_correct_base_y) /2;
                
            }
        } else
            if (m_i_res_adjust == C_RES_ADJUST_FULL) //fit screen
            {
                
                //save originals
                m_f_res_adjust_save_x = m_pt_pos.x;
                m_f_res_adjust_save_y = m_pt_pos.y;
                
                if (cls_tglo.get_screen_width() != m_i_res_correct_base_x)
                {
                    //adjust x
                    
                    
                    m_pt_pos.x = (f_x / float(m_i_res_correct_base_x)) * cls_tglo.get_screen_width();
                    
                    m_pt_pos.y = (f_y / float(m_i_res_correct_base_y)) * cls_tglo.get_screen_height();
                    
                }
            } 
            
            
            
}

void CConversation::dx_restore()
{
   	if (m_i_res_adjust != C_RES_ADJUST_NONE)
    {
        SetResAdjust(m_i_res_adjust, m_i_res_correct_base_x, m_i_res_correct_base_y);
        set_xy(m_f_res_adjust_save_x, m_f_res_adjust_save_y);
    }
    

    UpdatePositions();
}

int CConversation::GetDialogId()
{
	if (!m_p_tb_dialog) InitDialogText();
   return (m_p_tb_dialog->get_id())	;
}

int CConversation::GetChoiceId()
{
	if (!m_p_tb_choice) InitChoiceText();
   return (m_p_tb_choice->get_id())	;
}


void CConversation::InitDialogText()
{
  SAFE_DELETE(m_p_tb_dialog);
  m_p_tb_dialog = new CTextBox();
  p_my_thing_list->add_new_thing(m_p_tb_dialog);
 // m_p_tb_dialog->SetFontColor(D3DCOLOR_ARGB(255,255,255,0));
  m_p_tb_dialog->SetFontId(1);
  //setup how we want the font to act
  D3DXVECTOR2 pt_size, pt_offset, pt_limit;
  //size of the text surface, follow ^2 limitation.
  pt_size.x = 512; 
  pt_size.y = 256;
  
  //offset of our text as comparing to the room background
  pt_offset.x = 15;
  pt_offset.y = 15;
  
  //if we don't want to use the whole text surface, set this to limit it
  pt_limit.x = 395; //kind of small to leave room for the picture bubble
  pt_limit.y = 168;

  m_p_tb_dialog->SetLayer(m_i_layer); //make sure it's above our other sprites
  m_p_tb_dialog->InitTextBox(pt_size, pt_offset, pt_limit);
  m_p_tb_dialog->InitBG("interface\\textbox.dds");
  //let's also add up and down scroll buttons


  D3DXVECTOR2 pt_pos2 = D3DXVECTOR2(422, 114);  
  m_p_tb_dialog->AddUpScroll(pt_pos2, "interface\\arrow_up.dds", "interface\\arrow_up_2.dds");

  D3DXVECTOR2 pt_pos = D3DXVECTOR2(422, 161);  
  m_p_tb_dialog->AddDownScroll(pt_pos, "interface\\arrow_down.dds", "interface\\arrow_down_2.dds");
  
  UpdatePositions();

}

void CConversation::ClearDialogText()
{
	if (!m_p_tb_dialog) InitDialogText();
    m_p_tb_dialog->ClearText();
    UpdatePositions();

}

void CConversation::AddDialogText(WCHAR *p_ws)
{
  if (!m_p_tb_dialog) InitDialogText();
  m_p_tb_dialog->AddText(p_ws);
  UpdatePositions();
}

void CConversation::ClearChoiceText()
{
  	if (!m_p_tb_choice) InitChoiceText();
    m_p_tb_choice->ClearText();
    UpdatePositions();

}

void CConversation::InitChoiceText()
{
  SAFE_DELETE(m_p_tb_choice);
  m_p_tb_choice = new CTextChoice();
  p_my_thing_list->add_new_thing(m_p_tb_choice);
 // m_p_tb_choice->SetFontColor(D3DCOLOR_ARGB(255,255,255,0));
  m_p_tb_choice->SetFontId(2);
  //setup how we want the font to act

  D3DXVECTOR2 pt_size, pt_offset, pt_limit;
  
  //size of the text surface, follow ^2 limitation.
  pt_size.x = 512; 
  pt_size.y = 256;
  
  //offset of our text as comparing to the room background
  pt_offset.x = 12;
  pt_offset.y = 12;
  
  //if we don't want to use the whole text surface, set this to limit it
  pt_limit.x = 355;
  pt_limit.y = 140;

  m_p_tb_choice->SetLayer(m_i_layer); //make sure it's above our other sprites
  m_p_tb_choice->InitTextBox(pt_size, pt_offset, pt_limit);
  //m_p_tb_choice->InitBGBlank(pt_size, D3DCOLOR_ARGB(150, 0,0,0));

  //add a real dialog box
   m_p_tb_choice->InitBG("interface\\small_textbox.dds");

//   m_p_tb_choice->SetClickSound("sound\\click.wav");

  //let's also add up and down scroll buttons

  D3DXVECTOR2 pt_pos2 = D3DXVECTOR2(372, 13);  
  m_p_tb_choice->AddUpScroll(pt_pos2, "interface\\arrow_up.dds", "interface\\arrow_up_2.dds");

  D3DXVECTOR2 pt_pos = D3DXVECTOR2(372, 129);  
  m_p_tb_choice->AddDownScroll(pt_pos, "interface\\arrow_down.dds", "interface\\arrow_down_2.dds");
  m_p_tb_choice->AutoScrollNeighbor(m_p_tb_dialog);
 
  UpdatePositions();

}


void CConversation::AddChoiceText(int i_choice, WCHAR *p_ws)
{
	if (!m_p_tb_choice) InitChoiceText();

	m_p_tb_choice->AddChoice(i_choice, p_ws);
	UpdatePositions();

}

void CConversation::UpdatePositions()
{

  	if (m_p_pic_panel)				
	{
		m_p_pic_panel->set_xy(m_pt_panel.x + m_pt_pos.x, m_pt_panel.y + m_pt_pos.y);
	    m_p_pic_panel->set_sprite_layer(m_i_layer+1);
	}

	if (m_p_tb_dialog)
	{
		 m_p_tb_dialog->set_pos_x(m_pt_dialog.x +  m_pt_pos.x);
		 m_p_tb_dialog->set_pos_y(m_pt_dialog.y +  m_pt_pos.y);
		 m_p_tb_dialog->UpdatePositions();
	}
   	if (m_p_tb_choice)
	{
		 m_p_tb_choice->set_pos_x(m_pt_choice.x +  m_pt_pos.x);
		 m_p_tb_choice->set_pos_y(m_pt_choice.y +  m_pt_pos.y);
		 m_p_tb_choice->UpdatePositions();
	}

}

int CConversation::SetPanelPic(char *p_name)
{

     if (p_name[0] == 0)
     {
         //they sent a blank string, let's erase any pic that was there
         m_p_pic_panel->b_delete_flag = true;
         m_p_pic_panel = NULL;
         return 0;


     }

	if (!m_p_pic_panel)
	{
		//create the panel
		m_p_pic_panel = new CPic();
	    m_p_pic_panel->set_thing_name("PanelPic");
		m_p_pic_panel->set_sprite_layer(m_i_layer);
		p_my_thing_list->add_new_thing(m_p_pic_panel);
    } else
    {
        //deref the old panel pic that was there, if there was one
        m_p_pic_panel->ReleaseResourceIfWeHaveOne();

    }

    char st_temp[32];
	sprintf(st_temp, "Panel%d", get_id());

	int i_temp_id = m_p_pic_panel->init_visual_sprite(st_temp, p_name);
	
	if (i_temp_id == 0)
	{
		log_error("Couldn't create PanelPic in CConversation.");
	}

	if (cls_tglo.IsRunning()) m_p_pic_panel->dx_create();
    UpdatePositions();

	return m_p_pic_panel->get_id();
}

void CConversation::dx_kill()
{

	
}


void CConversation::think()
{
  if (GetPauseLevel() < cls_tglo.GetPauseLevel()) return; //game paused for us
 if (b_delete_flag) return; //scheduled to die, don't think here

 if (m_p_tb_choice)
   {
	   if (m_p_tb_choice->HasBeenClicked())
	   {
		   //they clicked something.  Let's clear the dialog above
		   if (m_p_tb_dialog)
		   {
			   m_p_tb_dialog->ClearText();
		   }

		   //HasBeenClicked() will set itself to false automatically, we don't have
		   //to worry about that.


	   }


   }


}
