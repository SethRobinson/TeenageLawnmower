#if !defined(AFX_DLG_NEW_CAT_H__190764CE_4A77_4889_BC27_3C3661EAA2FE__INCLUDED_)
#define AFX_DLG_NEW_CAT_H__190764CE_4A77_4889_BC27_3C3661EAA2FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_new_cat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_new_cat dialog

class dlg_new_cat : public CDialog
{
// Construction
public:
	dlg_new_cat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_new_cat)
	enum { IDD = IDD_NEW_CAT };
	CString	m_cst_fname;
	CString	m_cst_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_new_cat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_new_cat)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_NEW_CAT_H__190764CE_4A77_4889_BC27_3C3661EAA2FE__INCLUDED_)
