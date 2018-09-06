// dlg_edit_sub.cpp : implementation file
//

#include "stdafx.h"
#include "swed.h"
#include "dlg_edit_sub.h"
#include "dls_swed.h"
#include "CGLobals.h"
#include "..\\..\\..\\shared\\3d\\CThingSubEdit.h"
#include "..\\..\\..\\shared\\3d\\CGridTile.h"
#include "..\\..\\..\\shared\\3d\\CSubManager.h"
#include "..\\..\\..\\shared\\MFC_ALL.h"
#include "..\\..\\..\\shared\\3d\\CMessageHEader.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_edit_sub dialog

extern int g_i_last_tex_id_selected;
dlg_edit_sub::dlg_edit_sub(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_edit_sub::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_edit_sub)
	m_b_block = FALSE;
	//}}AFX_DATA_INIT
}


void dlg_edit_sub::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_edit_sub)
	DDX_Control(pDX, IDC_LIST_SUB, m_list_sub);
	DDX_Control(pDX, IDC_LIST_MODE, m_list_mode);
	DDX_Check(pDX, IDC_BLOCK, m_b_block);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_edit_sub, CDialog)
	//{{AFX_MSG_MAP(dlg_edit_sub)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_LIST_MODE, OnSelchangeListMode)
	ON_CBN_SELCHANGE(IDC_LIST_SUB, OnSelchangeListSub)
	ON_BN_CLICKED(IDC_BLOCK, OnBlock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_edit_sub message handlers



int i_edit_sub_paint_mode = C_EDIT_SUB_HARDNESS;

bool b_edit_sub_block_mode = false; //default


extern i_last_selection;//keep track of the last sub zone we worked on, SHARED with dlg_sub_prop

BOOL dlg_edit_sub::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	 glo.p_dlg_edit_sub = this;	

	  p_edit = new CThingSubEdit();
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

	
     SendSimpleWindowsEngineMessage(C_MESSAGE_COMMAND_FORCE_GRID_TILES_ON, 0);
  
    
    
   

	p_edit->set_block_mode(b_edit_sub_block_mode);
	this->m_b_block =  b_edit_sub_block_mode;
	p_edit->set_default_tex(g_i_last_tex_id_selected);
	build_combo_boxes();
	UpdateData(D_TO_WINDOW);
	return TRUE;  // return TRUE unless you set the focus to a control

}

void dlg_edit_sub::build_combo_boxes()
{
  m_list_mode.ResetContent();
  m_list_mode.AddString("Hardness"); //0
  m_list_mode.AddString("Sub Zone"); //1
  m_list_mode.AddString("Wall"); //2


  
  m_list_sub.ResetContent();
  //build sub zones
   int i_sub_zones = cls_tglo.p_map->p_sub_manager->get_count();

 CString cst_temp;
  for (int i = 0; i < i_sub_zones; i++)
  {
  	cst_temp.Format("%s (id %d)", cls_tglo.p_map->p_sub_manager->p_a_sub_zone[i].st_name, i);
	int i_index = m_list_sub.AddString(cst_temp);
    m_list_sub.SetItemData(i_index, i); //remember our array pos so we can use sorting without
	//getting confused
  }

  set_paint_mode(::i_edit_sub_paint_mode);

  set_region(i_last_selection);
}

void dlg_edit_sub::set_paint_mode(int i_new)
{
	i_edit_sub_paint_mode = i_new;
	m_list_mode.SetCurSel(i_new);
  
	//gret out set region box if in hardness edit mode
	if (i_edit_sub_paint_mode == C_EDIT_SUB_SUB_ZONE)
	{
	  m_list_sub.EnableWindow(true);
	} else
	{
	  m_list_sub.EnableWindow(false);
	}

  //set our edit controller to this paint mode
  this->p_edit->set_edit_mode(i_edit_sub_paint_mode);

}

void dlg_edit_sub::set_region(int i_new)
{
   	i_last_selection = i_new;
	if (i_new != -1)
	m_list_sub.SetCurSel(i_new);

   this->p_edit->set_sub_zone(m_list_sub.GetItemData(i_last_selection));

}


BOOL dlg_edit_sub::PreTranslateMessage(MSG* pMsg) 
{
	if (glo.p_dlg_main)
	if (glo.p_dlg_main->m_hAcceleratorTable != NULL)
		 if (::TranslateAccelerator(glo.p_dlg_main->m_hWnd, glo.p_dlg_main->m_hAcceleratorTable, pMsg))    
			 return TRUE;

	
	return CDialog::PreTranslateMessage(pMsg);
}

void dlg_edit_sub::PostNcDestroy() 
{
delete this;

}

void dlg_edit_sub::OnClose() 
{
	glo.p_dlg_edit_sub = NULL;
	//let's also kill our edit object that's inside the engine right now
	cls_things.delete_things_by_type(::C_THING_EDIT);
	//save the window placement, well, only the X an Y will be used
	WINDOWPLACEMENT win_temp;
	GetWindowPlacement(&win_temp);
	CopyRect(&glo.cls_swed.rect_tool, &win_temp.rcNormalPosition);

	//kill the tile grid too
	cls_things.delete_things_by_type(C_THING_TILE);


	
	CDialog::OnClose();
}

void dlg_edit_sub::OnSelchangeListMode() 
{
	// TODO: Add your control notification handler code here
  this->set_paint_mode(m_list_mode.GetCurSel());
}

void dlg_edit_sub::OnSelchangeListSub() 
{
  this->set_region(m_list_sub.GetCurSel());

}

void dlg_edit_sub::OnBlock() 
{
	// TODO: Add your control notification handler code here
	UpdateData(D_TO_VAR);
	b_edit_sub_block_mode = this->m_b_block != 0;
	p_edit->set_block_mode(b_edit_sub_block_mode);

	
}
