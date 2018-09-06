#if !defined(AFX_DLG_LOG_H__74552570_71F2_473C_B978_8B9AA4EACEFC__INCLUDED_)
#define AFX_DLG_LOG_H__74552570_71F2_473C_B978_8B9AA4EACEFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_log.h : header file
//

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// dlg_log dialog

const int d_max_log_lines=200;


class dlg_log : public CDialog
{
// Construction
public:
	void add_log(CString cst_add);
	dlg_log(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_log)
	enum { IDD = IDD_LOG };
	CEdit	cedit_log;
	CString	m_cst_log;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_log)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_log)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_LOG_H__74552570_71F2_473C_B978_8B9AA4EACEFC__INCLUDED_)
