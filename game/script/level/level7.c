void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
  int id_choice;
  int id_dialog;
  int id_con;
  Vector3 vec_cam;
  int i_temp;
  Vector3  vec_pos;
 
}

//client as in the "customer"

void SetClientInfo()
{
  g_i_level_time = 103;  //how many seconds it takes to finish
 if (g_i_difficulty == 1)
   {
  g_i_level_time = 83;  //how many seconds it takes to finish

   }
  
 if (g_i_difficulty == 2)
   {
  g_i_level_time = 76;  //how many seconds it takes to finish

   }
  

  g_st_client_name = "The Hermit";
  if (g_i_day > 19)
  {
g_st_client_name = "Dale";
  
  }
  g_st_client_home = "retreat";
  g_st_client_pic = "interface\cult_upset.dds";
  g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 75;
 g_i_client_hours = 4; //time to mow

  int randomExtra = RandomRange(0, g_i_client_pay);
  randomExtra /= 2;
  g_i_client_pay += randomExtra;

  //I don't have to do StringExpand() now because g_i_client_pay is a global
  


    //default
    g_st_client_phone = "Hey you.  I got lots o' grass and no mower.  I reckon you can come up and mow it if you want.  I got money.";
   g_st_client_decline_pic = "interface\cult_upset.dds";
    g_st_client_decline = "I don't care anyway.";

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
  ZoneLoad("data\isolatedhouse.zon");
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

vec_cam = MakeVector3(136.898, 7.753, 71.180);
EntSetPosition(id_cam, vec_cam);
EntSetDirection(id_cam, -0.491);
EntSetTilt(id_cam, -0.046);



vec_cam = MakeVector3(168.958, 44.665, 60.615);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, -0.828);
EntSetTargetTilt(id_cam, 0.587);

CameraSetLerp(0.3);
CameraSetDirLerp(0.3);

 //let's spawn the dogs now

 
  //create all the beagles
int i_count = 5;

if (g_i_difficulty == 1)
   {
     i_count += 3;
   }

if (g_i_difficulty == 2)
   {
     i_count += 6;
   }

  int i_new;

 again:
  //let's create a gopher thingie
i_temp = randomrange(1, 3);

 if (i_temp < 3)
  {
   i_new = CreateEntityFromTemplate( "Creatures" , "deko (dog)");  
  }

 if (i_temp == 3)
  {
   i_new = CreateEntityFromTemplate( "Creatures" , "puppy");  
  }

  //now let's put it in a good place

  vec_pos = GetRandomPointInSubZone("Mow");
  EntSetPosition(i_new, vec_pos);

 // LogMsg("Making gopher at $vec_pos$.");

  i_count -= 1;

 if (i_count > 0)
  {
    goto again;
  }


//smoothly move around

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
 
  DialogSet(id_dialog, "The Hermit peers at you through windows for a few minutes before coming out.\n\n\"Hi.  I ain't much of a people person.  Just do the job.  Hurt my dogs and I hurt you.\"");
  ConversationSetPic(id_con, "interface\cult_upset.dds");

  if (g_i_day > 19)
  {
  DialogSet(id_dialog, "Dale peers at you through windows for a few minutes before coming out.\n\n\"Hurmph.  I thought maybe your ma might come out with ya.  Oh well, start 'cher mowing already.\"");

  }

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"It's mow time\"");
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

 
 //create the rock spawner
// CreateEntScript("squirrelspawn", "script\rock_spawn.c"); 
 
 CreateEntScript("INTERFACE", "script\game_interface.c"); //setup the progress indicator and such

 //end talk music and possibly start level music
 SoundMusicStop();
g_i_end_level_on_accidents = 2;  //if we get three accidents, we'll end the level.

}

void FinishLevel()
{
  CreateScriptFunction("script\level\finish_global.c", "Finish");  //All scripts get this run at finish.
}  

void FinalExit()
{
  //switch camera to regular control mode and move it to the house again
  
  CameraSetFollow(0); //0 means don't follow anybody
vec_cam = MakeVector3(168.958, 44.665, 60.615);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, -0.828);
EntSetTargetTilt(id_cam, 0.587);
  //closing conversation

  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
  g_mouse_active = 1;
  id_con = CreateConversation(""); //no name for it, we don't need to keep track
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);

  int i_bonus = 0;
   
 int id_progress = EntGetByName("PROGRESS");
 float f_percent_mowed = EntMeterGet(id_progress);
 float f_damage = 0;

 int id_damage = EntGetByName("DAMAGE");
 if (id_damage != -1)
  {
   f_damage = EntMeterGet(id_damage);
  }

 
  ConversationSetPic(id_con, "interface\cult_upset.dds");


  if (g_i_accidents >= g_i_end_level_on_accidents)
  {
     //killed a dog
  //didn't finish the job.
  
 DialogSet(id_dialog, "The door opens and The Hermit stands before you with tears running down his cheeks.\n\n\"I saw what you did through the window... You are just like my four ex-wives, you just don't give a shit!\"");

  if (g_i_day > 19)
  {
DialogSet(id_dialog, "The door opens and Dale stands before you with tears running down his cheeks.\n\n\"I saw what you did through the window... You are just like my four ex-wives, you just don't give a shit!\"");

  }
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"It was an accident!\" (frantically run for the gate)");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;


  }

  if (f_percent_mowed < 1.0)
  {
 
  //didn't finish the job.
   DialogSet(id_dialog, "\"This a city boy joke?  You mow everything.  Get outta here before I sic muh dogs on yew!\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"Um, sir?  They're beagles.  Bye.\" (return home)");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;
  }

   ConversationSetPic(id_con, "");

   //default 
  DialogSet(id_dialog, "The hermit is nowhere to be found but you notice a letter on the porch.\n\nThe letter reads \"luks gud, tak the monee\".  You find $$$g_i_client_pay$ enclosed.");
 
  if (g_i_day > 19)
  {
  DialogSet(id_dialog, "Dale slowly opens the door and comes outside.\n\n\"Here is $$$g_i_client_pay$.  You done a fine job today.  You tell your moms to call me sometime, ok?\"");
  ConversationSetPic(id_con, "interface\cult_upset.dds");
  }

   DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "Go home");
    CreateScriptFunction("script\rep_mod.c", "GainSmall");  //reputation modification  
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
