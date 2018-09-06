//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
}

void Main()
{
 EntSetName(i_my_id, "TREE"); 
 //turn off physics
 EntSetPhysics(i_my_id, 0); 
}

void OnBurn()
{
  //don't actually do anything when we get hit
}
void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.


}
