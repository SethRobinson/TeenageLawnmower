//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
 //if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int i_random;
 float f_ran_dir;
 float f_ran_speed;

 float f_anim_run_speed = 1.4;
 float f_anim_walk_speed = 2.0;
  vector3 vec_pos;
float f_x;
float f_y;
float f_z;
   int i_temp;
}


void Think()
{


SetOnTouch(i_my_id, 1);


again:

//******** DEBUG CRAP START

//change direction randomly
//f_ran_dir = frandomrange(0.0, 6.3);
//EntSetTargetDirection(i_my_id, f_ran_dir);
//wait(1000);
//goto again;

//******** DEBUG CRAP FINISH

i_random = randomRange(1, 2); //run or walk?

if (i_random == 1)
 {
 EntSetAnimSpeedMod(i_my_id, f_anim_walk_speed);
 f_ran_speed = FRandomRange(4.0 ,6.0);
 EntSetSpeed(i_my_id, f_ran_speed);
EntSetAnimByName(i_my_id, "walk", -1); 
 }

if (i_random == 2)
  {
 EntSetAnimSpeedMod(i_my_id, f_anim_run_speed);

 f_ran_speed = 12; //FRandomRange(15.0 , 18.0);
 EntSetSpeed(i_my_id, f_ran_speed);
 EntSetAnimByName(i_my_id, "run", -1); 
}
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
 EntSetAnimSpeedMod(i_my_id, 0.4);
i_random = randomrange(1, 3);
if (i_random == 3)
   {
     //bark
     SoundEntPlay(i_my_id, "sound\doberman_bark.wav");

   }

EntSetSpeed(i_my_id, 0.0);
 i_random = RandomRange(1000, 4000);
wait(i_random);

goto again;

}
void Main()
{
 Sound3DPreload("doberman_mad.wav");
 Sound3DPreload("doberman_bark.wav");
 
EntSetMaxSpeed(i_my_id, 0.2);
  
 EntSetShadow(i_my_id, 1); 
 //don't have to specify the file extension.  the second part is the NAME of the texture set, used internally to figure out
 //how to let items share texture sets
EntSetDrawTwoSided(i_my_id, true);

 EntSetAnimTexture(i_my_id, "model\ent\doberman_s_skin.dds");
 EntSetTurnSpeed(i_my_id, 1.0); //turn slow
 EntSetSkid(i_my_id, 0.5); //accel fast

 //for some reason my exporter spits this model out at a weird angle..

// EntRotateBaseFrameYX(i_my_id, 1.40, -1.0);
// vector3 vec_offset = makevector3(0.0, 0.24, 0.0);
// EntSetDrawOffset(i_my_id, vec_offset);
 EntSetWallBounce(i_my_id, 1); //1 to bounce off walls and the level boundries
think();
}

void OnTouch()
{
SoundEntPlay(i_my_id, "sound\drop.wav");
   //dog touched by mower
  //don't allow any more hits
  SetOnTouch(i_my_id, 0); 
vec_pos = EntForceGet(g_id_player);


f_x = Vector3GetX(vec_pos);
f_y = Vector3GetY(vec_pos);
f_z = Vector3GetZ(vec_pos);

f_x *= 100;
f_y *= 100;
f_z *= 100;

EntForceMod(i_my_id, f_x,25.0, f_z);
CreateScriptFunction("script\damage_mod.c", "DamageQuarter");
wait(30);

SoundEntPlay(i_my_id, "sound\doberman_mad.wav");
wait(1000);

 SetOnTouch(i_my_id, 1); 
  EntRunFunction(i_my_id, "Think"); //this way, we don't recursively call it
}

void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.

}