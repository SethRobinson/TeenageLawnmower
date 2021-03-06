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
  

  g_st_client_name = "隠者";
  if (g_i_day > 19)
  {
g_st_client_name = "デール";
  
  }
  g_st_client_home = "隠居場";
  g_st_client_pic = "interface\cult_upset.dds";
  g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 75;
 g_i_client_hours = 4; //time to mow

  //I don't have to do StringExpand() now because g_i_client_pay is a global
  


    //default
    g_st_client_phone = "こんにちは。芝刈りをお願いしたいのだが…。現在私は世俗を逃れて山に閑居している。でも、君に支払うお金は持っている。";
   g_st_client_decline_pic = "interface\cult_upset.dds";
    g_st_client_decline = "まぁどちらでもよかったんだ。";

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
 
  DialogSet(id_dialog, "隠者は、窓の向こうから数分間こちらをじっと伺っていた。そしてようやく出てきた。\n\n\"こんにちは。私は隠居してもう結構になる。どうも人間と接するのが苦手なのだ…。今日は、ただ芝を刈ってもらったらよろしい。もし私の愛する犬たちを傷つけるようなことがあれば、私も君を傷つけるよ。いいかね？\"");
  ConversationSetPic(id_con, "interface\cult_upset.dds");

  if (g_i_day > 19)
  {
  DialogSet(id_dialog, "デールは、窓の向こうから数分間こちらをじっと伺っていた。そしてようやく出てきた。\n\n\"フン。君のお母さんも一緒に来るかと期待していたのだが…。まあいい。仕事を始めてくれたまえ。\"");

  }

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"分かりました。それでは始めます。\"");
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
  
 DialogSet(id_dialog, "ドアが開き、隠者が姿を見せた。頬に涙がつたっている…。\n\n\"窓の向こうから、お前が何をやったか見ていたぞ…。お前は、私の愚かな4人の先妻たちと同じことをしてくれた！！ 犬の命なんてどうでもいいと思っているんだろうがっ！！！この野郎、殺してやるっ！！！\"");

  if (g_i_day > 19)
  {
DialogSet(id_dialog, "ドアが開き、デールが姿を見せた。頬に涙がつたっている…。\n\n\"窓の向こうから、お前が何をやったか見ていたぞ…。お前は、私の愚かな4人の先妻たちと同じことをしてくれた！！ 犬の命なんてどうでもいいと思っているんだろうがっ！！！この野郎、殺してやるっ！！！\"");

  }
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"事故だったんだ！！！\" 　（殺される前に、急いで逃げ帰る）");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;


  }

  if (f_percent_mowed < 1.0)
  {
 
  //didn't finish the job.
   DialogSet(id_dialog, "\"世俗に生きるやつは、こういう中途半端な仕事しか出来ないのかね？この神聖なる館から早く出ていってくれ。さもないと、私の同士たちがお前を傷つけることになるぞ！！！\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"同士って、ただのビーグルじゃないですか…。はぁぁ…。\"　 (家に帰る）");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;
  }

   ConversationSetPic(id_con, "");

   //default 
  DialogSet(id_dialog, "隠者はどこにも見当たらなかったが、玄関に手紙がおいてあるのに気がついた。それには、\"よくやってくれた。\"と一言だけ書いてあり、$$$g_i_client_pay$が同封されていた。");
 
  if (g_i_day > 19)
  {
  DialogSet(id_dialog, "デールはゆっくりドアを開き、姿を見せた。\n\n\"ここに約束の$$$g_i_client_pay$がある。今日は、よくやってくれた。お母さんに、近いうちに私に電話するように伝えておいてくれ。分かったな？\"");
  ConversationSetPic(id_con, "interface\cult_upset.dds");
  }

   DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "家に帰る");
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
