// dlg_edit_entity.cpp : implementation file
//

#include "stdafx.h"
#include "swed.h"
#include "dlg_edit_entity.h"
 #include "CGLobals.h"
#include "dls_swed.h"
#include "dlg_new_cat.h"
#include "..\\..\\..\\shared\\3d\\CEntLib.h"
#include "dlg_ent_edit_base.h"
#include "..\\..\\..\\shared\\3d\\CControlEntEdit.h"
#include "dlg_edit_ent_default.h"
#include "..\\..\\..\\shared\\3d\\CEntItem.h"
#include "dlg_ent_choose.h"

unsigned int ui_ent_last_cat = 0;
int i_ent_last_item = -1;
int i_ent_last_mode = 0;


const WM_EDIT_ENTITY = WM_USER+20;

#include "..\\..\\..\\shared\\MFC_ALL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// dlg_edit_entity dialog


dlg_edit_entity::dlg_edit_entity(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_edit_entity::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_edit_entity)
	//}}AFX_DATA_INIT
}


void dlg_edit_entity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_edit_entity)
	DDX_Control(pDX, IDC_COMBO_MODE, m_combo_mode);
	DDX_Control(pDX, IDC_LIST_SUB, m_list_cat);
	DDX_Control(pDX, IDC_LIST_ENT, m_list_ent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_edit_entity, CDialog)
	//{{AFX_MSG_MAP(dlg_edit_entity)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_NEW_CAT, OnNewCat)
	ON_BN_CLICKED(IDC_NEW_ITEM, OnNewItem)
	ON_LBN_SELCHANGE(IDC_LIST_SUB, OnSelchangeListSub)
	ON_LBN_SELCHANGE(IDC_LIST_ENT, OnSelchangeListEnt)
	ON_LBN_DBLCLK(IDC_LIST_ENT, OnDblclkListEnt)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, OnSelchangeComboMode)
	ON_BN_CLICKED(IDC_EDIT_BY_NAME, OnEditByName)
	//}}AFX_MSG_MAP
	ON_LBN_SETFOCUS(IDC_LIST_SUB, OnLbnSetfocusListSub)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_edit_entity message handlers

BOOL dlg_edit_entity::OnInitDialog() 
{
	CDialog::OnInitDialog();
	glo.p_dlg_edit_entity = this;	
  
	  p_edit = new CControlEntEdit();
	  cls_things.add_new_thing(p_edit);
	
	//let's move the window to where we want it
	 	//force our size
	if (glo.cls_swed.rect_tool.right == 0)
	this->SetWindowPos(&wndTop, 500,300,GetSystemMetrics(SM_CXSCREEN),
	GetSystemMetrics(SM_CYSCREEN), SWP_NOZORDER | SWP_NOSIZE); 
	else
	this->SetWindowPos(&wndTop, glo.cls_swed.rect_tool.left,glo.cls_swed.rect_tool.top,
	glo.cls_swed.rect_tool.right-glo.cls_swed.rect_tool.left,
	glo.cls_swed.rect_tool.bottom-glo.cls_swed.rect_tool.top, SWP_NOZORDER | SWP_NOSIZE); 

	//build modes
	build_modes();
	build_category_list();
	// TODO: Add extra initialization here
	set_mode(i_ent_last_mode);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

}

void dlg_edit_entity::set_mode(int i_new_mode)
{
	i_ent_last_mode = i_new_mode;
 	this->m_combo_mode.SetCurSel(i_ent_last_mode);
	p_edit->set_mode(i_new_mode);

	//set the callback we should use for choosing an item
	p_edit->set_edit_callback(this->EditCallback);
     if (i_new_mode == C_ENT_EDIT_MODE_MOVE)
    {
        log_msg("ENTITY MOVE - Click and drag an object to move it around.  While dragging, you can also do the following:");
        log_msg("Rotate item: Use 4 and 6 (numeric pad) to rotate left or right.");
        log_msg("Move up/down: Use 8 and 2 (numeric pad) to move entity up and down. To go back to 'follow ground' move hit G.");
        log_msg("Note:  Hold down shift to rotate/move fast.  You can use camera movement keys while dragging.");
    }

    if (i_new_mode == C_ENT_EDIT_MODE_DELETE)
    {
        log_msg("ENTITY DELETE - Click an item to complete delete it.");
    }

    if (i_new_mode == C_ENT_EDIT_MODE_EDIT)
    {
        log_msg("ENTITY DELETE - Click an item to bring up its properties. (only effects item clicked)");
    }


}
void dlg_edit_entity::build_modes()
{
	this->m_combo_mode.ResetContent();
	
	this->m_combo_mode.AddString("Create");
	this->m_combo_mode.AddString("Move");
	this->m_combo_mode.AddString("Delete");
	this->m_combo_mode.AddString("Edit");
}


void dlg_edit_entity::build_category_list()
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
   p_edit->set_category_id(ui_ent_last_cat);

   //since our category may have just changed, we better update the items list
   build_item_list();

}

void dlg_edit_entity::build_item_list()
{
 	this->m_list_ent.ResetContent();
	
	//build items
	if (::ui_ent_last_cat == 0)
	{
	  	p_edit->set_item_index(0);
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
	p_edit->set_item_index(i_ent_last_item);

}

void dlg_edit_entity::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	glo.p_dlg_edit_entity = NULL;
		WINDOWPLACEMENT win_temp;
	GetWindowPlacement(&win_temp);
	CopyRect(&glo.cls_swed.rect_tool, &win_temp.rcNormalPosition);
	cls_things.delete_things_by_type(::C_THING_EDIT);


	CDialog::OnClose();
}

void dlg_edit_entity::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;

}

BOOL dlg_edit_entity::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
		if (glo.p_dlg_main)
	if (glo.p_dlg_main->m_hAcceleratorTable != NULL)
		 if (::TranslateAccelerator(glo.p_dlg_main->m_hWnd, glo.p_dlg_main->m_hAcceleratorTable, pMsg))    
			 return TRUE;
		

	return CDialog::PreTranslateMessage(pMsg);
}

void dlg_edit_entity::OnNewCat() 
{
	// TODO: Add your control notification handler code here
  dlg_new_cat dlg;
  if (dlg.DoModal() == IDOK)
  {
	//true
    //rebuild cat list
	  build_category_list();
	  cls_tglo.p_ent_lib->save_category(::ui_ent_last_cat);
  
  } else
  {
    //they hit cancel
  }

}

void dlg_edit_entity::OnNewItem() 
{
	//let's add a new item/monster/npc here
	if (ui_ent_last_cat == 0)
	{
	 log_error("No category chosen, can't add entity.");
	 return;
	}

	i_ent_last_item = cls_tglo.p_ent_lib->add_item(ui_ent_last_cat, new CBaseEntData());
	build_item_list();
	OnDblclkListEnt(); //start editting it right now
}

void dlg_edit_entity::OnSelchangeListSub() 
{
  	//get class id of category
	ui_ent_last_cat = m_list_cat.GetItemData(m_list_cat.GetCurSel());
	p_edit->set_category_id(::ui_ent_last_cat);

   
	//rebuild item list
	build_item_list();
}


void dlg_edit_entity::OnSelchangeListEnt() 
{
	//switch currently selected item
   	::i_ent_last_item = m_list_ent.GetItemData(m_list_ent.GetCurSel());
	 p_edit->set_item_index(i_ent_last_item);

}

void dlg_edit_entity::OnDblclkListEnt() 
{
 //edit a base item
  dlg_ent_edit_base dlg;
  dlg.DoModal();
  //update item in case the name changed
  build_item_list();
  	
}

void dlg_edit_entity::OnSelchangeComboMode() 
{
	this->set_mode(this->m_combo_mode.GetCurSel());
}


void dlg_edit_entity::EditCallback()
{
  CEntItem *p_ent = (CEntItem*) glo.p_dlg_edit_entity->p_edit->get_last_thing_clicked();
 
  if (p_ent->get_type() != C_THING_ENT_GENERIC)
  {
    log_msg("Can't edit %s.", p_ent->get_thing_name());
	return;
  }

  //update item with any movement changes made since
  p_ent->update_ent_data();
 	dlg_edit_ent_default *dlg = new dlg_edit_ent_default();
  
   dlg->set_item_to_edit( p_ent->get_entity_pointer() );

  cls_tglo.p_cls_input->set_focus(false);
  log_msg("Making edit box");
   //set entity pointer we're editting
   dlg->Create(IDD_EDIT_ENT_DEFAULT, glo.p_dlg_edit_entity);
  dlg->set_delete_self_on_exit(true);
  dlg->set_thing_ent_to_update(p_ent); //so it will get visually redrawn
  //when they are done editting
}



LRESULT dlg_edit_entity::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

void dlg_edit_entity::OnLbnSetfocusListSub()
{
	log_msg("Got focus.");
	// TODO: Add your control notification handler code here
}

void dlg_edit_entity::OnEditByName() 
{
//edit a base item
  dlg_ent_choose dlg;
  dlg.DoModal();
 	
}
