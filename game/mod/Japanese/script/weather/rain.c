//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded.

 int i_random;
 int id_target;
 float f_temp_x;
 float f_temp_y;
 float f_temp_z;
 int i_first_time;
 int id_lightning = -1;
 int id_weather = EntGetByName("WEATHER"); 
 int i_strikes = 0;
 int i_min_time;
 int i_max_time;

     i_min_time = 29000;
     i_max_time = 45000;
int h_thunder = 0;
Vector3 vec_pos;
int h_rain = 0;
int i_ps = -1;
}

void Main()
{
 EntSetPauseLevel(i_my_id, 200); //nothing can delete us
 //don't show us, we're just a nothing
EntSetNoDraw(i_my_id, 1);

vector3 vec_start;

EntSetPauseLevel(i_my_id, 200); //nothing can delete us  
  //create rain particle system
 i_ps = CreateParticle("script\rain.pss");
int i_target = EntGetByName("MAIN CAMERA");
EntSetFollow(i_ps, i_target);
if (h_rain != 0)
   {
    SoundPlayH(h_rain, 9999);   //play it once using the sound handle
   } 

 if (h_rain == 0)
  {
     //caching thunder sound
     h_rain = SoundPlayInstance("sound\rain_loop.wav");  
    SoundPlayH(h_rain, 9999);   //play it once using the sound handle
    SoundStayAlive(h_rain, 1); //don't die after finish playing.  Just stop and wait.
  }
i_random = randomrange(8000, 12000);
wait (i_random);

 again:
if (h_rain != 0)
   {
    SoundPlayH(h_rain, 9999);   //play it once using the sound handle
   } 

 if (h_rain == 0)
  {
     //caching thunder sound
     h_rain = SoundPlayInstance("sound\rain_loop.wav");  
    SoundPlayH(h_rain, 9999);   //play it once using the sound handle
    SoundStayAlive(h_rain, 1); //don't die after finish playing.  Just stop and wait.
  }

  i_random = randomrange(1, 15);
  if (i_random > 1)
   {
    goto skip;
  }


  id_target = EntGetRandomByName("TREE");   
 i_strikes += 1;
 // logmsg("Located ent $id_target$.");

if (id_target == -1)
  {
    //no more trees to hit...?
  
}

//use lightning very rarely


if (id_target != -1)
{
  //play thunder

 if (h_thunder != 0)
   {
    SoundPlayH(h_thunder, 1);   //play it once using the sound handle
   } 

 if (h_thunder == 0)
  {
     //caching thunder sound
     h_thunder = SoundPlayInstance("sound\thunder.wav");  
     SoundStayAlive(h_thunder, 1); //don't die after finish playing.  Just stop and wait.
  }
  

 vec_pos = EntGetPosition(id_target);

  f_temp_x = vector3getx(vec_pos);
  f_temp_y = vector3gety(vec_pos);
  f_temp_z = vector3getz(vec_pos);

  //first adjust the target a bit
  f_temp_y += 5;
  vec_pos = makevector3(f_temp_x, f_temp_y, f_temp_z); 

  f_temp_y = 100; //put the start directly over the tree   
  vec_start = makevector3(f_temp_x, f_temp_y, f_temp_z); 

  //do some lightning
  id_lightning = CreateLightning("Lightning", vec_start, vec_pos);
 

 EntRunFunction(id_target, "OnBurn");
EntWeatherFlash(id_weather, 100);
wait(200);
EntWeatherFlash(id_weather, 130);
wait(160);
EntWeatherFlash(id_weather, 130);
wait(220);
EntWeatherFlash(id_weather, 100);
wait(160);
EntWeatherFlash(id_weather, 100);
wait(200); 

EntWeatherFlash(id_weather, 130);
wait(160);
EntWeatherFlash(id_weather, 130);
wait(220);
EntWeatherFlash(id_weather, 100);
wait(160);
EntWeatherFlash(id_weather, 100);
wait(200); 
 EntKill(id_lightning);
id_lightning = -1;
  }
 
skip:
 i_random = RandomRange(i_min_time, i_max_time);

  wait(i_random);

 goto again;
}


void OnKill()
{
//logerror("rain says lightning is $id_lightning$ and i_ps is $i_ps$.");

  if (id_lightning != -1)
  {
 EntKill(id_lightning);
    id_lightning = -1;
  }    
  SoundStayAlive(h_thunder, 0);
 int i_host = GetHost();
if (i_host == 0)
  {
   //we're in the editor, don't move around
  
 

if (i_ps != -1)
  {
  EntKill(i_ps);
   i_ps = -1;   
 }
 }
SoundStayAlive(h_rain, 0);
SoundKill(h_rain);

}