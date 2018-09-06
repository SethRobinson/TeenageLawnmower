//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded.
 //let's preload the mole and hole graphics so we don't jerk later
// PreloadGraphic("model\molehole.x");
// PreloadGraphic("model\mole.x");
 int i_count = 80; //max moles to spawn
 int i_random;
}


void Main()
{
 
 //don't show us, we're just a nothing
 EntSetNoDraw(i_my_id, 1);

 //preload to stop jerks from happening later
 PreloadGraphic("model\\bill20.x");

vector3 vec_pos;
wait(7000); //initial wait period

 again:
  //let's create a gopher thingie
  int i_new = CreateEntityFromTemplate( "bill" , "bill20");  
  //now let's put it in a good place

  vec_pos = GetRandomPointInSubZone("Mow");
  EntSetPosition(i_new, vec_pos);

 
  i_count -= 1;

  i_random = RandomRange(10000, 13000);

  wait(i_random);

 if (i_count > 0)
  {
    goto again;
  }

}


void OnKill()
{
logmsg("Mole spawner killed.");
}