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
  g_i_level_time = 140;  //how many seconds it takes to finish

  if (g_i_difficulty == 1)
   {
      g_i_level_time = 110;  //how many seconds it takes to finish
   }

  if (g_i_difficulty == 2)
   {
      g_i_level_time = 80;  //how many seconds it takes to finish
   }



  g_st_client_name = "クラル";
  g_st_client_home = "墓地";
  g_st_client_pic = "interface\guy6_smile.dds";
  g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 55;
  g_i_client_hours = 2;

  
    //default
    g_st_client_phone = "もしもし、クラルです。墓地の芝刈りでも、してもらえると聞いたもので…。";
    g_st_client_decline_pic = "interface\guy6_normal.dds";
    g_st_client_decline = "そうですか、それはとても残念です。";

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
  ZoneLoad("data\graveyard2.zon");
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
i_temp = randomrange(1, 3);

st_temp = "data\graveyard2_fog.wet"; //force this weather, we need the lighting to see the hill sides
if (i_temp == 1)
{
    //lightning storm 1 out of 3
  st_temp = "data\graveyard2.wet"; //force this weather, we need the lighting to see the hill sides
 } 
  EntWeatherLoad(id_weather, st_temp); //change this if the level needs it

  int id_cam = EntGetByName("Main Camera");

//set initial camera location

vec_cam = MakeVector3(94.167, 55.801, 52.376);
EntSetPosition(id_cam, vec_cam);
EntSetDirection(id_cam, 5.854);
EntSetTilt(id_cam, 0.422);

vec_cam = MakeVector3(97.172, 31.995, 45.812);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, 5.854);
EntSetTargetTilt(id_cam, 0.042);

CameraSetLerp(0.3);
CameraSetDirLerp(0.3);

//smoothly move around


  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
 
  DialogSet(id_dialog, "\"ようこそ。とても安かったので、この土地を購入したのですが、墓場にはちょっと向いてませんでした…。ははは。芝を刈るのもとても困難ですが、あなたなら出来るでしょう？\"");
   ConversationSetPic(id_con, "interface\guy6_normal.dds");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"問題ないですよ。\"");
  DialogChoiceWait(id_choice);

  

   ConversationSetPic(id_con, "interface\guy6_smile.dds");
  DialogSet(id_dialog, "\"たくさんの人が、花やお金を、お墓に御供えしていくんです。バチが当たるかもしれませんが、お金を見つけたら、拾って持って帰ってもいいですよ。\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"本当ですか！？！それではさっそく始めましょうか。\"");
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
   CreateEntScript("moneyspawn", "script\bill_spawn.c"); 
 
 CreateEntScript("INTERFACE", "script\game_interface.c"); //setup the progress indicator and such

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
//look at the shack

vec_cam = MakeVector3(97.172, 31.995, 45.812);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, 5.854);
EntSetTargetTilt(id_cam, 0.042);
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
  
    if (f_damage <= 1)
   {
     //didn't finish the job due to blowing up
   ConversationSetPic(id_con, "interface\guy6_normal.dds");
   DialogSet(id_dialog, "\"まだ終わってないじゃないですか。\"");
   DialogChoiceClear(id_choice);
   DialogChoiceAdd(id_choice, 1, "\"…。すみませんでした。\"");
   DialogChoiceWait(id_choice);
 
  DialogChoiceClear(id_choice);
   ConversationSetPic(id_con, "interface\guy6_smile.dds");
  DialogSet(id_dialog, "\"もしかしてゴーストがでましたか。あはは。\"");
if (i_bonus != 0)
  {  

  DialogSet(id_dialog, "\"ははは。ゴーストがでましたか。あはは。\"\n\n クラルが笑っている。なんだか自分まで、朗らかな気持ちになる。少ないけど$$$i_bonus$見つけたし…。");
 } 

  DialogChoiceClear(id_choice);
   DialogChoiceAdd(id_choice, 1, "\"ははは。そうですね。ゴーストが出ました。それではまた！\" 　（家に帰る）");
   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
  g_i_total_lawns_mowed -= 1;

   DialogChoiceWait(id_choice);

  goto load_house;

   }

  //didn't finish the job.
   ConversationSetPic(id_con, "interface\guy6_normal.dds");
 DialogSet(id_dialog, "\"終わってないみたいですねぇ。\"");
 if (i_bonus != 0)
  {  
 DialogSet(id_dialog, "\"終わってないみたいですねぇ。\"\n\n 時間内に仕事を終わらせることは出来なかったが、$$$i_bonus$は拾うことが出来た。");
}
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"ここまで勾配のある土地だとは予想もしていませんでした…。\"　（家に帰る）");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
  g_i_total_lawns_mowed -= 1;
g_i_money += i_bonus;
    
   DialogChoiceWait(id_choice);

  goto load_house;
  }



   //default 

   ConversationSetPic(id_con, "interface\guy6_smile.dds");
 
   DialogSet(id_dialog, "\"素晴らしい出来ですね。ここに約束の$$$g_i_client_pay$があります。\"\n\n $$$g_i_bonus$拾うことも出来た。");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"毎度ありがとうございました！\" （家に戻る）");
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
