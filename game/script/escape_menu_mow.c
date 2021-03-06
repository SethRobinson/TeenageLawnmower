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
 CreateEntScript("", "script\mouse_on.c");

   id_choice = CreateDialogChoice("EscapeChoice"); //we'll use this for a menu
   EntSetLayer(id_choice, 150); //show up above stuff
   DialogSetStyleMow(id_choice);
   again:
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue Mowing");
  DialogChoiceAdd(id_choice, 2, "Quit Mowing And Go Home");
  DialogChoiceAdd(id_choice, 3, "Options");
  DialogChoiceAdd(id_choice, 4, "Quit to Windows");

 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

  SetPauseLevel(0); //unpause 
  EntKill(id_choice); 


 if (g_i_last_choice == 1)
 {
  
  //kill the choice menu
 if ( g_mouse_active == 0)
  {
    //the mouse we had one
    CreateEntScript("", "script\mouse_off.c");
  }
  EntKill(i_my_id); //kill this script
  return;
 }

if (g_i_last_choice == 2)
  {
  //go back to room
  int i_level_script = EntGetByName("LevelScript"); //our main script
 if ( g_mouse_active == 0)
  {
    //the mouse we had one
    CreateEntScript("", "script\mouse_off.c");
  }

  g_i_quit = 1; //disable the escape menu  
  EntRunFunction(i_level_script, "FinishLevel");
  EntKill(i_my_id); //kill this script
  }
if (g_i_last_choice == 3)
  {

  CreateEntScript("", "script\config.c"); 
  return;
 // EntKill(i_my_id); //kill this script
  }
if (g_i_last_choice == 4)
  {
    //quit game
 int i_demo = GetDemoMode();
if (i_demo == 1)
{
CreateEntScript("NAG", "script\\nag.c");
 EntKill(i_my_id); //kill this script
return;
	
} 
  EntKill(i_my_id); //kill this script
  PostQuitMessage();
  }

}

void OnKill()
{
	
}
