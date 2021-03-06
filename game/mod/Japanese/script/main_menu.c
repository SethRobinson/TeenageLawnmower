void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 
}



void Main()
{
EntSetName(i_my_id, "MainMenu");  
g_i_mode = 0; //0 means main menu, not playing.  1 is room, 2 is 3d mode.
  g_mouse_active = 1;
 //play music
  SoundMusicPlay("sound\main_menu.wav", 1);
  //skip everything and run the game for testing

  //load the level setup script
 // CreateEntScript("load_level", "script\level1.c");
 // goto skip;

   //create background
  int i_bg =  CreateEntPic("background"); //we can find this by name later if we want
  EntSetSprite(i_bg, "interface\mow_master.jpg");
  EntSetLayer(i_bg, -10); //show up behind everything for sure
   CreateEntScript("mousescript", "script\mouse_on.c"); //creates the mouse if it isn't already going

  int i_temp;

  //******** NEW
  i_temp = CreateEntButton("button", "", "interface\main_new_02.jpg", "script\main_but_new.c");
  EntSetXY(i_temp, 539.0, 244.0);

  //******** LOAD
  i_temp = CreateEntButton("button", "", "interface\main_load_02.jpg", "script\main_but_load.c");
  EntSetXY(i_temp, 560.0, 290.0);

  //******** OPTIONS
  i_temp = CreateEntButton("button", "", "interface\main_options_02.jpg", "script\main_but_options.c");
  EntSetXY(i_temp, 578.0, 339.0);

  //******** WEBSITE
  i_temp = CreateEntButton("button", "", "interface\main_website_02.jpg", "script\main_but_website.c");
  EntSetXY(i_temp, 594.0, 388.0);

  //******** QUIT
  i_temp = CreateEntButton("button", "", "interface\main_quit_02.jpg", "script\main_but_quit.c");
  EntSetXY(i_temp, 621.0, 434.0);
 
  //i_temp = CreateEntButton("button", "interface\main_user_01.jpg", "interface\main_user_02.jpg", "script\main_but_mod.c");
  //EntSetXY(i_temp, 480.0, 514.0);


 skip:
  //kill us

  ResetFrameTimer();
   EntPicBrandVersion(i_bg); //dynamically add the version # to this jpg
  float f_version = GetVersionFloat();
  logmsg("This is version $f_version$.");
  ResetFrameTimer();

  
if (f_version < 1.06)
{  

EntSetPauseLevel(i_my_id, 200); //give this script mega access
SetPauseLevel(200);
 CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
//ResetFrameTimer();
int i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
EntSetFadeUp(i_tb, 2.0); //fade speed.  
  DialogSet(i_tb, "Old TLM version detected:  You should upgrade to insure trouble free gaming.");
 SoundPlay("sound\quicktip.wav");

SetPauseLevel(200);

   DialogWaitForOk(i_tb);
   EntKill(i_tb);
EntSetPauseLevel(i_my_id, 0); //remove previously mentioned mega access
SetPauseLevel(0);
}

EntKill(i_my_id); 
return; 

}



void OnKill()
{
	
}
