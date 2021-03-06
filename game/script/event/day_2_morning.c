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

  DialogSet(id_dialog, "Your mother steps into your room. \n\n\"Hey kid.  I've some bad news and some .. well, more bad news I guess.\"");
  ConversationSetPic(id_con, "interface\mom_normal.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Let me hear the bad news first.\"");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  //PART 2

  DialogSet(id_dialog, "Your mother laughs nervously.\n\n\"Well, I'm going to be short on the electricity bill.  I need $$$g_i_goal_money$ in a few days.\"\n\nYou feel a little guilty, you've been running the AC and your computer 24 hours a day.");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Yeah mom, no problem.  Hey, I saw you bought some food.  Can I have that steak I saw for breakfast?\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
  DialogSet(id_dialog, "\"Sorry baby, that's for Todd's dinner.  I didn't think you'd be happy about him staying over tonight.  I've got to get to work, I love you.\"\n\nYour mother leaves.");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

  //turn off that little pic
  ConversationSetPic(id_con, "");
  DialogSet(id_dialog, "Your stomach growls.\n\nWhy should Todd eat the steak that your money bought?");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Eat Todd's meat");
  DialogChoiceAdd(id_choice, 2, "Have a poached egg and toast");
  DialogChoiceWait(id_choice);

  if (g_i_last_choice == 1)
  {
  DialogSet(id_dialog, "You carefully microwave and eat the steak.\n\nYou feel a bit ill.  You're not sure if it's the steak or simply fear.");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
  g_i_ate_meat = 1;
  }
  if (g_i_last_choice == 2)
  {
  DialogSet(id_dialog, "You leave Todd's dinner alone.  Hopefully you can avoid him tonight.");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
  g_i_ate_meat = 0;
  }
   

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
