void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
  int id_choice;
  int id_dialog;
  int id_con;
  vector3 vec_cam;
  int i_temp;
 
}

//client as in the "customer"

void SetClientInfo()
{
  g_i_level_time = 100;  //how many seconds it takes to finish
  i_temp = 15;
  i_temp *= g_i_difficulty;
  g_i_level_time -= i_temp; //higher difficulty means less time

  g_st_client_name = "The City";
  g_st_client_home = "park";
  g_st_client_pic = "interface\guy2_smile.dds";
  g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 40;
  g_i_client_hours = 4;

  int randomExtra = RandomRange(0, g_i_client_pay);
  randomExtra /= 2;
  g_i_client_pay += randomExtra;

  //I don't have to do StringExpand() now because g_i_client_pay is a global
  
 
    //default
    g_st_client_phone = "Hi, this is Dan from city hall.  We need a mow.  Also we've got a bit of a problem at the park, figured you'd be the man for the job.";
    g_st_client_decline_pic = "interface\guy2_normal.dds";
    g_st_client_decline = "Well, suit yourself but doing good work for us is a good way to increase your reputation around here.";

  done:
 
}

void Main()
{
  //don't actually do anything when loaded
}

void StartLevel()
{

  //actually set up the level
  g_i_mode = 2; //0 means main menu, not playing.  1 is room, 2 is 3d mode.

  //get some tunes goin'
  SoundMusicPlay("sound\talk_music.wav", 1000);


  if (g_st_force_level == "")
 {
  ZoneLoad("data\park.zon");
 }

  if (g_st_force_level != "")
 {
  logmsg("Forcing level $g_st_force_level$.");
  ZoneLoad(g_st_force_level);
 }

 //load the weather we were told to
  int id_weather = CreateEntWeather("Weather");
  EntWeatherLoad(id_weather, g_st_weather); //change this if the level needs it


  int id_cam = EntGetByName("Main Camera");

//set initial camera location

vec_cam = MakeVector3(79.213, 9.020, 61.263);
EntSetPosition(id_cam, vec_cam);
EntSetDirection(id_cam, -5.795);
EntSetTilt(id_cam, 0.253);


vec_cam = MakeVector3(66.534, 6.711, 68.114);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, -5.499);
EntSetTargetTilt(id_cam, 0.253);

CameraSetLerp(0.3);
CameraSetDirLerp(0.3);

//smoothly move around


  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
 
  DialogSet(id_dialog, "Dan comes over to meet with you.\n\n\"Hi $g_st_name$, here's the deal.  It turns out we shouldn't have been so careful about protecting the damn squirrels.\"");
  ConversationSetPic(id_con, "interface\guy2_smile.dds");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"What do you mean?\"");
  DialogChoiceWait(id_choice);


   ConversationSetPic(id_con, "interface\guy2_normal.dds");
  DialogSet(id_dialog, "\"They are multiplying like tribbles or something.  We want you to help them along in the circle of life.  The city will pay a buck for each squirrel heart you bring me.\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Woah, cool.  Let's get started.\"");
  DialogChoiceAdd(id_choice, 2, "\"Wait a minute.  Are you asking me to use my blades of destruction against innocent furry animals?\"");
  DialogChoiceWait(id_choice);


  if (g_i_last_choice == 2)
  {
     DialogSet(id_dialog, "\"Yes, that's exactly what I'm asking.\"");
     DialogChoiceClear(id_choice);
     DialogChoiceAdd(id_choice, 1, "\"Great, was just making sure.  Let's do this thing.\"");
    DialogChoiceWait(id_choice);
 }

  EntKill(id_con); //kill all conversation elements at once

  //also kill the mouse
  g_mouse_active = 0;
  CreateEntScript("", "script\mouse_off.c"); //turn the mouse off

  int id_start = EntGetByName("Start Point");
  Vector3 vec_pos = EntGetPosition(id_start);
  float f_start_dir = EntGetDirection(id_start);
  EntKill(id_start);  //don't draw this again.  Actually let's kill it.  Beware of saving, it would be missing.
  //ok, we now know where to start the player.  Let's make him.
  g_id_player = CreateEntMower("Player"); //put it into a global we defined somewhere else 
  EntSetPosition(g_id_player, vec_pos);
  EntSetDirection(g_id_player, f_start_dir);
  CreateScriptFunction("script\level\startmow.c", "Main"); 

 g_i_spawn_count = 300;

 //squirrels will come off the trees
 CreateEntScript("squirrelspawn", "script\squirrel_spawn.c"); 
 
 CreateEntScript("INTERFACE", "script\game_interface.c"); //setup the progress indicator and such

 //end talk music and possibly start level music
 SoundMusicStop();
g_i_end_level_on_accidents = 0;  //if we get three accidents, we'll end the level.
}

void FinishLevel()
{
  CreateScriptFunction("script\level\finish_global.c", "Finish");  //All scripts get this run at finish.
}  

void FinalExit()
{

  //switch camera to regular control mode and move it to the house again

  CameraSetFollow(0); //0 means don't follow anybody
 vec_cam = MakeVector3(111.127, 2.633, 107.173);
 EntSetTargetPosition(id_cam, vec_cam);
 EntSetTargetDirection(id_cam, 0.621);
 EntSetTargetTilt(id_cam, 0.127);
 
  //closing conversation

  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
  g_mouse_active = 1;
  id_con = CreateConversation(""); //no name for it, we don't need to keep track
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  ConversationSetPic(id_con, "interface\guy2_smile.dds");

  int i_bonus = 0;
  i_bonus *= 1;
  
 int id_progress = EntGetByName("PROGRESS");
 float f_percent_mowed = EntMeterGet(id_progress);
 float f_damage = 0;

 int id_damage = EntGetByName("DAMAGE");
 if (id_damage != -1)
  {
   f_damage = EntMeterGet(id_damage);
  }

  if (f_percent_mowed < 1.0)
  {
 
  //didn't finish the job.
   ConversationSetPic(id_con, "interface\guy2_normal.dds");
   DialogSet(id_dialog, "\"Um.. shouldn't you have mowed ALL the grass?\n\nI don't wanna be a stickler here but the city won't issue a check for a half done job.\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"I'm out of gas or something. Bye.\" (return home)");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;
  }
  

   //default 
 i_bonus = g_i_accidents;
 // i_bonus /= 5;

  ConversationSetPic(id_con, "interface\guy2_smile.dds");
 
   DialogSet(id_dialog, "\"Excellent work.  Here is your $$$g_i_client_pay$.  You also hit $g_i_accidents$ squirrels so your bonus is $$$i_bonus$.\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"Thanks!\" (return home)");
   CreateScriptFunction("script\rep_mod.c", "GainSmall");  //reputation modification  
     g_i_money += i_bonus;
    g_i_money += g_i_client_pay;
  
    DialogChoiceWait(id_choice);

  load_house:
  //kill stuff

  //add one ot the mowed #
  g_i_total_lawns_mowed += 1;

  EntKill(id_con); //kill all conversation elements at once//  wait(1);
  EntKillAllPics(); //get rid of all backgrounds and buttons on the screen
  EntKill(g_id_player);
  g_i_quit = 0; //enable the escape menu

  //Clear Level
  ZoneLoad(""); //blanks out the level
  //kill all interface items
   EntKill(id_weather);
  //kill this script
  EntSetPauseLevel(i_my_id, 100); //by doing this, we won't get deleted with a KillAllEnts command, which is good because this
  //script needs to continue
  LogMsg("Clearing level..");
  EntKillAllItems();
  
  //Load room
  CreateEntScript("RoomMenu", "script\room_menu.c");
  EntKill(i_my_id); //kill this script

}
void FinishNoTime()
{
id_progress = EntGetByName("PROGRESS");
  f_percent_mowed = EntMeterGet(id_progress);
 if (f_percent_mowed >= 1.0)
  {
    //they actually completely the level, ignore the no time thing.
   return; 
  }
 CreateScriptFunction("script\level\finish_global.c", "NoTime");  //All scripts get this run at finish.
 FinishLevel();
}

void OnKill()
{
	
}
