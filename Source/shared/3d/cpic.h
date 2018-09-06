/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */

#pragma once
#include "CVisualMulti.h"
#include "..\\CTimer.h"

enum
{
        C_PIC_TEXT_TIMER_MODE_NONE = 0,
        C_PIC_TEXT_TIMER_MODE_INT_CENTER, 
        C_PIC_TEXT_TIMER_MODE_CLOCK
};


const int C_PIC_MAX_INPUT_SIZE = 256;

const int C_METER_NONE = 0;  
const int C_METER_LEFT_BASED = 1;  


const int C_METER_TRIGGER_SAME_OR_HIGHER = 0;
const int C_METER_TRIGGER_SAME_OR_LOWER = 1;

enum
{
        C_CPIC_VISUAL_EFFECT_NONE,
        C_CPIC_VISUAL_EFFECT_BOB,
        
        
        //add above this line only
        C_CPIC_VISUAL_EFFECT_COUNT
    
};


//all purpose class to handle gui and 2d stuff

class CPic : public CVisualMulti
{
public:

 	virtual void think();
    CPic();
    virtual ~CPic();
   	virtual void dx_draw();
    virtual void dx_restore();
    void set_sprite_layer(int i_layer);
    virtual void receive_message(CMessageHeader *p_header, void *p_message);
    void hit_test_sprite(CMessageRayPick *p_ray);
    void InitAsButton(const char *p_st_normal, const char *p_st_clicked);
    D3DXVECTOR2 * GetLastHitPoint(){return &m_pt_last_hit;};
    void SetClickSound(char *p_st_sound);
    void SetHighlightSound(char *p_st_sound);
    int SetClickScript(char *p_st_script);
    void SetupFastTextRender(int i_fast_font_index,  const char *p_st_text);
    void EntSetFastTimer(int i_mode, float f_start, float f_time_mod, int i_timer);
    void TextFormat();
    void SetCallbackOnTimer(int i_script, float f_time);
    void EntSetTime(float f_time);
    float EntGetTime();
    void SetTextColor(DWORD dw_color){m_dw_font_color = dw_color;};
    void SetTrigger(int i_trigger_type, float f_trigger, int id_ent, const char *p_st_function);
    void RenderText(int i_font, WCHAR *p_ws_text, int i_x, int i_y, bool b_clear_first);
    bool SetButtonHighlight(const char *p_st_fname);
    bool InsideRect2D(int i_x, int i_y);
    void GetRect(RECT * p_rect_out);
    void SetCPicVisualEffect(int i_new) {m_i_cpic_visual_effect = i_new;}
    int GetCPicVisualEffect(){return m_i_cpic_visual_effect;}
        
protected:
    //mostly stuff to handle changing the image when clicked and playing a sound
    int SetButtonGraphic(int i_id);
    
    char m_st_sound[256];
    char m_st_highlight_sound[256];
    CTimer m_timer_click; //how long before we reset the button and allow another click
    CTimer m_timer_click_flash_speed; //how fast the button flashes after being clicked
    bool m_b_button_mode;
    int m_i_click_flash_speed_ms;
    bool m_b_blink_highlighted;
    int m_i_click_delay; 
    bool m_b_button_down;
    int m_i_pic_clicked_id;
    int m_i_pic_normal_id;
    int m_i_pic_highlight_id;
    bool m_b_button_mouse_over; //true if the mouse is hovering over our hit rect, applicable to buttons
    
    D3DXVECTOR2 m_pt_last_hit;  //save the last place we got clicked on.
    int m_i_script; //some buttons want a script to get run
    bool m_b_run_script; //only run the script once
    
    
    //fast font stuff
    int m_i_fast_font_index; //should be -1 normally, which disables "text draw mode"
    DWORD m_dw_font_color;
    char m_st_text[C_PIC_MAX_INPUT_SIZE]; //used in text mode
    
    int m_i_timer_mode;  //if not used, 1 for integer display with centering, 2 for float display
    CTimer m_timer_text; //how often to update the text if a timer is being used
    float m_f_time_mod; //how much to change the time by each mod period
    int m_i_text_wait; //how long to wait for each mod period, milliseconds
    float m_f_cur_time; //current time
    int m_i_callback_script; 
    float m_f_callback_time;
    
    //if we're going to use the script trigger option
    float m_f_script_trigger;
    int m_script_trigger_ent_id;
    char m_st_script_trigger_function[32];
    int m_i_trigger_type;
    int m_i_cpic_visual_effect;
    
};