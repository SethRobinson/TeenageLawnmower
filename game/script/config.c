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
EntSetName(i_my_id, "Config");

//now open a choice dialog, similar but different
 int i_last_choice = 0;

 SetPauseLevel(200); //freeze lawn mower controls and other dialog boxes, only things created DURING
 //this pause level will be active
CreateEntScript("", "script\mouse_on.c");

 again:
   id_choice = CreateDialogChoice("ConfigC"); //we'll use this for a menu
   EntSetLayer(id_choice, 120); //show up above stuff
   DialogSetStyleMow(id_choice);

  st_temp = "OFF";
  int i_vsync = GetVSync();
   if (i_vsync == 1)
   {
    st_temp = "ON";
   }
  DialogChoiceAdd(id_choice, 1, "VSync is currently $st_temp$");
  st_temp = "ENABLED";
  int i_joy_disabled = GetDisableJoystick();
   if (i_joy_disabled == 1)
   {
    st_temp = "DISABLED";
   }

  DialogChoiceAdd(id_choice, 2, "Gamepad support is currently $st_temp$");
  DialogChoiceAdd(id_choice, 3, "Configure Keyboard");
  DialogChoiceAdd(id_choice, 4, "Toggle Windowed Mode");
  DialogChoiceAdd(id_choice, 5, "Configure Video");

  DialogChoiceAdd(id_choice, 50, "Return");
  //move to the last selection they chose
  DialogChoiceSetCurrent(id_choice, i_last_choice);

  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
   i_last_choice = g_i_last_choice;
  EntKill(id_choice); 
if (g_i_last_choice == 1)
 {
   if (i_vsync == 1)
    {
      i_vsync = 0;
    goto vsync_pass;
    }

   if (i_vsync == 0)
    {
      i_vsync = 1;
    }

   vsync_pass:

   SetVSync(i_vsync);
   //rebuild display device
   Reset3DDevice();
   goto again; 
 }

if (g_i_last_choice == 2)
 {
 
  if (i_joy_disabled == 1)
    {
      i_joy_disabled = 0;
    goto joy_pass;
    }

   if (i_joy_disabled == 0)
    {
      i_joy_disabled = 1;
    }

   joy_pass:

   SetDisableJoystick(i_joy_disabled);
 goto again; 
 }
 


if (g_i_last_choice == 50)
  {
 
   EntKill(i_my_id); //kill this script
 if ( g_mouse_active == 0)
  {
    //the mouse we had one
    CreateEntScript("", "script\mouse_off.c");
  }
  SetPauseLevel(0); //unpause   
 
  //if on the main menu let the joystick control the mouse again
  
 int i_temp = EntGetByName("PLAYER");
 int  i_temp2 = EntGetByName("RoomMenu");

  if (i_temp == -1)
    if (i_temp2 == -1)
    {
     //rather nasty way to check if this guy is on the title screen right now
    SetMouseJoystickControl(true); 
    return;
    }

     if (i_temp2 != -1)
    {
    //on the room menu.. let's start it off again.	
     EntRunFunction(i_temp2, "Main");
     return;
   }
 
 //must be mowing, go back to the main esc menu
  CreateEntScript("Escape", "script\escape_menu_mow.c");
 
  return;  
}

 //they actually want to change something, let's do it.

 if (g_i_last_choice == 3)
 {
    EntKill(i_my_id); //kill this script
    CreateEntScript("", "script\config_keys.c");
    return;
 }
if (g_i_last_choice == 4)
 {
    ToggleWindowedMode();
 }


 if (g_i_last_choice == 5)
 {
   OpenConfigureVideo();
 }

  //kill dialog.
  EntKill(id_dialog);
  goto again;
  
}

void OnKill()
{
	
}
