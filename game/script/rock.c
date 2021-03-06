//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 
   int i_temp;
  vector3 vec_pos;
float f_x;
float f_y;
float f_z;
float f_vel;
int i_count = 0;
}

void KillRock()
{
SetOnTouch(i_my_id, 0); 
  EntSetColor(i_my_id, 255,255,255,230);
  wait(30);
  EntSetColor(i_my_id, 255,255,255,200);
  wait(30);
  EntSetColor(i_my_id, 255,255,255,170);
  wait(30);
  EntSetColor(i_my_id, 255,255,255,150);
  wait(30);
  EntSetColor(i_my_id, 255,255,255,130);
  wait(30);
  EntSetColor(i_my_id, 255,255,255,100);
  wait(30);
  EntSetColor(i_my_id, 255,255,255,60);
  wait(30);
  EntSetColor(i_my_id, 255,255,255,30);


  EntKill(i_my_id);
}

void WaitToDie()
{
 SetOnTouch(i_my_id, 1);
 //no reason to keep drawing it under the ground
 //EntSetNoDraw(i_my_id, 1);

  
  again:
  
  wait(600);
  
  i_count += 1;

  if (i_count > 5)
   {
     f_vel = EntVelocityGet(i_my_id); //get overall speed displacement
     //logmsg("Vel is $f_vel$.");
      if (f_vel < 0.01)
      {
         //wow this rock is going really slow.  kill it.
          KillRock();
      }
   }

if (i_count < 15)
   {
     goto again;
   }
  KillRock();

}

void Main()
{
 
 //Let's make us more bounce
  EntSetBounce(i_my_id, 0.30);
  EntSetTurnSpeed(i_my_id, 100.0); //turn instant
  EntSetMaxSpeed(i_my_id, 0.2);
  //More slidely
   EntSetGroundFriction(i_my_id, 0.0003);
  EntSetShadow(i_my_id, 1); 
 EntSetBounceSound(i_my_id, "sound\rock_bounce.wav", 0.3, 300); //ent, wave, min height to play sound, delay between sounds
//  PlaySound(sound\rock
  float f_scale = frandomrange(0.6, 1.0);

  EntSetUniformScale(i_my_id, f_scale);

  //let's make it roll
  EntSetSpecial(i_my_id, 0, 0.7); //0=roll, 1 is normal speed.
  EntSetCollisionType(i_my_id, 0); //0 is sphere.  Moves the rock above ground 
 EntSetSphereTreeMod(i_my_id, 1.5); //otherwise the player can go under us.  His spheretree has been modded to be tiny so
//items don't get picked up until he's in them.  Engine retro fit required this.

// EntSetNoDraw(i_my_id, 1);
 vec_pos = GetRandomPointInSubZone("Rocks");

  //modify the y a bit

  f_x = Vector3GetX(vec_pos);   
  f_y = Vector3GetY(vec_pos);   
  f_z = Vector3GetZ(vec_pos);   

  
   f_y += 1; //drop it from high up

   //make the vector again
  vec_pos = MakeVector3(f_x, f_y, f_z);

  EntSetPosition(i_my_id, vec_pos);
 

  EntSetPhysics(i_my_id, 1);  //turn on physics.  1 is normal. 
  //create particle system in the right place
  WaitToDie();
}



void OnTouch()
{

 //let the mower (or whatever) fly!
// float f_x_mod = frandomrange(0.0, 10.0);
// float f_z_mod = frandomrange(0.0, 10.0);

//ComputeForce(g_id_player, i_my_id); //if these two collided, 

vec_pos = EntForceGet(i_my_id);

f_x = Vector3GetX(vec_pos);
f_y = Vector3GetY(vec_pos);
f_z = Vector3GetZ(vec_pos);

f_x *= 100;
f_y *= 100;
f_z *= 100;


//logmsg("Got $f_x$, $f_y$, $f_z$.");

EntForceMod(g_id_player, f_x,10.0, f_z);
//EntForceMod(g_id_player, 30.0,20.0, f_z);

//make the rock bounce off now

 //we got hit.  let's die, 3d sound
 SoundEntPlay(i_my_id, "sound\explode.wav");

 CreateScriptFunction("script\damage_mod.c", "DamageSmall");

  //don't allow any more hits
  SetOnTouch(i_my_id, 0); 
//  EntSetSpecial(i_my_id, 0, 0.0); //0=roll, 1 is normal speed.
  EntFaceAway(i_my_id, g_id_player); //look away from the player

wait(1000);

//int i_ps = CreateParticle("script\rock_dirt.pss");
//  vec_pos = EntGetPosition(i_my_id);
  //move particle system to correct place
//  EntSetPosition(i_ps, vec_pos);
//  EntSetNoDraw(i_my_id, 1); //hide us
//  wait(80);
  //turn off any new particles
//  ParticleSetPause(i_ps, 1);

  //let the particles that are there fade out or whatever
//  wait(4000);

  //end script 
//  EntKill(i_ps);
//  EntKill(i_my_id);
WaitToDie();

}


void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.
 //kill particle system too

}