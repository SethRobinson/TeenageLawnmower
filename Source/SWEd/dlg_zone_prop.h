#if !defined(AFX_DLG_ZONE_PROP_H__D433C5B6_FF02_4551_AA93_2BACB036F412__INCLUDED_)
#define AFX_DLG_ZONE_PROP_H__D433C5B6_FF02_4551_AA93_2BACB036F412__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_zone_prop.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_zone_prop dialog

class dlg_zone_prop : public CDialog
{
// Construction
public:
	dlg_zone_prop(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_zone_prop)
	enum { IDD = IDD_ZONE_PROP };
	CString	m_st_gravity;
	CString	m_st_name;
	CString	m_cst_tile_size;
	CString	m_cst_tiles_per_block;
	CString	m_cst_height_scale_mod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_zone_prop)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_zone_prop)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_ZONE_PROP_H__D433C5B6_FF02_4551_AA93_2BACB036F412__INCLUDED_)
