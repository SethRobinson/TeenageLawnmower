//******************************************
// copyright 2002	Seth A. Robinson

// This file is NOT public domain and may not be shared, distributed or used
// without specific permission in any way.

// seth@rtsoft.com - www.rtsoft.com
//******************************************


#ifndef AFX_TEST4_H__73F70A65_131B_4E11_8615_BA54BDD6CC91__INCLUDED_
#define AFX_TEST4_H__73F70A65_131B_4E11_8615_BA54BDD6CC91__INCLUDED_




//-----------------------------------------------------------------------------
// Defines, and constants
//-----------------------------------------------------------------------------

#include <windows.h>
#include <basetsd.h>
#include <math.h>
#include <stdio.h>
#include <D3DX8.h>
#include <DXErr8.h>
#include <tchar.h>
#include "3d\\D3DApp.h"
#include "3d\\D3DFont.h"
#include "3d\\D3DFile.h"
#include "3d\\D3DUtil.h"
#include "3d\\DXUtil.h"
#include "resource.h"

#include "CGlobals.h"

   

//-----------------------------------------------------------------------------
// Name: class CMyD3DApplication
// Desc: Application class. The base class (CD3DApplication) provides the 
//       generic functionality needed in all Direct3D samples. CMyD3DApplication 
//       adds functionality specific to this sample program.
//-----------------------------------------------------------------------------
class CMyD3DApplication : public CD3DApplication
{
    BOOL                    m_bLoadingApp;          // TRUE, if the app is loading
    FLOAT                   m_fWorldRotX;           // World rotation state X-axis
    FLOAT                   m_fWorldRotY;           // World rotation state Y-axis

protected:
    HRESULT OneTimeSceneInit();
    HRESULT InitDeviceObjects();
    HRESULT RestoreDeviceObjects();
    HRESULT InvalidateDeviceObjects();
    HRESULT DeleteDeviceObjects();
    HRESULT Render();
    HRESULT FrameMove();
    HRESULT FinalCleanup();
    HRESULT ConfirmDevice( D3DCAPS8*, DWORD, D3DFORMAT );

    HRESULT RenderText();

 
    VOID    ReadSettings();
    VOID    WriteSettings();
     void OnLoseFocus();
     void OnGetFocus();
public:
    LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
    CMyD3DApplication();
};


//this function pointer system is how I change menus.  

extern void (*global_init)(); 
extern void (*global_kill)(); 
extern void (*global_kill_old)(); 
extern void (*global_think)();
extern void (*global_restore)();

void switch_control(void (*p_init)(),void (*p_kill)(), void (*p_think)() , void (*p_restore)());



#endif // !defined(AFX_TEST4_H__73F70A65_131B_4E11_8615_BA54BDD6CC91__INCLUDED_)
