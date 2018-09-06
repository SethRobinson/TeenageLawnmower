//******************************************
// copyright 2002	Seth A. Robinson

// This file is NOT public domain and may not be shared, distributed or used
// without specific permission in any way.

// seth@rtsoft.com - www.rtsoft.com
//******************************************

#include "CEntMower.h"
#include "3d\\CThing_Util.h"
#include "3d\\CEntItem.h"

#include "3d\\CManager.h"
#include "3d\\CMap.h" 
#include "3d\\CCamera.h"
#include "CInput.h"
#include "3d\\CTerrain.h"
#include "sound\\ISoundManager.h"
#include "3d\\CParticleEmitter.h"
#include "3d\\CThinglist.h"
#include "3d\\CSubManager.h"
#include "3d\\CMessageHeader.h"
#include "3d\\ent_utils.h"
#include "3d\\CPicMeter.h"
#include "CScriptEngine.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int C_JOYSTICK_SMOOTHING = 150;
const int C_JOYSTICK_SMOOTH_INITIAL_DELAY = 400;

const int C_MOW_NO_COMMAND = 0;
const int C_MOW_SWITCH_TO_IDLE = 1;
const int C_MOW_PLAY_STARTUP = 2;
const int C_MOW_STOP = 3; 
const int C_MOW_TURNING_OFF = 4;
const int C_MOW_MOWING = 5;
         
#define C_WEED_KILL_BUTTON_DELAY_MS 1000


CEntMower::CEntMower()
{
//	this->init_visual("model\\test\\ferarri.x", ::C_RESOURCE_PMESH);
	this->init_visual("model\\lawnmower2.x", ::C_RESOURCE_PMESH);
    this->p_clippings = NULL;
	this->p_smoke = NULL;
	m_b_joy_up_timer_active = false;
    m_f_percent_mowed = 0;
	this->m_i_total_zone_squares = 1;
    m_i_clipper_id = -1;
    m_i_smoke_id = -1;
    p_fire = NULL;
    h_fire_sound = C_SOUND_NONE;
    p_damage_smoke = NULL;
    SetMowedTile(50);
	set_desired_velocity(0);
   	setObjectPosition(D3DXVECTOR3(frandom(cls_tglo.p_map->get_size_x()*cls_tglo.p_map->get_block_size()),
		10,frandom(cls_tglo.p_map->get_size_y()*cls_tglo.p_map->get_block_size())));
	this->set_follow_ground_tilt(true); //rotate to match the ground angle
	this->set_scale_uniform(2.0f);
	f_bounce_power = 0.30f;
    set_thing_name("PLAYER");
    b_rolling = false;
    b_no_render_next_frame = false;
    m_b_exploded = false;
    m_b_joy_held_up = false;
	m_b_joy_held_down = false;
	m_i_last_touched_down = 0;
	m_i_last_touched_up = 0;

    if (!cls_tglo.p_camera)
    {
        log_error("Can't attach camera to player, not created yet");
    } else
    {
        cls_tglo.p_camera->set_visual_follow(this); //camera will follow us
        //the x is distance from object and the 3 is height in relation to object
        i_view = 1; //default view
        set_mower_view();
    }
    
  this->set_tilt_lerp(6); //extra smooth because a camera is attached
 this->f_accel = 100;  //instant
 this->set_type(C_THING_CONTROL);
 SetTurnLerp(15);
 
 //preload sounds and set distance settings

 h_mow = cls_tglo.p_sound->SoundInstanceCreateAndLoad3D(C_SOUND_NONE,CDir("sound\\mow_stop1.wav"));
 cls_tglo.p_sound->SoundInstanceSetStayAlive(h_mow, true); //won't auto destroy itself
 cls_tglo.p_sound->SoundInstance3DSetDistance(h_mow, 25, 0.0f);
 

 h_mow = cls_tglo.p_sound->SoundInstanceCreateAndLoad3D(h_mow,CDir("sound\\mow_loop1.wav"));
 h_mow = cls_tglo.p_sound->SoundInstanceCreateAndLoad3D(h_mow,CDir("sound\\pop.wav"));
 h_mow = cls_tglo.p_sound->SoundInstanceCreateAndLoad3D(h_mow,CDir("sound\\fall.wav"));
 h_mow = cls_tglo.p_sound->SoundInstanceCreateAndLoad3D(h_mow,CDir("sound\\drop.wav"));
  

 //play our starting sound too
 h_mow = cls_tglo.p_sound->SoundInstanceCreateAndLoad3D(h_mow,CDir("sound\\mow_start1.wav"));
 
 h_roll = cls_tglo.p_sound->SoundInstanceCreateAndLoad3D(C_SOUND_NONE,CDir("sound\\roll.wav"));
 cls_tglo.p_sound->SoundInstanceSetStayAlive(h_roll, true); //won't auto destroy itself
  
 //make mower louder from farther away
 
 cls_tglo.p_sound->SoundInstance3DSetDistance(h_roll, 25, 0.0f);
 
     //	i_sound_mode = C_MOW_NO_COMMAND;
    i_sound_mode = C_MOW_PLAY_STARTUP;
    
   //let's enable our smoke and clipping things

	//let' get the index of our mow zone
	m_i_mow_zone = cls_tglo.p_map->p_sub_manager->GetSubZoneByName("Mow");
	m_i_squares_mowed = 0; //have mowed none so far
	if (m_i_mow_zone == -1)
	{
		log_error("Unable to find a zone labeled Mow for this level, won't mow anything.");

	} else
	{
        //ok, we know the zone, but we want to count how many of these suckers there are.
        m_i_total_zone_squares 	= cls_tglo.p_map->p_sub_manager->p_a_sub_zone[m_i_mow_zone].i_zone_squares;
        log_msg("Squares to mow:  %d.", m_i_total_zone_squares );
    }

  cls_tglo.p_player = this; //put this memory address in a global
  m_i_alpha_sort =C_VISUAL_ALPHA_SORT_YES;

  //SetPhysics(C_ENT_PHYSICS_ACCURATE);
  SetShadowMode(C_VISUAL_SHADOW_ACCURATE);
  cls_tglo.p_camera->SetTiltFollowHeightMod(0.5f);
 
   
  m_f_turn_sensitivity = 2.3f;
  SetMowerSpeed(-100); //-1 means "set to default"

  SetHealth(1);
  SetDrunk(0); //not drunk
  SetHeightBlowUp(false, 0);
  SetBroadCastType(C_ENT_BROADCAST_MOVEMENT);
   
  BuildRadius(0.1f); //small
    //SetRadiusMod(0.001f); //tiny        
  SetSphereTreeMod(0.33f);
  SetPhysics(C_ENT_PHYSICS_SIMPLE); //so we can't jump over walls
  m_i_input_delay = GetTickCount()+C_WEED_KILL_BUTTON_DELAY_MS;
}

void CEntMower::SetHeightBlowUp(bool b_active, float f_height)
{
    m_f_height_check = f_height;
    m_b_height_check_active = b_active;

}

void CEntMower::SetMowerSpeed(float f_speed)
{
    if (f_speed <= -100)
    {
        //just set it to default
        m_f_mower_speed = 20;
    } else
    {
        m_f_mower_speed = f_speed;
    }

}
void CEntMower::SetMowedTile(int i_new)
{
    m_i_mowed_tile = i_new;
}


void CEntMower::SetHealth(float f_health)
{
    //are we damaged?  Turn on/off extra smoke and speed modifications.

    m_f_health = f_health;
    
    if (m_f_health < 0.7f)
    {
        if (!p_damage_smoke)
        {

            p_damage_smoke = new CParticleEmitter();
            p_my_thing_list->add_new_thing(p_damage_smoke);
            //setup what kind of particle system we need
            p_damage_smoke->load_and_compile_particle_system("script\\damage_smoke.pss");
           // p_damage_smoke->set_apply_momentum(true); //smoke moves with mower
           // p_damage_smoke->SetMomentumMod(4000); //fire should only partially be moved by momentum
            //attach to lawn mower
            p_damage_smoke->set_visual_follow(this);
            p_damage_smoke->dx_create();
    
        }  else
        {
            //just turn it on
        }

    }   else
    {
        if (p_damage_smoke)
        {
            //we shouldn't be smoking.  health is high.
            p_damage_smoke->set_no_new_particles(true);

        }

    }
    
    if (m_f_health < 0.4f)
    {
    
        if (!p_fire)
        {
            p_fire = new CParticleEmitter();
            p_my_thing_list->add_new_thing(p_fire);
            //setup what kind of particle system we need
            p_fire->load_and_compile_particle_system("script\\fire_mower.pss");
           // p_fire->set_apply_momentum(true); //smoke moves with mower
           // p_fire->SetMomentumMod(4000); //fire should only partially be moved by momentum
            //attach to lawn mower
            p_fire->set_visual_follow(this);
            p_fire->dx_create();
     
            //let's also attach a fire sound

           if (h_fire_sound != C_SOUND_NONE)
           {
               cls_tglo.p_sound->SoundInstanceKill(h_fire_sound);
               h_fire_sound = C_SOUND_NONE;

           }

           h_fire_sound =  Sound3DPlay(&m_vObjectPosition, "sound\\fire.wav", 99999); //get it looping
            cls_tglo.p_sound->SoundInstance3DSetDistance(h_fire_sound, 25, 0.0f);
 

        } else
        {
            //get the fire going again
              if (h_fire_sound != C_SOUND_NONE)
           {
               cls_tglo.p_sound->SoundInstanceKill(h_fire_sound);
                h_fire_sound = C_SOUND_NONE;
              }

           h_fire_sound =  Sound3DPlay(&m_vObjectPosition, "sound\\fire.wav", 99999); //get it looping
            cls_tglo.p_sound->SoundInstance3DSetDistance(h_fire_sound, 25, 0.0f);
 
     p_fire->set_no_new_particles(false);

        }
    }  else
    {
        //shouldn't be flaming
        if (p_fire)
        {
          p_fire->set_no_new_particles(true);

        }
        
        //kill fire sound if it was here
        if (h_fire_sound != C_SOUND_NONE)
        {
            cls_tglo.p_sound->SoundInstanceKill(h_fire_sound);
            h_fire_sound = C_SOUND_NONE;
        }
        
        
    }

   if (m_f_health == 0)
   {
       //explode sound effect (should we limit it to 1 max?)
       if (!m_b_exploded)
       {
       
           Sound3DPlay(&m_vObjectPosition, "sound\\explode.wav", 1);
           m_b_exploded = false;
           
       }
       
   }


}

void CEntMower::EnableClippings(char *st_name)
{
    if (p_clippings)
    {
        //remove old system

        if (p_clippings)
        {
            p_clippings->set_visual_follow(NULL);
            p_clippings->b_delete_flag = true;
            p_clippings = NULL;
        }
    }

    		//particle system
	p_clippings = new CParticleEmitter();
	m_i_clipper_id = this->p_my_thing_list->add_new_thing(p_clippings);
	//setup what kind of particle system we need
	p_clippings->load_and_compile_particle_system(st_name);  //name of particle system to use
   	//attach to lawn mower
	p_clippings->set_visual_follow(this);
	p_clippings->dx_create();
    p_clippings->set_no_new_particles(true);
  
}

void CEntMower::enable_smoke()
{
	p_smoke = new CParticleEmitter();
	m_i_smoke_id = this->p_my_thing_list->add_new_thing(p_smoke);
	//setup what kind of particle system we need
	p_smoke->load_and_compile_particle_system("script\\smoke.pss");
   	p_smoke->set_apply_momentum(true); //smoke moves with mower
	p_smoke->SetMomentumMod(4000); //smoke should only partially be moved by momentum
	//attach to lawn mower
	p_smoke->set_visual_follow(this);
	p_smoke->dx_create();
	//don't let them do anything now
	p_smoke->set_no_new_particles(true);
  
}

void CEntMower::KillDamageSmoke()
{
    if (p_damage_smoke)
    {
        p_damage_smoke->set_visual_follow(NULL);
        p_damage_smoke->b_delete_flag = true;
        p_damage_smoke = NULL;   
    }
    
    
}      

void CEntMower::KillFire()
{
    
    if (p_fire)
    {
        p_fire->set_visual_follow(NULL);
        p_fire->b_delete_flag = true;
        p_fire = NULL;
    }
    
    
}

CEntMower::~CEntMower()
{
 	
	if (cls_tglo.p_camera)
	{
	  cls_tglo.p_camera->set_visual_follow(NULL); //no longer follow us, we're done
	}
    
    if (!p_my_thing_list->b_deleting_list)
    {
        
        //kill our particle systems we were using
        if (p_smoke)
        {
            p_smoke->set_visual_follow(NULL);
            p_smoke->b_delete_flag = true;
        }
        if (p_clippings)
        {
            p_clippings->set_visual_follow(NULL);
            p_clippings->b_delete_flag = true;
        }
    
        KillFire();
        KillDamageSmoke();
    
    }

    
    //kill our two sounds
    if (cls_tglo.p_sound)
    {
    
    cls_tglo.p_sound->SoundInstanceKill(h_mow);
	cls_tglo.p_sound->SoundInstanceKill(h_roll);

    if (h_fire_sound != C_SOUND_NONE)
    {
        cls_tglo.p_sound->SoundInstanceKill(h_fire_sound);
        h_fire_sound = C_SOUND_NONE;
    }
    } 
    cls_tglo.p_camera->DontLookAtMe(this); //if a camera is looking at us, stop it.

	//it's possible there is a particle system out there connected to us.  If so, let's kill it
	EntKillConnectedPS(this->p_my_thing_list, this); //kill all particle systems connected to us
	  cls_tglo.p_player = NULL; //put this memory address in a global

}

void CEntMower::ToggleCameraFreeze()
{
	if (cls_tglo.p_camera->get_visual_follow() == NULL)
	{

	  cls_tglo.p_camera->set_visual_follow(this); //camera will follow us
 	 cls_tglo.p_camera->SetVisualLook(NULL);
	} else
	{
	
		cls_tglo.p_camera->set_visual_follow(NULL);
		//but let's also stare at the mower
		cls_tglo.p_camera->SetVisualLook(this);
	}


}

void CEntMower::set_mower_view()
{
 CThing *p_was_in_follow_mode = cls_tglo.p_camera->get_visual_follow();


	
	cls_tglo.p_camera->set_visual_follow(this); //camera will follow us
    cls_tglo.p_camera->SetVisualLook(this); //follow us
    cls_tglo.p_camera->SetSmartFadeActive(true);

 
 if (i_view == 0)
	{
    cls_tglo.p_camera->SetSmartFadeActive(false); //from this view we really don't need everything to fade
		
	cls_tglo.p_camera->set_visual_follow_offset(D3DXVECTOR3(0,0.6f,0));
       cls_tglo.p_camera->SetVisualLook(false); //follow us
		this->b_no_render_next_frame = true;
	   if (p_was_in_follow_mode)
	   {
		   //set a waiting period befre switching from third person view
	   		cls_tglo.p_camera->set_third_person_view(true);
   			cls_tglo.p_camera->TurnOffThirdPersonViewByTimer(200);
	   } else
	   {
		   		cls_tglo.p_camera->set_third_person_view(false);
 	   }

	}
	if (i_view == 1)
	{
		
		cls_tglo.p_camera->set_visual_follow_offset(D3DXVECTOR3(-20,10,0.25f));
		cls_tglo.p_camera->set_third_person_view(true);
		this->b_no_render_next_frame = false;

	}
	
		if (i_view == 2)
	{
		
		cls_tglo.p_camera->set_visual_follow_offset(D3DXVECTOR3(-40,25,0.50f));
		cls_tglo.p_camera->set_third_person_view(true);
		this->b_no_render_next_frame = false;

	}
		if (i_view == 3)
	{
		
			//standard follow cam
			this->b_no_render_next_frame = true;
			cls_tglo.p_camera->set_third_person_view(false);
	}

}

void CEntMower::process_sound()
{
    
    if (i_sound_mode == C_MOW_PLAY_STARTUP)
   {
	 //initial rev
		cls_tglo.p_sound->SoundInstanceStop(h_mow);
  		h_mow = cls_tglo.p_sound->SoundInstanceCreateAndLoad3D(h_mow, CDir("sound\\mow_start1.wav"));
		cls_tglo.p_sound->SoundInstancePlay(h_mow, 1);
	    timer_sound.reset();
		timer_sound.set_interval(2700);
		i_sound_mode = C_MOW_SWITCH_TO_IDLE;
		if (p_smoke)
		p_smoke->set_no_new_particles(false);

   }

	if (i_sound_mode == C_MOW_SWITCH_TO_IDLE)
	{
	if (timer_sound.interval_reached())
	  {
		//switch to idle
		i_sound_mode = C_MOW_MOWING;
		cls_tglo.p_sound->SoundInstanceStop(h_mow);
		h_mow = cls_tglo.p_sound->SoundInstanceCreateAndLoad3D(h_mow, CDir("sound\\mow_loop1.wav"));
	 	cls_tglo.p_sound->SoundInstancePlay(h_mow, 9999);
	   
	}
	}
	if (i_sound_mode == C_MOW_STOP)
	{

	  //turn the mower off
	 		i_sound_mode = C_MOW_TURNING_OFF;
			timer_sound.reset();
		timer_sound.set_interval(1000); //can't do anything during this time
	    cls_tglo.p_sound->SoundInstanceStop(h_mow);
		h_mow = cls_tglo.p_sound->SoundInstanceCreateAndLoad3D(h_mow, CDir("sound\\mow_stop1.wav"));
	 	cls_tglo.p_sound->SoundInstancePlay(h_mow, 1);

	}
	if (i_sound_mode == C_MOW_TURNING_OFF)
	{
	  if (timer_sound.interval_reached())
	  {
	      //mower is now completely off
		  i_sound_mode = C_MOW_NO_COMMAND;
	      if (p_smoke)
		  p_smoke->set_no_new_particles(true);
	  }

	}

}

void CEntMower::process_keys()
{


	//change views?
	if (
        (cls_tglo.p_cls_input->is_key_tapped(DIK_TAB))
        || (cls_tglo.p_cls_input->GetAnyJoystickButtonTapped(5))
        )
	{
		this->i_view++;
		if (i_view == 3) i_view = 0;
		set_mower_view();
	}
	
    if (cls_tglo.p_cls_input->is_key_tapped(DIK_L))
    {
        ToggleCameraFreeze();
    }
    
	
	//first make sure the mower isn't in the middle of turning on or off
	if (i_sound_mode != C_MOW_TURNING_OFF)
	{
		if (i_sound_mode == C_MOW_NO_COMMAND)
		{
			if (cls_tglo.p_cls_input->IsVirtualKeyTapped(C_INPUT_START))
			{
				//start mower
				i_sound_mode = C_MOW_PLAY_STARTUP;
			}
		} else
		{
			//mower is already going	
			if ( 
				(i_sound_mode == C_MOW_MOWING)
				|| (i_sound_mode == C_MOW_SWITCH_TO_IDLE)
				)
			{
				//mower is already running, stop it
				if (cls_tglo.p_cls_input->IsVirtualKeyTapped(C_INPUT_START))
				{
					//
					i_sound_mode = C_MOW_STOP;
				}


			}
		}
		
	}

	
}
   
void CEntMower::SetPower(bool b_on)
{
	//turn the mower on or off
    if (!b_on)
	{
		
		if (i_sound_mode != C_MOW_TURNING_OFF)
		{
			//turn it off!
			if ( 
				(i_sound_mode == C_MOW_MOWING)
				|| (i_sound_mode == C_MOW_SWITCH_TO_IDLE)
				)
			{
				//mower is already running, stop it
				i_sound_mode = C_MOW_STOP;
				
			}
			
		}
		
		
	}  else
	{
		//turn it on!
			if (i_sound_mode == C_MOW_NO_COMMAND)
		{
				//start mower
				i_sound_mode = C_MOW_PLAY_STARTUP;
		}

	}
	
}

bool CEntMower::set_tile_to_mowed()
{
 	static float f_x, f_y;

	if (m_i_mow_zone != (cls_tglo.p_map->get_tile_pointer(cls_tglo.p_map->get_tile_x(get_pos_x()),
		cls_tglo.p_map->get_tile_y(get_pos_z())))->i_sub_zone)
	{
		//not a mowable tile, ignore it.
		return false;
	}

    static int i_x;
	static int i_y;
	i_x = cls_tglo.p_map->get_block_x(this->get_pos_x());
	i_y = cls_tglo.p_map->get_block_y(this->get_pos_z());


	if (cls_tglo.p_map->p_block[GET_BLOCK_SMART(i_x,i_y)].i_tex_id != m_i_mowed_tile)
	{
		cls_tglo.p_map->p_block[GET_BLOCK_SMART(i_x,i_y)].i_tex_id = m_i_mowed_tile;
	   cls_tglo.p_terrain->update_terrain_block_from_map(i_x,i_y);
       AddTileToMowed();
    
        
	   //show some grass getting cut for a while
	   if (p_clippings)
	   p_clippings->set_no_new_particles(false);
	   
	   this->timer_clippings.set_interval(1000);
	   

	   return true;
	}

	//tile was already mowed
	return false;

}

void CEntMower::AddTileToMowed()
{
       
       //we just mowed this tile
       m_i_squares_mowed += cls_tglo.p_map->i_tiles_per_block*cls_tglo.p_map->i_tiles_per_block; 	 //this might be wrong, it should be subtiles per tile
       // log_msg("Mowed %d of %d.", m_i_squares_mowed, m_i_total_zone_squares);
       //recalculate percent
	   
       m_f_percent_mowed =  (float(m_i_squares_mowed) / float(m_i_total_zone_squares));
	 
       //set out meter to this if it exists

       CPicMeter *p_meter = (CPicMeter*)p_my_thing_list->get_thing_by_name("PROGRESS");
       
       if ( (m_f_percent_mowed > 0.99f) && (m_f_percent_mowed < 1))
       {
           //for visual effect let's not let it get to close being done until it's really complete
           m_f_percent_mowed = 0.99f;
       }
       p_meter->SetMeter(m_f_percent_mowed);
       
       /*
	   char st_temp[100];
	   sprintf(st_temp, "m_f_percent_mowed is %.0f", m_f_percent_mowed);
	   log_msg(st_temp);
	   */
	  
}

void CEntMower::think()
{
    
    //process sound
    process_sound();
    if (p_clippings)
        if (p_clippings->get_no_new_particles() == false)
        {
            //currently making particles, let's see if it's time to turn it off
            if (timer_clippings.interval_reached())
            {
                //shut 'er down
                p_clippings->set_no_new_particles(true);
            }
        }

	if (GetPauseLevel() >= cls_tglo.GetPauseLevel()) 
	{
		process_keys();
      
        

      
         if (cls_tglo.p_cls_input->GetAnyJoystickButton(3))
    {
        cls_tglo.p_cls_input->SetVirtualKey(C_INPUT_FORWARD, 1);
    }

    if (cls_tglo.p_cls_input->GetAnyJoystickButton(2))
    {
        cls_tglo.p_cls_input->SetVirtualKey(C_INPUT_BACKWARD, 1);
    }

    if (cls_tglo.p_cls_input->GetAnyJoystickButtonTapped(1))
    {
        cls_tglo.p_cls_input->SetVirtualKey(C_INPUT_ACTION3, 1);
    }

    if (m_i_input_delay < GetTickCount())
    {
    
    if (cls_tglo.p_cls_input->IsVirtualKeyTapped(C_INPUT_ACTION3))
    {
        
        CEntItem * p_script = new CEntItem();
        cls_tglo.p_thing_list->add_new_thing(p_script);
        if (p_script->InitScript("script\\weed_button.c") == C_SCRIPT_NONE)
        {
            //error loading script, forget the whole thing
            p_script->b_delete_flag = true;
        }  else
        {
            //set the name also
            p_script->set_thing_name("IFUNCTION");
            p_script->RunScriptFunction("USEWEEDKILLER");
        }
        
    }
    }

	if (cls_tglo.p_cls_input->GetAnyJoystickDir(C_INPUT_JOY_UP))
    {
       m_b_joy_held_down = false;

        if (!m_b_joy_up_timer_active)
        {
          m_b_joy_up_timer_active = true;
          m_timer_up.reset();
        } else
        {
            //if they hold this down more than so long, count it as 'holding'
            if (m_timer_up.get_elapsed_milliseconds() > C_JOYSTICK_SMOOTH_INITIAL_DELAY)
            {
                m_b_joy_held_up = true;
            }

        }
       
        m_i_last_touched_up = GetTickCount();

        cls_tglo.p_cls_input->SetVirtualKey(C_INPUT_FORWARD, 1);
    } else
    {
      
        m_b_joy_up_timer_active = false;
        
        if (m_b_joy_held_up)
        {
          if (m_i_last_touched_up+C_JOYSTICK_SMOOTHING < GetTickCount())
          {
              m_b_joy_held_up = false;
          }   else
          {
              //fake that they still have this held down
               cls_tglo.p_cls_input->SetVirtualKey(C_INPUT_FORWARD, 1);

          }
        } else
        {
            //they are not holding down the thing
        }
      
    }

    if (cls_tglo.p_cls_input->GetAnyJoystickDir(C_INPUT_JOY_DOWN))
    {
        m_b_joy_held_up = false;
      
           if (!m_b_joy_down_timer_active)
        {
          m_b_joy_down_timer_active = true;
          m_timer_down.reset();
        } else
        {
            //if they hold this down more than so long, count it as 'holding'
            if (m_timer_down.get_elapsed_milliseconds() > C_JOYSTICK_SMOOTH_INITIAL_DELAY)
            {
                m_b_joy_held_down = true;
            }

        }
       
        m_i_last_touched_down = GetTickCount();


        
        cls_tglo.p_cls_input->SetVirtualKey(C_INPUT_BACKWARD, 1);
    }  else
    {
      
        m_b_joy_down_timer_active = false;
        
        if (m_b_joy_held_down)
        {
          if (m_i_last_touched_down+C_JOYSTICK_SMOOTHING < GetTickCount())
          {
              m_b_joy_held_down = false;
          }   else
          {
              //fake that they still have this held down
               cls_tglo.p_cls_input->SetVirtualKey(C_INPUT_BACKWARD, 1);

          }
        } else
        {
            //they are not holding down the thing
        }
      
    }



   
   
   
         
        if (cls_tglo.p_cls_input->GetAnyJoystickDir(C_INPUT_JOY_LEFT))
        {
            cls_tglo.p_cls_input->SetVirtualKey(C_INPUT_LEFT, 1);
        }
        
        if (cls_tglo.p_cls_input->GetAnyJoystickDir(C_INPUT_JOY_RIGHT))
        {
            cls_tglo.p_cls_input->SetVirtualKey(C_INPUT_RIGHT, 1);
        }
      
    
    }
    
	
/*	

	//if we fell from high enough, let's play a sound effect
	if (f_bounce_height_from_last_frame < -0.05)
	{
		cls_tglo.p_sound->Play("sound\\fall.wav");
	}
  */

	bool b_moved = false;

	


	if (get_pos_y() < f_ground_height+0.01f)
	{
				if ( i_sound_mode == C_MOW_MOWING )
			{
                    
                    if (GetPauseLevel() >= cls_tglo.GetPauseLevel()) 
                    {
                        
                        if (set_tile_to_mowed())
                        {
                            //play sound effect
                            
                            Sound3DPlay(&m_vObjectPosition, "sound\\pop.wav", 1);
                        }
                    }
                }
	//we're on  the ground
                if (
                    
                    (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_JUMP))
                  //  ||
                  //  (cls_tglo.p_cls_input->GetAnyJoystickButtonTapped(0))
                    ||
                    (cls_tglo.p_cls_input->GetAnyJoystickButtonTapped(0))
                    )
                {
                    
                    //only jump if we're on the ground    
                    this->vec_vel.y += cls_tglo.time()*10;
                    
                }
           
	} else
	{
	  //flying through the air
	// 	 cls_tglo.p_sound->SoundInstanceSetPitch(h_mow, 22500);	
	}

    if (m_f_health > 0)
    {

    float f_sens_mod = 1;    
  

    if (
        (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_ACTION2))
        ||
        (cls_tglo.p_cls_input->GetAnyJoystickButton(6))
        ||
        (cls_tglo.p_cls_input->GetAnyJoystickButton(7))
        )
    {
      f_sens_mod = 2.0f; //super sharp turn
    }

    int i_forward_vkey1 = C_INPUT_FORWARD;
    int i_forward_vkey2 = C_INPUT_FORWARD_SECONDARY;


    int i_backward_vkey1 = C_INPUT_BACKWARD;
    int i_backward_vkey2 = C_INPUT_BACKWARD_SECONDARY;


    if (m_i_drunk > 0)
    {
        //reverse the keys

        i_forward_vkey1 = C_INPUT_BACKWARD;
        i_forward_vkey2 = C_INPUT_BACKWARD_SECONDARY;

        i_backward_vkey1 = C_INPUT_FORWARD;                
        i_backward_vkey2 = C_INPUT_FORWARD_SECONDARY;  
    }

	bool b_turn_in_place = true;


    
    if (GetPauseLevel() >= cls_tglo.GetPauseLevel()) 
	{

        
        if (b_turn_in_place)
        {
            if (!(cls_tglo.p_cls_input->IsVirtualKeyDown(i_backward_vkey1)))
                if (!(cls_tglo.p_cls_input->IsVirtualKeyDown(i_backward_vkey2)))
                {
                    
                    if ( (!cls_tglo.p_cls_input->IsVirtualKeyDown(i_forward_vkey1))
                        && (!cls_tglo.p_cls_input->IsVirtualKeyDown(i_forward_vkey2)))
                    {
                        
                        goto allow_move;
                    }
                }
                
        }
		if ( (cls_tglo.p_cls_input->IsVirtualKeyDown(i_forward_vkey1))
            || (cls_tglo.p_cls_input->IsVirtualKeyDown(i_forward_vkey2))
            )
		{
			set_desired_velocity(m_f_mower_speed);
     	b_moved = true;
		
allow_move:
            fforce_range(&f_target_vel, -f_max_vel, f_max_vel);
			
			if (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_LEFT))
            {
                if (m_i_drunk > 0)
                {
                    //BACKWARDS
                    this->set_direction_rad(f_dir_goal+cls_tglo.time()*m_f_turn_sensitivity*f_sens_mod);
                    
                } else
                {
                    
                    this->set_direction_rad(f_dir_goal-cls_tglo.time()*m_f_turn_sensitivity*f_sens_mod);
                }
                
            }
            
            if (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_RIGHT))
            {
                if (m_i_drunk > 0)
                {
                    //BACKWARDS
                    this->set_direction_rad(f_dir_goal-cls_tglo.time()*m_f_turn_sensitivity*f_sens_mod);
                } else
                {
                    
                    this->set_direction_rad(f_dir_goal+cls_tglo.time()*m_f_turn_sensitivity*f_sens_mod);
                }
            }
            
		}
		
		if (
            (cls_tglo.p_cls_input->IsVirtualKeyDown(i_backward_vkey1))
            ||   (cls_tglo.p_cls_input->IsVirtualKeyDown(i_backward_vkey2))
            )
		{
			set_desired_velocity(-m_f_mower_speed);
			fforce_range(&f_target_vel, -f_max_vel, f_max_vel);
			b_moved = true;
			
			
			if (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_LEFT))
			{
                if (m_i_drunk > 0)
                {
                    //BACKWARDS
                    this->set_direction_rad(f_dir_goal-cls_tglo.time()*m_f_turn_sensitivity*f_sens_mod);
                    
                } else
                {
                    
                    this->set_direction_rad(f_dir_goal+cls_tglo.time()*m_f_turn_sensitivity*f_sens_mod);
                }
            }
            
            if (cls_tglo.p_cls_input->IsVirtualKeyDown(C_INPUT_RIGHT))
            {
                if (m_i_drunk > 0)
                {
                    //BACKWARDS    
                    this->set_direction_rad(f_dir_goal+cls_tglo.time()*m_f_turn_sensitivity*f_sens_mod);
                    
                } else
                {
                    
                    this->set_direction_rad(f_dir_goal-cls_tglo.time()*m_f_turn_sensitivity*f_sens_mod);
                }
            }
            
			
		}
		
	}

    }

	if (!b_moved)
	{
	  //stop movement
		set_desired_velocity(0);
	}

			//handle rolling sounds

		if (!b_rolling)
		{
			if (i_sound_mode != C_MOW_MOWING)
			if (get_pos_y() < f_ground_height+0.01f)
			if (b_moved)
			{
			   //we're moving again
		   	 	cls_tglo.p_sound->SoundInstanceStop(h_roll);
		   	    cls_tglo.p_sound->SoundInstancePlay(h_roll, 9999);
			   b_rolling = true;
			}

		} else
		{
		  //currently rolling.  but did we stop?

			if (
			 (i_sound_mode == C_MOW_MOWING)
				||

			 (!b_moved)
 			 ||
				(get_pos_y() >= f_ground_height+0.01f)
			 )
			{
			   //we've stopped.
		   	 	cls_tglo.p_sound->SoundInstanceStop(h_roll);
			   b_rolling = false;
			}


		}

	// alpha_sort();

	//should we turn off the clippings coming out of the motor now?
	

	CEnt::think(); //do movement

	//visual sort
	CheckForAlphaSort(); 
    /*

	//let's send our movement info to everybody else

  CMessageHeader msg;
  msg.i_message = C_THING_MSG_MOVE;
	
  CMessageMove custom;
  custom.p_vec_pos = &m_vObjectPosition;
  custom.e_collision_type = e_collision_type;
  custom.i_collision_special = C_COLLISION_SPECIAL_NONE;

  custom.f_enemy_rad_mod = 1;

  if (i_sound_mode != C_MOW_MOWING)
  {
  	  //can't do damage, mower not on
	  custom.f_damage = 0;
  } else
  {
	 
      custom.f_damage = 1;
  }
  custom.f_radius = 1;//GetRadius(); //do circle/point not circle circle

  p_my_thing_list->broadcast_message(&msg,(void*)&custom, C_THING_ENT_GENERIC);
  */

   if (i_sound_mode != C_MOW_MOWING)
  {
  	  //can't do damage, mower not on
	  this->SetTouchDamage(0);
  } else
  {
	 
	  this->SetTouchDamage(1);
  }

  //update our sound
	  //in any case, let's update our 3d positions.
	  cls_tglo.p_sound->SoundInstance3DSetPos(h_mow, m_vObjectPosition.x, m_vObjectPosition.y-1, m_vObjectPosition.z);
	  cls_tglo.p_sound->SoundInstance3DSetPos(h_roll, m_vObjectPosition.x, m_vObjectPosition.y-1, m_vObjectPosition.z);
    
      if (m_b_height_check_active)
    {
        if (get_pos_y() < m_f_height_check)
        {
            CEntItem * p_script = new CEntItem();
            cls_tglo.p_thing_list->add_new_thing(p_script);
            if (p_script->InitScript("script\\damage_mod.c") == C_SCRIPT_NONE)
            {
                //error loading script, forget the whole thing
                p_script->b_delete_flag = true;
            
                
            }  else
            {
            //set the name also
            p_script->set_thing_name("IFUNCTION");
            p_script->RunScriptFunction("DAMAGEKILL");
            }
            
            m_b_height_check_active = false;
          //uncomment to force the camera view when dying
          //    i_view = 0;
          //  set_mower_view();
        }
    }
}

