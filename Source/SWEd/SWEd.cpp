// SWEd.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SWEd.h"
#include "dls_swed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SWEd

BEGIN_MESSAGE_MAP(SWEd, CWinApp)
	//{{AFX_MSG_MAP(SWEd)
	ON_COMMAND(IDR_WORLD_EDIT, OnWorldEdit)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SWEd construction

SWEd::SWEd()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only SWEd object

SWEd theApp;

/////////////////////////////////////////////////////////////////////////////
// SWEd initialization

BOOL SWEd::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
    //switch to current path
    switch_to_current_dir();
    randomize();


#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	dlg_swed dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


void SWEd::OnWorldEdit() 
{
	// TODO: Add your command handler code here
	
}
