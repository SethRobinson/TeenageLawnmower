#if !defined(AFX_DLG_SUB_PROP_H__8DCB6C01_8006_4BBC_A69C_421DAE20BDC8__INCLUDED_)
#define AFX_DLG_SUB_PROP_H__8DCB6C01_8006_4BBC_A69C_421DAE20BDC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_sub_prop.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_sub_prop dialog

class dlg_sub_prop : public CDialog
{
// Construction
public:
	dlg_sub_prop(CWnd* pParent = NULL);   // standard constructor
	void dlg_sub_prop::save_current_selection();
	void dlg_sub_prop::set_selection(int i_sel);

// Dialog Data
	//{{AFX_DATA(dlg_sub_prop)
	enum { IDD = IDD_SUB_ZONE };
	CComboBox	m_list_roof_tex;
	CComboBox	m_list_cealing_draw;
	CComboBox	m_list_wall_tex;
	CComboBox	m_list_ceil_style;
	CComboBox	m_list_sub;
	CString	m_st_name;
	CString	m_st_ceiling_height;
	CString	m_st_wall_tex_x;
	CString	m_list_wall_tex_y;
	CString	m_st_roof_tex_x;
	CString	m_st_roof_tex_y;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_sub_prop)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void load_sub_buttons();

	// Generated message map functions
	//{{AFX_MSG(dlg_sub_prop)
	virtual BOOL OnInitDialog();
	afx_msg void OnSubNew();
	afx_msg void OnSelchangeListSub();
	virtual void OnOK();
	afx_msg void OnChangeSubName();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_SUB_PROP_H__8DCB6C01_8006_4BBC_A69C_421DAE20BDC8__INCLUDED_)
