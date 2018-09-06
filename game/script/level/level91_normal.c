void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_dialog;
 int i_tb;
int i_temp;

 int id_con;
 int id_choice;
}


void Main()
{
 int i_cheat = CreateEntScript("STUFF", "script\misc_functions.c");
   EntRunFunction(i_cheat, "SetupForMenus");
   EntKill(i_cheat);

 //

 //play music
  SoundMusicStop();
 
  //create it!
int i_bg = CreateEntPic("background"); //we can find this by name later if we want
  EntSetSprite(i_bg, "interface\house.jpg");
   EntSetLayer(i_bg, -10); //show up behind everything for sure
 EntDefaultResAdjust(i_bg, 3); //simpler than using EntSetResAdjust

            i_tb = CreateDialog("");
            DialogSetStyleMow(i_tb);
           DialogSet(i_tb, "You take Excalibur and push it home.");
           DialogWaitForOk(i_tb);
     //close everything and load the main menu
   //             EntKill(i_bg);
  
  SoundMusicPlay("sound\room_music.wav", 1000);

  EntSetSprite(i_bg, "interface\kitchen.jpg");
           DialogSet(i_tb, "Your calls echo through the house.\n\nYour mother is nowhere to be found.\n\nYou decide to chill in your room.");
           DialogWaitForOk(i_tb);
  EntSetSprite(i_bg, "interface\room.jpg");
        
    EntKill(i_tb);


 //start the normal conversation thing
id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
 
  DialogSet(id_dialog, "An older woman is lying on your bed.\n\n\"Why hello, handsome.  I didn't think you'd mind if I let myself in.\"");
  ConversationSetPic(id_con, "interface\eve_smile.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Eve!  What the?  I thought I locked the place up...\"");
  DialogChoiceWait(id_choice);
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogSet(id_dialog, "\"Oh you did.  But in only half an hour I was able to jimmy a window open with some tools I just happened to have.\n\nDon't you think that's fate trying to tell us we should be together?\"");
  //ConversationSetPic(id_con, "interface\mom_smile.dds");
DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Oh yes!");
  DialogChoiceAdd(id_choice, 2, "What?! No!");
 
  DialogChoiceWait(id_choice);
  
if (g_i_last_choice == 1)
   {

  DialogSet(id_dialog, "\"Then come here you bad, bad boy and partake of the forbidden fruit!\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
 
  DialogChoiceWait(id_choice);
 
  DialogSet(id_dialog, "Your clothes and dignity fall away.\n\nA short time later Eve is gone.");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
  DialogChoiceWait(id_choice);
 

   }


if (g_i_last_choice == 2)
   {

  DialogSet(id_dialog, "As she starts to take off her shirt it requires every fiber in your being to control your gag reflexes.\n\nYou roughly push her out of your room.  You give her Dale’s address and tell her to try there.  She leaves with a smile.");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
  DialogChoiceWait(id_choice);
 
   }



  EntKill(id_con); //kill all conversation elements at once 

wait(20);

 CreateEntScript("EndMenu", "script\level\level91_final.c");

 EntKill(i_my_id); //kill this script
 

}

void OnKill()
{
	
}
