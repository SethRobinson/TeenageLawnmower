//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded.

 int i_count = 80; //max toolboxes
 int i_random;
}


void Main()
{
 PreloadGraphic("model\\toolbox.x");
 
 //don't show us, we're just a nothing
 EntSetNoDraw(i_my_id, 1);


vector3 vec_pos;
wait(5000); //initial wait period

 again:
  //let's create a gopher thingie
  int i_new = CreateEntityFromTemplate( "things" , "toolboxD");  
  //now let's put it in a good place

  vec_pos = GetRandomPointInSubZone("Mow");
  EntSetPosition(i_new, vec_pos);

 
  i_count -= 1;

  i_random = RandomRange(8000, 12000);

  wait(i_random);

 if (i_count > 0)
  {
    goto again;
  }

}


void OnKill()
{
logmsg("Toolbox spawner killed.");
}