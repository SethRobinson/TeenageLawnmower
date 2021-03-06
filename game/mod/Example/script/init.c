//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
   global int g_id_player = -1; 
   //globals used for mow level info
   global string g_st_client_name;
   global string g_st_client_home;
   global int g_i_client_pay;
   global string g_st_client_sound;
   global string g_st_client_pic;
   global string g_st_client_phone;
   global string g_st_client_decline_pic;
   global string g_st_client_decline;
   global int g_i_client_hours;
   global int g_i_force_money;
   global int g_i_spawn_count; //how many monsters/things we should spawn
   global int g_i_accidents;
   global int g_i_cleaned;
   global int g_i_total_lawns_mowed = 0;
   global string g_st_weather;
   global int g_i_mode = 0; //0 is not playing (main menu), 1 is room, 2 is 3d mowing mode
   global int g_mouse_active = 1; //if 0, all menus know they should turn the mouse off when done.
   //random globals we use
   global float g_f_rep_temp; 
   global int g_i_money = 0; //how much money we have total, temp far
   global string g_st_force_level; //used for cheating in cheat.c
   global int g_i_day = 0; //what day they are currently on.
   global int g_i_time = 9; //time of day
   global int g_i_level = 0; //what level they are playing
   global int g_i_last_level = 0; //last level we played
   global int g_i_quit = 0; //if 1, player is being shown exit message, don't allow escape menu, etc
   global int g_i_force_level_script = 0; 
   global int g_i_deadline_days = 30;
   global int g_i_deadline_money = 6000;
   global int g_i_difficulty = 0; //0 is easy, 1 is normal, 2 is hard
   global int g_i_morning_event_active = 1; //reset in new_day.c
   global int g_i_night_event_active = 1; //reset in new_day.c
   global int g_i_force_time = 0;
   global int g_i_force_day = 0;
   global int g_i_test_mode = 0;
   global int g_i_level_time = 20; //how many seconds they have
   global string g_st_name = "Noname"; //player's name
   global int g_i_end_level_on_accidents; //used by level scripts, if != 0 the level will end on a certain # of accidents
   global int g_i_goal; //# of current goal.  0 if none.
   global string g_st_goal_win_string;
   global string g_st_goal; //like "mother's operation"
   global int g_i_goal_money; //how much money we need
   global int g_i_goal_day; //what day we need to have the money buy
   global int g_i_last_goal; //helps us keet track of what we've done
   global string g_st_goal_lose_string; //show this if we can't pay it
   global string g_st_goal_start; //introduce them to the goal text
   global string g_st_stop_message; //if not "" will show this when quitting the arcade mode
   global int g_i_cult; //0 for not joined yet
  //story vars
   global int g_i_ate_meat; //1 if they ate todd's meat
   global string g_st_cult_title;
   global int g_i_drunk; //how drunk they are
  global int g_i_bonus; //some levels give otherways to make money this helps us keep track
  global string g_st_force_weather;
  global int g_i_quick_tip; //keep track of if we've showed our crappy quick tips or not
  global int g_i_mole_kill; //if true, killing moles is not an accident
  global int g_i_cur_mower; //0 for default, 1 for excalibur
  global int g_i_thomas_story = 0;
  global float g_f_damage_mod = 1.0; //set in startmow.c
}


//this script is run only once when the game starts.

void Main()
{
//set default fonts
 //  FontSet(0, "Courier New", "", 18); //choice dialog //for drawing the version #, reserved
   FontSet(1, "Courier New", "", 18); //choice dialog
   FontSet(2, "Courier New", "", 18); //normal dialog
   FontSet(3, "Courier New", "", 100); //for huge text
   FontSet(4, "Courier New", "", 26); //for message text
 
  AddFastFont("Courier New", "", 12); //fast font 1
  AddFastFont("Courier New", "",  24); //fast font 2

   
   //bind some default keys
   BindKeyToScript("escape", "script\key_escape.c", -1);    
    //let's create the main menu to kick things off
    CreateEntScript("main", "script\main.c");

   //kill this script
   EntKill(i_my_id); //kill this script
}

void OnKill()
{
	
}
