void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_choice;

}

void Main()
{
  //we're a button script, really we don't do much.
}

void OnClick()
{

int i_demo = GetDemoMode();
if (i_demo == 1)
{
CreateEntScript("NAG", "script\\nag.c");
return;
	
}
	//we got clicked, yay.
  PostQuitMessage();
 	
}
void OnKill()
{

}