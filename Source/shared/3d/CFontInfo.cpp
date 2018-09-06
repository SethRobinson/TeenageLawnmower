/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


      

#include "CFontInfo.h"
#include "..\uni.h"
#include "assert.h"
#include "..\std_all.h"
#include <DXErr8.h>

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define D_USE_ANSI

CFontInfo::CFontInfo()
{
   m_st_name[0] = 0;
   m_pFont = NULL;
   m_dw_options = DT_LEFT | DT_WORDBREAK;
   //set defaults
   m_dw_color = D3DCOLOR_ARGB(255, 255, 255, 0);
   m_log_font.lfHeight = 32;
   m_log_font.lfWidth = 0;
   m_log_font.lfEscapement = 0;
   m_log_font.lfOrientation = 0;
   //m_log_font.lfWeight = FW_SEMIBOLD;
   m_log_font.lfWeight = FW_MEDIUM;
   
   m_log_font.lfItalic = FALSE;
   m_log_font.lfUnderline = FALSE;
   m_log_font.lfStrikeOut = FALSE;
   m_log_font.lfCharSet = DEFAULT_CHARSET; //good for XP and 2000

   //OEM_CHARSET
   if (WindowsIs9xVersion())
   {
       
       m_log_font.lfCharSet = OEM_CHARSET;   //good for 98
   }
   
   
    //   m_log_font.lfCharSet = SHIFTJIS_CHARSET;


      
      m_log_font.lfOutPrecision = OUT_CHARACTER_PRECIS;
   m_log_font.lfClipPrecision = CLIP_DEFAULT_PRECIS;
   m_log_font.lfQuality = ANTIALIASED_QUALITY;
   
   
   m_log_font.lfPitchAndFamily = DEFAULT_PITCH;
   m_i_real_height = 0; 
   strcpy(m_log_font.lfFaceName, "Arial");
   m_b_allow_partial_rendering = false; //trim the heigh to perfectly match the font so we won't see any half fonts
}


    
CFontInfo::~CFontInfo()
{

    dx_kill();


}


void CFontInfo::SetFontOptions(int i_weight, bool b_italics)
{

     m_log_font.lfWeight = i_weight;
     m_log_font.lfItalic = b_italics;
    
    if (cls_tglo.IsRunning()) dx_create();

}
void CFontInfo::RenderText(WCHAR *p_st_text, RECT *p_rect_area)
{
	//let's actually make it so our font won't "half" render somewhere because the area isn't big enough
	
if (!m_b_allow_partial_rendering)
	p_rect_area->bottom = (p_rect_area->bottom / GetRealFontHeight()) * GetRealFontHeight();
if (!p_st_text)
{
    assert(0);
    return;
}

   // log_msg("Text %s is %d in size.", uni(p_st_text).to_st(), wcslen(p_st_text));
    


	m_pFont->Begin();

#ifdef D_USE_ANSI
    m_pFont->DrawTextA(uni(p_st_text).to_st(),-1,p_rect_area,m_dw_options,m_dw_color);

#else
    m_pFont->DrawTextW(p_st_text,-1,p_rect_area,m_dw_options,m_dw_color);
#endif
	m_pFont->End();
}

DWORD CFontInfo::SetPenOptions(DWORD dw_new)
{
	DWORD h = m_dw_options;
	m_dw_options = dw_new; 
	return h;
}


//this change won't take place until you dx_create it, btw. 
int CFontInfo::SetFontHeight(int i_height)
{
	if (i_height == m_log_font.lfHeight) 
	{
		//no change needed
		return i_height;

	}
	int i_hold = m_log_font.lfHeight; //save this for later
	m_log_font.lfHeight = i_height;
	if (cls_tglo.IsRunning()) dx_create();

	return i_hold;
}

void CFontInfo::SetFontName(char * st_name, int i_height)
{

 	if (
		(i_height == m_log_font.lfHeight)
		&&
		(stricmp(st_name, m_log_font.lfFaceName) == 0)
		) 
	{
		//no change needed
		return;

	}
  	 strcpy(m_log_font.lfFaceName,  st_name);
	m_log_font.lfHeight = i_height;
	if (cls_tglo.IsRunning()) dx_create();
}


int CFontInfo::GetFontHeight()
{
	return m_log_font.lfHeight;
}

void CFontInfo::dx_kill()
{

  	//clean up anything floating around
	SAFE_RELEASE(m_pFont);

}

void CFontInfo::dx_create()
{
   dx_kill(); //make sure it's really deinitted

   HRESULT hr;

   hr=D3DXCreateFontIndirect(cls_tglo.p_d3dDevice,&m_log_font,&m_pFont);
   
   if(hr != D3D_OK)
   {
     log_error("Failed creating font. (%s)", DXGetErrorString8(hr));
     return;
   }

   //adjust font size to be what we actually use
  	RECT rect;
	SetRect(&rect, 0,0,500, 0);
  #ifdef D_USE_ANSI

    m_i_real_height = m_pFont->DrawTextA(&"TEST",-1,&rect,m_dw_options | DT_CALCRECT,m_dw_color);
#else
    m_i_real_height = m_pFont->DrawTextW((LPCWSTR)&L"TEST",-1,&rect,m_dw_options | DT_CALCRECT,m_dw_color);

#endif
    if (m_i_real_height > 10000)
    {
        //it's obviously garbage, I guess the font wasn't found.
        m_i_real_height = 0;
        log_error("Looks like garbage font was created. Couldn't find %s?", m_log_font.lfFaceName);
    }

//	log_msg("Rebuilt font, true height is only %d.", m_i_real_height);

}

int CFontInfo::ComputeHowManyLines(	WCHAR *p_wchar, int i_width)
{
	if (!p_wchar) return 0; //null!
    
    RECT rect;
	SetRect(&rect, 0,0,i_width, 0);
    #ifdef D_USE_ANSI
  		int i_chars = m_pFont->DrawTextA(uni(p_wchar).to_st(),-1,&rect,m_dw_options | DT_CALCRECT,m_dw_color);

#else
		int i_chars = m_pFont->DrawTextW(p_wchar,-1,&rect,m_dw_options | DT_CALCRECT,m_dw_color);
        
#endif
		if (rect.bottom == 0)
		{
			return 0; //empty?
		}
		
	//	log_msg("Height was %d.", i_chars);
		return (rect.bottom / m_i_real_height);
	
}


int CFontInfo::ComputeStartingChar( WCHAR *p_wchar, int i_width, int i_start_line)
{
	//if we were to start drawing on line i_start_line, which char would we start with?

	if (i_start_line == 1) return 0; //you would always start line 1 with char 0
	
    if (!p_wchar)
    {
        log_error("p_wchar was null in CFontInfo::ComputeStartingChar");
        return 0;
    }
    int i_length = wcslen(p_wchar);
	WCHAR wc_temp; 
	
	for (int i=1; i < i_length; i++)
	{
	
		//check every word
		if (p_wchar[i+1] == L' ')
	 {
	 
		wc_temp = p_wchar[i+1];
		p_wchar[i+1] = 0; //temporarily, let's make this string really small
		
		if (ComputeHowManyLines (p_wchar, i_width) == i_start_line)
		{
			//we just went one char too far
			//fix the char we had effed up first
			p_wchar[i+1] = wc_temp;
			
			//now go back to the start of this word.
			int j=i;
			for (; j > 0; j--)
			{
				if (p_wchar[j-1] == L' ')
				{
					//found the beginning of the word!
					break;
				}
			}
			return j; //this is the char we should start on to start on line i_start_line
		} else
		{
			//fix it and move on
			p_wchar[i+1] = wc_temp;
		}
	 }
	}
	

	//if we got here it means we need to send just the last word.
	for (int j=i_length; j > 0; j--)
	{
		if (p_wchar[j-1] == L' ')
		{
			//found the space before the word.
			return j;
		}
	}
	
	log_error("Error compute starting char.");

	return i_length;

}



