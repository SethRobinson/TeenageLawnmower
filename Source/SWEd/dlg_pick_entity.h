#if !defined(AFX_DLG_PICK_ENTITY_H__DFCD43F3_6C3C_4115_95EF_E2CB6CE629E5__INCLUDED_)
#define AFX_DLG_PICK_ENTITY_H__DFCD43F3_6C3C_4115_95EF_E2CB6CE629E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_pick_entity.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_pick_entity dialog

class dlg_pick_entity : public CDialog
{
// Construction
public:
	dlg_pick_entity(CWnd* pParent = NULL);   // standard constructor
	void dlg_pick_entity::build_item_list();
	void dlg_pick_entity::build_category_list();
	unsigned int m_ui_save_cat;
	int m_i_save_ent;
// Dialog Data
	//{{AFX_DATA(dlg_pick_entity)
	enum { IDD = IDD_PICK_ENTITY };
	CListBox	m_list_cat;
	CListBox	m_list_ent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_pick_entity)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_pick_entity)
	afx_msg void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListSub();
	afx_msg void OnDblclkListEnt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_PICK_ENTITY_H__DFCD43F3_6C3C_4115_95EF_E2CB6CE629E5__INCLUDED_)
