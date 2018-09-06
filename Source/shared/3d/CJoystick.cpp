#include "CJoystick.h"
#include <tchar.h>
#include <DXErr8.h>
#include "CThing_GLobals.h"
#include "d3dapp.h"


#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CJoystick::CJoystick()
{
    m_p_did = NULL;
    m_b_last_reading_is_valid = false;
}

CJoystick::~CJoystick()
{
    if (m_p_did) m_p_did->Unacquire();
    SAFE_RELEASE(m_p_did);
}


//-----------------------------------------------------------------------------
// Name: EnumAxesCallback()
// Desc: Callback function for enumerating the axes on a joystick
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumAxesCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext )
{
   	//	return DIENUM_STOP;
    CJoystick * p_joystick = (CJoystick*)pContext;

    if (strcmp(pdidoi->tszName, "Slider") == 0)
	{
	  //there is a slider on this stick.  Let's use it instead if the usual
		//throttle axis.
		//b_use_slider = true;
	}

  //some gamepads will FAIL this call, so I am just setting it to what the normal gamepad
    //settings are anyway.

    //log_msg("Adjusting %s.",pdidoi->tszName, pdidoi->); 
  DIPROPRANGE diprg; 
    diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
    diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
    diprg.diph.dwHow        = DIPH_BYOFFSET; 
    diprg.diph.dwObj        = pdidoi->dwOfs; // Specify the enumerated axis
    diprg.lMin              = 0; 
    diprg.lMax              = 65535; 
    // Set the range for the axis
//    msg
    if( FAILED( p_joystick->GetDeviceID()->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
	{
//		log_error("failed setting a joystick axis.");
		return DIENUM_STOP;
	}
    
    return DIENUM_CONTINUE;

}




bool CJoystick::SetupJoystick( LPDIRECTINPUTDEVICE8 p_new_did)
{
           
      
 HRESULT hr;
    m_p_did = p_new_did;
     //set the data format
     if (FAILED(hr = m_p_did->SetDataFormat(&c_dfDIJoystick2)))
     {
      	LogError(_T("Error setting data format to joystick (%s)"), DXGetErrorString8(hr));
     }
    if (!cls_tglo.p_d3dapp || !cls_tglo.p_d3dapp->m_hWnd)
    {
        LogError(_T("Error initting joystick, hwnd in d3dapp not initted."));
        return false;
    }
      if (FAILED(hr = m_p_did->SetCooperativeLevel(cls_tglo.p_d3dapp->m_hWnd, 
                    DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
      {
      	LogError(_T("Set Cooperative level (joystick) failed (%s)"), DXGetErrorString8(hr));
      
      }
    
    memset(&m_caps, 0, sizeof(DIDEVCAPS));
    m_caps.dwSize = sizeof(DIDEVCAPS);

    if (FAILED(hr =m_p_did->GetCapabilities(&m_caps)))
    {
        	LogError(_T("Couldn't get caps (joystick) failed (%s)"), DXGetErrorString8(hr));
   
    }
   
     
    	log_msg("Found Joystick with %d axis and %d buttons.",m_caps.dwAxes, m_caps.dwButtons);

      m_p_did->EnumObjects( EnumAxesCallback, (VOID*)this, DIDFT_AXIS );


   return true; //success

}     


void CJoystick::Update()
{
    HRESULT hr;
    
    hr = m_p_did->Poll(); 
    if (FAILED(hr)) 
    {
        hr = m_p_did->Acquire();
        //while(hr == DIERR_INPUTLOST) 
          //  hr = m_p_did->Acquire();
       m_b_last_reading_is_valid = false;
    }  else
    {

      hr = m_p_did->GetDeviceState(sizeof(DIJOYSTATE2), &m_js);
      if (FAILED(hr))
      {
          m_b_last_reading_is_valid = false;
      } else
      {
         m_b_last_reading_is_valid = true;
       
         //convert readings to virtual key mappings
         ProcessData();
      }
     
    }
    
}

void CJoystick::SetDirectionButtonInfo(int i_dir, bool b_on)
{
   m_dir[i_dir].b_tapped = false; 
    m_dir[i_dir].b_down_now = b_on;
    
	   if (b_on)
       {
           //key pressed!
           if (m_dir[i_dir].b_tap_locked == false)
           {
               m_dir[i_dir].b_tap_locked = true;
               m_dir[i_dir].b_tapped = true; //register click
           }
       }	else
       {
           //key not pressed
           m_dir[i_dir].b_tap_locked = false;
       }
       
}

void CJoystick::SetButtonInfo(int i_but, bool b_on)
{
     
    m_buttons[i_but].b_down_now = b_on;
    m_buttons[i_but].b_tapped = false;

	   if (b_on)
       {
           //key pressed!
           if (m_buttons[i_but].b_tap_locked == false)
           {
               m_buttons[i_but].b_tap_locked = true;
               m_buttons[i_but].b_tapped = true; //register click
           }
       }	else
       {
           //key not pressed
           m_buttons[i_but].b_tap_locked = false;
           
       }
       
}


void CJoystick::ProcessData()
{
    
    const int i_epsilon = 5000;
    const int i_nutural = 32767; 
    
  

    //let's process the directions and buttons for "tapped" response

  // log_msg("X is %d, y is %d",m_js.lX, m_js.lY );
    if (m_js.lX < i_nutural-i_epsilon)
    {
        //left
        this->SetDirectionButtonInfo(C_INPUT_JOY_LEFT, 1);
    } else
    {
        this->SetDirectionButtonInfo(C_INPUT_JOY_LEFT, 0);
    }
 
     if (m_js.lX > i_nutural+i_epsilon)
    {
        //right
        this->SetDirectionButtonInfo(C_INPUT_JOY_RIGHT, 1);
    } else
    {
        this->SetDirectionButtonInfo(C_INPUT_JOY_RIGHT, 0);
    }
 

    
    
    if (m_js.lY < i_nutural-i_epsilon)
    {
        //up
       this->SetDirectionButtonInfo(C_INPUT_JOY_UP, 1);

    } else
    {
       this->SetDirectionButtonInfo(C_INPUT_JOY_UP, 0);
    }
    
    if (m_js.lY > i_nutural+i_epsilon)
    {
        //down
        this->SetDirectionButtonInfo(C_INPUT_JOY_DOWN, 1);
    } else
    {
        this->SetDirectionButtonInfo(C_INPUT_JOY_DOWN, 0);
    }
    

    //now do same thing but for the buttons


    for (int i=0; i < C_JOY_MAX_BUTTONS; i++)
    {
        if (m_js.rgbButtons[i])
          this->SetButtonInfo(i, 1); else this->SetButtonInfo(i, 0);
    }


}


void CJoystick::SetAllTappedButtonsOff()
{
    for (int i=0; i < C_JOY_MAX_BUTTONS; i++)
    {
          this->m_buttons[i].b_tapped = false;
    }

}





