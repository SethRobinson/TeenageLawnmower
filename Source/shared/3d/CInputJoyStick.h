#pragma once

#include <windows.h>
#include <basetsd.h>
#include <math.h>
#include <stdio.h>
#include <dinput.h>

#include "CJoystick.h"
//this handles joysticks and gamepads.  

//we'll throw all enumerated joysticks into an STL vector
#pragma warning (disable:4786)
#include <vector>

using namespace std;

typedef vector<CJoystick*> JoyStickVec;

class CInputJoystick
{
public:
    CInputJoystick()
    {
        m_p_di = NULL;
    }

   ~CInputJoystick()
   {
       Kill();
   }
    
   bool Init(LPDIRECTINPUT8 p_di);
   bool Kill();
   void AddJoystick(LPDIRECTINPUTDEVICE8 p_new);
   void UpdateJoysticks();
   bool GetJoystickDirTapped(int i_joy, int i_dir);
   bool GetJoystickButtonTapped(int i_joy, int i_button);
   bool GetAnyJoystickDirTapped(int i_dir);
   bool GetAnyJoystickButtonTapped(int i_button);
   bool GetAnyJoystickButton(int i_button);
   bool GetAnyJoystickDir(int i_dir);
   void SetAllJoystickButtonsOff();

   LPDIRECTINPUT8 m_p_di; //static so I can use it in
   //the callback function.

private:
 JoyStickVec m_vec_joy; //holds all our joysticks

};
