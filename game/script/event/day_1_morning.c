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
  DialogSet(i_tb, "There is a knock at your door.");
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

  DialogSet(id_dialog, "Your mother steps into your room.  You see dark circles under her eyes.  She looks tired.  She's wearing a Mr. Hamburger uniform.\n\n\"Mornin' baby doll.  Just wanted to say hi before I go.  How is your new business going?\"");
  ConversationSetPic(id_con, "interface\mom_normal.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"My lawn mower business? No calls yet.\" (Press ENTER or click this line to continue)");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  //PART 2

  DialogSet(id_dialog, "Your mother laughs.\n\n\"Hang in there champ.  I really appreciate you trying to help out with the bills.  But if we can't make ends meet.. well, Todd will let us move in with him.\"\n\nYour blood runs cold.  Todd is your mother's latest boyfriend, easily the worst of the bunch.");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Todd's an asshole.  I'll make the money we need.\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
  DialogSet(id_dialog, "\"Ok, you really want to help out?  I need to do some grocery shopping tomorrow morning.  Can you make $$$g_i_goal_money$ for me?\"");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"I'll try, mom.  Have a good day at work.\"");
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
