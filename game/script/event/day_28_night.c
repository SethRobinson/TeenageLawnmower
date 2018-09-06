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
  DialogSet(i_tb, "Nature calls, too much coffee.");
  //  wait(300);
   //SoundPlay("sound\knock.wav");
   DialogWaitForOk(i_tb);
   EntKill(i_tb);
   //setup background
int i_bg = CreateEntPic("background"); //we can find this by name later if we want
 EntDefaultResAdjust(i_bg, 3); //simpler than using EntSetResAdjust

EntSetSprite(i_bg, "interface\bathroom.jpg"); 
  //now open a choice dialog, similar but different

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);


  DialogSet(id_dialog, "In the bathroom you see a row of empty alcohol bottles next to the sink.\n\nYou smile.");
  //ConversationSetPic(id_con, "interface\father_bigsmile.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
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
