void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
  int id_choice;
  int id_dialog;
  int id_con;
  int i_temp;
}

//client as in the "customer"

void SetClientInfo()
{
  g_i_level_time = 130;  //how many seconds it takes to finish

  if (g_i_difficulty == 1)
   {

  g_i_level_time = 94;  //how many seconds it takes to finish

   }
  if (g_i_difficulty == 2)
   {

  g_i_level_time = 70;  //how many seconds it takes to finish

   }

  g_st_client_name = "Bill Farnsworth";
  g_st_client_home = "house";
  g_st_client_pic = "interface\moleguy_normal.dds";
  g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 42;
  g_i_client_hours = 4;

  int randomExtra = RandomRange(0, g_i_client_pay);
  randomExtra /= 2;
  g_i_client_pay += randomExtra;

  //I don't have to do StringExpand() now because g_i_client_pay is a global
  
 if (g_i_day > 7)
  {
    g_st_client_phone = "Hi.  This is Bill Farnsworth. I've got a bit of mow for ya.  I've heard good things about you, kid. ";
    g_st_client_decline_pic = "interface\moleguy_normal.dds";
    g_st_client_decline = "Ok, I see.  Maybe next time.";
    goto done;
  }
 
 if (g_i_day > 4)
  {
    g_st_client_phone = "Hi.  This is Bill Farnsworth. I've got a bit of mow for ya. I heard you ain't so bad. ";
    g_st_client_decline_pic = "interface\moleguy_normal.dds";
    g_st_client_decline = "Ok, I see.  Maybe next time.";
    goto done;
  }

    //default
    g_st_client_phone = "Hi.  This is Bill Farnsworth. I've got a job for ya and a possible bonus.";
    g_st_client_decline_pic = "interface\moleguy_surprised.dds";
    g_st_client_decline = "You little punk.  I guess the rumors are true.";

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
  ZoneLoad("data\lawn.zon");
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

vec_cam = MakeVector3(61.616, 52.274, 81.771);
EntSetPosition(id_cam, vec_cam)
EntSetDirection(id_cam, 1.127);
EntSetTilt(id_cam, 0.844);

CameraSetLerp(0.3);
CameraSetDirLerp(0.3);

//smoothly move around

 vec_cam = MakeVector3(111.127, 2.633, 107.173);
 EntSetTargetPosition(id_cam, vec_cam);
 EntSetTargetDirection(id_cam, 0.621);
 EntSetTargetTilt(id_cam, 0.127);


  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
 
  DialogSet(id_dialog, "\"Hi kid.  Thanks for coming out.\n\nI would like you to give 'er a good mow but also have a special request.\"");
   ConversationSetPic(id_con, "interface\moleguy_normal.dds");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Like what?\"");
  DialogChoiceWait(id_choice);

   ConversationSetPic(id_con, "interface\moleguy_normal.dds");
  DialogSet(id_dialog, "\"Well, my hobby is training moles.  I don't want you to accidently hurt them.. they are very curious and may come out to play.\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"I see.  If I see a mole, what should I do?\"");
  DialogChoiceWait(id_choice);
   ConversationSetPic(id_con, "interface\moleguy_normal.dds");

  DialogSet(id_dialog, "\"Wait for him to go back under ground, then clean the dirt he made by mowing it, k?\n\nI'll pay you extra for cleaning dirt.. just don't hurt my babies!\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Is that all? Too easy.  I'm ready to start.\"");
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

  //hey, let's get those moles going!
  int id_mole_spawner = CreateEntScript("molespawn", "script\gopher_spawn.c"); 
 
 CreateEntScript("INTERFACE", "script\game_interface.c"); //setup the progress indicator and such

 //end talk music and possibly start level music
  SoundMusicStop();
  g_i_end_level_on_accidents = 3;  //if we get three accidents, we'll end the level.

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
  ConversationSetPic(id_con, "interface\moleguy_normal.dds");

  int i_bonus = g_i_cleaned;
  i_bonus *= 2;
  
 int id_progress = EntGetByName("PROGRESS");
 float f_percent_mowed = EntMeterGet(id_progress);

if (g_i_accidents < 3)
{
   //they didn't kill enough modes to get yelled out, so make sure thet mowed everything.

  if (f_percent_mowed < 1.0)
  {
    //didn't finish the job.
     DialogSet(id_dialog, "\"I'm sorry young man, but a partially mowed lawn just doesn't cut it.  No pay!\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"Gah!  You should have told me you meant every little piece of grass...\" (return home)");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
  g_i_total_lawns_mowed -= 1;

   DialogChoiceWait(id_choice);

  goto load_house;
  }

}

if (g_i_accidents > 0)
  {
  
    //they killed at least 1 mole

  if (g_i_accidents < 3)
  {
    ConversationSetPic(id_con, "interface\moleguy_normal.dds");

    DialogSet(id_dialog, "\"Hi.  I guess you're done.  Hey, that's funny, I don't see Jeffry anywhere...\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"About that, a funny thing really, a mole or two happened to come up under the mower.. hehe.  not much I could do, really.\"");
   
    DialogChoiceWait(id_choice);
   ConversationSetPic(id_con, "interface\moleguy_smile.dds");

    DialogSet(id_dialog, "\"Hmm.. I guess you're right.  Oh well, natural selection at work, right? Haha.\n\nHere's your $$$g_i_client_pay$ plus a $$$i_bonus$ tip for cleaning holes.\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"Thanks, good doing business with ya, pard.\"");
    DialogChoiceWait(id_choice);
    g_i_money += i_bonus;
    g_i_money += g_i_client_pay;
   CreateScriptFunction("script\rep_mod.c", "GainSmall");  //reputation modification  
  
   goto load_house;
   }
 
   //else this gets run, too many dead.
 ConversationSetPic(id_con, "interface\moleguy_surprised.dds");

    DialogSet(id_dialog, "\"Hi.  I guess you're done and.. OH MY GOD... there is blood all over my lawn!\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"About that, a funny thing really, your wacky damn moles kept popping up under the mower!\"");
   
    DialogChoiceWait(id_choice);
    ConversationSetPic(id_con, "interface\moleguy_normal.dds");

    DialogSet(id_dialog, "\"You won't see one red penny from me young man!!! LEAVE NOW!\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"Cheapskate!!!\"");
    DialogChoiceWait(id_choice);
   CreateScriptFunction("script\rep_mod.c", "LoseMedium");  //reputation modification  
  
  goto load_house;
 }


   //default 

   ConversationSetPic(id_con, "interface\moleguy_smile.dds");
 
   DialogSet(id_dialog, "\"Nice job son.  Here is the $$$g_i_client_pay$ as promised.  I'm also giving you an extra $$$i_bonus$ tip for cleaning holes.\"");
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
