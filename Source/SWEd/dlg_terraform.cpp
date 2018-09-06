// dlg_terraform.cpp : implementation file
//

#include "stdafx.h"
#include "SWEd.h"
#include "dlg_terraform.h"
#include "..\\..\\..\\shared\\3d\\CThing_globals.h"
#include "..\\..\\..\\shared\\3d\\CMap.h"
#include "..\\..\\..\\shared\\mfc_all.h"
#include "..\\..\\..\\shared\\3d\\CMessageHeader.h"
#include "dls_swed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_terraform dialog


dlg_terraform::dlg_terraform(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_terraform::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_terraform)
	m_f_height = 0.0f;
	m_f_height_scale = 0.0f;
	m_f_hill_freq = 0.0f;
	m_f_hill_scale = 0.0f;
	m_i_run_times = 0;
	//}}AFX_DATA_INIT
}


void dlg_terraform::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_terraform)
	DDX_Text(pDX, IDC_F_HEIGHT, m_f_height);
	DDX_Text(pDX, IDC_F_HEIGHT_SCALE, m_f_height_scale);
	DDX_Text(pDX, IDC_F_HILL_FREQ, m_f_hill_freq);
	DDX_Text(pDX, IDC_F_HILL_SCALE, m_f_hill_scale);
	DDX_Text(pDX, IDC_RUN_TIMES, m_i_run_times);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_terraform, CDialog)
	//{{AFX_MSG_MAP(dlg_terraform)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_terraform message handlers

BOOL dlg_terraform::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->m_f_height = 8;
	this->m_f_height_scale = 0.6f;
	this->m_f_hill_freq = 4;
	this->m_f_hill_scale = 6;
	this->m_i_run_times = 30;
	UpdateData(D_TO_WINDOW);
	ShowWindow(SW_SHOW);
	this->BringWindowToTop();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlg_terraform::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(D_TO_VAR);

       CEngineMessage *p_new = new CEngineMessage();
    p_new->m_i_message_type = C_THING_MSG_GENERIC;
    CMessageGeneric *p_mg = new CMessageGeneric();
    p_new->m_p_message = p_mg;
    p_new->m_b_system_message = true;
    p_mg->i_message_command = C_MESSAGE_COMMAND_TERRAFORM_MAP;
    p_mg->f_height = m_f_height;
    p_mg->f_height_scale = m_f_height_scale;
    p_mg->f_hill_freq = m_f_hill_freq;
    p_mg->f_hill_scale = m_f_hill_scale;
    p_mg->i_run_times = m_i_run_times;
    
    SendWindowsEngineMessage(p_new);
 
	CDialog::OnOK();
}

BOOL dlg_terraform::PreTranslateMessage(MSG* pMsg) 
{
	// Check me out, I'm pretty sure I'm breaking every MFC rule there is!
	   	 	if (glo.p_dlg_main)
	if (glo.p_dlg_main->m_hAcceleratorTable != NULL)
		 if (::TranslateAccelerator(glo.p_dlg_main->m_hWnd, glo.p_dlg_main->m_hAcceleratorTable, pMsg))    
			 return TRUE;


	return CDialog::PreTranslateMessage(pMsg);
}
