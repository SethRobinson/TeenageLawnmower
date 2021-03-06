//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.

int i_rand;
float f_dir;
float f_speed;
}


void Think()
{
 again:
  //let's move crazy!
  
  //pick a random dir
  f_dir = frandomrange(0.0, 6.3);
  EntSetTargetDirection(i_my_id, f_dir);
  i_rand = randomrange(1, 2);
   if (i_rand == 1)
    {
     //point directly at player
      if (g_id_player != -1)
   {
     EntFace(i_my_id, g_id_player); //look at the player
   }	
    }

  
 i_rand = randomrange(2000, 3300);
  wait(i_rand);
  goto again; 
	
	
}

void Main()
{
int i_host = GetHost();
if (i_host == 0)
  {
   //we're in the editor, don't move around
    return;
  }

//turn off physics
 //EntSetPhysics(i_my_id, 0);
 EntSetRadiusMod(i_my_id, 0.4); //much smaller collision area
//let's respond to taking damage.
 
 //1 is true
 SetOnTouch(i_my_id, 1);
 wait_to_start:
 i_rand = randomrange(800, 1300);
  wait(i_rand);
 if (g_i_spawn_count != 10)
   {
     goto wait_to_start; //game hasn't started, don't move around yet 
  } 

 f_speed = fRandomRange(5.0, 25.0); 
 EntSetSpeed(i_my_id, f_speed);

 Think();
} 

void OnTouch()
{
  //we got touched.
//let the mower (or whatever) fly!
 float f_x_mod = frandomrange(0.0, 10.0);
 float f_z_mod = frandomrange(0.0, 10.0);

 EntForceMod(g_id_player, f_x_mod,45.0, f_z_mod);

 //we got hit.  let's die, 3d sound
 SoundEntPlay(i_my_id, "sound\skel_attack.wav");
// ParticleSetPause(i_ps, 1); 
 CreateScriptFunction("script\damage_mod.c", "DamageMedium");

  //don't allow any more hits
  SetOnTouch(i_my_id, 0); 
  wait(600);
  SetOnTouch(i_my_id, 1); 
  Think();
}

void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.

}