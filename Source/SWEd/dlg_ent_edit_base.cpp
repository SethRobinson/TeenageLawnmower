// dlg_ent_edit_base.cpp : implementation file
//

#include "stdafx.h"
#include "swed.h"
#include "dlg_ent_edit_base.h"
#include "CGlobals.h"
#include "..\\..\\..\\shared\\3d\\CEntLib.h"
#include "..\\..\\..\\shared\\mfc_all.h" 
#include "dlg_edit_ent_default.h"
#include "..\\..\\..\\shared\\3d\\file_utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_ent_edit_base dialog

extern unsigned int ui_ent_last_cat;
extern int i_ent_last_item;

dlg_ent_edit_base::dlg_ent_edit_base(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_ent_edit_base::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_ent_edit_base)
	m_cst_name = _T("");
	m_cst_visual = _T("");
	m_b_follow_ground_tilt = FALSE;
	m_st_size_x = _T("");
	m_st_size_y = _T("");
	m_st_size_z = _T("");
	m_b_enable_lighting = FALSE;
	m_cst_script = _T("");
	//}}AFX_DATA_INIT
}


void dlg_ent_edit_base::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_ent_edit_base)
	DDX_Control(pDX, IDC_DRAW_MODE, m_combo_draw_mode);
	DDX_Text(pDX, IDC_NAME, m_cst_name);
	DDV_MaxChars(pDX, m_cst_name, 59);
	DDX_Text(pDX, IDC_VISUAL, m_cst_visual);
	DDX_Check(pDX, IDC_FOLLOW_GROUND_TILT, m_b_follow_ground_tilt);
	DDX_Text(pDX, IDC_SIZE_X, m_st_size_x);
	DDX_Text(pDX, IDC_SIZE_Y2, m_st_size_y);
	DDX_Text(pDX, IDC_SIZE_Z, m_st_size_z);
	DDX_Check(pDX, IDC_ENABLE_LIGHTING, m_b_enable_lighting);
	DDX_Text(pDX, IDC_SCRIPT, m_cst_script);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_ent_edit_base, CDialog)
	//{{AFX_MSG_MAP(dlg_ent_edit_base)
	ON_BN_CLICKED(IDC_DEFAULTS, OnDefaults)
	ON_BN_CLICKED(IDC_CHOOSE_GRAPHIC, OnChooseGraphic)
	ON_BN_CLICKED(IDC_ENABLE_LIGHTING, OnEnableLighting)
	ON_BN_CLICKED(IDC_CHOOSE_SCRIPT, OnChooseScript)
	ON_BN_CLICKED(IDC_EDIT_SCRIPT, OnEditScript)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_ent_edit_base message handlers



BOOL dlg_ent_edit_base::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	this->m_combo_draw_mode.ResetContent();
	m_combo_draw_mode.AddString("Normal");
	m_combo_draw_mode.AddString("Follow Y Axis");
	m_combo_draw_mode.AddString("Follow All");



	p_base = cls_tglo.p_ent_lib->get_base_item(::ui_ent_last_cat, ::i_ent_last_item);


	m_combo_draw_mode.SetCurSel(p_base->i_draw_mode);
	this->m_cst_name.Format("%s", p_base->st_name);
    this->m_cst_visual.Format("%s", p_base->st_visual);
    this->m_cst_script.Format("%s", p_base->st_script);
	
	
	this->m_b_follow_ground_tilt = p_base->b_follow_ground_mode;
	this->m_b_enable_lighting = p_base->b_enable_lighting;
	this->m_st_size_x.Format("%.3f", p_base->vec_scale.x);
	this->m_st_size_y.Format("%.3f", p_base->vec_scale.y);
	this->m_st_size_z.Format("%.3f", p_base->vec_scale.z);

	
	UpdateData(D_TO_WINDOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void dlg_ent_edit_base::OnOK() 
{
	//actually save the changes we made
	UpdateData(D_TO_VAR);

	strcpy(p_base->st_name, m_cst_name);
	strcpy(p_base->st_visual, m_cst_visual);
	strcpy(p_base->st_script, m_cst_script);
	p_base->i_draw_mode = this->m_combo_draw_mode.GetCurSel();
	p_base->b_follow_ground_mode = this->m_b_follow_ground_tilt != 0;
	p_base->b_enable_lighting = this->m_b_enable_lighting != 0;
	p_base->vec_scale.x = atof(m_st_size_x);
	p_base->vec_scale.y = atof(m_st_size_y);
	p_base->vec_scale.z = atof(m_st_size_z);

	//save to disk
    cls_tglo.p_ent_lib->save_category(::ui_ent_last_cat);
  
	
	//also let's update any items of this type current in use
	reinit_objects_on_map(&cls_things, ui_ent_last_cat);

	CDialog::OnOK();
}

void dlg_ent_edit_base::OnDefaults() 
{
  dlg_edit_ent_default dlg;

  //set entity pointer we're editting
  dlg.set_item_to_edit(p_base->p_ent_default);
  
  dlg.DoModal();
  //all done


}

void dlg_ent_edit_base::OnChooseGraphic() 
{
	UpdateData(D_TO_VAR);
	//choose a file to be the graphic
    //load a new map	
 	 CFileDialog cfd(true, //true if open, false if close
		 ".zon", //default file extension
		 "", OFN_HIDEREADONLY|OFN_NOCHANGEDIR ,
		 "Image or model (*.*)|*.*|All files (*.*)|*.*|",//file filter
		 AfxGetApp()->m_pMainWnd);
     char st_temp[MAX_PATH];
     strcpy((char*)&st_temp, GetExeDir());
     if (cls_tglo.mod.m_st_path[0])
     {
      strcat(st_temp, cls_tglo.mod.m_st_path);
     }
     
     //	 strcat(st_temp, "\\data");
	 cfd.m_ofn.lpstrInitialDir = st_temp;
//		 log_msg("Going to do %s.",st_temp);
	
		 
	 if(cfd.DoModal() == IDOK)
		 {
	   //remove the initial dir from the path we chose, it's not needed
		char st_new[256];
		strcpy(st_new, cfd.GetPathName());

	    
        if (strnicmp(st_new, st_temp, strlen(st_temp)) != 0)
		{
		
            if (!cls_tglo.mod.m_st_path[0] || (strnicmp(st_new, GetExeDir(), strlen(GetExeDir())) != 0))
            {
                
                
                log_msg("Error, file must be in the main engine dir or a sub dir of it.");
                return;
            } else
            {
                strcpy(st_temp, GetExeDir()); //allow stuff from the base dir to be used
               st_temp[strlen(st_temp)-1] = 0;
            }
		}
		 
		   
           //remove path info
		    m_cst_visual.Format("%s",  st_new);
		    m_cst_visual.Delete(0, strlen(st_temp)); //the plus one grabs the extra \
			
			UpdateData(D_TO_WINDOW);
		 } else
		 {
		   log_msg("Did not load.");
		 }

	
}

void dlg_ent_edit_base::OnEnableLighting() 
{
	// TODO: Add your control notification handler code here
	
}

void dlg_ent_edit_base::OnChooseScript() 
{
	UpdateData(D_TO_VAR);
	//choose a file to be the graphic
    //load a new map	
 	 CFileDialog cfd(true, //true if open, false if close
		 ".c", //default file extension
		 "", OFN_HIDEREADONLY|OFN_NOCHANGEDIR ,
		 "Script (*.c)|*.c|All files (*.*)|*.*|",//file filter
		 AfxGetApp()->m_pMainWnd);
     char st_temp[MAX_PATH];
     strcpy((char*)&st_temp, GetExeDir());
     if (cls_tglo.mod.m_st_path[0])
     {
      strcat(st_temp, cls_tglo.mod.m_st_path);
     }
  	 cfd.m_ofn.lpstrInitialDir = st_temp;
//		 log_msg("Going to do %s.",st_temp);
	
		 
	 if(cfd.DoModal() == IDOK)
		 {
	   //remove the initial dir from the path we chose, it's not needed
		char st_new[256];
		strcpy(st_new, cfd.GetPathName());

 if (strnicmp(st_new, st_temp, strlen(st_temp)) != 0)
		{
		
            if (!cls_tglo.mod.m_st_path[0] || (strnicmp(st_new, GetExeDir(), strlen(GetExeDir())) != 0))
            {
                
                
                log_msg("Error, file must be in the main engine dir or a sub dir of it.");
                return;
            } else
            {
                strcpy(st_temp, GetExeDir()); //allow stuff from the base dir to be used
               st_temp[strlen(st_temp)-1] = 0;
            }
		}
		 
		//remove path info
		    m_cst_script.Format("%s",  st_new);
		    m_cst_script.Delete(0, strlen(st_temp)); //the plus one grabs the extra \
			
			UpdateData(D_TO_WINDOW);
		 } else
		 {
		   log_msg("Did not load.");
		 }

	
}
	

void dlg_ent_edit_base::OnEditScript() 
{
    
    //make sure we have the latest filename entered
    UpdateData(D_TO_VAR);
    if (m_cst_script.GetLength() < 3)
    {
      return; 
    }
        
        
    if (exist((char*)(LPCSTR)m_cst_script))
    {
        open_file(this->m_hWnd, (char*)(LPCSTR)m_cst_script);
        return;
    } 
    
    //if a mod, check in the main dir too for this scripts existance
    if (cls_tglo.mod.m_st_path[0])
    {
        
        char st_temp[256];
        sprintf(st_temp, "%s%s", GetExeDir(),(LPCSTR)m_cst_script);
        if (exist(st_temp))
        {
            open_file(this->m_hWnd, st_temp);
            return;
        }
    }
    
    
     //go ahead and make the file for them
    log_msg("File %s did not exist in the MOD or main dir, creating it in unicode format.",
       (LPCSTR) m_cst_script);

    add_text("ÿþ", (char*)(LPCSTR) m_cst_script);
    
    open_file(this->m_hWnd, (char*)(LPCSTR)m_cst_script);
  
}


