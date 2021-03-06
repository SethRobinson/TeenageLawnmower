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
    g_i_client_pay = 200;

  g_i_level_time = 170;  //how many seconds it takes to finish
 
   if (g_i_difficulty == 1)
  {
    g_i_level_time = 150;  //how many seconds it takes to finish
  g_i_client_pay = 200;

  }
  if (g_i_difficulty == 2)
  {
    g_i_level_time = 125;  //how many seconds it takes to finish
  g_i_client_pay = 200;
  }
  

  g_st_client_name = "トーマス";
  g_st_client_home = "豪邸";
  g_st_client_pic = "interface\chessguy_normal.dds";
  g_st_client_sound = "sound\hellog1.wav";
 g_i_client_hours = 4; //time to mow

  //I don't have to do StringExpand() now because g_i_client_pay is a global
  
      g_st_client_phone = "もしもし、トーマスだ。君に仕事を依頼したいのだが…。";
 
 if (g_i_thomas_story == 0)
 {
       g_st_client_phone = "もしもし、トーマスだ。君にしか出来ない仕事があるのだが…。";
  }
 
  g_st_client_phone = StringExpand(g_st_client_phone);
   g_st_client_decline_pic = "interface\chessguy_normal.dds";
   g_st_client_decline = "そうか。残念だな。君を高く評価し過ぎていたようだ。";

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
  ZoneLoad("data\chess.zon");
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

vec_cam = MakeVector3(53.462, 3.780, 52.730);
EntSetPosition(id_cam, vec_cam);
EntSetDirection(id_cam, 0.687);
EntSetTilt(id_cam, 0.464);

vec_cam = MakeVector3(20.423, 29.854, 12.477);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, 0.687);
EntSetTargetTilt(id_cam, 0.464);

CameraSetLerp(0.3);
CameraSetDirLerp(0.3);

 
  //create the dogs
int i_count = 0;

if (g_i_difficulty == 1)
   {
     i_count += 3;
   }

if (g_i_difficulty == 2)
   {
     i_count += 5;
   }

if (i_count == 0)
   {
     //easy level, no dogs
     goto nodogs;

   }

  int i_new;

 again:
  //let's create a gopher thingie

 //  i_new = CreateEntityFromTemplate( "bill" , "bill20");  
  i_new = CreateEntityFromTemplate( "Creatures" , "Doberman");  
  
  //now let's put it in a good place

  vec_pos = GetRandomPointInSubZone("Mow");
  EntSetPosition(i_new, vec_pos);

 // LogMsg("Making gopher at $vec_pos$.");

  i_count -= 1;

 if (i_count > 0)
  {
    goto again;
  }

 nodogs:



//smoothly move around

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
  ConversationSetPic(id_con, "interface\chessguy_normal.dds");
  
 
  
  DialogSet(id_dialog, "警備員に導かれ、トーマスのいる『チェスガーデン』に着いた。\n\n\"$g_st_name$君、よくぞ再び『チェスガーデン』に来てくれた。前回は大活躍だったな。また、やってくれるかね？\"");
  DialogChoiceClear(id_choice);

  i_rand = randomRange(1, 2);

  

  if (i_rand == 1)
   {
     DialogChoiceAdd(id_choice, 1, "\"はい、さっそく始めましょうか！\"");
   }
  if (i_rand == 2)
   {
     DialogChoiceAdd(id_choice, 1, "\"準備はできました。\"");
   }
 
    i_rand = randomRange(1, 8);
   if (i_rand == 1)
   {
     DialogChoiceAdd(id_choice, 1, "\"さっそく始めましょう。\"");
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

g_i_spawn_count = 10; //tell teh creatures to start moving around

//let's start that weird random gong sound
CreateEntScript("GONG", "script\level\gong.c"); //setup the progress indicator and such

}


void FinishLevel()
{
  CreateScriptFunction("script\level\finish_global.c", "Finish");  //All scripts get this run at finish.
}  

void FinalExit()
{
  //switch camera to regular control mode and move it to the house again
  
  CameraSetFollow(0); //0 means don't follow anybody
vec_cam = MakeVector3(20.423, 29.854, 12.477);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, 0.687);
EntSetTargetTilt(id_cam, 0.464);

  //closing conversation

id_con = -1;
 int i_bonus =g_i_bonus;
    
 int id_progress = EntGetByName("PROGRESS");
 float f_percent_mowed = EntMeterGet(id_progress);
 float f_damage = 0;

 int i_won = 0;
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
   DialogSet(id_dialog, "仕事が完全に済む前に、芝刈り機のエンジンを止めた。\n\n\"どうして全部、終わらせなかったのかな？まぁしょうがない。次回を期待しているよ。\"\n\n");

  if (i_bonus > 0)
    {
       DialogAdd(id_dialog, "拾った$$$i_bonus$を手に、帰る準備をする…");
    }

    if (i_bonus == 0)
    {
       DialogAdd(id_dialog, "");

    }

    g_i_money += i_bonus;
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "とぼとぼと、家に帰る");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;
  }

   //default 
    DialogSet(id_dialog, "\"とても驚いたよ。素晴らしい出来だ。君が拾った$$$i_bonus$と一緒に、最初に約束した$$$g_i_client_pay$を持っていきなさい。ほら。\"");
if (i_bonus == 0)
  {
    DialogSet(id_dialog, "\"とても驚いたよ。素晴らしい出来だ。ここに約束の$$$g_i_client_pay$がある。どうぞ。\"");
  } 
       DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"ありがとうございました！\"");
i_won = 1;
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
  
 
  //instead of loading the room, let's load the special ending tribute
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
