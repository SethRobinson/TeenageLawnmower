// dlg_swed.cpp : implementation file
//


#include "stdafx.h"
#include "SWEd.h"
#include "dls_swed.h"
#include "CGlobals.h"
#include "dlg_log.h"
#include "file_swed.h"
#include "dlg_new.h"
#include "dlg_terraform.h"
#include "dlg_world_edit.h"
#include "dlg_landscape.h"
#include "..\\..\\..\\shared\\mfc_all.h"
#include "..\\..\\..\\shared\\all.h"
#include "..\\..\\..\\shared\\3d\\CEntBall.h"
#include "dlg_zone_prop.h"
#include "..\\..\\..\\shared\\3d\\CEntHuman.h"
#include "dlg_sub_prop.h"
#include "..\\..\\..\\shared\\3d\\CGridTile.h"
#include "dlg_edit_sub.h"
#include "dlg_edit_entity.h"
#include "..\\..\\..\\shared\\3d\\file_utils.h"
#include "..\\..\\..\\shared\\3d\\CMessageHeader.h"
#include "dlg_weather.h"
#include "CDlgEditConfig.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// dlg_swed dialog

dlg_swed::dlg_swed(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_swed::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_swed)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_hAcceleratorTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE (IDR_ACCELERATOR1));
}

void dlg_swed::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_swed)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

//	DDX_Control(pDX, IDC_VIEW_1, m_but_view_1);
//	DDX_Control(pDX, IDC_VIEW_WALK, m_but_view_walk);

}

BEGIN_MESSAGE_MAP(dlg_swed, CDialog)
	//{{AFX_MSG_MAP(dlg_swed)
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_VIEW_FREECAMERA, OnViewFreecamera)
	ON_COMMAND(ID_VIEW_PLAYERCAMERA, OnViewPlayercamera)
	ON_WM_CLOSE()
	ON_COMMAND(IDD_NEW_ZONE, OnNewZone)
	ON_COMMAND(IDC_TERRAFORM, OnTerraform)
	ON_COMMAND(IDD_RAND_MOVE_SUN, OnRandMoveSun)
	ON_COMMAND(IDD_RAND_SET_FOG, OnRandSetFog)
	ON_COMMAND(IDD_LIGHT_RESET, OnLightReset)
	ON_COMMAND(ID_ZONE_TOGGLEWIREFRAMEMODE, OnZoneTogglewireframemode)
	ON_COMMAND(IDR_WORLD_EDIT, OnWorldEdit)
	ON_COMMAND(ID_OPEN_LANDSCAPE, OnOpenLandscape)
	ON_COMMAND(ID_LOADLEVEL_LOADLEVEL, OnLoadlevelLoadlevel)
	ON_COMMAND(ID_LOADLEVEL_SAVELEVEL, OnLoadlevelSavelevel)
	ON_COMMAND(ID_DIAGNOSTICS_LISTLOADEDRESOURCES, OnDiagnosticsListloadedresources)
	ON_COMMAND(ID_DIAGNOSTICS_CREATESOMECARS, OnDiagnosticsCreatesomecars)
	ON_COMMAND(ID_DIAGNOSTICS_CREATESOMESPHERES, OnDiagnosticsCreatesomespheres)
	ON_COMMAND(IDR_ZONE_PROP, OnZoneProp)
	ON_COMMAND(ID_DIAGNOSTICS_LISTLOADEDENTITIES, OnDiagnosticsListloadedentities)
	ON_COMMAND(ID_DIAGNOSTICS_CREATESOMETREESPRITES, OnDiagnosticsCreatesometreesprites)
	ON_COMMAND(ID_VIEW_SETCAMERATOTOPVIEWMODE, OnViewSetcameratotopviewmode)
	ON_COMMAND(ID_VIEW_CARDRIVINGMODE, OnViewCardrivingmode)
	ON_COMMAND(ID_DIAG_CREATEHUMAN, OnDiagCreatehuman)
	ON_COMMAND(ID_SUBZONE_PROPERTIES, OnSubzoneProperties)
	ON_COMMAND(ID_TILE_TOGGLETILESHOWING, OnTileToggletileshowing)
	ON_COMMAND(ID_OPEN_SUB_ZONE_EDIT, OnOpenSubZoneEdit)
	ON_COMMAND(ID_CREATE500CARS, OnCreate500cars)
	ON_COMMAND(ID_VIEW_SETCAMERATOPLAYERWALKMODE, OnViewSetcameratoplayerwalkmode)
	ON_COMMAND(ID_OPEN_ENTITY_TOOL, OnOpenEntityTool)
	ON_COMMAND(ID_ZONE_DELETEALLENTS, OnZoneDeleteallents)
	ON_COMMAND(MI_WEATHER_PRESET, OnWeatherPreset)
	ON_COMMAND(ID_EDITOR_CONFIG, OnEditorConfig)
	ON_COMMAND(ID_ZONE_WIRE, OnZoneWire)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_swed message handlers

BOOL dlg_swed::OnInitDialog()
{
	
	CDialog::OnInitDialog();
   	glo.p_dlg_main = this;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	
	  //switch to current dir
	switch_to_current_dir();


	//first let's load out defaults
	file_swed_load(&glo.cls_swed);
	

	//force our size
	if (glo.cls_swed.rect_main.right == 0)
	this->SetWindowPos(&wndTop, 0,0,GetSystemMetrics(SM_CXSCREEN),
	GetSystemMetrics(SM_CYSCREEN), SWP_NOZORDER); 
	else
	this->SetWindowPos(&wndTop, glo.cls_swed.rect_main.left,glo.cls_swed.rect_main.top,
	glo.cls_swed.rect_main.right-glo.cls_swed.rect_main.left,glo.cls_swed.rect_main.bottom-glo.cls_swed.rect_main.top, SWP_NOZORDER); 
	//Create the 3d moveable window
  
	dlg_log *p_dlg_log = new dlg_log();
	p_dlg_log->Create(IDD_LOG, this);

	/*
		m_but_view_1.SetBitmaps(IDB_VIEW_3D, RGB(0, 255, 0));
	    m_but_view_1.SetTooltipText("Free floating camera");
		m_but_view_1.ActivateTooltip(true);
	
		m_but_view_walk.SetBitmaps(IDB_VIEW_WALK, RGB(0, 255, 0));
	    m_but_view_walk.SetTooltipText("Free floating camera");
		m_but_view_walk.ActivateTooltip(true);
	 */
 
    
    
    log_msg("SWED v%.2f Initialized.", C_F_VERSION);
 	log_msg("Click 3d window to get focus then use S,D,F,V to move the view around. Use Space and R to go up and down. Arrow keys to turn and tilt.");

  dlg_3d *dlg = new dlg_3d();
	dlg->Create(IDD_3D, this);



	return TRUE;  // return TRUE  unless you set the focus to a control
}


dlg_swed::~dlg_swed()
	{
	  //save defaults
	
	
	  SAFE_DELETE(glo.p_dlg_log);
 	  SAFE_DELETE(glo.p_dlg_3d);
	}




// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR dlg_swed::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void dlg_swed::OnViewFreecamera() 
{
   SendSimpleWindowsEngineMessage(C_MESSAGE_COMMAND_SWITCH_TO_FREE_CAM, 0);
   if (glo.p_dlg_3d)
   glo.p_dlg_3d->SetFocus();
}

void dlg_swed::OnViewPlayercamera() 
{
  SendSimpleWindowsEngineMessage(C_MESSAGE_COMMAND_SWITCH_TO_PLAY_CAM, 0);
  
  
  if (glo.p_dlg_3d)
   glo.p_dlg_3d->SetFocus();
}

void dlg_swed::save_defaults()
{
  //grab info we need and save the defaults
  	WINDOWPLACEMENT win_temp;

	this->GetWindowPlacement(&win_temp);
	CopyRect(&glo.cls_swed.rect_main, &win_temp.rcNormalPosition);
	if (glo.p_dlg_3d)
	{
	glo.p_dlg_3d->GetWindowPlacement(&win_temp);
	CopyRect(&glo.cls_swed.rect_3d, &win_temp.rcNormalPosition);
	}

	if (glo.p_dlg_log)
	{
	glo.p_dlg_log->GetWindowPlacement(&win_temp);
	CopyRect(&glo.cls_swed.rect_log, &win_temp.rcNormalPosition);
	}

	
	//save stuff to disc
	file_swed_save(&glo.cls_swed);
}

void dlg_swed::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	save_defaults();
	
	CDialog::OnClose();
}

void dlg_swed::OnNewZone() 
{

	dlg_new dlg;
	dlg.DoModal();

}

void dlg_swed::OnTerraform() 
{
	// TODO: Add your command handler code here
	dlg_terraform dlg;
	dlg.DoModal();
	
}

void dlg_swed::OnRandMoveSun() 
{
	// TODO: Add your command handler code here
  SendSimpleWindowsEngineMessage(C_MESSAGE_COMMAND_RANDOM_SUN, C_THING_ENT_GENERIC);
    if (glo.p_dlg_3d)
   glo.p_dlg_3d->SetFocus();
}

void dlg_swed::OnRandSetFog() 
{
	// TODO: Add your command handler code here
  SendSimpleWindowsEngineMessage(C_MESSAGE_COMMAND_RANDOM_FOG, C_THING_ENT_GENERIC);
    if (glo.p_dlg_3d)
   glo.p_dlg_3d->SetFocus();
}

void dlg_swed::OnLightReset() 
{
	// TODO: Add your command handler code here
  SendSimpleWindowsEngineMessage(C_MESSAGE_COMMAND_RESET_LIGHT_AND_FOG, C_THING_ENT_GENERIC);
 	
   if (glo.p_dlg_3d)
   glo.p_dlg_3d->SetFocus();
}

void dlg_swed::OnZoneTogglewireframemode() 
{
   SendSimpleWindowsEngineMessage(C_MESSAGE_COMMAND_TOGGLE_LAND_WIREFRAME, C_THING_ENT_GENERIC);
  if (glo.p_dlg_3d)
   glo.p_dlg_3d->SetFocus();
}

void dlg_swed::OnWorldEdit() 
{
	// TODO: Add your command handler code here
	if (!glo.p_dlg_world_edit)
	{
	kill_all_focus_required_dialogs();
	log_msg("Opening world edit dialog.");
	dlg_world_edit *p_dlg = new dlg_world_edit();
	p_dlg->Create(IDD_WORLD_EDIT, this);
	} else glo.p_dlg_world_edit->SetFocus();

	
}

void dlg_swed::OnOpenLandscape() 
{
	if (!glo.p_dlg_landscape)
	{
	  //kill any confilicting dialogs
		kill_all_focus_required_dialogs();
		
	log_msg("Opening landscape dialog.");
	dlg_landscape *p_dlg = new dlg_landscape();
	p_dlg->Create(IDD_LANDSCAPE, this);
	} else glo.p_dlg_landscape->SetFocus();

	
}

void kill_all_focus_required_dialogs()
{
   //tell these guys to close.  They will set the pointer to NULL after thet
   //do.
   
   if (glo.p_dlg_landscape) glo.p_dlg_landscape->SendMessage(WM_CLOSE);
   if (glo.p_dlg_world_edit) glo.p_dlg_world_edit->SendMessage(WM_CLOSE);
   if (glo.p_dlg_edit_sub) glo.p_dlg_edit_sub->SendMessage(WM_CLOSE);
   if (glo.p_dlg_edit_entity) glo.p_dlg_edit_entity->SendMessage(WM_CLOSE);

}

BOOL dlg_swed::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_TIMER)
	{
	//og_msg("Die!");
	}
	if (m_hAcceleratorTable != NULL)
		 if (::TranslateAccelerator(m_hWnd, m_hAcceleratorTable, pMsg))    
			 return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void dlg_swed::OnLoadlevelLoadlevel() 
{
    if (!cls_tglo.p_d3dapp)
    {
        log_msg("No 3d window active, can't load.");
        return;
    }
     cls_tglo.p_d3dapp->Pause(true);
	
    //load a new map	
 	 CFileDialog cfd(true, //true if open, false if close
		 ".zon", //default file extension
		 "", OFN_HIDEREADONLY|OFN_NOCHANGEDIR ,
		 "Zone Files (*.zon)|*.zon|All files (*.*)|*.*|",//file filter
		 AfxGetApp()->m_pMainWnd);
		char st_temp[MAX_PATH];
	 strcpy((char*)&st_temp, GetExeDir());
	 
     if (cls_tglo.mod.m_st_path[0])
     {
      strcat(st_temp, "\\");
     strcat(st_temp, cls_tglo.mod.m_st_path);
      }
    
     
     strcat(st_temp, "\\data");
	 cfd.m_ofn.lpstrInitialDir = st_temp;
//	 log_msg("Going to do %s.",st_temp);
   		                       
		 if(cfd.DoModal() == IDOK)
		 {
		     
             strcpy((char*)&st_temp, GetExeDir());
             strcat(st_temp, "\\");

	 
			 strcpy(glo.cls_swed.st_file_name,   cfd.GetPathName());
		
             if (cls_tglo.mod.m_st_path[0])
             {
                 strcat(st_temp, "\\");
                 strcat(st_temp, cls_tglo.mod.m_st_path);
             }
             
             
             replace(st_temp, "", (char*)&glo.cls_swed.st_file_name);
					
			 
			 bool b_load_tile_grid = false;
			 //kill tile grid if it's up, it will be wrong if we don't
			 if (cls_things.get_thing_count(C_THING_TILE) > 0)
			 {
				 //it exists, just kill it
				 cls_things.delete_things_by_type(C_THING_TILE);
				 b_load_tile_grid = true; //reload it later
			 }
			 
			 //freeze game, or we'll be using bad data because this is multithreaded
			 
			 extern int i_last_selection;
			 i_last_selection = -1; //for the edit sub menu
			
			 
			 
			 if ( cls_tglo.p_map->load_zone(glo.cls_swed.st_file_name))
			{
			 	  if (cls_tglo.p_camera)
				  {
					  bool b_handle_iso = false;
					  if (cls_tglo.p_camera->get_camera_mode() == C_CAMERA_MODE_ORTHOGRAPHIC)
					  {
					    cls_tglo.p_camera->toggle_isometric_view(); //switch back for a sec  
						b_handle_iso = true;
					  }
					  
				  //update camera
				  cls_tglo.p_camera->setObjectPosition(cls_tglo.p_map->vec_camera_pos);
				  cls_tglo.p_camera->setAngleX(cls_tglo.p_map->vec_camera_look.x);
				  cls_tglo.p_camera->setAngleY(cls_tglo.p_map->vec_camera_look.y);
				  cls_tglo.p_camera->setAngleZ(cls_tglo.p_map->vec_camera_look.z);
				  
				  if (b_handle_iso)
				  {
				    cls_tglo.p_camera->toggle_isometric_view();

				  }
				  
				  }

				  clear_objects_from_map(&cls_things, ::C_THING_ENT_GENERIC);
				  load_objects_for_map(&cls_things, glo.cls_swed.st_file_name);


			}
		  		
                     if (b_load_tile_grid)
					 {
					  OnTileToggletileshowing(); 
					 }

					 if (glo.p_dlg_edit_sub)
						 glo.p_dlg_edit_sub->build_combo_boxes(); //reinit this

		 } else
		 {

		   log_msg("Did not load.");
		 }
     	 cls_tglo.p_d3dapp->Pause(false);
	  glo.p_dlg_3d->Invalidate(false); //kickstart drawing
			
}  	


void dlg_swed::OnLoadlevelSavelevel() 
{

    	 cls_tglo.p_d3dapp->Pause(true);


    CFileDialog cfd(false, //true if open, false if close
		 ".zon", //default file extension
		 glo.cls_swed.st_file_name, OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
		 "Zone Files (*.zon)|*.zon|All files (*.*)|*.*|",//file filter
		 AfxGetApp()->m_pMainWnd);
		char st_temp[MAX_PATH];
	 strcpy((char*)&st_temp, GetExeDir());

     
     if (cls_tglo.mod.m_st_path[0])
     {
      strcat(st_temp, "\\");
     strcat(st_temp, cls_tglo.mod.m_st_path);
     }
     


     strcat(st_temp, "\\data");
	 
	 cfd.m_ofn.lpstrInitialDir = st_temp;
   
	
		 if(cfd.DoModal() == IDOK)
		 {
		 		     strcpy(glo.cls_swed.st_file_name,   cfd.GetPathName());
		    		 //remove system specific path
				  _getcwd(st_temp, 256);
                      
				 strcat(st_temp, "\\");

			 replace(st_temp, "", (char*)&glo.cls_swed.st_file_name);
					  log_msg("Saving %s.",glo.cls_swed.st_file_name );
					 //remember current camera settings
					  //remember where the camera was for edit mode
				  if (cls_tglo.p_camera)
				  {
				  cls_tglo.p_map->vec_camera_pos = cls_tglo.p_camera->getObjectPosition();
				  cls_tglo.p_map->vec_camera_look.x = cls_tglo.p_camera->getAngleX();
				  cls_tglo.p_map->vec_camera_look.y = cls_tglo.p_camera->getAngleY();
				  cls_tglo.p_map->vec_camera_look.z = cls_tglo.p_camera->getAngleZ();
				  }
 		
				  cls_tglo.p_map->save_zone(glo.cls_swed.st_file_name);
				  save_objects_for_map(&cls_things, glo.cls_swed.st_file_name);


		 } else
		 {

		   log_msg("Did not save.");
		 }
	   	 cls_tglo.p_d3dapp->Pause(false);
	  glo.p_dlg_3d->Invalidate(false); //kickstart drawing

}

void dlg_swed::OnDiagnosticsListloadedresources() 
{
	// TODO: Add your command handler code here
       SendSimpleWindowsEngineMessage(C_MESSAGE_COMMAND_LIST_ALL_RESOURCES, 0);
 
 //   cls_tglo.p_manager->list_contents();	
}

void dlg_swed::OnDiagnosticsCreatesomecars() 
{
  	log_msg("This has been disabled.");
	
}

void dlg_swed::OnDiagnosticsCreatesomespheres() 
{
	// TODO: Add your command handler code here
	log_msg("Creating 20 spheres. Tap E to make them bounce. They will disappear in 30 seconds.");

     SendSimpleWindowsEngineMessage(C_MESSAGE_COMMAND_CREATE_SOME_SPHERES, 0);
 
}

void dlg_swed::OnZoneProp() 
{
    dlg_zone_prop dlg;
	dlg.DoModal();

}

void dlg_swed::OnDiagnosticsListloadedentities() 
{
  
    SendSimpleWindowsEngineMessage(C_MESSAGE_COMMAND_LIST_ALL_THINGS, 0);
 
	
}

void dlg_swed::OnDiagnosticsCreatesometreesprites() 
{
	// TODO: Add your command handler code here
	log_msg("This function has been disabled.");
  	
	
}

void dlg_swed::OnViewSetcameratotopviewmode() 
{
	// TODO: Add your command handler code here
   //
    SendSimpleWindowsEngineMessage(C_MESSAGE_COMMAND_SWITCH_TO_TOP_VIEW_CAM, 0);
 
    if (glo.p_dlg_3d)
   glo.p_dlg_3d->SetFocus();
	
}


void dlg_swed::OnViewCardrivingmode() 
{
	// TODO: Add your command handler code here
 // switch_to_car_cam();
    log_msg("Sorry, the car driving mode has been disabled.  I didn't want to include the model.");
    if (glo.p_dlg_3d)
   glo.p_dlg_3d->SetFocus();
	
}

void dlg_swed::OnDiagCreatehuman() 
{
	// TODO: Add your command handler code here
    log_msg("Cal3d skeleton system disabled.  It's too slow anyway.");
    /*

    log_msg("Creating a skinned human using skeletal animation.");
	CEntHuman *p_human;

	for (int i=0; i < 1; i++)					  
	{
	p_human = new CEntHuman();
	cls_things.add_new_thing(p_human);
	p_human->dx_create();
	}
	  */

}

void dlg_swed::OnSubzoneProperties() 
{
	  dlg_sub_prop dlg;
	  dlg.DoModal();
}

void dlg_swed::OnTileToggletileshowing() 
{
  SendSimpleWindowsEngineMessage(C_MESSAGE_COMMAND_TOGGLE_SHOW_TILE, 0);
   if (glo.p_dlg_3d)
   glo.p_dlg_3d->SetFocus();
 
}

void dlg_swed::OnOpenSubZoneEdit() 
{
	if (!glo.p_dlg_edit_sub)
	{
	  //kill any confilicting dialogs
		kill_all_focus_required_dialogs();
		
	log_msg("Opening sub zone edit dialog.");
	dlg_edit_sub *p_dlg = new dlg_edit_sub();
	p_dlg->Create(IDD_EDIT_SUB, this);
	} else glo.p_dlg_edit_sub->SetFocus();

	
}

void dlg_swed::OnCreate500cars() 
{
	// TODO: Add your command handler code here
	log_msg("This function has been disabled.");
	
}

void dlg_swed::OnViewSetcameratoplayerwalkmode() 
{
  //SendSimpleWindowsEngineMessage(C_MESSAGE_COMMAND_SWITCH_TO_PLAYER_WALK_CAM, 0);
  
   log_msg("Sorry, the player cam has been disabled.");
  
   if (glo.p_dlg_3d)
   glo.p_dlg_3d->SetFocus();

}

void dlg_swed::OnOpenEntityTool() 
{
	// TODO: Add your command handler code here
	if (!glo.p_dlg_edit_entity)
	{
	  //kill any confilicting dialogs
		kill_all_focus_required_dialogs();
		
	log_msg("Opening entity edit dialog.");
	dlg_edit_entity *p_dlg = new dlg_edit_entity();
	p_dlg->Create(IDD_EDIT_ENTITY, this);
	} else glo.p_dlg_edit_entity->SetFocus();

	
}

void dlg_swed::OnZoneDeleteallents() 
{
	// TODO: Add your command handler code here
   //do an are you sure message
 	
	if (::MessageBox(NULL, 
		"Are you sure you wish to erase all entities on this scene? (affects the loaded zone only)",
		"Delete all entities from loaded Zone",
		MB_OKCANCEL |  MB_ICONWARNING ) == IDOK)
	{

		log_msg("Deleting all ents from zone.");
      SendSimpleWindowsEngineMessage(C_MESSAGE_COMMAND_DELETE_THINGS_BY_TYPE, C_THING_ENT_GENERIC);
  
	}
	 
   
  

//  	cls_things.delete_things_by_type(C_THING_ENT_GENERIC);
  }

void dlg_swed::OnWeatherPreset() 
{
	dlg_weather dlg;
	dlg.DoModal();

}

void dlg_swed::OnEditorConfig() 
{

    CDlgEditConfig dlg;
    dlg.DoModal();

}

void dlg_swed::OnZoneWire() 
{
    //they want to turn wireframe mod on

    //send a message to the terrain object by way of windows messages, it's
    //threadsafe.
    CEngineMessage *p_new = new CEngineMessage();
    p_new->m_i_message_type = C_THING_MSG_GENERIC;
    CMessageGeneric *p_mg = new CMessageGeneric();
    p_new->m_p_message = p_mg;
    p_mg->i_message_command = C_MESSAGE_COMMAND_TOGGLE_WIREFRAME;
    strcpy(p_new->m_st_entity_name, "Terrain");
    SendWindowsEngineMessage(p_new);

}
