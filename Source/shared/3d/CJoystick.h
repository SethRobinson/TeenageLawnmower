#pragma once

#include "..\\all.h"
#include <dinput.h>
#include "..\\Cinput.h"
        
const int C_JOY_MAX_BUTTONS = 12;
const int C_JOY_MAX_DIRECTIONS = 4;

class CJoystick
{

public:
    
    CJoystick();
    ~CJoystick();
    bool SetupJoystick(LPDIRECTINPUTDEVICE8 p_new_did);
    LPDIRECTINPUTDEVICE8 GetDeviceID(){return m_p_did;}
    void SetDirectionButtonInfo(int i_dir, bool b_on);
    void SetButtonInfo(int i_but, bool b_on);
    bool GetDirectionTapped(int i_dir){return m_dir[i_dir].b_tapped;}
    bool GetDirection(int i_dir){return m_dir[i_dir].b_down_now;}
    bool GetButtonTapped(int i_button){return m_buttons[i_button].b_tapped;}
    bool GetButton(int i_button){return m_buttons[i_button].b_down_now;}
    void SetAllTappedButtonsOff();
    
    
    void Update();

private:
    void CJoystick::ProcessData();

   LPDIRECTINPUTDEVICE8 m_p_did;
   DIDEVCAPS m_caps; //the capabilities of this joystick/gamepad
   DIJOYSTATE2 m_js;
   bool m_b_last_reading_is_valid;

   key_info m_dir[C_JOY_MAX_DIRECTIONS]; //room for the buttons plus directions
   key_info m_buttons[C_JOY_MAX_BUTTONS]; //room for the buttons plus directions

};