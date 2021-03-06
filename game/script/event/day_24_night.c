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
  DialogSet(i_tb, "You hear your mother call you from the kitchen.");
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
  
  DialogSet(id_dialog, "Your mother is sitting at the table, surprisingly she looks sober.\n\n\"Hi $g_st_name$.  I wanted to talk to you about something.\"");
  ConversationSetPic(id_con, "interface\mom_smile.dds");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Sure mom, what?\"");
 DialogChoiceWait(id_choice);

  DialogSet(id_dialog, "\"Well, Todd sent me flowers today.  He said he's sorry and he stopped drinking.\n\nHe wants me to give him another chance.  What do you think?\"");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Mom, he's made those promises before\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
  ConversationSetPic(id_con, "interface\mom_normal.dds");
 
  DialogSet(id_dialog, "\"Yeah, I guess you're right.  His 'changes' last about a week tops and then...\"");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Exactly.  So are you still seeing the herm..I mean, Dale?\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

  DialogSet(id_dialog, "\"I don't think so.  You know what?  He seems a little unstable.\n\nI really want to get my life back on track.\"");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"It's never too late\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

  DialogSet(id_dialog, "\"I realize that.  It took some trial and error but now I understand that nobody else can make me happy.\n\nOnly I can make me happy, do you understand what I'm saying?\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"I guess so, mom\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

DialogSet(id_dialog, "\"I have to go work on my résumé now, bye sweetie.\"\n\nShe gives you a kiss on the forehead before leaving.");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Go back to your room");
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
