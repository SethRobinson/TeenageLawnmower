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
  g_i_level_time = 145;  //how many seconds it takes to finish

  if (g_i_difficulty == 1)
  {
	  g_i_level_time = 122;  //how many seconds it takes to finish
  }

  if (g_i_difficulty == 2)
  {
	  g_i_level_time = 110;  //how many seconds it takes to finish
  }

  g_st_client_name = "ゾラン";
  g_st_client_home = "カルト教団住宅地";
  g_st_client_pic = "interface\priest_normal.dds";
  g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 125;
  g_i_client_hours = 6; //time to mow

  //I don't have to do StringExpand() now because g_i_client_pay is a global
 

  g_st_client_decline_pic = "interface\priest_mad.dds";
    g_st_client_decline = "なんと！！お前は、神ザイロスによって、天罰を下されるであろう！！";

    //default
    g_st_client_phone = "もしもし。ザイロス教団代表のものだ。ホーリーネームはゾランだ。君に、出家修行者用住宅の芝を刈ってもらいたい。安くしてもらえるとなおありがたいんだが。";
  if (g_i_cult > 0)
     {
       //special messages for members
         g_st_client_phone = "もしもし。信仰の豊かなお恵みを受けにきてもらえるとありがたいのだが。";
         g_st_client_decline_pic = "interface\priest_normal.dds";
       g_st_client_decline = "まぁいい。とても重要な用事があるに違いない。神ザイロスは常にお前の行動を見ているぞ！！";

      }  

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
  ZoneLoad("data\cult.zon");
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
if (g_i_cult == 0)
   { 

  DialogSet(id_dialog, "ゾランは数人の信者を引き連れて、姿を現した。\n\n\"よくぞ来てくれた。今日は、神ザイロスのためにも、しっかり芝を刈って行ってもらいたい。\"");
  ConversationSetPic(id_con, "interface\priest_normal.dds");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"分かりました。\"");
  DialogChoiceWait(id_choice);


  DialogSet(id_dialog, "\"私たちは、全ての家と車を共同で使用している。故に、ここにある5件全ての家の芝を刈ってもらいたい。番犬達は、君に危害を加えないようにしっかり訓練されているから心配なされぬよう。\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"はい。それでは始めましょうか。\"");
  DialogChoiceWait(id_choice);
  }


  if (g_i_cult != 0)
   {
        //********* they are a member
  DialogSet(id_dialog, "ゾランは『こちらに来い』と合図した。\n\n\"君が入信してくれて、私たちはとてもうれしいぞ。今日はしっかりこの土地の芝を刈ってもらいたい。まぁ言い換えれば、修行のようなものだ。この土地で体を動かせば、身体を浄化し病気も改善するぞ。\"");
  ConversationSetPic(id_con, "interface\priest_normal.dds");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"ゾラン師、ありがとうございます。それでは始めさせていただきます。\"");
  DialogChoiceWait(id_choice);
   }


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

//let's spawn X amount of dogs and get started

 
 //create the rock spawner
// CreateEntScript("squirrelspawn", "script\rock_spawn.c"); 
 
 CreateEntScript("INTERFACE", "script\game_interface.c"); //setup the progress indicator and such

 //end talk music and possibly start level music
 SoundMusicStop();
g_i_end_level_on_accidents = 0;  //if we get three accidents, we'll end the level.

  //create all the dobermans
int i_count = 5;

if (g_i_difficulty == 1)
   {
     i_count += 3;
   }

if (g_i_difficulty == 2)
   {
     i_count += 6;
   }


 again:
  //let's create a gopher thingie
  int i_new = CreateEntityFromTemplate( "Creatures" , "Doberman");  
  //now let's put it in a good place

  vec_pos = GetRandomPointInSubZone("Mow");
  EntSetPosition(i_new, vec_pos);

 // LogMsg("Making gopher at $vec_pos$.");

  i_count -= 1;

 if (i_count > 0)
  {
    goto again;
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

  int i_bonus = 0;
   
 int id_progress = EntGetByName("PROGRESS");
 float f_percent_mowed = EntMeterGet(id_progress);
 float f_damage = 0;

 int id_damage = EntGetByName("DAMAGE");
 if (id_damage != -1)
  {
   f_damage = EntMeterGet(id_damage);
  }

 
  ConversationSetPic(id_con, "interface\priest_normal.dds");


   //if they are members let's do special code

  if (g_i_cult > 0)
  {
    //they are members

 if (f_percent_mowed < 1.0)
  {
    ConversationSetPic(id_con, "interface\priest_mad.dds");

  //didn't finish the job.
   DialogSet(id_dialog, "\"兄弟よ、まだ終わっていないように見えるのだが…\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"すみません、ザイロスが『もう切り上げなさい』と私に告げたような気がしたもので…。\" （家に帰る）");
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;
  }

    i_bonus = 50;

   i_bonus *= g_i_cult;

   //default 
  DialogSet(id_dialog, "ゾランと握手を交わした。\n\n\"君の素晴らしい仕事は、ザイロスの心を強く揺さぶった。ここに約束した$$$g_i_client_pay$がある。そして$$$i_bonus$のボーナスだ。\n\nところで、もうすぐ瞑想の時間なのだが、君もどうかな？\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"いや結構です。マインドコントロールはごめんです。\"");
    DialogChoiceAdd(id_choice, 2, "\"もちろん。\"");
    DialogChoiceWait(id_choice);
    g_i_money += g_i_client_pay;
   g_i_money += i_bonus;
      DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, " ");
   
  if (g_i_last_choice == 1)
    {
      //leave 
      goto load_house;
     }

    //don't let them get past heigh priest

   if (g_i_cult == 4)
     {

   //not interested
       DialogSet(id_dialog, "\"君の階級は$g_st_cult_title$まで上がった。もう修行をする必要はない。これ以上高い階級にあがってもらうと、私の地位よりも上に行ってしまうからな。それはザイロスが許さない…。と思う。\"");
       DialogChoiceClear(id_choice);
       DialogChoiceAdd(id_choice, 1, "家に戻る");
       DialogChoiceWait(id_choice);
       goto load_house;
    }



   CreateScriptFunction("script\rep_mod.c", "GainSmall");  //reputation modification  
     ConversationSetPic(id_con, "interface\priest_smile.dds");

   DialogSet(id_dialog, "ゾランは満足げに笑った。\n\n\"私たちは苦悩を超え、人生を開拓する方法を知っている。君は何について知りたいかね？\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"上の階級に上がったら、一夫多妻制のやりたいほうだいですよね？\"");
    DialogChoiceAdd(id_choice, 2, "\"どうしたら幸せになれますか？\"");
    DialogChoiceAdd(id_choice, 3, "\"本当の幸福とは何ですか？\"");
    DialogChoiceWait(id_choice);
   string st_temp;

   if (g_i_last_choice == 1)
    {
     st_temp = "そうだな。君もたくさんお布施を出して、上の階級に上がれるように努力をすべしだ。そうすれば未来は明るいぞ。";        
    }

   if (g_i_last_choice == 2)
    {
     st_temp = "幸せとは、真理の修行によって得られるものだ。しっかり修行をつめば、天界への転生が確定できる。だが、まずはお布施だ。";        
    }

   if (g_i_last_choice == 3)
    {
     st_temp = "一般的に言われる幸福とは『みかけの幸福』だ。だが、外界に幸福を求めても有限だから満たされない。";        
    }
    int i_join = 150;
    i_temp = randomrange(1, 100);
    i_join += i_temp;

    DialogSet(id_dialog, "\"$st_temp$\n\n 純粋に高い境地を求めるのであれば、追加で$$$i_join$の寄付が必要だ。\"\n\n　現在の所持金は$$$g_i_money$だ。");
    DialogChoiceClear(id_choice);
  
    if (g_i_money < i_join)
    {
         DialogChoiceAdd(id_choice, 2, "\"所持金が足りないので寄付できません。\"");
     }

    if (g_i_money >= i_join)
       {
         DialogChoiceAdd(id_choice, 1, "\"もちろん寄付します。\"");
         DialogChoiceAdd(id_choice, 2, "\"いいえ、結構です\"");
       }

    DialogChoiceWait(id_choice);
   
    if (g_i_last_choice == 2)
    {
        //not interested
       DialogSet(id_dialog, "\"そうか。それではまた別の機会に。神の怒りを受くべき者にならぬよう気をつけたまえ。");
       DialogChoiceClear(id_choice);
       DialogChoiceAdd(id_choice, 1, "家に帰る");
       DialogChoiceWait(id_choice);
       goto load_house;
    }

    //they want to upgrade
   //set their new status
   g_i_cult += 1;


  if (g_i_cult == 2)
    { 
       g_st_cult_title = "『聖者』";
    }

 if (g_i_cult == 3)
    { 
     g_st_cult_title = "『正悟師』";
    }

 if (g_i_cult == 4)
    { 
      g_st_cult_title = "『正大師』";
    }
   

    g_i_money -= i_join; //pay for membership
   ConversationSetPic(id_con, "interface\priest_smile.dds");
        SoundPlay("sound\cash register.wav");
   DialogSet(id_dialog, "ゾランにお布施を渡すと、彼はそれを急いでポケットにしまった。\n\n\"素晴らしい！おめでとう！！現在の君の階級は$g_st_cult_title$だ、$g_st_name$.\"\n\n　新しいバスローブらしきものをいただいた。");
   DialogChoiceClear(id_choice);
   DialogChoiceAdd(id_choice, 1, "家に戻る");
   DialogChoiceWait(id_choice);
      goto load_house;
 
   }










///************ NOT MEMBERS ***********

  if (f_percent_mowed < 1.0)
  {
    ConversationSetPic(id_con, "interface\priest_mad.dds");

  //didn't finish the job.
   DialogSet(id_dialog, "\"どうして終わっていないのだ？？これは裏切り行為だぞ！！\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "逃げるように家に帰る");
   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;
  }

  

   //default 
  DialogSet(id_dialog, "ゾランが近寄って来た。\n\n\"ザイロスは君の仕事に大変満足しておられるぞ！ここに約束した$$$g_i_client_pay$がある。持っていきなさい。それはそうと、私たち教団についてもう少し詳しくお話したいのだが、時間はあるかな？\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"いいえ、さようなら。\"");
    DialogChoiceAdd(id_choice, 2, "\"はい。もっと教えてください。\"");
    DialogChoiceWait(id_choice);
    g_i_money += g_i_client_pay;
 
      DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, " ");
   
  if (g_i_last_choice == 1)
    {
      //leave 
      goto load_house;
     }

   

  //they want to hear more


   CreateScriptFunction("script\rep_mod.c", "GainSmall");  //reputation modification  
     ConversationSetPic(id_con, "interface\priest_normal.dds");

   DialogSet(id_dialog, "ゾランは満足げな顔をした。\n\n\"基本的に、私たちは全ての魂の本当の幸福、自由、歓喜を願って修行を続けている教団だ。単純に物質を手に入れることや、欲望、煩悩を満足させることによって求められる幸福ではないと考えている。\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"要するに、カルト教団ですか？?\"");
    DialogChoiceAdd(id_choice, 2, "\"ところでザイロスって誰ですか？\"");
    DialogChoiceAdd(id_choice, 3, "\"素敵なバスローブですね\"");
    DialogChoiceWait(id_choice);

   if (g_i_last_choice == 1)
    {
     st_temp = "カルト教団？私たちはそうは思っていない。周りからはカルト呼ばわりされるがの、ほっほっほ。";        
    }

   if (g_i_last_choice == 2)
    {
     st_temp = "煩悩を満たし快楽を追求することがよしとされる現代にあって、その価値観に疑問を持ち、自己の欲望との闘いに果敢に挑戦し始めた人、それがザイロス師だ。今はもうこの世にはいないが、天から私たち聖者を常に見張っておる。";        
    }

   if (g_i_last_choice == 3)
    {
     st_temp = "ありがとう。スタイリッシュなスリムラインで、かつとても柔らかい心地の良い素材だ。お布施で集まった金で購入したものだ。宗教団体はいいものだ、ほっほっほ。";        
    }
   i_join = 250;
   DialogSet(id_dialog, "\"$st_temp$\n\nそれはそうと、もし$$$i_join$寄付する気があるのなら、君も入信できるぞ。\"\n\n （現在の所持金は $$$g_i_money$）");
    DialogChoiceClear(id_choice);
  
    if (g_i_money < i_join)
    {
         DialogChoiceAdd(id_choice, 2, "\"すみません、所持金が足りないので。\"");
     }

    if (g_i_money >= i_join)
       {
         DialogChoiceAdd(id_choice, 1, "\"もちろん！入団申し込み用紙を下さい。\"");
         DialogChoiceAdd(id_choice, 2, "\"いいえ、結構です。\"");
       }

    DialogChoiceWait(id_choice);
   
    if (g_i_last_choice == 2)
    {
        //not interested
       DialogSet(id_dialog, "\"そうか。それではまた別の機会に。);
       DialogChoiceClear(id_choice);
       DialogChoiceAdd(id_choice, 1, "家に戻る");
     DialogChoiceWait(id_choice);
     goto load_house;
    }

    //if we got here they want to join the cult
    g_i_money -= i_join; //pay for membership
   ConversationSetPic(id_con, "interface\priest_smile.dds");
        SoundPlay("sound\cash register.wav");
   DialogSet(id_dialog, "お布施をゾランに渡すと、彼はそれを急いでポケットにしまった。\n\n\"君は正しい選択をした！必ず神の豊かなお恵みがあるだろう。\"\n\nゾランと握手を交わし、玄関先まで送ってもらった。");
   DialogChoiceClear(id_choice);
   g_i_cult = 1; //joined, level 1
   g_st_cult_title = "ゾラン";  
   DialogChoiceAdd(id_choice, 1, "家に戻る");
   
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
