void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
  int id_choice;
  int id_dialog;
  int id_con;
  vector3 vec_cam;
  int i_temp;
 
}

//client as in the "customer"

void SetClientInfo()
{
  g_i_level_time = 145;  //how many seconds it takes to finish

  if (g_i_difficulty == 1)
  {
	  g_i_level_time = 122;  //how many seconds it takes to finish
  }

  if (g_i_difficulty == 2)
  {
	  g_i_level_time = 110;  //how many seconds it takes to finish
  }

  g_st_client_name = "Brother Zoran";
  g_st_client_home = "compound";
  g_st_client_pic = "interface\priest_normal.dds";
  g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 125;
  g_i_client_hours = 6; //time to mow

  int randomExtra = RandomRange(0, g_i_client_pay);
  randomExtra /= 2;
  g_i_client_pay += randomExtra;

  //I don't have to do StringExpand() now because g_i_client_pay is a global
 

  g_st_client_decline_pic = "interface\priest_mad.dds";
    g_st_client_decline = "By the eye of Xylos you shall regret this!";

    //default
    g_st_client_phone = "My  name is Zoran, traveler and mower of lawns.  My people require your assistance.  We would like a package deal.";
  if (g_i_cult > 0)
     {
       //special messages for members
         g_st_client_phone = "Greetings $g_st_cult_title$ $g_st_name$.  Xylos requires your assistance once again.";
         g_st_client_decline_pic = "interface\priest_normal.dds";
       g_st_client_decline = "I am sure you have a very good reason Brother.  Xylos is watching.";

      }  

g_st_client_phone = StringExpand(g_st_client_phone);
  done:
 
}

void Main()
{
  //don't actually do anything when loaded
}

void StartLevel()
{

  //actually set up the level
  g_i_mode = 2; //0 means main menu, not playing.  1 is room, 2 is 3d mode.

  //get some tunes goin'
  SoundMusicPlay("sound\talk_music.wav", 1000);


  if (g_st_force_level == "")
 {
  ZoneLoad("data\cult.zon");
 }

  if (g_st_force_level != "")
 {
  logmsg("Forcing level $g_st_force_level$.");
  ZoneLoad(g_st_force_level);
 }

 //load the weather we were told to
  int id_weather = CreateEntWeather("Weather");
  EntWeatherLoad(id_weather, g_st_weather); //change this if the level needs it


  int id_cam = EntGetByName("Main Camera");

//set initial camera location

vec_cam = MakeVector3(59.469, 17.828, 186.547);
EntSetPosition(id_cam, vec_cam);
EntSetDirection(id_cam, -3.545);
EntSetTilt(id_cam, 0.080);



vec_cam = MakeVector3(156.065, 23.811, 173.144);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, -2.278);
EntSetTargetTilt(id_cam, 0.249);


CameraSetLerp(0.3);
CameraSetDirLerp(0.3);

//smoothly move around


  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
if (g_i_cult == 0)
   { 

  DialogSet(id_dialog, "Zoran greets you with a few 'followers'.\n\n\"Hello, grass cutter.  Xylos may return from the stars at any time.  Thus, we must prepare the lawn for him.\"");
  ConversationSetPic(id_con, "interface\priest_normal.dds");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Um, ok.\"");
  DialogChoiceWait(id_choice);


  DialogSet(id_dialog, "\"We share all houses and the car.  Please mow all our lawns as one.  Our guard dogs have been instructed to leave you alone, all is well.\"");
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"As long as the money is green.  Let's do it.\"");
  DialogChoiceWait(id_choice);
  }


  if (g_i_cult != 0)
   {
        //********* they are a member
  DialogSet(id_dialog, "Zoran signals you to approach.\n\n\"I'm glad you could make the trek out here in pilgrimage.  Shake off the worldly influence you've had and prepare our lawn in the style that Xylos prefers.\"");
  ConversationSetPic(id_con, "interface\priest_normal.dds");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Yes Brother Xoran, I shall.\"");
  DialogChoiceWait(id_choice);
   }


  EntKill(id_con); //kill all conversation elements at once

  //also kill the mouse
  g_mouse_active = 0;
  CreateEntScript("", "script\mouse_off.c"); //turn the mouse off

  int id_start = EntGetByName("Start Point");
  Vector3 vec_pos = EntGetPosition(id_start);
  float f_start_dir = EntGetDirection(id_start);
  EntKill(id_start);  //don't draw this again.  Actually let's kill it.  Beware of saving, it would be missing.
  //ok, we now know where to start the player.  Let's make him.
  g_id_player = CreateEntMower("Player"); //put it into a global we defined somewhere else 
  EntSetPosition(g_id_player, vec_pos);
  EntSetDirection(g_id_player, f_start_dir);
  CreateScriptFunction("script\level\startmow.c", "Main"); 

// g_i_spawn_count = 3;
// g_i_spawn_count += g_i_difficulty; //more stuff spawns if the level is set to harder

//let's spawn X amount of dogs and get started

 
 //create the rock spawner
// CreateEntScript("squirrelspawn", "script\rock_spawn.c"); 
 
 CreateEntScript("INTERFACE", "script\game_interface.c"); //setup the progress indicator and such

 //end talk music and possibly start level music
 SoundMusicStop();
g_i_end_level_on_accidents = 0;  //if we get three accidents, we'll end the level.

  //create all the dobermans
int i_count = 5;

if (g_i_difficulty == 1)
   {
     i_count += 3;
   }

if (g_i_difficulty == 2)
   {
     i_count += 6;
   }


 again:
  //let's create a gopher thingie
  int i_new = CreateEntityFromTemplate( "Creatures" , "Doberman");  
  //now let's put it in a good place

  vec_pos = GetRandomPointInSubZone("Mow");
  EntSetPosition(i_new, vec_pos);

 // LogMsg("Making gopher at $vec_pos$.");

  i_count -= 1;

 if (i_count > 0)
  {
    goto again;
  }


}

void FinishLevel()
{
  CreateScriptFunction("script\level\finish_global.c", "Finish");  //All scripts get this run at finish.
}  

void FinalExit()
{
  //switch camera to regular control mode and move it to the house again
  
  CameraSetFollow(0); //0 means don't follow anybody
 vec_cam = MakeVector3(111.127, 2.633, 107.173);
 EntSetTargetPosition(id_cam, vec_cam);
 EntSetTargetDirection(id_cam, 0.621);
 EntSetTargetTilt(id_cam, 0.127);
 
  //closing conversation

  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
  g_mouse_active = 1;
  id_con = CreateConversation(""); //no name for it, we don't need to keep track
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);

  int i_bonus = 0;
   
 int id_progress = EntGetByName("PROGRESS");
 float f_percent_mowed = EntMeterGet(id_progress);
 float f_damage = 0;

 int id_damage = EntGetByName("DAMAGE");
 if (id_damage != -1)
  {
   f_damage = EntMeterGet(id_damage);
  }

 
  ConversationSetPic(id_con, "interface\priest_normal.dds");


   //if they are members let's do special code

  if (g_i_cult > 0)
  {
    //they are members

 if (f_percent_mowed < 1.0)
  {
    ConversationSetPic(id_con, "interface\priest_mad.dds");

  //didn't finish the job.
   DialogSet(id_dialog, "\"Brother, why have you forsaken us by not finishing the job?\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"Sorry, Xylos willed it I guess\" (return home)");
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;
  }

    i_bonus = 50;

   i_bonus *= g_i_cult;

   //default 
  DialogSet(id_dialog, "You flag Zoran down and give him the secret handshake.\n\n\"Xylos is pleased with your work.  Here is your $$$g_i_client_pay$ plus $$$i_bonus$ bonus, brother.\n\nWould you like to join us in meditation?\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"No, bye\"");
    DialogChoiceAdd(id_choice, 2, "\"Of course\"");
    DialogChoiceWait(id_choice);
    g_i_money += g_i_client_pay;
   g_i_money += i_bonus;
      DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, " ");
   
  if (g_i_last_choice == 1)
    {
      //leave 
      goto load_house;
     }

    //don't let them get past heigh priest

   if (g_i_cult == 4)
     {

   //not interested
       DialogSet(id_dialog, "\"Now that you have reached the title of $g_st_cult_title$ you do not need meditation.  Any higher and you'd get my position.  Xylos doesn't want that.  Sorry.\"");
       DialogChoiceClear(id_choice);
       DialogChoiceAdd(id_choice, 1, "Return home");
       DialogChoiceWait(id_choice);
       goto load_house;
    }



   CreateScriptFunction("script\rep_mod.c", "GainSmall");  //reputation modification  
     ConversationSetPic(id_con, "interface\priest_smile.dds");

   DialogSet(id_dialog, "Xoran smiles.\n\n\"While I commend you for joining us Xylos tells us only through sacrifice can we truly understand him.  Ask any questions you may have.\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"At what point do I get many wives?\"");
    DialogChoiceAdd(id_choice, 2, "\"Why so many dogs?\"");
    DialogChoiceAdd(id_choice, 3, "\"When can I see Xylos?\"");
    DialogChoiceWait(id_choice);
   string st_temp;

   if (g_i_last_choice == 1)
    {
     st_temp = "Well.. about that.  Xylos has pretty much ordered that all the women live in my house.  He's funny that way.";        
    }

   if (g_i_last_choice == 2)
    {
     st_temp = "Dog is mans best friend, they are cute.  Also we have a lot of guns and weed; they keep people off the property.";        
    }

   if (g_i_last_choice == 3)
    {
     st_temp = "No man knoweth the time nor day of his arrival.  He's crazy like that, likes to keep us on our toes I guess.";        
    }
    int i_join = 150;
    i_temp = randomrange(1, 100);
    i_join += i_temp;

    DialogSet(id_dialog, "\"$st_temp$\n\nIf you would like to donate an additional $$$i_join$ you will be truly blessed.\"\n\nYou are holding $$$g_i_money$.");
    DialogChoiceClear(id_choice);
  
    if (g_i_money < i_join)
    {
         DialogChoiceAdd(id_choice, 2, "\"I can't afford that\"");
     }

    if (g_i_money >= i_join)
       {
         DialogChoiceAdd(id_choice, 1, "\"Sure, Xylos wills it\"");
         DialogChoiceAdd(id_choice, 2, "\"No thanks\"");
       }

    DialogChoiceWait(id_choice);
   
    if (g_i_last_choice == 2)
    {
        //not interested
       DialogSet(id_dialog, "\"I see.  Maybe another time.  Go now.  Spread his truth as you spread the cuttings of your labor.");
       DialogChoiceClear(id_choice);
       DialogChoiceAdd(id_choice, 1, "Return home");
       DialogChoiceWait(id_choice);
       goto load_house;
    }

    //they want to upgrade
   //set their new status
   g_i_cult += 1;


  if (g_i_cult == 2)
    { 
       g_st_cult_title = "Acolyte";
    }

 if (g_i_cult == 3)
    { 
     g_st_cult_title = "Priest";
    }

 if (g_i_cult == 4)
    { 
      g_st_cult_title = "High Priest";
    }
   

    g_i_money -= i_join; //pay for membership
   ConversationSetPic(id_con, "interface\priest_smile.dds");
        SoundPlay("sound\cash register.wav");
   DialogSet(id_dialog, "You hand the money to brother Zoran who quickly pockets it.\n\n\"Wonderful!  Congratulations, $g_st_cult_title$ $g_st_name$.\"\n\nYou are given a new robe.");
   DialogChoiceClear(id_choice);
   DialogChoiceAdd(id_choice, 1, "Return home");
   DialogChoiceWait(id_choice);
      goto load_house;
 
   }










///************ NOT MEMBERS ***********

  if (f_percent_mowed < 1.0)
  {
    ConversationSetPic(id_con, "interface\priest_mad.dds");

  //didn't finish the job.
   DialogSet(id_dialog, "\"It would appear that you have deceived us.  Mend your ways or Xylos can never accept you into his house of love.\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "Return home (by running)");
   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;
  }

  

   //default 
  DialogSet(id_dialog, "You flag Zoran down.\n\n\"Xylos is pleased with your work.  Here is your $$$g_i_client_pay$.\n\nWould you be interested hearing more about our brotherhood?\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"No, bye.\"");
    DialogChoiceAdd(id_choice, 2, "\"Yes, tell me more.\"");
    DialogChoiceWait(id_choice);
    g_i_money += g_i_client_pay;
 
      DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, " ");
   
  if (g_i_last_choice == 1)
    {
      //leave 
      goto load_house;
     }

   

  //they want to hear more


   CreateScriptFunction("script\rep_mod.c", "GainSmall");  //reputation modification  
     ConversationSetPic(id_con, "interface\priest_normal.dds");

   DialogSet(id_dialog, "Xoran looks pleased.\n\n\"Basically we do not accept the values that are so prevalent on this earth.  We worship a higher being, we strive for a better way of life.\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"Is this like Scientology?\"");
    DialogChoiceAdd(id_choice, 2, "\"Who is Xylos?\"");
    DialogChoiceAdd(id_choice, 3, "\"Nice robe\"");
    DialogChoiceWait(id_choice);

   if (g_i_last_choice == 1)
    {
     st_temp = "Scientology?  Haha, no.";        
    }

   if (g_i_last_choice == 2)
    {
     st_temp = "Xylos is a being from another race who follows the comet Zolorious in his space ship which is glorious.  Hey, that rhymed.";        
    }

   if (g_i_last_choice == 3)
    {
     st_temp = "Thank you.  It's the new slim-line model; stylish yet durable.  We get them in bulk.";        
    }
   i_join = 250;
   DialogSet(id_dialog, "\"$st_temp$\n\nIf you would make a donation of only $$$i_join$ you may become a brother of Xylos.\"  (You are holding $$$g_i_money$)");
    DialogChoiceClear(id_choice);
  
    if (g_i_money < i_join)
    {
         DialogChoiceAdd(id_choice, 2, "\"I can't afford that\"");
     }

    if (g_i_money >= i_join)
       {
         DialogChoiceAdd(id_choice, 1, "\"Sure, sign me up\"");
         DialogChoiceAdd(id_choice, 2, "\"No thanks\"");
       }

    DialogChoiceWait(id_choice);
   
    if (g_i_last_choice == 2)
    {
        //not interested
       DialogSet(id_dialog, "\"I see.  Maybe another time.  Again, thanks for shortening our blades with yours, grass man.");
       DialogChoiceClear(id_choice);
       DialogChoiceAdd(id_choice, 1, "Return home");
     DialogChoiceWait(id_choice);
     goto load_house;
    }

    //if we got here they want to join the cult
    g_i_money -= i_join; //pay for membership
   ConversationSetPic(id_con, "interface\priest_smile.dds");
        SoundPlay("sound\cash register.wav");
   DialogSet(id_dialog, "You hand the money to brother Zoran who quickly pockets it.\n\n\"You've made the right choice. We will be in touch.\"\n\nYou are taught the secret handshake and sent on your way.");
   DialogChoiceClear(id_choice);
   g_i_cult = 1; //joined, level 1
   g_st_cult_title = "Brother";  
   DialogChoiceAdd(id_choice, 1, "Return home");
   
     DialogChoiceWait(id_choice);


   

  load_house:
  //kill stuff

  //add one ot the mowed #
  g_i_total_lawns_mowed += 1;

  EntKill(id_con); //kill all conversation elements at once//  wait(1);
  EntKillAllPics(); //get rid of all backgrounds and buttons on the screen
  EntKill(g_id_player);
  g_i_quit = 0; //enable the escape menu

  //Clear Level
  ZoneLoad(""); //blanks out the level
  //kill all interface items
   EntKill(id_weather);
  //kill this script
  EntSetPauseLevel(i_my_id, 100); //by doing this, we won't get deleted with a KillAllEnts command, which is good because this
  //script needs to continue
  LogMsg("Clearing level..");
  EntKillAllItems();
  
  //Load room
  CreateEntScript("RoomMenu", "script\room_menu.c");
  EntKill(i_my_id); //kill this script

}
void FinishNoTime()
{
id_progress = EntGetByName("PROGRESS");
  f_percent_mowed = EntMeterGet(id_progress);
 if (f_percent_mowed >= 1.0)
  {
    //they actually completely the level, ignore the no time thing.
   return; 
  }
 CreateScriptFunction("script\level\finish_global.c", "NoTime");  //All scripts get this run at finish.
 FinishLevel();
}

void OnKill()
{
	
}
