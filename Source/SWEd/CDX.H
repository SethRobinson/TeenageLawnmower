//-----------------------------------------------------------------------------
// File: test4.h
//
// Desc: Header file test4 sample app
//-----------------------------------------------------------------------------
#pragma once

#include "CGlobals.h"


#include <D3DX8.h>
#include <DXErr8.h>
#include <tchar.h>
#include "..\\..\\..\\shared\\3d\\D3DApp.h"

#include "..\\..\\..\\shared\\3d\\D3DFile.h"
#include "..\\..\\..\\shared\\3d\\D3DUtil.h"
#include "..\\..\\..\\shared\\3d\\DXUtil.h"
#include "resource.h"
void load_initial_scene();

#include "..\\..\\..\\shared\\3d\\CThing_globals.h"


//-----------------------------------------------------------------------------
// Defines, and constants
//-----------------------------------------------------------------------------
// TODO: change "DirectX AppWizard Apps" to your name or the company name
#define DXAPP_KEY        TEXT("Software\\DirectX AppWizard Apps\\test4")





//-----------------------------------------------------------------------------
// Name: class CMyD3DApplication
// Desc: Application class. The base class (CD3DApplication) provides the 
//       generic functionality needed in all Direct3D samples. CMyD3DApplication 
//       adds functionality specific to this sample program.
//-----------------------------------------------------------------------------
class CMyD3DApplication : public CD3DApplication
{
public:
	BOOL                    m_bLoadingApp;          // TRUE, if the app is loading
  
    FLOAT                   m_fWorldRotX;           // World rotation state X-axis
    FLOAT                   m_fWorldRotY;           // World rotation state Y-axis
    HWND    m_hwndRenderWindow;
    HWND    m_hwndRenderFullScreen;
    HWND    m_hWndTopLevelParent;



    HRESULT ConfirmDevice( D3DCAPS8*,DWORD,D3DFORMAT );
	HRESULT OneTimeSceneInit();
    HRESULT InitDeviceObjects();
    HRESULT RestoreDeviceObjects();
    HRESULT InvalidateDeviceObjects();
    HRESULT DeleteDeviceObjects();
    HRESULT Render();
    HRESULT FrameMove();
    HRESULT FinalCleanup();
  
    HRESULT RenderText();
   virtual HRESULT AdjustWindowForChange();


public:
	
  virtual LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
    CMyD3DApplication(); 
	  BOOL IsReady() { return m_bReady; }
 	  VOID RenderScene() { Render3DEnvironment(); }
    HRESULT CheckForLostFullscreen();

};	


extern CMyD3DApplication * g_pApp; 
extern HINSTANCE          g_hInst;
