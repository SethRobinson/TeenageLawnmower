//-----------------------------------------------------------------------------
// File: CDX
//
// Desc: Controls the engine stuff.  
// Seth A. Robinson
//-----------------------------------------------------------------------------

#include "CDX.h"

#include "..\\..\\..\\shared\\3d\\CFloatCam.h"
#include "..\\..\\..\\shared\\3d\\CEnt.h"
#include "..\\..\\..\\shared\\3d\\CTerrain.h"
#include "..\\..\\..\\shared\\3d\\CManager.h"
#include "..\\..\\..\\shared\\3d\\CThingFont.h"
#include "..\\..\\..\\shared\\profile.h"
#include "..\\..\\..\\shared\\3d\\CThingStats.h"
#include "..\\..\\..\\shared\\custom_time.h"
#include "..\\..\\..\\shared\\3d\\CVisualSky.h"
#include "..\\..\\..\\shared\\3d\\CWeather.h"
#include "..\\..\\..\\shared\\3d\\CGridTile.h"
#include "..\\..\\..\\shared\\3d\\CBuildingManager.h"
#include "..\\..\\..\\shared\\3d\\CEntLib.h"
#include "..\\..\\..\\shared\\3d\\file_Utils.h"
#include "..\\..\\..\\shared\\3d\\CPic.h"
#include "..\\..\\..\\shared\\CScriptEngine.h"
#include "..\\..\\..\\shared\\3d\\CEntItem.h"
#include "..\\..\\..\\shared\\3d\\Script_utils.h"
#include "..\\..\\..\\shared\\Sound\\DX\\gdpSoundManagerDX.h" //DirectMusic sound system
#include "..\\..\\..\\shared\\Sound\\FMOD\\FMSoundManager.h" //FMOD sound system
#include "..\\..\\..\\shared\\Sound\\ISoundManager.h" //interface for the sound system
#include "..\\..\\..\\shared\\3d\\CMessageHeader.h"
#include <direct.h> //for the cd command

//-----------------------------------------------------------------------------
// Global access to the app (needed for the global WndProc())
//-----------------------------------------------------------------------------
CMyD3DApplication* g_pApp  = NULL;
HINSTANCE          g_hInst = NULL;

//-----------------------------------------------------------------------------
// Name: CMyD3DApplication()
// Desc: Application constructor. Sets attributes for the app.
//-----------------------------------------------------------------------------
CMyD3DApplication::CMyD3DApplication()
{
   
	m_hwndRenderWindow     = NULL;
    m_hwndRenderFullScreen = NULL;
    m_hWndTopLevelParent   = NULL;
	
	m_dwCreationWidth           = 200;
    m_dwCreationHeight          = 200;
    m_strWindowTitle            = TEXT( "SWED by Seth A. Robinson - www.rtsoft.com" );
    m_bUseDepthBuffer           = TRUE;
    m_bShowCursorWhenFullscreen = TRUE;


    // Create a D3D font using d3dfont.cpp
    m_bLoadingApp               = TRUE;
  	m_fWorldRotX                = 0.0f;
    m_fWorldRotY                = 0.0f;
 
}




//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------

void SetDefaultKeys()
{
    if (!cls_tglo.p_cls_input)
    {
        log_error("SetDefaultKeys can't operate on null input.");
        return ;
    }
    cls_tglo.p_cls_input->AddAliasBindVirtualKey("f", C_INPUT_FORWARD);
   cls_tglo.p_cls_input->AddAliasBindVirtualKey("v", C_INPUT_BACKWARD);
   cls_tglo.p_cls_input->AddAliasBindVirtualKey("s", C_INPUT_STRAFE_LEFT);
   cls_tglo.p_cls_input->AddAliasBindVirtualKey("d", C_INPUT_STRAFE_RIGHT);
   cls_tglo.p_cls_input->AddAliasBindVirtualKey("right", C_INPUT_RIGHT);
   cls_tglo.p_cls_input->AddAliasBindVirtualKey("left", C_INPUT_LEFT);
   cls_tglo.p_cls_input->AddAliasBindVirtualKey(" ", C_INPUT_JUMP);

}

void load_initial_scene()
{
    
	//let's go ahead and change the default keys for what I like
     SetDefaultKeys();
	
	//cls_tglo.p_map->create_random_height(8, 0.6f, 4, 6, 30);

     cls_tglo.p_sound = new FMSoundManager();

     cls_tglo.p_sound->Init();

	char st_level[256];

	sprintf(st_level, "%s", glo.cls_swed.st_file_name);

	CTerrain *p_terrain = new CTerrain();
	cls_things.add_new_thing(p_terrain);
 	p_terrain->set_map_source(cls_tglo.p_map);
	cls_tglo.p_terrain = p_terrain;
   bool b_loaded = false;

   CEntItem *p_start_script = new CEntItem();
   cls_things.add_new_thing(p_start_script);
   p_start_script->InitScript("script\\Editor\\init.c");
   

   
   
   if (exist(st_level))
    {
        b_loaded = cls_tglo.p_map->load_zone(st_level);
  } else
    {
      //  cls_tglo.p_map->generate_map(20,20,5,3);
      cls_tglo.p_map->send_map_to_terrain();
    }
        

      p_terrain->dx_create();

  	 
	cls_tglo.p_ent_lib->load_category_wildcard("*.ent");

	clear_objects_from_map(&cls_things, ::C_THING_ENT_GENERIC);
    if (exist(st_level))
    {
        
        load_objects_for_map(&cls_things, st_level);
    }

	//move camera to saved position
   if (b_loaded)
    if (cls_tglo.p_camera)
    {
        cls_tglo.p_camera->setObjectPosition(cls_tglo.p_map->vec_camera_pos);
        cls_tglo.p_camera->setAngleX(cls_tglo.p_map->vec_camera_look.x);
        cls_tglo.p_camera->setAngleY(cls_tglo.p_map->vec_camera_look.y);
        cls_tglo.p_camera->setAngleZ(cls_tglo.p_map->vec_camera_look.z);
    }
    

	CWeather *p_weather = new CWeather();
	cls_things.add_new_thing(p_weather);
	p_weather->Init();
	p_weather->LoadWeather(glo.cls_swed.st_weather_file_name);


	//let's load up our test
	
//	CLightning *p_light = new CLightning();
//	cls_things.add_new_thing(p_light);
	//setup what kind of particle system we need
	
 
}


HRESULT CMyD3DApplication::OneTimeSceneInit()
{
    //  perform one time initialization
#ifdef _PROFILE
	  	PROFILE_INIT
#endif
    	randomize();
      
        char st_temp[MAX_PATH];
        
        sprintf(st_temp, "%sdata\\resource.cfl",cls_tglo.GetRootDir());
        
        if (!GetApp()->GetFileLib()->GetCFL()->openLibrary(st_temp))
        {
            LogError("Error opening %s", st_temp);
            assert(0);
        }
        
        //look for a patch for future use...
        sprintf(st_temp, "%spatch.cfl",cls_tglo.GetRootDir());
        
        if (GetApp()->GetFileLib()->GetCFL()->openLibrary(st_temp))
        {
            LogMsg("Patch found and applied.");
        }
        
        
        GetApp()->GetCFL()->setOpenStrategy(CFL_OPEN_BEVAVIOR_DISK_CFL);
        
   
        cls_tglo.p_thing_list = &cls_things;
		cls_tglo.set_running(false);
    // Initialize DirectInput
	cls_input.init(this->m_hWndTopLevelParent);
    cls_tglo.p_cls_input = &cls_input;
	//add our things
	//cls_things.

	
	cls_tglo.p_script_engine = new CScriptEngine();
    cls_tglo.p_script_engine->SetFileLib(GetApp()->GetFileLib());
	add_global_functions_to_script_0(); //add the 3d engine specific functions to the scripting engine


	//make the generic font for statistics and stuff
	CThingFont *p_font = new CThingFont();
	cls_things.add_new_thing(p_font);
	cls_tglo.p_font = p_font;


	CCamera *p_cam = new CCamera();
	cls_things.add_new_thing(p_cam);
	cls_tglo.p_camera = p_cam;
	
 	p_cam->setObjectPosition(D3DXVECTOR3(  20, 14.0f ,20 ));
	p_cam->setAngleY(0); //look towards Z+
	//p_cam->setAngleX(-3);
	
	//load item base classes
	cls_tglo.p_ent_lib = new CEntLib();



	//Let's create the texture manager and put it in our global
	CManager *p_manager = new CManager();
	cls_things.add_new_thing(p_manager);
	cls_tglo.p_manager = p_manager; //put in global
	
    //move to data dir
    if (glo.cls_swed.st_main_path[0] != 0)
    {
        log_msg("Moving to mod dir \"%s\".", glo.cls_swed.st_main_path);
        chdir(glo.cls_swed.st_main_path);
        strcpy(cls_tglo.mod.m_st_path,glo.cls_swed.st_main_path );
     
         }  else
    {
   
    }
    
    
    
    p_manager->load_resource_list("data\\textures.txt");


	CBuildingManager *p_b = new CBuildingManager();
	cls_things.add_new_thing(p_b);


 
  //init map
	
	cls_tglo.p_map = &cls_map;

	cls_tglo.p_map->generate_map(50,50,5,3);
	
	CFloatCam *p_control = new CFloatCam();
	cls_things.add_new_thing(p_control);
   

	//add stats
	CThingStats *p_stats = new CThingStats();
	cls_things.add_new_thing(p_stats);
	p_stats->activate_draw_toggle(DIK_2);

   	
	m_bLoadingApp = FALSE;
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

    // TODO: Perform checks to see if these display caps are acceptable.
    bCapsAcceptable = TRUE;

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
	cls_tglo.p_d3dapp = this;
 
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
 
    /*
    //enable WBuffer if possible
	if ((m_d3dCaps.RasterCaps & D3DPRASTERCAPS_WBUFFER) != 0)
	{
	  //supports W buffer!
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_USEW);
	} else
	{
		//use normal boring Z buffer
	 	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,  D3DZB_TRUE);
	}
*/
	 Material mat;
//	mat.SetDiffuse(1,1,1,1);
  //  mat.SetSpecular(0.1f,0.1f,0.1f,0.1f);
    //mat.SetAmbient(0,0,0,0);
    //mat.SetPower(1);
    
    mat.Use(m_pd3dDevice);

	
		cls_tglo.p_d3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, true );	


	// Set up the default texture states
    m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	  // Turn off culling, so we see the front and back of the Terrain.
//    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,     D3DCULL_NONE );

	FLOAT fAspect = m_d3dsdBackBuffer.Width / (FLOAT)m_d3dsdBackBuffer.Height;

	cls_tglo.p_camera->perspective( D3DX_PI/4, fAspect, 0.6f, 600 );


    // Set up lighting states
    D3DLIGHT8 light;
    //D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, -1.0f, -1.0f, 2.0f );
    
	D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, 0.7f, -1, 0.0f );
	 light.Specular.a = 1;
	 light.Specular.r = 1;
	 light.Specular.g = 1;
	 light.Specular.b = 1;
	 light.Attenuation0 = 1.0f;
	 light.Ambient.a = 0.6f;
	 light.Ambient.r = 0.6f;
	 light.Ambient.g = 0.6f;
	 light.Ambient.b = 0.6f;

	 // light.Falloff = 3;
	
	m_pd3dDevice->SetLight( 0, &light );
    m_pd3dDevice->LightEnable( 0, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    m_pd3dDevice->SetRenderState(D3DRS_AMBIENT,0x33333333);
  
		 m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
//    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,     D3DCULL_NONE );
 	cls_things.dx_restore();

    return S_OK;
}

HRESULT CMyD3DApplication::CheckForLostFullscreen()
{
    HRESULT hr;

    if( m_bWindowed )
        return S_OK;

    if( FAILED( hr = m_pd3dDevice->TestCooperativeLevel() ) )
        ForceWindowed();

    return S_OK;
}

HRESULT CMyD3DApplication::AdjustWindowForChange()
{
    if( m_bWindowed )
    {
        if (m_hwndRenderFullScreen)
        ::ShowWindow( m_hwndRenderFullScreen, SW_HIDE );
        CD3DApplication::m_hWnd = m_hwndRenderWindow;
    }
    else
    {
        if( ::IsIconic( m_hwndRenderFullScreen ) )
            ::ShowWindow( m_hwndRenderFullScreen, SW_RESTORE );
        ::ShowWindow( m_hwndRenderFullScreen, SW_SHOW );
        CD3DApplication::m_hWnd = m_hwndRenderFullScreen;
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FrameMove()
{
 
	  //average framerate is less than 30, let's use quicker stuff
	  
//	cls_tglo.set_simulation_speed(1.0f/cls_tglo.p_d3dapp->m_fFPS);
	if (cls_tglo.p_d3dapp->m_fElapsedTime > 1)
	{
  	//it must have been paused...
	cls_tglo.game_time_update(0.01f);
	} else
	{
	//think as many times as need to
	cls_tglo.game_time_update(cls_tglo.p_d3dapp->m_fElapsedTime);
  	} 

 #ifdef _PROFILE
	
		PROFILE_BEGIN("Main Loop");
 		PROFILE_BEGIN("UpdateFrame");
     #endif


	while (cls_tglo.game_time_get())
	{
	
	// Update user input state
   
 // 	PROFILE_BEGIN("Input");
    cls_input.update();
  //	PROFILE_END("Input");
  
   //process objects
  //	PROFILE_BEGIN("ZSort");
 
  //	PROFILE_END("ZSort");

  	PROFILE_BEGIN("AI");

 	cls_things.think();
  	cls_things.zsort();
  
    PROFILE_END("AI");
 
  }
      if (cls_tglo.p_sound) cls_tglo.p_sound->UpdateSounds();


			PROFILE_END("UpdateFrame");
   
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
   // m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
   //                      0x000000ff, 1.0f, 0L );
     m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );

   


    // Begin the scene
    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
    {
    #ifdef _PROFILE
	PROFILE_BEGIN("Render");
     #endif

		cls_things.dx_draw();
		RenderText();

    #ifdef _PROFILE
	PROFILE_END("Render");
		PROFILE_END("Main Loop");
	#endif
	  #ifdef _PROFILE 
	PROFILE_TALLY
	  #endif

        // End the scene.
        m_pd3dDevice->EndScene();
	}

      if (cls_tglo.p_sound) cls_tglo.p_sound->UpdateSounds();



    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RenderText()
// Desc: Renders stats and help text to the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::RenderText()
{
  
    return S_OK;
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
    /*
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
        */

    case WM_ENGINE_MESSAGE:
        ProcessWindowsEngineMessage(wParam, (CEngineMessage *)lParam);
     
        break;
    default:
     break;  
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
    // TODO: Perform any final cleanup needed
	if (cls_tglo.p_d3dapp)
	{
		//get latest handle
		cls_tglo.p_d3dDevice = cls_tglo.p_d3dapp->m_pd3dDevice;
	}
	cls_input.de_init();
    // Write the settings to the registry
	cls_things.delete_things_by_type(C_THING_CPIC);
	cls_things.delete_things_by_type(C_THING_TEXTBOX);
	cls_things.process_delete_requests();

	cls_things.delete_list(); 

    if (cls_tglo.p_sound) cls_tglo.p_sound->Kill();

	SAFE_DELETE(cls_tglo.p_ent_lib);
 	SAFE_DELETE(cls_tglo.p_script_engine);
    SAFE_DELETE(cls_tglo.p_sound);
	return S_OK;
}






