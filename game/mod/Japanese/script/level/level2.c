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
  g_i_level_time = 108;  //how many seconds it takes to finish

  if (g_i_difficulty == 1)
   {
      g_i_level_time = 74;  //how many seconds it takes to finish
   }

  if (g_i_difficulty == 2)
   {
      g_i_level_time = 53;  //how many seconds it takes to finish
   }



  g_st_client_name = "シェームス";
  g_st_client_home = "墓地";
  g_st_client_pic = "interface\guy3_normal.dds";
  g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 36;
  g_i_client_hours = 2;

  //I don't have to do StringExpand() now because g_i_client_pay is a global
  if (g_i_day > 7)
  {
    g_st_client_phone = "もしもし。うちの墓場の芝が、ぼうぼうに茂ってしまっているんだ。ということで、君の力を借りたいのだが…。";
  g_st_client_decline_pic = "interface\guy3_normal.dds";
      g_st_client_decline = "そうか。それでは、また次の機会に。";
    goto done;
  }
 
  if (g_i_day > 4)
  {
    g_st_client_phone = "もしもし、私はシェームスだ。墓場の管理をしているものだ。うちの芝を刈ってもらいたいのだが…。";
     g_st_client_decline_pic = "interface\guy3_smile.dds";
    g_st_client_decline = "'客を選ぶなんて、生意気な小僧だな…。まぁいい。それではまた。";
    goto done;
  }

    //default
    g_st_client_phone = "もしもし、芝はどんな場所でも刈ってもらえるのかな？もし君が小心者でないならば、とっておきの仕事を提供したいのだが、どうかね。";
    g_st_client_decline_pic = "interface\guy3_smile.dds";
    g_st_client_decline = "君まさか、この世に幽霊がいるなんて思ってないだろうね。";

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
//  SoundMusicPlay("sound\talk_music.wav", 1000);
 SoundMusicPlay("sound\graveyard_music.wav", 1000);


  if (g_st_force_level == "")
 {
  ZoneLoad("data\graveyard.zon");
 }

  if (g_st_force_level != "")
 {
  logmsg("Forcing level $g_st_force_level$.");
  ZoneLoad(g_st_force_level);
 }

 //load the weather we were told to
  int id_weather = CreateEntWeather("Weather");


//  EntWeatherLoad(id_weather, g_st_weather); //change this if the level needs it
//force foggy weather
string st_temp;

 i_temp = randomrange(1, 6);

  if (i_temp < 5)
  {
    st_temp =  "data\2_fog.wet");
   }

  if (i_temp == 5)
  {
    st_temp = "data\2_storm.wet";
  if (g_i_time > 18)
   {  
    st_temp = "data\3_storm.wet";
   }
  }

  if (i_temp == 6)
  {
    st_temp = "data\2_rain.wet";
  if (g_i_time > 18)
   {  
    st_temp = "data\3_rain.wet";
   }
  }

  
  EntWeatherLoad(id_weather, st_temp); //change this if the level needs it

  int id_cam = EntGetByName("Main Camera");

//set initial camera location

vec_cam = MakeVector3(100.383, 0.356, 65.977);
EntSetPosition(id_cam, vec_cam);
EntSetDirection(id_cam, -5.511);
EntSetTilt(id_cam, -0.253);

vec_cam = MakeVector3(93.118, 13.461, 53.164);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, -5.689);
EntSetTargetTilt(id_cam, 0.338);

CameraSetLerp(0.3);
CameraSetDirLerp(0.3);

//smoothly move around


  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
 
  DialogSet(id_dialog, "\"本日はどうぞ、宜しく。ところで、ここの墓地のお客さんが、最近よく不満を言ってこられるんだ。だから、墓の周りの芝も、見逃さないようにしっかり刈ってほしい。いいかね？\"");
   ConversationSetPic(id_con, "interface\guy3_normal.dds");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"何か他に、注意することはありますか。\"");
  DialogChoiceWait(id_choice);

  //look at the shack

vec_cam = MakeVector3(108.869, 8.426, 61.863);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, -0.672);
EntSetTargetTilt(id_cam, 0.169);


   ConversationSetPic(id_con, "interface\guy3_smile.dds");
  DialogSet(id_dialog, "\"そうだな、しっかりあたりを見回して、芝を刈ったほうがいいと思う。ここは墓地、何が起こるか分からんからな。私はあちらの小屋の中でビールでも飲みながら一息つくとするよ。\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"分かりました。じゃ、今から始めます。\"");
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

  //hey, let's get those skeleton arms going
  int id_mole_spawner = CreateEntScript("skelspawn", "script\skel_spawn.c"); 
 
 CreateEntScript("INTERFACE", "script\game_interface.c"); //setup the progress indicator and such
if (g_i_day > 7)
  {
    CreateEntScript("moneyspawn", "script\bill_spawn.c"); 
  }
 //end talk music and possibly start level music
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

  int i_bonus = g_i_bonus;
  
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
  
    if (f_damage <= 0.0)
   {
     //didn't finish the job due to blowing up
   ConversationSetPic(id_con, "interface\guy3_normal.dds");
   DialogSet(id_dialog, "\"調子はどうだい？…どうして君の芝刈り機は燃えているんだね？\"");
   DialogChoiceClear(id_choice);
   DialogChoiceAdd(id_choice, 1, "\"僕のせいじゃないよ！！！おじさん、あのお化けのこと、僕にあらかじめ教えとこうとか思わなかったの！？\"");
   DialogChoiceWait(id_choice);
 
  DialogChoiceClear(id_choice);
   ConversationSetPic(id_con, "interface\guy3_smile.dds");
DialogSet(id_dialog, "\"あれ、伝えていなかったかね？それは驚かしてすまなかったね。ははは。\"");
  
 if (i_bonus != 0)
 {
DialogSet(id_dialog, "\"あれ、伝えていなかったかね？それは驚かしてすまなかったね。ははは。まあ少なくとも、$$$i_bonus$拾えたのだから、よかったんじゃないかね？ははは。\"");

 }
   DialogChoiceClear(id_choice);
   DialogChoiceAdd(id_choice, 1, "\"こりごりですよ。\" （家に戻る）");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
  g_i_total_lawns_mowed -= 1;
   g_i_money += i_bonus;

   DialogChoiceWait(id_choice);

  goto load_house;

   }

  //didn't finish the job.
   ConversationSetPic(id_con, "interface\guy3_normal.dds");
   DialogSet(id_dialog, "\"時間切れだな。もうちょっとで終わったようだけど…。今日のところは、報酬は無しだ。\"");
    DialogChoiceClear(id_choice);

  if (i_bonus > 0)
   {
      DialogChoiceAdd(id_choice, 1, "\"そんな～あ。でも少なくとも、$$$i_bonus$拾えたから、まあいいや…。\"");
  }  
if (i_bonus == 0)
  {
    DialogChoiceAdd(id_choice, 1, "\"ああー悔しいなぁ…。\" （家に戻る）");
   }

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
  g_i_total_lawns_mowed -= 1;
  g_i_money += i_bonus;
   DialogChoiceWait(id_choice);

  goto load_house;
  }



   //default 

   ConversationSetPic(id_con, "interface\guy3_smile.dds");
 
   DialogSet(id_dialog, "\"いい出来だ。ここに約束した$$$g_i_client_pay$がある。もっていきな。\"");
  if (i_bonus > 0)
   {
     DialogSet(id_dialog, "\"いい出来だ。ここに約束した$$$g_i_client_pay$がある。もっていきな。\"\n\n　プラス、$$$i_bonus$拾った。");
  }  

  DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"毎度ありがとうございました。\" （家に帰る）");
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
