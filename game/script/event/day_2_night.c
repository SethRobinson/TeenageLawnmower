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
  DialogSet(i_tb, "You hear some yelling from the kitchen.");
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
  
  DialogSet(id_dialog, "You decide to investigate.  Todd is sitting at the table, scowling.\n\nYour mother greets you, it looks like she's been crying.\n\n"Hi sweetie.\"");
ConversationSetPic(id_con, "interface\mom_normal.dds");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"I heard yelling.  What's the problem?\"");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  //PART 2
 ConversationSetPic(id_con, "interface\father2_normal.dds");
 
  DialogSet(id_dialog, "\"Well sorry to bother you, your majesty,\" Todd's voice is dripping with sarcasm.\n\n\"The goddamned problem is none of your goddamned business.\"");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Mom?\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
 ConversationSetPic(id_con, "interface\mom_normal.dds");
 
 if (g_i_ate_meat == 1)
  {

      DialogSet(id_dialog, "\"Todd's right, it's private, $g_st_name$.  Let's just eat.  Doesn't that sound good?\"\n\nYour mother opens the fridge.  You see her eyes desperately searching.\n\nTodd taps his fingers on the table impatiently.");
      DialogChoiceClear(id_choice);
      DialogChoiceAdd(id_choice, 1, "\"I ate the steak that was there.\"");
     //let's wait for input, it will be put in a global var called "g_i_last_choice"
     DialogChoiceWait(id_choice);

     ConversationSetPic(id_con, "interface\father2_mad.dds");
     DialogSet(id_dialog, "Your confession enrages Todd.  He jumps up and shoves you against the stove.  You struggle but your small arms are easily held.\n\n\"Well ain't you brave.  Looks like I got a new problem in this house.  A seventeen year old thief.\"");
     DialogChoiceClear(id_choice);
     DialogChoiceAdd(id_choice, 1, "\"I paid for it!\"");
     //let's wait for input, it will be put in a global var called "g_i_last_choice"
     DialogChoiceWait(id_choice);

    ConversationSetPic(id_con, "interface\mom_normal.dds");
    DialogSet(id_dialog, "\"Leave him alone!\" your mother screams.\n\nTodd diverts his attention from you long enough to get away.  To your horror you feel tears welling up in your eyes.\n\nAshamed, you retreat to your room; closing your door - and ears.");
     DialogChoiceClear(id_choice);
     DialogChoiceAdd(id_choice, 1, "Continue");
  }

 if (g_i_ate_meat == 0)
  {

      DialogSet(id_dialog, "\"Todd's right, it's private, $g_st_name$.  Let's just eat.  Doesn't that sound good?\"\n\nYour mother opens the fridge.  She takes out the steak and puts it in the oven.");
      DialogChoiceClear(id_choice);
      DialogChoiceAdd(id_choice, 1, "Go back to your room");
  }
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
