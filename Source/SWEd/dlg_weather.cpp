// dlg_weather.cpp : implementation file
//

#include "stdafx.h"
#include "swed.h"
#include "dlg_weather.h"
#include "..\\..\\..\\shared\\mfc_all.h"
#include "..\\..\\..\\shared\\3d\\CThing_globals.h"
#include "..\\..\\..\\shared\\3d\\CThingList.h"
#include "..\\..\\..\\shared\\3d\\CWeather.h"
#include "..\\..\\..\\shared\\3d\\d3dapp.h"
#include "dlg_3d.h"
#include "CGlobals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_weather dialog
//kind of a global we use for this
CWeatherSetting g_weather;

bool g_b_loaded_default_weather = false;
dlg_weather::dlg_weather(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_weather::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_weather)
	m_cst_back = _T("");
	m_cst_bottom = _T("");
	m_cst_front = _T("");
	m_cst_left = _T("");
	m_cst_right = _T("");
	m_cst_top = _T("");
	m_skybox_height_offset = _T("");
	m_skybox_size = _T("");
	m_st_ambient_b = _T("");
	m_st_ambient_g = _T("");
	m_st_ambient_r = _T("");
	m_st_light_b = _T("");
	m_st_light_r = _T("");
	m_st_light_g = _T("");
	m_st_light_dir_x = _T("");
	m_st_light_dir_y = _T("");
	m_st_light_dir_z = _T("");
	m_st_fog_end = _T("");
	m_st_fog_start = _T("");
	m_st_fog_b = _T("");
	m_st_fog_g = _T("");
	m_st_fog_r = _T("");
	m_st_skydome_tex = _T("");
	m_skydome_height_offset = _T("");
	m_skydome_light_disable = FALSE;
	m_skydome_size_x = _T("");
	m_skydome_size_y = _T("");
	m_skydome_size_z = _T("");
	m_cst_script = _T("");
	//}}AFX_DATA_INIT
}


void dlg_weather::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_weather)
	DDX_Text(pDX, IDC_SKYBOX_BACK, m_cst_back);
	DDX_Text(pDX, IDC_SKYBOX_BOTTOM, m_cst_bottom);
	DDX_Text(pDX, IDC_SKYBOX_FRONT, m_cst_front);
	DDX_Text(pDX, IDC_SKYBOX_LEFT, m_cst_left);
	DDX_Text(pDX, IDC_SKYBOX_RIGHT, m_cst_right);
	DDX_Text(pDX, IDC_SKYBOX_TOP, m_cst_top);
	DDX_Text(pDX, IDC_SKYBOX_HEIGHT_OFFSET, m_skybox_height_offset);
	DDX_Text(pDX, IDC_SKYBOX_SIZE, m_skybox_size);
	DDX_Text(pDX, IDC_AMBIENT_B, m_st_ambient_b);
	DDX_Text(pDX, IDC_AMBIENT_G, m_st_ambient_g);
	DDX_Text(pDX, IDC_AMBIENT_R, m_st_ambient_r);
	DDX_Text(pDX, IDC_LIGHT_B, m_st_light_b);
	DDX_Text(pDX, IDC_LIGHT_R, m_st_light_r);
	DDX_Text(pDX, IDC_LIGHT_G, m_st_light_g);
	DDX_Text(pDX, IDC_LIGHT_DIR_X, m_st_light_dir_x);
	DDX_Text(pDX, IDC_LIGHT_DIR_Y, m_st_light_dir_y);
	DDX_Text(pDX, IDC_LIGHT_DIR_Z, m_st_light_dir_z);
	DDX_Text(pDX, IDC_FOG_END, m_st_fog_end);
	DDX_Text(pDX, IDC_FOG_START, m_st_fog_start);
	DDX_Text(pDX, IDC_FOG_B, m_st_fog_b);
	DDX_Text(pDX, IDC_FOG_G, m_st_fog_g);
	DDX_Text(pDX, IDC_FOG_R, m_st_fog_r);
	DDX_Text(pDX, IDC_SKYDOME_TEX, m_st_skydome_tex);
	DDX_Text(pDX, IDC_SKYDOME_HEIGHT_OFFSET, m_skydome_height_offset);
	DDX_Check(pDX, IDC_SKYDOME_LIGHT_DISABLE, m_skydome_light_disable);
	DDX_Text(pDX, IDC_SKYDOME_SIZE_X, m_skydome_size_x);
	DDX_Text(pDX, IDC_SKYDOME_SIZE_Y, m_skydome_size_y);
	DDX_Text(pDX, IDC_SKYDOME_SIZE_Z, m_skydome_size_z);
	DDX_Text(pDX, IDC_SCRIPT, m_cst_script);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_weather, CDialog)
	//{{AFX_MSG_MAP(dlg_weather)
	ON_BN_CLICKED(ID_APPLY, OnApply)
	ON_BN_CLICKED(ID_SAVE, OnSave)
	ON_BN_CLICKED(ID_LOAD, OnLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_weather message handlers

void dlg_weather::OnApply() 
{
 
	PutWeatherSettings(&g_weather); //put info back into our var

  //send this info to the engine
  CWeather *p_weth = (CWeather*)cls_tglo.p_thing_list->get_thing_by_name("Weather");

  if (p_weth)
  {
  
     p_weth->UpdateFromWeatherSetting(&g_weather, true);
  }
}

void dlg_weather::OnOK() 
{
    OnApply(); //activate any changes
	CDialog::OnOK();
}

BOOL dlg_weather::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//copy the info in

	if (!g_b_loaded_default_weather)
	{
		g_b_loaded_default_weather = true;
		//one time refresh
		CWeather *p_weth = (CWeather*)cls_tglo.p_thing_list->get_thing_by_name("Weather");
		
		if (p_weth)
		{
			p_weth->GetWeatherSettings(&g_weather);
		}
		
		
	}

	GetWeatherSettings(&g_weather);


	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlg_weather::GetWeatherSettings(CWeatherSetting * p_weather)
{
	//copy everything to our local vars, then update our window.

	this->m_cst_back = p_weather->a_st_tex[C_SKYBOX_BACK];
	this->m_cst_front = p_weather->a_st_tex[C_SKYBOX_FRONT];
	this->m_cst_left = p_weather->a_st_tex[C_SKYBOX_LEFT];
	this->m_cst_right = p_weather->a_st_tex[C_SKYBOX_RIGHT];
	this->m_cst_bottom = p_weather->a_st_tex[C_SKYBOX_BOTTOM];
	this->m_cst_top = p_weather->a_st_tex[C_SKYBOX_TOP];

	this->m_skybox_height_offset.Format("%.3f", p_weather->f_skybox_height_offset);
	this->m_skybox_size.Format("%.3f", p_weather->f_skybox_side_size);

	//lighting
	this->m_st_light_dir_x.Format("%.3f", p_weather->vec_light_dir.x);
	this->m_st_light_dir_y.Format("%.3f", p_weather->vec_light_dir.y);
	this->m_st_light_dir_z.Format("%.3f", p_weather->vec_light_dir.z);

	this->m_st_light_r.Format("%.3f", p_weather->vec_light_color.x);
 	this->m_st_light_g.Format("%.3f", p_weather->vec_light_color.y);
	this->m_st_light_b.Format("%.3f", p_weather->vec_light_color.z);

	this->m_st_ambient_r.Format("%.3f", p_weather->vec_ambient_light.x);
	this->m_st_ambient_g.Format("%.3f", p_weather->vec_ambient_light.y);
	this->m_st_ambient_b.Format("%.3f", p_weather->vec_ambient_light.z);

	//fog
	this->m_st_fog_start.Format("%.3f", p_weather->f_fog_start);
	this->m_st_fog_end.Format("%.3f", p_weather->f_fog_end);

	this->m_st_fog_r.Format("%.3f", p_weather->vec_fog_color.x);
	this->m_st_fog_g.Format("%.3f", p_weather->vec_fog_color.y);
	this->m_st_fog_b.Format("%.3f", p_weather->vec_fog_color.z);

	//skydome
	this->m_st_skydome_tex = p_weather->st_skydome_tex;
	this->m_skydome_height_offset.Format("%.3f", p_weather->f_skybox_height_offset);

	this->m_skydome_light_disable =  p_weather->b_skydome_disable_lighting;

	this->m_skydome_size_x.Format("%.3f", p_weather->vec_skydome_size.x);
	this->m_skydome_size_y.Format("%.3f", p_weather->vec_skydome_size.y);
	this->m_skydome_size_z.Format("%.3f", p_weather->vec_skydome_size.z);

    this->m_cst_script = p_weather->st_script;

	//send the info to the window
	UpdateData(D_TO_WINDOW);
}

void dlg_weather::PutWeatherSettings(CWeatherSetting * p_weather)
{
	//copy all current info into the vars from the window boxes
	UpdateData(D_TO_VAR);

	strcpy(p_weather->a_st_tex[C_SKYBOX_BACK], (LPCSTR)m_cst_back);
	strcpy(p_weather->a_st_tex[C_SKYBOX_FRONT], (LPCSTR)m_cst_front);
	strcpy(p_weather->a_st_tex[C_SKYBOX_LEFT], (LPCSTR)m_cst_left);
	strcpy(p_weather->a_st_tex[C_SKYBOX_RIGHT], (LPCSTR)m_cst_right);
	strcpy(p_weather->a_st_tex[C_SKYBOX_BOTTOM], (LPCSTR)m_cst_bottom);
	strcpy(p_weather->a_st_tex[C_SKYBOX_TOP], (LPCSTR)m_cst_top);

	p_weather->f_skybox_height_offset = atof(m_skybox_height_offset);
	p_weather->f_skybox_side_size = atof(m_skybox_size);

 	p_weather->vec_light_dir.x = atof(m_st_light_dir_x);
 	p_weather->vec_light_dir.y = atof(m_st_light_dir_y);
 	p_weather->vec_light_dir.z = atof(m_st_light_dir_z);

 	p_weather->vec_light_color.x = atof(m_st_light_r);
 	p_weather->vec_light_color.y = atof(m_st_light_g);
 	p_weather->vec_light_color.z = atof(m_st_light_b);

 	p_weather->vec_ambient_light.x = atof(m_st_ambient_r);
 	p_weather->vec_ambient_light.y = atof(m_st_ambient_g);
 	p_weather->vec_ambient_light.z = atof(m_st_ambient_b);
    //fog

 	p_weather->f_fog_start = atof(m_st_fog_start);
 	p_weather->f_fog_end = atof(m_st_fog_end);

 	p_weather->vec_fog_color.x = atof(m_st_fog_r);
 	p_weather->vec_fog_color.y = atof(m_st_fog_g);
 	p_weather->vec_fog_color.z = atof(m_st_fog_b);


	//skydome
	strcpy(p_weather->st_skydome_tex, (LPCSTR)m_st_skydome_tex);
 	p_weather->f_skydome_height_offset = atof(m_skydome_height_offset);

 	p_weather->vec_skydome_size.x = atof(m_skydome_size_x);
 	p_weather->vec_skydome_size.y = atof(m_skydome_size_y);
 	p_weather->vec_skydome_size.z = atof(m_skydome_size_z);

	p_weather->b_skydome_disable_lighting = m_skydome_light_disable != 0;

    strcpy(p_weather->st_script, (LPCSTR)m_cst_script);
 
}
																															  																						 

void dlg_weather::OnSave() 
{
       cls_tglo.p_d3dapp->Pause(true);
    //save our weather settings to a file of our choice
	PutWeatherSettings(&g_weather);


	CFileDialog cfd(false, //true if open, false if close
		 ".wet", //default file extension
		 glo.cls_swed.st_weather_file_name, OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
		 "Weather Files (*.wet)|*.wet|All files (*.*)|*.*|",//file filter
		 AfxGetApp()->m_pMainWnd);
   	char st_temp[MAX_PATH];
    strcpy((char*)&st_temp, GetExeDir());
    
	 strcat(st_temp, "\\data");
	 cfd.m_ofn.lpstrInitialDir = st_temp;
	 

		 if(cfd.DoModal() == IDOK)
		 {
		 
             strcpy((char*)&st_temp, GetExeDir());
             
			 strcat(st_temp, "\\");
			 sprintf(glo.cls_swed.st_weather_file_name, "%s", (LPCSTR)cfd.GetPathName());
		
			 
			 replace(st_temp, "", (char*)&glo.cls_swed.st_weather_file_name);
		


			 log_msg("Saving %s.", glo.cls_swed.st_weather_file_name);	     
		 		 
					 //remember current weathersettings
					  //remember where the camera was for edit mode
				  g_weather.Save(glo.cls_swed.st_weather_file_name);
		 } else
		 {

		   log_msg("Did not save.");
		 }
	
         cls_tglo.p_d3dapp->Pause(false);
         glo.p_dlg_3d->Invalidate(false); //kickstart drawing

}

void dlg_weather::OnLoad() 
{

    cls_tglo.p_d3dapp->Pause(true);
    
    // TODO: Add your control notification handler code here
	 CFileDialog cfd(true, //true if open, false if close
		 ".wed", //default file extension
		 "", OFN_HIDEREADONLY|OFN_NOCHANGEDIR ,
		 "Weather Files (*.wet)|*.wet|All files (*.*)|*.*|",//file filter
		 AfxGetApp()->m_pMainWnd);
		char st_temp[MAX_PATH];
	 strcpy((char*)&st_temp, GetExeDir());
	 strcat(st_temp, "\\data");
	 cfd.m_ofn.lpstrInitialDir = st_temp;
	// log_msg("Going to do %s.",st_temp);

		 if(cfd.DoModal() == IDOK)
		 {
             strcpy((char*)&st_temp, GetExeDir());
             strcat(st_temp, "\\");
		     strcpy(glo.cls_swed.st_weather_file_name,   cfd.GetPathName());
			 replace(st_temp, "", (char*)&glo.cls_swed.st_weather_file_name);
		

			 
			 g_weather.Load(glo.cls_swed.st_weather_file_name);
		 } else
		 {

		   log_msg("Did not load.");
		 }
	GetWeatherSettings(&g_weather);
 
    cls_tglo.p_d3dapp->Pause(false);
    glo.p_dlg_3d->Invalidate(false); //kickstart drawing
}
