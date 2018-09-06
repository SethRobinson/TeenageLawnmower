void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
int i_demo;
 int i_clear = -1;
 int id_choice = -1;
 int id_dialog = -1;
 int id_con = -1;
 int i_tb -1;
 int id_rep_text;
 int i_temp;
 string st_message;
 int i_periods;
 int i_ring;
 string st_temp;
 string st_hour;
 string st_am;
 int i_time;
 int id_temp;
 int i_force_answer = 0;
}

void KillOurPics()
{
  EntKillByName("MONEYTEXT"); //not the fastest way, but easy
  EntKillByName("NOTETEXT"); //not the fastest way, but easy
  EntKillByName("DAYTEXT"); //not the fastest way, but easy
  EntKillByName("TIMETEXT"); //not the fastest way, but easy
  EntKillByName("INFOBG"); //not the fastest way, but easy
  EntKillByName("WEEDTEXT"); //not the fastest way, but easy
}

void UpdateRep()
{
 // g_f_rep_temp = ReputationGet();
//  EntSetFastText(id_rep_text, 1, "Reputation: $g_f_rep_temp$%"); //draw text wit "fast font" index 1, inits text mode
}

void UpdateInterface()
{
   
  //update time
 
  st_am = "am";
   i_time = g_i_time;

  if (i_time > 11)
  {
    //it's pm
    st_am = "pm";
   if (i_time > 12)
    {
       i_time -= 12;
    }
   }
  id_temp = EntGetByName("MONEYTEXT");
  EntSetFastText(id_temp, 1, "$$$g_i_money$.00"); //draw text wit "fast font" index 1, inits text mode
  EntSetPicTextColor(id_temp, 255,255,255,255); //the last part of the alpha
 

  id_temp =EntGetByName("TIMETEXT");
  EntSetFastText(id_temp, 1, "$i_time$:00 $st_am$"); //draw text wit "fast font" index 1, inits text mode
  EntSetPicTextColor(id_temp, 255,255,255,255); //the last part of the alpha
 
 
 
  id_temp = EntGetByName("DAYTEXT");
  EntSetFastText(id_temp, 1, "$g_i_day$"); //draw text wit "fast font" index 1, inits text mode
  EntSetPicTextColor(id_temp, 255,255,255,255); //the last part of the alpha

  id_temp = EntGetByName("WEEDTEXT");
  EntSetFastText(id_temp, 1, "$g_i_weed_killer$"); //draw text wit "fast font" index 1, inits text mode
  EntSetPicTextColor(id_temp, 255,255,255,255); //the last part of the alpha


 //update note

 int i_day_temp;
i_day_temp = g_i_goal_day;

 i_day_temp -= 1;  
  
 st_temp = "Need $$$g_i_goal_money$ by the end of day $i_day_temp$ for $g_st_goal$";

//well, this doesn't get interpreted until it's set to teh string, so let's use these goto's
//to not set i_day_temp to the wrong day


 if (i_day_temp == g_i_day)
  {
    st_temp = "Need $$$g_i_goal_money$ by the end of the day for $g_st_goal$";
   goto skip;
  }

  i_day_temp -= 1; 
 if (i_day_temp == g_i_day)
  {
    st_temp = "Need $$$g_i_goal_money$ by the end of tomorrow for $g_st_goal$";
   goto skip;
  }

//fix temp to go back

i_day_temp += 1; //it was late when I wrote this logic, trust me it works


skip:

 if (g_i_goal == 0)
   {
      st_temp = "Nothing special happening.";
   }



 //st_temp = StringExpand(st_temp);
  id_temp = EntGetByName("NOTETEXT");
  EntSetFastText(id_temp, 1, st_temp); //draw text wit "fast font" index 1, inits text mode
  EntSetPicTextColor(id_temp, 255,255,255,255); //the last part of the alpha
 
 i_day_temp = g_i_goal_day;
 i_day_temp -= 1;  
  
 if (i_day_temp == g_i_day)
  {
    if (g_i_money < g_i_goal_money)
     {
       //they need money, fast!
         EntSetPicTextColor(id_temp, 255,10,10,255); //the last part of the alpha
 
      }
  }

}


void Main()
{
 
   i_clear = CreateEntScript("STUFF", "script\misc_functions.c");
   EntRunFunction(i_clear, "SetupForMenus");
   EntKill(i_clear);

  EntSetName(i_my_id, "RoomMenu"); //make sure we're called the right thing
  
  string st_myname = EntGetName(i_my_id);
  LogMsg("Currently called $st_myname$.");
  if (g_i_force_day != 0)
   {
     //activate one time force day cheat
     g_i_day = g_i_force_day;
     g_i_force_day = 0;
   }

  if (g_i_force_time != 0)
   {
     //activate one time force time cheat
    g_i_time = g_i_force_time;
    g_i_force_time = 0;

   }

 if (g_i_force_money != 0)
   {
     //activate one time force time cheat
    g_i_money = g_i_force_money;
    g_i_force_money = 0;

   }

  g_i_mode = 1; //0 means main menu, not playing.  1 is room, 2 is 3d mode.
  g_mouse_active = 1;

  SoundClearCache();
  //play music
     SoundMusicPlay("sound\room_music.wav", 1000);
 
 EntKillAllPics(); //get rid of all backgrounds and buttons on the screen

  //setup background
   int i_bg = EntGetByName("background"); //if we already have a background, let's not create it

  if (i_bg == -1)
  {
    //create it!
    i_bg = CreateEntPic("background"); //we can find this by name later if we want
   }  

  if (g_i_time < 11)
  {
    EntSetSprite(i_bg, "interface\room.jpg");
  }

    if (g_i_time > 10)
  {
    if (g_i_time < 15)
    {
      EntSetSprite(i_bg, "interface\room_noon.jpg");
    }
  }

    if (g_i_time > 14)
  {
      EntSetSprite(i_bg, "interface\room_evening.jpg");
  }

    EntSetLayer(i_bg, -10); //show up behind everything for sure
  EntDefaultResAdjust(i_bg, 3); //simpler than using EntSetResAdjust




  logmsg("Mowed $g_i_total_lawns_mowed$ lawns so far.");

 float f_infobar_x = 35;
 float f_infobar_y = 510;
 float f_o_x;
 float f_o_y; //relative offsets for the actual items that go on top of the interface graphic
 
 //create and place interface items

 //add bg info bar
 int id_pic = CreateEntPic("INFOBG"); 
 EntSetResAdjust(id_pic, 2, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom
 EntSetXY(id_pic, f_infobar_x, f_infobar_y);
 EntSetSprite(id_pic, "interface\infobar.dds");

  //create money text
  f_o_x = f_infobar_x;
  f_o_y = f_infobar_y;
  f_o_x += 405;
  f_o_y += 21;  //this won't change for the next three things!!!!!!
  id_pic = CreateEntPic("MONEYTEXT");
  EntSetXY(id_pic, f_o_x, f_o_y);
  EntSetLayer(id_pic, 10); //make the text appear above the bar
 EntSetResAdjust(id_pic, 2, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom, 2 is normal

  //create time of day
  f_o_x = f_infobar_x;
  f_o_x += 237;
  id_pic = CreateEntPic("TIMETEXT");
  EntSetXY(id_pic, f_o_x, f_o_y);
  EntSetLayer(id_pic, 10); //make the text appear above the bar
 EntSetResAdjust(id_pic, 2, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom, 2 is normal

  //create date
  f_o_x = f_infobar_x;
  f_o_x += 70;
  id_pic= CreateEntPic("DAYTEXT");
  EntSetXY(id_pic, f_o_x, f_o_y);
  EntSetLayer(id_pic, 10); //make the text appear above the bar
 EntSetResAdjust(id_pic, 2, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom, 2 is normal


 //create date
  f_o_x = f_infobar_x;
  f_o_x += 614;
  id_pic= CreateEntPic("WEEDTEXT");
  EntSetXY(id_pic, f_o_x, f_o_y);
  EntSetLayer(id_pic, 10); //make the text appear above the bar
 EntSetResAdjust(id_pic, 2, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom, 2 is normal



  //Create note text
  f_o_x = f_infobar_x;
  f_o_y = f_infobar_y;
  f_o_x += 73;
  f_o_y += 47;  
  id_pic = CreateEntPic("NOTETEXT");
  EntSetXY(id_pic, f_o_x, f_o_y);
  EntSetLayer(id_pic, 10); //make the text appear above the bar
 EntSetResAdjust(id_pic, 2, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom, 2 is normal


 
  room_options:

  UpdateInterface();

  if (g_i_morning_event_active == 1)
   {
     i_temp = FileExists("script\event\day_$g_i_day$_morning.c");
    if (i_temp == 1)
     {
	g_i_morning_event_active = 0; //don't call this again
	//go ahead and run this morning script.  It will call us when it's done.  It will also handle erasing the back ground if it wants to do
              //it's own. 
              CreateEntScript("EVENT", "script\event\day_$g_i_day$_morning.c");	      
	       EntKill(i_my_id);

              return;
     }

   }
if (g_i_goal == 0)
  {
   //should we set a new goal?
  CreateScriptFunction("script\goals.c", "Setup");  //setup their goal

  if (g_i_goal == 1)
   {
    //we got a new goal.  Let's let them know that.

       i_tb = CreateDialog("");
       DialogSetStyleMow(i_tb);
       g_st_goal_start = StringExpand(g_st_goal_start);      
  SoundMusicPlay("sound\pay_time.wav", 1);
   DialogSet(i_tb, g_st_goal_start);
      
       DialogWaitForOk(i_tb);
  SoundMusicPlay("sound\room_music.wav", 1000);
      
 EntKill(i_tb);    
      i_tb = -1;
   UpdateInterface();

 }        

}
 st_am = "am";
   i_time = g_i_time;

  if (i_time > 11)
  {
    //it's pm
    st_am = "pm";
   if (i_time > 12)
    {
       i_time -= 12;

  
    if  (i_time > 5)
     {
        //it's 6 pm or later, no more mowing tonight.

     //run our night script if we have one

    if (g_i_night_event_active == 1)
   {
     i_temp = FileExists("script\event\day_$g_i_day$_night.c");
    if (i_temp == 1)
     {
	g_i_night_event_active = 0; //don't call this again
	//go ahead and run this morning script.  It will call us when it's done.  It will also handle erasing the back ground if it wants to do
              //it's own. 
              CreateEntScript("EVENT", "script\event\day_$g_i_day$_night.c");	      
   	    EntKill(i_my_id);
	return;
     }

   }
    
       //show a little message and go to sleep.
      i_temp = randomrange(1,5);
   
     if (i_temp == 1)
      {
        st_message = "You look at the clock, $i_time$ $st_am$.  Probably a good idea to turn in about now.";
      }

     if (i_temp == 2)
      {
       st_message = "It's $i_time$ $st_am$.  You doubt you'll get any more calls tonight so you call it a day.";
      }

     if (i_temp == 3)
      {
        st_message = "It is $i_time$ $st_am$.  You figure you better get some rest for an early start tommorow. ";
      }

     if (i_temp == 4)
      {
        st_message = "It's almost $i_time$ $st_am$, time to surf the net, play some games and then hit the sack.";
      }
  
     if (i_temp == 5)
      {
        st_message = "It's already $i_time$ $st_am$.  Your exhausted  muscles scream for rest.";
      }
     	 i_tb = CreateDialog("");
   	DialogSetStyleMow(i_tb);
   	DialogSet(i_tb, st_message);  
	DialogWaitForOk(i_tb);
              EntKill(i_tb);
             i_tb = -1;
       KillOurPics();
       EntKillAllPics(); //get rid of all backgrounds and buttons on the screen
       CreateEntScript("NEWDAY", "script\\new_day.c");
       EntKill(i_my_id);
       return;

     }

  }
}
 
 menu_start:
 if (g_i_skip_to_play == 1)
{
  goto cheat_skip_ahead;
} 
  id_con = CreateConversation(""); //no name for it, we don't need to keep track
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);



  DialogSet(id_dialog, "You are sitting in your room.");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Wait for phone call");
  DialogChoiceAdd(id_choice, 2, "Game Options");
  DialogChoiceWait(id_choice);


  if (g_i_last_choice == 2)
   {
  option_start:
  DialogSet(id_dialog, "Choose an option.");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 3, "Load Game");
  DialogChoiceAdd(id_choice, 4, "Save Game");
  DialogChoiceAdd(id_choice, 5, "Video && Setup Options");
  DialogChoiceAdd(id_choice, 6, "Quit to main menu");
  DialogChoiceAdd(id_choice, 1, "Return to room");
  DialogChoiceWait(id_choice);

 if (g_i_last_choice != 6)
  {
   //option five needs the menu still active
   EntKill(id_con); //kill all conversation elements at once
   id_con = -1;
  }

  if (g_i_last_choice == 4)
  {
     CreateEntScript("SaveGame", "script\save_game.c");  
     //kill the pics we created
     KillOurPics();
     EntKill(i_my_id); //kill this script
     return;
  }

if (g_i_last_choice == 5)
  {
      CreateEntScript("", "script\config.c");   //kill the pics we created
    // KillOurPics();
    // EntKill(i_my_id); //kill this script
    // goto option_start;
     KillOurPics();
     return;
  }


 if (g_i_last_choice == 6)
  {
    DialogSet(id_dialog, "Are you sure you wish to quit your game and return to the main menu?");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Yes");
  DialogChoiceAdd(id_choice, 2, "No");
  DialogChoiceWait(id_choice);
  if (g_i_last_choice == 2)
    {
      goto option_start;
    }
 
  //they want to quit to the main menu
     //quit game
    EntKill(id_con); //kill all conversation elements at once
 id_con = -1;
 
  //if room menu exists, kill it and whatnot
  //kill everything and load the main menu
  EntSetPauseLevel(i_my_id, 500);
   KillOurPics();

  EntKillByName("background");
  
  EntKillAll(); //kill all conversation and pics and scripts within pause level range
  EntSetPauseLevel(i_my_id, 0);

  CreateEntScript("Main", "script\main_menu.c");
//  PostQuitMessage();
  EntKill(i_my_id); //kill this script
  Return; 
 }

  if (g_i_last_choice == 3)
  {
    CreateEntScript("LoadGame", "script\load_game.c");  
     //kill the pics we created
     KillOurPics();
     EntKill(i_my_id); //kill this script
     return;
   }
  goto menu_start;
 }
  EntKill(id_con); //kill all conversation elements at once
 id_con = -1;
i_demo = GetDemoMode();
if (i_demo == 1)
{
if (g_i_day > 2)
   {

      //um, don't let them play anymore
 i_tb = CreateDialog("");
 DialogSetStyleMow(i_tb);
 // EntSetFadeUp(i_tb, 2.0); //fade speed.  

  DialogSet(i_tb, "Congratulations, you have completed the demo!");
  SoundPlay("sound\quicktip.wav");

  
//SetPauseLevel(200);

   DialogWaitForOk(i_tb);
 
   EntKill(i_tb);
   i_tb = -1;
   KillOurPics();

  CreateEntScript("NAG", "script\\nag.c");
   EntKill(i_my_id);
  return;
   }
}

  //let's show some dialog
  i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
  DialogSet(i_tb, "Waiting for phone call...");
  //   DialogWaitForOk(i_tb);
  
  i_periods = 4; //how long we should wait max
 
  
  i_ring = -1; //disable


 i_temp = randomrange(0, 10);

//hack to always get a customer, more fun this way
i_force_answer = 1;

if (i_force_answer == 1)
  {
    i_force_answer = 0;
   i_temp = 7; //force an answer
  }


 if (i_temp > 5)
  {
    
   //they will get a customer.  But when?
    i_ring = randomrange(1, i_periods); //pick a random spot to ring
 //   i_ring -= 3; //the lower the number, the more at the end the ring will occur

 
 }
  int i_tick = 1; 
  again:
  wait (800);
  DialogAdd(i_tb, ".");
 if (i_tick == 1)
   {
     SoundPlay("sound\tick.wav");
    i_tick = 0;
    goto tick_skip;
   }

 if (i_tick == 0)
   {
     SoundPlay("sound\tock.wav");
     i_tick = 1;
   }
 
 tick_skip:

  if (i_ring == i_periods) 
   {
     goto ring;
   }  
  i_periods -= 1;
  if (i_periods > 0)
   {
      goto again;
   }

     i_force_answer = 1;
   DialogAdd(i_tb, "an hour passes.");
    DialogWaitForOk(i_tb);   
    EntKill(i_tb);
   i_tb = -1;
    g_i_time += 1;
    goto room_options;
 
  ring:

  SoundPlay("sound\ring.wav");

  EntKill(i_tb);
  i_tb = -1;

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:

  DialogSet(id_dialog, "Phone ring!");
 

 ConversationSetPic(id_con, "interface\phone_bright.dds");
  wait(200);
  ConversationSetPic(id_con, "interface\phone_dark.dds");
  wait(200);
  ConversationSetPic(id_con, "interface\phone_bright.dds");
  wait(200);
  ConversationSetPic(id_con, "interface\phone_dark.dds");
  wait(200);
  ConversationSetPic(id_con, "interface\phone_bright.dds");
  wait(200);
  ConversationSetPic(id_con, "interface\phone_dark.dds");
  wait(200);
  ConversationSetPic(id_con, "interface\phone_bright.dds");
  wait(200);
  ConversationSetPic(id_con, "interface\phone_dark.dds");
  wait(200);
 
 cheat_skip_ahead:


  //clear vars

  g_i_accidents = 0;
  g_i_cleaned = 0;


//check the cheats file

   int i_cheat = CreateEntScript("CHEAT", "script\cheat.c");
   EntRunFunction(i_cheat, "SetupCheat");
   EntKill(i_cheat);



  //build the weather string

int i_rand_weather;



 if (g_i_time < 11)
  {
    i_rand_weather = 1; //morning
   goto weather_skip;
  }

 if (g_i_time < 17)
  {
    i_rand_weather = 2; //noonish
   goto weather_skip;
  }

//default

 if (g_i_time < 25)
  {
    i_rand_weather = 3; //noonish
   goto weather_skip;
  }

weather_skip:
 
string st_extra = "clear";

i_temp = randomrange(1, 6);

if (i_temp > 4)
  {
    i_temp = 1;
   //make it sunny half the time
  }

if (i_temp == 1)
  {
 st_extra = "clear";
  }

if (i_temp == 2)
  {
  st_extra = "fog";
  }
if (i_temp == 3)
  {
  st_extra = "storm";
   if (i_rand_weather == 1)
  {
     //let's not have mornings be stormy at all.
    st_extra = "clear";    
   }

  }
if (i_temp == 4)
  {
  st_extra = "rain";
  }
g_st_weather = "data\$i_rand_weather$_$st_extra$.wet";

//let's go ahead and expand it now while our local vars are valid
g_st_weather = StringExpand(g_st_weather);

if (g_st_force_weather != "")
  {
    g_st_weather = g_st_force_weather;

   }

  //get the real data
  //load the level setup script.  main() will be empty, let's run the function to get info


  int i_lev_temp = CreateEntScript("RANDOML", "script\level\get_level.c"); //choose our level based on our info..name of script really doesn't matter
  EntRunFunction(i_lev_temp, "SetupLevel");
  EntKill(i_lev_temp);


   
  g_i_quit = 0; //reset this, if 1 they can't use escape menu/etc
  int id_level = CreateEntScript("LevelScript", "script\level\level$g_i_level$.c");
  EntRunFunction(id_level, "SetClientInfo"); //will return 0 if it doesn't exist
  

  SoundPlay(g_st_client_sound);
  ConversationSetPic(id_con, g_st_client_pic);

  st_hour = "s";

  if (g_i_client_hours == 1)
   {
     st_hour = "";
   }
  //replace their text
  DialogSet(id_dialog, "\"$g_st_client_phone$\"\n\nThe job will pay $$$g_i_client_pay$.  You estimate it will take $g_i_client_hours$ hour$st_hour$.");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Accept");
  DialogChoiceAdd(id_choice, 2, "Decline");


  DialogChoiceWait(id_choice);



 if (g_i_last_choice == 2)
  {
    //they don't want the job.  Show reaction.
  ConversationSetPic(id_con, g_st_client_decline_pic);
  DialogChoiceClear(id_choice);
  DialogSet(id_dialog, "\"$g_st_client_decline$\"");
  float f_rep_mod = FRandomRange(-0.1, -0.01);
  g_i_time += 1;
  ReputationMod(f_rep_mod);
  UpdateRep();
  DialogChoiceAdd(id_choice, 1, "Hang up");
  DialogChoiceWait(id_choice);
  EntKill(id_con); //kill all conversation elements at once
  id_con = -1;  
  EntKill(id_level);
  id_level = -1;
  goto room_options;    
  }
 

  EntKill(id_con); //kill all conversation elements at once
  id_con = -1;  
//let's hide the mouse
  CreateEntScript("", "script\mouse_off.c");

  KillOurPics();

  //show message as we're loading
   i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
   DialogSet(i_tb, "Traveling to $g_st_client_name$'s $g_st_client_home$...");

   wait(1); //make sure this get's shown
   wait(1); //make sure this get's shown
   wait(1); //make sure this get's shown

 
   i_clear = CreateEntScript("STUFF", "script\misc_functions.c");
   EntRunFunction(i_clear, "SetupForAction");
   EntKill(i_clear);


  g_i_time += g_i_client_hours;
  //hey, let's also kill the background
  EntRunFunction(id_level, "StartLevel");
  EntKillByName("background");
  EntKill(i_tb);  //kill the loading message
  i_tb = -1;
  //kill this script
  EntKill(i_my_id); //kill this script
 
}

void OnKill()
{
   
  // if (i_tb != -1)
  // {
  //   EntKill(i_tb);
  //   i_tb = -1;
  //   }
   
    //KillOurPics();
}
