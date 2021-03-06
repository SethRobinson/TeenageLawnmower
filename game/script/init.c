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
   global int g_i_skip_to_play = 0; //used for debugging to quickly try a level
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

   global int g_i_weed_killer = 0; //how many weed killers we're holding
  global int g_i_weed_killer_tip = 0; //keep track of if we've shown the weed killer help thingie or not
  global int C_MAX_WEED_KILLERS = 7; //how many we can hold at once
  
  //Used with EntSetForceType()
   global int C_FORCE_GROUND_ONLY = 0;
   global int C_FORCE_GROUND_AND_AIR = 1; 

  //Used with EntSetGravityType()
   global int C_GRAVITY_WORLD = 0;
   global int C_GRAVITY_OVERRIDE = 1;
   global int C_GRAVITY_MOD = 2;

   //used with EntSetWallBounce()
   global int C_HITWALL_NOTHING = 0;
   global int C_HITWALL_REVERSE = 1;

   global int C_ENT_PHYSICS_NONE = 0;
   global int C_ENT_PHYSICS_NORMAL = 1;

  //Used with EntSetShadow()
  global int C_VISUAL_SHADOW_NONE = 0;
  global int C_VISUAL_SHADOW_ACCURATE = 1; //accurate shadow based on the mesh, works with .x, .md2 and .rts 
  global int C_VISUAL_SHADOW_FAKE = 2; //draw a little fake circle under them for a shadow, fast but crappy looking (implemented yet? I forget)

  //used with EntSetCollisionType()
  global int C_COLLISION_SPHERICAL = 0; //good for round objects like boulders
  global int C_COLLISION_BOX = 1; //default

  //used with EntSetBroadcastType()
  global int C_ENT_BROADCAST_NONE = 0;
  global int C_ENT_BROADCAST_MOVEMENT = 1;
  
  //used with EntSetHardness()
  global int C_ENT_HARDNESS_NONE = 0;
  global int C_ENT_HARDNESS_NORMAL = 1;


  //used with GetHost()
  global int C_HOST_EDITOR = 0;
  
  //used with EntSetCentering
  global int C_CENTERING_UPPER_LEFT = 0;
  global int C_CENTERING_CENTER = 1;
  global int C_CENTERING_BOTTOM_MIDDLE = 2;
  
  //used with EntSetPicVisualEffect
  
  global int C_PIC_VISUAL_EFFECT_NONE = 0;
  global int C_PIC_VISUAL_EFFECT_BOB = 1;
 
  global int C_RES_ADJUST_NONE = 0;
  global int C_RES_ADJUST_CENTER_BOTTOM = 1;                                                 
  global int C_RES_ADJUST_NORMAL = 2; //keep same proportions, but as if it was centered     
  global int C_RES_ADJUST_FULL = 3; //remap completely to new space                            
  global int C_RES_ADJUST_FULL_NO_SCALE = 4; //remap completely to new space, but don't scale the object
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

 CreateEntScript("", "script\mouse_on.c");
  int id_mouse = EntGetByName("MOUSE");

  EntSetXY(id_mouse, 400.0, 400.0);  //move the mouse to a good starting place
 // CreateEntScript("", "script\mouse_off.c");
 //wait(1);
   
   //bind some default keys
   BindKeyToScript("escape", "script\key_escape.c", 8);    
   BindKeyToScript("p", "script\key_p.c", -1);    
   BindKeyToScript("o", "script\key_o.c", -1);    

   //Set Some defaults (same ones that get set when creating a new game)
   CreateEntScript("DEFAULTS", "script\setdefaults.c"); //this will set the defaults and then kill itself
  
  
   //let's create the main menu to kick things off
  CreateEntScript("mainmenu", "script\main_menu.c");
  SetMouseJoystickControl(true);
   
  
     
    
 
   
   
   //kill this script
   EntKill(i_my_id); //kill this script
}

void OnKill()
{
	
}
