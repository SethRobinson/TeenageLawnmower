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

 
}

void OnTouch()
{
  //we got touched.
  g_i_drunk += 1;

  //play sound effect
  SoundPlay("sound\gulp.wav");
//don't allow any more hits
  SetOnTouch(i_my_id, 0); 




   ShowMessage("Picked up lemonade");

  EntSetName(i_my_id, "DRUNK_MOD");

 MowSetDrunk(1); //how drunk we are
  //shrink the can down and then stop drawing it

  if (g_i_drunk > 2)
   {
     if (g_i_quit == 0)
       {
         int i_levelscript = EntGetByName("LEVELSCRIPT");
         EntRunFunction(i_levelscript, "FinishLevel");  
       }

   }

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
  
 //wait for the effects to wear off
 wait(6000);

  EntSetName(i_my_id, "LEMONADE");

 int i_temp = EntGetByName("DRUNK_MOD");
  if (i_temp == -1)
{
 //only do this if nobody else is playing with the drunk meter too
if (g_i_quit == 0)
{  
ShowMessage("You feel better.");
 wait(300);
}
i_temp = EntGetByName("DRUNK_MOD");
  if (i_temp == -1)
{
 //only do this if nobody else is playing with the drunk meter too
 MowSetDrunk(0); //how drunk we are

}
} 
 

EntKill(i_my_id);
}


void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.

}