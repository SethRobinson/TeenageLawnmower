
//******************************************
// copyright 2004	Seth A. Robinson

// This file is NOT public domain and may not be shared, distributed or used
// without specific permission.

// seth@rtsoft.com - www.rtsoft.com
//******************************************

#pragma once

#include "3d\\CEnt.h"
#include "CTimer.h"
#include "sound\\ISoundManager.h"

class CParticleEmitter; //forward for speed

class CEntMower : public CEnt
{

public:

  CEntMower();
  virtual ~CEntMower(); //clean up
  virtual void think();
  void process_sound();
  void process_keys();
  bool set_tile_to_mowed();
  void enable_smoke();
  void SetHealth(float f_health);
  void ToggleCameraFreeze();
  void SetPower(bool b_on);
  void set_mower_view();
  float GetPercentMowed(){return m_f_percent_mowed;}
  void SetMowerSpeed(float f_speed);
  void SetMowedTile(int i_new);
  void EnableClippings(char *st_name);
  void SetDrunk(int i_new){m_i_drunk = i_new;} 
  void KillFire();
  void KillDamageSmoke();
  void SetHeightBlowUp(bool b_active, float f_height);
  int GetMowedTile(){return m_i_mowed_tile;}
  int GetMowZone(){return m_i_mow_zone;}
  void AddTileToMowed();


private:

	hSoundInstance h_mow; //sound handle to the lawn mowers sound
	hSoundInstance h_roll; //sound handle to the lawn mowers sound
	hSoundInstance h_fire_sound; //keep track of our looping fire sound when needed.. so we
    //can turn it off.
    bool b_rolling;
	CTimer timer_sound; //help us with changing the sound
	int i_sound_mode; //keep track of which sound to play
	int i_view;
    CParticleEmitter *p_clippings;
	CParticleEmitter *p_smoke;
	CParticleEmitter *p_fire;
    CParticleEmitter *p_damage_smoke;
	CTimer timer_clippings;
    int m_i_mow_zone; //the index of our mow sub zone we've setup in  the map editor
	float m_f_percent_mowed; //how much we've mowed already, starts at 0
	int m_i_total_zone_squares; //how many squares we have to mow to finish
	int m_i_squares_mowed; //remember, 9 squares make up one full tile or.. i forget
    int m_i_smoke_id, m_i_clipper_id; //remember the ID of our objects we create
    float m_f_health;
    float m_f_turn_sensitivity;
    float m_f_mower_speed;
    int m_i_mowed_tile; //after mowing a tile it changes to this
    int m_i_drunk; //how drunk we are
    bool m_b_exploded; //so we don't keep playing the explode sound over and over when we have 0 health
    bool m_b_height_check_active; //if yes, we will blow up if they go below a certain height
    float m_f_height_check; //how low they have to go
    bool m_b_joy_up_timer_active;
    CTimer m_timer_up;
    DWORD m_i_last_touched_up;
    bool m_b_joy_held_up;

    bool m_b_joy_down_timer_active;
    CTimer m_timer_down;
    DWORD m_i_last_touched_down;
    bool m_b_joy_held_down;
    int m_i_input_delay; //let input be unresponsive for the weed killer at the start to stop accidental clicking

};
