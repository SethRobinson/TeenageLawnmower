#if !defined(AFX_DLG_EDIT_SUB_H__C0464060_3DFD_4EF9_A860_E3EC96E88D70__INCLUDED_)
#define AFX_DLG_EDIT_SUB_H__C0464060_3DFD_4EF9_A860_E3EC96E88D70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_edit_sub.h : header file
//
#include "..\\..\\..\\shared\\3d\\CThingSubEdit.h"
/////////////////////////////////////////////////////////////////////////////
// dlg_edit_sub dialog

class dlg_edit_sub : public CDialog
{
// Construction
public:
	dlg_edit_sub(CWnd* pParent = NULL);   // standard constructor
    CThingSubEdit *p_edit;
	 void dlg_edit_sub::build_combo_boxes();
void dlg_edit_sub::set_paint_mode(int i_new);
void dlg_edit_sub::set_region(int i_new);


// Dialog Data
	//{{AFX_DATA(dlg_edit_sub)
	enum { IDD = IDD_EDIT_SUB };
	CComboBox	m_list_sub;
	CComboBox	m_list_mode;
	BOOL	m_b_block;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_edit_sub)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_edit_sub)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSelchangeListMode();
	afx_msg void OnSelchangeListSub();
	afx_msg void OnBlock();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_EDIT_SUB_H__C0464060_3DFD_4EF9_A860_E3EC96E88D70__INCLUDED_)
