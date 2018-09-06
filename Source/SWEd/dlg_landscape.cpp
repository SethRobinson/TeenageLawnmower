// dlg_landscape.cpp : implementation file
//

#include "stdafx.h"
#include "SWEd.h"
#include "dlg_landscape.h"
#include "CGlobals.h"
#include "..\\..\\..\\shared\\mfc_all.h"
#include "dls_swed.h"
#include "..\\..\\..\\shared\\3d\\CSubManager.h"
#include "..\\..\\..\\shared\\3d\\CMap.h"
#include "..\\..\\..\\shared\\3d\\CBuilding.h"
#include "..\\..\\..\\shared\\3d\\CBuildingManager.h"
#include "..\\..\\..\\shared\\3d\\CMessageHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int g_i_landscape_last_op = IDC_OP_ADD; //store radio button position
float g_f_landscape_tool_setting = 0.1f;
/////////////////////////////////////////////////////////////////////////////
// dlg_landscape dialog


dlg_landscape::dlg_landscape(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_landscape::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_landscape)
	m_st_mod_amount = _T("");
	//}}AFX_DATA_INIT
}


void dlg_landscape::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_landscape)
	DDX_Text(pDX, IDC_MOD_AMOUNT, m_st_mod_amount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_landscape, CDialog)
	//{{AFX_MSG_MAP(dlg_landscape)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_MOD_AMOUNT, OnChangeModAmount)
	ON_BN_CLICKED(IDC_UPDATE_ALL, OnUpdateAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_landscape message handlers

BOOL dlg_landscape::OnInitDialog() 
{
	CDialog::OnInitDialog();
	 glo.p_dlg_landscape = this;	


	  p_edit = new CControlEdit();
	  cls_things.add_new_thing(p_edit);
  	  p_edit->set_brush_type(C_BRUSH_TYPE_LANDSCAPE);

	  
	  CheckRadioButton(IDC_OP_ADD, IDC_OP_SMOOTH, g_i_landscape_last_op);
	  process_button(g_i_landscape_last_op);

	  	  //let's move the window to where we want it
	 	//force our size
	if (glo.cls_swed.rect_tool.right == 0)
	this->SetWindowPos(&wndTop, 500,300,GetSystemMetrics(SM_CXSCREEN),
	GetSystemMetrics(SM_CYSCREEN), SWP_NOZORDER | SWP_NOSIZE); 
	else
	this->SetWindowPos(&wndTop, glo.cls_swed.rect_tool.left,glo.cls_swed.rect_tool.top,
	glo.cls_swed.rect_tool.right-glo.cls_swed.rect_tool.left,
	glo.cls_swed.rect_tool.bottom-glo.cls_swed.rect_tool.top, SWP_NOZORDER | SWP_NOSIZE); 
	//Format() can't handle floats right so let's do it this way..
	char st_crap[255]; 
	sprintf(st_crap, "%f", g_f_landscape_tool_setting);
	m_st_mod_amount.Format("%s",st_crap);
	p_edit->set_tool_setting(g_f_landscape_tool_setting);

	UpdateData(D_TO_WINDOW);
	  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlg_landscape::PostNcDestroy() 
{
 delete this;

}

void dlg_landscape::OnClose() 
{
	glo.p_dlg_landscape = NULL;
	//let's also kill our edit object that's inside the engine right now
	cls_things.delete_things_by_type(::C_THING_EDIT);
	//save the window placement, well, only the X an Y will be used
	WINDOWPLACEMENT win_temp;
	GetWindowPlacement(&win_temp);
	CopyRect(&glo.cls_swed.rect_tool, &win_temp.rcNormalPosition);


	CDialog::OnClose();
}

void dlg_landscape::process_button(int nID)
{
		 g_i_landscape_last_op = nID; //remember this for later

		 if (nID == IDC_OP_ADD)
		 {
		   p_edit->set_current_brush(::C_BRUSH_MOD);
		 }
		 if (nID == IDC_OP_SUBTRACT)
		 {
		   p_edit->set_current_brush(::C_BRUSH_SET);
		 }
		 if (nID == IDC_OP_SMOOTH)
		 {
		   p_edit->set_current_brush(::C_BRUSH_SMOOTH);
		 }
}

BOOL dlg_landscape::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	 if (nCode == BN_CLICKED)
	 {
		 if (nID != 2) // what's with 2?? should only give button ID's, right?
		 process_button(nID);
	 }
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void dlg_landscape::OnChangeModAmount() 
{
	UpdateData(D_TO_VAR);
	
	g_f_landscape_tool_setting = atof(m_st_mod_amount);
	p_edit->set_tool_setting(g_f_landscape_tool_setting);
}

BOOL dlg_landscape::PreTranslateMessage(MSG* pMsg) 
{
		// Check me out, I'm pretty sure I'm breaking every MFC rule there is!
	   	 	if (glo.p_dlg_main)
	if (glo.p_dlg_main->m_hAcceleratorTable != NULL)
		 if (::TranslateAccelerator(glo.p_dlg_main->m_hWnd, glo.p_dlg_main->m_hAcceleratorTable, pMsg))    
			 return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

void dlg_landscape::OnUpdateAll() 
{

           SendSimpleWindowsEngineMessage(C_MESSAGE_COMMAND_REBUILD_ALL_WALLS, 0);
	
}
