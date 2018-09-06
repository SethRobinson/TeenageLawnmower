#include "CInputJoystick.h"
#include <tchar.h>
#include <DXErr8.h>
#include "dxutil.h"
#include "..\all.h"
#include <assert.h>

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CInputJoystick * g_p_joy_info; //i need this as a global for the enum stuff

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE*     
                                       pdidInstance, VOID* pContext)
{
    HRESULT hr;
    LPDIRECTINPUTDEVICE8 p_new_joystick = NULL;
    // Obtain an interface to the enumerated joystick.
    hr = g_p_joy_info->m_p_di->CreateDevice(pdidInstance->guidInstance,  
                                &p_new_joystick, NULL);
    if(FAILED(hr))
    {
      	LogError(_T("Error enumming a joystick (%s)"), DXGetErrorString8(hr));
    }   else
    {
        g_p_joy_info->AddJoystick(p_new_joystick);

    }
  
    //free it

    //no matter what, continue, we want them all.
    return DIENUM_CONTINUE;
}

bool CInputJoystick::Init(LPDIRECTINPUT8 p_di)
{
  //remember this
    g_p_joy_info = this;
    m_p_di = p_di;
   
    if (!m_p_di) 
    {
        LogError("CInputJoystick -> Why isn't s_p_di initted?");
        return false;
    }
    LogMsg(_T("Initting joystick."));
    //let's enumerate the joysticks attached

     m_p_di->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback,
                       NULL, DIEDFL_ATTACHEDONLY);
    return true; //success
}


bool CInputJoystick::Kill()
{
 LogMsg(_T("Killing joystick."));
  for (unsigned int i=0; i < m_vec_joy.size(); i++)
 {
	 SAFE_DELETE(m_vec_joy[i]);
 }

  m_vec_joy.clear();

  return true; //success
}  

void CInputJoystick::AddJoystick(LPDIRECTINPUTDEVICE8 p_new)
{
     
     //add to our list of joysticks
     m_vec_joy.push_back(NULL); //add the pointer
     int i_index = m_vec_joy.size()-1; //get position in array
     m_vec_joy[i_index] = new CJoystick();

     //actually fill in the info
     m_vec_joy[i_index]->SetupJoystick(p_new);

}

void CInputJoystick::UpdateJoysticks()
{
    for (unsigned int i=0; i < m_vec_joy.size(); i++)
    {
        m_vec_joy[i]->Update();
    }
}

bool CInputJoystick::GetJoystickDirTapped(int i_joy, int i_dir)
{
    if (i_joy >= int(m_vec_joy.size()))
    {
        assert(0); //out of range
        return false;
    }
    return m_vec_joy[i_joy]->GetDirectionTapped(i_dir);
}

bool CInputJoystick::GetJoystickButtonTapped(int i_joy, int i_button)
{
    if (i_joy >= int(m_vec_joy.size()))
    {
        assert(0); //out of range
        return false;
    }
    return m_vec_joy[i_joy]->GetButtonTapped(i_button);
}


bool CInputJoystick::GetAnyJoystickDirTapped(int i_dir)
{
    bool b_tapped = false;
    for (unsigned int i=0; i < m_vec_joy.size(); i++)
    {
        if (m_vec_joy[i]->GetDirectionTapped(i_dir))
        {
            b_tapped = true;
            break; //all done
        }
    }
    return b_tapped;
}

bool CInputJoystick::GetAnyJoystickButtonTapped(int i_button)
{
    bool b_tapped = false;
    for (unsigned int i=0; i < m_vec_joy.size(); i++)
    {
        if (m_vec_joy[i]->GetButtonTapped(i_button))
        {
            b_tapped = true;
            break; //all done
        }
        
    }
    
    return b_tapped;
}

bool CInputJoystick::GetAnyJoystickButton(int i_button)
{
    bool b_tapped = false;
    for (unsigned int i=0; i < m_vec_joy.size(); i++)
    {
        if (m_vec_joy[i]->GetButton(i_button))
        {
            b_tapped = true;
            break; //all done
        }
        
    }
    
    return b_tapped;
}

bool CInputJoystick::GetAnyJoystickDir(int i_dir)
{
    bool b_down = false;
    for (unsigned int i=0; i < m_vec_joy.size(); i++)
    {
        if (m_vec_joy[i]->GetDirection(i_dir))
        {
            b_down = true;
            break; //all done
        }
        
    }
    
    return b_down;
}

   void CInputJoystick::SetAllJoystickButtonsOff()
{
       log_msg("Clearing buttons");
       for (unsigned int i=0; i < m_vec_joy.size(); i++)
    {
        m_vec_joy[i]->SetAllTappedButtonsOff();
    }
    
    return;
}
