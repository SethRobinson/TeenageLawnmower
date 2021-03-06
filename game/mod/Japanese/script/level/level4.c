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

 if (g_i_difficulty == 1)
   {
	  g_i_level_time = 76;  //how many seconds it takes to finish
   }

 if (g_i_difficulty == 2)
   {
	  g_i_level_time = 55;  //how many seconds it takes to finish
   }


  g_st_client_name = "デイビット";
  g_st_client_home = "開拓地";
  g_st_client_pic = "interface\guy7_normal.dds";
  g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 80;

 g_i_client_hours = 6; //time to mow

  //I don't have to do StringExpand() now because g_i_client_pay is a global
  
 if (g_i_day > 8)
  {
    g_st_client_phone = "もしもし、デイビットだ。うちの会社が所有する山の芝刈りをしてもらえるかね？";
    g_st_client_decline_pic = "interface\guy7_normal.dds";
    g_st_client_decline = "そうか…。君ぐらいしかこの仕事を引き受ける人はいないと思うので、また電話することにするよ。";
    goto done;
  }
 
 if (g_i_day > 4)
  {
    g_st_client_phone = "もしもし、デイビットだ。僕のクライアントの所有地の芝刈りをお願いしたいと思う。どうかね？";
    g_st_client_decline_pic = "interface\guy7_normal.dds";
    g_st_client_decline = "そうか…。危険を伴う仕事だからね。しょうがないな。また電話するよ。";
    goto done;
  }

    //default
    g_st_client_phone = "もしもし。現在開拓中の土地の芝刈りをお願いできないかな。ちょっと危険な仕事になると思うけども、報酬はいいよ。どうかい？";
    g_st_client_decline_pic = "interface\guy7_normal.dds";
    g_st_client_decline = "しょうがないね、とても危険だから…。じゃあまたの機会に。";

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
  ZoneLoad("data\rollingrocks.zon");
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

vec_cam = MakeVector3(59.469, 17.828, 186.547);
EntSetPosition(id_cam, vec_cam);
EntSetDirection(id_cam, -3.545);
EntSetTilt(id_cam, 0.080);



vec_cam = MakeVector3(156.065, 23.811, 173.144);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, -2.278);
EntSetTargetTilt(id_cam, 0.249);


CameraSetLerp(0.3);
CameraSetDirLerp(0.3);

//smoothly move around


  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
 
  DialogSet(id_dialog, "\"君が$g_st_name$クンかい？よく来てくれたね。ちょっとここの仕事はハードだと思うが、ぜひ頑張ってくれたまえ。");
  DialogAdd(id_dialog, "\n\nそんなに時間のかかる仕事ではないと思うんだが…。とにかく、頭上注意だよ。分かった？\"");
  ConversationSetPic(id_con, "interface\guy7_normal.dds");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"準備ができました。始めますよ。\"");
  DialogChoiceWait(id_choice);


//   ConversationSetPic(id_con, "interface\guy7_worry.dds");
//  DialogSet(id_dialog, "\"Kids love the squirrels.  If they find squirrel body parts it’s really gonna crush ‘em.  So be real careful, ok?\"");
//  DialogChoiceClear(id_choice);
//  DialogChoiceAdd(id_choice, 1, "\"You got it, let's start.\"");
//  DialogChoiceWait(id_choice);

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
 CreateEntScript("squirrelspawn", "script\rock_spawn.c"); 
 
 CreateEntScript("INTERFACE", "script\game_interface.c"); //setup the progress indicator and such

 //end talk music and possibly start level music
 SoundMusicStop();
g_i_end_level_on_accidents = 0;  //if we get three accidents, we'll end the level.
 //let's set the sucker to blow up if it go's below a certain height
MowSetHeightBlowUp(1, -20.0);
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
   ConversationSetPic(id_con, "interface\guy7_surprised.dds");
   DialogSet(id_dialog, "\"終わってないのに、帰るのかい？\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"岩なんて大嫌いだ…。さようなら。\"  （家に戻る）");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;
  }

  

   //default 

   ConversationSetPic(id_con, "interface\guy7_smile.dds");
 
   DialogSet(id_dialog, "\"よくしてくれたね。どうもありがとう。 ここに約束の$$$g_i_client_pay$がある。どうぞ。\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"ありがとうございます！\" （家に戻る）");
   CreateScriptFunction("script\rep_mod.c", "GainSmall");  //reputation modification  
    g_i_money += g_i_client_pay;
  
//    MoneyMod(25); //bonus

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
