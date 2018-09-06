// dlg_new_cat.cpp : implementation file
//

#include "stdafx.h"
#include "swed.h"
#include "dlg_new_cat.h"
#include "..\\..\\..\\shared\\mfc_all.h"
#include "..\\..\\..\\shared\\3d\\CThing_globals.h"
#include "CGlobals.h"
#include "..\\..\\..\\shared\\3d\\CEntLib.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern unsigned int ui_ent_last_cat;

/////////////////////////////////////////////////////////////////////////////
// dlg_new_cat dialog


dlg_new_cat::dlg_new_cat(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_new_cat::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_new_cat)
	m_cst_fname = _T("");
	m_cst_name = _T("");
	//}}AFX_DATA_INIT
}


void dlg_new_cat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_new_cat)
	DDX_Text(pDX, IDC_EDIT_FNAME, m_cst_fname);
	DDX_Text(pDX, IDC_EDIT_SUBCAT, m_cst_name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_new_cat, CDialog)
	//{{AFX_MSG_MAP(dlg_new_cat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_new_cat message handlers

void dlg_new_cat::OnOK() 
{
	// TODO: Add extra validation here
	//get the info and process it
	UpdateData(D_TO_VAR);

	if (!cls_tglo.p_ent_lib)
	{
	  log_error("Entlib not initted yet.");
	  return;
	}

	ui_ent_last_cat = cls_tglo.p_ent_lib->add_class((char*)(LPCSTR)m_cst_fname, (char*)(LPCSTR)(m_cst_fname+".ent"));



	CDialog::OnOK();
}
