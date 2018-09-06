/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */




#include "CEnt.h"
#include "CTerrain.h"
#include "CThing_util.h"
#include "..\\CInput.h"
#include "CBaseEntData.h"			 
#include "Ent_utils.h"
#include "CEntItem.h"      
#include "CMessageHeader.h"

#include "CThingList.h"
#include "SphereTree\\SpherePack.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


  static CMessageHeader s_msg;  //shared between all
  static CMessageMove s_move;   //shared between all

#define C_AIRBORN_CONTROL_MOD 0.5f

CEnt::CEnt()
{
//  	init_visual("model\\test\\box_man.x", C_RESOURCE_PMESH);
	this->vec_vel = D3DXVECTOR3(0,0,0);
	this->vec_accel = D3DXVECTOR3(0,0,0);
	
    this->f_mass = 1;
	f_accel = 0.05f;
	this->f_max_vel = 40.0f;
	set_direction_rad(0);
	f_ground_friction = 0.01f;
    this->f_traction = 0.015f;
	f_target_vel = 5;
	f_current_vel = 0;
	b_roll = false;	
    m_f_roll_mod = 1; //multiplied against the normal roll
	f_bounce_power = 0.50f;
	f_tilt_lerp = 11;
    f_bounce_height_from_last_frame = 0;
	f_dir_goal = f_dir;
	f_dir_lerp = 8; //no smoothing
	f_tilt_goal = 0;
	f_ground_height = 0; 
	SetGravityMode(C_GRAVITY_WORLD, 0); //default, just use the world's gravity
    SetForceType(C_FORCE_GROUND_ONLY); //can't move under it's own force while falling/in the air
    vec_current_movement = D3DXVECTOR3(0,0,0);
	m_i_physics = C_ENT_PHYSICS_NORMAL; //default
    m_f_dir_offset = 0;
	
    m_timer_bounce.set_interval(300); 
    m_timer_bounce.TriggerInterval(); //make sure even right at the start this will be active
    SetHitWallBehavior(C_HITWALL_NOTHING);
    m_st_bounce_wave[0] = 0;
    SetHardness(C_ENT_HARDNESS_NONE);
    SetBroadCastType(C_ENT_BROADCAST_NONE);
    SetTouchDamage(0);
    
}

CEnt::~CEnt()
{
     
}


void CEnt:: SetBounceSound(char *p_st_sound, float f_height, int i_time_between_bounces)
{
    strcpy(m_st_bounce_wave, p_st_sound);
    //ignore the rest for now

}

/*
//select mesh name to load.  Enter NULL to load the default tiger.
*/
void CEnt::SetTurnLerp(float f_turn_lerp)
{
    f_dir_lerp = f_turn_lerp;
}

void vec_force_range(D3DXVECTOR3 *p_vec, float f_min, float f_max)
{
    if (p_vec->x < f_min) p_vec->x = f_min; else if (p_vec->x > f_max) p_vec->x = f_max;
    if (p_vec->y < f_min) p_vec->y = f_min; else if (p_vec->y > f_max) p_vec->y= f_max;
    if (p_vec->z < f_min) p_vec->z = f_min; else if (p_vec->z > f_max) p_vec->z = f_max;
}




void CEnt::place_on_ground()
{
  	 float f_ground_height;
	 float f_y_offset = 0;

	 if (this->e_collision_type == ::C_COLLISION_SPHERICAL)
	 {
	   f_y_offset = m_f_radius;
	 } else
	 {
	   f_y_offset = f_height;
	 }

 	  //get height and ground normal
     if (cls_tglo.p_terrain)
	 {
	 f_ground_height = cls_tglo.p_terrain->get_height(get_pos_x(), get_pos_z());
	 } else
	 {
	   //no terrain exists, figure something out
		 f_ground_height = 0;
	 }

   if (f_ground_height != get_pos_y())
   {
	 //move to the ground level
	 set_pos_y(f_ground_height);
//	 this->translateMovement(); //apply rotation
   }
}

void CEnt::set_desired_velocity(float f_new)
{
  f_target_vel = f_new;	
}

void CEnt::set_direction_rad(float f_new)
{
	//scale them so we turn the correct way
	
	f_new =  float(fmod(f_new, 2*D3DX_PI));
	f_dir =  float(fmod(f_dir, 2*D3DX_PI));

	f_dir_goal = f_new;
}

void CEnt::set_tilt_rad(float f_new)
{
  	f_tilt_goal = f_new;
 
}


void CEnt::compute_tilt_amount(float *f_x_tilt, float *f_z_tilt, D3DXVECTOR3 *vec_ground_normal)

{
    static D3DXVECTOR3 vec_x;
    static D3DXVECTOR3 vec_z;
    
    vec_x.z = vec_ground_normal->z;
    vec_x.y = vec_ground_normal->y;
    vec_x.x = 0;
    if (vec_x.z == 0)
    {
        *f_x_tilt = 0;
    } else
    {
        float f_x_mag = D3DXVec3Length(&vec_x);
        vec_x = vec_x / f_x_mag;
        
        *f_x_tilt = float(acos(D3DXVec3Dot(&vec_x, &g_vec_up)));
        
        if (vec_ground_normal->z < 0)
        {
            *f_x_tilt = (*f_x_tilt*-1);
        }
    }
    
    vec_z.z = 0;
    vec_z.y = vec_ground_normal->y;
    vec_z.x = vec_ground_normal->x;
    if (vec_z.x == 0)
    {
        *f_z_tilt = 0;
    } else
    {
        
        float f_z_mag = D3DXVec3Length(&vec_z);
        vec_z = vec_z / f_z_mag;
        
        *f_z_tilt = float(acos(D3DXVec3Dot(&vec_z, &g_vec_up)));
        
        if (vec_ground_normal->x > 0)
        {
            *f_z_tilt = (*f_z_tilt*-1);
        }
    }
    
    
}

void CEnt::SetPhysics(int i_physics)
{

	m_i_physics = i_physics;
    if (i_physics != C_ENT_PHYSICS_NONE)
    {
        //make sure free move isn't on if it's an CEntItem
        if (get_type() == C_THING_ENT_GENERIC)
        {
            ((CEntItem*)this)->SetFloatingMode(false);
        }
    }

}

float CEnt::GetGravity()
{
    if (m_i_gravity_type == C_GRAVITY_WORLD)
        return cls_tglo.p_map->get_gravity();
     else
    if (m_i_gravity_type == C_GRAVITY_OVERRIDE)
        return m_f_gravity_override;
    else
        if (m_i_gravity_type == C_GRAVITY_MOD)
        {
            return cls_tglo.p_map->get_gravity() * m_f_gravity_override;
        }

    
        //if we go here there was a big fat error
        log_error("Unknown gravity type for Ent %s.", get_thing_name());
        return cls_tglo.p_map->get_gravity();
}

void CEnt::think()
{
    CThing::think();

	if (m_i_physics == C_ENT_PHYSICS_NONE)
	{

//		 if (m_i_draw_mode == C_DRAW_MODE_NORMAL)
		 {
			this->set_translate_asap(true);
	  
            m_f_dir_offset = D3DX_PI;
		    this->setAngleY(f_dir+m_f_dir_offset);
           }


         //we've been told not to do physics on this thing
	  	f_dir = lerp_float_direction(f_dir, f_dir_goal, f_dir_lerp * cls_tglo.time());
       CVisual::think();

	  return;
	}
	
	
	 float f_desired_vel = f_max_vel;
	 static float f_z_tilt;
	 static float f_x_tilt;
	 static D3DXVECTOR3 vec_walk; //out own internal force
   	 static D3DXVECTOR3 vec_ground_normal;

     static bool b_touched_wall;
     static bool b_touched_ground;
     static bool b_touched_ceiling;

     b_touched_ground = false;
     b_touched_wall = false;
     b_touched_ceiling = false;

	 int i_tried = 0;


	 float f_y_offset = 0.1f;

   	f_dir = lerp_float_direction(f_dir, f_dir_goal, f_dir_lerp* cls_tglo.time());
 

    
    if (this->e_collision_type == ::C_COLLISION_SPHERICAL)
    {
        f_y_offset = m_f_radius;
    } else
    {
        f_y_offset = f_height;
    }
    
    //get height and ground normal
    if (cls_tglo.p_terrain)
    {
        f_ground_height = cls_tglo.p_terrain->get_ground_and_y(this->get_pos_x(), this->get_pos_z(), &vec_ground_normal);
    } else
    {
        //no terrain exists, figure something out
        f_ground_height = 0;
    }
   
    static D3DXVECTOR3 vec_walk_goal;
   
    //find out what the ground tilt is here
    compute_tilt_amount(&f_x_tilt, &f_z_tilt, &vec_ground_normal);
    
    //apply walk 
    vec_walk.z = float(cls_tglo.time()*f_current_vel*cos(f_dir));
    vec_walk.x = float(cls_tglo.time()*f_current_vel*sin(f_dir));
    vec_walk.y = 0; //we're not exerting any Y power (unless we can fly)
    
     if (GetForceType() == C_FORCE_GROUND_AND_AIR)
     {
       //we can fly.
         vec_walk.y = float(cls_tglo.time()*f_current_vel*f_tilt);

     }

    
    f_bounce_height_from_last_frame = 0; //clear last bounce info

	 //did we bounce?
                                           //used to be 0.01
    if ((get_pos_y()-f_y_offset <= (f_ground_height+0.05f)) )
    {
        //we are touching or below the ground
         b_touched_ground = true;  //checked at the end of this func

			 	 	 //but did we bounce?
        float C_MIN_BOUNCE_VEL = 0.03f;
        //do we have enough oomph to bounce?
        
        if (vec_vel.y < -C_MIN_BOUNCE_VEL)
        {
            //figure out which way to bounce  
            float i_temp_height = 0;
            
            if (cls_tglo.p_terrain)
            {
                i_temp_height = f_ground_height;
            }
            
            f_bounce_height_from_last_frame = vec_vel.y;
            
            D3DXVECTOR3 vec_projected_on_n;
            vec_projected_on_n = D3DXVec3Dot(&vec_vel, &vec_ground_normal) * vec_ground_normal;
            D3DXVECTOR3 vec_per_of_vel = vec_vel-vec_projected_on_n;
            vec_vel = (vec_per_of_vel- (vec_projected_on_n*(f_bounce_power)));
          
            //also modify our velocity

            vec_projected_on_n = D3DXVec3Dot(&vec_accel, &vec_ground_normal) * vec_ground_normal;
            vec_per_of_vel = vec_accel-vec_projected_on_n;
            vec_accel = (vec_per_of_vel- (vec_projected_on_n*(f_bounce_power)));
            set_pos_y(f_y_offset+i_temp_height);

        //play bounce wave?
            if (m_st_bounce_wave[0] != 0)
            {
                //we have this timer so we won't keep playing the bounce .wav too often
                if (m_timer_bounce.interval_reached())
                {
                
                Sound3DPlay(&m_vObjectPosition, m_st_bounce_wave, 1);
                }
            }
        
        } else
        {
            //don't bounce, but don't go into the ground either
            set_pos_y(	f_y_offset+f_ground_height);
            //we hit the ground, kill downward velocity
            if (vec_vel.y < 0)
                vec_vel.y = 0;
        }
        
        
        //add acceleration to current vel
        set_float_with_target(&f_current_vel, f_target_vel, f_accel*cls_tglo.time()*100);
        //rotate to match ground
        if (b_follow_ground_tilt)
        {
            this->setAngleZ(lerp_float(getAngleZ(),f_z_tilt, f_tilt_lerp*cls_tglo.time()));
            this->setAngleX(lerp_float(getAngleX(),f_x_tilt, f_tilt_lerp*cls_tglo.time()));
        } 
    
    //save this
 
        //go ahead and mod our accel
    vec_walk_goal = vec_walk* (cls_tglo.time())*100;	 

    set_float_with_target(&vec_accel.x, vec_accel.x, 0.2f); //get rid of small amounts
    set_float_with_target(&vec_accel.y, vec_accel.y, 0.2f); //get rid of small amounts
    set_float_with_target(&vec_accel.z, vec_accel.z, 0.2f); //get rid of small amounts
    lerp_vec3(&vec_accel, &vec_accel, &vec_walk_goal, 13 * cls_tglo.time());

    } else
    {
        //flying
   
     if (GetForceType() == C_FORCE_GROUND_AND_AIR)
     {
         //hey, they can move in the air too
         //add acceleration to current vel
         set_float_with_target(&f_current_vel, f_target_vel, f_accel*cls_tglo.time()*100);
         //rotate to match ground
         if (b_follow_ground_tilt)
         {
             this->setAngleZ(lerp_float(getAngleZ(),f_z_tilt, f_tilt_lerp*cls_tglo.time()));
             this->setAngleX(lerp_float(getAngleX(),f_x_tilt, f_tilt_lerp*cls_tglo.time()));
         } 
         
         //save this
        
         //go ahead and mod our accel
         vec_walk_goal = vec_walk* (cls_tglo.time())*100;	 
         
         set_float_with_target(&vec_accel.x, vec_accel.x, 0.2f); //get rid of small amounts
         set_float_with_target(&vec_accel.y, vec_accel.y, 0.2f); //get rid of small amounts
         set_float_with_target(&vec_accel.z, vec_accel.z, 0.2f); //get rid of small amounts
         lerp_vec3(&vec_accel, &vec_accel, &vec_walk_goal, 13 * cls_tglo.time());
      
         //if we're above the designated roof let's do something
         if (m_vObjectPosition.y >= cls_tglo.p_map->GetCeilingHeight(m_vObjectPosition.x, m_vObjectPosition.z))
         {
             //notify object that it hit the ceiling
             b_touched_ceiling = true;
         }

     } else
     {
//         LogMsg("FLYING!");
         //go ahead and mod our accel
         //50% power
           if (this == cls_tglo.p_player)
          {
              //special physics for him
              set_float_with_target(&f_current_vel, f_target_vel, f_accel*cls_tglo.time()*20);
              vec_walk_goal = vec_walk* (cls_tglo.time())*100;	 
              
              set_float_with_target(&vec_accel.x, vec_accel.x, 0.01f); //get rid of small amounts
              set_float_with_target(&vec_accel.y, vec_accel.y, 0.01f); //get rid of small amounts
              set_float_with_target(&vec_accel.z, vec_accel.z, 0.01f); //get rid of small amounts
              lerp_vec3(&vec_accel, &vec_accel, &vec_walk_goal, 3 * cls_tglo.time());
          
          }  else
           {

              vec_walk = g_vec_null;
        
           }
       
     }
    
    }
       
	 f_tilt = lerp_float(f_tilt, f_tilt_goal, f_tilt_lerp * cls_tglo.time());
 	
	 //add gravity at all times
     vec_vel.y -= cls_tglo.time()*GetGravity();

     if (get_pos_y()-f_y_offset <= f_ground_height)
     {
         //on ground, if they walk it will matter
          //on ground, add slop ing effect
        
        /* if (
             (fabs(vec_ground_normal.x) > 1)
             || 
             (fabs(vec_ground_normal.y) > 1)
             )
         {
          */
         //don't remove this or it will break the rocks in TLM
         
         vec_vel.x += f_traction *  (vec_ground_normal.x/2);
         vec_vel.z += f_traction *  (vec_ground_normal.z/2);
        // }
         
         //add ground friction
         set_float_with_target(&vec_vel.x, 0, f_ground_friction);
         set_float_with_target(&vec_vel.z, 0, f_ground_friction);
     }

	 //rotate direction
  	 if (m_i_draw_mode == C_DRAW_MODE_NORMAL)
	 {
	 	 m_f_dir_offset = D3DX_PI;
		 this->setAngleY(f_dir+m_f_dir_offset);
	 }

    //accel is our current movement speed, let's check it to make sure we're not going too fast
  
	vec_current_movement = vec_vel + vec_accel;  //save to show other people
  
    if (m_i_physics == C_ENT_PHYSICS_NORMAL)
    {
         
        //collision detection modification
        if (!ValidateCollision(&vec_current_movement, &m_vObjectPosition, 100.0f, GetRadiusDevided()))
        {
            //we hit a wall and we were stopped perhaps.
            //do 180 degree turn
            //this could be modified to also return the walls normal.. hmmm

            //throw a message to the Ent in case he wants to deal with this
            
            b_touched_wall = true;
            

            if (m_i_hit_wall_behavior == C_HITWALL_REVERSE)
            {
                f_dir += D3DX_PI;
                set_direction_rad(f_dir);
                vec_accel = -vec_accel;
            }
        }
        
    } else
        if (m_i_physics == C_ENT_PHYSICS_ACCURATE)
        {
           //collision detection modification
            if (!ValidateCollisionAccurate(&vec_current_movement, &m_vObjectPosition, 100.0f, GetRadiusDevided()))
            {
                //we hit a wall and we were stopped perhaps.
                //do 180 degree turn
               // log_msg("HIT!");
                            b_touched_wall = true;

                
                if (m_i_hit_wall_behavior == C_HITWALL_REVERSE)
                {
                    
                    f_dir += D3DX_PI;
                    set_direction_rad(f_dir);
                      vec_accel = -vec_accel;
             }
                
            }
        }  else
            if (m_i_physics == C_ENT_PHYSICS_SIMPLE)
            {
                
                //collision detection modification
                if (!ValidateCollisionSimple(&vec_current_movement, &m_vObjectPosition, 100.0f, GetRadiusDevided()))
                {
                    //we hit a wall and we were stopped perhaps.
                    //do 180 degree turn
                    //this could be modified to also return the walls normal.. hmmm
                    
                    //throw a message to the Ent in case he wants to deal with this
                    
                    b_touched_wall = true;
                   
                    if (m_i_hit_wall_behavior == C_HITWALL_REVERSE)
                    {
                        f_dir += D3DX_PI;
                        set_direction_rad(f_dir);
                        vec_accel = -vec_accel;
                    }
                }
                
            }
            s_move.p_blocked_by = NULL; //clear this
     
       if (GetBroadCastType() == C_ENT_BROADCAST_MOVEMENT) 
       {
       
     //if we are 'hard' make sure we don't pass through another 'hard' thing
       
        s_msg.i_message = C_THING_MSG_MOVE;
        
        s_move.e_collision_type = e_collision_type;
        s_move.f_damage = float(GetTouchDamage());
        s_move.i_collision_special = 0;
        s_move.p_me = this;
       
        Vector3d<float> p(get_pos_x(),get_pos_y(),get_pos_z());
        p.x += vec_current_movement.x *  (cls_tglo.time()*100);
        p.y += vec_current_movement.y *  (cls_tglo.time()*100);
        p.z += vec_current_movement.z *  (cls_tglo.time()*100);

        
       // char st_crap[50];
       // sprintf(st_crap, "rad is %.2f", f_radius);
       // log_msg(st_crap);
        this->p_my_thing_list->GetSphereTree()->RangeTest(p,m_f_radius*m_f_sphere_tree_mod,this);
       } 
    
    //validate collision returns a valid vector for us to use to move
	
       if (s_move.p_blocked_by)
       {
           //got their asses blocked by another ent
           //  vec_current_movement = g_vec_null; //vec_current_movement * -1;
           if (m_i_hit_wall_behavior == C_HITWALL_REVERSE)
           {
               
               f_dir += D3DX_PI;
               set_direction_rad(f_dir);
               vec_accel = -vec_accel;
           }
           if (get_type() == C_THING_CONTROL)
           {
              
               //let's find the plane on which our collision happens

               //get the normal

               //we will be inside another object if we complete this move. :/
               
               //vec_current_movement = vec_current_movement * -1;
               vec_current_movement.x = 0;
               vec_current_movement.z = 0;
               vec_current_movement.y = 0;
               //vec_force_range(&vec_vel, -0.1f, 0.1f);
               
               //vec_accel = g_vec_null;
               //this->vec_vel = g_vec_null;
               
               //if we're still stuck inside, go up to the top
                       
               s_msg.i_message = C_THING_MSG_MOVE;
               
               s_move.e_collision_type = e_collision_type;
               s_move.f_damage = 0;
               s_move.i_collision_special = 0;
               CEnt *p_hold = s_move.p_blocked_by;
               s_move.p_me = this;
               s_move.p_blocked_by = NULL; //clear this
               
               
               
               Vector3d<float> p(get_pos_x(),get_pos_y(),get_pos_z());
               
               // char st_crap[50];
               // sprintf(st_crap, "rad is %.2f", f_radius);
               // log_msg(st_crap);
               this->p_my_thing_list->GetSphereTree()->RangeTest(p,m_f_radius*m_f_sphere_tree_mod,this);
               
               
               if (s_move.p_blocked_by)
               {
                   //  f_ground_height = s_move.p_blocked_by->f_ground_height + s_move.p_blocked_by->f_radius*2+0.001f;
                   // goto force_ground;
                   
                   vec_current_movement.y = -0.1f;
                   vec_current_movement.z = 0.1f;
                   vec_vel = g_vec_null;
               }
               s_move.p_blocked_by = p_hold; //clear this
               
               
           }
           
       } 
       

	  this->m_vObjectPosition += (vec_current_movement*(cls_tglo.time()*100));
     
	 //apply roll
	 if (b_roll)
	 {
		 float f_mod = 50 - (m_f_radius * 4); //go slower the bigger we get
         if (f_mod < 0.001f) f_mod = 0.001f;
         //force the way we're facing
		 this->setAngleZ(getAngleZ()+ (vec_current_movement.x*(f_mod*(m_f_roll_mod))*cls_tglo.time()));
		 this->setAngleX(getAngleX()+ (vec_current_movement.z*(f_mod*(m_f_roll_mod))*cls_tglo.time()));
	 }
  
   /*
	 if (cls_tglo.p_cls_input->is_key_down(DIK_I))
	 {
	 char st_crap[255];
	 sprintf(st_crap, "Accel x:%f, z: %f - f_dir is %f. Normal is %s - X Tilt: %f - Z Tilt: %f",vec_accel.x, vec_accel.z,
		 f_dir, print_vector(vec_ground_normal), f_x_tilt, f_z_tilt);
	 log_msg(	st_crap);
     }
	 */
       
         
     if (m_i_draw_mode == C_DRAW_MODE_NORMAL)
	 {
	
 	 this->set_translate_asap(true);
    }
	
     
        if (GetShadowMode() == C_VISUAL_SHADOW_ACCURATE)
            {
                //this is ugly.  Force the angles, build the matrix then put them back.
                float f_hold_x = getAngleX();
                float f_hold_z = getAngleZ();

                setAngleX(f_x_tilt);
                setAngleZ(f_z_tilt);


                //also set the scale temporarily

                 static D3DXVECTOR3 vec_scale_temp;

                 vec_scale_temp.x = f_scale_x;
                 vec_scale_temp.y = f_scale_y;
                 vec_scale_temp.z = f_scale_z;
                    
                 
                 float f_scale;
                 if (get_pos_y() == 0)
                 {
                     f_scale = 1; 
                 }   else
                 {
                     
                     f_scale =  1 - (get_pos_y() - f_ground_height)/30;
                 }

                 set_scale(vec_scale_temp.x *f_scale, 0.02f,vec_scale_temp.z * f_scale );
              
                 translateMovement();
               //copy it to our shadow matrix
               m_mat_shadow = m_objectMatrix;
               m_mat_shadow._42 = f_ground_height+0.03f;
               
               setAngleX(f_hold_x);
               setAngleZ(f_hold_z);
               set_scale(vec_scale_temp.x, vec_scale_temp.y,vec_scale_temp.z );
     }


      if (b_touched_wall)
      {
          //we touched a wall, send a message so the owner can handle it better.  Might want to modify
          //this later to send the normal of the wall it hit too or something
          CMessageHeader msg;
          msg.i_message = C_THING_MSG_TOUCHED_WALL;
          this->receive_message(&msg, NULL);
      }

      if (b_touched_ground)
      {
          //we touched a wall, send a message so the owner can handle it better.  Might want to modify
          //this later to send the normal of the wall it hit too or something
        CMessageHeader msg;
          msg.i_message = C_THING_MSG_TOUCHED_GROUND;
           this->receive_message(&msg, NULL);
      }

        if (b_touched_ceiling)
      {
          //we touched a wall, send a message so the owner can handle it better.  Might want to modify
          //this later to send the normal of the wall it hit too or something
        CMessageHeader msg;
          msg.i_message = C_THING_MSG_TOUCHED_CEILING;
           this->receive_message(&msg, NULL);
      }



     if (s_move.p_blocked_by)
     {
         //send a message to let them know they touched someone, otherwise fireballs
         //can't knnow to die after killing something themselves.. uh.. it's messy
         s_msg.i_message = C_THING_MSG_MOVE;
         s_move.e_collision_type = s_move.p_blocked_by->e_collision_type;
         s_move.f_damage = float(s_move.p_blocked_by->GetTouchDamage());
         s_move.i_collision_special = 0;
         s_move.p_me = s_move.p_blocked_by;
         s_move.p_blocked_by = NULL;
         receive_message(&s_msg, &s_move);
         
     }

	 CVisual::think();
}


void CEnt::RangeTestCallback(const Vector3d<float> &p,float distance,SpherePack *sphere,ViewState state)
{
 // const Vector3d<float> pos = sphere->GetPos();

  //float radius = sphere->GetRadius();

  //DrawCircle( int(pos.x), int(pos.y), int(radius), 0x00FF00);
    
  
     if (this != sphere->GetUserData())
       ((CThing*)sphere->GetUserData())->receive_message(&s_msg, &s_move);
       
     //log_msg("Player close to %s.", ((CThing*)sphere->GetUserData())->get_thing_name());
    
}



void CEnt::dx_draw()
{
   	 CVisualMulti::dx_draw();  
}


void CEnt::dx_restore()
{

	CVisualMulti::dx_restore();
}

void CEnt::dx_create()
{
	CVisualMulti::dx_create();

}

void CEnt::dx_kill()
{
  CVisual::dx_kill();
}					  


