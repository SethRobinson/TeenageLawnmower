// dlg_3d.cpp : implementation file
//
#pragma once


#include "stdafx.h"
#include "dlg_3d.h"

#include "SWEd.h"
#include "dls_swed.h"
#include "..\\..\\..\\shared\\CTimer.h"
CTimer timer_start;
bool b_first_time = true;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 

/////////////////////////////////////////////////////////////////////////////
// dlg_3d dialog


dlg_3d::dlg_3d(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_3d::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_3d)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void dlg_3d::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_3d)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_3d, CDialog)
	//{{AFX_MSG_MAP(dlg_3d)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_3d message handlers


BOOL dlg_3d::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	g_pApp  = &d3dApp;
    bool b_first_time = false;
	//force our size
	if (glo.cls_swed.rect_3d.right == 0)
    {
        b_first_time = true;
        this->SetWindowPos(&wndTop, 0,0,400, 400, SWP_NOZORDER); 
    }
	else
	this->SetWindowPos(&wndTop, glo.cls_swed.rect_3d.left,glo.cls_swed.rect_3d.top,
	glo.cls_swed.rect_3d.right- glo.cls_swed.rect_3d.left,glo.cls_swed.rect_3d.bottom-glo.cls_swed.rect_3d.top-100, SWP_NOZORDER); 
	   // Get the top level parent hwnd


    d3dApp.m_hWndTopLevelParent = GetTopLevelParent()->GetSafeHwnd();
  	d3dApp.m_hwndRenderWindow = this->m_hWnd;
  	
	d3dApp.m_hWnd = d3dApp.m_hwndRenderWindow;
 //   d3dApp.m_hwndRenderFullScreen = d3dApp.m_hwndRenderWindow;
	d3dApp.m_hWndFocus = GetTopLevelParent()->GetSafeHwnd();
		d3dApp.b_mfc = true;
  
    d3dApp.Create( AfxGetInstanceHandle() );

	
	glo.p_dlg_3d = this;
	
   
	//do a bunch of weirdness to get the frame the right size without garbage around it
	
	this->d3dApp.m_rcWindowClient.bottom -=20; //make them different so it will
	//actually resize it
	this->d3dApp.Pause(true);
	PostMessage(WM_EXITSIZEMOVE);
    	if (b_first_time)
	this->SetWindowPos(&wndTop, 10,60,600, 400, SWP_NOZORDER| SWP_FRAMECHANGED); else

    this->SetWindowPos(&wndTop, glo.cls_swed.rect_3d.left,glo.cls_swed.rect_3d.top,
		glo.cls_swed.rect_3d.right- glo.cls_swed.rect_3d.left,glo.cls_swed.rect_3d.bottom-glo.cls_swed.rect_3d.top, 
		SWP_NOZORDER| SWP_FRAMECHANGED); 
      
    
    //let's load the real level very soon
    SetTimer(0, 0, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL dlg_3d::PreTranslateMessage(MSG* pMsg) 
{

    	if (this->GetFocus() == this)
	{
		cls_input.HandleMouseMessages( pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam );
	}
	

	if (pMsg->message == WM_KEYDOWN)
	 {
	  if (pMsg->wParam == VK_ESCAPE)
        return TRUE;
	   
	   if (pMsg->wParam == VK_RETURN)
        return TRUE;

	 }
   
    if (glo.p_dlg_main)
	if (glo.p_dlg_main->m_hAcceleratorTable != NULL)
		 if (::TranslateAccelerator(glo.p_dlg_main->m_hWnd, glo.p_dlg_main->m_hAcceleratorTable, pMsg))    
			 return TRUE;
     

	return CDialog::PreTranslateMessage(pMsg);
}


void dlg_3d::OnPaint() 
{
    CPaintDC dc(this); // device context for painting
 	
	if( d3dApp.IsReady() )
    {
         d3dApp.CheckForLostFullscreen();
        if (d3dApp.m_pd3dDevice)
        {
        
        d3dApp.RenderScene();
        }
  }
   
  
}


LRESULT dlg_3d::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
 
	if (message == WM_KILLFOCUS)
	{
	   if (cls_tglo.p_cls_input)
		   cls_tglo.p_cls_input->set_focus(false);
	}
    if (message == WM_SETFOCUS)
    {
        if (cls_tglo.p_cls_input)
        {
            cls_tglo.p_cls_input->set_focus(true);
        }
        
    }
    
 
    if (message == WM_EXITSIZEMOVE)
	{
	  //kickstart the painting again
		Invalidate(false);
	  	if (b_first_time)
		{
	    load_initial_scene();
		 b_first_time = false;
		}

       //clean up any other active windows too
       
	}
 // 	return CDialog::WindowProc(message, wParam, lParam);

   this->d3dApp.MsgProc(this->m_hWnd, message, wParam,lParam);
    this->Invalidate(false);
    
    return CDialog::WindowProc(message, wParam, lParam);
}



