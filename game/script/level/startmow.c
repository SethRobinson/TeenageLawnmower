void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
int i_ran; //I made a mistake, this is called by CreateEntFunction() which calls a function instantly, I had it call Main() which gets run upon creation too
//so I use this so it won't run twice because I'm too lazy to change the scripts that reference this.
}

void Main()
{
if (i_ran == 1)
  {
    return;
  }

i_ran = 1;
EntSetBounceSound(g_id_player, "sound\jump_land.wav", 0.3, 300); //ent, wave, min height to play sound, delay between sounds
g_st_stop_message = "";

CreateEntScript("Quicktip", "script\quicktips.c");
g_f_damage_mod = 1.0;

if (g_i_cur_mower == 1)
  {
    //switch skins
    g_f_damage_mod = 0.5; //only takes half the damage, wow    
  // EntReplaceTexture(g_id_player, "excal", "REDFIBER_128", "lawnmower_skin_new.dds");

   EntSetModel(g_id_player, "model\lawnmower_new.x");
  }



}

void OnKill()
{

}
