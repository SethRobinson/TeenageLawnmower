//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded.
 
 int i_count = 150; //rocks
 int i_random;
 int i_min = 900;
 int i_max = 1500;

if (g_i_difficulty == 2) //mow master setting
  {
    i_min = 700;
   i_max = 1100;
  }
 
}


void Main()
{
 
 //don't show us, we're just a nothing
 EntSetNoDraw(i_my_id, 1);


vector3 vec_pos;

 again:
  //let's create a rock
  int i_new = CreateEntityFromTemplate( "Creatures" , "SmartRock1");  
  //now let's put it in a good place
 // LogMsg("Making gopher at $vec_pos$.");

  i_count -= 1;

  i_random = RandomRange(i_min, i_max);

  wait(i_random);

 if (i_count > 0)
  {
    goto again;
  }

}


void OnKill()
{

}