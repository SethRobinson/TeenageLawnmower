//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
Vector3 vec_pos;


}

void Main()
{
again: 
///play random sounds
 vec_pos = GetRandomPointInZone(0); //the 0 says all tiles ok, 1 would be non-hard tiles only
  Sound3DPlay(vec_pos, "sound\gong.wav", 1);
int i_random = RandomRange(7000, 14000);
wait(i_random);
goto again;
}

void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.

}