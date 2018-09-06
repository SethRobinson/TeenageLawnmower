#if !defined(AFX_DLG_ITEM_LIST_H__089507DE_F769_40C4_BB62_6592CEF90981__INCLUDED_)
#define AFX_DLG_ITEM_LIST_H__089507DE_F769_40C4_BB62_6592CEF90981__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_item_list.h : header file
//
 #include "..\\..\\..\\shared\\3d\\CEntData.h"

/////////////////////////////////////////////////////////////////////////////
// dlg_item_list dialog

class dlg_item_list : public CDialog
{
// Construction
public:
	dlg_item_list(CWnd* pParent = NULL);   // standard constructor
      CEntData *p_ent;
	 void dlg_item_list::set_parent_item(CEntData *p_ent_data);
	 void dlg_item_list::refresh_list();

	 // Dialog Data
	//{{AFX_DATA(dlg_item_list)
	enum { IDD = IDD_ITEM_LIST };
	CListBox	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_item_list)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_item_list)
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_ITEM_LIST_H__089507DE_F769_40C4_BB62_6592CEF90981__INCLUDED_)
