// dlg_ent_choose.cpp : implementation file
//

#include "stdafx.h"
#include "swed.h"
#include "dlg_ent_choose.h"
#include "..\\..\\..\\shared\\3d\\CThing_Globals.h"
#include "..\\..\\..\\shared\\3d\\CEntLib.h"
#include "dlg_pick_entity.h"
#include "dlg_edit_ent_default.h"
#include "..\\..\\..\\shared\\3d\\CThingList.h"
#include "..\\..\\..\\shared\\3d\\CEntItem.h"
#include "..\\..\\..\\shared\\mfc_all.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_ent_choose dialog


dlg_ent_choose::dlg_ent_choose(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_ent_choose::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_ent_choose)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void dlg_ent_choose::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_ent_choose)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_ent_choose, CDialog)
	//{{AFX_MSG_MAP(dlg_ent_choose)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_LBN_DBLCLK(IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_ent_choose message handlers

BOOL dlg_ent_choose::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	              // EXCEPTION: OCX Property Pages should return FALSE
//first populate the list with info


  RefreshList();
 	return TRUE;  // return TRUE unless you set the focus to a control

}

void dlg_ent_choose::RefreshList()
{
    UpdateData(D_TO_VAR);
    
    this->m_list.ResetContent();

  CEntItem *p_temp;
  CString cst_temp;

  while (p_temp = (CEntItem*) cls_tglo.p_thing_list->get_next())
  {
      if (p_temp->get_type() == C_THING_ENT_GENERIC)
      {
      
      cst_temp.Format("%s (id %d)",p_temp->get_thing_name(), p_temp->get_id());
      int i_index = m_list.AddString(cst_temp);
      m_list.SetItemData(i_index,p_temp->get_id()); //remember this id for later
    }
  }
    UpdateData(D_TO_WINDOW);
	
}


void dlg_ent_choose::OnEdit() 
{
    UpdateData(D_TO_VAR);
    int i_index = m_list.GetCurSel();

   if (i_index == LB_ERR)
   {
       log_msg("Nothing selected.");
           return;
   }

   CEntItem *p_ent = (CEntItem*) cls_tglo.p_thing_list->get_thing_by_id(m_list.GetItemData(i_index));

   if (!p_ent)
   {
       log_msg("Error getting item to edit.");
       return;
   }


    //was an item selected?  Find it.
   p_ent->update_ent_data();

   /*
   dlg_edit_ent_default *dlg = new dlg_edit_ent_default();
   dlg->set_item_to_edit( p_ent->get_entity_pointer() );
  cls_tglo.p_cls_input->set_focus(false);
   //set entity pointer we're editting
  dlg->Create(IDD_EDIT_ENT_DEFAULT, this->m_hWndTop);
  dlg->set_delete_self_on_exit(true);
 
  */
   dlg_edit_ent_default dlg;
   
   //set entity pointer we're editting
   dlg.set_thing_ent_to_update(p_ent); //so it will get visually redrawn
   dlg.set_item_to_edit(p_ent->get_entity_pointer());
   
   
   dlg.DoModal();
  dlg_ent_choose::RefreshList();
}

void dlg_ent_choose::OnDelete() 
{
	// TODO: Add your control notification handler code here
  UpdateData(D_TO_VAR);
    int i_index = m_list.GetCurSel();

   if (i_index == LB_ERR)
   {
       log_msg("Nothing selected.");
           return;
   }

   CEntItem *p_ent = (CEntItem*) cls_tglo.p_thing_list->get_thing_by_id(m_list.GetItemData(i_index));

   if (!p_ent)
   {
       log_msg("Error getting item to edit.");
       return;
   }

     p_ent->b_delete_flag = true;
     cls_tglo.p_thing_list->process_delete_requests(); 
     
     dlg_ent_choose::RefreshList();

}

void dlg_ent_choose::OnDblclkList() 
{
	// TODO: Add your control notification handler code here
	OnEdit();
}
