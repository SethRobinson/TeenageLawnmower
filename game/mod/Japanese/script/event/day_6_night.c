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
  DialogSet(i_tb, "ドアを、ノックする音がする。");
  wait(300);
   SoundPlay("sound\knock.wav");
   DialogWaitForOk(i_tb);
   EntKill(i_tb);

  SoundMusicPlay("sound\talk_music.wav", 1000);

  //now open a choice dialog, similar but different

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:

  DialogSet(id_dialog, "お母さんが部屋に入ってきた。\n\n\"元気？今朝、びっくりするようなこと言ったから、まだショックから立ち直ってないんじゃないかと、心配になって…。\"");
  ConversationSetPic(id_con, "interface\mom_normal.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"全然大丈夫だよ。ひとりっこよりも、二人のほうがいいよ。兄弟が増えるの、結構楽しみにしてるんだ。\"");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  //PART 2

  DialogSet(id_dialog, "\"あなたは、本当に優しい子ね。ところで、あなたが外出中に誰が電話をかけてきたと思う？あの、トーマスからだったの。最初、信じられなかったわよ。\"");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"トーマスって、いくつもの億ションを所有する、前代未聞のお金持ちのトーマスのこと？\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
  DialogSet(id_dialog, "\"そうなのよ。冗談で電話してきたのかもね。あんなにお金持ちの人が、なんであなたを雇いたいのか、理由が見つからないじゃない？何かおかしいわよ。\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"そうだね。彼程の金持ちなら、4人乗りの高級芝刈り車ゴルフコース用とか、所有してるに違いないからね。\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
   
  DialogSet(id_dialog, "\"まぁ、そういうことよ。今日も疲れたわ…。じゃ、おやすみなさい。\"\n\nお母さんは、疲れきった顔をして、部屋を出ていった。");
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
