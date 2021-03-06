//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.


}

void Main()
{
 

 //don't have to specify the file extension.  the second part is the NAME of the texture set, used internally to figure out
 //how to let items share texture sets

 EntReplaceTexture(i_my_id, "house2_pattern2", "house_2_skin", "house_2_skin_pattern2.dds");

 EntReplaceTexture(i_my_id, "house2_pattern2", "STUCCO4_small", "STUCCO10_small.dds");

 EntReplaceTexture(i_my_id, "house2_pattern2", "CARDBOAR_small", "RUSTILE_small_128.dds");

 EntReplaceTexture(i_my_id, "house2_pattern2", "OLDWOOD_dark_xsmall", "STUCCO10_small.dds");

// EntSetColor(i_my_id,255,255,255,  160,);
 EntSetSmartFade(i_my_id, 1.0, 0.0); //normal ratio on close fade, don't do far fade.

 //turn off physics
 EntSetPhysics(i_my_id, 0); 
}



void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.

}