//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_con;
 int id_dialog;
 int id_choice;
 int i_tb;
 int i_temp;
int i_mood = 0; //jerrys mood
}

void Main()
{
    i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
  EntSetFadeUp(i_tb, 2.0); //fade speed.  
  wait(200)    
  DialogSet(i_tb, "You hear your mother from the kitchen.\n\n\"$g_st_name$, Jerry is here.  Again.\"");
  //  wait(300);
   //SoundPlay("sound\knock.wav");
   DialogWaitForOk(i_tb);
   EntKill(i_tb);
   //setup background
  int i_bg = CreateEntPic("background"); //we can find this by name later if we want
   EntDefaultResAdjust(i_bg, 3); //simpler than using EntSetResAdjust
  EntSetSprite(i_bg, "interface\kitchen.jpg"); 
  //now open a choice dialog, similar but different

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);

  DialogSet(id_dialog, "\"Jerry closes the fridge.  \"Hey man!  What’s the haps?  You want to play D&&D tonight?\"");
  ConversationSetPic(id_con, "interface\jerry_smile.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Sure\"");
  DialogChoiceAdd(id_choice, 2, "\"Go home, Jerry\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
   
   if (g_i_last_choice == 2)
     {
         //they don't want to play
  DialogSet(id_dialog, "\"But I.. since you starting mowing lawns everyday you've changed, man.  I hope you remember who your friends are!\"\n\nJerry leaves in a huff.");
  ConversationSetPic(id_con, "interface\jerry_normal.dds");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
    goto finish;
   }

   //start playing
    DialogSet(id_dialog, "Jerry opens his duffle bag.\n\n\"I’ll GM as usual.  Here is your character sheet from before.\"");
    ConversationSetPic(id_con, "interface\jerry_smile.dds");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "Continue");
    //let's wait for input, it will be put in a global var called "g_i_last_choice"
    DialogChoiceWait(id_choice);
    
   again:

//start playing
    DialogSet(id_dialog, "\"Ok, you are standing in a forest.  You can go south or north.\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "Go south");
    DialogChoiceAdd(id_choice, 2, "Go north");
    DialogChoiceAdd(id_choice, 3, "Go west");
    //let's wait for input, it will be put in a global var called "g_i_last_choice"
    DialogChoiceWait(id_choice);

   if (g_i_last_choice  == 1)
    {
     //they don't want to play
  DialogSet(id_dialog, "\"Ok, you go south.  A big bear surprises you and eats you.  Ha-ha!  Hilarious!\"\n\nYou sigh.  This is the problem with Jerry.  Well, one of them.");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Tell Jerry goodbye and go back to your room");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
    goto finish;
    }
   if (g_i_last_choice  == 2)
    {
  
  //they don't want to play
  DialogSet(id_dialog, "\"Ok, you start walking north.\"\n\nJerry pretends to roll a few times.\n\n\"Oh bad luck.  A random bolt of lightning killed you.  You should have a found a cave or something I guess.\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Lightning?  But you didn't even tell me there was a storm\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
 

  //they don't want to play
  DialogSet(id_dialog, "Jerry smiles.\n\n\"Ah, but you never asked.\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Kick Jerry out and go back to your room");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

    goto finish;
    }


  //if they want to piss jerry off...

 if (i_mood == 0)
   {
    ConversationSetPic(id_con, "interface\jerry_normal.dds");
//they don't want to play
  DialogSet(id_dialog, "Jerry looks agitated.\n\n\"You can't go that way!  You can only go north or south.  Let's try this again.\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Ok\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
   i_mood = 1;
   goto again;
    }

   //jerry is really pissed now

    ConversationSetPic(id_con, "interface\jerry_upset.dds");
//they don't want to play
  DialogSet(id_dialog, "Jerry face turns bright shade of red.\"You always do this!  I'm too pissed to play now.  BYE.\"\n\nJerry has left.");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Go back to your room");
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
