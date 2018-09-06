//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
int i_ps;
int i_rand;

}


void Main()
{

if (g_i_test_mode == 0)
{
 return;
}
if (g_i_quit == 1)
  {
    //already quitting, can't use this
    return;

  }  

loop:
i_ps = CreateParticle("script\big_dirt.pss");
 vector3 vec_pos = EntGetPosition(g_id_player);
//move particle system to correct place

//EntSetPosition(i_ps, vec_pos);
EntSetFollow(i_ps, g_id_player);
wait(8000);
ParticleSetPause(i_ps, 1);
wait(10);
EntKill(i_ps);



goto loop; 
}



void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.

}