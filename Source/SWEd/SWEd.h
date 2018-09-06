// SWEd.h : main header file for the SWED application
//

#if !defined(AFX_SWED_H__569DBD44_19B8_4D5D_B667_20EDE4364561__INCLUDED_)
#define AFX_SWED_H__569DBD44_19B8_4D5D_B667_20EDE4364561__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

void kill_all_focus_required_dialogs();

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// SWEd:
// See SWEd.cpp for the implementation of this class
//

class SWEd : public CWinApp
{
public:
	SWEd();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SWEd)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(SWEd)
	afx_msg void OnWorldEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

  

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWED_H__569DBD44_19B8_4D5D_B667_20EDE4364561__INCLUDED_)
