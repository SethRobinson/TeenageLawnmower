//utility script to do simple things

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{

}

void Main()
{
 
}

void UseWeedKiller()
{
 
 if (g_i_weed_killer < 1)
 {
  
      ShowMessage("Don't have weed killer!");
      EntKill(i_my_id);
      return; 
 }

//remove one of their weed killers
  int i_weed = CreateEntScript("WEEDY", "script\weed_function.c");
   EntRunFunction(i_weed, "RemoveWeedKiller");
 
 EntKill(i_my_id);
}

void OnKill()
{
	
}
