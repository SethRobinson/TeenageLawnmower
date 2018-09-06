//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.


}

void Main()
{
 

 //don't have to specify the file extension.  the second part is the NAME of the texture set, used internally to figure out
 //how to let items share texture sets

 EntReplaceTexture(i_my_id, "1garbera_orange", "garbera_pink_64", "garbera_orange_64.dds");
 EntReplaceTexture(i_my_id, "1garbera_orange", "7leaves_128", "9leaves_128.dds");
 EntSetDrawTwoSided(i_my_id, true);
 SetOnTouch(i_my_id, true);

 //turn off physics
 EntSetPhysics(i_my_id, false); 
}


void OnTouch()
{
  //we got touched.
  SetOnTouch(i_my_id, 0);
  EntSetNoDraw(i_my_id, true);
   SoundEntPlay(g_id_player, "sound\tree_dead.wav");
     //let's apply a cool particle system to us
   int i_ps = CreateParticle("script\weed_die.pss");
   EntSetFollow(i_ps, i_my_id);
   wait(500);
   ParticleSetPause(i_ps, 1);
   wait(2000);
   EntKill(i_ps);
   EntKill(i_my_id);
}

void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.

}