//Class to handle input in Directx 8.1 by Seth A. Robinson
#pragma once
//if you get a dinput compiler warning about the version, add this to your preprocesser definitations
//,DIRECTINPUT_VERSION=0x0800

#include <windows.h>
#include <dinput.h>


#include "3d\\CAliasBind.h"

class CInputJoystick;

#pragma warning (disable:4786)
#include <vector>

using namespace std;

// Struct to store the current input state
class key_info
{
public:
  bool b_tapped;
  bool b_tap_locked;
  bool b_down_now;
  key_info()
  {
    b_tapped = false;
	b_tap_locked = false;
    b_down_now = false;
  }

};


//a vkey is a way for us to visual and access a generic key, such as "Jump".  The physical keyboard
//key (or joystick button/mouse) that triggers it is not important to us.

//virtual key consts, for speed
const int C_INPUT_FORWARD = 0;
const int C_INPUT_BACKWARD = 1;
const int C_INPUT_STRAFE_LEFT = 2;
const int C_INPUT_STRAFE_RIGHT = 3;
const int C_INPUT_RIGHT = 4;
const int C_INPUT_LEFT = 5;
const int C_INPUT_JUMP = 6;
const int C_INPUT_ACTION1 = 7;
const int C_INPUT_ACTION2 = 8;
const int C_INPUT_FORWARD_SECONDARY = 9;
const int C_INPUT_BACKWARD_SECONDARY = 10;
const int C_INPUT_START = 11;
const int C_INPUT_ACTION3 = 12;



//defines to make joystick access easier

const int C_INPUT_JOY_LEFT = 0;
const int C_INPUT_JOY_RIGHT = 1;
const int C_INPUT_JOY_UP = 2;
const int C_INPUT_JOY_DOWN = 3;


class CVKey
{
public:
	//id is our index
	char m_st_name[32]; //official name
	key_info kinfo; //is this alias tapped?  Released?  
	bool b_down_now; //is the key down now?
};



//let's set them up in dynamic arrays
typedef vector<CAliasBind*> AliasBindVec;
typedef vector<CVKey> VKeyVec;

#define KEYDOWN(name, key) (name[key] & 0x80) 

enum
{
   C_MOUSE_LEFT = 0,
   C_MOUSE_RIGHT,
   C_MOUSE_MIDDLE,
   C_MOUSE_SPECIAL, //not used, does it actually work?

   //only add more buttons above this line
   C_NUM_MOUSE_BUTTONS
};


struct UserInput
{
    BYTE diks[256];   // DirectInput keyboard state buffer 
	key_info keys[256];
};

class CInput
{
public:
    LPDIRECTINPUT8          m_pDI;                  // DirectInput object
    LPDIRECTINPUTDEVICE8    m_pKeyboard;            // DirectInput keyboard device
    UserInput               m_UserInput;            // Struct for storing user input 
	bool b_exclusive_keyboard;	
	bool b_exclusive_mouse;
	
	LRESULT     HandleMouseMessages( HWND, UINT, WPARAM, LPARAM );
	int i_mouse_change_x, i_mouse_change_y;
	
	bool mouse_button[C_NUM_MOUSE_BUTTONS];
	key_info mouse_info[C_NUM_MOUSE_BUTTONS];
	void set_focus(bool b_new);
	HWND hWnd_save;
	bool b_reset_mouse; //if true, all offsets are reset
	LPDIRECTINPUTDEVICE8 g_pMouse;
	void update_mouse_info();
	void SetMouseExclusive(bool b_exclusive);
	void reset_mouse(); //clear all button presses and movements
	void set_key_tapped(byte dik);
    void set_key_not_tapped(byte dik);

    CInput(); 

    ~CInput();
    HRESULT init( HWND hWnd );
    void update();
    void de_init();
    bool is_key_down(byte dik); 
    bool is_key_tapped(byte dik); 
    inline bool is_mouse_tapped(int i_button){return mouse_info[i_button].b_tapped;};
    inline bool is_mouse_down(int i_button){return mouse_button[i_button];};
    inline int get_mouse_change_x(){return i_mouse_change_x;};
    inline int get_mouse_change_y(){return i_mouse_change_y;};
    void set_mouse_tapped(int i_button){mouse_info[i_button].b_tapped = true;}
    int AddAliasBindScript(char *p_st_keys, char *p_st_script, int i_repeat);
    void InitBaseVirtualKeys();
    int AddVirtualKey(char *st_name);
    int AddAliasBindVirtualKey(char *p_st_keys, int i_vk);
    void SetVirtualKey(int i_vkey, bool b_on);
    bool IsVirtualKeyDown(int i_vkey){return m_vkey_vec[i_vkey].b_down_now;};
    bool IsVirtualKeyTapped(int i_vkey){return m_vkey_vec[i_vkey].kinfo.b_tapped;};
    void GetTextFromVirtualKey(char *p_st_out, char *p_st_vk);
    int  GetVirtualKeyIdFromString(char *p_st_vk);
    bool BuildStringFromKeysDownNow(char *p_out_string, int i_mask);
    int  AddAliasBindVirtualKeyByString(char *p_vstring, char *p_st_keys);
    bool SaveCustomKeyBindings(FILE *fp);
    bool LoadCustomKeyBindings(FILE *fp);
    void AddDefaultKeyBindings();
    void ResetKeyBindings();
    bool GetJoystickDirTapped(int i_joystick, int i_dir);
    bool GetAnyJoystickDirTapped(int i_dir);
    bool GetJoystickButtonTapped(int i_joystick, int i_button);
    bool GetAnyJoystickButtonTapped(int i_button);
    bool GetAnyJoystickButton(int i_button);
    bool GetAnyJoystickDir(int i_dir);
    void SetAllJoystickButtonsOff();
    void SetDisableJoystick(bool b_new);
    int GetMouseX(){return i_mouse_x;};
    int GetMouseY(){return i_mouse_y;};
    void SetMouseX(int i_new){i_mouse_x = i_new;};
    void SetMouseY(int i_new){i_mouse_y = i_new;};
    bool GetMouseExclusive(){return g_pMouse != 0;}
    
    
    
private:
    
    void ProcessAliases();
  
    bool b_has_focus;
    
    AliasBindVec m_alias_bind_vec; //holds our aliases, if any
    VKeyVec m_vkey_vec; //holds are virtual keys, imaginary keys, basically
    CInputJoystick *p_joy;
    bool m_b_disable_joystick; 
    HANDLE m_hMouseEvent;
    int i_mouse_x;
    int i_mouse_y;
    int m_i_old_mouse_x;
    int m_i_old_mouse_y;
    
};