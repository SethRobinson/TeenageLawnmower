//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded.
 //let's preload the mole and hole graphics so we don't jerk later

}


void Main()
{
 

 EntSetPhysics(i_my_id, 0); 

int i_host = GetHost();
if (i_host == 0)
  {
   //we're in the editor, don't hide us
    return;
  }

 //don't show us, we're just a nothing
 EntSetNoDraw(i_my_id, 1);

}


void OnKill()
{

}