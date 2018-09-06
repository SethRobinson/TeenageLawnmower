// CDlgEditConfig.cpp : implementation file
//

#include "stdafx.h"
#include "swed.h"
#include "CDlgEditConfig.h"
#include "CGlobals.h"
#include "..\\..\\..\\shared\mfc_all.h"
#include "..\\..\\..\\shared\\3d\\CMessageHeader.h"
#include <afxdlgs.h> 
#include "dlgs.h"
#include "Swed.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEditConfig dialog


CDlgEditConfig::CDlgEditConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEditConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEditConfig)
	m_path = _T("");
	//}}AFX_DATA_INIT
}


void CDlgEditConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEditConfig)
	DDX_Text(pDX, IDC_PATH, m_path);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEditConfig, CDialog)
	//{{AFX_MSG_MAP(CDlgEditConfig)
	ON_BN_CLICKED(IDC_FILE, OnFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEditConfig message handlers

BOOL CDlgEditConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	ShowWindow(SW_SHOW);
	this->BringWindowToTop();   
    
    m_path = glo.cls_swed.st_main_path;

    UpdateData(D_TO_WINDOW);
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEditConfig::OnOK() 
{
    UpdateData(D_TO_VAR);
    
    if (IDOK != MessageBox("Changing the base path will cause you to lose all unsaved work and completely reset the program.  Continue?", "Warning!",  MB_ICONWARNING | MB_OKCANCEL | MB_SETFOREGROUND))
   {
       return;
   }

    //close any dialogs so we don't have them there with outdated data
    kill_all_focus_required_dialogs();


   //copy the dir
    strcpy((char*)&glo.cls_swed.st_main_path, m_path);

    //force the complete refresh to take place
    strcpy((char*)&cls_tglo.mod.m_st_path, m_path);
   
   SendSimpleWindowsEngineMessage(C_MESSAGE_COMMAND_INIT_ENGINE_FROM_PATH, 0);
   CDialog::OnOK();
}

void CDlgEditConfig::OnFile() 
{
	UpdateData(D_TO_VAR);

}
