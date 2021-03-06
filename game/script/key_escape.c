//as part of a key binding, you never want to EntKill this script.  It's always loaded and shared between key hits.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
    int i_temp;
}

void Main()
{
 
if (g_i_quit == 1)
  {
    //we're dead or doing something, don't let them hit escape, mofo
    return;
  }

//get's run if pressed.  Note:  Do not use WAIT() or anything that causes a delay, this script is not monitored
  //and won't get called. 
 
  int id_escape = EntGetByName("Escape");

  if (id_escape != -1)
  {
    //an escape window is already active, kill and the choice dialog if it exists
    EntKill(id_escape);
   
    id_escape = EntGetByName("EscapeChoice");
    if (id_escape != -1)
      {
   	EntKill(id_escape);
      }
     
     SetPauseLevel(0); //unpause
 i_temp = EntGetByName("PLAYER");

  if (i_temp != -1)
  {
  	//let's assume they are mowing and want to kill the mouse
  	 CreateEntScript("", "script\mouse_off.c");
 	
  }
   return;
  }
 

  i_temp = EntGetByName("PLAYER");

  if (i_temp != -1)
  {
    //a lawn mower exists somewhere, so let's assume they are in the middle of cutting grass and show a different menu.
    CreateEntScript("escape", "script\escape_menu_mow.c");
    return;  
  }
 
   i_temp = EntGetByName("RoomMenu");

  if (i_temp == -1)
  {
    //no room menu?  can't use escape then
    return;  
  }
 

  CreateEntScript("Escape", "script\escape_menu.c");
}
