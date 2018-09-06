//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.

float f_scale;

}

void Main()
{
 
//turn off physics
//EntSetPhysics(i_my_id, 0; 
//scale the dirt up

Vector3 vec_scale = EntGetScale(i_my_id); 
f_scale = Vector3GetY(vec_scale);

 //let's respond to taking damage.
 
 //1 is true
 SetOnTouch(i_my_id, 1);

} 

void OnTouch()
{
 
SoundPlay("sound\bloopdown.wav");
 //we got touched.
  //if another powerup is in effect, let's kill it.

//  ShowMessage("Cleaned burger");
  //play sound effect
  //don't allow any more hits
  SetOnTouch(i_my_id, 0); 
//let's apply a cool particle system to the mower
   g_i_drunk += 1;
  if (g_i_drunk == 1)
    {
       ShowMessage("出来立てのハンバーガーを食べた！！！");
     goto ate;
   }
  if (g_i_drunk == 2)
    {
   	ShowMessage("ウマイ！でも、もうお腹いっぱいだ！");
     goto ate;
     }
  if (g_i_drunk == 3)
    {
    	ShowMessage("胃が痛い…");
     goto ate;
  	}
  if (g_i_drunk == 4)
    {
    	ShowMessage("食べすぎで気分が悪い…");
     goto ate;
  	}
  if (g_i_drunk == 5)
    {
    	ShowMessage("もう喉まで込み上げている…！！！");
     goto ate;
  	}
ate:  
if (g_i_drunk > 4)
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
    EntKill(i_my_id);
}

void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.

}