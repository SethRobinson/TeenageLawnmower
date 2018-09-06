#if !defined(AFX_DLG_NEW_H__D9B252F1_C65D_4214_8832_2F83CC771FB6__INCLUDED_)
#define AFX_DLG_NEW_H__D9B252F1_C65D_4214_8832_2F83CC771FB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_new.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_new dialog

class dlg_new : public CDialog
{
// Construction
public:
	dlg_new(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_new)
	enum { IDD = IDD_NEW };
	int		m_i_x;
	int		m_i_y;
	float	m_f_size;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_new)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_new)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_NEW_H__D9B252F1_C65D_4214_8832_2F83CC771FB6__INCLUDED_)
