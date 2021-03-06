//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_con;
 int id_dialog;
 int id_choice;
 int i_tb;
 int i_temp;
}



void Main()
{
   
//let's show some dialog
 i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
  EntSetFadeUp(i_tb, 2.0); //fade speed.  
  wait(200)    
  DialogSet(i_tb, "誰かが、ドアをノックする音がする。");
  wait(300);
   SoundPlay("sound\knock.wav");
   DialogWaitForOk(i_tb);
   EntKill(i_tb);

  SoundMusicPlay("sound\talk_music.wav", 1000);
 CreateScriptFunction("script\goals.c", "Setup");  //setup their goal

  //now open a choice dialog, similar but different

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:

  DialogSet(id_dialog, "お母さんが部屋に入ってきた。\n\n\"ちょっと悪いニュースがあるの…。というより、かなり悪いニュースだわ…。\"");
  ConversationSetPic(id_con, "interface\mom_normal.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"何？とりあえず、聞かせてよ。\"");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  //PART 2

  DialogSet(id_dialog, "お母さんは、苦笑いした。\n\n\"今月の電気代が、どうしても払えそうにないのよ…。数日中に $$$g_i_goal_money$ 支払わないと、いけないの。\"\n\nそれを聞いて、罪責感を感じた。最近、自分の部屋のエアコンとコンピューターを２４時間つけっぱなしだったから。");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"いいよ、全然問題ないよ。僕がなんとか払っておくから。あ、そういえば、キッチンでステーキを見つけたんだけど、朝ごはんに食べてもいいかなぁ？\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
  DialogSet(id_dialog, "\"ごめんなさいね、あれ、トッドの晩御飯なの…。トッドが今晩、うちに泊まっていくってこと、どうしても貴方に言いにくくて…今まで黙ってたの。あ、仕事に行く時間だわ。それじゃあね。\"\n\n　母は仕事に出かけた。");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

  //turn off that little pic
  ConversationSetPic(id_con, "");
  DialogSet(id_dialog, "グーっと、おなかが鳴った。\n\nでも、どうして僕が稼いだお金で買ったステーキを、トッドが食べる権利があるんだ？");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "トッドのステーキを食べてしまえ。");
  DialogChoiceAdd(id_choice, 2, "納豆と、あつあつご飯を食べる。");
  DialogChoiceWait(id_choice);

  if (g_i_last_choice == 1)
  {
  DialogSet(id_dialog, "ステーキを温めて、食べた。\n\n　…なぜか、身震いがしてきた。ステーキが古かったのか、それとも、トッドの仕返しが怖いからなのか、自分でも分からない。");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
  g_i_ate_meat = 1;
  }
  if (g_i_last_choice == 2)
  {
  DialogSet(id_dialog, "トッドの夕飯には手をつけなかった。今晩は、彼に一切かかわらずにすむことを願う。");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
  g_i_ate_meat = 0;
  }
   

  DialogChoiceWait(id_choice);
   
   EntKill(id_con); //kill all conversation elements at once 
   
  
//let's start the real room menu
   CreateEntScript("room menu", "script\room_menu.c");
   //kill this script
   EntKill(i_my_id); //kill this script
}

void OnKill()
{
	
}
