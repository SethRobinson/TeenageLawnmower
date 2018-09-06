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
 EntDefaultResAdjust(i_bg, 3); //simpler than using EntSetResAdjust
 EntSetLayer(i_bg, -10); //show up behind everything for sure

            i_tb = CreateDialog("");
            DialogSetStyleMow(i_tb);
           DialogSet(i_tb, "You take Excalibur and bring it home.");
           DialogWaitForOk(i_tb);
     //close everything and load the main menu
   //             EntKill(i_bg);
 
    EntKill(i_tb);
SoundMusicPlay("sound\room_music.wav", 1000);

  EntSetSprite(i_bg, "interface\kitchen.jpg");

 //start the normal conversation thing
id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
 
  DialogSet(id_dialog, "Your mother looks shocked as you push the mower into the kitchen.\n\n\"Why $g_st_name$, it’s beautiful!  You have really starting making bank on this whole mowing thing, dear.\"");
  ConversationSetPic(id_con, "interface\mom_smile.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Yeah I guess so\"");
  DialogChoiceWait(id_choice);
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogSet(id_dialog, "\"What would you say to teaming up?  I need a job and it looks like you've got more work than you can handle!\"");
DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Sorry mom, no drunks\"");
  DialogChoiceAdd(id_choice, 2, "\"Deal, partner\"");
 
  DialogChoiceWait(id_choice);
  
if (g_i_last_choice == 1)
   {
 ConversationSetPic(id_con, "interface\mom_normal.dds");

  DialogSet(id_dialog, "Your mother stomps her foot in anger.\n\n\"What?!  You know damn well I’m off the liquor!\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"But what about the drugs, mom?\"");
 
  DialogChoiceWait(id_choice);
  ConversationSetPic(id_con, "interface\mom_smile.dds");
 
  DialogSet(id_dialog, "\"Haha, you've got me there kid.  You probably saw the needles in my room.\n\nBit I swear it won't impair my lawn care judgment.  What do you say?\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Ok, I guess you're right\"");
  DialogChoiceAdd(id_choice, 2, "\"Um, no mom\"");
  DialogChoiceWait(id_choice);
 

  if (g_i_last_choice == 2)
    {
       //said no
  ConversationSetPic(id_con, "interface\mom_normal.dds");
 DialogSet(id_dialog, "Your mother looks downcast.  But after you remind her that you should NEVER operate heavy machinery while under the influence she comes to see your point of view.\n\nGreat work.");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
  DialogChoiceWait(id_choice);
  goto done;
    }

   }

//yes, work with mom

  DialogSet(id_dialog, "You and your mother shake hands.\n\n\"I’m really proud of you, $g_st_name$.  You took your father’s old rusty mower and created something great.\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
  DialogChoiceWait(id_choice);
 
done:


  EntKill(id_con); //kill all conversation elements at once 

wait(20);

 CreateEntScript("EndMenu", "script\level\level91_final.c");

 EntKill(i_my_id); //kill this script
 

}

void OnKill()
{
	
}
