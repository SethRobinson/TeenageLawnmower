void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_choice;
 int i_max_saves = 6;
}

void Main()
{
 
   //this pause level will be active
   SetPauseLevel(50); //freeze lawn mower controls and other dialog boxes, only things created DURING

   id_choice = CreateDialogChoice("LoadChoice"); //we'll use this for a menu
   EntSetLayer(id_choice, 150); //show up above stuff
   DialogSetStyleLoad(id_choice);
   DialogSetBG(id_choice, "interface\textbox_load.dds"); //set the background to nothing
  
   

   again:
 
  DialogChoiceClear(id_choice);

  string st_info = "Empty";
  int i_count = 1;

  again_create:
  st_info = GetSaveDescription("save\save$i_count$.dat");

  if (st_info == "Empty")
   {
     //can't really load this one
   // DialogChoiceAdd(id_choice, 0 , "$st_info$");
   }

  if (st_info != "Empty")
  {
    DialogChoiceAdd(id_choice,  i_count, "$st_info$");
  }

  i_count += 1;
  if (i_count <= i_max_saves)
  {
  goto again_create;
  }
  DialogChoiceAdd(id_choice, 11, "Cancel");

 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
 
  if (g_i_last_choice == 0)
   {
     goto again;
  }

  SetPauseLevel(0); //freeze lawn mower controls and other dialog boxes, only things created DURING
 

  EntKill(id_choice); 

 if (g_i_last_choice < 11)
 {

   LogMsg("Loading game..");
   LoadGame("save\save$g_i_last_choice$.dat");  //kill the choice menu

  //load the cheats up real quick, just in case...
   int i_cheat = CreateEntScript("CHEAT", "script\cheat.c");
   EntRunFunction(i_cheat, "SetupCheat");
   EntKill(i_cheat);

  //kill us and load the room script back up
  CreateEntScript("RoomMenu", "script\room_menu.c"); 
  EntKill(i_my_id); //kill this script
  return;
 }

  //if we got here they chose abort.  Go back to either main menu or the room menu.

 if (g_i_mode == 1) //we were in our room
  {
  CreateEntScript("RoomMenu", "script\room_menu.c"); 
  EntKill(id_choice); 
  EntKill(i_my_id); //kill this script
  return;
  }
 
 if (g_i_mode == 0) //main menu
 {

 //if on the main menu let the joystick control the mouse again
      SetMouseJoystickControl(true); 
    EntKill(i_my_id); //kill this script
   

 return;
  }

}

void OnKill()
{
	
}
