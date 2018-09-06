//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_con;
 int id_dialog;
 int id_choice;
 int i_tb;
 int i_temp;
}

void Main()
{
  
   //setup background

 
  //now open a choice dialog, similar but different

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:

  DialogSet(id_dialog, "After finishing breakfast you are feeling great.  It's amazing how much better you sleep after doing physical labor.\n\nBut what should you do now?");
  //ConversationSetPic(id_con, "interface\father_bigsmile.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Check your favorite websites");
  DialogChoiceAdd(id_choice, 2, "Take a shower");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);

 string st_response;
  if (g_i_last_choice == 1)
  {
    st_response = "You check a few sites.. no, the new webcomic won't be out until thursday but.. hey, some good links...\n\nTwo hours later you realize you still need to take your shower.";
   g_i_time += 2; //two hours lost, haha. 
DialogSet(id_dialog, st_response);
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);


 }
  
    st_response = "While taking your shower you notice a home pregnancy test in the garbage.";
 int i_bg = EntGetByName("background");
 EntSetSprite(i_bg, "interface\bathroom.jpg");

  //PART 2

  DialogSet(id_dialog, st_response);
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
   
   EntKill(id_con); //kill all conversation elements at once 
   //let's start the real room menu
   CreateEntScript("room menu", "script\room_menu.c");
   //kill this script
   EntKill(i_my_id); //kill this script
}

void OnKill()
{
	
}
