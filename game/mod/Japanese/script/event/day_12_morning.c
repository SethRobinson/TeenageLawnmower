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

  DialogSet(id_dialog, "お母さんが、千鳥足で部屋に入ってきた。今日はミスターハンバーガーのTシャツを着ていない。");
  DialogAdd(id_dialog, "\n\n\"起きてちょうだい。困ったことがおきたわ。私たちはもう、ジ・エンドよ…。実は昨晩、仕事首になったの…。\"");
  ConversationSetPic(id_con, "interface\mom_drunk.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"首になって当たり前だよ。いつも二日酔いのべろべろの状態で、仕事に行ってたじゃないか！\"");
  DialogChoiceAdd(id_choice, 2, "\"お母さん、もうちょっと、おなかの赤ちゃんのこと考えてあげて。お酒飲むの止めようよ…。\"");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  //PART 2
  if (g_i_last_choice == 2)
  {
  DialogSet(id_dialog, "お母さんは、ひきつった顔で笑った。\n\n\"赤ちゃん？あぁ赤ちゃんね。そうそう、赤ちゃんと私の二人分、飲んでたのよ～。\"");
  }
 
if (g_i_last_choice == 1)
  {
  DialogSet(id_dialog, "\"もう、生きることに疲れたわよ。本当に意味のない、最悪の人生…。昔は医者になりたいって思ってた時期もあったのにね。母さん、落ちるとこまで落ちたわよ。\"\n\n舌がまわっておらず、何を言っているのか、あまり理解できなかった。");
  }
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");


  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
  DialogSet(id_dialog, "\"そしてとどめがあるのよ…。家のローンの支払い期限が、三日後にくるというのに、そんなお金、どこを探したってないのよ！！\n\n");
  DialogAdd(id_dialog, "まぁあなたが、なんとかお金を調達してくれたら、この家を失わなくても済むんだけどね…。無理でしょうね…。まぁいいわ。もう寝ます。ホームレス万歳！\"");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
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
