//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
  int i_said_hi = 0;
  int i_counter;
  int i_wait;
  int i_saying;
  string st_wave;
  float f_temp;
  int i_ps = -1;
  float f_start_y;
float f_x;
float f_y;
 float f_z;
 int i_temp;
  vector3 vec_pos;
}

void GoDown()
{
 //let's go back down...
  ParticleSetPause(i_ps, 0); 

i_counter = 0;

 down_loop:
  f_y -= 0.16f;
  //set the Y of vec_pos
  vec_pos = MakeVector3(f_x, f_y, f_z);
  EntSetPosition(i_my_id, vec_pos);
  i_counter += 1;
  wait(30);
 
 if (f_y > f_start_y)
 {
   goto down_loop;
 }
  ParticleSetPause(i_ps, 1); 
}

void Main()
{
  EntSetNoDraw(i_my_id, 1);
  i_wait = randomrange(2000, 5000);
  wait(i_wait);
  start_again:
 //find good position
  
 vec_pos = GetRandomPointInSubZone("Mow");

 // vec_pos = EntGetPosition(g_id_player);

  //randomly move it a bit
//   f_x = Vector3GetX(vec_pos);   
//   f_y = Vector3GetY(vec_pos);   
//   f_z = Vector3GetZ(vec_pos);   

  
//    f_x += FRandomRange(-3.0,3.0);
//    f_z += FRandomRange(-3.0,3.0);

   //make the vector again
//  vec_pos = MakeVector3(f_x, f_y, f_z);

  //is it valid?
//  i_temp = IsPositionValid(vec_pos, "Mow", 1); //must be in mow zone, must not be "hard"
 
// if (i_temp == 0)
//  {
  // logmsg("bad position. ");
//  wait(1000);
//  goto start_again;
//  }
 
  //get true ground height at this pos
// f_y = GetGroundHeight(f_x, f_z);


   //make the vector again
//  vec_pos = MakeVector3(f_x, f_y, f_z);
 EntSetPosition(i_my_id, vec_pos);

 EntSetAnimTexture(i_my_id, "model\ent\bone_xsmall.dds");
//turn off physics otherwise it will force him to be above the ground
//  Wait(1); //give us time to orient to the ground
  EntSetPhysics(i_my_id, 0);  //turn off physics.  1 is normal. 
  //create particle system in the right place


  float f_scale = 0.0f;
 //let's move the mole down, then up slowly
 
vec_pos = EntGetPosition(i_my_id);


  i_ps = CreateParticle("script\skel_dirt.pss");
  EntSetPosition(i_ps, vec_pos);

  //get the Y of vec_pos


  f_x = Vector3GetX(vec_pos);   
  f_y = Vector3GetY(vec_pos);   
 f_z = Vector3GetZ(vec_pos);   

  f_y -= 3.7;  //get under the ground
f_start_y = f_y; //remeber this for later 
 float f_crap;
EntFace(i_my_id, g_id_player); //look at the player when spawned, will kind of look random

start:
wait(1000); //dirt build up...
  EntSetNoDraw(i_my_id, 0);

 //if we're close to the player, play scary sound
//f_temp = EntDistance(i_my_id, g_id_player);
f_temp = 1; //hack to always play sound	
   if (f_temp < 9.0)
  {
     SoundEntPlay(i_my_id, "sound\skel.wav");
  }
//1 is true
 SetOnHit(i_my_id, 1);
 //make sure we're showing
 EntSetNoDraw(i_my_id, 0);
i_counter = 0;
repeat:
 f_y += 0.16f;
  //set the Y of vec_pos
  vec_pos = MakeVector3(f_x, f_y, f_z);
  EntSetPosition(i_my_id, vec_pos);
  i_counter += 1;
  wait(30);
 
 if (i_counter < 20)
 {
   goto repeat;
 }
 
  ParticleSetPause(i_ps, 1); 
i_wait = RandomRange(700, 2700);
 wait (i_wait);

GoDown();
 //let's wait a bit then do it again
 //no one can hurt us when we're under the ground
 SetOnHit(i_my_id, 0);
 //no reason to keep drawing it under the ground
 EntSetNoDraw(i_my_id, 1);

 //to quit out instead of loop
 wait(700);
 Main();
 
 return;
 i_wait = RandomRange(1500, 7000);
 wait (i_wait);
  ParticleSetPause(i_ps, 0); 

 goto start;
 EntKill(i_my_id); 
}



void OnHit()
{

 //let the mower (or whatever) fly!
 float f_x_mod = frandomrange(0.0, 10.0);
 float f_z_mod = frandomrange(0.0, 10.0);

 EntForceMod(g_id_player, f_x_mod,15.0, f_z_mod);

 //we got hit.  let's die, 3d sound
 SoundEntPlay(i_my_id, "sound\skel_attack.wav");
// ParticleSetPause(i_ps, 1); 
 CreateScriptFunction("script\damage_mod.c", "DamageMedium");

  //don't allow any more hits
  SetOnHit(i_my_id, 0); 
  GoDown();  
  Main();
}


void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.
 //kill particle system too
  EntKill(i_ps);
}