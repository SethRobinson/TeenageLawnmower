// dls_swed.h : header file
//

#if !defined(AFX_DLS_SWED_H__6DE31499_D57E_4D20_9B42_9F5CDFF340D0__INCLUDED_)
#define AFX_DLS_SWED_H__6DE31499_D57E_4D20_9B42_9F5CDFF340D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dlg_3d.h"
//#include "..\\..\\..\\shared\\btnst.h"

/////////////////////////////////////////////////////////////////////////////
// dlg_swed dialog
void kill_all_focus_required_dialogs();

class dlg_swed : public CDialog
{
// Construction
public:
	void save_defaults();
	dlg_swed(CWnd* pParent = NULL);	// standard constructor
 	HACCEL m_hAcceleratorTable;
 

	~dlg_swed();
	
// Dialog Data
	//{{AFX_DATA(dlg_swed)
	enum { IDD = IDD_SWED_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_swed)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(dlg_swed)
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnViewFreecamera();
	afx_msg void OnViewPlayercamera();
	afx_msg void OnClose();
	afx_msg void OnNewZone();
	afx_msg void OnTerraform();
	afx_msg void OnRandMoveSun();
	afx_msg void OnRandSetFog();
	afx_msg void OnLightReset();
	afx_msg void OnZoneTogglewireframemode();
	afx_msg void OnWorldEdit();
	afx_msg void OnOpenLandscape();
	afx_msg void OnLoadlevelLoadlevel();
	afx_msg void OnLoadlevelSavelevel();
	afx_msg void OnDiagnosticsListloadedresources();
	afx_msg void OnDiagnosticsCreatesomecars();
	afx_msg void OnDiagnosticsCreatesomespheres();
	afx_msg void OnZoneProp();
	afx_msg void OnDiagnosticsListloadedentities();
	afx_msg void OnDiagnosticsCreatesometreesprites();
	afx_msg void OnViewSetcameratotopviewmode();
	afx_msg void OnViewCardrivingmode();
	afx_msg void OnDiagCreatehuman();
	afx_msg void OnSubzoneProperties();
	afx_msg void OnTileToggletileshowing();
	afx_msg void OnOpenSubZoneEdit();
	afx_msg void OnCreate500cars();
	afx_msg void OnViewSetcameratoplayerwalkmode();
	afx_msg void OnOpenEntityTool();
	afx_msg void OnZoneDeleteallents();
	afx_msg void OnWeatherPreset();
	afx_msg void OnEditorConfig();
	afx_msg void OnZoneWire();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLS_SWED_H__6DE31499_D57E_4D20_9B42_9F5CDFF340D0__INCLUDED_)
