#if !defined(AFX_DLG_WORLD_EDIT_H__02224A12_5E9A_4FDD_AED0_F2E9309F60F9__INCLUDED_)
#define AFX_DLG_WORLD_EDIT_H__02224A12_5E9A_4FDD_AED0_F2E9309F60F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_world_edit.h : header file
//
#include "..\\..\\..\\shared\\3d\\CControlEdit.h"
/////////////////////////////////////////////////////////////////////////////
// dlg_world_edit dialog

class dlg_world_edit : public CDialog
{
// Construction
public:
	dlg_world_edit(CWnd* pParent = NULL);   // standard constructor
   void dlg_world_edit::send_settings_to_pedit();

// Dialog Data
	//{{AFX_DATA(dlg_world_edit)
	enum { IDD = IDD_WORLD_EDIT };
	CComboBox	m_list_mode;
	CListBox	m_list_tex;
	BOOL	m_paste_full;
	BOOL	m_b_flip_x;
	BOOL	m_b_flip_z;
	BOOL	m_b_overlay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_world_edit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CControlEdit *p_edit;
	// Generated message map functions
	//{{AFX_MSG(dlg_world_edit)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSelchangeTexList();
	afx_msg void OnPasteFull();
	afx_msg void OnFlipx();
	afx_msg void OnFlipz();
	afx_msg void OnSelchangeListMode();
	afx_msg void OnOverlay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_WORLD_EDIT_H__02224A12_5E9A_4FDD_AED0_F2E9309F60F9__INCLUDED_)
