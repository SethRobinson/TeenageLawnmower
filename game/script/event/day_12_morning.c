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

  DialogSet(id_dialog, "Your mother staggers into the room.  You notice she isn't wearing her Mr. Hamburger uniform.");
  DialogAdd(id_dialog, "\n\n\"Wake up, sweetie.  Guess what?  We're officially screwed.  I lost my damn job last night.\"");
  ConversationSetPic(id_con, "interface\mom_drunk.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Mom, what did you think was going to happen?  You've been going in hung-over every other day!\"");
  DialogChoiceAdd(id_choice, 2, "\"Mom, think of the baby.  Please stop drinking.\"");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  //PART 2
  if (g_i_last_choice == 2)
  {
  DialogSet(id_dialog, "Your mother laughs.\n\n\"Baby?  The baby?  That just means I'm drinking for two, honey.\"");
  }
 
if (g_i_last_choice == 1)
  {
  DialogSet(id_dialog, "\"You think it's so easy living this life?  I was going to be a doctor\"\n\nYou have some difficulty making out the slurred speech.");
  }
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");


  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
  DialogSet(id_dialog, "\"Do you know what the real kicker is?  The mortgage is due in three days and guess what? I don't have it!\n\n");
  DialogAdd(id_dialog, "Unless you've got some magic trick up your sleeve we're going to lose the house.  I'm going back to bed.\"");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
   
   EntKill(id_con); //kill all conversation elements at once 
   
  
//let's start the real room menu
   CreateEntScript("RoomMenu", "script\room_menu.c");
   //kill this script
   EntKill(i_my_id); //kill this script
}

void OnKill()
{
	
}
