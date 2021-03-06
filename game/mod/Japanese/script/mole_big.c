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
}


void Main()
{
//turn off physics otherwise it will force him to be above the ground
 EntSetNoDraw(i_my_id, 1); 
Wait(1); //give us time to orient to the ground
EntSetNoDraw(i_my_id, 0); 
  EntSetPhysics(i_my_id, 0);  //turn off physics.  1 is normal. 
 

  float f_scale = 0.0f;
 //let's move the mole down, then up slowly
 
  vector3 vec_pos = EntGetPosition(i_my_id);
  //get the Y of vec_pos

  float f_x = Vector3GetX(vec_pos);   
  float f_y = Vector3GetY(vec_pos);   
  float f_z = Vector3GetZ(vec_pos);   


  f_y -= 4.0;  //get under the ground
  float f_crap;
EntFace(i_my_id, g_id_player); //look at the player when spawned, will kind of look random

start:

 
//1 is true
 SetOnHit(i_my_id, 1);
 //make sure we're showing
 EntSetNoDraw(i_my_id, 0);
i_counter = 0;
repeat:
 f_y += 0.20;
  //set the Y of vec_pos
  vec_pos = MakeVector3(f_x, f_y, f_z);
  EntSetPosition(i_my_id, vec_pos);
  i_counter += 1;
  wait(30);
 
 if (i_counter < 20)
 {
   goto repeat;
 }
 
  //just popped up, let's say something funny

i_wait = RandomRange(400, 1000);

wait (i_wait);
if (i_said_hi == 0)
{
 	f_temp = EntDistance(i_my_id, g_id_player); //look at the player
	
   if (f_temp < 9.0)
  {
 	EntFace(i_my_id, g_id_player); //look at the player

// 	i_saying = RandomRange(1,6);
//	st_wave = "sound\mole_happy$i_saying$.wav";
	//int i_sound = Sound3DPlay(vec_pos, st_wave, 1); //play 1 time
//	SoundEntPlay(i_my_id, st_wave);

	//don't say anything else
//	i_said_hi = 1;
   }
}
i_wait = RandomRange(700, 2700);
 wait (i_wait);

 //let's go back down...

i_counter = 0;


 down_loop:
  f_y -= 0.20;
  //set the Y of vec_pos
  vec_pos = MakeVector3(f_x, f_y, f_z);
  EntSetPosition(i_my_id, vec_pos);
  i_counter += 1;
  wait(30);
 
 if (i_counter < 20)
 {
   goto down_loop;
 }

 //let's wait a bit then do it again
 //no one can hurt us when we're under the ground
 SetOnHit(i_my_id, 0);
 //no reason to keep drawing it under the ground
 EntSetNoDraw(i_my_id, 1);

 i_wait = RandomRange(1500, 7000);
 wait (i_wait);

 goto start;
 EntKill(i_my_id); 
}


void OnHit()
{
  //we got hit.  let's die, 3d sound
  SoundEntPlay(i_my_id, "sound\frag.wav");
 //let the mower (or whatever) fly!
 float f_x_mod = frandomrange(0.0, 10.0);
 float f_z_mod = frandomrange(0.0, 10.0);

 EntForceMod(g_id_player, f_x_mod,15.0, f_z_mod);
i_saying = RandomRange(1,1);
st_wave = "sound\mole_die$i_saying$_big.wav";
 SoundEntPlay(i_my_id, st_wave);

  //don't allow any more hits
  SetOnHit(i_my_id, 0); 
 // EntSetNoDraw(i_my_id, 1);
EntSetUniformScale(i_my_id, 0.0001); //instead of nodraw I do this, so the mole hole won't think I'm hiding 
 
if (g_i_mole_kill == 0)
{
 //chalk one up to the score?
   g_i_accidents += 1;
}
if (g_i_mole_kill == 1)
  {
    //killing moles is good on this level
   g_i_cleaned += 1;
  }
  //create particle system
  
  int i_ps = CreateParticle("script\big_blood.pss");
  vec_pos = EntGetPosition(i_my_id);
  //move particle system to correct place
  EntSetPosition(i_ps, vec_pos);
 CreateScriptFunction("script\damage_mod.c", "DamageQuarter");

  wait(80);

  //turn off any new particles
  ParticleSetPause(i_ps, 1);

  //let the particles that are there fade out or whatever
  wait(1500);

  //end script 
  EntKill(i_ps);
if (g_i_quit == 0)
  {

  if (g_i_end_level_on_accidents != 0)
  {
  if (g_i_accidents >= g_i_end_level_on_accidents)
    {
    // logmsg("have $g_i_accidents$.  Must end on $g_i_end_level_on_accidents$.");  
    //must end level
      g_st_stop_message = "クライアントが現れた";

       int i_level_script = EntGetByName("LevelScript"); //our main script
       g_i_quit = 1; //disable the escape menu  
       EntRunFunction(i_level_script, "FinishLevel");
     }
  }
 }
  EntKill(i_my_id);



}


void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.

}