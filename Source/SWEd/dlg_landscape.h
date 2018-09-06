#if !defined(AFX_DLG_LANDSCAPE_H__19DD450E_E4D0_4732_B30F_7D27CF93E032__INCLUDED_)
#define AFX_DLG_LANDSCAPE_H__19DD450E_E4D0_4732_B30F_7D27CF93E032__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_landscape.h : header file
//
#include "..\\..\\..\\shared\\3d\\CControlEdit.h"

/////////////////////////////////////////////////////////////////////////////
// dlg_landscape dialog

class dlg_landscape : public CDialog
{
// Construction
public:
	dlg_landscape(CWnd* pParent = NULL);   // standard constructor
	CControlEdit *p_edit;
// Dialog Data
	//{{AFX_DATA(dlg_landscape)
	enum { IDD = IDD_LANDSCAPE };
	CString	m_st_mod_amount;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_landscape)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void process_button(int nID);

	// Generated message map functions
	//{{AFX_MSG(dlg_landscape)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnChangeModAmount();
	afx_msg void OnUpdateAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_LANDSCAPE_H__19DD450E_E4D0_4732_B30F_7D27CF93E032__INCLUDED_)
