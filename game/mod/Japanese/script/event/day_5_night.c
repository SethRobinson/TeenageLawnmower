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
  DialogSet(i_tb, "誰かが、ドアをノックしている。");
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

  DialogSet(id_dialog, "友達のジェリーだった。\n\n\"元気にやってる？君に会うのは、ずいぶん久しぶりだよね。今日は、そこまできたついでに、ちょっと寄らせてもらったんだ。最近ずっと家にいなかったよなぁ？一体何をしてたの？\"");
  ConversationSetPic(id_con, "interface\jerry_smile.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"色々だよ、ジェリー。\"");
  DialogChoiceAdd(id_choice, 2, "\"色々さ。話すと長いんだよ。\"");
  DialogChoiceAdd(id_choice, 3, "\"あれやこれやさ。\"");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  //PART 2
  ConversationSetPic(id_con, "interface\jerry_normal.dds");

  DialogSet(id_dialog, "僕が相手にしないので、ジェリーはため息をついて、去っていった。ちょっとかわいそうだったかも。次回はジェリーに、もうちょっとやさしく接しないといけないな、と心に留めた。");
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
