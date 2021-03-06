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
  DialogSet(i_tb, "キッチンから、叫び声が聞こえた。");
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
  
  DialogSet(id_dialog, "お母さんが床に横たわって、泣いている。顔から血が流れている。トッドが側にいた。酔っ払っているようだ。\n\n\"このクソガキ、今すぐ部屋に戻りやがれ！！おい、今って言ってるだろう！\"");
ConversationSetPic(id_con, "interface\father2_drunk.dds");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "コンティニュー");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  //PART 2
 ConversationSetPic(id_con, "interface\father2_surprised.dds");
 
  DialogSet(id_dialog, "頭がうまく回らない。どうやらこれ以上、自分自身にコントロールをきかせることは、不可能だ。\n\nキッチンへ行き、背後からトッドの頭をつかみ、何度もテーブルに、ぶち当てた。そして、トッドのでかい体を、部屋の隅に投げつけた。トッドは、驚愕し、そして床へと、へたばった。");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"今すぐ出て行け！2度とこの家に顔を見せるな！！\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

 ConversationSetPic(id_con, "");
 
  DialogSet(id_dialog, "数分後、トッドは家から去っていった。それを見て、お母さんはベッドに横たわった。\n\nいまだに心臓がバクバクいっている…。はっきりとは分からないが、自分の中で何かが変化しているような気がする。");
 
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
