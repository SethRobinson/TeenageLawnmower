//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 logmsg("Initting weather script.");
 string st_sound;
Vector3 vec_pos;
int h_rain = 0;

}


//this is a weather script.  

void Main()
{
  EntSetPauseLevel(i_my_id, 200); //nothing can delete us 
again:

 if (h_rain != 0)
   {
    SoundPlayH(h_rain, 9999);   //play it once using the sound handle
   } 

 if (h_rain == 0)
  {
     //caching thunder sound
     h_rain = SoundPlayInstance("sound\evening_crickets.wav");  
    SoundPlayH(h_rain, 9999);   //play it once using the sound handle
    SoundStayAlive(h_rain, 1); //don't die after finish playing.  Just stop and wait.
  }
  wait(30000); //wait 30 seconds
goto again;  
}

void OnKill()
{
 logmsg("Closing weather script.");	

SoundStayAlive(h_rain, 0);
SoundKill(h_rain);

}
