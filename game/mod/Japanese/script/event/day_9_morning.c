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
    i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
  EntSetFadeUp(i_tb, 2.0); //fade speed.  
  wait(200)    
  DialogSet(i_tb, "キッチンから、何か食べるものでも取ってこようか。");
  //  wait(300);
   //SoundPlay("sound\knock.wav");
   DialogWaitForOk(i_tb);
   EntKill(i_tb);
   //setup background
  int i_bg = CreateEntPic("background"); //we can find this by name later if we want
  EntSetSprite(i_bg, "interface\kitchen.jpg"); 
  //now open a choice dialog, similar but different

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);

  DialogSet(id_dialog, "お母さんが、床に寝そべっていた。\n\n顔にたくさんのあざがある。\n\n\"どうやら私、気絶してたみたいね…。\"");
  ConversationSetPic(id_con, "interface\mom_bruise.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"一体何が起こったの？\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
  
   //start playing
    DialogSet(id_dialog, "\"間違った種類のオレンジジュース、買ってきちゃったわ…。\"\n\nお母さんは、足を引きずりながらバスルームへ行き、ドアを閉めた。");
     DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "部屋に戻る");
    //let's wait for input, it will be put in a global var called "g_i_last_choice"
    DialogChoiceWait(id_choice);

  finish:
 
   EntKill(id_con); //kill all conversation elements at once 
   //let's start the real room menu
   CreateEntScript("room menu", "script\room_menu.c");
   //kill this script
   EntKill(i_my_id); //kill this script
}

void OnKill()
{
	
}
