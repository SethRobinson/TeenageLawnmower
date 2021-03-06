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
  g_i_level_time = 95;  //how many seconds it takes to finish

  if (g_i_difficulty == 1)
   {

  g_i_level_time = 85;  //how many seconds it takes to finish

   }
  if (g_i_difficulty == 2)
   {

  g_i_level_time = 77;  //how many seconds it takes to finish

   }

  g_st_client_name = "ビル";
  g_st_client_home = "死の家";
   g_st_client_decline_pic = "interface\moleguy_surprised.dds";
   g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 45;
  g_i_client_hours = 4;

  //I don't have to do StringExpand() now because g_i_client_pay is a global
 

    //default
    g_st_client_phone = "もしもし、ビルだ！$g_st_name$君？ 今すぐ来てくれないか？！";
    g_st_client_pic = "interface\moleguy_surprised.dds";
    g_st_client_decline = "そんなぁ！！あぁ、僕は殺されてしまう！！";

  g_st_client_phone = StringExpand(g_st_client_phone);

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
 
  DialogSet(id_dialog, "\"うちのモグラたちが、巨大化したんだ！！！！そして、近所の子供たちを食べてしまったんだ！！！！\"");
   ConversationSetPic(id_con, "interface\moleguy_surprised.dds");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"正気ですか！？！それで僕はどうしたら…？\"");
  DialogChoiceWait(id_choice);

   ConversationSetPic(id_con, "interface\moleguy_normal.dds");
  DialogSet(id_dialog, "\"なんとかして、あいつらを殺してくれ…。どうか僕が殺されてしまう前に、あいつらを殺してくれ！！！！一匹殺すごとに $$5 支払おう…。本当に恐しいことが起こってしまった…。それと、芝刈りのほうも忘れないように、やっておいてくれよ。いいかね？\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"了解です。それでは巨大モグラ退治、プラス芝刈りを始めますよ！\"");
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
  int id_mole_spawner = CreateEntScript("molespawn", "script\mole_spawn_big.c"); 
  CreateEntScript("toolspawn", "script\tool_spawn.c"); 
 
 CreateEntScript("INTERFACE", "script\game_interface.c"); //setup the progress indicator and such

 //end talk music and possibly start level music
  SoundMusicStop();
  g_i_mole_kill = 1;
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
  ConversationSetPic(id_con, "interface\moleguy_normal.dds");

  int i_bonus = g_i_cleaned;
  i_bonus *= 5;
  
 int id_progress = EntGetByName("PROGRESS");
 float f_percent_mowed = EntMeterGet(id_progress);


   //they didn't kill enough modes to get yelled out, so make sure thet mowed everything.

  if (f_percent_mowed < 1.0)
  {
    //didn't finish the job.
     DialogSet(id_dialog, "\"フン…。どうやらモグラを殺すのに忙しくて、芝刈りに専念できなかったようだな…。というわけで、俺は君にお金を払う必要はないというわけだ！残念だったな！\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"アナタなんて、モグラに食べられてしまったらよかったのに…。\"　（家に帰る）");


   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
  g_i_total_lawns_mowed -= 1;

   DialogChoiceWait(id_choice);

  goto load_house;
  }




   //default 

   ConversationSetPic(id_con, "interface\moleguy_smile.dds");
 
   DialogSet(id_dialog, "\"助けてくれてありがとう！！！ ここに約束の $$$g_i_client_pay$がある。それと、巨大モグラを$g_i_cleaned$匹殺してくれた分のボーナス$$$i_bonus$だ。\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"ありがとうございました！！\" 　（家に戻る）");
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
