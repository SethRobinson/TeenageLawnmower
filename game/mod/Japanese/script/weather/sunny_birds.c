//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 logmsg("Initting weather script.");
 string st_sound;
Vector3 vec_pos;
}


//this is a weather script.  Just play random noises.

void Main()
{
 EntSetPauseLevel(i_my_id, 200); //nothing can delete us
   again:
   int i_wait = randomrange(6000, 10000);
  wait(i_wait);

  int i_rand = randomrange(1, 4);
  st_sound = "sound\bird$i_rand$.wav";

  //play from a random location
  vec_pos = GetRandomPointInZone(0); //the 0 says all tiles ok, 1 would be non-hard tiles only
  Sound3DPlay(vec_pos, st_sound, 1);

  goto again;
}

void OnKill()
{
 logmsg("Closing weather script.");	
}
