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

#include "CThing.h"
#include "d3dfont.h"
#pragma warning (disable:4786)
#include <vector>
#include "CFontInfo.h"
using namespace std;


typedef vector<CFontInfo*> FontVec;


//for some reason if I don't totally destroy and recreate the cd3dfont object
//.. so I have to waste memory while storying the creation info here

class CFastFont 
{
public:

	CD3DFont * p_font;
	char st_font_name[80];
	int i_size;
	DWORD dw_options;
    
	CFastFont()
	{
	  p_font = NULL;
	  st_font_name[0] = 0;

	}
};

typedef vector<CFastFont*> FastTextVec;


class CThingFont : public CThing
{
 
public:
  CThingFont(); //init   
  ~CThingFont();

	 virtual void dx_create();
  virtual void dx_kill();
  virtual void dx_invalidate();
  virtual void dx_restore(); 
 
  int CThingFont::AddFont(char st_name[256], char st_filename[256], int i_height);
  void CThingFont::RenderText(int i_font, WCHAR *p_wchar, RECT * p_rect_area);
  DWORD CThingFont::SetPenColor(int i_font, DWORD dw_color);
  DWORD CThingFont::GetPenColor(int i_font);
  DWORD CThingFont::SetPenOptions(int i_font, DWORD dw_options);
  int CThingFont::SetFontHeight(int i_font, int i_height);
  int CThingFont::GetFontHeight(int i_font);
  int CThingFont::ComputeHowManyLines(int i_font, WCHAR *p_wchar, int i_width);
  int CThingFont::ComputeStartingChar(int i_font, WCHAR *p_wchar, int i_width, int i_start_line);
  int CThingFont::GetRealFontHeight(int i_font);
  void CThingFont::SetFontName(int i_font, char st_name[256], char st_filename[256], int i_height);
  int CThingFont::AddFastFont(char *st_name, char *st_filename, int i_size, DWORD dw_options);
  void CThingFont::DrawFastText(int i_fast_font, float f_x, float f_y, DWORD dw_color, char *st_text);
  void CThingFont::SetFontOptions(int i_font, int i_weight, bool b_italics);
  void CThingFont::ClearFastFonts();
  void CThingFont::ResetFastFonts(); //clears them all and adds the default one


private:
  FontVec m_vec_font; //store all our font types
  FastTextVec m_vec_fast; //fast fonts here
};