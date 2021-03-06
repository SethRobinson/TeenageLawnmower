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
  
g_i_level_time = 115;  //how many seconds it takes to finish

if (g_i_difficulty == 1)
   {
    g_i_level_time = 93;  //how many seconds it takes to finish

   }

if (g_i_difficulty == 2)
   {
    g_i_level_time = 75;  //how many seconds it takes to finish

   }

  g_st_client_name = "The City";
  g_st_client_home = "park";
  g_st_client_pic = "interface\guy2_smile.dds";
  g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 50;
  g_i_client_hours = 4;

  int randomExtra = RandomRange(0, g_i_client_pay);
  randomExtra /= 2;
  g_i_client_pay += randomExtra;


  //I don't have to do StringExpand() now because g_i_client_pay is a global
  
 if (g_f_rep_temp > 0.3)
  {
    g_st_client_phone = "Hey $g_st_name, it's me, Dan.  It's that time again.";
    g_st_client_decline_pic = "interface\guy2_normal.dds";
    g_st_client_decline = "Ok, if you're schedule is full there is nothing I can do I guess.";
    goto done;
  }
 
 if (g_f_rep_temp > 0.15)
  {
    g_st_client_phone = "Hi, this is Dan from the city.  Our park is looking like a jungle, can you come by?";
    g_st_client_decline_pic = "interface\guy2_normal.dds";
    g_st_client_decline = "Randomly declining work is no way to do business!";
    goto done;
  }

    //default
    g_st_client_phone = "Hi, I'm calling from city hall.  Our budget is a little low and we’re trying to cut some corners.  We’d like you to give the city park a little trim, can you do that?";
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
 
  DialogSet(id_dialog, "\"Glad you could make it out.  This is a state job so you could have been late and nobody would’ve really minded.. haha.");
  DialogAdd(id_dialog, "\n\nAnyway, feel free to mow away but I’d like you to watch out for one thing.\"");
  ConversationSetPic(id_con, "interface\guy2_smile.dds");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Sure Dan, what's that?\"");
  DialogChoiceWait(id_choice);


   ConversationSetPic(id_con, "interface\guy2_normal.dds");
  DialogSet(id_dialog, "\"Kids love the squirrels.  If they find squirrel body parts it’s really gonna crush ‘em.  So be real careful, ok?\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"You got it, let's start.\"");
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

 g_i_spawn_count = 3;
 g_i_spawn_count += g_i_difficulty; //more stuff spawns if the level is set to harder

 //squirrels will come off the trees
 CreateEntScript("squirrelspawn", "script\squirrel_spawn.c"); 
 
 CreateEntScript("INTERFACE", "script\game_interface.c"); //setup the progress indicator and such

 //end talk music and possibly start level music
 SoundMusicStop();
g_i_end_level_on_accidents = 2;  //if we get three accidents, we'll end the level.

if (g_i_difficulty == 0)
  {
    //make things easier
    g_i_end_level_on_accidents = 3;	
  }
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

  if (g_i_accidents > 1)
   {
      
  //killed too many things
   ConversationSetPic(id_con, "interface\guy2_surprised.dds");
   DialogSet(id_dialog, "\"Great, let me pay you fo... um, I think I just stepped on part of a squirrel.\n\nThere are squirrels parts all over!  I can't pay you.\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"Crap.  I thought the ones I hit were big rats.  Oh well.\" (return home)");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;
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

   if (g_i_accidents > 0)
   {
      
  //killed too many things
   ConversationSetPic(id_con, "interface\guy2_smile.dds");
   DialogSet(id_dialog, "\"Great, I see you've done a nice mow job.  So are all the squirrels ok?\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"I might have nicked one.  But he was a big mean nasty one.\"");
    DialogChoiceAdd(id_choice, 2, "\"No, I didn't hurt any.\"");

  

   CreateScriptFunction("script\rep_mod.c", "GainTiny");  //reputation modification  
   DialogChoiceWait(id_choice);
  string st_answer = "Hmm.  Ok, that's not so bad I guess, he might have had rabies or something.";
  
  if (g_i_last_choice == 2)
    {
       st_answer = "Really?  Wow.  You wouldn't be lying to old, Dan, would you?  Heh, oh well, I get paid either way.";
     }   

   DialogSet(id_dialog, "\"$st_answer$\n\nHere is the $$$g_i_client_pay$ as promised.\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"Thanks!\" (return home)");
     g_i_money += g_i_client_pay;
   
    DialogChoiceWait(id_choice);

   goto load_house;
    }

   //default 

   ConversationSetPic(id_con, "interface\guy2_smile.dds");
 
   DialogSet(id_dialog, "\"Excellent job son.  Here is the $$$g_i_client_pay$ as promised plus a $$25 bonus for not hurting a single squirrel.\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"Thanks!\" (return home)");
   CreateScriptFunction("script\rep_mod.c", "GainSmall");  //reputation modification  
     g_i_money += 25;
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
