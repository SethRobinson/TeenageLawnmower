//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
 //if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int i_random;
 float f_ran_dir;
 float f_ran_speed;
 float f_hop_anim_speed = 3.0;
}

void Main()
{
 //don't have to specify the file extension.  the second part is the NAME of the texture set, used internally to figure out
 //how to let items share texture sets

  //put us in a random position
 EntSetDrawTwoSided(i_my_id, true);

 int id_target = EntGetRandomByName("RSPAWN");   

if (id_target == -1)
  {
   logError("Unable to find an rspawn point for squirrel.");
   EntKill(i_my_id);
   return;
  }

 vector3 vec_pos = EntGetPosition(id_target);
 //let's move us to the spot
 EntSetPosition(i_my_id, vec_pos);


 EntSetAnimTexture(i_my_id, "model\ent\squirrel_skin.dds");
 EntSetTurnSpeed(i_my_id, 4.0); //turn slow
 EntSetWallBounce(i_my_id, 1); //1 to bounce off walls and the level boundries

 //for some reason my exporter spits this model out at a weird angle..

// EntRotateBaseFrameYX(i_my_id, 1.40, -1.0);
// vector3 vec_offset = makevector3(0.0, 0.24, 0.0);
// EntSetDrawOffset(i_my_id, vec_offset);

//change direction randomly
f_ran_dir = frandomrange(0.0, 6.3);
EntSetTargetDirection(i_my_id, f_ran_dir);
SetOnHit(i_my_id, 1);
wait(1000);

again:

//******** DEBUG CRAP START


//goto again;

//******** DEBUG CRAP FINISH

 EntSetAnimSpeedMod(i_my_id, f_hop_anim_speed);
 f_ran_speed = FRandomRange(7.0 ,10.0);
 EntSetSpeed(i_my_id, f_ran_speed);
 EntSetAnimByName(i_my_id, "hop", -1); 


 //move foward
 i_random = RandomRange(2000, 5000);
wait(i_random);

//change direction randomly
f_ran_dir = frandomrange(0.0, 6.3);

EntSetTargetDirection(i_my_id, f_ran_dir);

 //move foward
 i_random = RandomRange(2000, 5000);

wait(i_random);

//stop for a bit
EntSetAnimByName(i_my_id, "idle", -1); 
EntSetSpeed(i_my_id, 0.0);
EntSetAnimSpeedMod(i_my_id, 0.5);

 i_random = RandomRange(2000, 5000);
wait(i_random);





goto again;

}

void OnHit()
{
  //we got hit.  let's die, 3d sound
  SoundEntPlay(i_my_id, "sound\frag.wav");

  //don't allow any more hits
  SetOnHit(i_my_id, 0); 
 //don't draw ourself anymore
  EntSetNoDraw(i_my_id, 1);

 

  //chalk one up to the score
  g_i_accidents += 1;
// CreateScriptFunction("script\damage_mod.c", "DamageTiny");

  //create particle system
  
  int i_ps = CreateParticle("script\blood.pss");
  vec_pos = EntGetPosition(i_my_id);
  //move particle system to correct place
  EntSetPosition(i_ps, vec_pos);

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
      g_st_stop_message = "INTERRUPTED BY CLIENT";

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