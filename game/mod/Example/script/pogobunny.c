//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
 //if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int i_random;
 float f_ran_dir;
 float f_ran_speed;
 vector3 vec_pos;
 float f_x;
 float f_y;
 float f_z;
}

void Think()
{
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
  f_ran_speed = FRandomRange(2.0 ,3.0);
  EntSetSpeed(i_my_id, f_ran_speed);
  EntSetAnimByName(i_my_id, "attack", -1); 
 }

if (i_random == 2)
{
   f_ran_speed = 12; //FRandomRange(5.0 , 7.0);
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
EntSetAnimByName(i_my_id, "flip", 0); 
EntSetSpeed(i_my_id, 0.0);
// i_random = RandomRange(1000, 4000);
 i_random = 2000;
 wait(i_random);
 goto again;


}


void Main()
{
 //don't have to specify the file extension.  the second part is the NAME of the texture set, used internally to figure out
 //how to let items share texture sets

 EntSetAnimTexture(i_my_id, "model\pogo1.bmp");
 EntSetTurnSpeed(i_my_id, 0.01); //turn slow

 //the model needs to be rotated 90 degrees to the right
 EntRotateBaseFrameYX(i_my_id, 1.57, 0.0f); // Y and X rotation to apply, Y (dir) applied first.
 EntSetAnimSpeedMod(i_my_id, 0.3); //we can make the animation play faster or slower with this.  1 is no change
 //this particular model is too low into the ground.  Instead of editting the md2 let's just set an offset.  (offset command only
 //works correctly with Y at the moment)
 vector3 vec_offset = MakeVector3(0.0,2.6,0.0); 
 EntSetDrawOffset(i_my_id, vec_offset);

 float f_size = frandomrange(0.9, 1.1);
 EntSetUniformScale(i_my_id, f_size); //change size randomly, just a bit.
 EntSetWallBounce(i_my_id, 1); //1 to bounce off walls and the level boundries
 EntSetSkid(i_my_id, 0.5); //accel fast


 EntSetShadow(i_my_id, 1); //draw a realtime shadow for this object

int i_host = GetHost();
if (i_host == 0)
  {
   //we're in the editor, don't move around
    return;
  }


SetOnTouch(i_my_id, 1);

Think();
}

void OnTouch()
{
SoundEntPlay(i_my_id, "sound\drop.wav");
   //dog touched by mower
  //don't allow any more hits
  SetOnTouch(i_my_id, 0); 

 //this gives errors in the editor, ignore it. The editor has no TLM specific vars or code in it.
 vec_pos = EntForceGet(g_id_player);


f_x = Vector3GetX(vec_pos);
f_y = Vector3GetY(vec_pos);
f_z = Vector3GetZ(vec_pos);

f_x *= 100;
f_y *= 100;
f_z *= 100;

EntForceMod(i_my_id, f_x,25.0, f_z);

//uncomment to hurt the player
//CreateScriptFunction("script\damage_mod.c", "DamageQuarter");
wait(30);

 SoundEntPlay(i_my_id, "sound\beagle_hurt.wav");
 wait(1000);

 //allow ourselves to be 'touched again'
 SetOnTouch(i_my_id, 1); 
 EntRunFunction(i_my_id, "Think"); //this way, we don't recursively call it
}

void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.
}