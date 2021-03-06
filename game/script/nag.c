void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_choice = -1;
 int id_dialog = -1;
  int id_con = -1;
 int i_tb -1;
 

}


void Main()
{

  g_i_mode = 1; //0 means main menu, not playing.  1 is room, 2 is 3d mode.
  g_mouse_active = 1;
SoundMusicPlay("demo_stuff\\nag.ogg", 1000);
 
 EntKillAllPics(); //get rid of all backgrounds and buttons on the screen

  int id_weather = EntGetByName("Weather");
  if (id_weather != -1)
  {
 int i_temp;
  LogMsg("Killing all stuffs");
  EntKillAllPics(); //get rid of all backgrounds and buttons on the screen
  EntKill(g_id_player);
  //Clear Level
  ZoneLoad(""); //blanks out the level
  EntSetPauseLevel(i_my_id, 600); //by doing this, we won't get deleted with a KillAllEnts command, which is good because this
  //script needs to continue
  LogMsg("Clearing level..");
  EntKillAllItems();
 
 //disable progress timer
 int id_progress = EntGetByName("PROGRESS");
 float f_percent_mowed = EntMeterGet(id_progress);
//disable progress trigger, we don't want it to happen now that we're finishing the level.
  EntPicSetTrigger(id_progress, -1, 1.0, -1, "l"); //the first parm is trigger type, 0 is SAME OR HIGHER. 

  if (f_percent_mowed >= 1)
  {
         SoundPlay("sound\level_finished.wav");
   }

  g_i_quit = 1; //disable the escape menu
 
 //time is up, calculate finish.
  EntMowerSetPower(g_id_player, 0); //turn mower off
 
 
 SetPauseLevel(599);
 
  }
  if (id_weather == -1)
  {
   id_weather = CreateEntWeather("Weather");
 	
  }
 
  EntWeatherLoad(id_weather, "data\2_clear.wet");
  
  //setup background
   int i_bg = EntGetByName("background"); //if we already have a background, let's not create it

  if (i_bg == -1)
  {
    //create it!
    i_bg = CreateEntPic("background"); //we can find this by name later if we want
   }  
   EntDefaultResAdjust(i_bg, 3); //simpler than using EntSetResAdjust
  EntSetSprite(i_bg, "demo_stuff\\nag.jpg");


   //create the buttoms

 //******** NEW
int i_temp;  

  i_temp = CreateEntButton("button", "", "demo_stuff\\click_buy_02.jpg", "script\\nag_but_buy.c");
  EntSetXY(i_temp, 136.0, 491.0);
  EntDefaultResAdjust(i_temp, 3); //simpler than using EntSetResAdjust
  EntSetButtonHighlightSound(i_temp, "sound\highlight.wav");
  EntSetButtonHighlight(i_temp, "demo_stuff\\click_buy_02.jpg");

  i_temp = CreateEntButton("button", "", "demo_stuff\\click_quit_02.jpg", "script\\nag_but_quit.c");
  EntSetXY(i_temp, 452.0, 491.0);
  EntDefaultResAdjust(i_temp, 3); //simpler than using EntSetResAdjust
  EntSetButtonHighlightSound(i_temp, "sound\highlight.wav");
 EntSetButtonHighlight(i_temp, "demo_stuff\\click_quit_02.jpg");
 ResetFrameTimer();
 //add rotating mower for fun
  int id_mower = CreateEntPic("MOW"); 
  EntSetModel(id_mower, "model\lawnmower2.X");
  EntSetLayer(id_mower, 50); //show up in front 
  EntSetUniformScale(id_mower, 0.073);
  EntSetPicVisualEffect(id_mower, C_PIC_VISUAL_EFFECT_BOB);
  EntSetXY(id_mower, 100,460);
  EntSetResAdjust(id_mower, 4, 800, 600); //if screen is resized, this scales with it.  1 means middle_bottom

wait(1);
 ResetFrameTimer();
 
    //EntSetCentering(id_mower, C_CENTERING_BOTTOM_MIDDLE); 

//EntKill (i_my_id);

}

void OnKill()
{
	EntKill(id_weather);

}
