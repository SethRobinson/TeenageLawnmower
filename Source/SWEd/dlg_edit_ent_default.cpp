// dlg_edit_ent_default.cpp : implementation file
//

#include "stdafx.h"
#include "swed.h"
#include "dlg_edit_ent_default.h"
#include "..\\..\\..\\shared\\mfc_all.h"
#include "..\\..\\..\\shared\\3d\\CEntItem.h"

#include "dlg_item_list.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_edit_ent_default dialog


dlg_edit_ent_default::dlg_edit_ent_default(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_edit_ent_default::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_edit_ent_default)
	m_st_size_x = _T("");
	m_st_size_y = _T("");
	m_st_size_z = _T("");
	m_cst_name = _T("");
	m_cst_dir = _T("");
	m_b_ground_sticky = FALSE;
	//}}AFX_DATA_INIT

 //init a few custom vars
 	 b_delete_self_on_exit = false;
	 this->p_thing_ent = NULL; //if not null we'll make this thing update on ok
	 
	 

}


void dlg_edit_ent_default::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_edit_ent_default)
	DDX_Text(pDX, IDC_SIZE_X, m_st_size_x);
	DDX_Text(pDX, IDC_SIZE_Y, m_st_size_y);
	DDX_Text(pDX, IDC_SIZE_Z, m_st_size_z);
	DDX_Text(pDX, IDC_NAME, m_cst_name);
	DDV_MaxChars(pDX, m_cst_name, 39);
	DDX_Text(pDX, IDC_DIR, m_cst_dir);
	DDX_Check(pDX, IDC_GROUND_STICKY, m_b_ground_sticky);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_edit_ent_default, CDialog)
	//{{AFX_MSG_MAP(dlg_edit_ent_default)
	ON_BN_CLICKED(IDC_ITEMS, OnItems)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_edit_ent_default message handlers

void dlg_edit_ent_default::set_item_to_edit(CEntData *p_ent_data)
{
  p_ent = p_ent_data;
}

BOOL dlg_edit_ent_default::OnInitDialog() 
{
	CDialog::OnInitDialog();
	 
	 
	 //hopefully p_ent has already been set or we'll have big problems
	this->m_st_size_x.Format("%.3f", p_ent->vec_scale.x);
	this->m_st_size_y.Format("%.3f", p_ent->vec_scale.y);
	this->m_st_size_z.Format("%.3f", p_ent->vec_scale.z);
	this->m_cst_name.Format("%s", p_ent->st_name);
	this->m_b_ground_sticky = p_ent->b_floating;
	this->m_cst_dir.Format("%.3f", p_ent->f_dir);
	UpdateData(D_TO_WINDOW);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlg_edit_ent_default::OnOK() 
{
	//accept data and copy it

	UpdateData(D_TO_VAR);
	p_ent->vec_scale.x = atof(m_st_size_x);
	p_ent->vec_scale.y = atof(m_st_size_y);
	p_ent->vec_scale.z = atof(m_st_size_z);
	p_ent->f_dir = atof(m_cst_dir);
	p_ent->b_floating = this->m_b_ground_sticky != 0; 
	strcpy(p_ent->st_name, m_cst_name);


	if (p_thing_ent)
	{
	    
		
		//also update the visual of it
		this->p_thing_ent->setup_item_from_cent(p_ent);
	}
	CDialog::OnOK();
}

void dlg_edit_ent_default::OnItems() 
{
	//edit the list of items attached to this
	dlg_item_list dlg;
	dlg.set_parent_item(this->p_ent);
	dlg.DoModal();

}

void dlg_edit_ent_default::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	 if (b_delete_self_on_exit)
	 {
		 //we must be non model dialox box
		 delete this;
	 }
	CDialog::PostNcDestroy();
}
