//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_con;
 int id_dialog;
 int id_choice;
 int i_tb;
}



void GetDifficulty()
{
 
    int i_bg = CreateEntPic("background"); //we can find this by name later if we want
    EntSetSprite(i_bg, "interface\house.jpg");
    EntSetLayer(i_bg, -10); //show up behind everything for sure

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:

  DialogSet(id_dialog, "ゲームを始める前に、ゲームの難易度を選択してください　\n\nゲームの難易度によって、エンディングが違います。");
 // ConversationSetPic(id_con, "interface\father_bigsmile.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 0, "初心者");
  DialogChoiceAdd(id_choice, 1, "中級者");
  DialogChoiceAdd(id_choice, 2, "超上級者");

  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);
  g_i_difficulty =  g_i_last_choice;

  if (g_i_difficulty == 0)
   {
     //set values for EASY
     g_i_deadline_money = 4000;
   }

  if (g_i_difficulty == 1)
   {
     //set values for NORMAL
     g_i_deadline_money = 6000;
   }

  if (g_i_difficulty == 2)
   {
     //set values for hard
     g_i_deadline_money = 8000;
   }

  DialogChoiceClear(id_choice);
  EntKill(id_con); //kill all conversation elements at once




}

void GotName()
{
 g_st_name = g_st_last_string;
 

 EntKill(i_tb);


  g_i_day = 0; //it will get set to 0 in newday
  //show a little text  blurb welcoming them to the game
wait(200);  
  
   i_tb = CreateDialog("STORY");
   DialogSetStyleMow(i_tb);
   DialogSet(i_tb, "アメリカの中流家庭の生活を、どうぞ体験して下さい…\n\n");
     DialogWaitForOk(i_tb);
 EntKill(i_tb);
  wait(1000);  

  //let's start the real game, starting on the newday screen
  CreateEntScript("NEWDAY", "script\\new_day.c");
   EntKill(i_bg); 
  //EntKillAllPics(); //get rid of all backgrounds and buttons on the screen
  EntKill(i_my_id);
}

void Main()
{
  EntKillAllPics(); //get rid of all backgrounds and buttons on the screen
  //Set Some defaults
   CreateEntScript("DEFAULTS", "script\setdefaults.c"); //this will set the defaults and then kill itself

 //first get difficulty level
 GetDifficulty();

 //now get the  name.

   i_tb = CreateEntPic("TEMP");
   EntSetSprite(i_tb, "interface\enter_name.dds");
   EntSetResAdjust(i_tb, 1, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom

   EntSetXY(i_tb, 170.0, 240.0);
   //wait for enter to get hit
  int i_input = CreatePicInput("NAME", "", 0, 2); //2nd parm is default text, 3rd is style of input. 0 for normal string input. last parm is font id
 EntSetResAdjust(i_input, 1, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom
 EntSetPicTextColor(i_input, 20,255,10,255); //the last part of the alpha
 EntSetXY(i_input, 242.0, 269.0);
 EntPicInputSetTrigger(i_input, i_my_id, "GotName"); //call us back if they hit ENTER (or escape?)
}

void OnKill()
{
	
}
