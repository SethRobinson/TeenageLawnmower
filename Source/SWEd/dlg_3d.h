#if !defined(AFX_DLG_3D_H__1AD3C4C1_4134_4102_B8F2_7DB35FF7907E__INCLUDED_)
#define AFX_DLG_3D_H__1AD3C4C1_4134_4102_B8F2_7DB35FF7907E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_3d.h : header file
//
////////////////////////////////////////////////////////////////////////////
// dlg_3d dialog
#include "CDX.h" //directx stuff


class dlg_3d : public CDialog
{
// Construction
public:
	dlg_3d(CWnd* pParent = NULL);   // standard constructor
    CMyD3DApplication d3dApp;
	bool b_left_mouse_button_pressed;
	~dlg_3d()
	{
//	 	this->d3dApp.InvalidateDeviceObjects();
//		this->d3dApp.DeleteDeviceObjects();
//	    this->d3dApp.FinalCleanup();
		   d3dApp.Cleanup3DEnvironment();
      
		   //  d3dApp.DestroyMenu( GetMenu(hWnd) );
          //  d3dApp.DestroyWindow( hWnd );
          //  d3dApp.PostQuitMessage(0);

	}


// Dialog Data
	//{{AFX_DATA(dlg_3d)
	enum { IDD = IDD_3D };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_3d)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_3d)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_3D_H__1AD3C4C1_4134_4102_B8F2_7DB35FF7907E__INCLUDED_)
