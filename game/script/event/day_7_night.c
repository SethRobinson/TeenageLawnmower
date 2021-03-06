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

//int i_bg = EntGetByName("background");
// EntSetSprite(i_bg, "interface\kitchen.jpg");
// EntSetLayer(i_bg, -10); //show up behind everything for sure


   //now open a choice dialog, similar but different
  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:
  
  DialogSet(id_dialog, "Todd bangs the door open.\n\n\"Hey kid.  I need to borrow ten bucks.\"");
 ConversationSetPic(id_con, "interface\father2_normal.dds");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Get it from mom\"");
  DialogChoiceAdd(id_choice, 2, "\"Ok, here it is\"");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  
 if (g_i_last_choice == 1)
  {
  DialogSet(id_dialog, "\"Your ma is broke from supporting your dumb ass.\n\nI heard you was out mowing lawns and I know you gots the money so you better goddamn hand it over.\"");
  ConversationSetPic(id_con, "interface\father2_smile.dds");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Give Todd the money");
  DialogChoiceAdd(id_choice, 2, "Give Todd the finger");
  DialogChoiceWait(id_choice);
 
   if (g_i_last_choice == 1)
    {
     //do nothing
    }

   if (g_i_last_choice == 2)
    {
    DialogSet(id_dialog, "You casually flip Todd off.\n\n\"You little som'bitch!\"\n\nTodd puts your arm in a pain hold and grabs your wallet from the back pocket.");
    ConversationSetPic(id_con, "interface\father2_mad.dds");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "Continue");
    DialogChoiceWait(id_choice);
    }

  } 
 

 //this happens no matter what

    DialogSet(id_dialog, "Todd laughs.\n\n\"Glad I could do business with your geeky ass.\n\nJesus, your ma should’ve bought you a football or something and not some homo computer.\"");
    ConversationSetPic(id_con, "interface\father2_normal.dds");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "Continue");
 
//actually take the money

 g_i_money -= 10;
  if (g_i_money < 0)
   {
     g_i_money = 0;
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
