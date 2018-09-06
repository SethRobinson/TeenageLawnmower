// dlg_sub_prop.cpp : implementation file
//

#include "stdafx.h"
#include "swed.h"
#include "dlg_sub_prop.h"
#include "..\\..\\..\\shared\\mfc_all.h"	
#include "CGlobals.h"
#include "..\\..\\..\\shared\\3d\\CManager.h"
#include "..\\..\\..\\shared\\3d\\CMap.h"

#include "..\\..\\..\\shared\\3d\\CSubManager.h"
#include "CGlobals.h"
#include "dlg_edit_sub.h"
#include "..\\..\\..\\shared\\3d\\CBuildingManager.h"
#include "..\\..\\..\\shared\\3d\\CBuilding.h"
#include "..\\..\\..\\shared\\3d\\CMessageHeader.h"

#include "dlg_3d.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_sub_prop dialog

int i_last_selection = -1; //keep track of the last sub zone we worked on

dlg_sub_prop::dlg_sub_prop(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_sub_prop::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_sub_prop)
	m_st_name = _T("");
	m_st_ceiling_height = _T("");
	m_st_wall_tex_x = _T("");
	m_list_wall_tex_y = _T("");
	m_st_roof_tex_x = _T("");
	m_st_roof_tex_y = _T("");
	//}}AFX_DATA_INIT
}


void dlg_sub_prop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_sub_prop)
	DDX_Control(pDX, IDC_LIST_ROOF_TEX, m_list_roof_tex);
	DDX_Control(pDX, IDC_LIST_CEALING_DRAW, m_list_cealing_draw);
	DDX_Control(pDX, IDC_LIST_WALL_TEX, m_list_wall_tex);
	DDX_Control(pDX, IDC_LIST_STYLE, m_list_ceil_style);
	DDX_Control(pDX, IDC_LIST_SUB, m_list_sub);
	DDX_Text(pDX, IDC_SUB_NAME, m_st_name);
	DDX_Text(pDX, IDC_CEALING_HEIGHT, m_st_ceiling_height);
	DDX_Text(pDX, IDC_WALL_TEX_X, m_st_wall_tex_x);
	DDX_Text(pDX, IDC_WALL_TEX_Y, m_list_wall_tex_y);
	DDX_Text(pDX, IDC_ROOF_TEX_X, m_st_roof_tex_x);
	DDX_Text(pDX, IDC_ROOF_TEX_Y, m_st_roof_tex_y);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_sub_prop, CDialog)
	//{{AFX_MSG_MAP(dlg_sub_prop)
	ON_BN_CLICKED(IDC_SUB_NEW, OnSubNew)
	ON_CBN_SELCHANGE(IDC_LIST_SUB, OnSelchangeListSub)
	ON_EN_CHANGE(IDC_SUB_NAME, OnChangeSubName)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_sub_prop message handlers

BOOL dlg_sub_prop::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//setup ceiling type combo box

	m_list_ceil_style.AddString("None");
	m_list_ceil_style.AddString("Fixed");
	m_list_ceil_style.AddString("Relative");

	
	//setup the wall texture styles
	m_list_wall_tex.AddString("World Coordinates");
	m_list_wall_tex.AddString("Local Coordinates");

	//setup the roof texture styles
	m_list_roof_tex.AddString("World Coordinates");
	m_list_roof_tex.AddString("Local Coordinates");


	//cealing draw 
	m_list_cealing_draw.AddString("Normal");
	m_list_cealing_draw.AddString("Walls Tops Only");
	m_list_cealing_draw.AddString("No ceiling at all");
	m_list_cealing_draw.AddString("Draw Nothing");

	//set defaults
	load_sub_buttons();
	if (i_last_selection != -1) 
	{
	  if (i_last_selection < cls_tglo.p_map->p_sub_manager->get_count())
	  {
		set_selection(i_last_selection);
	  }

	} else
	{
	  if (cls_tglo.p_map->p_sub_manager->get_count() > 0) this->set_selection(0);
	}
	UpdateData(D_TO_WINDOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




void dlg_sub_prop::load_sub_buttons()
{
  this->m_list_sub.ResetContent();

  int i_sub_zones = cls_tglo.p_map->p_sub_manager->get_count();

  CString cst_temp;
  for (int i = 0; i < i_sub_zones; i++)
  {
  	cst_temp.Format("%s (id %d)", cls_tglo.p_map->p_sub_manager->p_a_sub_zone[i].st_name, i);
	int i_index = m_list_sub.AddString(cst_temp);
    m_list_sub.SetItemData(i_index, i); //remember our array pos so we can use sorting without
	//getting confused
  }

}

void dlg_sub_prop::OnSubNew() 
{
   //add new record
	int i_new_zone = cls_tglo.p_map->p_sub_manager->add_sub_zone();
	
	//set defaults
	CSubZone *p_new = &cls_tglo.p_map->p_sub_manager->p_a_sub_zone[i_new_zone];

	memset(p_new, 0, sizeof(CSubZone)); //clear it

	strcpy(p_new->st_name, "Unnamed");
	
	p_new->i_ceil_type = ::C_SUBZONE_CEIL_RELATIVE;
	p_new->f_wall_height = 5;
	p_new->f_wall_tex_x = 0.20f;
	p_new->f_wall_tex_y = 0.20f;
	p_new->i_wall_tex_style = 0; //world mapping

	
	p_new->f_roof_tex_x = 0.20f;
	p_new->f_roof_tex_y = 0.20f;
	p_new->i_roof_tex_style = 0; //world mapping


	save_current_selection(); //save what they are working on

	load_sub_buttons();
	set_selection(i_new_zone);
	
	//also update our toolbar if applicable
	if (glo.p_dlg_edit_sub)
	{
	  glo.p_dlg_edit_sub->build_combo_boxes();

	}
}


void dlg_sub_prop::save_current_selection()
{
  	if (i_last_selection == -1) return; //don't save data
	UpdateData(D_TO_VAR);

	CSubZone *p_sub = &cls_tglo.p_map->p_sub_manager->p_a_sub_zone[m_list_sub.GetItemData(i_last_selection)];

	if (m_st_name.GetLength() > 49)
	{
	  log_error("Error, name can only be 49 characters or smaller.");
	} else
	strcpy(p_sub->st_name, this->m_st_name);

	p_sub->f_wall_height = atof(this->m_st_ceiling_height);
	p_sub->i_ceil_type = this->m_list_ceil_style.GetCurSel();
	p_sub->i_wall_tex_style = m_list_wall_tex.GetCurSel();
	p_sub->f_wall_tex_x = atof(this->m_st_wall_tex_x);
	p_sub->f_wall_tex_y = atof(this->m_list_wall_tex_y);

	p_sub->f_roof_tex_x = atof(this->m_st_roof_tex_x);
	p_sub->f_roof_tex_y = atof(this->m_st_roof_tex_y);
	p_sub->i_roof_tex_style = m_list_roof_tex.GetCurSel();

	
	p_sub->i_cealing_draw = this->m_list_cealing_draw.GetCurSel();

    //go ahead and tell the engine to do it


    CEngineMessage *p_new = new CEngineMessage();
    p_new->m_i_message_type = C_THING_MSG_GENERIC;
    CMessageGeneric *p_mg = new CMessageGeneric();
    p_new->m_p_message = p_mg;
    p_new->m_b_system_message = true;
    p_mg->i_message_command = C_MESSAGE_COMMAND_REBUILD_WALLS;
    p_mg->i_building = m_list_sub.GetItemData(i_last_selection);
    SendWindowsEngineMessage(p_new);



}


void dlg_sub_prop::set_selection(int i_sel)
{
   	i_last_selection = i_sel;
	this->m_list_sub.SetCurSel(i_sel);
	//set properties
	CSubZone *p_sub = &cls_tglo.p_map->p_sub_manager->p_a_sub_zone[m_list_sub.GetItemData(i_sel)];


	this->m_st_name.Format("%s", p_sub->st_name);
	this->m_st_ceiling_height.Format("%f", p_sub->f_wall_height);
	this->m_list_ceil_style.SetCurSel(p_sub->i_ceil_type);
	
	this->m_st_wall_tex_x.Format("%f", p_sub->f_wall_tex_x);
	this->m_list_wall_tex_y.Format("%f", p_sub->f_wall_tex_y);
	this->m_list_wall_tex.SetCurSel(p_sub->i_wall_tex_style);
	this->m_list_cealing_draw.SetCurSel(p_sub->i_cealing_draw);

	this->m_st_roof_tex_x.Format("%f", p_sub->f_roof_tex_x);
	this->m_st_roof_tex_y.Format("%f", p_sub->f_roof_tex_y);
	this->m_list_roof_tex.SetCurSel(p_sub->i_roof_tex_style);


	UpdateData(D_TO_WINDOW);
}

void dlg_sub_prop::OnSelchangeListSub() 
{
	// TODO: Add your control notification handler code here
	int i_new_sel = m_list_sub.GetCurSel();
	if (i_last_selection != -1) save_current_selection();
	set_selection(i_new_sel);
}

BOOL dlg_sub_prop::PreTranslateMessage(MSG* pMsg) 
{
	
	return CDialog::PreTranslateMessage(pMsg);
}

void dlg_sub_prop::OnOK() 
{
	//save any changes made
	if (i_last_selection != -1) save_current_selection();


	CDialog::OnOK();
}




void dlg_sub_prop::OnChangeSubName() 
{
	if (i_last_selection != -1)
	{
	UpdateData(D_TO_VAR);
	int i_data_temp = m_list_sub.GetItemData(i_last_selection);
	CSubZone *p_sub = &cls_tglo.p_map->p_sub_manager->p_a_sub_zone[m_list_sub.GetItemData(i_last_selection)];
	if (m_st_name.GetLength() > 49)
	{
	  log_error("Error, name can only be 49 characters or smaller.");
	} else
	strcpy(p_sub->st_name, this->m_st_name);

		//rebuild names
	load_sub_buttons();
	i_last_selection = set_selection_by_data(&m_list_sub, i_data_temp);
	}


}

void dlg_sub_prop::OnApply() 
{
	save_current_selection();
	//chaneg focus
//	glo.p_dlg_3d->SetFocus();
}
