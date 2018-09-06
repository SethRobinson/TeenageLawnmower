#if !defined(AFX_DLG_ENT_CHOOSE_H__60E4DC3B_54D8_475C_8403_F05A0E101558__INCLUDED_)
#define AFX_DLG_ENT_CHOOSE_H__60E4DC3B_54D8_475C_8403_F05A0E101558__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_ent_choose.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_ent_choose dialog

class dlg_ent_choose : public CDialog
{
// Construction
public:
	dlg_ent_choose(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(dlg_ent_choose)
	enum { IDD = IDD_ENT_CHOOSE };
	CListBox	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_ent_choose)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
         void dlg_ent_choose::RefreshList();

	// Generated message map functions
	//{{AFX_MSG(dlg_ent_choose)
	virtual BOOL OnInitDialog();
	afx_msg void OnEdit();
	afx_msg void OnDelete();
	afx_msg void OnDblclkList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_ENT_CHOOSE_H__60E4DC3B_54D8_475C_8403_F05A0E101558__INCLUDED_)
