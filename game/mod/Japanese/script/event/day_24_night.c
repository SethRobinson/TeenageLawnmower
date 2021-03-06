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
  DialogSet(i_tb, "お母さんが、キッチンから僕の名前を呼んでいる。");
//  wait(300);
   //SoundPlay("sound\knock.wav");
   DialogWaitForOk(i_tb);
   EntKill(i_tb);

  SoundMusicPlay("sound\talk_music.wav", 1000);
 
  
  //int i_bg = CreateEntPic("background"); //we can find this by name later if we want

int i_bg = EntGetByName("background");
 EntSetSprite(i_bg, "interface\kitchen.jpg");
 EntSetLayer(i_bg, -10); //show up behind everything for sure


   //now open a choice dialog, similar but different
  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:
  
  DialogSet(id_dialog, "お母さんが、一人テーブルについている。驚いたことに、今日は飲んでいないようだ。\n\n\"おはよう$g_st_name$。ちょっと話したいことがあって…。\"");
  ConversationSetPic(id_con, "interface\mom_smile.dds");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"いいよ、何？\"");
 DialogChoiceWait(id_choice);

  DialogSet(id_dialog, "\"あのね、トッドが今日、自宅に花を送ってきてくれたの…。メッセージに、「今までのことは、とても反省している。これからは酒を断つ。」というようなことが書いてあったの。\n\n彼はもう一回、やり直すチャンスがほしいみたいなの。どうしたらいいと思う？\"");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"お母さん、トッドは前も同じようなことを、言っていたじゃないか！\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
  ConversationSetPic(id_con, "interface\mom_normal.dds");
 
  DialogSet(id_dialog, "\"そうよね…。一週間ぐらいは、変わろうと努力をしていたけど、すぐに元通りの彼に戻ったわよね…。\"");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"その通りだよ。ところで、変人デールとはどうなったの？\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

  DialogSet(id_dialog, "\"あの人とはもう別れたわ。彼、ちょっと精神面で問題ありなの…。不潔で、におうし…。\n\n私はただ、ごく普通の生活に戻りたいのよ。\"");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"決して今からでも遅くはないよ！\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

  DialogSet(id_dialog, "\"私もそう思うわ。いろんな試練があったけど、やっと気づくことができたの。誰も私を幸せにすることは出来ないって。自分自身で努力して、幸せを築いていくしかないのよね。お母さん、間違ってないわよね？\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"うん、僕もそう思うよ。\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

DialogSet(id_dialog, "\"今から、就職活動するために、履歴書でも書くわ。じゃあまた後でね。\"\n\n軽く手を振り、キッチンから出ていった。");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "自分の部屋に戻る");
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
