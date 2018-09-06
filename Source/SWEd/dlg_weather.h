#if !defined(AFX_DLG_WEATHER_H__0710B560_4BDE_400D_83CC_1B8EE306E47C__INCLUDED_)
#define AFX_DLG_WEATHER_H__0710B560_4BDE_400D_83CC_1B8EE306E47C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_weather.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// dlg_weather dialog
#include "..\\..\\..\\shared\\3d\\CWeatherSetting.h"

class dlg_weather : public CDialog
{
// Construction
public:
	dlg_weather(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(dlg_weather)
	enum { IDD = IDD_WEATHER };
	CString	m_cst_back;
	CString	m_cst_bottom;
	CString	m_cst_front;
	CString	m_cst_left;
	CString	m_cst_right;
	CString	m_cst_top;
	CString	m_skybox_height_offset;
	CString	m_skybox_size;
	CString	m_st_ambient_b;
	CString	m_st_ambient_g;
	CString	m_st_ambient_r;
	CString	m_st_light_b;
	CString	m_st_light_r;
	CString	m_st_light_g;
	CString	m_st_light_dir_x;
	CString	m_st_light_dir_y;
	CString	m_st_light_dir_z;
	CString	m_st_fog_end;
	CString	m_st_fog_start;
	CString	m_st_fog_b;
	CString	m_st_fog_g;
	CString	m_st_fog_r;
	CString	m_st_skydome_tex;
	CString	m_skydome_height_offset;
	BOOL	m_skydome_light_disable;
	CString	m_skydome_size_x;
	CString	m_skydome_size_y;
	CString	m_skydome_size_z;
	CString	m_cst_script;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_weather)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void dlg_weather::GetWeatherSettings(CWeatherSetting * p_weather);
    void dlg_weather::PutWeatherSettings(CWeatherSetting * p_weather);


	// Generated message map functions
	//{{AFX_MSG(dlg_weather)
	afx_msg void OnApply();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSave();
	afx_msg void OnLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_WEATHER_H__0710B560_4BDE_400D_83CC_1B8EE306E47C__INCLUDED_)
