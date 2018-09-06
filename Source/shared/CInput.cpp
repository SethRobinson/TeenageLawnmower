#include "CInput.h"
#include "3d\\CInputJoystick.h"
#include "all.h"
#include <DXErr8.h>
#include "3d\\dxutil.h"
#include <assert.h>

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define C_MOUSE_SAMPLE_BUFFER_SIZE  16


//-----------------------------------------------------------------------------
// Name: InitInput()
// Desc: Initialize DirectInput objects
//-----------------------------------------------------------------------------
CInput::CInput()
{
	p_joy = NULL;
    ZeroMemory(this, sizeof(CInput));
    m_pDI                       = NULL;
    m_pKeyboard                 = NULL;
    ZeroMemory( &m_UserInput, sizeof(m_UserInput) );
	b_exclusive_keyboard = false;
	b_exclusive_mouse = false;
	b_has_focus = true;
	b_reset_mouse = true;
    m_b_disable_joystick = false;
    InitBaseVirtualKeys();
    AddDefaultKeyBindings();
    m_i_old_mouse_x = m_i_old_mouse_y = 0;
   
}
	   
CInput::~CInput()
{
 de_init();

 

}

void CInput::de_init()
{
 
    SAFE_DELETE(p_joy);
    
    ResetKeyBindings();
    // Cleanup DirectX input objects
   	if (m_pKeyboard) m_pKeyboard->Unacquire();
	if (g_pMouse) g_pMouse->Unacquire();
	SAFE_RELEASE( m_pKeyboard );
	SAFE_RELEASE( g_pMouse);
    SAFE_RELEASE( m_pDI );
        
}
  
void CInput::SetDisableJoystick(bool b_new)
{
    
    m_b_disable_joystick = b_new;
    
    if (m_pDI)
    {
        p_joy->Kill();
        
        if (!m_b_disable_joystick)
        {
            
            p_joy->Init(m_pDI);
        }
        
    }
    
}

//if a key is hit, will build the string and return true.

bool CInput::BuildStringFromKeysDownNow(char *p_out_string, int i_mask)
{
  bool b_alt = false;
  bool b_control = false;
  bool b_shift = false;
  char st_key[C_ALIAS_MAX_STRING_SIZE];
  st_key[0] = 0;
  //first, find out if any key is hit.
  for (int i=0; i < 256; i++)
  {
      if (is_key_down(i))
      {
          if ( (i == DIK_LALT) || (i == DIK_RALT) )
          {
              b_alt = true;
              continue;
          }
          
          if ( (i == DIK_LSHIFT) || (i == DIK_RSHIFT) )
          {
              b_shift = true;
              continue;
          }
          
          if ( (i == DIK_LCONTROL) || (i == DIK_RCONTROL) )
          {
              b_control = true;
              continue;
          }
          
          //don't allow certain keys to be scanned
          if (i == DIK_ESCAPE) continue;   //escape is ALWAYS a menu, don't mess with it
          if (i == DIK_F1) continue;   //reserved for help?
          if (i == DIK_F2) continue; //reserved for changing resolution sright now
          
          
          //if we got here, go ahead and convert it.
          log_msg("Hit key %d.", i);
          ConvertDikToString((char*)&st_key, i);
      }
  }
  
  //append control/shift/alt here later


  //when it's all said and done..

  if (st_key[0] != 0)
  {
      //we did find something.
      strcpy(p_out_string, st_key);
      return true;
  }

  return false;

}

void CInput::AddDefaultKeyBindings()
{
    	//Add aliases that will activate them
	AddAliasBindVirtualKey("w", C_INPUT_FORWARD);
	AddAliasBindVirtualKey("up", C_INPUT_FORWARD_SECONDARY);
	AddAliasBindVirtualKey("s", C_INPUT_BACKWARD);
	AddAliasBindVirtualKey("down", C_INPUT_BACKWARD_SECONDARY);
	AddAliasBindVirtualKey("a", C_INPUT_STRAFE_LEFT);
	AddAliasBindVirtualKey("d", C_INPUT_STRAFE_RIGHT);
	AddAliasBindVirtualKey("right", C_INPUT_RIGHT);
	AddAliasBindVirtualKey("left", C_INPUT_LEFT);
	AddAliasBindVirtualKey("space", C_INPUT_JUMP);
	AddAliasBindVirtualKey("[", C_INPUT_ACTION1);
	AddAliasBindVirtualKey("]", C_INPUT_ACTION2);
    AddAliasBindVirtualKey("enter", C_INPUT_ACTION3);
	
	//AddAliasBindVirtualKey("control,nokey", C_INPUT_ACTION1);
	//AddAliasBindVirtualKey(" ", C_INPUT_JUMP);


}
void CInput::ResetKeyBindings()
{
   //dellocate our aliases

 for (unsigned int i=0; i < m_alias_bind_vec.size(); i++)
 {
	 SAFE_DELETE(m_alias_bind_vec[i]);
 }

  m_alias_bind_vec.clear();

}


void CInput::InitBaseVirtualKeys()
{
	log_msg("Initting base VK's");
	
	//generally we would store the return value, but I already know what they will be and
	//make them consts so I don't need to here.  DON'T CHANGE THE ORDER
	AddVirtualKey("FORWARD");
	AddVirtualKey("BACKWARD");
	AddVirtualKey("STRAFE_LEFT");
	AddVirtualKey("STRAFE_RIGHT");
	AddVirtualKey("RIGHT");
	AddVirtualKey("LEFT");
	AddVirtualKey("JUMP");
	AddVirtualKey("ACTION1");
	AddVirtualKey("ACTION2");
	AddVirtualKey("FORWARD2");
	AddVirtualKey("BACKWARD2");
	AddVirtualKey("START");
    AddVirtualKey("ACTION3");

	
}


bool CInput::GetJoystickDirTapped(int i_joystick, int i_dir)
{
    return p_joy->GetJoystickDirTapped(i_joystick, i_dir);
}

bool CInput::GetAnyJoystickDirTapped(int i_dir)
{
    return p_joy->GetAnyJoystickDirTapped(i_dir);
}



bool CInput::GetJoystickButtonTapped(int i_joystick, int i_button)
{
    return p_joy->GetJoystickButtonTapped(i_joystick, i_button);
}

bool CInput::GetAnyJoystickButtonTapped(int i_button)
{
    return p_joy->GetAnyJoystickButtonTapped(i_button);
}

bool CInput::GetAnyJoystickButton(int i_button)
{
    return p_joy->GetAnyJoystickButton(i_button);
}

bool CInput::GetAnyJoystickDir(int i_dir)
{
    return p_joy->GetAnyJoystickDir(i_dir);
}


void CInput::SetAllJoystickButtonsOff()
{
    p_joy->SetAllJoystickButtonsOff();
}

bool CInput::SaveCustomKeyBindings(FILE *fp)
{
    
    char st_string[C_ALIAS_MAX_STRING_SIZE];

  unsigned int i_count = 0; //how many we have to save
  int i_vkey;
    for (unsigned int i=0; i <m_alias_bind_vec.size(); i++)
    {
        if (m_alias_bind_vec[i]->GetVirtualKey() != -1)
        {
            //it's a vk alias, let's save it.
            i_count++;
        }
        
    }

    //first write how many we're going to save
	fwrite(&i_count, sizeof(int), 1, fp);

    for (UINT i=0; i <m_alias_bind_vec.size(); i++)
    {
        if (m_alias_bind_vec[i]->GetVirtualKey() != -1)
        {
            //it's a vk alias, let's save it.
            GetTextFromVirtualKey((char*)&st_string, m_vkey_vec[m_alias_bind_vec[i]->GetVirtualKey()].m_st_name);
            //save it
            //to recreate this binding later we only need two things, the vkey index and the string used to
            //build it.
            //first save the index
            i_vkey = m_alias_bind_vec[i]->GetVirtualKey();
            fwrite(&i_vkey, sizeof(int), 1, fp);

            //now write the string used to create it
            fwrite(&st_string, C_ALIAS_MAX_STRING_SIZE, 1, fp);

        }
        
    }

    return true;
}

bool CInput::LoadCustomKeyBindings(FILE *fp)
{
    //first check to see how many we need to load  
    int i_count;

   	if (fread(&i_count, sizeof(int), 1, fp) == 0)
    {
        //nothing to read
       log_error("Error reading key bindings.");
        return false;
    }

    int i_vk = 0;
    char st_control[C_ALIAS_MAX_STRING_SIZE];

    //loop through and load each one, then init it

    for (int i=0; i < i_count; i++)
    {
        //read the vkey
        if (fread(&i_vk, sizeof(int), 1, fp) == 0)
        {
            log_error("Error reading vk info");
            return false;
        }
      if (fread(&st_control, C_ALIAS_MAX_STRING_SIZE, 1, fp) == 0)
      {
          log_error("Error reading vkey info");
          return false;
      }

        //now init it
       AddAliasBindVirtualKey(st_control, i_vk);
    }

    return true;
}


//32 char name max
int CInput::AddVirtualKey(char *st_name)
{
	
	CVKey vkey;
	strcpy(vkey.m_st_name, st_name); 
	this->m_vkey_vec.push_back(vkey);
	return m_vkey_vec.size()-1;
}

int CInput::GetVirtualKeyIdFromString(char *p_st_vk)
{
    for (unsigned int i=0; i <m_vkey_vec.size(); i++)
    {
        if (stricmp(m_vkey_vec[i].m_st_name, p_st_vk) == 0)
        {
            //found it!
            return i;
        }
    }

    log_error("Unable to find virtual key %s.", p_st_vk);
    return -1; //failed
}


void CInput::GetTextFromVirtualKey(char *p_st_out, char *p_st_vk)
{
    
    
    
    int i_vk = GetVirtualKeyIdFromString(p_st_vk);
    
    
    if (i_vk == -1)
    {
        strcpy(p_st_out, "BADVKEY");
        return;
    }

    //we know the vkey, let's figure out which keys are set to activate it right now.

     
    for (unsigned int i=0; i <m_alias_bind_vec.size(); i++)
    {
        if (m_alias_bind_vec[i]->GetVirtualKey() == i_vk)
        {
            //found it
            m_alias_bind_vec[i]->GetStringFromKey(p_st_out);
            return;
        }
        
    }
    
  //if we got here there was no key attached to it at all.
  strcpy(p_st_out, "none");


}


//vk as in our own kind of vk, not the windows kind
//usage:  AddAliasBindScript("Control,m,a", C_INPUT_FORWARD);

int CInput::AddAliasBindVirtualKey(char *p_st_keys, int i_vk)
{

 int i_index = -1;
	
  //first check, we might already have a control for this action, if so, replace it.
 
 
 for (unsigned int i=0; i <m_alias_bind_vec.size(); i++)
 {
     if (m_alias_bind_vec[i]->GetVirtualKey() == i_vk)
     {
         //found one already using this vkey!  Let's replace it.
         i_index = i;
         break;
     }
     
 }
 
 if (i_index == -1)
 {
     //add key to next spot
     m_alias_bind_vec.push_back(NULL); //add the pointer
     i_index = m_alias_bind_vec.size()-1; //get position in array
     m_alias_bind_vec[i_index] = new CAliasBind();
 } else
 {
     //just clear out the old one.
     m_alias_bind_vec[i_index]->Clear();
     
 }
 //now that's it's actually  been made, let's set it up
 m_alias_bind_vec[i_index]->ConvertStringToKeys(p_st_keys);
 m_alias_bind_vec[i_index]->SetVirtualKey(i_vk);
 //all done
 
 return i_index;
 

}

//same as above but takes a string
int CInput::AddAliasBindVirtualKeyByString(char *p_vstring, char *p_st_keys)
{
    int i_vk = GetVirtualKeyIdFromString(p_vstring);
   
    if (i_vk == -1)
    {
        log_error("Unable to find virtual key %s.", p_vstring);
        return -1;
    }

   return AddAliasBindVirtualKey(p_st_keys,i_vk);
}



//usage:  AddAliasBindScript("Control,m,a", "script\crap.c");

int CInput::AddAliasBindScript(char *p_st_keys, char *p_st_script, int i_repeat)
{

  //add key to next spot

  m_alias_bind_vec.push_back(NULL); //add the pointer
  int i_index = m_alias_bind_vec.size()-1; //get position in array

  m_alias_bind_vec[i_index] = new CAliasBind();

  //now that's it's actually  been made, let's set it up
  m_alias_bind_vec[i_index]->SetRepeat(i_repeat);
  m_alias_bind_vec[i_index]->ConvertStringToKeys(p_st_keys);
  m_alias_bind_vec[i_index]->SetScript(p_st_script);

  //all done
  return i_index;
}





void CInput::reset_mouse()
{
  set_focus(false);
  set_focus(true);

}

void CInput::set_focus(bool b_new)
{
 b_has_focus = b_new; 
 if (b_has_focus)
 {
  b_reset_mouse = true; //mouse can have screwy values
 } else
 {
   //turn off any buttons that were presed
   	for (int i=0; i < ::C_NUM_MOUSE_BUTTONS; i++)
	{
	  mouse_info[i].b_tap_locked = false;
	  mouse_info[i].b_tapped = false;
	  mouse_button[i] = false;
	}

 }

}



LRESULT  CInput::HandleMouseMessages( HWND hWnd, UINT uMsg, WPARAM wParam,
                                          LPARAM lParam )

{

   //this is only needed if you aren't using the DInput mouse stuff (the editor doesn't)  The game itself sets the input
   //stuff in CMouse (based on the CThing class) and does not pass windows messages to here.
    
    // Current mouse position
    int iMouseX = LOWORD(lParam);
    int iMouseY = HIWORD(lParam);

	 switch( uMsg )
    {
        case WM_RBUTTONDOWN:
            i_mouse_x = iMouseX;
            i_mouse_y = iMouseY;
            mouse_button[C_MOUSE_RIGHT] = true;
          	return TRUE;


		
		case WM_MBUTTONDOWN:
            // Store of the position of the cursor when the button is pressed
            i_mouse_x = iMouseX;
            i_mouse_y = iMouseY;
            mouse_button[C_MOUSE_MIDDLE] = true;
            return TRUE;
	  
		case WM_RBUTTONUP:
            i_mouse_x = iMouseX;
            i_mouse_y = iMouseY;
            mouse_button[C_MOUSE_RIGHT] = false;
            return TRUE;


		
		case WM_MBUTTONUP:
            // Store off the position of the cursor when the button is pressed
            i_mouse_x = iMouseX;
            i_mouse_y = iMouseY;
            mouse_button[C_MOUSE_MIDDLE] = false;
            return TRUE;


        case WM_LBUTTONDOWN:
			// Start drag mode
            mouse_button[C_MOUSE_LEFT] = true;
            i_mouse_x = iMouseX;
            i_mouse_y = iMouseY;
           //	log_msg("Mouse down");
       		return TRUE;

        case WM_LBUTTONUP:
      		// End drag mode
             mouse_button[C_MOUSE_LEFT] = false;
		    i_mouse_x = iMouseX;
            i_mouse_y = iMouseY;
//           	log_msg("Mouse up");
	         return TRUE;

        case WM_MOUSEMOVE:
            // Drag object
                // Store mouse coordinate
             
            i_mouse_x = iMouseX;
            i_mouse_y = iMouseY;
            //was return true?
			i_mouse_change_x += (m_i_old_mouse_x - iMouseX);
            i_mouse_change_y += (m_i_old_mouse_y - iMouseY);

            m_i_old_mouse_x = iMouseX;
            m_i_old_mouse_y = iMouseY;

            
            break;
   
	 
        case WM_KILLFOCUS:
            
            {
                set_focus(false);
             }
            break;
            
        case WM_SETFOCUS:

			{
			   set_focus(true);
   			   break;
			}
		
	
        case WM_ACTIVATEAPP:
            {
                bool b_lost_focus = ( LOWORD( wParam ) == WA_INACTIVE ) ? true : false;
                
                if (b_lost_focus)
                {
                    set_focus(false);
             
                }    else
                {
                     set_focus(true);
   		        
                }
            }

	 
	 }

    return FALSE;


}



HRESULT CInput::init( HWND hWnd )
{
    HRESULT hr;
	 hWnd_save = hWnd;


    // Create a IDirectInput8*
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&m_pDI, NULL ) ) )
        return DXTRACE_ERR_NOMSGBOX( "DirectInput8Create", hr );
    
    // Create a IDirectInputDevice8* for the keyboard
    if( FAILED( hr = m_pDI->CreateDevice( GUID_SysKeyboard, &m_pKeyboard, NULL ) ) )
        return DXTRACE_ERR_NOMSGBOX( "CreateDevice", hr );
    
    // Set the keyboard data format
    if( FAILED( hr = m_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
        return DXTRACE_ERR_NOMSGBOX( "SetDataFormat", hr );
    
    // Set the cooperative level on the keyboard
    if (b_exclusive_keyboard)
	{
	if( FAILED( hr = m_pKeyboard->SetCooperativeLevel( hWnd, 
                                            DISCL_NONEXCLUSIVE | 
                                            DISCL_FOREGROUND | 
                                            DISCL_NOWINKEY ) ) )
        return DXTRACE_ERR_NOMSGBOX( "SetCooperativeLevel", hr );
    } else
	{

	if( FAILED( hr = m_pKeyboard->SetCooperativeLevel( hWnd, 
                                            DISCL_NONEXCLUSIVE | 
                                            DISCL_BACKGROUND /*| 
                                            DISCL_NOWINKEY*/ ) ) )
        return DXTRACE_ERR_NOMSGBOX( "SetCooperativeLevel", hr );

	}


    // Acquire the keyboard
    m_pKeyboard->Acquire();

    //Acquire the mouse
   if (b_exclusive_mouse)
   {
   
	hr = m_pDI->CreateDevice(GUID_SysMouse, &g_pMouse, NULL);
 
	if (FAILED(hr)) 
	{
		return DXTRACE_ERR_NOMSGBOX( "CreateDevice (mouse) ", hr );
	}

hr = g_pMouse->SetDataFormat(&c_dfDIMouse);
 
if (FAILED(hr))
 {
   	return DXTRACE_ERR_NOMSGBOX( "SetDataFormat (mouse) ", hr );

}



SetMouseExclusive(b_exclusive_mouse);


//setup for buffered mouse input

assert(!m_hMouseEvent && "This shouldn't be initted yet!");

m_hMouseEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

if (!m_hMouseEvent)
{
    LogError("Failed to create mouse event.");
    assert(0);
    
} else
{
    
    hr = g_pMouse->SetEventNotification(m_hMouseEvent);
    
    if (FAILED(hr)) 
    {
        assert(0);
        return DXTRACE_ERR_NOMSGBOX( "SetEventNotification (mouse) ", hr );
    }


    DIPROPDWORD dipdw;
    // the header
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    // the data
    dipdw.dwData            = C_MOUSE_SAMPLE_BUFFER_SIZE;
    
    hr = g_pMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
    
    if (FAILED(hr)) 
    {
        assert(0);
        return DXTRACE_ERR_NOMSGBOX( "SetProperty (mouse) ", hr );
    }
  
}
   }

    SAFE_DELETE(p_joy);
     p_joy = new CInputJoystick;
  //hey I know, let's init the joystick too, if possible.
     if (!m_b_disable_joystick)
     p_joy->Init(m_pDI);

    return S_OK;
}
		


void CInput::SetMouseExclusive(bool b_exclusive)
{

	
	b_exclusive_mouse = b_exclusive;
	b_reset_mouse = true;

   	if (g_pMouse) g_pMouse->Unacquire(); else 
    {
        
        return; //mouse not initted
    }


 HRESULT hr;

 if (b_exclusive_mouse)
{
	hr = g_pMouse->SetCooperativeLevel(hWnd_save,
		DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	
	if (FAILED(hr)) 
	{
		DXTRACE_ERR_NOMSGBOX( "SetCooperativeLevel (mouse) ", hr );
		return;
		
	}
	
	
} else
{
	
hr = g_pMouse->SetCooperativeLevel(hWnd_save, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND );
	
	if (FAILED(hr)) 
	{
		 DXTRACE_ERR_NOMSGBOX( "SetCooperativeLevel (mouse) ", hr );
		return;
	}
	
}




}

void CInput::SetVirtualKey(int i_vkey, bool b_on)
{
	m_vkey_vec[i_vkey].b_down_now = b_on;

	   if (b_on)
	  {
	   	//key pressed!
			   if (this->m_vkey_vec[i_vkey].kinfo.b_tap_locked == false)
			   {
				   this->m_vkey_vec[i_vkey].kinfo.b_tap_locked = true;
				   this->m_vkey_vec[i_vkey].kinfo.b_tapped = true; //register click
			   }
	}	else
	  {
	    //key not pressed
		this->m_vkey_vec[i_vkey].kinfo.b_tap_locked = false;
	  }

}


//-----------------------------------------------------------------------------
// Name: UpdateInput()
// Desc: Update the user input.  Called once per frame 
//-----------------------------------------------------------------------------
void CInput::update()
{

	if (!b_has_focus)
	{
		ZeroMemory( &m_UserInput.diks, sizeof(m_UserInput.diks) );

		//no control update
		//but let's turn off any keys
		for (UINT i=0; i < 256; i++)
		{
			//shut off any active keypress
			m_UserInput.keys[i].b_tapped = false;
		}
		
	  
		   //also reset mouse info
		   for (int i=0; i < ::C_NUM_MOUSE_BUTTONS; i++)
		   {
			   mouse_info[i].b_tap_locked = false;
			   mouse_info[i].b_tapped = false;
			   mouse_button[i] = false;
		   }
         
		  
            i_mouse_change_x = 0;
		   i_mouse_change_y = 0;

		   //also turn off all our vkeys
		   for (int i=0; i < m_vkey_vec.size(); i++)
		   {
			   m_vkey_vec[i].kinfo.b_tapped = false;
		   }

		   return;  
	   }
       i_mouse_change_x = 0;
       i_mouse_change_y = 0;
       
    if (!m_pKeyboard) return; 

	UserInput* pUserInput = &this->m_UserInput;
	HRESULT hr;

    // Get the input's device state, and put the state in dims
    ZeroMemory( &pUserInput->diks, sizeof(pUserInput->diks) );
    hr = m_pKeyboard->GetDeviceState( sizeof(pUserInput->diks), &pUserInput->diks );
   
	if( FAILED(hr) ) 
    {
        m_pKeyboard->Acquire();
        //return; 
    } else
	{
	

	//refresh key information
	for (int i=0; i < 256; i++)
	{
	   //shut off any active keypress
	  m_UserInput.keys[i].b_tapped = false;
	  
	   if (this->is_key_down(i))
	  {
	   	//key pressed!
			   if (this->m_UserInput.keys[i].b_tap_locked == false)
			   {
				   m_UserInput.keys[i].b_tap_locked = true;
				   m_UserInput.keys[i].b_tapped = true; //register click
			   }
	}	else
	  {
	    //key not pressed
		this->m_UserInput.keys[i].b_tap_locked = false;
	  }

	}
	}
      

		//also turn off all our vkeys
			for (unsigned int i=0; i < m_vkey_vec.size(); i++)
			{
				m_vkey_vec[i].kinfo.b_tapped = false;
			}


	//update mouse info

	for (int i=0; i < ::C_NUM_MOUSE_BUTTONS; i++)
	{
	   //shut off any active mouse taps
	  mouse_info[i].b_tapped = false;
	  
	  if (this->mouse_button[i])
	  {
	   	//key pressed!
		if (this->mouse_info[i].b_tap_locked == false)
		{
		  mouse_info[i].b_tap_locked = true;
		  mouse_info[i].b_tapped = true; //register click
		}

	  }	else
	  {
	    //key not pressed
		this->mouse_info[i].b_tap_locked = false;
	  }

	}
  
	update_mouse_info();
 
  
   //ok, we got all info, let's also cycle through our keybindings
    ProcessAliases();
  
     p_joy->UpdateJoysticks();


 }


 void CInput::ProcessAliases()
 {
	 for (unsigned int i=0; i < this->m_alias_bind_vec.size(); i++)
	 {
		m_alias_bind_vec[i]->Update();
	 }
 }
 
 
 bool CInput::is_key_down(byte dik)
{
  return (KEYDOWN(this->m_UserInput.diks, dik) != 0); 
}


bool CInput::is_key_tapped(byte dik)
{
  return (this->m_UserInput.keys[dik].b_tapped); 
}

void CInput::set_key_tapped(byte dik)
{
	m_UserInput.keys[dik].b_tapped = true;
}

void CInput::set_key_not_tapped(byte dik)
{
	m_UserInput.keys[dik].b_tapped = false;
}

void CInput::update_mouse_info()
{

    if (!g_pMouse) return;
    
    DIDEVICEOBJECTDATA od;
    HRESULT hr;
    while(1)
    {
        
        DWORD dwElements = 1;
        
        hr = g_pMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), 
            &od, &dwElements, 0);
        
        
 if (FAILED(hr)) 
 { 
	 if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) 
	 { 
		 // device lost... reacquire 
		 hr = g_pMouse->Acquire(); 
		 if (FAILED(hr)) 
		 { 
			 // error handling goes here 
//			 log_error("Failed to aquire mouse.");
			 return;
			 
		 } 
	 } 
	 else 
	 { 
		 // it’s some other error – handle it 
         log_error("Mouse error.");
		 return;
         
	 } 
 } 

if (dwElements == 0)
{
  
    return; //done reading stuff
} 

    switch (od.dwOfs)
    {
    
    
    case DIMOFS_X:
        i_mouse_change_x += od.dwData;
        break;
    
    case DIMOFS_Y:
        i_mouse_change_y += od.dwData;
        break;
    
    case DIMOFS_BUTTON0:
        mouse_button[0] = ((od.dwData & 0x80))!=0;
        break;

    case DIMOFS_BUTTON1:
        mouse_button[1] = ((od.dwData & 0x80))!=0;
        break;

    case DIMOFS_BUTTON2:
        mouse_button[2] = ((od.dwData & 0x80))!=0;
        break;

    case DIMOFS_BUTTON3:
        mouse_button[3] = ((od.dwData & 0x80))!=0;
        break;
       
    }


/*
if (this->b_exclusive_mouse)
{
    
    //a better way to read the mouse buttons
    for (int q=0; q < C_NUM_MOUSE_BUTTONS; q++) 
    { 
        mouse_button[q] = ((dims.rgbButtons[q] & 0x80))!=0; 
    } 
}

*/

  }
    
    if (b_reset_mouse)
    {
        i_mouse_change_x = 0;
        i_mouse_change_y = 0;
        b_reset_mouse = false;
        
    } 
}

