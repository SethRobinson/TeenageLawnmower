/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CWeather.h"
#include "CVisualSky.h"
#include "CThingList.h"
#include "CThing_Globals.h"
#include "d3dutil.h"
#include "CSkyBox.h"
#include "CThing_util.h"
#include "..\\CScriptEngine.h"
#include "CEntItem.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CWeather::CWeather()
{
	m_p_sky = NULL;
	m_p_skybox = NULL;
	set_thing_name("Weather"); //so it can be found easy later
    cls_tglo.p_weather = this;
    m_b_set_to_hold_active = false;
    m_b_fade_to_hold = false;
    m_i_script_ent = -1;
}    

CWeather::~CWeather()
{
  
	if (!p_my_thing_list->b_deleting_list)
	{
    
        KillWeatherScriptIfActive();

		
		if (m_p_sky)
		{
			m_p_sky->b_delete_flag = true;
		    m_p_sky = NULL;
		}
		
		if (m_p_skybox)
		{
			m_p_skybox->b_delete_flag = true;
			m_p_skybox = NULL;
			
		}
	}
 cls_tglo.p_weather = NULL;
}
void CWeather::dx_restore()
{
   
 	 UpdateLights();

}

void CWeather::SetRandomFlash( int i_time)
{
    if (!m_b_set_to_hold_active)
    {
        //copy our master settings to a temp buff so we remember what it was originally when
        //the effect is done
        GetWeatherSettings(&m_weather_hold);
        //pa
    
    }
   
    //change the settings we want


 //   m_weather.vec_light_dir.x = frandom(1);
 //   m_weather.vec_light_dir.y = frandom(1);
 //   m_weather.vec_light_dir.z = frandom(1);

    m_weather.vec_ambient_light.x = frandom_range(0.9f,1);
    m_weather.vec_ambient_light.y = frandom_range(0.9f,1);
    m_weather.vec_ambient_light.z = frandom_range(0.9f,1);

    UpdateLights();

    SetDelayToSetToHold(i_time);

}

void CWeather::SetDelayToSetToHold(int i_time)
{
    m_b_set_to_hold_active = true;
    m_timer_set_to_hold_wait.set_interval(i_time);

}

void CWeather::ProcessDelayToSetToHoldTimer()
{
    if (!m_b_set_to_hold_active) return;
    
    //should we switch back to the old settings and done with our effect?
    if (m_timer_set_to_hold_wait.interval_reached())
    {
        UpdateFromWeatherSetting(&m_weather_hold, false); //don't update script too
        m_b_set_to_hold_active = false;
        m_b_fade_to_hold = false; //turn off any fading that was happening too
    }

}

void CWeather::UpdateLights()
{

	  if ( m_weather.vec_light_color.x != 0 
         && m_weather.vec_light_color.y != 0
         && m_weather.vec_light_color.z != 0
          )
      {
      
	 D3DUtil_InitLight( m_light, D3DLIGHT_DIRECTIONAL, m_weather.vec_light_dir.x,
		  m_weather.vec_light_dir.y,  m_weather.vec_light_dir.z);
	 m_light.Specular.a = 1;
	 m_light.Specular.r = 1;
	 m_light.Specular.g = 1;
	 m_light.Specular.b = 1;
	 m_light.Attenuation0 = 1.0f;
	 m_light.Ambient.a = 0.0f;
	 m_light.Diffuse.a = 1; //does this matter?  I don't know
	 m_light.Diffuse.r = m_weather.vec_light_color.x;
	 m_light.Diffuse.g = m_weather.vec_light_color.y;
	 m_light.Diffuse.b = m_weather.vec_light_color.z;
	 
	
	cls_tglo.p_d3dDevice->SetLight( 0, &m_light );
    cls_tglo.p_d3dDevice->LightEnable( 0, TRUE );
      } else
      {
    
   /*       
          	 D3DUtil_InitLight( m_light, D3DLIGHT_POINT, m_weather.vec_light_dir.x,
		  m_weather.vec_light_dir.y,  m_weather.vec_light_dir.z);
	 m_light.Specular.a = 1;
	 m_light.Specular.r = 1;
	 m_light.Specular.g = 1;
	 m_light.Specular.b = 1;
	 m_light.Attenuation0 = 1.0f;
	 m_light.Ambient.a = 0.0f;
	 m_light.Diffuse.a = 1; //does this matter?  I don't know
	 m_light.Diffuse.r = 1.0f;
	 m_light.Diffuse.g = 1.0f;
	 m_light.Diffuse.b = 1.0f;
	 m_light.Position = D3DXVECTOR3(75,2,20);
     m_light.Range = 35;

	
	cls_tglo.p_d3dDevice->SetLight( 0, &m_light );
    cls_tglo.p_d3dDevice->LightEnable( 0, TRUE );
  	 m_light.Position = D3DXVECTOR3(130,2.5f,26);
   
    cls_tglo.p_d3dDevice->SetLight( 1, &m_light );
    cls_tglo.p_d3dDevice->LightEnable( 1, TRUE );


  	 m_light.Position = D3DXVECTOR3(130,2.5f,36);
   
    cls_tglo.p_d3dDevice->SetLight( 2, &m_light );
    cls_tglo.p_d3dDevice->LightEnable( 2, TRUE );



          LogMsg("Light disabled.");
        */
        cls_tglo.p_d3dDevice->LightEnable( 0, FALSE );
  
        }
    
    cls_tglo.p_d3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    cls_tglo.p_d3dDevice->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_COLORVALUE(m_weather.vec_ambient_light.x,
		m_weather.vec_ambient_light.y, m_weather.vec_ambient_light.z, 1));


	//also setup fog
  if (m_weather.f_fog_end != 0)
  {
  
    SetupPixelFog(D3DCOLOR_COLORVALUE(m_weather.vec_fog_color.x,
		m_weather.vec_fog_color.y,m_weather.vec_fog_color.z,1),
		D3DFOG_LINEAR,
		m_weather.f_fog_start, m_weather.f_fog_end);
  } else
  {
	  //turn off fog if it was on
	 cls_tglo.p_d3dDevice->SetRenderState(D3DRS_FOGENABLE, false);
  }


}

bool CWeather::LoadWeather(char *p_file)
{
   if (!m_weather.Load(p_file))
   {
	   return false;
   }

   //loaded it, now set it.
   UpdateFromWeatherSetting(&m_weather, true);
   
   return true;
}

bool CWeather::LoadAndFadeToWeather(char *p_file, int i_delay, float f_lerp)
{
   if (!m_weather_hold.Load(p_file))
   {
       return false;
   }

   //loaded it in to hold.  Let's set the fade to be active
   this->SetDelayToSetToHold(i_delay);
   //Also activate the fade
   m_b_fade_to_hold = true;
   m_f_fade_lerp = f_lerp;
   return true;
}


void CWeather::ProcessFade()
{
    if (!m_b_fade_to_hold) return;
    float f_lerp_speed = m_f_fade_lerp * cls_tglo.time();

    m_weather.f_fog_start = lerp_float(m_weather.f_fog_start, m_weather_hold.f_fog_start, f_lerp_speed);
    m_weather.f_fog_end = lerp_float(m_weather.f_fog_end, m_weather_hold.f_fog_end, f_lerp_speed);
    
    D3DXVec3Lerp(&m_weather.vec_fog_color, &m_weather.vec_fog_color, &m_weather_hold.vec_fog_color, f_lerp_speed);
    D3DXVec3Lerp(&m_weather.vec_ambient_light, &m_weather.vec_ambient_light, &m_weather_hold.vec_ambient_light, f_lerp_speed);
    D3DXVec3Lerp(&m_weather.vec_light_color  , &m_weather.vec_light_color, &m_weather_hold.vec_light_color, f_lerp_speed);
    D3DXVec3Lerp(&m_weather.vec_light_dir  , &m_weather.vec_light_dir , &m_weather_hold.vec_light_dir , f_lerp_speed);

    D3DXVec3Lerp(&m_weather.vec_skydome_size, &m_weather.vec_skydome_size , &m_weather_hold.vec_skydome_size, f_lerp_speed);

    
    m_weather.f_skydome_height_offset = lerp_float(m_weather.f_skydome_height_offset, m_weather_hold.f_skydome_height_offset, f_lerp_speed);


    //fade the weather a bit
    UpdateLights();

}

void CWeather::GetWeatherSettings(CWeatherSetting * p_out_weather)
{
	memcpy(p_out_weather, &m_weather, sizeof(CWeatherSetting));
}


void CWeather::KillWeatherScriptIfActive()
{
    if (m_i_script_ent != -1)
    {
        //find item and kill it
        CEntItem *p_ent = (CEntItem*) p_my_thing_list->get_thing_by_id(m_i_script_ent);
        if (!p_ent)
        {
            log_error("Unable to kill weather script, already dead?");

        }   else
        {
           p_ent->b_delete_flag = true;
        }

        m_i_script_ent = -1;
    }
    
}
void CWeather::UpdateFromWeatherSetting(CWeatherSetting *p_weather, bool b_update_script)
{

	//before we copy it, let's compare it what we already had

	bool b_textures_changed = false;

	for (int i = 0; i < C_SKYBOX_SIDES; i++)
	{
		if (strcmp(p_weather->a_st_tex[i], m_weather.a_st_tex[i]) != 0)
		{
			//not the same!
			b_textures_changed = true;
			break;
		}
	}
	
	
	if (p_weather != &m_weather)
	{
		//copy it to our internal copy
		memcpy(&m_weather, p_weather, sizeof(CWeatherSetting));
	}


	//init our skybox with this info
	if (*p_weather->a_st_tex[0] != 0)
	{
		//a skybox exists.
		if (!m_p_skybox)
		{
		    b_textures_changed = true;
			//init it if needed	
			m_p_skybox = new CSkyBox();
			p_my_thing_list->add_new_thing(m_p_skybox);
		}
		

		if (b_textures_changed)
		{
			
			m_p_skybox->InitTextures(
				p_weather->a_st_tex[C_SKYBOX_LEFT],
				p_weather->a_st_tex[C_SKYBOX_RIGHT],
				p_weather->a_st_tex[C_SKYBOX_BACK],
				p_weather->a_st_tex[C_SKYBOX_FRONT],
				p_weather->a_st_tex[C_SKYBOX_TOP],
				p_weather->a_st_tex[C_SKYBOX_BOTTOM]);
		}
		m_p_skybox->SetWallSize(p_weather->f_skybox_side_size);
		
	} else
	{
		//no skybox is needed. kill it if it exists.
	 		if (m_p_skybox)
		{
			m_p_skybox->b_delete_flag = true;
			m_p_skybox = NULL;
			
		}

	}

  
	if (m_p_sky)
	{
	
		if (p_weather->st_skydome_tex[0] == 0)
		{
		m_p_sky->b_delete_flag = true;
		m_p_sky = NULL;
		}
	}



	if (p_weather->st_skydome_tex[0])
	{
		if (!m_p_sky)
		{
		  m_p_sky = new CVisualSky();
		  p_my_thing_list->add_new_thing(m_p_sky);
		  m_p_sky->Init(p_weather->st_skydome_tex);
		}

	   	//not implemented yet
		//  m_p_sky->UpdateTexture(p_weather->st_skydome_tex);
		m_p_sky->set_scale(p_weather->vec_skydome_size.x, p_weather->vec_skydome_size.y, p_weather->vec_skydome_size.z);
		m_p_sky->translateMovement();
		m_p_sky->set_no_lighting(p_weather->b_skydome_disable_lighting);
		m_p_sky->set_visual_follow_offset(D3DXVECTOR3(0,p_weather->f_skydome_height_offset,0));
	}

	
  	if (cls_tglo.IsRunning())
	{
		
	    if (m_p_sky)	
		m_p_sky->dx_create();

	    UpdateLights();

	}

   if (b_update_script)
   {
   

    //handle the script
    KillWeatherScriptIfActive();
       

    if (p_weather->st_script[0] != 0)
    {
        //script exists, let's activate it
        CEntItem *p_new = new CEntItem();
        p_new->set_thing_name("WeatherScript");
        
        p_my_thing_list->add_new_thing(p_new);
	    p_new->InitScript(p_weather->st_script);
        m_i_script_ent = p_new->get_id();
  }
   }

}

void CWeather::Init()
{
	//get our weather stuff up and functioning
	if (m_p_sky)
	{
		//kill the old sky if it existed
		m_p_sky->b_delete_flag = true;
	    m_p_sky = NULL; //we don't need to remember it anymore
	}

	if (m_p_skybox)
	{

		//kill the old sky if it existed
		m_p_skybox->b_delete_flag = true;
	    m_p_skybox = NULL; //we don't need to remember it anymore
	}

	if (cls_tglo.IsRunning())
	{
		
	    if (m_p_sky)	
		m_p_sky->dx_create();

	
	}

    UpdateLights();


}


void CWeather::think()
{
    ProcessFade();
    ProcessDelayToSetToHoldTimer();
 
}
