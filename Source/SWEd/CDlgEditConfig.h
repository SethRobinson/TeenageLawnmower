#if !defined(AFX_CDLGEDITCONFIG_H__F7A2D433_B621_47D9_9CD9_A5E7D2DE9ADB__INCLUDED_)
#define AFX_CDLGEDITCONFIG_H__F7A2D433_B621_47D9_9CD9_A5E7D2DE9ADB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgEditConfig.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEditConfig dialog

class CDlgEditConfig : public CDialog
{
// Construction
public:
	CDlgEditConfig(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEditConfig)
	enum { IDD = IDD_EDITOR_CONFIG };
	CString	m_path;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEditConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEditConfig)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGEDITCONFIG_H__F7A2D433_B621_47D9_9CD9_A5E7D2DE9ADB__INCLUDED_)
