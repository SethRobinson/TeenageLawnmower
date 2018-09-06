//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded.

 int i_count = 80; //max weed killers
 int i_random;
}


void Main()
{
 
 //preload to stop jerks from happening later
 PreloadGraphic("model\\weedkiller.x");
 
 //don't show us, we're just a nothing
 EntSetNoDraw(i_my_id, 1);


vector3 vec_pos;
 again:
 i_random = RandomRange(8000, 60000);
   Wait(i_random);
 
  int i_new = CreateEntityFromTemplate( "Special" , "WeedKiller");  
  //now let's put it in a good place

  vec_pos = GetRandomPointInSubZone("Mow");
  EntSetPosition(i_new, vec_pos);

 
  i_count -= 1;

 

 if (i_count > 0)
  {
    goto again;
  }

}


void OnKill()
{
LogMsg("Wow, Weed Killer spawner killed.");
}