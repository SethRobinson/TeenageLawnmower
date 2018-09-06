#if !defined(AFX_DLG_EDIT_ENTITY_H__E1B5EEA6_DB00_473E_A062_65D996F9F771__INCLUDED_)
#define AFX_DLG_EDIT_ENTITY_H__E1B5EEA6_DB00_473E_A062_65D996F9F771__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_edit_entity.h : header file
//
class CControlEntEdit; //forward for speed
/////////////////////////////////////////////////////////////////////////////
// dlg_edit_entity dialog

class dlg_edit_entity : public CDialog
{
// Construction
public:
	dlg_edit_entity(CWnd* pParent = NULL);   // standard constructor
   void dlg_edit_entity::build_category_list();
   void dlg_edit_entity::build_item_list();
   void dlg_edit_entity::build_modes();
   void dlg_edit_entity::set_mode(int i_new_mode);
   static void EditCallback();
   CControlEntEdit *p_edit;
   // Dialog Data
	//{{AFX_DATA(dlg_edit_entity)
	enum { IDD = IDD_EDIT_ENTITY };
	CComboBox	m_combo_mode;
	CListBox	m_list_cat;
	CListBox	m_list_ent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_edit_entity)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_edit_entity)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnNewCat();
	afx_msg void OnNewItem();
	afx_msg void OnSelchangeListSub();
	afx_msg void OnSelchangeListEnt();
	afx_msg void OnDblclkListEnt();
	afx_msg void OnSelchangeComboMode();
	afx_msg void OnEditByName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSetfocusListSub();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_EDIT_ENTITY_H__E1B5EEA6_DB00_473E_A062_65D996F9F771__INCLUDED_)
