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
  g_i_level_time = 185;  //how many seconds it takes to finish

  if (g_i_difficulty == 1)
   {
     g_i_level_time = 145;  //how many seconds it takes to finish

   }

  if (g_i_difficulty == 2)
   {
     g_i_level_time = 120;  //how many seconds it takes to finish

   }



  g_st_client_name = "ゲイル";
  g_st_client_home = "ゴルフコース";
  g_st_client_pic = "interface\woman1_normal.dds";
  g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 110;
 g_i_client_hours = 6; //time to mow

  //I don't have to do StringExpand() now because g_i_client_pay is a global
  
    //default
      i_temp = RandomRange(1, 3);
 
       g_st_client_phone = "もしもし、オークゴルフコースの、ゲイルです。とても大きいエリアですが、芝刈りをお願いしたいと思います。";

 
  g_st_client_phone = StringExpand(g_st_client_phone);
   g_st_client_decline_pic = "interface\woman1_normal.dds";
   g_st_client_decline = "残念ですが、しょうがないですね。又今度、宜しくお願いします。";

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
  ZoneLoad("data\golf.zon");
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

vec_cam = MakeVector3(146.066, 37.880, 14.913);
EntSetPosition(id_cam, vec_cam);
EntSetDirection(id_cam, 5.679);
EntSetTilt(id_cam, 0.474);

vec_cam = MakeVector3(137.895, 6.925, 36.164);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, 5.679);
EntSetTargetTilt(id_cam, 0.136);



CameraSetLerp(0.3);
CameraSetDirLerp(0.3);

 
  //create the ducks
int i_count = 4;

if (g_i_difficulty == 1)
   {
     i_count += 1;
   }

if (g_i_difficulty == 2)
   {
     i_count += 2;
   }

  int i_new;

 again:

  i_new = CreateEntityFromTemplate( "Creatures" , "Duck");  
  
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
  ConversationSetPic(id_con, "interface\woman1_normal.dds");
    DialogSet(id_dialog, "若い女の人が迎えてくれた。\n\n\"ようこそいらっしゃいました。 …はっきり言って、このパー3のホールを、その小さい芝刈り機で出来るのかちょっと不安ですが…。\"");

  ConversationSetPic(id_con, "interface\woman1_normal.dds");
   DialogChoiceClear(id_choice);
  
  int i_rand = randomrange(1, 3);
  
  if (i_rand == 1)
  {
  DialogChoiceAdd(id_choice, 1, "\"このぐらいの大きさは問題ないですよ。\"");
  }

  if (i_rand == 2)
  {
  DialogChoiceAdd(id_choice, 1, "\"余裕ですよ。任せてください。\"");
  }
  if (i_rand == 3)
  {
  DialogChoiceAdd(id_choice, 1, "\"心配されなくても大丈夫です。任せてください。\"");
  }

  DialogChoiceWait(id_choice);

    DialogSet(id_dialog, "\"よかった。ところで、もしモグラを退治してくれたら、一匹につき$$2、そしてモグラが作った穴を均してくれたら、ひとつにつき $$1　お支払いしますので。でも、どうかアヒルたちは傷つけないようにしてください。もし3匹以上傷つけてしまったら、1セントたりともお支払いすることはできませんので注意してくださいね…。\"");
  ConversationSetPic(id_con, "interface\woman1_normal.dds");
    DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"分かりました。準備が出来たので始めます。\"");
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

   //hey, let's get those moles going!
  int id_mole_spawner = CreateEntScript("molespawn", "script\gopher_spawn_slow.c"); 

 //create the rock spawner
//CreateEntScript("moneyspawn", "script\bill_spawn.c"); 
 
 CreateEntScript("INTERFACE", "script\game_interface.c"); //setup the progress indicator and such

 //end talk music and possibly start level music
 SoundMusicStop();
g_i_end_level_on_accidents = 3;  //if we get three accidents, we'll end the level.
g_i_mole_kill = 1; //it's ok to kill moles
}


void FinishLevel()
{
  CreateScriptFunction("script\level\finish_global.c", "Finish");  //All scripts get this run at finish.
}  

void FinalExit()
{
  //switch camera to regular control mode and move it to the house again
  
  CameraSetFollow(0); //0 means don't follow anybody
vec_cam = MakeVector3(137.895, 6.925, 36.164);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, 5.679);
EntSetTargetTilt(id_cam, 0.136);

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


  ConversationSetPic(id_con, "interface\woman1_normal.dds");

 if (g_i_accidents >= g_i_end_level_on_accidents)
  {
  ConversationSetPic(id_con, "interface\woman1_mad.dds");
    //killed a dog
/didn't finish the job.
   DialogSet(id_dialog, "ゲイルがゴルフカートに乗ってやってきた。何か不満があるような顔をしている。\n\n\"説明不足だったでしょうか？私がお願いしたのは、無実のアヒルたちを殺さないでくれということです。大変残念ですが、今直ぐ出て行ってください。もちろんいくらもお支払いできません…。\"\n\nゲイルはアヒルのもぎ取られた頭を拾い拾い、ゴルフカードに戻って行った。");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "とぼとぼと、家に帰る");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;


  }

  if (f_percent_mowed < 1.0)
  {
 
  //didn't finish the job.
  ConversationSetPic(id_con, "");


   DialogSet(id_dialog, "芝刈り機のエンジンを止めた。n\n\"あら、どうして時間内に終わらなかったの？\"");

      g_i_money += i_bonus;
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"お客さん、この大きいゴルフコースは僕の小さい芝刈り機の許容範囲じゃないんですよ！最初から無理に決まってました…。さようなら…。\"");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;
  }
  int i_temp_money = g_i_cleaned;

  //i_temp_money *= 2;
   //default 
    ConversationSetPic(id_con, "interface\woman1_smile.dds");
DialogSet(id_dialog, "\"素晴らしい仕事をしてくれましたね！ここに約束の$$$g_i_client_pay$と、あとモグラ退治などをしてくれた分のボーナス$$$i_temp_money$があります。\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "家に帰る");
    DialogChoiceWait(id_choice);  //let's wait for input, it will be put in a global var called "g_i_last_choice"
    CreateScriptFunction("script\rep_mod.c", "GainSmall");  //reputation modification  
    g_i_money += g_i_client_pay;
  
    g_i_money += i_bonus;
     g_i_money += i_temp_money;
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
