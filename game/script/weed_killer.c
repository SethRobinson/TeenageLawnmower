//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.


}

void Main()
{
 
//turn off physics
 EntSetPhysics(i_my_id, 0); 
//scale the dirt up

 //let's respond to taking damage.
 
 float f_scale = 0.0f;

 scale_start:
 f_scale += 0.03f;
 
EntSetUniformScale(i_my_id, f_scale); 
 wait (50);
 if (f_scale < 0.7)
 {
   //loop around
   goto scale_start;  
 }

//1 is true
 SetOnTouch(i_my_id, 1);
 scale_start2:
 f_scale += 0.03f;
 
EntSetUniformScale(i_my_id, f_scale); 
 wait (50);
 if (f_scale < 1)
 {
   //loop around
   goto scale_start2;  
 }
 
  wait(8000)

 //shrink, they missed the chance to get us

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
 

EntKill(i_my_id);
 
}

void OnTouch()
{
  //we got touched.
  //if another powerup is in effect, let's kill it.

//LogMsg("g_i is $g_i_weed_killer$ and max is $C_MAX_WEED_KILLERS$");
if (g_i_weed_killer >= C_MAX_WEED_KILLERS)
{
  SetOnTouch(i_my_id, 0); 
  ShowMessage("Can't hold more!");
  Wait(1000);
  SetOnTouch(i_my_id, 1); 
  return;	
 }


  ShowMessage("Weed Killer!");
  //play sound effect
  SoundPlay("sound\weed_pickup.wav");
 //EntMowerSetSpeed(g_id_player, 40.0);
  //don't allow any more hits
  SetOnTouch(i_my_id, 0); 
  int i_weed = CreateEntScript("WEEDY", "script\weed_function.c");
  EntRunFunction(i_weed, "AddWeedKiller");
 
//CreateScriptFunction("script\damage_mod.c", "HealDamageHalf");



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
  
  if (g_i_weed_killer_tip == 1)
  {
   //already displayed the tip, let's bail
   wait(2000);
   EntKill(i_my_id);
   return;
   }

if (g_i_quit == 1)
{
  //an automatic process is happening, such as showing a conversation with a client, don't show anything now	
   wait(2000);
   EntKill(i_my_id);
   return;
}

//show quicktip

//pause the whole game
EntSetPauseLevel(i_my_id, 200); //give this script mega access
SetPauseLevel(200);
 CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
//ResetFrameTimer();
int i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
  EntSetFadeUp(i_tb, 2.0); //fade speed.  
 string st_temp = VKeyToString("ACTION3");
 string st_message = "Quick tip:  You just picked up Weed Killer.  Press $st_temp$ or button 1 to use it.  You can store these up and use them on other levels too!";
 DialogSet(i_tb, st_message);
  SoundPlay("sound\quicktip.wav");
g_i_weed_killer_tip = 1; //don't show this again
SetPauseLevel(200);

   DialogWaitForOk(i_tb);
   EntKill(i_tb);
EntSetPauseLevel(i_my_id, 0); //remove previously mentioned mega access
 CreateEntScript("", "script\mouse_off.c"); //turn the mouse off

SetPauseLevel(0);
wait(2000);
  EntKill(i_my_id);

}

void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.

}