//as part of a key binding, you never want to EntKill this script.  It's always loaded and shared between key hits.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{

}

void Main()
{
  //get's run if pressed.  Note:  Do not use WAIT() or anything that causes a delay, this script is not monitored
  //and won't get called. 

  CreateEntScript("pee", "script\pstest.c");
}
