void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_choice;
 int id_con;
 int id_dialog;

}

void Main()
{

//SetPauseLevel(100); //freeze lawn mower controls and other dialog boxes, only things created DURING

 id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
 

 int i_mod_count = GetModCount();
 int i_total_mod_count = i_mod_count; //save this for later
  dad_start:

  DialogSet(id_dialog, "Which user level/mod would you like to run?");
 // ConversationSetPic(id_con, "interface\father_bigsmile.dds");
  DialogChoiceClear(id_choice);
 

  //add all the mods
  add_mod:
  String st_mod_name;
  if (i_mod_count > 0)
   {
    st_mod_name = GetModName(i_mod_count);  
    DialogChoiceAdd(id_choice, i_mod_count, st_mod_name);
    i_mod_count -= 1;
    goto add_mod;
   } 
 
 if (i_total_mod_count == 0)
  {
    DialogChoiceAdd(id_choice, 0, "Cancel (no user levels/mods currently installed)");
  }
  
 if (i_total_mod_count > 0)
  {
    DialogChoiceAdd(id_choice, 0, "Cancel");
  }
  

  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  EntKill(id_con); //kill all conversation elements at once


 if (g_i_last_choice != 0)
  {
   //load up mod
   EntSetPauseLevel(i_my_id, 200);
  int i_bg = EntGetByName("background");
  EntSetPauseLevel(i_bg, 200); //don't let it get deleted 
  EntKillAll(); //kill all conversation and pics and scripts within pause level range
  EntSetPauseLevel(i_my_id, 0);
  Wait(100);
   logmsg("Loading mod $g_i_last_choice$.");
     ModRun(g_i_last_choice); //run mod 0 to run main game

   EntSetPauseLevel(i_bg, 0); //don't let it get deleted 
   CreateEntScript("Main", "script\init.c");
    EntKill(i_my_id); //kill this script
     
 Return;
 
  }
//  PostQuitMessage();
 // CreateEntScript("Main", "script\main_menu.c"); 
SetMouseJoystickControl(true); 
 EntKill(i_my_id); //kill this script
  
 Return;
	
}
void OnKill()
{

}