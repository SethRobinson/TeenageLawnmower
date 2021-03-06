void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
  int id_choice;
  int id_dialog;
  int id_con;
  int i_temp;
  Vector3  vec_pos;
  int i_rand;
}

//client as in the "customer"

void SetClientInfo()
{
  g_i_level_time = 240;  //how many seconds it takes to finish
  g_st_client_name = "Mr Test";
  g_st_client_home = "mansion";
  g_st_client_pic = "interface\chessguy_normal.dds";
  g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 50;
  g_i_client_hours = 4; //time to mow

  //I don't have to do StringExpand() now because g_i_client_pay is a global
  g_st_client_phone = "My name is Thomas Beaumont.  I would like to meet with you.";
  g_st_client_phone = StringExpand(g_st_client_phone);
  g_st_client_decline_pic = "interface\chessguy_normal.dds";
  g_st_client_decline = "I see.  Very well then.";
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
    ZoneLoad("data\oil.zon");
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
  Vector3 vec_cam;

  //set initial camera location

  vec_cam = MakeVector3(67.373, 9.346, 87.937);
  EntSetPosition(id_cam, vec_cam);
  EntSetDirection(id_cam, -1.156);
  EntSetTilt(id_cam, -6.250);
  
  wait(1000);
  
  CameraSetLerp(0.03);
  CameraSetDirLerp(0.03);
  
  //smoothly move around

  vec_cam = MakeVector3(168.522, 27.621, 150.808);
  EntSetPosition(id_cam, vec_cam);
  EntSetDirection(id_cam, -2.464);
  EntSetTilt(id_cam, -6.039);

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
  ConversationSetPic(id_con, "interface\chessguy_normal.dds");
  DialogSet(id_dialog, "A man with a texas drawl approaches you.\n\n\"Aye there. As you can see, we have a bit of a problem... the tanker caused all sorts of ruckus.\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"So how much are you giving me to keep quiet?\"");
  DialogChoiceWait(id_choice);

  DialogSet(id_dialog, "\"Um... $$400 should keep you little yap from telling anyone, right?\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Hell, I'd help clean up Nuclear Waste for $$400!\"");
  DialogChoiceWait(id_choice);
  
  DialogSet(id_dialog, "\"I'll keep that in mind, kid. You have 4 minutes before the EPA arrives!\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Whoa! I really need to get started then!\"");
  DialogChoiceWait(id_choice);
  
  EntKill(id_con); //kill all conversation elements at once
  
  //also kill the mouse
  g_mouse_active = 0;
  CreateEntScript("", "script\mouse_off.c"); //turn the mouse off
  
  int id_start = EntGetByName("Start Point");
  vec_pos = EntGetPosition(id_start);
  float f_start_dir = EntGetDirection(id_start);
  EntKill(id_start);  //don't draw this again.  Actually let's kill it.  Beware of saving, it would be missing.
  //ok, we now know where to start the player.  Let's make him.
  g_id_player = CreateEntMower("Player"); //put it into a global we defined somewhere else 
  EntSetPosition(g_id_player, vec_pos);
  EntSetDirection(g_id_player, f_start_dir);
  CreateScriptFunction("script\level\startmow.c", "Main"); 

  // g_i_spawn_count = 3;
  // g_i_spawn_count += g_i_difficulty; //more stuff spawns if the level is set to harder

 
  //create the thing that spawns random money
  CreateEntScript("moneyspawn", "script\bill_spawn.c"); 
  CreateEntScript("INTERFACE", "script\game_interface.c"); //setup the progress indicator and such
  //end talk music and possibly start level music
  SoundMusicStop();

  //if you had somemusic for the level you could do it here... 
  // SoundMusicPlay("test.mp3", 3);

  g_i_end_level_on_accidents = 0;  //if we get this many accidents, we'll end the level.
}


void FinishLevel()
{
  CreateScriptFunction("script\level\finish_global.c", "Finish");  //All scripts get this run at finish.
}  

void FinalExit()
{
  //switch camera to regular control mode and move it to the house again
  
  CameraSetFollow(0); //0 means don't follow anybody
  vec_cam = MakeVector3(168.522, 27.621, 150.808);
  EntSetPosition(id_cam, vec_cam);
  EntSetDirection(id_cam, -2.464);
  EntSetTilt(id_cam, -6.039);
  //closing conversation

  id_con = -1;
  int i_bonus =g_i_bonus;
   
  int id_progress = EntGetByName("PROGRESS");
  float f_percent_mowed = EntMeterGet(id_progress);
  float f_damage = 0;

  int id_damage = EntGetByName("DAMAGE");
  if (id_damage != -1)
  {
    f_damage = EntMeterGet(id_damage);
  }

  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
  g_mouse_active = 1;
  id_con = CreateConversation(""); //no name for it, we don't need to keep track
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);

  ConversationSetPic(id_con, "interface\chessguy_normal.dds");

  if (f_percent_mowed < 1.0)
  {
 
    //didn't finish the job.
    ConversationSetPic(id_con, "");


    DialogSet(id_dialog, "You stop the mower without finishing the lawn.\n\n");

    if (i_bonus > 0)
    {
      DialogAdd(id_dialog, "You decide to just leave with the $$$i_bonus$ you found.  You can here the EPA agents coming through the trees...");
    }

    if (i_bonus == 0)
    {
      DialogAdd(id_dialog, "You decide to be on Uncle Sam's good side, and make sure the Evil Texan is thrown in jail, where he will bribe a congressman to make it mandatory to sprinkle oil all over undeveloped areas.");
    }

    g_i_money += i_bonus;
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "Unload mod");

    CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
    //combat the total_lawns mowed that will get added in a sec
    g_i_total_lawns_mowed -= 1;
    DialogChoiceWait(id_choice);
    goto load_house;
  }

  //default 
  DialogSet(id_dialog, "\"I see you made everything spick-and-span. Now if only more kids were willing to bend the law like you... thanks!\"");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Exit Mod");
  DialogChoiceWait(id_choice);  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  CreateScriptFunction("script\rep_mod.c", "GainSmall");  //reputation modification  
  g_i_money += g_i_client_pay;
  
  g_i_money += i_bonus;
 
  load_house:
  //kill stuff

  //add one ot the mowed #
  g_i_total_lawns_mowed += 1;

  //load the main menu pic to show while we don't have a level loaded
  
  int i_bg =  CreateEntPic("background"); //we can find this by name later if we want
  EntSetSprite(i_bg, "interface\mow_master.jpg");
  EntSetLayer(i_bg, -10); //show up behind everything for sure
  EntSetPauseLevel(i_bg, 200); 

  if (id_con != -1)
  {
    EntKill(id_con); //kill all conversation elements at once//  wait(1);
  } 
  EntKillAllPics(); //get rid of all backgrounds and buttons on the screen
  EntKill(g_id_player);
  g_i_quit = 0; //enable the escape menu

  //Clear Level
  // ZoneLoad(""); //blanks out the level
  //kill all interface items
  EntKill(id_weather);
  //kill this script
  EntSetPauseLevel(i_my_id, 100); //by doing this, we won't get deleted with a KillAllEnts command, which is good because this
  //script needs to continue
  LogMsg("Clearing level..");
  EntKillAllItems();
  wait(1); //the delete and screen update actually happen before we continue 
  ModRun(0); //run mod 0 to run main game
  EntSetPauseLevel(i_bg, 0);  //unprotect the background

  CreateEntScript("Main", "script\init.c");
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
