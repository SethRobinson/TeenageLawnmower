/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CSettings.h"
#include <D3Dx8.h>
#include "d3dapp.h"
#ifndef _NO_TLM_GLOBALS
#include "CThing_Globals.h"
#endif



void GetOriginalScreenSizeOfOS(int &xOut, int &yOut)
{
	RECT desktop;  
	// Get a handle to the desktop window  
	const HWND hDesktop = GetDesktopWindow();  
	// Get the size of screen to the variable desktop  
	GetWindowRect(hDesktop, &desktop);  
	xOut = desktop.right;  
	yOut = desktop.bottom;  
}


CSettings::CSettings()
{
	 ZeroMemory(this, sizeof(CSettings));
	 m_b_windowed = false;
	 m_i_desired_bits = 32;
	 int x, y;
	 GetOriginalScreenSizeOfOS(x, y);
	 m_point_window.x = x;
	 m_point_window.y = y;

	 m_b_vsync = true;
}

void CSettings::SetVideoSettingsFromConfig(CD3DApplication *p_d3dapp)
{
    if (p_d3dapp)
    {
      LogMsg("Setting vsync.");
      p_d3dapp->SetVSync(m_b_vsync);
      m_b_windowed =  p_d3dapp->m_d3dpp.Windowed != 0;
      m_point_window.x = p_d3dapp->m_d3dpp.BackBufferWidth;
      m_point_window.y = p_d3dapp->m_d3dpp.BackBufferHeight;
    }

    
   #ifndef _NO_TLM_GLOBALS
    if (cls_tglo.p_cls_input)
    {
      cls_tglo.p_cls_input->SetDisableJoystick(m_b_disable_joystick);
    }
#endif
}   
void CSettings::SetKey(const char *p_name, const char *p_key)
{
    strcpy(m_st_name, p_name);
    strcpy(m_st_key, p_key);

}

bool CSettings::KeyInfoExists()
{
   if ( (m_st_key[0] == 0) || (m_st_name[0] == 0))
   {

       return false;
   }

  return true;
}


void CSettings::ResetKey()
{
 m_st_key[0] = 0;
 m_st_name[0] = 0;

}
bool CSettings::Save(char *p_st_fname,  bool b_force_base_dir)
{
    //if a mod is open, let's make sure we save to the correct dir
    char st_name[256];
    if (b_force_base_dir)
    {
        
        sprintf(st_name, "%s%s", GetExeDir(), p_st_fname);
    } else
    {
        sprintf(st_name, "%s", p_st_fname);
        
    }
    //open the file
    FILE *fp = fopen(st_name, "wb");
    
    if (!fp)
    {
		LogError("Error saving CSettings to %s.", st_name);
		return false;
	}
	
	
	int i_size_of_us = sizeof(CSettings);

   	//first write our data
	fwrite(&i_size_of_us, sizeof(int), 1, fp);

	//now write the real data
	fwrite(this, i_size_of_us, 1, fp);

    
#ifndef _NO_TLM_GLOBALS
    
    //write anything else we need to
    if (cls_tglo.p_cls_input)
        cls_tglo.p_cls_input->SaveCustomKeyBindings(fp);
#endif
    fclose(fp);
    return true;
    
    
}


bool  CSettings::Load(char *p_st_fname, bool b_force_base_dir)
{
   //if a mod is open, let's make sure we save to the correct dir
     //the unlock util won't work from other dirs if it keep switching, yet we need this
      //to save correctly when running mods from a different base dir, yet still save
        //global data in the right place
    
    char st_name[256];
    if (b_force_base_dir)
    {
    sprintf(st_name, "%s%s", GetExeDir(), p_st_fname);
    } else
    {
    sprintf(st_name, "%s", p_st_fname);

    }

    FILE *fp = fopen(st_name, "rb");
	if (!fp)
	{
		LogMsg("%s not found, using defaults.", st_name);
		return false;
	}

	//clear us
	ZeroMemory(this, sizeof(CSettings));

	//load it
    int i_our_size = 0;
  
  //first load the size we're supposed to be
  if (!fread(&i_our_size, sizeof(int), 1, fp))
  {
	  LogError("Couldn't read CSettings size info");
	  fclose(fp);
 return false;
  }

  if (!fread(this, i_our_size, 1, fp))
  {
	  fclose(fp);
        LogError("Couldn't read CSetting info");
	  return false;
  }
   
  
  #ifndef _NO_TLM_GLOBALS
  if (cls_tglo.p_cls_input)
      cls_tglo.p_cls_input->LoadCustomKeyBindings(fp);

  #endif  
  //all done
    fclose(fp);
 #ifndef _NO_TLM_GLOBALS
 
//	SetVideoSettingsFromConfig(cls_tglo.p_d3dapp);
#endif

	return true;
}