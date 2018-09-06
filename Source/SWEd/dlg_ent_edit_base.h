#if !defined(AFX_DLG_ENT_EDIT_BASE_H__5BDB2550_E3FF_4684_983A_EC6F499B24B0__INCLUDED_)
#define AFX_DLG_ENT_EDIT_BASE_H__5BDB2550_E3FF_4684_983A_EC6F499B24B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_ent_edit_base.h : header file
//
#include "..\\..\\..\\shared\\3d\\CBaseEntData.h"
/////////////////////////////////////////////////////////////////////////////
// dlg_ent_edit_base dialog

class dlg_ent_edit_base : public CDialog
{
// Construction
public:
	dlg_ent_edit_base(CWnd* pParent = NULL);   // standard constructor
	CBaseEntData *p_base;
// Dialog Data
	//{{AFX_DATA(dlg_ent_edit_base)
	enum { IDD = IDD_EDIT_ENT_BASE };
	CComboBox	m_combo_draw_mode;
	CString	m_cst_name;
	CString	m_cst_visual;
	BOOL	m_b_follow_ground_tilt;
	CString	m_st_size_x;
	CString	m_st_size_y;
	CString	m_st_size_z;
	BOOL	m_b_enable_lighting;
	CString	m_cst_script;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_ent_edit_base)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_ent_edit_base)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDefaults();
	afx_msg void OnChooseGraphic();
	afx_msg void OnEnableLighting();
	afx_msg void OnChooseScript();
	afx_msg void OnEditScript();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_ENT_EDIT_BASE_H__5BDB2550_E3FF_4684_983A_EC6F499B24B0__INCLUDED_)
