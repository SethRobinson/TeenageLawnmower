/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CThingFont.h"
#include "dxutil.h"
#include "CThing_globals.h"



#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CThingFont::CThingFont()
{
  this->set_sort(-5000); //make sure we're processed early
  cls_tglo.p_font = this; //so everyone can access us

  //let's add our default font as font 0
  AddFont("Arial", "", 30);

  ResetFastFonts();
  // AddFastFont("Lucida Console", 16, D3DFONT_BOLD);
  set_thing_name("CFontEngine");

}

int CThingFont::AddFastFont(char *st_name, char *st_filename, int i_size, DWORD dw_options)
{
  
       if (st_filename[0] != 0)
    {
        if (!exist(st_filename))
        {
            log_error("Unable to find font %s.", st_filename);
        }

       if (AddFontResource(st_filename) == 0)
       {
           log_error("Unable to add font %s.", st_filename);
       }
    }
    



    m_vec_fast.push_back(NULL);

   int i_index = m_vec_fast.size()-1;

   m_vec_fast[i_index] = new CFastFont();

    //copy our info over into it
   strcpy(m_vec_fast[i_index]->st_font_name, st_name);
   
   m_vec_fast[i_index]->dw_options = dw_options;
   m_vec_fast[i_index]->i_size = i_size;
 

   if (cls_tglo.IsRunning())
   {
       //create the font now

  		m_vec_fast[i_index]->p_font = new CD3DFont(m_vec_fast[i_index]->st_font_name, m_vec_fast[i_index]->i_size, m_vec_fast[i_index]->dw_options);
		m_vec_fast[i_index]->p_font->InitDeviceObjects( cls_tglo.p_d3dDevice);
		m_vec_fast[i_index]->p_font->RestoreDeviceObjects();

   }

   return i_index;
}

CThingFont::~CThingFont()
{
	
	CThingFont::dx_invalidate();

	//kill everything we had allocated before
	for (UINT i=0; i < m_vec_font.size(); i++)
	{
		SAFE_DELETE (m_vec_font[i]);
	}

	m_vec_font.clear();
    ClearFastFonts();
	
}

void CThingFont::ClearFastFonts()
{
    //first invalidate them

     
    for (UINT i=0; i < m_vec_fast.size(); i++)
	{
	    if (m_vec_fast[i]->p_font)
        {
            m_vec_fast[i]->p_font->InvalidateDeviceObjects();
        }
        SAFE_DELETE (m_vec_fast[i]);
	}
  	m_vec_fast.clear();
}


void CThingFont::ResetFastFonts()
{
    ClearFastFonts();
    AddFastFont("Lucida Console", "", 12, D3DFONT_BOLD); //standard, used internally in the engine

    
}

void CThingFont::DrawFastText(int i_fast_font, float f_x, float f_y, DWORD dw_color, char *st_text)
{
	if (i_fast_font < int(m_vec_fast.size()))
	{
		m_vec_fast[i_fast_font]->p_font->DrawText(f_x, f_y, dw_color, st_text);
	} else
	{
		log_error("Fastfont index %d does not exist.", i_fast_font);
	}
}

int CThingFont::AddFont(char st_name[256], char st_filename[256], int i_height)
{
	
    //if it's in this dir, add it as a font resource too
    if (st_filename[0] != 0)
    {
        if (!exist(st_filename))
        {
            log_error("Unable to find font %s.", st_filename);
        }

       if (AddFontResource(st_filename) == 0)
       {
           log_error("Unable to add font %s.", st_filename);
       }
    }
    
    m_vec_font.push_back(NULL);
	int i_cur = m_vec_font.size()-1;
    m_vec_font[i_cur] = new CFontInfo();
	strcpy(m_vec_font[i_cur]->m_st_name, st_name);
	m_vec_font[i_cur]->m_log_font.lfHeight = i_height;
	
	//if we're currently in 3d mode let's create it now
	if (cls_tglo.IsRunning()) m_vec_font[i_cur]->dx_create();
	
	return i_cur; 
}


//change a font's name and height together

void CThingFont::SetFontName(int i_font, char st_name[256], char st_filename[256], int i_height)
{
 
    if (i_font+1 > int(m_vec_font.size()))
    {
        log_error("Font %d has not been added so it can't be changed.", i_font);
        return;
    }


 //  strcpy (st_name, "Arial");
 //  st_filename[0] = 0;

        if (st_filename[0] != 0)
    {
        if (!exist(st_filename))
        {
            log_error("Unable to find font %s. (%s)", st_filename, st_name);
        }

       if (AddFontResource(st_filename) == 0)
       {
           log_error("Unable to add font %s. (%s)", st_filename, st_name);
       }
    }
    

    
    m_vec_font[i_font]->SetFontName(st_name, i_height);

}


void CThingFont::RenderText(int i_font, WCHAR *p_wchar, RECT * p_rect_area)
{
  if (i_font+1 > int(m_vec_font.size()))
    {
        log_error("RenderText: Font %d has not been added so it can't be changed.", i_font);
        return;
    }
	m_vec_font[i_font]->RenderText(p_wchar, p_rect_area);
}

void CThingFont::dx_create()
{
  	dx_invalidate();

	//create all fonts

	//little fast ones
	for (unsigned int i=0; i < m_vec_fast.size(); i++)
	{	 
 		m_vec_fast[i]->p_font = new CD3DFont(m_vec_fast[i]->st_font_name, m_vec_fast[i]->i_size, m_vec_fast[i]->dw_options);
		m_vec_fast[i]->p_font->InitDeviceObjects( cls_tglo.p_d3dDevice);
		m_vec_fast[i]->p_font->RestoreDeviceObjects();
	}
	
	
  	//slow true type ones that support asian chars, good for heavy duty dialog boxes

	for (int i=0; i < m_vec_font.size(); i++)
	{
		m_vec_font[i]->dx_create();
	}

}

int CThingFont::ComputeHowManyLines(int i_font, WCHAR *p_wchar, int i_width)
{
  if (i_font+1 > int(m_vec_font.size()))
    {
        log_error("GetFontHeight : Font %d has not been added so it can't be changed.", i_font);
        return 0;
    }
	return (m_vec_font[i_font]->ComputeHowManyLines(p_wchar, i_width));

}


int CThingFont::ComputeStartingChar(int i_font, WCHAR *p_wchar, int i_width, int i_start_line)
{
  if (i_font+1 > int(m_vec_font.size()))
    {
        log_error("GetFontHeight: Font %d has not been added so it can't be changed.", i_font);
        return 0 ;
    }
	return (m_vec_font[i_font]->ComputeStartingChar(p_wchar, i_width, i_start_line));
}

int CThingFont::GetFontHeight(int i_font)
{
  if (i_font+1 > int(m_vec_font.size()))
    {
        log_error("GetFontHeight: Font %d has not been added so it can't be changed.", i_font);
        return 0;
    }
	return m_vec_font[i_font]->GetFontHeight();
}

 DWORD CThingFont::GetPenColor(int i_font)
{
  if (i_font+1 > int(m_vec_font.size()))
    {
        log_error("GetPenColor: Font %d has not been added.", i_font);
        return 0;
    }
	return m_vec_font[i_font]->GetPenColor();
}
void CThingFont::dx_invalidate()
{
	
	for (unsigned int i=0; i < m_vec_fast.size(); i++)
	{
		SAFE_DELETE(m_vec_fast[i]->p_font);
	}
	
	for (int i=0; i < m_vec_font.size(); i++)
	{
		m_vec_font[i]->dx_kill();
	}

}

void CThingFont::dx_kill()
{
  dx_invalidate();
  

}


void CThingFont::dx_restore()
{
  dx_create();
}

DWORD CThingFont::SetPenColor(int i_font, DWORD dw_color)
{
 if (i_font+1 > int(m_vec_font.size()))
    {
        log_error("SetPenColor: Font %d has not been added so it can't be changed.", i_font);
        return 0 ;
    }
	return m_vec_font[i_font]->SetPenColor(dw_color);
}

DWORD CThingFont::SetPenOptions(int i_font, DWORD dw_options)
{
   if (i_font+1 > int(m_vec_font.size()))
    {
        log_error("SetPenOptions: Font %d has not been added so it can't be changed.", i_font);
        return 0 ;
    }

  	return m_vec_font[i_font]->SetPenOptions(dw_options);
}

int CThingFont::SetFontHeight(int i_font, int i_height)
{
if (i_font+1 > int(m_vec_font.size()))
    {
        log_error("SetFontHeight: Font %d has not been added so it can't be changed.", i_font);
        return 0 ;
    }
	return m_vec_font[i_font]->SetFontHeight(i_height);
}
int CThingFont::GetRealFontHeight(int i_font)
{
if (i_font+1 > int(m_vec_font.size()))
    {
        log_error("GetRealFontHeight: Font %d has not been added so it can't be changed.", i_font);
        return 0 ;
    }
	return m_vec_font[i_font]->GetRealFontHeight();
}

void CThingFont::SetFontOptions(int i_font, int i_weight, bool b_italics)
{
    if (i_font+1 > int(m_vec_font.size()))
    {
        log_error("SetFontOptions: Font %d has not been added so it can't be changed.", i_font);
        return ;
    }
    
    m_vec_font[i_font]->SetFontOptions(i_weight, b_italics);
    
    return;
}




