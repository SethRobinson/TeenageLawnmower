//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded.
 //let's preload the mole and hole graphics so we don't jerk later
// PreloadGraphic("model\molehole.x");
// PreloadGraphic("model\mole.x");
 int i_count = 280; //max  to spawn
 int i_random;
float f_x;
float f_y;
float f_z;
}


void Main()
{
 
 //don't show us, we're just a nothing
 EntSetNoDraw(i_my_id, 1);


vector3 vec_pos;
wait(2000); //initial wait period

 again:
  //let's create a gopher thingie
  int i_new = CreateEntityFromTemplate( "cafe" , "garbage");  
  //now let's put it in a good place

  vec_pos = GetRandomPointInSubZone("Mow");
  
  //raise the height a bit
  
 //modify the y a bit

  f_x = Vector3GetX(vec_pos);   
  f_y = Vector3GetY(vec_pos);   
  f_z = Vector3GetZ(vec_pos);   

  
   f_y += 20; //drop it from high up

   //make the vector again
  vec_pos = MakeVector3(f_x, f_y, f_z);

EntSetPosition(i_new, vec_pos);

 
  i_count -= 1;

  i_random = RandomRange(3000, 5000);

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