void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_choice;
 int i_max_saves = 6;
}

void Main()
{
 

//now open a choice dialog, similar but different

 //this pause level will be active
SetPauseLevel(50); //freeze lawn mower controls and other dialog boxes, only things created DURING

  id_choice = CreateDialogChoice("LoadChoice"); //we'll use this for a menu
   EntSetLayer(id_choice, 150); //show up above stuff
   DialogSetStyleLoad(id_choice);
   DialogSetBG(id_choice, "interface\textbox_save.dds"); //set the background to nothing
  
   again:
 
  DialogChoiceClear(id_choice);
  string st_info = "Empty";
  int i_count = 1;

  again_create:
  st_info = GetSaveDescription("..\\..\\save\save$i_count$.dat");
  DialogChoiceAdd(id_choice,  i_count, "$st_info$");
  i_count += 1;
  if (i_count <= i_max_saves)
  {
  goto again_create;
  }

  DialogChoiceAdd(id_choice, 11, "Cancel");

 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
SetPauseLevel(0); //freeze lawn mower controls and other dialog boxes, only things created DURING
 


 if (g_i_last_choice < 11)
 {
  

 string st_diff;
 if (g_i_difficulty == 0)
  {
    st_diff = "Easy";
  }

 if (g_i_difficulty == 1)
  {
    st_diff = "Normal";
  }
 if (g_i_difficulty == 2)
  {
    st_diff = "Master";
  }

LogMsg("Saving game..");
  SaveGame("$g_st_name$ - $$$g_i_money$ on day $g_i_day$ - $st_diff$", "..\\..\\save\save$g_i_last_choice$.dat");
  //kill the choice menu
 }

  //either way, kill us and load the room script back up
  CreateEntScript("RoomMenu", "script\room_menu.c"); 
  EntKill(id_choice); 
  EntKill(i_my_id); //kill this script

}

void OnKill()
{
	
}
