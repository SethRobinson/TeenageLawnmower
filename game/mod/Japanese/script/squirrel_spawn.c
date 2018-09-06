//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded.
 //let's preload the mole and hole graphics so we don't jerk later
// PreloadGraphic("model\ent\arm.rts");
 int i_count = g_i_spawn_count; //squirrels to spawn
 int i_random;

 int i_min = 3000;
int i_max = 5000;
}


void Main()
{
 if (i_count > 30)
  {
     //speed things up a bit
      i_min = 50;
     i_max = 200;

   }
 //don't show us, we're just a nothing
 EntSetNoDraw(i_my_id, 1);

randomrange(i_min, 5000);

vector3 vec_pos;

 again:
  //let's create a gopher thingie
  int i_new = CreateEntityFromTemplate( "Creatures" , "squirrel");  
  //now let's put it in a good place
 // LogMsg("Making gopher at $vec_pos$.");

  i_count -= 1;

  i_random = RandomRange(i_min, 5000);

  wait(i_random);

 if (i_count > 0)
  {
    goto again;
  }

}


void OnKill()
{
logmsg("Skele spawner killed.");
}