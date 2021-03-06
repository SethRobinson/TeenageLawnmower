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
  g_i_level_time = 95;  //how many seconds it takes to finish

 if (g_i_difficulty == 1)
   {
	  g_i_level_time = 75;  //how many seconds it takes to finish
   }

 if (g_i_difficulty == 2)
   {
	  g_i_level_time = 60;  //how many seconds it takes to finish
   }


  g_st_client_name = "Bolt Montana";
  g_st_client_home = "Café";
  g_st_client_pic = "interface\bolt_smile.dds";
  g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 30;

 g_i_client_hours = 2; //time to mow

  //I don't have to do StringExpand() now because g_i_client_pay is a global
  
   int randomExtra = RandomRange(0, g_i_client_pay);
  randomExtra /= 2;
  g_i_client_pay += randomExtra;

    //default
    g_st_client_phone = "The name is Bolt.  I offer you the honor of mowing the trendiest café in the city.";
    g_st_client_decline_pic = "interface\bolt_upset.dds";
    g_st_client_decline = "Is this a joke?  Did you know who I am?!";

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
  ZoneLoad("data\cafe.zon");
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

vec_cam = MakeVector3(82.607, 11.201, 126.835);
EntSetPosition(id_cam, vec_cam);
EntSetDirection(id_cam, -0.580);
EntSetTilt(id_cam, 0.1);


vec_cam = MakeVector3(128.106, 29.381, 57.461);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, -0.580);
EntSetTargetTilt(id_cam, 0.1);

CameraSetLerp(0.3);
CameraSetDirLerp(0.3);

//smoothly move around


  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
 
  DialogSet(id_dialog, "\"Greetings!  Behold the grandeur of Deko-Café.\n\nFeel free to eat any burgers you find.  If you find any tips you can keep 'em!\n\nIs this a great place or what?"\");
  ConversationSetPic(id_con, "interface\bolt_smile.dds");

  DialogChoiceClear(id_choice);
  i_temp = randomrange(1, 3);

  if (i_temp == 1)
  {
  DialogChoiceAdd(id_choice, 1, "\"Nice hair.  Let's do it\"");
  }

  if (i_temp == 2)
  {
  DialogChoiceAdd(id_choice, 1, "\"Move out of the way and watch me make mow magic\"");
  }

 if (i_temp == 3)
  {
  DialogChoiceAdd(id_choice, 1, "\"I'm ready to start\"");
  }
  DialogChoiceWait(id_choice);


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

// g_i_spawn_count = 3;
// g_i_spawn_count += g_i_difficulty; //more stuff spawns if the level is set to harder

 
 //create the rock spawner
CreateEntScript("squirrelspawn", "script\food_spawn.c"); 
CreateEntScript("billspawn", "script\bill_spawn.c"); 
 
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
vec_cam = MakeVector3(162.340, 31.875, 85.285);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, -1.060);
EntSetTargetTilt(id_cam, 0.4);

 
  //closing conversation

 int i_bonus = 0;
  
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
   
   if (g_i_drunk > 4)
     {
  //didn't finish mowing
 	ShowMessage("YOU BLOW CHUNKS EVERYWHERE!");
	wait(2500);
	CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
        g_mouse_active = 1;
 
    int i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
   EntSetFadeUp(i_tb, 2.0); //fade speed.  

if (g_i_bonus > 0)
  {
    DialogSet(i_tb, "Your 'accident' greatly amuses Bolt and his staff.\n\nAmidst the laughing you quickly clean up your mess and leave with only the $$$g_i_bonus$ you found.");
  }

 if (g_i_bonus == 0)
   {

    DialogSet(i_tb, "Your 'accident' greatly amuses Bolt and his staff.\n\nAmidst the laughing you quickly clean up your mess and leave in shame.");
   }
   
    g_i_money += g_i_bonus;
    g_i_total_lawns_mowed -= 1;
    DialogWaitForOk(i_tb);
   EntKill(i_tb);
       goto load_house;
     }

   }


  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
  g_mouse_active = 1;
  id_con = CreateConversation(""); //no name for it, we don't need to keep track
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);


 

 


  if (f_percent_mowed < 1.0)
  {
 
  //didn't finish the job.
   ConversationSetPic(id_con, "interface\bolt_upset.dds");
 if (g_i_bonus > 0)
  {
   DialogSet(id_dialog, "\"You're leaving?!  I have to open this place in ten minutes!  You really let me down.\"\n\nAt least you found $$$g_i_bonus$ in tips.");
 }
 if (g_i_bonus == 0)
 {
 
   DialogSet(id_dialog, "\"You're leaving?!  I have to open this place in ten minutes!  You really let me down.\"\n\nYou shrug.");
 
 }
 
    DialogChoiceClear(id_choice);
   i_temp = randomrange(1, 2);
   
    if (i_temp == 1)
    {
    DialogChoiceAdd(id_choice, 1, "\"Sorry Bolt, bye.\" (return home)");
    }

    if (i_temp == 2)
    {
    DialogChoiceAdd(id_choice, 1, "\"Hate to eat 'n run but.. ta-ta\" (return home)");
    }


   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
    g_i_money += g_i_bonus;
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;
  }

  

   //default 

   ConversationSetPic(id_con, "interface\bolt_smile.dds");
 
   DialogSet(id_dialog, "\"Wahoo!  Great mowin' man.  Here is your $$$g_i_client_pay$.  Tell your friends about this place!\"\n\nYou found $$$g_i_bonus$ in tips.");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"Thanks Bolt\" (return home)");
   CreateScriptFunction("script\rep_mod.c", "GainSmall");  //reputation modification  
    g_i_money += g_i_client_pay;
    g_i_money += g_i_bonus;

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
