//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.


void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
}

void Main()
{
  logmsg("Turning mouse on.");
   int id_mouse = EntGetByName("MOUSE");

   if (id_mouse != -1)
   {
	//mouse already exists
        EntKill(i_my_id);
	return;
   }


 //mouse does not exist.  Let's create it

  id_mouse = CreateEntMouse("MOUSE"); //create a mouse entity
  EntSetSprite(id_mouse, "interface\mouse.tga");
  EntSetIgnoreKill(id_mouse, 1); //won't die with a kill Cpic command
  EntSetLayer(id_mouse, 200); //on top of everything
  //move it to the middle of the screen
  EntSetXY(id_mouse, 400.0, 400.0);  //good for moving 2d items

  //all done
  EntKill(i_my_id);
}

void OnKill()
{
	
}
