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

  //now open a choice dialog, similar but different

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:

  DialogSet(id_dialog, "Your mother enters the room.\n\n\"How ya doin' kid?  You must still be in shock after this morning's announcement.\"");
  ConversationSetPic(id_con, "interface\mom_normal.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"It's cool mom.  I never liked being an only child, anyway.\"");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  //PART 2

  DialogSet(id_dialog, "\"That's sweet.  Oh, by the way, guess who called while you were out?  Thomas Beaumont.  I couldn’t believe it.\"");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"That the guy with the huge mansion outside of town?\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
  DialogSet(id_dialog, "\"Yep.  Supposed to be a real nut-case.  I don’t trust him, $g_st_name$, why would a rich guy like that want you to mow his lawn?  Something doesn’t compute.\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Yeah.  I bet he already has some kind of fancy riding mower\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
   
  DialogSet(id_dialog, "\"Anyway, it's been a long day.  G'night hon.\"\n\nYour mother gives you a peck on the forehead before leaving the room.");
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
