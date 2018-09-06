//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
 //if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int i_random;
 float f_ran_dir;
 float f_ran_speed;
 float f_hop_anim_speed = 3.0;
 vector3 vec_pos;
 int i_state = 0; //1 is no head
 int i_ps_drip = -1;
}

void think()
{
again:


//******** DEBUG CRAP FINISH

 EntSetAnimSpeedMod(i_my_id, f_hop_anim_speed);

 f_ran_speed = FRandomRange(6.0 ,9.0);

  if (i_state == 1)
  {
    f_ran_speed = FRandomRange(9.0 ,16.0);
  }

 EntSetSpeed(i_my_id, f_ran_speed);
 EntSetAnimByName(i_my_id, "Walk", -1); 


 //move foward
 i_random = RandomRange(2000, 5000);
wait(i_random);

//change direction randomly
f_ran_dir = frandomrange(0.0, 6.3);

EntSetTargetDirection(i_my_id, f_ran_dir);
//change direction randomly
i_random = randomrange(1, 10);

if  (i_random > 5)
  {
  //look at the player
  if (g_id_player != -1)
   {
     EntFace(i_my_id, g_id_player); //look at the player
   }
  }
 //move foward
 i_random = RandomRange(2000, 5000);

wait(i_random);

if (i_state == 0)
 {
//stop for a bit
EntSetAnimByName(i_my_id, "idle", -1); 
EntSetSpeed(i_my_id, 0.0);
EntSetAnimSpeedMod(i_my_id, 0.3);
i_random = randomrange(1, 6);
if (i_random == 3)
   {
     //bark
     SoundEntPlay(i_my_id, "sound\quack.wav");

   }

 i_random = RandomRange(2000, 5000);
wait(i_random);
}

goto again;

}

void Main()
{
	
 //preload to stop jerks from happening later
 PreloadGraphic("model\\duck_headonly.x");
 Sound3DPreload("sound\quack.wav");
	
EntSetDrawTwoSided(i_my_id, true);

//EntSetShadow(i_my_id, 1); 
 //don't have to specify the file extension.  the second part is the NAME of the texture set, used internally to figure out
 //how to let items share texture sets
  EntSetModel(i_my_id, "model\ent\duck.rtm");

 EntSetAnimTexture(i_my_id, "model\ent\duck_skin_blood.dds");

 //set a random size

 float f_size = frandomrange(1.2, 1.5);

 EntSetUniformScale(i_my_id, f_size); //random duck size

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
EntSetSkid(i_my_id, 0.5); //accel fast
think();
}

void OnHit()
{
 //we got hit.  let's die, 3d sound
  SoundEntPlay(i_my_id, "sound\frag.wav");
if (i_state == 0)
{
EntSetModel(i_my_id, "model\ent\duck_nohead.rtm");
}
  //don't allow any more hits

  SetOnHit(i_my_id, 0); 

 //don't draw ourself anymore
if (i_state == 1)
 {
 EntSetNoDraw(i_my_id, 1);
  if (i_ps_drip != -1)
   {
     ParticleSetPause(i_ps_drip, 1);
  }

}
 
if (i_state == 0)
{

//create a duck head flying
int i_duckhead = CreateEntityFromTemplate("golf", "duck_headonly");
vec_pos = EntGetPosition(i_my_id);
EntSetPosition(i_duckhead, vec_pos);
vec_pos = EntGetScale(i_my_id);
EntSetScale(i_duckhead, vec_pos);

  //chalk one up to the score
  g_i_accidents += 1;
// CreateScriptFunction("script\damage_mod.c", "DamageTiny");
}

  //create particle system
  
   int i_ps = CreateParticle("script\blood.pss");
  vec_pos = EntGetPosition(i_my_id);
  //move particle system to correct place
  EntSetPosition(i_ps, vec_pos);

  wait(80);
  //turn off any new particles
  ParticleSetPause(i_ps, 1);
  ParticleKillTimer(i_ps, 2000); //kill in two seconds
  //let the particles that are there fade out or whatever
  wait(300);
 if (i_ps_drip != -1)
   {
     EntKill(i_ps_drip);
   i_ps_drip = -1;
  }
 
if (i_state == 1)
   {
     /kill this sucker now
     EntKill(i_my_id);
      return;

   }


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

  SetOnHit(i_my_id, 1); 
if (i_state == 0)
  {
    i_state = 1; 
  }

 //create another particle system for blood coming out of the duck's neck

 i_ps_drip = CreateParticle("script\duck_blood.pss");
// ParticleSetFrameFollow(i_ps_drip, 1);
  EntSetFollow(i_ps_drip, i_my_id;

think();

//EntKill(i_my_id);
}


void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.
 

  if (i_ps_drip != -1)
   {
     EntKill(i_ps_drip);
   i_ps_drip = -1;
  }
}