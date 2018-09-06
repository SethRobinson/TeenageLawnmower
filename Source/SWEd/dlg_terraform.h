#if !defined(AFX_DLG_TERRAFORM_H__0D624150_7453_4F20_AC95_94FED4156A66__INCLUDED_)
#define AFX_DLG_TERRAFORM_H__0D624150_7453_4F20_AC95_94FED4156A66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_terraform.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_terraform dialog

class dlg_terraform : public CDialog
{
// Construction
public:
	dlg_terraform(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_terraform)
	enum { IDD = IDD_TERRAFORM };
	float	m_f_height;
	float	m_f_height_scale;
	float	m_f_hill_freq;
	float	m_f_hill_scale;
	int		m_i_run_times;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_terraform)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_terraform)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_TERRAFORM_H__0D624150_7453_4F20_AC95_94FED4156A66__INCLUDED_)
