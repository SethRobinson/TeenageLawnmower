//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_con;
 int id_dialog;
 int id_choice;
 int i_tb;
 
 int i_abort = 0;
}



void GetDifficulty()
{
 
    int i_bg = CreateEntPic("background"); //we can find this by name later if we want
    EntSetSprite(i_bg, "interface\house.jpg");
    EntSetLayer(i_bg, -10); //show up behind everything for sure
    EntDefaultResAdjust(i_bg, 3); //simpler than using EntSetResAdjust

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:

  DialogSet(id_dialog, "Before starting, how much of a challenge are you really up to?\n\nThe difficulty setting modifies how much money you need to earn and also affect many small changes throughout the game.\n\nThey also effect the endings.");
 // ConversationSetPic(id_con, "interface\father_bigsmile.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 0, "Easy");
  DialogChoiceAdd(id_choice, 1, "Normal");
  DialogChoiceAdd(id_choice, 2, "Mow Master");
  DialogChoiceAdd(id_choice, 3, "Cancel");
  DialogChoiceSetCurrent(id_choice, 2);
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  g_i_difficulty =  g_i_last_choice;

  if (g_i_difficulty == 0)
   {
     //set values for EASY
     g_i_deadline_money = 4000;
   }

  if (g_i_difficulty == 1)
   {
     //set values for NORMAL
     g_i_deadline_money = 6000;
   }

  if (g_i_difficulty == 2)
   {
     //set values for hard
     g_i_deadline_money = 8000;
   }

   if (g_i_difficulty == 3)
   {
     //they don't want to start a new game afterall
	i_abort = 1;
   }
  DialogChoiceClear(id_choice);
  EntKill(id_con); //kill all conversation elements at once

}

void GotName()
{
 //Special sound for the ENTER key
 SoundPlay("sound\click2.wav");
 
 g_st_name = g_st_last_string;
 if (g_st_name == "")
 {
     g_st_name = "Stranger";
 }

 EntKill(i_tb);


  g_i_day = 0; //it will get set to 0 in newday
  //show a little text  blurb welcoming them to the game
wait(200);  
  
   i_tb = CreateDialog("STORY");
   DialogSetStyleMow(i_tb);
   DialogSet(i_tb, "Welcome to middle class suburbia, $g_st_name$.\n\nDon’t be fooled by the rows of similar houses, SUV’s in every driveway and the fastidiously painted fences.\n\nAll is not well here.");
     DialogWaitForOk(i_tb);
 EntKill(i_tb);
  wait(1000);  

  //let's start the real game, starting on the newday screen
  CreateEntScript("NEWDAY", "script\\new_day.c");
   EntKill(i_bg); 
  //EntKillAllPics(); //get rid of all backgrounds and buttons on the screen
  EntKill(i_my_id);
}

void Main()
{
  EntKillAllPics(); //get rid of all backgrounds and buttons on the screen
  //Set Some defaults
   CreateEntScript("DEFAULTS", "script\setdefaults.c"); //this will set the defaults and then kill itself

 //first get difficulty level
 GetDifficulty();

if (i_abort == 1)
  {
   //go back to the main menu
   EntKill(i_bg);
   CreateEntScript("Main", "script\main_menu.c");
   EntKill(i_my_id); //kill this script
   Return;	
  }
 //now get the  name.

   i_tb = CreateEntPic("TEMP");
   EntSetSprite(i_tb, "interface\enter_name.dds");
   EntSetResAdjust(i_tb, 1, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom

   EntSetXY(i_tb, 170.0, 240.0);
   //wait for enter to get hit
  int i_input = CreatePicInput("NAME", "", 0, 2); //2nd parm is default text, 3rd is style of input. 0 for normal string input. last parm is font id
  EntSetResAdjust(i_input, 1, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom
  EntSetPicTextColor(i_input, 20,255,10,255); //the last part of the alpha
  EntSetXY(i_input, 242.0, 269.0);
  EntSetPicInputSound(i_input, "sound\highlight.wav");
  EntSetPicInputTrigger(i_input, i_my_id, "GotName"); //call us back if they hit ENTER (or escape?)
}

void OnKill()
{
	
}
