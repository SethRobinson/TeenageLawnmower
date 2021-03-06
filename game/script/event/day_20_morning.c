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
   
//let's show some dialog
 i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
  EntSetFadeUp(i_tb, 2.0); //fade speed.  
  wait(200)    
  DialogSet(i_tb, "Rise and shine.\n\nAfter brushing your teeth you get a craving for pancakes and head to the kitchen.");
//  wait(300);
   //SoundPlay("sound\knock.wav");
   DialogWaitForOk(i_tb);
   EntKill(i_tb);

  SoundMusicPlay("sound\talk_music.wav", 1000);
 
  
  //int i_bg = CreateEntPic("background"); //we can find this by name later if we want

int i_bg = EntGetByName("background");
 EntSetSprite(i_bg, "interface\kitchen.jpg");
 EntSetLayer(i_bg, -10); //show up behind everything for sure


   //now open a choice dialog, similar but different
  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  dad_start:
  
  DialogSet(id_dialog, "Your mother is sitting at the table still in her bathrobe.\n\n\"Hi Baby, there's someone I'd like you to meet.\"\n\nYou blink, not realizing she wasn't alone.");
  ConversationSetPic(id_con, "interface\mom_smile.dds");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
 DialogChoiceWait(id_choice);
 ConversationSetPic(id_con, "interface\cult_upset.dds");

  DialogSet(id_dialog, "\"Er, um hello.  I like your mom real fine,\" The Hermit mumbles without looking up.");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Mom!  That's The Hermit!  I mow his lawn for chrissakes!\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);
  ConversationSetPic(id_con, "interface\mom_normal.dds");
 
  DialogSet(id_dialog, "\"My God $g_st_name$, show some hospitality.  Dale is a very nice person.\"");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"He murdered his four wives!\"");
  DialogChoiceAdd(id_choice, 2, "\"Ok, Sorry dale\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

  if (g_i_last_choice == 1)
    {

  DialogSet(id_dialog, "Your mother slaps you, hard.\n\n\"Don't you ever speak that way to Dale!  Go to your room!\"");
  ConversationSetPic(id_con, "interface\mom_normal.dds");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

    }

  if (g_i_last_choice == 2)
{
  DialogSet(id_dialog, "\"It's ok, kid.  You can bad mouth me but don't you never say nothin' bad to or about my dogs.\"");
  ConversationSetPic(id_con, "interface\cult_upset.dds");
  
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Ok, Dale\"");
  DialogChoiceAdd(id_choice, 2, "\"Your dogs?  I hate those stupid bitches\"");
  //let's wait for input, it will be put in a global var called "g_i_last_choice"
  DialogChoiceWait(id_choice);

   if (g_i_last_choice == 1)
   {
  DialogSet(id_dialog, "Your mother smiles.  \"Well it looks like you two are getting along famously!  Why don't you join us for breakfast?\"\n\nAn hour later Dale goes home and you head back to your room.");
  ConversationSetPic(id_con, "interface\mom_smile.dds");
  g_i_time += 1; //one hour passes
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
  DialogChoiceWait(id_choice);

   }
if (g_i_last_choice == 2)
   {
  DialogSet(id_dialog, "Dale stands up.  \"MY DOGS AIN'T STUPID!  DON'T YOU CALL THEM THAT!  Damn, now I ain't sorry I used your toothbrush last night without asking ya.\"");
  DialogAdd (id_dialog, "\n\nYour mother looks nonplussed.  \"You go to your room this instant, young man.\"");
   
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Continue");
  DialogChoiceWait(id_choice);

   }
}

   EntKill(id_con); //kill all conversation elements at once 
  
//let's start the real room menu
   CreateEntScript("room menu", "script\room_menu.c");
   //kill this script
   EntKill(i_my_id); //kill this script
}

void OnKill()
{
	
}
