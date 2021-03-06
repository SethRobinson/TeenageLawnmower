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
  

//   int i_bg = CreateEntPic("background"); //we can find this by name later if we want
//   EntSetSprite(i_bg, "interface\room.jpg");
//   EntSetLayer(i_bg, -10); //show up behind everything for sure
   
//let's show some dialog
 i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
   DialogSet(i_tb, "There is a knock at your door.");
   wait(500);
   SoundPlay("sound\knock.wav");
   DialogWaitForOk(i_tb);
   EntKill(i_tb);

  SoundMusicPlay("sound\talk_music.wav", 1000);
 
  //now open a choice dialog, similar but different

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:

  DialogSet(id_dialog, "Your mother steps into your room.  She looks a little unsure on her feet and you definitely smell alcohol.\n\n\"Hi sweetie.  Sorry about what happened last night.\"");
  ConversationSetPic(id_con, "interface\mom_normal.dds");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Mom, it doesn't matter, forget it.\"");
  DialogChoiceAdd(id_choice, 2, "\"Wow mom, that's a mighty strong aroma you're wearing.  It's that new Jack Daniels fragance, isn't it?\"");
 
  //let's wait for input, it will be put in a global var called "g_i_last_choice"

  DialogChoiceWait(id_choice);

 string st_response;
  if (g_i_last_choice == 1)
  {
    st_response = "Oh..  right.  Well I still feel bad about it, $g_st_name$.";
  }
  
  if (g_i_last_choice == 2)
  {
    st_response = "You know what, $g_st_name$?  I really don't need to be hearing that from you right now.  I've had a long day.";
  } 
 st_response = StringExpand(st_response); //make sure $ vars get converted
  //PART 2

  DialogSet(id_dialog, "\"$st_response$\n\nAnyway, Eve told me you've been out mowing lawns today.  I'm really proud of you.\"");
 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Hmm, for once Eve's gossip is right.  Why does she have to spy on everyone, anyway?\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

  DialogSet(id_dialog, "Your mother laughs, although it quickly turns into a coughing fit.\n\n");
  DialogAdd(id_dialog, "\"She's a little out there.  I don't think you should mow her lawn if she asks.  I have to get to bed now, I have to go somewhere early.\"");

 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Yeah, I know, she's... unique.  Go where? It's your day off.\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);


  DialogSet(id_dialog, "\"Oh, just run some errands.  I love you, 'night.\"");
 
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
