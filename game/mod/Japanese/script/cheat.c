void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
}

void SetupCheat()
{
  //set defaults
  g_st_force_level = "";
  //logmsg("Checking cheats..");  

  //uncomment the line below to force a level to get loaded
  //0 active is real game


  //******* If you want to force a level script to be run, set this to a non 0 number

g_i_force_level_script = 0;


g_i_test_mode = 1//if 1, the phone calls are really fast and such.. don't forget to test with this at 0 too!!
g_i_force_day = 0; //0 to disable
g_i_force_time = 0; //0 to disable
g_i_force_money = 0;
g_st_force_weather = "";

//g_st_force_weather = "data\3_storm.wet";
}

void Main()
{
  //don't actually do anything when loaded
}


void OnKill()
{
	
}
