void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{


}

void Main()
{
  //we're a button script, really we don't do much.
}

void OnClick()
{

  SetMouseJoystickControl(false);  
CreateEntScript("LoadMod", "script\\load_mod.c");
}

void OnKill()
{

}