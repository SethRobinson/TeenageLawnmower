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

  g_i_level_time = 90;  //how many seconds it takes to finish

 if (g_i_difficulty == 1)
   {

  g_i_level_time = 73;  //how many seconds it takes to finish
   }

 if (g_i_difficulty == 2)
   {

  g_i_level_time = 60;  //how many seconds it takes to finish
   }

  g_st_client_name = "イブ";
  g_st_client_home = "家";
  g_st_client_pic = "interface\eve_smile.dds";
  g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 30;
if (g_i_day > 24)
 {
//give a little more cash now  
g_i_client_pay = 45;

  } 
g_i_client_hours = 2; //time to mow

  //I don't have to do StringExpand() now because g_i_client_pay is a global
  
    //default
      i_temp = RandomRange(1, 3);

  if (i_temp == 1)
   {
     g_st_client_phone = "もしもし、近所のイブよ。あなたのビジネスをサポートしたいわ。どうぞうちにおいでなさって。";
   }

   if (i_temp == 2)
   {
       g_st_client_phone = "もしもし、イブよ。ぜひあなたに、うちの芝を、刈ってもらいたいわ。";
  }

   if (i_temp == 3)
   {
       g_st_client_phone = "もしもし、イブよ。もしうちに芝刈りに来てくれたら、とーってもうれしいわ。どうかしら？";
  }
 
  g_st_client_phone = StringExpand(g_st_client_phone);
   g_st_client_decline_pic = "interface\eve_mad.dds";
   g_st_client_decline = "とても寂しいわ…！じゃあまたの機会にね。";

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
  ZoneLoad("data\keizer.zon");
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

 
  //create the drink
int i_count = 6;

if (g_i_difficulty == 1)
   {
     i_count += 2;
   }

if (g_i_difficulty == 2)
   {
     i_count += 4;
   }

  int i_new;

 again:
  //let's create a gopher thingie

   i_new = CreateEntityFromTemplate( "Cafe" , "lemonade");  
 
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
  ConversationSetPic(id_con, "interface\eve_smile.dds");

  i_temp = randomrange(1, 2);

if (i_temp == 1)
  {

  DialogSet(id_dialog, "イブは庭の入り口で出迎えてくれた。気絶しそうなくらい、きつい香水のにおいがする。\n\n『今日は来てくれたお礼に、レモネードを置いておいたわ。もしよかったら、どんどん飲んでね！』\n\n果たして、イブから出されたレモネードを飲んでも、安全だろうか…。");
  } 


if (i_temp == 2)
  {
      DialogSet(id_dialog, "イブは庭の入り口で出迎えてくれた。\n\n\"ああらぁ、今日は来てくれてどうもありがとう！寂しかったのよ！！！とりあえず、うちの庭、まだそんなに芝刈りが必要とも思わないんだけど…。もしあなたに刈ってもらえたら、私はとてもハッピーよ！\n\nもし暑くなったら、どうぞ遠慮しないでシャツを脱いで、裸でやってちょうだいね！誰も見てやしないんだから！\"");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"はい…。今日は寒いので、シャツは着たままでいます。お気遣いどうもありがとう。\"");
  DialogChoiceAdd(id_choice, 2, "\"はい…。そうですね、日焼けしたいので、そしたらシャツは脱いでします。\"");
  DialogChoiceWait(id_choice);
  DialogSet(id_dialog, "\"どうぞお好きにね！ところで、今日は来てくれたお礼にレモネードを庭のそこら中に置いておいたわ。見落とさないで、しっかり飲んでネ！\"\n\n イブのことだから、レモネードには何か入ってるに違いない…。間違いなく避けて通るほうが賢明だ。");
  } 


 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"それでは始めます。\"");
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
vec_cam = MakeVector3(168.958, 44.665, 60.615);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, -0.828);
EntSetTargetTilt(id_cam, 0.587);
  //closing conversation

id_con = -1;


  int i_bonus = 0;
   
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
   
   if (g_i_drunk > 2)
     {
  //didn't finish mowing
 	ShowMessage("眠たくなってきた…。");
	wait(2500);
	ShowMessage("意識を失い、地面に倒れてしまった…。");
	wait(3000);
	g_i_time += 2; //extra two hours   
	CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
        g_mouse_active = 1;
    g_i_total_lawns_mowed -= 1;
 
    int i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
   EntSetFadeUp(i_tb, 2.0); //fade speed.  
    DialogSet(i_tb, "裏庭で、2時間後に目が覚めた。\n\nなにかが違う…。分かった、シャツが裏表になっているんだ…。\n\nどうにか気を取り直し、芝刈り機をうちまで押して帰った。");
    DialogWaitForOk(i_tb);
   EntKill(i_tb);
       goto load_house;
     }

   }

  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
  g_mouse_active = 1;
  id_con = CreateConversation(""); //no name for it, we don't need to keep track
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);


  ConversationSetPic(id_con, "interface\eve_smile.dds");

  if (f_percent_mowed < 1.0)
  {
 
  //didn't finish the job.
   DialogSet(id_dialog, "完全に仕事を終える前に、芝刈り機のエンジンを止めた。そして、イブが近づいて来た。\n\n\"どうして途中で止めたの？お手洗いにでも行きたかったのかしら？？\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"いえ、家に帰ります。さようなら。\"");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;
  }

  
   //default 
    DialogSet(id_dialog, "\"あら、とてもよくしてくれたのね！ここに約束の$$$g_i_client_pay$があるわ。可愛いあなた、熱くて汗だくで、とても疲れているはずよ。もしよかったら、うちに入って、一緒にお茶でもいかが？\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"全然興味ありません。\"");
    DialogChoiceAdd(id_choice, 2, "\"ええ、ぜひ。\"");
    DialogChoiceWait(id_choice);  //let's wait for input, it will be put in a global var called "g_i_last_choice"
    CreateScriptFunction("script\rep_mod.c", "GainSmall");  //reputation modification  
    g_i_money += g_i_client_pay;
  

  if (g_i_last_choice == 1)
    {
          DialogSet(id_dialog, "\"フン。それじゃあまた次回のときにでもね。家に帰って、熱いシャワーでも浴びるんでしょ？\"");
          DialogChoiceClear(id_choice);
          DialogChoiceAdd(id_choice, 1, "イブにウィンクして、去る");
          DialogChoiceAdd(id_choice, 2, "逃げるように急いで去る");
          DialogChoiceWait(id_choice);   //let's wait for input, it will be put in a global var called "g_i_last_choice"
      goto load_house; 
   }

  if (g_i_last_choice == 2)
    {
          DialogSet(id_dialog, "家の中へと、イブについて行き、クッキーを食べた。イブの目はこちらを誘っているようだ。\n\n\"ねぇあなた…。私ね、一人暮らしでしょ？たまにとても人恋しくなるのよ…。それで提案なんだけど、一時間ぐらい、私の相手をしてくれないかしら？損はさせないわ。\"");
          DialogChoiceClear(id_choice);
          DialogChoiceAdd(id_choice, 1, "\"はい、ぜひ。\"");
     
        int i_rand = randomrange(1, 5);

        if (i_rand == 1)
        {
          DialogChoiceAdd(id_choice, 2, "\"そうやって僕のような少年をたぶらかしてばかりいるんでしょう？\"");
        }

        if (i_rand == 2)
        {
          DialogChoiceAdd(id_choice, 2, "\"貴方のようなあばずれ女はもちろんお断りです！\"");
        }

        if (i_rand == 3)
        {
          DialogChoiceAdd(id_choice, 2, "\"ごめんなさい。僕はまだ若く、貴方のようなしわくちゃおばさんはふさわしくないんです。\"");
        }

        if (i_rand == 4)
        {
          DialogChoiceAdd(id_choice, 2, "\"自分の年齢と容姿をもっとわきまえてくださいよ～。\"");
        }
 
         if (i_rand == 5)
        {
          DialogChoiceAdd(id_choice, 2, "\"最近、美容整形手術が流行ってるじゃないですか？まずはコラーゲン注入でもしてもらってみてください。全ては顔のしわが減ってからです。\"");
        }
         
     DialogChoiceWait(id_choice);   //let's wait for input, it will be put in a global var called "g_i_last_choice"
    
        if (g_i_last_choice == 1)
     {    
       //lose one hour but make a bit of cash
        i_bonus = randomrange(5, 25);
         DialogSet(id_dialog, ".......激しすぎてとても描写できない");
  DialogChoiceClear(id_choice);
   DialogChoiceAdd(id_choice, 1, " ");
    
 SoundPlay("sound\tick.wav");
    wait(500);
    SoundPlay("sound\tock.wav");
      DialogAdd(id_dialog, ".");
     wait(500);
     SoundPlay("sound\tick.wav");
    DialogAdd(id_dialog, ".");
     wait(500);
    SoundPlay("sound\tock.wav");
     DialogAdd(id_dialog, ".");
     wait(500);
     DialogAdd(id_dialog, ".");
   SoundPlay("sound\cash register.wav");

  DialogAdd(id_dialog, "\n\n一時間後、イブは頬を赤らめながら、チップとして$$$i_bonus$をくれた。\n\n\"また、一緒にエンジョイしましょうね！\"");
     
  DialogChoiceClear(id_choice);

        g_i_money += i_bonus;        
       i_temp = randomrange(1, 3);
          if (i_temp == 1)
           {
           DialogChoiceAdd(id_choice, 1, "家に帰って、急いで自分の体を消毒する");
            }
   
           if (i_temp == 2)
          {
           DialogChoiceAdd(id_choice, 1, "家に帰って、服を燃やす");
          }

           if (i_temp == 3)
          {
          DialogChoiceAdd(id_choice, 1, "家に帰って、瞑想にふける");
          }
          DialogChoiceWait(id_choice);   //let's wait for input, it will be put in a global var called "g_i_last_choice"
           goto load_house;
      }

     if (g_i_last_choice == 2)
        {    
               //decline  
    	 ConversationSetPic(id_con, "interface\eve_mad.dds");

                DialogSet(id_dialog, "\"イブは切れた。\n\n\"何でよ！腹がたつわ！早く出ていってちょうだいな！\"");
            DialogChoiceClear(id_choice);
            DialogChoiceAdd(id_choice, 1, "急いで家に帰る");
            DialogChoiceWait(id_choice);   //let's wait for input, it will be put in a global var called "g_i_last_choice"
    
         }

      }
   

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
