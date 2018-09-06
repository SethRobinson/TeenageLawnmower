void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int i_temp;
int i_tb;
}

void Main()
{

//check the cheats file

   int i_cheat = CreateEntScript("CHEAT", "script\cheat.c");
   EntRunFunction(i_cheat, "SetupCheat");
   EntKill(i_cheat);
//build the weather string

int i_rand_weather;



 if (g_i_time < 11)
  {
    i_rand_weather = 1; //morning
   goto weather_skip;
  }

 if (g_i_time < 17)
  {
    i_rand_weather = 2; //noonish
   goto weather_skip;
  }

//default

 if (g_i_time < 25)
  {
    i_rand_weather = 3; //noonish
   goto weather_skip;
  }

weather_skip:
 
string st_extra = "clear";

i_temp = randomrange(1, 6);

if (i_temp > 4)
  {
    i_temp = 1;
   //make it sunny half the time
  }

if (i_temp == 1)
  {
 st_extra = "clear";
  }

if (i_temp == 2)
  {
  st_extra = "fog";
  }
if (i_temp == 3)
  {
  st_extra = "storm";
   if (i_rand_weather == 1)
  {
     //let's not have mornings be stormy at all.
    st_extra = "clear";    
   }

  }
if (i_temp == 4)
  {
  st_extra = "rain";
  }
g_st_weather = "data\$i_rand_weather$_$st_extra$.wet";

//let's go ahead and expand it now while our local vars are valid
g_st_weather = StringExpand(g_st_weather);

if (g_st_force_weather != "")
  {
    g_st_weather = g_st_force_weather;

   }

  //get the real data
  //load the level setup script.  main() will be empty, let's run the function to get info

 g_i_level = 9;
 g_i_accidents = 0;
 g_i_cleaned = 0;
 g_i_drunk = 0;
 g_i_bonus = 0;
 g_id_player = -1; //assigned when the mower is created
 g_i_mole_kill = 0; //by default moles can't be killed without registering an 'accident'
 g_i_spawn_count = 0; 
 g_i_quit = 0; //reset this, if 1 they can't use escape menu/etc

//we don't want any quicktips to run

 g_i_quick_tip = 10; //disables it


  int id_level = CreateEntScript("LevelScript", "script\level\oil.c");
  EntRunFunction(id_level, "SetClientInfo"); //will return 0 if it doesn't exist
 
  CreateEntScript("", "script\mouse_off.c");

  //show message as we're loading
   i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
   DialogSet(i_tb, "Traveling to Highway 57...");

   wait(1); //make sure this get's shown
   wait(1); //make sure this get's shown
  //hey, let's also kill the background
  EntRunFunction(id_level, "StartLevel");
  EntKillByName("background");
  EntKill(i_tb);  //kill the loading message
  i_tb = -1;
  //kill this script
  EntKill(i_my_id); //kill this script
 
}

void OnKill()
{
 
}
