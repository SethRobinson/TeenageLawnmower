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
  SoundMusicPlay("sound\main_menu.wav", 1);
 
  //create it!
  int i_bg = EntGetByName("background");

 //  EntDefaultResAdjust(i_bg, 2); //simpler than using EntSetResAdjust
  EntSetSprite(i_bg, "interface\data.jpg");

 //  EntSetLayer(i_bg, -10); //show up behind everything for sure
g_i_thomas_story = 2; //let everybody know we did this
g_i_cur_mower = 1; //he'll use the cool mower from this day forth 
//set parms for the next day
 wait(6000);
float f_rep = ReputationGet();
f_rep *= 100;
int i_rep = f_rep;

            i_tb = CreateDialog("");
            DialogSetStyleMow(i_tb);
           DialogSet(i_tb, "You have completed the story portion of the game after successfully mowing $gg_i_total_lawns_mowed$ lawns with an overall rating of $i_rep$.\n\nCongratulations!");
 	SoundPlay("sound\pay_time.wav");
           DialogWaitForOk(i_tb);
     //close everything and load the main menu
                EntKill(i_bg);
      EntKill(i_tb);
wait(20);

  int i_crap = CreateEntScript("RoomMenu", "script\room_menu.c"); 
  EntKill(i_my_id); //kill this script
 

}

void OnKill()
{
	
}
