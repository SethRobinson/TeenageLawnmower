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

  DialogSet(id_dialog, "お母さんが部屋に入ってきた。目の下の、大きなくまが目立つ。とても疲れているようだ。ミスターハンバーガーのTシャツを着ている。\n\n\"おはよう。出かける前に、ちょっとあなたの顔だけ見ておこうと思って…。それで、新しく始めたビジネスの調子は、どう？\"");
  ConversationSetPic(id_con, "interface\mom_normal.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"芝刈りビジネス？ぼちぼちかな。");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  //PART 2

  DialogSet(id_dialog, "母は笑いながら言った。\n\n\"頑張ってね。あなたが、私たちを経済的に援助しようとしてくれていることに、とても感謝しているわ。だけど、もし借金を返済することが出来なかったら、トッドが私たちを彼のところへひきとってくれるみたいだから…。\"\n\n　血の気がひいた。トッドは、お母さんの前のボーイフレンドだ。至上最悪の人間だ。");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"トッドなんてクソ野郎に、死んでも世話になりたくないよ。僕がちゃんと、家族に必要なお金を儲けてくるからさ、心配しないで。\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
  DialogSet(id_dialog, "\"そうなの？それじゃあ、さっそくで申し訳ないんだけど、明日の朝、食料品を買いに行くお金が必要なの。$$$g_i_goal_money$ 用意してもらえるかしら？\"");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"なんとかするよ。仕事、がんばってきてね。\"");
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
