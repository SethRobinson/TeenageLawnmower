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
  g_i_drunk += 1;

  //play sound effect
  SoundPlay("sound\cash register.wav");
 //don't allow any more hits
  SetOnTouch(i_my_id, 0); 
ShowMessage("Picked up $$5 bill");
g_i_bonus += 5;
  
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


void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.

}