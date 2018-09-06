//This is kind of a dumb class that just ques up messages to show and moves them
//in a reliable way.

#pragma once

#include "CThing.h"
#include "..\\CTimer.h"
#include "d3dx8math.h"

class CPic; //for speed
const int C_MESSAGE_BUFFER_SIZE = 100;

const int C_MESSAGE_STATE_READY = 0;
const int C_MESSAGE_STATE_GOING_DOWN = 1;
const int C_MESSAGE_STATE_GOING_UP = 2;
const int C_MESSAGE_STATE_WAITING = 3;


class CMessage: public CThing
{
 
public:

     CMessage();
     void CMessage::SetMessageByText( WCHAR *p_wst_message);
     void CMessage::CreatePicFromText();
     virtual ~CMessage();
     void CMessage::KillPic();
     virtual void CMessage::think();
     void ProcessMovement();


private:

    CPic * m_p_pic;
    WCHAR m_wst_message_text[C_MESSAGE_BUFFER_SIZE];
    bool m_b_messaged_qued;
    DWORD m_dw_color; //color of font
    DWORD m_dw_options; //options for font
    int m_i_font; //what font style
    int m_i_minimum_message_time;
    float m_f_move_speed;
    CTimer m_timer_wait;
    int m_i_state; //what we're doing right now
    float m_f_down_target_y;
    float m_f_up_target_y;
    D3DXVECTOR2 m_vec2_pos; //our local copy of the coords, pre modified for screen size, etc
};