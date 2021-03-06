void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
  int id_choice;
  int id_dialog;
  int id_con;
  Vector3 vec_cam;
  int i_temp;
  Vector3  vec_pos;
int i_rand;
 
}

//client as in the "customer"

void SetClientInfo()
{
  g_i_level_time = 85;  //how many seconds it takes to finish

  if (g_i_difficulty == 1)
    {
	g_i_level_time = 60;  //how many seconds it takes to finish

    }
 
  if (g_i_difficulty == 2)
    {
	g_i_level_time = 55;  //how many seconds it takes to finish

    }

 

 g_st_client_name = "トーマス";
  g_st_client_home = "豪邸";
  g_st_client_pic = "interface\chessguy_normal.dds";
  g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 50;
 g_i_client_hours = 2; //time to mow

  //I don't have to do StringExpand() now because g_i_client_pay is a global
  
    //default
      i_temp = RandomRange(1, 3);

  if (i_temp == 1)
   {
     g_st_client_phone = "もしもし、トーマスだ。君にお願いしたい仕事があるのだが…。";
   }

   if (i_temp == 2)
   {
       g_st_client_phone = "もしもし、トーマスだ。ちょっと危険だが、報酬のいい仕事をお願いしたい。どうかな？";
  }

   if (i_temp == 3)
   {
       g_st_client_phone = "もしもし、トーマスだ。君は高所恐怖症かな？もしそうでなければ、ぜひお願いしたい仕事があるのだが…。";
  }
 
  g_st_client_phone = StringExpand(g_st_client_phone);
   g_st_client_decline_pic = "interface\chessguy_normal.dds";
   g_st_client_decline = "そうか。それではしょうがない。";

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
  ZoneLoad("data\bike.zon");
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

vec_cam = MakeVector3(82.276, -25.047, 70.275);
EntSetPosition(id_cam, vec_cam);
EntSetDirection(id_cam, -4.868);
EntSetTilt(id_cam, 0.0);


vec_cam = MakeVector3(61.199, 44.518, 61.339);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, -5.543);
EntSetTargetTilt(id_cam, 0.324);

CameraSetLerp(0.3);
CameraSetDirLerp(0.3);

 

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
  ConversationSetPic(id_con, "interface\chessguy_normal.dds");
  

  
  
  DialogSet(id_dialog, "\"$g_st_name$クン、今日はよく来てくれた。\n\n担当の庭師が、どうしてもこの仕事だけは出来ないというものだから、ぜひ君にお願いしたい。このコースは、芝刈り自体は簡単なのだが、芝までたどり着くまでが大変なのだ…。\"");

  DialogChoiceClear(id_choice);
  i_rand = randomrange(1, 10);

  if (i_rand == 1)
  {
     //this makes absolutely no sense.  but it made me laugh.  haha 
    DialogChoiceAdd(id_choice, 1, "\"見た目、ゲームのエキサイトバイクみたいなものですね。\"");
  }
 
  if (i_rand > 1)
   {

    DialogChoiceAdd(id_choice, 1, "\"…。とりあえずやってみましょうか…。\" （始める）");

   }
 
 
  skip:
 
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
CreateEntScript("moneyspawn", "script\bill_spawn.c"); 
 
 CreateEntScript("INTERFACE", "script\game_interface.c"); //setup the progress indicator and such

 //end talk music and possibly start level music
 SoundMusicStop();
g_i_end_level_on_accidents = 0;  //if we get three accidents, we'll end the level.
 //let's set the sucker to blow up if it go's below a certain height
MowSetHeightBlowUp(1, -10.0);

}


void FinishLevel()
{
  CreateScriptFunction("script\level\finish_global.c", "Finish");  //All scripts get this run at finish.
}  

void FinalExit()
{
  //switch camera to regular control mode and move it to the house again
  
  CameraSetFollow(0); //0 means don't follow anybody
vec_cam = MakeVector3(61.199, 44.518, 61.339);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, -5.543);
EntSetTargetTilt(id_cam, 0.324);

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
 
  if (f_damage <= 0.0)
    {
       //they must have fell off the cliff, poor fools
 //didn't finish the job.
   DialogSet(id_dialog, "\"まだ全部済んでいないのに、どうして帰るんだい？\"\n\n");

    g_i_money += i_bonus;
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"…どうか今日は、もう帰らせて下さい…。\"");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;
    }

  //didn't finish the job.
  ConversationSetPic(id_con, "");
   DialogSet(id_dialog, "全て終える前に、芝刈り機のエンジンを止めた。\n\n");

  if (i_bonus > 0)
    {
       DialogAdd(id_dialog, "拾った$$$i_bonus$だけ持って、家に帰ることにした。　");
    }

    if (i_bonus == 0)
    {
       DialogAdd(id_dialog, "");

    }

    g_i_money += i_bonus;
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "とぼとぼと家に帰る");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;
  }

   //default 
    DialogSet(id_dialog, "\"素晴らしい！芝までたどり着くことが出来るのか少し不安だったが、本当によくやってくれた。君の見つけた$$$i_bonus$に加えて、ここに約束していた$$$g_i_client_pay$がある。また近いうちに連絡するよ。\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "家に戻る");
    DialogChoiceWait(id_choice);  //let's wait for input, it will be put in a global var called "g_i_last_choice"
    CreateScriptFunction("script\rep_mod.c", "GainSmall");  //reputation modification  
    g_i_money += g_i_client_pay;
  
  g_i_money += i_bonus;
 
  load_house:
  //kill stuff

  //add one ot the mowed #
  g_i_total_lawns_mowed += 1;

  if (id_con != -1)
  {
  EntKill(id_con); //kill all conversation elements at once//  wait(1);
  } 
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
