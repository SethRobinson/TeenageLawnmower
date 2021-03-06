void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
}

void Main()
{


if (g_i_difficulty > 0)
  {
    //they are doing a hard mode, so they won't need tips.
// EntKill(i_my_id);  // return;
  //but what if they are just cocky and still need the tips?
  }

if (g_i_quick_tip > 2)
  {
     //already showed all quick tips, don't show anymore
    EntKill(i_my_id); 
     return;
 }


wait(1000);
  //should we show a quick tip?
logmsg("Showing quick tip..");



//pause the whole game
EntSetPauseLevel(i_my_id, 200); //give this script mega access
SetPauseLevel(200);
 CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
//ResetFrameTimer();
int i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
EntSetFadeUp(i_tb, 2.0); //fade speed.  

if (g_i_quick_tip == 0)
  {
 string st_temp = VKeyToString("ACTION2");
   DialogSet(i_tb, "Quick tip:  IMPORTANT:  ** HOLD DOWN ** the V key WHILE turning to Power Turn (turn sharply).  The game is impossible unless you learn this technique!\n\nUse the arrow keys to move and space to jump.  Don't forget you can mow backwards too.");
  }


if (g_i_quick_tip == 1)
  {
  DialogSet(i_tb, "Quick tip:  Did you know you can tap TAB to cycle through different camera views?\n\nPressing ESCAPE while mowing will bring up a menu that let's you abort and go home if you need to.");
 }
if (g_i_quick_tip == 2)
  {
  DialogSet(i_tb, "Quick tip:  You can use L to lock the camera in place. (press again to unlock)\n\nUse Alt-Enter to toggle windowed/fullscreen mode.");
 }

   SoundPlay("sound\quicktip.wav");

SetPauseLevel(200);

   DialogWaitForOk(i_tb);
   EntKill(i_tb);
EntSetPauseLevel(i_my_id, 0); //remove previously mentioned mega access
 CreateEntScript("", "script\mouse_off.c"); //turn the mouse off

SetPauseLevel(0);

//increment quick tip
g_i_quick_tip += 1;

   EntKill(i_my_id);

}

void OnKill()
{

}
