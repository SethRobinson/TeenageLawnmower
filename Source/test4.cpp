
//******************************************
// copyright 2002	Seth A. Robinson

// This file is NOT public domain and may not be shared, distributed or used
// without specific permission in any way.

// seth@rtsoft.com - www.rtsoft.com
//******************************************


#define STRICT

#include "test4.h"
#include "3d\\CThing_globals.h"
#include "3d\\CFloatCam.h"
#include "3d\\CControlPlayer.h"
#include "3d\\CEnt.h"
#include "3d\\CTerrain.h"
#include "3d\\CManager.h"
#include "3d\\CThingFont.h"
#include "3d\\CThingStats.h"
#include "profile.h"
#include "3d\\CBuildingManager.h"	
#include "3d\\CEntLib.h"
#include "3d\\file_utils.h"
#include "CFileLib\\CFileLib.h"
#include "3d\\CEntItem.h"
#include "sound\\ISoundManager.h"
#include "sound\\dx\\gdpSoundManagerDX.h"
#include "sound\\FMOD\\FMSoundManager.h"

#include "control_main.h"
#include "CScriptEngine.h"
#include "3d\\Script_utils.h"
#include "3d\\CConsole.h"
#include "mow_script_utils.h"
#include "3d\\CSettings.h"
#include "3d\\CMessageHeader.h" //message format used in interobject communication 
#include "3d\\CMessage.h" //a thing to handle displaying messages
#include "dlg_security.h"
#include "CProtection.h"
#include "getdxver.h"
#include <d3d8.h>

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEMO
 int g_i_demo_hacked = 0; //1 if not ok
#endif

 #ifdef _EXPIRATION
 int g_i_expiration_hacked = 0; //1 if not ok
#endif


void (*global_init) () = NULL; //callback function pointer
void (*global_kill) () = NULL; //callback function pointer
void (*global_kill_old) () = NULL; //callback function pointer
void (*global_think) () = NULL; //callback function pointer
void (*global_restore) () = NULL; //callback function pointer

bool b_have_run_init = false;

void switch_control(void (*p_init)(),void (*p_kill)(), void (*p_think)(),  void (*p_restore)() )
{
	
	if (global_kill) global_kill_old = global_kill; //remember the old kill
   
   //setup the new one
   global_init = p_init;
   global_think = p_think;
   global_kill = p_kill;
   global_restore = p_restore;
   b_have_run_init = false; //will init ASAP
}

int C_SMOOTHING_FRAMES = 20; //smooth timing over last X amount of frames //set to 1 minimum

float *g_a_f_delta; //array of frames to keep track off
CTimer g_timer_key_check;
//-----------------------------------------------------------------------------
// Global access to the app (needed for the global WndProc())
//-----------------------------------------------------------------------------
CMyD3DApplication* g_pApp  = NULL;
HINSTANCE          g_hInst = NULL;


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything, and goes into a
//       message-processing loop. Idle time is used to render the scene.
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR p_st_command_line, INT )
{
    
    char st_temp[MAX_PATH];
   
    sprintf(st_temp, "%sdata\\resource.cfl",cls_tglo.GetRootDir());
   
	
    if (!GetApp()->GetFileLib()->GetCFL()->openLibrary(st_temp))
    {
        LogError("Error opening %s (not an error if raw .c script files exist instead..)", st_temp);
       // assert(0);
    }
     
    //look for a patch for future use...
    sprintf(st_temp, "%spatch.cfl",cls_tglo.GetRootDir());
   
    if (GetApp()->GetFileLib()->GetCFL()->openLibrary(st_temp))
    {
       LogMsg("Patch found and applied.");
    }
  

    GetApp()->GetCFL()->setOpenStrategy(CFL_OPEN_BEVAVIOR_DISK_CFL);

    
    CMyD3DApplication d3dApp;
   g_pApp  = &d3dApp;					  
    g_hInst = hInst;
    cls_tglo.p_d3dapp = &d3dApp;
    
    cls_tglo.p_cls_input = &cls_input;
   //set some defaults
     cls_tglo.p_cls_input->AddAliasBindVirtualKey("up", C_INPUT_FORWARD);
     cls_tglo.p_cls_input->AddAliasBindVirtualKey("down", C_INPUT_BACKWARD);
     cls_tglo.p_cls_input->AddAliasBindVirtualKey("v", C_INPUT_ACTION2);
     cls_tglo.p_cls_input->AddAliasBindVirtualKey("s", C_INPUT_START);
     cls_tglo.p_cls_input->AddAliasBindVirtualKey("enter", C_INPUT_ACTION3);

    
	//should we even run this?
    cls_pro.UseFixedReduce(true);
    cls_pro.SetSerialMod(3.4f, 23752);
	int i_days = GetDaysSinceDate(2, 1,2003);

#ifdef _EXPIRATION
    
    if ( (i_days < 0) || (i_days > 60))
	{
		show(NULL, "IGF EVALUATION BUILD, NOT TO BE DISTRIBUTED", "This evaluation version of %s has expired.  Contact seth@rtsoft.com",
		app_glo.st_name);
	    return 0;
	}

    //let's show warning thingie
 	//	show(NULL, "IGF EVALUATION BUILD, NOT TO BE DISTRIBUTED","PLEASE DO NOT DISTRIBUTE TO PUBLIC\r\n\r\nThis is an expiring version for evaluation only. (expires in April)\n\nCHEAT ENABLED:  Press P while mowing a level to instantly pass it.",
    //   	app_glo.st_name);

#endif    
 
    switch_to_current_dir();  
  //check dx version
    DWORD dw_version  = GetDXVersion();

    if (dw_version < 0x801)
    {
       int i_result = MessageBox(NULL, "Small problem.  You need to install version 8.1b or newer of DirectX to run this game.\n\nWould you like to open Microsoft's Directx download page now?", "DirectX version from the 70's detected",MB_OKCANCEL);

       if (i_result == IDOK)
       {
               winall_create_url_file("www.microsoft.com/directx");
                        int result = (int)ShellExecute(NULL,"open","temp.url", NULL,NULL, SW_MAXIMIZE);
                        ProcessError(NULL, result, "temp.url","www.microsoft.com/directx"); 
       }

       return 0;

    }
#ifndef _DEBUG

#ifdef _DEMO
   if (exist("texture\\golf_green_1.dds"))
   {
      int i_result = MessageBox(NULL, "Retail data detected.  The demo version can't run the real game.  Install the full version.", "Unknown data detected",MB_OK);
      return 0;
   }
#endif
#endif
   cls_tglo.p_settings = &g_settings;
   g_settings.Load("defaults.dat", true);
	
    //security
   //the real game starts here
#ifndef _NO_PROTECTION
   if (!SecurityCheck(g_hInst))
    {

        //they aborted for some reason.
        return 0;
    }
	
     g_settings.Save("defaults.dat",true);

     cls_pro.demake(g_settings.GetKey(), g_settings.GetName());


     if (!cls_pro.is_validated())
     {
         //code was bad.  let's do an easy to hack message.  (don't worry, the tricky security comes later)
         
       	 g_settings.ResetKey();
         g_settings.Save("defaults.dat", true);
         show(NULL, "Error", "The unlock key is not valid.  Please restart program to enter it again.");
         return false;
     }
#endif

     #ifdef _EXPIRATION
  	i_days = GetDaysSinceDate(1, 1,2003);

  
    if ( (i_days < 0) || (i_days > 91))
	{
	    //should never have got here.. activate emergency thingie.
       g_i_expiration_hacked = random_range(2, 600);
	}

#endif    

    /*
    //fake a hack
    cls_pro.SetSerialMod(2.4f, 23752);
    */
  
    g_a_f_delta = new float[C_SMOOTHING_FRAMES];
	ZeroMemory(g_a_f_delta, sizeof(float)*C_SMOOTHING_FRAMES);
	
	app_glo.process_command_line(p_st_command_line);
//	d3dApp.SetupStartMode(true, 1280, 1024, D3DFMT_X8R8G8B8); //start fullscreen at high res

   if (app_glo.b_disable_joystick) 
   {
       //command line -nojoy joystick override
       g_settings.SetDisableJoystick(true);
       g_settings.Save("defaults.dat", true);

   }
  //  if (!app_glo.b_start_windowed)

   //we have to remember this because it's screwed up during our init, which must be done in
   //windowed mode.
   bool b_save_windowed = g_settings.GetWindowed();
   d3dApp.SetupStartMode(true, g_settings.GetScreenX(), g_settings.GetScreenY(), D3DFMT_X8R8G8B8); //start fullscreen at high res

    
    
    if( FAILED( d3dApp.Create( hInst ) ) )
        return 0;
		//hide curser
    d3dApp.m_bShowCursorWhenFullscreen = false;
	d3dApp.m_bClipCursorWhenFullscreen = false;

    g_settings.SetWindowed(b_save_windowed);
	//force fullscreen now
if (!g_settings.GetWindowed())
{

    d3dApp.ToggleFullscreen();
    
}
			
	return d3dApp.Run();
}

															 


//-----------------------------------------------------------------------------
// Name: CMyD3DApplication()
// Desc: Application constructor. Sets attributes for the app.
//-----------------------------------------------------------------------------
CMyD3DApplication::CMyD3DApplication()
{
    m_dwCreationWidth           = 800;
    m_dwCreationHeight          = 600;
//    m_strWindowTitle            = TEXT( "3D Engine Test by Seth A. Robinson - www.rtsoft.com - not to be distributed or shown to anyone." );
    m_strWindowTitle            = TEXT( app_glo.st_name);
    m_bUseDepthBuffer           = TRUE;
    m_bShowCursorWhenFullscreen = TRUE;


    m_bLoadingApp               = TRUE;
  	m_fWorldRotX                = 0.0f;
    m_fWorldRotY                = 0.0f;
  
}



//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::OneTimeSceneInit()
{
    // TODO: perform one time initialization
   #ifdef _PROFILE
	  	PROFILE_INIT
   #endif
	  
    cls_tglo.p_thing_list = &cls_things;
	cls_tglo.m_i_program = C_GLOBAL_GAME;  //tell the engine to use any game specific code
#ifdef _DEBUG
	randomize();

#endif
        g_timer_key_check.set_interval(random_range(30000, 50000));

    // Drawing loading status message until app finishes loading
    SendMessage( m_hWnd, WM_PAINT, 0, 0 );
   // Initialize DirectInput
	cls_input.SetMouseExclusive(false);
 
	cls_input.init(m_hWnd);
    g_settings.Load("defaults.dat", true); //load again tis time wit cls_input initted.
 
	cls_tglo.set_running(false);
	cls_tglo.p_script_engine = new CScriptEngine();
  cls_tglo.p_script_engine->SetFileLib(GetApp()->GetFileLib());
	add_global_functions_to_script_0(); //add the 3d engine specific functions to the scripting engine
    add_global_functions_to_script_0_mow(); //add our custom functions/vars to the scripting engine
    
    //p_cls_sound = new gdp::SoundManagerDX();
    p_cls_sound = new FMSoundManager();
    
    if (!app_glo.b_disable_sound)
    p_cls_sound->Init(); //comment this out to disable sound
	cls_tglo.p_sound = p_cls_sound; //put this in our global to remember it
    //p_cls_sound->SetMetersPerUnit(0.5); //everything will be louder
    //cls_sound.SetRollOffFactor(2.0f);
	//add our things
   	//make the generic font for statistics and stuff
	CThingFont *p_font = new CThingFont();
	cls_things.add_new_thing(p_font);
	cls_tglo.p_font = p_font;
	//font 0 is for our normal small font
	cls_tglo.p_font->AddFont("Arial","", 36); //font 1 is for our fancy big font
	cls_tglo.p_font->AddFont("Arial","", 24); //font 2 is for our choice font
	cls_tglo.p_font->AddFont("Arial","", 24); //font 3 is for huge things
	cls_tglo.p_font->AddFont("Arial","", 24); //font 4 is for CMessage
    cls_tglo.p_font->SetFontOptions(4, 1000,false); //make this extra bold

 	//let's turn on the console debug system

	CConsole *p_con = new CConsole();
	cls_things.add_new_thing(p_con);

	CCamera *p_cam = new CCamera();
	cls_things.add_new_thing(p_cam);
	cls_tglo.p_camera = p_cam;

	p_cam->setObjectPosition(D3DXVECTOR3(  30*5, 1.0f ,30*5 ));
	p_cam->setAngleY(0);

		//load item base classes
	cls_tglo.p_ent_lib = new CEntLib();

	//Let's create the texture manager and put it in our global
	CManager *p_manager = new CManager();
	cls_things.add_new_thing(p_manager);
	cls_tglo.p_manager = p_manager; //put in global
	p_manager->load_resource_list("data\\textures.txt");
	
	CBuildingManager *p_b = new CBuildingManager();
	cls_things.add_new_thing(p_b);

  	//init map
	
	cls_tglo.p_map = &cls_map;
	cls_tglo.p_map->generate_map(1,1,5,3);
		
	CTerrain *p_terrain = new CTerrain();
	cls_things.add_new_thing(p_terrain);
	cls_tglo.p_terrain = p_terrain;
  	cls_tglo.p_map->send_map_to_terrain(); //must do this to init everything

	//add stats
	CThingStats *p_stats = new CThingStats();
	cls_things.add_new_thing(p_stats);
	p_stats->activate_draw_toggle(DIK_2);
    p_stats->SetShowSimpleFPS(true);
//	CEntHuman *p_human;
//	p_human = new CEntHuman();
//	cls_things.add_new_thing(p_human);
	
    m_bLoadingApp = FALSE;
    cls_tglo.p_ent_lib->load_category_wildcard("*.ent");


control_main_switch();
//startup our first control system

//control_room_switch();


	cls_tglo.set_running(true);

	return S_OK;
}





//-----------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device initialization, this code checks the display device
//       for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::ConfirmDevice( D3DCAPS8* pCaps, DWORD dwBehavior,
                                          D3DFORMAT Format )
{
    BOOL bCapsAcceptable;

	if (pCaps->Caps)

    // TODO: Perform checks to see if these display caps are acceptable.
    bCapsAcceptable = TRUE;


	  if( 0 == ( pCaps->SrcBlendCaps & D3DPBLENDCAPS_ONE ) )
        return E_FAIL;
    if( 0 == ( pCaps->DestBlendCaps & D3DPBLENDCAPS_ONE ) )
        return E_FAIL;

		
		if( bCapsAcceptable )         
			return S_OK;
		else
			return E_FAIL;
	}



//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InitDeviceObjects()
{
    // TODO: create device objects
  	//tell our globals what are going on
	cls_tglo.p_d3dDevice = m_pd3dDevice;
	log_msg("Creating objects");
   	//actually create our objects in 3d space, they have already been created, this is just the
	//final command to tell them it's ok to create the 3d part
	cls_things.dx_create();
    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc: Restores scene objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::RestoreDeviceObjects()
{
 	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
    m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
    m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
    Material mat;

    
    mat.Use(m_pd3dDevice);

	// Set up the default texture states
    m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	  // Turn off culling, so we see the front and back of the Terrain.
     m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,     D3DCULL_CCW );

  
	FLOAT fAspect = m_d3dsdBackBuffer.Width / (FLOAT)m_d3dsdBackBuffer.Height;

	cls_tglo.p_camera->perspective( D3DX_PI/4, fAspect, 0.6f, 600 );

	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, true );	
    
    //m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,     D3DCULL_NONE );
    log_msg("Restoring stuff");
    cls_things.dx_restore();
    
    cls_tglo.p_camera->update_matrix(); //rebuild the camera matrix
	
	if (b_have_run_init)
	{
		if (global_restore)
		global_restore();
	}

	cls_tglo.SetIgnoreNextFrameSmooth(true);
    cls_tglo.p_settings->SetVideoSettingsFromConfig(cls_tglo.p_d3dapp);
	 

    //if debugging, let's not clip the mouse, it's easier to debug that way
#ifndef _DEBUG
    if (!GetApp()->p_d3dapp->IsWindowed())
    {
        
        RECT rcWindow;
        GetWindowRect( m_hWnd, &rcWindow );
        ClipCursor( &rcWindow );
        
    } else
    {
        ClipCursor(NULL);
    }
#endif
    if (!GetApp()->p_d3dapp->IsWindowed())
    {
        cls_tglo.p_d3dapp->SetShowCursor(false);
        
    } else
    {
        cls_tglo.p_d3dapp->SetShowCursor(true);
        
    }
    //but whatever our video mode is into the settings
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FrameMove()
{

	//think as many times as need to

	static int i_smooth_count = 0;
	static float f_delta_save = 0; //time the last 6 frames took?
	static float f_delta = 0;  //time that has passed
	static int i_smooth_active;
   if (!cls_tglo.IsRunning()) return S_OK;
	
	//insert our time into the time array
	g_a_f_delta[i_smooth_count] = cls_tglo.p_d3dapp->m_fElapsedTime; 

	
    if (g_timer_key_check.interval_reached())
    {
        cls_pro.m_i_master_seed = random(100000);

        cls_pro.demake_check(); //another registration check
       
    }
	//in some cases like after a long load we want to reset our frame interpolation

	if (cls_tglo.ResetSmoothing())
	{
		i_smooth_count = 0;
		f_delta_save = 0;
		for (int i=0; i < C_SMOOTHING_FRAMES; i++)
		{
			g_a_f_delta[i] = 0;
		}

		cls_tglo.SetIgnoreNextFrameSmooth(false);

	}

	//get average

	f_delta = 0;
	i_smooth_active = 0;

	for (int i=0; i < C_SMOOTHING_FRAMES; i++)
	{
		f_delta += g_a_f_delta[i];
		if (g_a_f_delta[i]) i_smooth_active++;
	}

	if (f_delta != 0)
	{
		f_delta /= i_smooth_active;
        LONG i_temp =  LONG(f_delta*10000);
 	}

    
	//get ready for next frame

	i_smooth_count++;
	if (i_smooth_count >= C_SMOOTHING_FRAMES) i_smooth_count = 0;
	

	cls_tglo.game_time_update(f_delta);
   		
	while (cls_tglo.game_time_get())
	{

		if (!b_have_run_init)
	{
		if (global_kill_old)
		global_kill_old();	 //run the kill sequence for the last menu
		global_init();	  //start sequence for the new menu/control
		b_have_run_init = true;
	}

	
	// Update user input state
  	
#ifndef _NO_PROTECTION
    if  (cls_pro.m_i_seed == cls_pro.m_i_master_seed) //anti piracy thing
#endif
        cls_input.update();
	
 
#ifndef _NO_PROTECTION
    if (cls_pro.i_max_players != 0)
    cls_things.think();
#endif

#ifdef _EXPIRATION
    if (g_i_expiration_hacked == 0)
    cls_things.think();

#endif
    
    
#ifdef _DEMO    
    if (g_i_demo_hacked == 0)
        cls_things.think();
#endif
    
#if !defined(_DEMO) && defined(_NO_PROTECTION)
	cls_things.think();
	

#endif
    
    //process objects
    
    cls_things.zsort();
    
    //menu handler
    ::global_think();
    }
    
    if (cls_tglo.p_sound) cls_tglo.p_sound->UpdateSounds();
    
    // Sleep(1);
    return S_OK;
}
//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Called once per frame, the call is the entry point for 3d
//       rendering. This function sets up render states, clears the
//       viewport, and renders the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::Render()
{
  // Clear the viewport
   //m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
    //               0x00000000, 1.0f, 0L );
    
    m_pd3dDevice->Clear( 0L, NULL, cls_tglo.p_d3dapp->GetClearFlags(), cls_tglo.p_d3dapp->GetClearColor(), 1.0f, 0L );


    // Begin the scene
    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
    {
   
        
        
     //************* stop the mouse from being laggy ***************   
     IDirect3DSurface8* p_render_target;
        
     HRESULT hr = m_pd3dDevice->GetRenderTarget(&p_render_target);

     if (FAILED(hr))
     {
         log_error("GetRenderTarget: (%s)", DXGetErrorString8(hr));
     }
     
     D3DLOCKED_RECT r;
     RECT rect_area;
     SetRect(&rect_area, 0,0,1,1);
     hr = p_render_target->LockRect(&r, &rect_area, D3DLOCK_READONLY);
     if (FAILED(hr))
     {
         log_error("LockRect: (%s)", DXGetErrorString8(hr));
     }
  
     hr = p_render_target->UnlockRect();
     if (FAILED(hr))
     {
         log_error("UnlockRect: (%s)", DXGetErrorString8(hr));
     }
     
      p_render_target->Release();

      //***********************************************************
      
     if (cls_tglo.p_d3dapp->m_bReady)
        {
            
            cls_things.dx_draw();
        }
        
            //TEST
/*
            RECT rect_ass = {80,0,500,500};
            cls_tglo.p_font->RenderText(0, L"TEST!!!!" , &rect_ass);
    */
  
    // End the scene.
			m_pd3dDevice->EndScene();
    }

    return S_OK;
}


void CMyD3DApplication::OnLoseFocus()
{

    if (cls_tglo.p_sound) cls_tglo.p_sound->SetMute(true);
    ClipCursor(NULL);
    
}

void CMyD3DApplication::OnGetFocus()
{
    
    if (cls_tglo.p_sound) cls_tglo.p_sound->SetMute(false);
    if (!GetApp()->p_d3dapp->IsWindowed())
    {
        
        RECT rcWindow;
        GetWindowRect( m_hWnd, &rcWindow );
        ClipCursor( &rcWindow );
        
    } else
    {
        ClipCursor(NULL);
    }
  
}


//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Overrrides the main WndProc, so the sample can do custom message
//       handling (e.g. processing mouse, keyboard, or menu commands).
//-----------------------------------------------------------------------------
LRESULT CMyD3DApplication::MsgProc( HWND hWnd, UINT msg, WPARAM wParam,
                                    LPARAM lParam )
{
    switch( msg )
    {
        case WM_PAINT:
        {
            if( m_bLoadingApp )
            {
                // Draw on the window tell the user that the app is loading
                // TODO: change as needed
                HDC hDC = GetDC( hWnd );
                TCHAR strMsg[MAX_PATH];
                wsprintf( strMsg, TEXT("Loading... Please wait") );
                RECT rct;
                GetClientRect( hWnd, &rct );
                DrawText( hDC, strMsg, -1, &rct, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
                ReleaseDC( hWnd, hDC );
            }
            break;
        }
	
        case WM_CHAR:
            {
              
                CMessageHeader msg_hdr;
                msg_hdr.i_message = C_THING_MSG_INPUT;
                
                CMessageInput msg_input;

                msg_input.c_key = wParam;
                if (cls_tglo.i_need_wm_messages > 0)
                {
                    cls_tglo.p_thing_list->broadcast_message(&msg_hdr, &msg_input, C_THING_INPUT);
                }


            }
            break;

         case WM_KILLFOCUS:
         
             OnLoseFocus();
             break;


         case WM_SETFOCUS:
             OnGetFocus();
           
         
                    break;

         case WM_ACTIVATEAPP:
             {
                 bool b_lost_focus = ( LOWORD( wParam ) == WA_INACTIVE ) ? true : false;
                 
                 if (b_lost_focus)
                 {
                     OnLoseFocus(); 
                     
                 }    else
                 {
                     OnGetFocus(); 
                     
                   }
             }
             break;
             
         case WM_DESTROY:
#ifdef _BETA_
             if (MessageBox(NULL, "Thanks for beta testing!  After you've played a bit and have a free minute\r\nit would be great if you could fill out a simple questionaire about the game,\r\n\r\nI'll use this info to make the game better.  Would you like to see the questionaire?", "Greetings dear Beta Tester!", MB_OKCANCEL | MB_ICONQUESTION ) == IDOK)
             {
                    char st_temp[MAX_PATH];
                sprintf(st_temp, "www.rtsoft.com/pages/feedback.htm?game=%s&version=%s", app_glo.st_name,app_glo.st_version);
                //this method doesn't work with mozilla?  sucks
               // LaunchURL(st_temp);

                 winall_create_url_file(st_temp);
                 int result = (int)ShellExecute(NULL,"open","temp.url", NULL,NULL, SW_MAXIMIZE);
                 ProcessError(NULL , result, "temp.url",st_temp); 
    
             }
#endif
             break;


   }

//route to our input stuff too
    if (cls_tglo.p_cls_input)
    {
                   cls_tglo.p_cls_input->HandleMouseMessages(hWnd, msg, wParam, lParam);
    }

    return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Invalidates device objects.  
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InvalidateDeviceObjects()
{
    // TODO: Cleanup any objects created in RestoreDeviceObjects()
  	cls_things.dx_invalidate();
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.  
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::DeleteDeviceObjects()
{
    // TODO: Cleanup any objects created in InitDeviceObjects()
   	cls_things.dx_kill();
   return S_OK;
}


//-----------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FinalCleanup()
{
  	if (cls_tglo.p_d3dapp)
    {

        //get latest handle
		cls_tglo.p_d3dDevice = cls_tglo.p_d3dapp->m_pd3dDevice;
        cls_tglo.p_d3dapp->SetShowCursor(true);

	}	// TODO: Perform any final cleanup needed
   
    
      
    g_settings.Save("defaults.dat", true); //load again tis time wit cls_input initted.
    cls_input.de_init();

	cls_things.delete_things_by_type(C_THING_TEXTBOX);
	cls_things.delete_things_by_type(C_THING_TEXTBOX_CHOICE);
	cls_things.delete_things_by_type(C_THING_CPIC);
    cls_things.process_delete_requests();
	cls_things.delete_list();

   
    SAFE_DELETE(cls_tglo.p_ent_lib);
	SAFE_DELETE(cls_tglo.p_script_engine);
	SAFE_DELETE_ARRAY(g_a_f_delta);
  
    SAFE_DELETE(p_cls_sound);
    cls_tglo.p_sound = NULL; //sound is no longer valid
    ClipCursor(NULL);
    

 
	return S_OK;
}

























