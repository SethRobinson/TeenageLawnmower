//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.


}

void Main()
{
 

 //don't have to specify the file extension.  the second part is the NAME of the texture set, used internally to figure out
 //how to let items share texture sets

 EntReplaceTexture(i_my_id, "tablestone2_1", "tombstone2", "tombstone1.dds");

 //turn off physics
 EntSetPhysics(i_my_id, 0); 
}



void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.

}