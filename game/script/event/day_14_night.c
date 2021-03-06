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
  DialogSet(i_tb, "You hear a loud banging on the door.");
  wait(300);
  SoundPlay("sound\knock.wav");
   DialogWaitForOk(i_tb);
   EntKill(i_tb);

  SoundMusicPlay("sound\talk_music.wav", 1000);
 
  
  //int i_bg = CreateEntPic("background"); //we can find this by name later if we want



   //now open a choice dialog, similar but different
  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:
  
  DialogSet(id_dialog, "\"Your mom says I’m ‘sposed to spend some quality time with your dumb ass.\"\n\nYou follow Todd out to the Kitchen, not sure what to expect.");
 ConversationSetPic(id_con, "interface\father2_normal.dds");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
  
 int i_bg = EntGetByName("background");
 EntSetSprite(i_bg, "interface\kitchen.jpg");
 EntSetLayer(i_bg, -10); //show up behind everything for sure

    DialogSet(id_dialog, "Todd gets a beer from the fridge.\n\n\"You want a brewskie, kid?\"");
    ConversationSetPic(id_con, "interface\father2_smile.dds");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "Sure");
    DialogChoiceAdd(id_choice, 2, "No thanks");
    DialogChoiceWait(id_choice);
 

if (g_i_last_choice == 1)
   {
    DialogSet(id_dialog, "Todd grabs another can and tosses it to you.\n\n\"Here.  This is a one time thing though, don’t you ever touch my beer stash, you got it?\"\n\nYou nod.");
    ConversationSetPic(id_con, "interface\father2_normal.dds");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "Continue");
    DialogChoiceWait(id_choice);
    }

if (g_i_last_choice == 2)
   {
    DialogSet(id_dialog, "Todd laughs.\n\n\"Geez kid, you really outta grow a pair.  Medical research indicates alcohol can lower your risk of something.  My gut is a small price to pay for that.\"");
    ConversationSetPic(id_con, "interface\father2_normal.dds");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "Continue");
    DialogChoiceWait(id_choice);
    }


   DialogSet(id_dialog, "Todd lights a cigarette and leans back in his chair.\n\nA few minutes of uncomfortable silence pass.");
    ConversationSetPic(id_con, "interface\father2_normal.dds");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "Continue");
     //let's wait for input, it will be put in a global var called "g_i_last_choice"
     DialogChoiceWait(id_choice);

   DialogSet(id_dialog, "\"Well goddamn, it's time for the game.  Uh, tell your ma we bonded or some such shit.\"\n\nTodd stands up and heads for the living room.");
    ConversationSetPic(id_con, "interface\father2_normal.dds");
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
