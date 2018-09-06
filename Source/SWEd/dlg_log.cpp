// dlg_log.cpp : implementation file
//

#include "stdafx.h"
#include "SWEd.h"
#include "dlg_log.h"
#include "cglobals.h"
#include "..\\..\\..\\shared\\mfc_all.h"
#include "dls_swed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_log dialog


dlg_log::dlg_log(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_log::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_log)
	m_cst_log = _T("");
	//}}AFX_DATA_INIT
}


void dlg_log::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_log)
	DDX_Control(pDX, IDC_LOG, cedit_log);
	DDX_Text(pDX, IDC_LOG, m_cst_log);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_log, CDialog)
	//{{AFX_MSG_MAP(dlg_log)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_log message handlers

BOOL dlg_log::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    glo.p_dlg_log = this;	
	//force our size
	if (glo.cls_swed.rect_log.right == 0)
	this->SetWindowPos(&wndTop, 10,470,630, 230, SWP_NOZORDER); 
	else
	this->SetWindowPos(&wndTop, glo.cls_swed.rect_log.left,glo.cls_swed.rect_log.top,
	glo.cls_swed.rect_log.right-glo.cls_swed.rect_log.left,glo.cls_swed.rect_log.bottom-glo.cls_swed.rect_log.top, SWP_NOZORDER); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlg_log::OnSize(UINT nType, int cx, int cy) 
{
	
	if (cedit_log)
	{
		CRect rect_cur;
		CRect rect_box;
		cedit_log.GetWindowRect(&rect_cur);
		this->GetWindowRect(&rect_box);
		
		rect_cur.OffsetRect(-rect_box.left, -rect_box.top); //get current local X/Y of our control
		rect_box.OffsetRect(-rect_box.left, -rect_box.top); //get current local X/Y of our window
		
		cedit_log.SetWindowPos(&wndTop, 0,0,(rect_box.right - rect_cur.left)-5,(rect_box.bottom-rect_cur.top)-5, SWP_NOMOVE);
		
		
	//	Invalidate();
	}

  CDialog::OnSize(nType, cx, cy);
}



void dlg_log::add_log(CString cst_add)
{
  	cst_add += "\r\n";
	m_cst_log += cst_add;
 
  
 // OutputDebugString(cst_add+"\r\n");
  /*
  if (sys.b_log)
	{
	  //we're instructed to log info to disk
		add_text((char *)(LPCSTR)cst_add, "log.txt");
	}
   */
  int i_lines = 0;
  while (i_lines = cedit_log.GetLineCount() > d_max_log_lines)
  {
	 int i_line_size = cedit_log.LineLength(1);
	 m_cst_log.Delete(0, i_line_size+2); //plus 2 is to get the linefeed
	 this->UpdateData(d_to_window);
  } 

  this->UpdateData(d_to_window);
  cedit_log.LineScroll(1000000, 0);
  //Invalidate(false);
}

LRESULT dlg_log::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
		if (message == WM_ADD_TO_LOG)
	{
	  char *temp = (char *) lParam;
	  this->add_log(temp);
	  //free the memory that was created to pass this string
	  free(temp);
	}

/*		
	if ( (message == WM_EXITSIZEMOVE) || (message == WM_MOVE))
	{
	Invalidate(true);
	this->SendMessage(WM_NCPAINT, 1, NULL);
	}
  */
	
	return CDialog::WindowProc(message, wParam, lParam);

}


BOOL dlg_log::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	 	if (glo.p_dlg_main)
	if (glo.p_dlg_main->m_hAcceleratorTable != NULL)
		 if (::TranslateAccelerator(glo.p_dlg_main->m_hWnd, glo.p_dlg_main->m_hAcceleratorTable, pMsg))    
			 return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}
