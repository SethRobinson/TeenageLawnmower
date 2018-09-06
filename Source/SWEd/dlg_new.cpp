// dlg_new.cpp : implementation file
//

#include "stdafx.h"
#include "SWEd.h"
#include "dlg_new.h"
#include "..\\..\\..\\shared\\mfc_all.h"
#include "..\\..\\..\\shared\\3d\\CThing_globals.h"
#include "..\\..\\..\\shared\\3d\\CMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_new dialog


dlg_new::dlg_new(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_new::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_new)
	m_i_x = 0;
	m_i_y = 0;
	m_f_size = 0.0f;
	//}}AFX_DATA_INIT
}


void dlg_new::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_new)
	DDX_Text(pDX, IDC_X, m_i_x);
	DDX_Text(pDX, IDC_Y, m_i_y);
	DDX_Text(pDX, IDC_BLOCK_SIZE, m_f_size);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_new, CDialog)
	//{{AFX_MSG_MAP(dlg_new)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_new message handlers

void dlg_new::OnOK() 
{
	// TODO: Add extra validation here
	//create it
	UpdateData(D_TO_VAR);
	 extern int i_last_selection;
	 i_last_selection = -1; //for the edit sub menu
	
	 
	cls_tglo.p_map->generate_map(this->m_i_x, m_i_y, m_f_size, 3);
	cls_tglo.p_map->send_map_to_terrain();
   
	//kill any existing walls


	CDialog::OnOK();
}

BOOL dlg_new::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->m_i_x = 40;
	this->m_i_y = 40;
    m_f_size = 5; //space of tiles
	UpdateData(D_TO_WINDOW);
	ShowWindow(SW_SHOW);
	this->BringWindowToTop();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
