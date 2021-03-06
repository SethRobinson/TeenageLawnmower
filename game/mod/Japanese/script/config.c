void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_choice;
 string st_temp;
 string st_vk_name;
 int id_dialog;
}

void Main()
{
 
 int i_checker =  EntGetByName("Config");

if (i_checker != -1)
   {
   //this window is already open in another script
   EntKill(i_my_id);

   return;   
  }


//now open a choice dialog, similar but different
 int i_last_choice = 0;

 SetPauseLevel(200); //freeze lawn mower controls and other dialog boxes, only things created DURING
 //this pause level will be active
CreateEntScript("", "script\mouse_on.c");

 again:
   id_choice = CreateDialogChoice("Config"); //we'll use this for a menu
   EntSetLayer(id_choice, 120); //show up above stuff
   DialogSetStyleMow(id_choice);
 

  DialogChoiceClear(id_choice);
  st_temp = VKeyToString("FORWARD");
  DialogChoiceAdd(id_choice, 1, "Set move forward ($st_temp$)");
  st_temp = VKeyToString("BACKWARD");
  DialogChoiceAdd(id_choice, 2, "Set move backward ($st_temp$)");

  st_temp = VKeyToString("LEFT");
  DialogChoiceAdd(id_choice, 3, "Set move left ($st_temp$)");

  st_temp = VKeyToString("RIGHT");
  DialogChoiceAdd(id_choice, 4, "Set move right ($st_temp$)");

  st_temp = VKeyToString("JUMP");
  DialogChoiceAdd(id_choice, 5, "Set jump ($st_temp$)");

  st_temp = VKeyToString("ACTION2");
  DialogChoiceAdd(id_choice, 6, "Set power turn ($st_temp$)");

  DialogChoiceAdd(id_choice, 50, "Return");

  //move to the last selection they chose
  DialogChoiceSetCurrent(id_choice, i_last_choice);

  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
   i_last_choice = g_i_last_choice;


if (g_i_last_choice == 50)
  {
   EntKill(id_choice); 
   EntKill(i_my_id); //kill this script
 if ( g_mouse_active == 0)
  {
    //the mouse we had one
    CreateEntScript("", "script\mouse_off.c");
  }
 SetPauseLevel(0); //unpause   
  return;  
}

 //they actually want to change a key, let's do it.

 if (g_i_last_choice == 1)
 {
   st_vk_name = "FORWARD";
 }

 if (g_i_last_choice == 2)
 {
   st_vk_name = "BACKWARD";
 }

 if (g_i_last_choice == 3)
 {
   st_vk_name = "LEFT";
 }

 if (g_i_last_choice == 4)
 {
   st_vk_name = "RIGHT";
 }
 if (g_i_last_choice == 5)
 {
   st_vk_name = "JUMP";
 }
 
 if (g_i_last_choice == 6)
 {
   st_vk_name = "ACTION2";
 }

  //kill the old dialog and show the "enter a key one" one
  EntKill(id_choice); 


   id_dialog = CreateDialog("Config"); //just to show something
   EntSetLayer(id_dialog, 120); //show up above stuff
   DialogSetStyleMow(id_dialog);
   DialogSet(id_dialog, "$st_vk_name$ のためのキーを選び、タイプしてください。");
   WaitForKey(0); //will wait until a key/key combo is hit and put it in g_st_last_string; 0 means all keys ok including ctrl/alt combos
  // LogMsg("You pressed $g_st_last_string$!");

  //got the key, let's dynamically change the binding to this.
  VKeySet(st_vk_name, g_st_last_string);

  //kill dialog.
  EntKill(id_dialog);

  goto again;
  

}

void OnKill()
{
	
}
