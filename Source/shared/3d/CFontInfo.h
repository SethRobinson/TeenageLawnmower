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
#include "CThing_Globals.h"

class CFontInfo
{
public:

  ~CFontInfo(); //kill everything
	CFontInfo(); //init and clear
	
	char m_st_name[256];
	LOGFONT m_log_font;
	LPD3DXFONT m_pFont;
	
    void CFontInfo::dx_create();
	void CFontInfo::dx_kill();
	void CFontInfo::RenderText(WCHAR *p_st_text, RECT *p_rect_area);
	DWORD GetPenColor() {return m_dw_color;}
	DWORD SetPenColor(DWORD dw_new) {DWORD h = m_dw_color; m_dw_color = dw_new; return h;}
    void CFontInfo::SetFontOptions(int i_weight, bool b_italics);

	DWORD GetPenOptions() {return m_dw_options;}
	DWORD SetPenOptions(DWORD dw_new);
	int SetFontHeight(int i_height);
	int ComputeHowManyLines(WCHAR *p_wchar, int i_width);
	int GetFontHeight();
	int GetRealFontHeight(){return m_i_real_height;}; 
	int CFontInfo::ComputeStartingChar( WCHAR *p_wchar, int i_width, int i_start_line);
	void CFontInfo::SetFontName(char * st_name, int i_height);

private:
	DWORD m_dw_color; //font color, a d3dcolor
	DWORD m_dw_options; //DT_LEFT | DT_WORDBREAK and such
	int m_i_real_height;
	bool m_b_allow_partial_rendering;
};