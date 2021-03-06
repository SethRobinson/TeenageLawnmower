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

void Main()
{
 //don't have to specify the file extension.  the second part is the NAME of the texture set, used internally to figure out
 //how to let items share texture sets
 int i_skin = randomrange(1,2);

 //the model needs to be rotated 90 degrees to the right
EntRotateBaseFrameYX(i_my_id, 1.57, 1.57f); // Y and X rotation to apply, Y (dir) applied first.
EntSetTurnSpeed(i_my_id, 4.0); //turn slow

//vector3 vec_offset = MakeVector3(0.0,2.6,0.0);

//EntSetDrawOffset(i_my_id, vec_offset);
f_ran_dir = frandomrange(0.0, 6.3);

EntSetTargetDirection(i_my_id, f_ran_dir);

vec_pos = EntForceGet(g_id_player);


f_x = Vector3GetX(vec_pos);
f_y = Vector3GetY(vec_pos);
f_z = Vector3GetZ(vec_pos);

f_x *= 100;
f_y *= 100;
f_z *= 100;

EntForceMod(i_my_id, f_x,15.0, f_z);

}



void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.

}