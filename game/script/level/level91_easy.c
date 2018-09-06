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
   SoundMusicPlay("sound\graveyard_music.wav", 1000);
 
  //create it!
int i_bg = CreateEntPic("background"); //we can find this by name later if we want
  EntSetSprite(i_bg, "interface\house.jpg");
   EntSetLayer(i_bg, -10); //show up behind everything for sure
 EntDefaultResAdjust(i_bg, 3); //simpler than using EntSetResAdjust

            i_tb = CreateDialog("");
            DialogSetStyleMow(i_tb);
           DialogSet(i_tb, "You take Excalibur and push it home.\n\nIn your excitement you don't notice Todd's truck out front.");
           DialogWaitForOk(i_tb);
     //close everything and load the main menu
   //             EntKill(i_bg);
  

  EntSetSprite(i_bg, "interface\kitchen.jpg");
           DialogSet(i_tb, "Your calls echo through the house.\n\nYour mother is nowhere to be found.\n\nYou notice the bathroom light is on.");
           DialogWaitForOk(i_tb);
  EntSetSprite(i_bg, "interface\bathroom.jpg");
           DialogSet(i_tb, "Todd's body is here, slumped over the toilet.  A revolver lies on the floor.\n\nYou don't have to pull back the shower curtain to know the other body is your mother.");
           DialogWaitForOk(i_tb);

  SoundMusicPlay("sound\room_music.wav", 1000);
  EntSetSprite(i_bg, "interface\house.jpg");
        

      DialogSet(i_tb, "You shake your head, that was a crazy daydream.\n\nYou go inside the house and look for your mother.");
        DialogWaitForOk(i_tb);

  EntSetSprite(i_bg, "interface\kitchen.jpg");
    EntKill(i_tb);


 //start the normal conversation thing
id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
 
  DialogSet(id_dialog, "\"Hi sweetie.  Wow, that's a great looking new mower!  I guess that old Mr. Beaumont isn't such a weird freak after all.\"");
  ConversationSetPic(id_con, "interface\mom_smile.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"No mom, he still is, but yeah, it's a nice mower\"");
  DialogChoiceWait(id_choice);
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogSet(id_dialog, "Your mother laughs.  She really seems like her old self again.\n\n\"No get that thing out of the kitchen this instant!\"");
  ConversationSetPic(id_con, "interface\mom_smile.dds");
DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
 
  DialogChoiceWait(id_choice);
   EntKill(id_con); //kill all conversation elements at once 
 

wait(20);

 CreateEntScript("EndMenu", "script\level\level91_final.c");

 EntKill(i_my_id); //kill this script
 

}

void OnKill()
{
	
}
