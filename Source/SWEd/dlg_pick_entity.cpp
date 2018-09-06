// dlg_pick_entity.cpp : implementation file
//

#include "stdafx.h"
#include "swed.h"
#include "dlg_pick_entity.h"
#include "CGLobals.h"
#include "..\\..\\..\\shared\\3d\\CEntLib.h"
#include "..\\..\\..\\shared\\mfc_all.h"


//these are declared elsewhere

extern unsigned int ui_ent_last_cat;
extern int i_ent_last_item;



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_pick_entity dialog


dlg_pick_entity::dlg_pick_entity(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_pick_entity::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_pick_entity)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void dlg_pick_entity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_pick_entity)
	DDX_Control(pDX, IDC_LIST_SUB, m_list_cat);
	DDX_Control(pDX, IDC_LIST_ENT, m_list_ent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_pick_entity, CDialog)
	//{{AFX_MSG_MAP(dlg_pick_entity)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_LBN_SELCHANGE(IDC_LIST_SUB, OnSelchangeListSub)
	ON_LBN_DBLCLK(IDC_LIST_ENT, OnDblclkListEnt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_pick_entity message handlers

void dlg_pick_entity::OnCancel() 
{
	//don't wanna do nuthin
	m_ui_save_cat = 0;
	m_i_save_ent = 0;
	CDialog::OnCancel();


}

BOOL dlg_pick_entity::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ui_save_cat = ui_ent_last_cat;
    this->m_i_save_ent  = i_ent_last_item;
	build_category_list();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void dlg_pick_entity::build_category_list()
{
   this->m_list_cat.ResetContent();

   //traverse the loaded categorys and convert them into strings

   if (!cls_tglo.p_ent_lib)
   {
	log_error("Can't build cat list, p_ent_lib is null");
	return;
   }

   CCategory *p_cat;
   int i_index;
   while (p_cat = cls_tglo.p_ent_lib->get_all_categories())
   {
    i_index = m_list_cat.AddString(p_cat->st_cat_name);
	m_list_cat.SetItemData(i_index, p_cat->ui_class_id); //remember this id for later
   } 

   //set default
   set_selection_by_data(&m_list_cat, ::ui_ent_last_cat);
   //since our category may have just changed, we better update the items list
   build_item_list();

}

void dlg_pick_entity::build_item_list()
{
 	this->m_list_ent.ResetContent();
	
	//build items
	if (::ui_ent_last_cat == 0)
	{
		return; //no category
	}

	if (!cls_tglo.p_ent_lib)
	{
	  log_error("p_ent_lib is null, can't build item list");
	  return;
	}

	
	CBaseEntData *p_base;
    CString cst_temp;
	int i_index;
	while (p_base = cls_tglo.p_ent_lib->get_all_ents_in_this_cat(::ui_ent_last_cat))
	{
	  //process item
	  cst_temp.Format("%s (id %d)", p_base->st_name, p_base->i_index);
	  i_index = m_list_ent.AddString(cst_temp);
	  //also save it's number in the item array
	  m_list_ent.SetItemData(i_index, p_base->i_index);
	}
	
     //set default
	set_selection_by_data(&m_list_ent, ::i_ent_last_item);

}


void dlg_pick_entity::OnSelchangeListSub() 
{
	// TODO: Add your control notification handler code here
		//get class id of category
	ui_ent_last_cat = m_list_cat.GetItemData(m_list_cat.GetCurSel());
	//rebuild item list
	m_ui_save_cat = ui_ent_last_cat;
	build_item_list();

}

void dlg_pick_entity::OnDblclkListEnt() 
{

	//they chose something
	  	::i_ent_last_item = m_list_ent.GetItemData(m_list_ent.GetCurSel());
	   m_i_save_ent = i_ent_last_item;

	
	   CDialog::OnOK();

}
