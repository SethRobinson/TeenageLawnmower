void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_choice;
 int id_con;
 int id_dialog;

}

void Main()
{
  //we're a button script, really we don't do much.
}

void OnClick()
{

//launch website in their browser



//SetPauseLevel(100); //freeze lawn mower controls and other dialog boxes, only things created DURING



SetPauseLevel(0); //freeze lawn mower controls and other dialog boxes, only things created DURING
LaunchPurchaseSite();
//EntKillAll();

PostQuitMessage();


	
}
void OnKill()
{

}