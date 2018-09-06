/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */




#pragma once

#pragma warning (disable:4786)


const int C_ENT_PHYSICS_NONE = 0;
const int C_ENT_PHYSICS_NORMAL = 1; //most things are this
const int C_ENT_PHYSICS_ACCURATE = 2;  //not really used yet, failed attempt
const int C_ENT_PHYSICS_SIMPLE = 3; //can't go over walls, good for the player in TLM

const int C_HITWALL_NOTHING = 0;
const int C_HITWALL_REVERSE = 1;

const int C_GRAVITY_WORLD = 0;
const int C_GRAVITY_OVERRIDE = 1;
const int C_GRAVITY_MOD = 2;

const int C_FORCE_GROUND_ONLY = 0;
const int C_FORCE_GROUND_AND_AIR = 1;     

const int C_ENT_BROADCAST_NONE = 0;
const int C_ENT_BROADCAST_MOVEMENT = 1;

const int C_ENT_HARDNESS_NONE = 0;
const int C_ENT_HARDNESS_NORMAL = 1;


#include "CVisualMulti.h"
#include "CThing_globals.h"
#include "..\\CTimer.h"


//class for entities with some simple physics

class CEnt : public CVisualMulti
{

public:
 	virtual void dx_draw();
	virtual void think();
	D3DXVECTOR3 get_velocity(){return vec_current_movement;};
	virtual void dx_restore();
	virtual void dx_create();
	virtual void dx_kill();
	void CEnt::set_desired_velocity(float f_new);
    void CEnt::SetMaxSpeed(float f_new) {f_max_vel = f_new;}
    void RangeTestCallback(const Vector3d<float> &p,float distance,SpherePack *sphere,ViewState state);
    void SetBroadCastType(int i_new){m_i_broadcast_type = i_new;}
    int GetBroadCastType() {return m_i_broadcast_type;}
    void SetHardness(int i_new){m_i_hardness = i_new;}
    int GetHardness(){return m_i_hardness;}
    

    float CEnt::GetMaxSpeed() { return f_max_vel;}
    float CEnt::GetGravity();
    void CEnt::SetGravityMode(int i_gravity_type, float f_amount)
    {
       m_i_gravity_type = i_gravity_type;
       m_f_gravity_override = f_amount;
    }

    void SetForceType(int i_new_type){m_i_force_type = i_new_type;}
    int GetForceType() {return m_i_force_type;}
    
    inline float get_desired_velocity(){return f_target_vel;};
	void CEnt::set_direction_rad(float f_new);
	void CEnt::compute_tilt_amount(float *f_x_tilt, float *f_y_tilt, D3DXVECTOR3 *vec_ground_normal);
    void CEnt::place_on_ground();
	void set_tilt_lerp(float f_new){f_tilt_lerp = f_new;};
	CEnt();
	virtual ~CEnt();
	void CEnt::set_tilt_rad(float f_new);
	void CEnt::SetPhysics(int i_physics);
	void CEnt::SetTurnLerp(float f_turn_lerp);
	void CEnt::SetHitWallBehavior(int i_hitwall){m_i_hit_wall_behavior = i_hitwall;}
    void ModifyAccel(D3DXVECTOR3 *vec_force){vec_accel += *vec_force;};
    D3DXVECTOR3 * GetAccel(){return &vec_accel;};
    D3DXVECTOR3 * GetForce(){return &vec_current_movement;}; //force is accel + vel

    D3DXVECTOR3 * GetMovement(){return &vec_vel;};
    void SetMovement(D3DXVECTOR3 *vec_force){vec_vel = *vec_force;};
    

    
    //actually should be called SetNewForceToAdd...not setaccel. It's SetForce in script
    void SetAccel(D3DXVECTOR3 *vec_force){vec_accel = *vec_force;};
    
    void SetGroundFriction(float f_new){f_ground_friction = f_new;}
    void SetBounce(float f_new){f_bounce_power = f_new;}
    void SetRoll(bool b_new){b_roll = b_new;}
    void SetRollMod(float f_new) {m_f_roll_mod = f_new;}
    void SetBounceSound(char *p_st_sound, float f_height, int i_time_between_bounces);
    void SetAcceleration(float f_new){f_accel = f_new;} //0.3 is normal
    void SetTouchDamage(int i_new){m_i_touch_damage = i_new;}
    int GetTouchDamage(){return m_i_touch_damage;}
protected:

//physics
  D3DXVECTOR3 vec_vel; //our current movement
  D3DXVECTOR3 vec_accel; //change of speed to apply, 0 if none
  D3DXVECTOR3 vec_current_movement; //how we're moving right now
  float f_mass; //how heavy we are
  float f_max_vel; //fastest possible to move, by our power and external power
  float f_accel; //how fast we can accel/deacell by our own power
  float f_ground_friction; //0.002 is normal, higher is more friction
  float f_traction; //1 means full effect from sloping, 0 is not affected
  float f_current_vel; //how fast we're going by our own power
  float f_target_vel; //how fast we want to go
  bool b_roll; //if true we rotate on movement, for balls
  float f_bounce_power; //0.50 to keep 50% of power on a bounce
  float f_tilt_lerp; //smoothing between hill angles, 0.10 is normal
  float f_dir_lerp; //smoothing for turning
  float f_tilt_goal; //allow smooth tilting
  float f_bounce_height_from_last_frame; //if not 0, we bounced
  int m_i_physics; //0 for none, 1 for normal
  int m_i_gravity_type; //allow ent to float or do a mod on global gravity settigns
  float m_f_gravity_override; //only used if gravity type isn't C_GRAVITY_WORLD
  int m_i_force_type; //control if the thing propel itself in the air or ground or what
  float m_f_dir_offset;
  int m_i_hit_wall_behavior;
  float m_f_roll_mod; //1 mreans normal roll, 0.001 means REALLY slow
  char m_st_bounce_wave[256]; //if not null we should play this .wav if they bounce 
  CTimer m_timer_bounce; //don't play the bounce .wav too much
  int m_i_broadcast_type; //what we tell other close by ents
  int m_i_hardness; //can other players walk through us?
  int m_i_touch_damage; //0 for none
   

};