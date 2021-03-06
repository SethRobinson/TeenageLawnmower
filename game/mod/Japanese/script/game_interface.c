void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_pic;
}


void Main()
{
  
 float f_progress_x = 10.0;
 float f_progress_y = 532.0;

 float f_time_x = 687.0;
 float f_time_y = 532.0;


//add the progress background
 id_pic = CreateEntPic("PROGRESSBG"); 
 EntSetResAdjust(id_pic, 1, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom
 EntSetXY(id_pic, f_progress_x, f_progress_y);
 EntSetSprite(id_pic, "interface\meter_progress.dds");

 //add progress percent text

  //offset from where the background is
  f_progress_x += 10.0;
  f_progress_y += 25.0;


 int id_progress = CreateEntPicMeter("PROGRESS", 1, 0.0); //the one means left based meter, third parm is what to start at
 EntSetResAdjust(id_progress, 1, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom
 EntSetLayer(id_progress, 5); //make the text appear above the bar
 EntSetPicTextColor(id_progress, 255,255,255,140); //the last part of the alpha
 EntSetXY(id_progress, f_progress_x, f_progress_y);
// EntSetSprite(id_progress, "interface\meter_progress.dds");

 int i_levelscript = EntGetByName("LEVELSCRIPT");
 //it will call FinishLevel() automatically when we hit 100 percent
 EntSetFastText(id_progress, 2, ""); //doing this to a picmeter causes it to draw it's results in a text form.  the num is font.

 EntPicSetTrigger(id_progress, 0, 1.0, i_levelscript, "FinishLevel"); //the first parm is trigger type, 0 is SAME OR HIGHER. 


//also create damage indicator

 //add percent mowed indicator
 id_progress = CreateEntPicMeter("DAMAGE", 1, 0.0); //the one means left based meter, third parm is what to start at
 EntSetResAdjust(id_progress, 1, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom
 EntSetLayer(id_progress, 5); //make the text appear above the bar
 EntSetColor(id_progress, 255,255,255,140); //the last part of the alpha
 EntSetXY(id_progress, 300.0, 560.0);
 EntSetSprite(id_progress, "interface\meter_condition.dds");
 EntMeterSet(id_progress, 1.0f);

//also add the background
  id_pic = CreateEntPic("DAMAGEBG"); 
 EntSetResAdjust(id_pic, 1, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom
 EntSetXY(id_pic, 300.0, 560.0);
 EntSetSprite(id_pic, "interface\meter_condition_02.dds");


//background for time display

if (g_i_level_time != 0)
   {

   //this is a timed level.
 //background graphic
id_pic = CreateEntPic("TIMEBG"); 
 EntSetResAdjust(id_pic, 1, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom
 EntSetXY(id_pic, f_time_x, f_time_y);
 EntSetSprite(id_pic, "interface\meter_time.dds");

  //offset from where the background is
  f_time_x += 9.0;
  f_time_y += 25.0;

 id_progress = CreateEntPic("TIME"); //the one means left based meter, third parm is what to start at
 EntSetResAdjust(id_progress, 1, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom
 EntSetLayer(id_progress, 5); //make the text appear above the bar
 EntSetFastText(id_progress, 2, ""); //doing this to a picmeter causes it to draw it's results in a text form.  the num is font.

 EntSetPicTextColor(id_progress, 255,255,255,140); //the last part of the alpha
 EntSetXY(id_progress, f_time_x, f_time_y);
 float f_temp = g_i_level_time; //convert to float

 EntSetPicTimer(id_progress, f_temp, -1.0, 1000, 2); //ent, start, mod, timer between mods, mode = 1 normal, 2 for clock

 EntPicSetTrigger(id_progress, 1, 0.0, i_levelscript, "FinishNoTime"); //the first parm is trigger type, 0 is SAME OR HIGHER. 1 is SAME or lower
 

}

  //all done, let's get back to the game!  
 EntKill(i_my_id);

}



void OnKill()
{
	
}
