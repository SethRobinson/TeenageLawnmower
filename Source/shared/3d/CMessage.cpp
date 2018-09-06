#include "CMessage.h"
#include "CPic.h"
#include "CThingList.h"
#include "CThingFont.h"
#include "CResourceTex.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMessage::CMessage()
{
     m_p_pic = NULL;
     m_b_messaged_qued = false;
     m_wst_message_text[0] = 0;
     set_thing_name("CMESSAGE");
     m_dw_color = D3DCOLOR_ARGB(255, 255, 255, 255);
     m_dw_options = DT_CENTER;
     m_i_font = 4;
     m_i_state = C_MESSAGE_STATE_READY;
     m_i_minimum_message_time = 2000;

     m_f_move_speed = 1000;
     m_f_down_target_y = 170;
     m_f_up_target_y = -30;     
}


CMessage::~CMessage()
{
   KillPic();
}

void CMessage::KillPic()
{
    if (m_p_pic)
    {
        if (!p_my_thing_list->b_deleting_list)
        {
            m_p_pic->b_delete_flag = true;
        }
        m_p_pic = NULL;
    }
    
}


//don't send this more than 256 chars of text or else
void CMessage::SetMessageByText( WCHAR *p_wst_message)
{
   
    //if text already exists, let's tell it to go up
    if (m_i_state != C_MESSAGE_STATE_READY)
    {
        m_i_state = C_MESSAGE_STATE_GOING_UP;
    }
    
    //copy the text to our internal buffer
    if (wcslen(p_wst_message) > C_MESSAGE_BUFFER_SIZE-1)
    {
        log_error("Um, SetMessageByText can only handle a message up to %d chars.", C_MESSAGE_BUFFER_SIZE-1);
        return;
    }

    wcscpy(m_wst_message_text, p_wst_message);
    m_b_messaged_qued = true; //we'll show this message as soon as possible
}


void CMessage::CreatePicFromText()
{

    m_p_pic = new CPic();
	int i_id = p_my_thing_list->add_new_thing(m_p_pic);
	//first create a new unique resource name
	char st_text[32];
	sprintf(st_text, "MESSTEXT%d", m_p_pic->get_id());
	
    D3DXVECTOR2 pt_size = D3DXVECTOR2(1024, 32);
    m_p_pic->init_visual_sprite_blank(st_text, pt_size, D3DCOLOR_ARGB(0,0,0,0));
	m_p_pic->set_sprite_layer(60);
    m_p_pic->SetResAdjust(C_RES_ADJUST_NORMAL, 800, 600);
    
    m_p_pic->SetPauseLevel(GetPauseLevel());
   	//go ahead and load it if we are currently running
	if (cls_tglo.IsRunning())
	{
		m_p_pic->dx_create();
	}

    //let's also render to it now

   	//let's write our options
    CResourceTex * p_tex = m_p_pic->GetSpriteTex();
  
 
	cls_tglo.p_font->SetPenOptions(m_i_font, m_dw_options);
	//let's clear our text palette first
//	p_tex->FillWithColor(D3DCOLOR_ARGB(0,0,0,0));
    RECT rect_box;
    SetRect(&rect_box, 0, 0, int(pt_size.x), int (pt_size.y));

    //draw shadow
    
    cls_tglo.p_font->SetPenColor(m_i_font, D3DCOLOR_ARGB(255,0,0,0));

    rect_box.top += 1;
    
    p_tex->RenderText(m_i_font, &rect_box, m_wst_message_text);

    rect_box.left += 2;
    p_tex->RenderText(m_i_font, &rect_box, m_wst_message_text);

    rect_box.left -= 1; //back to the start
    rect_box.top -= 1; //to top
    
    p_tex->RenderText(m_i_font, &rect_box, m_wst_message_text);

    //one more, bottom
    rect_box.top += 2; 
    p_tex->RenderText(m_i_font, &rect_box, m_wst_message_text);

    //final
    rect_box.top -= 1;

    cls_tglo.p_font->SetPenColor(m_i_font, m_dw_color);
    p_tex->RenderText(m_i_font, &rect_box, m_wst_message_text);


    //start the thing moving down from the top

    m_i_state = C_MESSAGE_STATE_GOING_DOWN;
    m_vec2_pos = D3DXVECTOR2(-112.0f, m_f_up_target_y);

    m_p_pic->set_xy(m_vec2_pos.x, m_vec2_pos.y);

}


void CMessage::ProcessMovement()
{
    
     if (m_i_state == C_MESSAGE_STATE_READY) return; //nothing to do
   
      
     if (m_i_state == C_MESSAGE_STATE_GOING_DOWN)
     {
         //move your ass down until we hit teh spot
         
         m_vec2_pos.y += m_f_move_speed * cls_tglo.time();
         if (m_vec2_pos.y > m_f_down_target_y)
         {
             m_vec2_pos.y = m_f_down_target_y;
             m_i_state = C_MESSAGE_STATE_WAITING;
             m_timer_wait.set_interval(m_i_minimum_message_time);
         }
     } else
     if (m_i_state == C_MESSAGE_STATE_WAITING)
     {
       if (m_timer_wait.interval_reached())
       {
           m_i_state = C_MESSAGE_STATE_GOING_UP;
       }

     } else
         if (m_i_state == C_MESSAGE_STATE_GOING_UP)
         {
              m_vec2_pos.y -= m_f_move_speed * cls_tglo.time();
      
             if (m_vec2_pos.y < m_f_up_target_y)
             {
                 
                 //all done
                 m_vec2_pos.y = m_f_up_target_y;
                 m_i_state = C_MESSAGE_STATE_READY;
                 KillPic();
                 return;
             }


         }
    //translate
    m_p_pic->set_xy(m_vec2_pos.x, m_vec2_pos.y);


}

void CMessage::think()
{
    if (m_b_messaged_qued)
    {
        //if we're able...
        if (m_i_state == C_MESSAGE_STATE_READY)
        {
            
            m_b_messaged_qued = false;
            
            //release the pic we currently have
            KillPic();
            
            if (m_wst_message_text)
            {
                CreatePicFromText();
            }
        }
        
    }


    ProcessMovement();

}