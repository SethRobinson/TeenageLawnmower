// dlg_world_edit.cpp : implementation file
//

#include "stdafx.h"
#include "SWEd.h"
#include "dlg_world_edit.h"
#include "CGlobals.h"
#include "..\\..\\..\\shared\\3d\\CManager.h"
#include "dls_swed.h"
#include "..\\..\\..\\shared\\mfc_all.h"
#include "..\\..\\..\\shared\\3d\\CMapBlock.h"

int g_last_texture_selected = -1;
int g_i_last_tex_id_selected = 1;
bool g_world_paste_full = true;
bool g_b_overlay = false;
int i_world_edit_mode = 0; //normal


bool g_world_flip_x = false;
bool g_world_flip_z = false;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_world_edit dialog


dlg_world_edit::dlg_world_edit(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_world_edit::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_world_edit)
	m_paste_full = FALSE;
	m_b_flip_x = FALSE;
	m_b_flip_z = FALSE;
	m_b_overlay = FALSE;
	//}}AFX_DATA_INIT
}


void dlg_world_edit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_world_edit)
	DDX_Control(pDX, IDC_LIST_MODE, m_list_mode);
	DDX_Control(pDX, IDC_TEX_LIST, m_list_tex);
	DDX_Check(pDX, IDC_PASTE_FULL, m_paste_full);
	DDX_Check(pDX, IDC_FLIPX, m_b_flip_x);
	DDX_Check(pDX, IDC_FLIPZ, m_b_flip_z);
	DDX_Check(pDX, IDC_OVERLAY, m_b_overlay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_world_edit, CDialog)
	//{{AFX_MSG_MAP(dlg_world_edit)
	ON_WM_CLOSE()
	ON_LBN_SELCHANGE(IDC_TEX_LIST, OnSelchangeTexList)
	ON_BN_CLICKED(IDC_PASTE_FULL, OnPasteFull)
	ON_BN_CLICKED(IDC_FLIPX, OnFlipx)
	ON_BN_CLICKED(IDC_FLIPZ, OnFlipz)
	ON_CBN_SELCHANGE(IDC_LIST_MODE, OnSelchangeListMode)
	ON_BN_CLICKED(IDC_OVERLAY, OnOverlay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_world_edit message handlers

BOOL dlg_world_edit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	  glo.p_dlg_world_edit = this;	

	  //make combo box
	 this->m_list_mode.AddString("Landscape");
	 this->m_list_mode.AddString("Buildings");

     m_list_mode.SetCurSel(i_world_edit_mode);

	  
	  
	  //add stuff to the listbox
	  char st_name[256];
	  int i_id;
	  
	  int i_index = 0;

	  p_edit = new CControlEdit();
	  cls_things.add_new_thing(p_edit);
	  p_edit->set_brush_type(C_BRUSH_TYPE_PAINT);
	  p_edit->set_current_brush(i_world_edit_mode);

      if (cls_tglo.p_manager)
      {
      
	  while (cls_tglo.p_manager->scan_resources(::C_RESOURCE_TEXTURE,C_RESOURCE_SUB_LANDSCAPE, (char*)&st_name, &i_id))
	  {
	    //add this resource to  the list
	  int i_index = this->m_list_tex.AddString(st_name);
	  this->m_list_tex.SetItemData(i_index, i_id);

	  }
	  	  UpdateData(D_TO_WINDOW);

	  if (g_last_texture_selected == -1)
	  {
	    g_last_texture_selected = 0;

	  }

	  //highlight the default
	  m_list_tex.SetCurSel(g_last_texture_selected);
	  OnSelchangeTexList(); //actually send it to the edit window
	  
	  //let's also add our world edit controller to the object stack
	  this->m_paste_full = g_world_paste_full;
	  this->m_b_flip_x = g_world_flip_x;
	  this->m_b_flip_z = g_world_flip_z;
	  this->m_b_overlay = g_b_overlay;

	  
	  
	  UpdateData(D_TO_WINDOW);
	send_settings_to_pedit();

	  //let's move the window to where we want it
	 	//force our size
	if (glo.cls_swed.rect_tool.right == 0)
	this->SetWindowPos(&wndTop, 500,300,GetSystemMetrics(SM_CXSCREEN),
	GetSystemMetrics(SM_CYSCREEN), SWP_NOZORDER | SWP_NOSIZE); 
	else
	this->SetWindowPos(&wndTop, glo.cls_swed.rect_tool.left,glo.cls_swed.rect_tool.top,
	glo.cls_swed.rect_tool.right-glo.cls_swed.rect_tool.left,
	glo.cls_swed.rect_tool.bottom-glo.cls_swed.rect_tool.top, SWP_NOZORDER | SWP_NOSIZE); 
      }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlg_world_edit::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	glo.p_dlg_world_edit = NULL;
	//let's also kill our edit object that's inside the engine right now
	cls_things.delete_things_by_type(::C_THING_EDIT);

	//save the window placement, well, only the X an Y will be used
	WINDOWPLACEMENT win_temp;
	GetWindowPlacement(&win_temp);
	CopyRect(&glo.cls_swed.rect_tool, &win_temp.rcNormalPosition);

	CDialog::OnClose();
}

LRESULT dlg_world_edit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	/*
	if ( (message == WM_EXITSIZEMOVE) || (message == WM_MOVE))
	{
	Invalidate(true);
	this->SendMessage(WM_NCPAINT, 1, NULL);
	}
	  */
	return CDialog::WindowProc(message, wParam, lParam);
}

void dlg_world_edit::PostNcDestroy() 
{
		 delete this;
}

void dlg_world_edit::OnSelchangeTexList() 
{
   //send our selection change to the edit object
	int i_sel = this->m_list_tex.GetCurSel();
    g_last_texture_selected = i_sel;	//remember this for later

	if (p_edit)
	{
	  p_edit->set_current_brush(m_list_tex.GetItemData(i_sel));
	}

	g_i_last_tex_id_selected = m_list_tex.GetItemData(i_sel);
}

BOOL dlg_world_edit::PreTranslateMessage(MSG* pMsg) 
{
		// Check me out, I'm pretty sure I'm breaking every MFC rule there is!
	   	 	if (glo.p_dlg_main)
	if (glo.p_dlg_main->m_hAcceleratorTable != NULL)
		 if (::TranslateAccelerator(glo.p_dlg_main->m_hWnd, glo.p_dlg_main->m_hAcceleratorTable, pMsg))    
			 return TRUE;


	return CDialog::PreTranslateMessage(pMsg);
}

void dlg_world_edit::send_settings_to_pedit()
{
	p_edit->set_paste_full(g_world_paste_full);
    p_edit->set_brush_sub_mode(i_world_edit_mode);
    p_edit->set_current_brush(m_list_tex.GetItemData(g_last_texture_selected));
    p_edit->set_overlay_mode(g_b_overlay); //operate on the layer above

	if (::g_world_flip_x && g_world_flip_z)
	{
		p_edit->set_texture_mode(::C_MB_TEXTURE_MODE_FLIP_X_AND_Z);
	} else
	if ((!g_world_flip_x) && (!g_world_flip_z))
	{
  		p_edit->set_texture_mode(::C_MB_TEXTURE_MODE_NONE);

	} else if (g_world_flip_x)
	{
  		p_edit->set_texture_mode(::C_MB_TEXTURE_MODE_FLIP_X);

	} else if (g_world_flip_z)
	{
  		p_edit->set_texture_mode(::C_MB_TEXTURE_MODE_FLIP_Z);
	}


}
void dlg_world_edit::OnPasteFull() 
{
	UpdateData(D_TO_VAR);
	::g_world_paste_full = this->m_paste_full != 0;
		 send_settings_to_pedit();
	//get info and send it to the edit thing
}

void dlg_world_edit::OnFlipx() 
{
	// TODO: Add your control notification handler code here
	UpdateData(D_TO_VAR);
	::g_world_flip_x = this->m_b_flip_x != 0;
	 send_settings_to_pedit();
	//p_edit->set_paste_texture_mode(::C_MB_TEXTURE_MODE_FLIP_X);
	
}

void dlg_world_edit::OnFlipz() 
{
	UpdateData(D_TO_VAR);
	::g_world_flip_z = this->m_b_flip_z != 0;
	// TODO: Add your control notification handler code here
	 send_settings_to_pedit();
	
}

void dlg_world_edit::OnSelchangeListMode() 
{
	i_world_edit_mode = m_list_mode.GetCurSel(); 
	send_settings_to_pedit();
	
}

void dlg_world_edit::OnOverlay() 
{
	// TODO: Add your control notification handler code here
	UpdateData(D_TO_VAR);
	g_b_overlay = this->m_b_overlay != 0;
	// TODO: Add your control notification handler code here
	 send_settings_to_pedit();
	
	
}
