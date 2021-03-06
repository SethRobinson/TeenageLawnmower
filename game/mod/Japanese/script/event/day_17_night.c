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
  DialogSet(i_tb, "何か軽い食べ物を探しに、キッチンへ行く。");
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
  
  DialogSet(id_dialog, "お母さんが落胆しきった顔で、壁を見つめていた。右手で、空になった酒のビンを、力なさそうに掴んでいる。\n\nお母さんは、ちらっとこちらを見、そしてとてもいびつな顔で、歯を見せて微笑んだ。");
  ConversationSetPic(id_con, "interface\mom_drunk.dds");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"お母さん、死にたいの？\"");
 DialogChoiceWait(id_choice);

  DialogSet(id_dialog, "\"あなたのお父さんが亡くなった後、私はもう誰も愛することなんて出来ないと思ったわ…。\n\n今は、誰か側にいないと、寂しくて生きていけないような気がするのよ…。\"");
  ConversationSetPic(id_con, "interface\mom_drunk.dds");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"時間が癒してくれるから。あせらなくてもいいじゃないか。\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

 DialogSet(id_dialog, "\"私のおなかの中に、新しい命がやどっているなんて、とても信じられないわ。…。本当に一体、どうすればいいの？\n\n私一人で…。\"");
ConversationSetPic(id_con, "interface\mom_drunk.dds");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"僕がいるじゃないか。\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

  DialogSet(id_dialog, "\"ありがとう…本当に、貴方だけが、私の生きがいよ…。\"\n\n　お母さんは、無言で僕を抱きしめた。すすり泣く声がする。何と声をかければいいのか分からず、僕はただ、黙っているしかなかった。");
  ConversationSetPic(id_con, "interface\mom_drunk.dds");
  
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
