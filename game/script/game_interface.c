void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_pic;
 int i=0;
 
}


void Main()
{
  
 float f_progress_x = 78.0;
 float f_progress_y = 595.0;

 float f_time_x = 745.0;
 float f_time_y = 595.0;

 //preload sounds we know we'll need
 SoundPreload("sound\powerup.ogg");
 SoundPreload("sound\weedkiller.ogg");
 SoundPreload("sound\weed_pickup.wav");
 Sound3DPreload("sound\tree_dead.wav");
 Sound3DPreload("sound\jump_land.wav");
 Sound3DPreload("sound\explode.wav");
 Sound3DPreload("sound\fire.wav");

//add the progress background
 id_pic = CreateEntPic("PROGRESSBG"); 
 EntSetXY(id_pic, f_progress_x, f_progress_y);
 EntSetResAdjust(id_pic, C_RES_ADJUST_FULL_NO_SCALE, 800, 600); //if screen is resized, this scales with it.  1 means middle_bottom
 EntSetSprite(id_pic, "interface\meter_progress.dds");
 EntSetCentering(id_pic, C_CENTERING_BOTTOM_MIDDLE); 

 //add progress percent text
 int id_progress = CreateEntPicMeter("PROGRESS", 1, 0.0); //the one means left based meter, third parm is what to start at
 EntSetLayer(id_progress, 5); //make the text appear above the bar
 EntSetPicTextColor(id_progress, 255,255,255,140); //the last part of the alpha
 //EntSetXY(id_progress, f_progress_x, f_progress_y);
 //EntSetResAdjust(id_progress, 3, 800, 600); //if screen is resized, this scales with it.  1 means middle_bottom
  EntSetFollow(id_progress, id_pic);
  EntSetFollowOffset(id_progress, -50.0, -40.0, 0.0); 
 int i_levelscript = EntGetByName("LEVELSCRIPT");
 //it will call FinishLevel() automatically when we hit 100 percent
 EntSetFastText(id_progress, 2, ""); //doing this to a picmeter causes it to draw it's results in a text form.  the num is font.
EntPicSetTrigger(id_progress, 0, 1.0, i_levelscript, "FinishLevel"); //the first parm is trigger type, 0 is SAME OR HIGHER. 


//also create damage indicator

 //add percent mowed indicator
 id_progress = CreateEntPicMeter("DAMAGE", 1, 0.0); //the one means left based meter, third parm is what to start at
 EntSetLayer(id_progress, 5); //make the text appear above the bar
 EntSetColor(id_progress, 255,255,255,140); //the last part of the alpha
 EntSetXY(id_progress, 400.0, 590.0);
 EntSetResAdjust(id_progress, C_RES_ADJUST_FULL_NO_SCALE, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom
 EntSetSprite(id_progress, "interface\meter_condition.dds");
 EntMeterSet(id_progress, 1.0f);
 EntSetCentering(id_progress, C_CENTERING_BOTTOM_MIDDLE); 

//also add the background
  id_pic = CreateEntPic("DAMAGEBG"); 
  EntSetXY(id_pic, 400.0, 590.0);
 EntSetResAdjust(id_pic, C_RES_ADJUST_FULL_NO_SCALE, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom
EntSetSprite(id_pic, "interface\meter_condition_02.dds");
EntSetCentering(id_pic, C_CENTERING_BOTTOM_MIDDLE); 


//background for time display

if (g_i_level_time != 0)
   {

   //this is a timed level.
 //background graphic
id_pic = CreateEntPic("TIMEBG"); 
 EntSetSprite(id_pic, "interface\meter_time.dds");
 EntSetXY(id_pic, f_time_x, f_time_y);
//
 EntSetResAdjust(id_pic, C_RES_ADJUST_FULL_NO_SCALE, 800, 600); //if screen is resized, this scales with it.  1 means middle_bottom
 EntSetCentering(id_pic, C_CENTERING_BOTTOM_MIDDLE); 

  //offset from where the background is
  //f_time_x += 9.0;
  //f_time_y += 25.0;
 int id_time = CreateEntPic("TIME"); //the one means left based meter, third parm is what to start at
 EntSetLayer(id_time, 5); //make the text appear above the bar
 EntSetFastText(id_time, 2, ""); //doing this to a picmeter causes it to draw it's results in a text form.  the num is font.
 EntSetFollow(id_time, id_pic);
 EntSetFollowOffset(id_time, -50.0, -40.0, 0.0);

 EntSetPicTextColor(id_time, 255,255,255,140); //the last part of the alpha
// EntSetXY(id_time, f_time_x, f_time_y);
 //if screen is resized, this scales with it.  1 means middle_bottom
// EntSetResAdjust(id_time, 3, 800, 600); 
 float f_temp = g_i_level_time; //convert to float
 EntSetPicTimer(id_time, f_temp, -1.0, 1000, 2); //ent, start, mod, timer between mods, mode = 1 normal, 2 for clock
 EntPicSetTrigger(id_time, 1, 0.0, i_levelscript, "FinishNoTime"); //the first parm is trigger type, 0 is SAME OR HIGHER. 1 is SAME or lower
 }
CreateEntScript("toolspawn", "script\weed_killer_spawn.c"); 
 
// g_i_weed_killer = 3;
 int i_weed = CreateEntScript("WEEDY", "script\weed_function.c");
 EntRunFunction(i_weed, "AddWeedKillers");
 
  //all done, let's get back to the game!  
 EntKill(i_my_id);

}



void OnKill()
{
	
}
