//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
  //if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
}

void Main()
{
} 


void SetupForMenus()
{
  //we only need this if we have screens that don't cover everything which we don't now
  //clear each frame, clear z-buffer, clear color rgb
  //SetRenderOptions(true,true,0,0,0);	
}


void SetupForAction()
{
  //clear each frame, clear z-buffer, clear color rgb
  //SetRenderOptions(false,true,0,0,0);	

}

void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.
}