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
	//we got clicked, yay.
  
   CreateEntScript("start", "script\\new_game.c"); //room menu will kill all buttons when it's initted
	
}
void OnKill()
{

}