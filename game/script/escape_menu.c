void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_choice;

}

void Main()
{
 
int i_pause = GetPauseLevel();

if (i_pause == 200)
{
  //something more important than us is going on
  EntKill(i_my_id);
 return;
}
//now open a choice dialog, similar but different

 SetPauseLevel(100); //freeze lawn mower controls and other dialog boxes, only things created DURING
 //this pause level will be active

   id_choice = CreateDialogChoice("EscapeChoice"); //we'll use this for a menu
   EntSetLayer(id_choice, 150); //show up above stuff
   DialogSetStyleMow(id_choice);
   again:
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue playing");
  DialogChoiceAdd(id_choice, 4, "Quit to main menu");

 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

  SetPauseLevel(0); //unpause 


 if (g_i_last_choice == 1)
 {
  
  //kill the choice menu
  EntKill(id_choice); 
  EntKill(i_my_id); //kill this script
  return;
 }


if (g_i_last_choice == 4)
  {
    //quit game
  EntKill(id_choice); 
  //if room menu exists, kill it and whatnot
  //kill everything and load the main menu
  EntSetPauseLevel(i_my_id, 500);
  
  EntKillByName("RoomMenu");
 //wait(1); //let the script kill it's stuff
  EntKillByName("background");
  
  EntKillAll(); //kill all conversation and pics and scripts within pause level range
  EntSetPauseLevel(i_my_id, 0);

  CreateEntScript("Main", "script\main_menu.c");
//  PostQuitMessage();
  EntKill(i_my_id); //kill this script
  Return;
  }


}

void OnKill()
{
	
}
