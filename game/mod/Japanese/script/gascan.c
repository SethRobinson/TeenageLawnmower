//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.


}

void Main()
{
 
//turn off physics
EntSetPhysics(i_my_id, 0; 
//scale the dirt up

 //let's respond to taking damage.
 
 //1 is true
 SetOnTouch(i_my_id, 1);
 float f_scale = 0.0f;

 scale_start:
 f_scale += 0.03f;
 
EntSetUniformScale(i_my_id, f_scale); 
 wait (50);

 if (f_scale < 1)
 {
   //loop around
   goto scale_start;  
 }

 
int i_host = GetHost();
if (i_host == 0)
  {
   //we're in the editor, don't move around
    return;
  }
}

void OnTouch()
{
  //we got touched.
  //if another powerup is in effect, let's kill it.

  ShowMessage("ガソリン缶を拾った！");
  //play sound effect
  SoundPlay("sound\powerup.wav");
  EntMowerSetSpeed(g_id_player, 40.0);
  //don't allow any more hits
  SetOnTouch(i_my_id, 0); 
  EntSetName(i_my_id, "SPEED_MOD");
//let's apply a cool particle system to the mower

  int i_ps = CreateParticle("script\gas.pss");

  //we want the particle system to follow around the mower, but we need it's id to do that
  int i_player = EntGetByName("Player");

  EntSetFollow(i_ps, i_player);
  //Let's have the effect move with the mower, rather than leave a trail (1 is true)
  //ParticleSetFrameFollow(i_ps, 1);


  //shrink the can down and then stop drawing it

  loop:
  f_scale -= 0.03;
  EntSetUniformScale(i_my_id, f_scale); 
  wait(30);
  if (f_scale > 0.04)
  {
   goto loop;
  }
  //don't draw ourself anymore
  EntSetNoDraw(i_my_id, 1);

  wait(6000);
 
 EntSetName(i_my_id, "GAS");

 int i_temp = EntGetByName("SPEED_MOD");
  if (i_temp == -1)
{
 //only do this if nobody else is playing with the speed too
 EntMowerSetSpeed(g_id_player, -100.0); //-100 means set to default
}
 //turn off any new particles
  ParticleSetPause(i_ps, 1);
  //let the particles that are there fade out or whatever
  wait(6000);

  EntKill(i_ps);
  EntKill(i_my_id);

}

void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.

}