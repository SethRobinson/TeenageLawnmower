void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_dialog;
 int i_tb;
int i_temp;
}


void Main()
{
  //play music
  SoundMusicStop();
  SoundPlay("sound\bird1.wav");

  //create it!
int i_bg = CreateEntPic("background"); //we can find this by name later if we want
  EntSetSprite(i_bg, "interface\house.jpg");
   EntSetLayer(i_bg, -10); //show up behind everything for sure
  EntDefaultResAdjust(i_bg, 3); //simpler than using EntSetResAdjust

 //set parms for the next day
 
  g_i_time = randomrange(9, 9); //set it to an AM time
  g_i_day += 1; //advance the day counter
  g_i_morning_event_active = 1; //if there is an event today, make sure we do it
  g_i_night_event_active = 1; //if there is an event today, make sure we do it
  //let's show some dialog
  i_tb = CreateDialog("Crap");
 
  DialogSetStyleMow(i_tb);
  DialogSetBG(i_tb, ""); //set the background to nothing
  DialogSetFont(i_tb, 3); //force the big font 
  DialogSet(i_tb, "Day $g_i_day$");
  
  //center it so it looks ok at all resolutions
  int i_temp_x = GetScreenX();
  int i_temp_y = GetScreenY();
  
  i_temp_x /= 2;
  i_temp_y /= 2;
  i_temp_x -= 150;
   
  EntSetXY(i_tb, i_temp_x, i_temp_y);

  if (g_i_day > 9)
   {
       //center it a bit better
      i_temp_x -= 40;
      EntSetXY(i_tb, i_temp_x, i_temp_y);
    }
//  ResetFrameTimer(); //good to do after a possible long load
  EntSetFadeUp(i_tb, 0.0); //fade speed.  
  wait(20);
  EntSetFadeUp(i_tb, 1.0); //fade speed.  
  wait(2000);
  EntSetFadeDown(i_tb, 2.0); //fade speed.  
  Wait(500);
 
 
 EntKill(i_tb);

 if (g_i_goal > 0)
   {
    logmsg("goal day is $g_i_goal_day$ and today is $g_i_day$.");
     if (g_i_day >= g_i_goal_day)
       {
            i_tb = CreateDialog("");
            DialogSetStyleMow(i_tb);
           DialogSet(i_tb, "The $$$g_i_goal_money$ for $g_st_goal$ is due!\n\n");
 	SoundPlay("sound\pay_time.wav");
           DialogWaitForOk(i_tb);
         //out time is up, did we make enough money?
          if (g_i_money >= g_i_goal_money)
           {
              //we have enough money
	SoundPlay("sound\cash register.wav");
              g_st_goal_win_string = StringExpand(g_st_goal_win_string);
	DialogSet(i_tb, "$g_st_goal_win_string$");
              DialogWaitForOk(i_tb);
 	 EntKill(i_tb);
	g_i_goal = 0;             //reset the goal
             //don't forget to remove the cash
              g_i_money -= g_i_goal_money;

          }
      
        if (g_i_goal != 0)
          {
          if (g_i_money < g_i_goal_money)
             {
                  //we have enough money
	    g_st_goal_lose_string = StringExpand(g_st_goal_lose_string);
	    DialogSet(i_tb, "You seem to be a bit short on funds.\n\n$g_st_goal_lose_string$");
	SoundPlay("sound\loser.wav");
  	 DialogWaitForOk(i_tb);
	   //close everything and load the main menu
                EntKill(i_bg);
	 EntKill(i_tb);
   	  CreateEntScript("MainMenu", "script\main_menu.c"); 
	   EntKill(i_my_id); //kill this script
                 return;
             }
          }
       }

   }
 

  int i_crap = CreateEntScript("RoomMenu", "script\room_menu.c"); 
  wait(1);
 //don't need to kill bg I guess, roommenu handles it
 // EntKill(i_bg);
//  logMsg("Killing newday script");
  EntKill(i_my_id); //kill this script
 

}

void OnKill()
{
	
}
