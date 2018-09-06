// dlg_item_list.cpp : implementation file
//

#include "stdafx.h"
#include "swed.h"
#include "dlg_item_list.h"	 
#include "..\\..\\..\\shared\\3d\\CThing_Globals.h"
#include "..\\..\\..\\shared\\3d\\CEntLib.h"
#include "dlg_pick_entity.h"
#include "dlg_edit_ent_default.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_item_list dialog


dlg_item_list::dlg_item_list(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_item_list::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_item_list)
	//}}AFX_DATA_INIT
}


void dlg_item_list::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_item_list)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_item_list, CDialog)
	//{{AFX_MSG_MAP(dlg_item_list)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_LBN_DBLCLK(IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_item_list message handlers

void dlg_item_list::OnAdd() 
{
	//let them select a new entity to add
	dlg_pick_entity dlg;
	dlg.DoModal();
    if (dlg.m_ui_save_cat != 0)
	{
		//extract info about who they picked
		CBaseEntData *p_base = cls_tglo.p_ent_lib->get_base_item(dlg.m_ui_save_cat, dlg.m_i_save_ent);
		
		
		CEntData * p_ent = NULL;
		p_base->p_ent_default->clone(&p_ent); //send it the pointer, it will init the mem it needs
		log_msg("Just cloned %s.",p_ent->st_name);
		this->p_ent->attach_entity(p_ent);
		refresh_list();
	}
	
}

void dlg_item_list::OnRemove() 
{
	//remove this item from the linked list (delete it basically)

  	int i_num = m_list.GetCurSel();
	
	this->p_ent->remove_entity(i_num); //i_num is the index of the item on the
	//linked list by foward traversal.  Slow? Yes.  Is that ok? Yes.
    
	//update the list
	refresh_list();

}

void dlg_item_list::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL dlg_item_list::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    
	refresh_list();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void dlg_item_list::set_parent_item(CEntData *p_ent_data)
{
	 p_ent = p_ent_data;
}


void dlg_item_list::refresh_list()
{
  this->m_list.ResetContent();

  CEntData *p_temp;
  CString cst_temp;
  while (p_temp = this->p_ent->get_all_attached_entities())
  {
      cst_temp.Format("%s (%s)",p_temp->st_name, cls_tglo.p_ent_lib->base[p_temp->i_class_id]->st_cat_name);
	  m_list.AddString(cst_temp);
  }
	
}


void dlg_item_list::OnDblclkList() 
{
	//let's let them edit this item
	dlg_edit_ent_default dlg;
	
	int i_num = m_list.GetCurSel();
	
	//let's get the address to this item through the linked list

   CEntData *p_temp = NULL;
   CEntData *p_item_to_edit = NULL;
   
   int i_counter = 0;
   while (p_temp = this->p_ent->get_all_attached_entities())
   {
    if (i_counter == i_num)
	{
	  //this is the one we want
	  p_item_to_edit = p_temp;	
	}
    i_counter++;
   }


	if (p_item_to_edit)
	{
    dlg.set_item_to_edit(p_item_to_edit);
	dlg.DoModal();
	refresh_list();
	} else
	{
	  log_error("Unable to find item to edit on linked list..");

	}


}
