#if !defined(AFX_DLG_EDIT_ENT_DEFAULT_H__3BAED6BE_30D0_414C_9810_A16789FF13A0__INCLUDED_)
#define AFX_DLG_EDIT_ENT_DEFAULT_H__3BAED6BE_30D0_414C_9810_A16789FF13A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_edit_ent_default.h : header file
//
#include "..\\..\\..\\shared\\3d\\CEntData.h"

class CEntItem; //forward for speed

/////////////////////////////////////////////////////////////////////////////
// dlg_edit_ent_default dialog

class dlg_edit_ent_default : public CDialog
{
// Construction
public:
	dlg_edit_ent_default(CWnd* pParent = NULL);   // standard constructor
   void dlg_edit_ent_default::set_item_to_edit(CEntData *p_ent_data);
   CEntData *p_ent;
   bool b_delete_self_on_exit;
   void set_delete_self_on_exit(bool b_new){b_delete_self_on_exit=b_new;};
   CEntItem * p_thing_ent;

   void set_thing_ent_to_update(CEntItem *p_ent){p_thing_ent = p_ent;};
  
// Dialog Data
	//{{AFX_DATA(dlg_edit_ent_default)
	enum { IDD = IDD_EDIT_ENT_DEFAULT };
	CString	m_st_size_x;
	CString	m_st_size_y;
	CString	m_st_size_z;
	CString	m_cst_name;
	CString	m_cst_dir;
	BOOL	m_b_ground_sticky;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_edit_ent_default)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_edit_ent_default)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnItems();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_EDIT_ENT_DEFAULT_H__3BAED6BE_30D0_414C_9810_A16789FF13A0__INCLUDED_)
