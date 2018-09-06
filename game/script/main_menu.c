void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 
}

void Main()
{
 
  int i_cheat = CreateEntScript("STUFF", "script\misc_functions.c");
   EntRunFunction(i_cheat, "SetupForMenus");
   EntKill(i_cheat);

 
  i_cheat = CreateEntScript("CHEAT", "script\cheat.c");
   EntRunFunction(i_cheat, "SetupCheat");
   EntKill(i_cheat);



if (g_i_skip_to_play == 1)
{
  //thing I use to test with
   LoadGame("save\save4.dat");  //load this saved game
  //load the cheats up real quick, just in case...
  CreateEntScript("", "script\mouse_on.c");
 
  //kill us and load the room script back up
  CreateEntScript("RoomMenu", "script\room_menu.c"); 
  EntKill(i_my_id); //kill this script
  return;	
}


SetMouseJoystickControl(true); 
EntSetName(i_my_id, "MainMenu");  
g_i_mode = 0; //0 means main menu, not playing.  1 is room, 2 is 3d mode.
  g_mouse_active = 1;
 //play music
  SoundMusicPlay("sound\main_menu.ogg", 1);
  //skip everything and run the game for testing

  //load the level setup script
 // CreateEntScript("load_level", "script\room_menu.c");
 //goto skip;

   //create background
  int i_bg =  CreateEntPic("background"); //we can find this by name later if we want
  
  EntSetSprite(i_bg, "interface\mow_master.jpg");
  EntSetLayer(i_bg, -10); //show up behind everything for sure
  CreateEntScript("mousescript", "script\mouse_on.c"); //creates the mouse if it isn't already going
  EntDefaultResAdjust(i_bg, 3); //simpler than using EntSetResAdjust
   int i_temp;

  //******** NEW
  i_temp = CreateEntButton("button", "", "interface\main_new_02.jpg", "script\main_but_new.c");
  EntSetButtonHighlight(i_temp, "interface\main_new_02.jpg");
  EntSetButtonHighlightSound(i_temp, "sound\highlight.wav");
  EntSetXY(i_temp, 539.0, 244.0);
  EntDefaultResAdjust(i_temp, 3); //simpler than using EntSetResAdjust

  //******** LOAD
  i_temp = CreateEntButton("button", "", "interface\main_load_02.jpg", "script\main_but_load.c");
  EntSetButtonHighlight(i_temp, "interface\main_load_02.jpg");
  EntSetXY(i_temp, 560.0, 290.0);
  EntSetButtonHighlightSound(i_temp, "sound\highlight.wav");
  EntDefaultResAdjust(i_temp, 3); //simpler than using EntSetResAdjust

  //******** OPTIONS
  i_temp = CreateEntButton("button", "", "interface\main_options_02.jpg", "script\main_but_options.c");
  EntSetButtonHighlight(i_temp, "interface\main_options_02.jpg");
  EntSetXY(i_temp, 578.0, 339.0);
   EntSetButtonHighlightSound(i_temp, "sound\highlight.wav");
  EntDefaultResAdjust(i_temp, 3); //simpler than using EntSetResAdjust

  //******** WEBSITE
  i_temp = CreateEntButton("button", "", "interface\main_website_02.jpg", "script\main_but_website.c");
  EntSetButtonHighlight(i_temp, "interface\main_website_02.jpg");
  EntSetXY(i_temp, 594.0, 388.0);
  EntSetButtonHighlightSound(i_temp, "sound\highlight.wav");
  EntDefaultResAdjust(i_temp, 3); //simpler than using EntSetResAdjust

  //******** QUIT
  i_temp = CreateEntButton("button", "", "interface\main_quit_02.jpg", "script\main_but_quit.c");
  EntSetButtonHighlight(i_temp, "interface\main_quit_02.jpg");
  EntSetXY(i_temp, 621.0, 434.0);
  EntSetButtonHighlightSound(i_temp, "sound\highlight.wav");
  EntDefaultResAdjust(i_temp, 3); //simpler than using EntSetResAdjust

 //******** WEBSITE
  i_temp = CreateEntButton("button", "interface\main_rtsoft_01.jpg", "interface\main_rtsoft_01.jpg", "script\main_but_website.c");
  // EntSetButtonHighlight(i_temp, "interface\main_website_02.jpg");
  EntSetXY(i_temp, 20, 492);
  EntSetButtonHighlightSound(i_temp, "sound\highlight.wav");
  EntDefaultResAdjust(i_temp, 3); //simpler than using EntSetResAdjust
    EntSetColor(i_temp, 255,255,255,0);
  
 //mod support disabled, the engine was upgraded too much to support the old system...
 
 // i_temp = CreateEntButton("button", "interface\main_user_01.jpg", "interface\main_user_02.jpg", "script\main_but_mod.c");
 // EntSetButtonHighlight(i_temp, "interface\main_user_02.jpg");
 // EntSetXY(i_temp, 480.0, 514.0);
 // EntSetButtonHighlightSound(i_temp, "sound\highlight.wav");
 // EntDefaultResAdjust(i_temp, 3); //simpler than using EntSetResAdjust
 
  //Set default lighting by loading a weather profile and killing it (lighting will remain)
  int id_weather = CreateEntWeather("Weather");
  EntWeatherLoad(id_weather, "data\2_clear.wet");
  EntKill(id_weather);

  ////setup the GUI for the weed killer
 // int id_pic = CreateEntPic("WEEDK"); 
  //EntSetXY(id_pic, 500.0,500.0);
 // EntSetModel(id_pic, "model\weedkiller.x");
 // EntSetLayer(id_pic, 5); //show up in front
 // EntSetUniformScale(id_pic, 0.05);
 
  //kill us

  ResetFrameTimer();
  EntPicBrandVersion(i_bg); //dynamically add the version # to this jpg
  float f_version = GetVersionFloat();
  logmsg("This is version $f_version$.");

  ResetFrameTimer();
  wait(1);
  ResetFrameTimer();
  wait(50);
   EntSetColorFade(i_temp, 255,255,255,255,70);

  skip:

EntKill(i_my_id); 
return; 

}



void OnKill()
{
	
}
