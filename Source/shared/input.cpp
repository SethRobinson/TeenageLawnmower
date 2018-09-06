/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */




//Copyright 1998: RTsoft, Inc.

#include <windows.h>
#include "stdio.h"
#include <io.h>
#include <stdlib.h>
#include <fcntl.h>
#include <windowsx.h>
#include "string.h"
#include <process.h>
#include <direct.h>
#include <ddraw.h>
#include <dinput.h>

#include "e:\d\sdk\samples\fastfile\fastfile.h"


//global vars for mouse stuff, hope to remove later


LPDIRECTINPUT          g_pdi;
LPDIRECTINPUTDEVICE    g_pMouse;
#define DINPUT_BUFFERSIZE           16

HANDLE                 g_hevtMouse;


BOOL init_mouse(HWND hwnd, HINSTANCE MyInstance)
{
    HRESULT hr;
	
    /*
	*  Register with DirectInput and get an IDirectInput to play with.
	*/
    hr = DirectInputCreate(MyInstance, DIRECTINPUT_VERSION, &g_pdi, NULL);
	
    if (FAILED(hr)) {
        Msg( "DirectInputCreate");
        return FALSE;
    }
	
    /*
	*  Obtain an interface to the system mouse device.
	*/
    hr = g_pdi->CreateDevice(GUID_SysMouse, &g_pMouse, NULL);
	
    if (FAILED(hr)) {
        Msg("CreateDevice(SysMouse)");
        return FALSE;
    }
	
    /*
	*  Set the data format to "mouse format".
	*/
    hr = g_pMouse->SetDataFormat(&c_dfDIMouse);
	
    if (FAILED(hr)) {
		Msg("SetDataFormat(SysMouse, dfDIMouse)");
        return FALSE;
    }
	
    /*
	*  Set the cooperativity level.
	*/
    hr = g_pMouse->SetCooperativeLevel(hwnd,
		DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	
    if (FAILED(hr)) {
		Msg( "Error: SetCooperativeLevel(SysMouse)");
        return FALSE;
    }
	
    /*
	*  Set the buffer size to DINPUT_BUFFERSIZE elements.
	*  The buffer size is a DWORD property associated with the device.
	*/
    DIPROPDWORD dipdw =
	{
		{
			sizeof(DIPROPDWORD),        // diph.dwSize
                sizeof(DIPROPHEADER),       // diph.dwHeaderSize
                0,                          // diph.dwObj
                DIPH_DEVICE,                // diph.dwHow
		},
		DINPUT_BUFFERSIZE,              // dwData
	};
	
    hr = g_pMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	
    if (FAILED(hr)) {
		Msg( "Set buffer size(SysMouse)");
        return FALSE;
    }
	
    
	
	return TRUE;
	
}

