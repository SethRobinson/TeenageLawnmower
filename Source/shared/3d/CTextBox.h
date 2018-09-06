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
#include "CThing_globals.h"
#include "CVisual.h"
#include "d3dx8math.h"
#include "..\\CTimer.h"

class CPic;

const int C_TEXTBOX_NO_FADE = 0;
const int C_TEXTBOX_UP = 1;
const int C_TEXTBOX_DOWN = 2;


class CTextBox : public CVisual
{
public:
	
	CTextBox();
	~CTextBox();
	int CTextBox::InitTextBox(D3DXVECTOR2 pt_size, D3DXVECTOR2 pt_offset, D3DXVECTOR2 pt_limit);
	
	void SetLayer(int i_layer);
	int InitBG(char st_name[256]);
	int InitBGBlank(D3DXVECTOR2 pt_size, DWORD dw_color);
	void SetText(WCHAR *p_wchar); //unicode version
	void SetTextA(char *p_text); 	//ascii version
	void AddText(WCHAR *p_wchar);
	void AddTextA(char *p_text);
	int AddDownScroll(D3DXVECTOR2 pt_pos, char st_up[256], char st_down[256]);
	int AddUpScroll(D3DXVECTOR2 pt_pos, char st_up[256], char st_down[256]);
	virtual void think();
	virtual void dx_create();
	virtual void dx_restore();
	
	void UpdateText(bool b_scroll_to_end);
	void SetFontColor(DWORD dw_color);
	virtual void receive_message(CMessageHeader *p_header, void *p_message);
	void ClearText();
	void SetPageDownKey(byte b_dik){m_b_page_down_dik = b_dik;};
	void SetPageUpKey(byte b_dik){m_b_page_up_dik = b_dik;};
	void SetFontId(int i_font) {m_i_font = i_font;}
	void UpdatePositions();
     int AddOkButton(D3DXVECTOR2 pt_pos, const char* p_st_up, const char* p_st_down);
	void SetScriptCallbackOnOk(int i_script);
	void SetClickSound(char *st_name);
    void SetFadeUp(float f_speed);
    void SetFadeDown(float f_speed);
    void ProcessFades();
    bool CanScrollDown();
    void ScrollDown();
    void set_xy(float f_x, float f_y);
    void SetResAdjust(int i_type, int i_base_x, int i_base_y);

protected:
  
    void UpdateTextPosition();
    void UpdateScrollDownPosition();
    void UpdateScrollUpPosition();
    void UpdateOkPosition();
 	
   CPic *m_p_pic_text;
   CPic *m_p_pic_bg;
   //CSlider *p_slider;
   int m_i_layer;
   D3DXVECTOR2 m_pt_text_offset; //we add it to our position to control the text start position,
   //it could be different from  the background
   D3DXVECTOR2 m_pt_text_limit; //the size of our draw area may be bigger than we actually
   //want to use.  (texture ^2 limitation)
   
   WCHAR *m_p_wchar; //our actual text.  wide char, any length ok
   int m_i_font; //index of font we should use
   DWORD m_dw_color;   //save info about how we draw our font
   DWORD m_dw_options; //save info about how we draw our font
   int m_i_total_lines; //how many lines of text we have
   int m_i_display_lines; //how many lines we can show at once
   int m_i_cur_line; //line we're on now
 

   	char m_st_click_sound[256];

   //up/down scroll buttons

   CPic *m_p_pic_down; 
   D3DXVECTOR2 m_pt_down_offset;

   CPic *m_p_pic_up; 
   D3DXVECTOR2 m_pt_up_offset;


   CPic *m_p_pic_ok; 
   D3DXVECTOR2 m_pt_ok_offset;


   //optional extra controls
   int m_b_page_up_dik;
   int m_b_page_down_dik;

   bool b_update_text;

   int m_i_script; //if != C_SCRIPT_NONE we're supposed to kickstart a script that's waiting
   //for us when we get user input.  Could be OK for a textbox or an actual choice.

   DWORD m_dw_inactive_color; //inactive color
   float m_f_fade_speed;
   int m_i_fade_control;
   float m_f_fade_alpha; //from 0 to 1

   bool m_b_wait_to_click; //used for the ok button
   CTimer m_timer_wait_to_click; //used to show the ok button for a bit before processing
   int m_i_res_adjust; //if not C_VISUAL_RES_ADJUST_NONE then we need to correct for res changes
   int m_i_res_correct_base_x;
   int m_i_res_correct_base_y;
   float m_f_res_adjust_save_x; //if a screen resize happens, this is used to rebuild the location
   float m_f_res_adjust_save_y; //if a screen resize happens, this is used to rebuild the location

};

