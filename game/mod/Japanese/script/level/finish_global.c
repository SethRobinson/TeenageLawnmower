void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
}

void Main()
{
int i_temp;
}

void OnKill()
{
}
void NoTime()
{
 g_st_stop_message = "時間切れ！";
 
}


void Finish()
{
 //let's stop the timer
  i_temp = EntGetByName("TIME");
 if (i_temp != -1)
   {
      EntSetPauseLevel(i_temp, -10); //disable timer
   }

 //disable progress timer
 int id_progress = EntGetByName("PROGRESS");
 float f_percent_mowed = EntMeterGet(id_progress);
//disable progress trigger, we don't want it to happen now that we're finishing the level.
  EntPicSetTrigger(id_progress, -1, 1.0, -1, "l"); //the first parm is trigger type, 0 is SAME OR HIGHER. 



  g_i_quit = 1; //disable the escape menu
 
 //time is up, calculate finish.
  EntMowerSetPower(g_id_player, 0); //turn mower off
 
  if (g_st_stop_message != "")
   {

     ShowMessage(g_st_stop_message);
    wait(2800);
   }


   if (g_st_stop_message == "")
   {
      wait(2000);
   }

  EntSetPauseLevel(g_id_player, -1); //can't control mower any more

  //load the final finish of the level script
      int i_levelscript = EntGetByName("LEVELSCRIPT");
       EntRunFunction(i_levelscript, "FinalExit");  
  


  EntKill(i_my_id);
}