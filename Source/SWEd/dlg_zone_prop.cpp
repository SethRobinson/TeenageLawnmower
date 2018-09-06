// dlg_zone_prop.cpp : implementation file
//

#include "stdafx.h"
#include "SWEd.h"
#include "dlg_zone_prop.h"
#include "..\\..\\..\\shared\\3d\\CThing_Globals.h"
#include "..\\..\\..\\shared\\3d\\CMap.h"
#include "..\\..\\..\\shared\\mfc_all.h"
#include "..\\..\\..\\shared\\3d\\d3dapp.h"
#include "..\\..\\..\\shared\\3d\\CMessageHeader.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_zone_prop dialog


dlg_zone_prop::dlg_zone_prop(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_zone_prop::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_zone_prop)
	m_st_gravity = _T("");
	m_st_name = _T("");
	m_cst_tile_size = _T("");
	m_cst_tiles_per_block = _T("");
	m_cst_height_scale_mod = _T("");
	//}}AFX_DATA_INIT
}


void dlg_zone_prop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_zone_prop)
	DDX_Text(pDX, IDC_GRAVITY, m_st_gravity);
	DDX_Text(pDX, IDC_ST_NAME, m_st_name);
	DDX_Text(pDX, IDC_ST_TILE_SIZE, m_cst_tile_size);
	DDX_Text(pDX, IDC_ST_TILES_PER_BLOCK, m_cst_tiles_per_block);
	DDX_Text(pDX, IDC_ST_HEIGHT_MOD, m_cst_height_scale_mod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_zone_prop, CDialog)
	//{{AFX_MSG_MAP(dlg_zone_prop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_zone_prop message handlers

BOOL dlg_zone_prop::OnInitDialog() 
{
	CDialog::OnInitDialog();
	char st_temp[256];

	sprintf(st_temp, "%.6f",cls_tglo.p_map->f_gravity_override);
	this->m_st_gravity.Format("%s", st_temp);

   	sprintf(st_temp, "%.3f",cls_tglo.p_map->get_block_size());
	this->m_cst_tile_size.Format("%s", st_temp);

 	this->m_cst_tiles_per_block.Format("%d", cls_tglo.p_map->GetTilesPerBlock());
    m_cst_height_scale_mod.Format("1.0");
    this->m_st_name.Format("%s", cls_tglo.p_map->st_name); 
	UpdateData(D_TO_WINDOW);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlg_zone_prop::OnOK() 
{
	UpdateData(D_TO_VAR);
	
	cls_tglo.p_map->f_gravity_override = atof(m_st_gravity);
	strcpy(cls_tglo.p_map->st_name, m_st_name);

	cls_tglo.p_map->init_gravity();

    cls_tglo.p_map->set_block_size(atof((LPCSTR)m_cst_tile_size));
    cls_tglo.p_map->SetTilesPerBlock(atoi((LPCSTR)m_cst_tiles_per_block));

    float f_scale = atof(m_cst_height_scale_mod);

    //reinit the map

      CEngineMessage *p_new = new CEngineMessage();
    p_new->m_i_message_type = C_THING_MSG_GENERIC;
    CMessageGeneric *p_mg = new CMessageGeneric();
    p_new->m_p_message = p_mg;
    p_new->m_b_system_message = true;
    p_mg->i_message_command = C_MESSAGE_COMMAND_REINIT_MAP;
    p_mg->f_scale = f_scale;
    SendWindowsEngineMessage(p_new);
 
	CDialog::OnOK();
}
