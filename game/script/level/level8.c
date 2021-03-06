void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
  int id_choice;
  int id_dialog;
  int id_con;
  Vector3 vec_cam;
  int i_temp;
  Vector3  vec_pos;
 
}

//client as in the "customer"

void SetClientInfo()
{

  g_i_level_time = 90;  //how many seconds it takes to finish

 if (g_i_difficulty == 1)
   {

  g_i_level_time = 73;  //how many seconds it takes to finish
   }

 if (g_i_difficulty == 2)
   {

  g_i_level_time = 60;  //how many seconds it takes to finish
   }

  g_st_client_name = "Eve";
  g_st_client_home = "house";
  g_st_client_pic = "interface\eve_smile.dds";
  g_st_client_sound = "sound\hellog1.wav";
  g_i_client_pay = 30;
if (g_i_day > 24)
 {
//give a little more cash now  
g_i_client_pay = 45;

  } 

  int randomExtra = RandomRange(0, g_i_client_pay);
  randomExtra /= 2;
  g_i_client_pay += randomExtra;

g_i_client_hours = 2; //time to mow

  //I don't have to do StringExpand() now because g_i_client_pay is a global
  
    //default
      i_temp = RandomRange(1, 3);

  if (i_temp == 1)
   {
     g_st_client_phone = "Why hello there, neighbor, it's Eve.  I'd like to support your little home business.  Come on over.";
   }

   if (i_temp == 2)
   {
       g_st_client_phone = "It's Eve, hon.  I'd like you to come over and give me a good mow.";
  }

   if (i_temp == 3)
   {
       g_st_client_phone = "Hi $g_st_name$, this is Eve.  I would just be tickled pink if you’d come over and do me too.  Mow my lawn I mean, silly.";
  }
 
  g_st_client_phone = StringExpand(g_st_client_phone);
   g_st_client_decline_pic = "interface\eve_mad.dds";
   g_st_client_decline = "Who do you think you are?";

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
  ZoneLoad("data\keizer.zon");
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

vec_cam = MakeVector3(136.898, 7.753, 71.180);
EntSetPosition(id_cam, vec_cam);
EntSetDirection(id_cam, -0.491);
EntSetTilt(id_cam, -0.046);



vec_cam = MakeVector3(168.958, 44.665, 60.615);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, -0.828);
EntSetTargetTilt(id_cam, 0.587);

CameraSetLerp(0.3);
CameraSetDirLerp(0.3);

 
  //create the drink
int i_count = 6;

if (g_i_difficulty == 1)
   {
     i_count += 2;
   }

if (g_i_difficulty == 2)
   {
     i_count += 4;
   }

  int i_new;

 again:
  //let's create a gopher thingie

   i_new = CreateEntityFromTemplate( "Cafe" , "lemonade");  
 
  //now let's put it in a good place

  vec_pos = GetRandomPointInSubZone("Mow");
  EntSetPosition(i_new, vec_pos);

 // LogMsg("Making gopher at $vec_pos$.");

  i_count -= 1;

 if (i_count > 0)
  {
    goto again;
  }


//smoothly move around

  id_con = CreateConversation(""); //no name for it, we don't need to keep track
 
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);
  
  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
  ConversationSetPic(id_con, "interface\eve_smile.dds");

  i_temp = randomrange(1, 2);

if (i_temp == 1)
  {

  DialogSet(id_dialog, "Eve waves you over.  Her perfume almost knocks you out.\n\n\"Well if it ain't little $g_st_name$!  My but you've grown.  I've put lemonade out for you, drink it up!\"\n\nYou're not sure that would be such a good idea...");
  } 


if (i_temp == 2)
  {
      DialogSet(id_dialog, "Eve waves you over.\n\n\"Why that little mower of yours is just darlin’!  I realize my lawn doesn’t even look like it needs mowing but.. I’d feel sooo much better if you did.\n\nYou don’t have to wear a shirt if you don’t want to.\"");

  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "\"Um, it’s cold out, I think I’ll leave my shirt on, thanks.\"");
  DialogChoiceAdd(id_choice, 2, "\"I guess I could use a little tan, I’ll take my shirt off.\"");
  DialogChoiceWait(id_choice);
  DialogSet(id_dialog, "\"Ok honey, suit yourself.   By the way, I've placed some of my original recipe lemonade out for ya's.  You wouldn't want to miss out on that, would you?\"\n\nYou have a sneaking suspicion missing out is exactly what you want to do.");
  } 


 
  DialogChoiceClear(id_choice);
  DialogChoiceAdd(id_choice, 1, "Start mowing");
  DialogChoiceWait(id_choice);


  EntKill(id_con); //kill all conversation elements at once

  //also kill the mouse
  g_mouse_active = 0;
  CreateEntScript("", "script\mouse_off.c"); //turn the mouse off

  int id_start = EntGetByName("Start Point");
  vec_pos = EntGetPosition(id_start);
  float f_start_dir = EntGetDirection(id_start);
  EntKill(id_start);  //don't draw this again.  Actually let's kill it.  Beware of saving, it would be missing.
  //ok, we now know where to start the player.  Let's make him.
  g_id_player = CreateEntMower("Player"); //put it into a global we defined somewhere else 
  EntSetPosition(g_id_player, vec_pos);
  EntSetDirection(g_id_player, f_start_dir);
  CreateScriptFunction("script\level\startmow.c", "Main"); 

// g_i_spawn_count = 3;
// g_i_spawn_count += g_i_difficulty; //more stuff spawns if the level is set to harder

 
 //create the rock spawner
// CreateEntScript("squirrelspawn", "script\rock_spawn.c"); 
 
 CreateEntScript("INTERFACE", "script\game_interface.c"); //setup the progress indicator and such

 //end talk music and possibly start level music
 SoundMusicStop();
g_i_end_level_on_accidents = 0;  //if we get three accidents, we'll end the level.

}

void FinishLevel()
{
  CreateScriptFunction("script\level\finish_global.c", "Finish");  //All scripts get this run at finish.
}  

void FinalExit()
{
 
  //switch camera to regular control mode and move it to the house again
  
  CameraSetFollow(0); //0 means don't follow anybody
vec_cam = MakeVector3(168.958, 44.665, 60.615);
EntSetTargetPosition(id_cam, vec_cam);
EntSetTargetDirection(id_cam, -0.828);
EntSetTargetTilt(id_cam, 0.587);
  //closing conversation

id_con = -1;


  int i_bonus = 0;
   
 int id_progress = EntGetByName("PROGRESS");
 float f_percent_mowed = EntMeterGet(id_progress);
 float f_damage = 0;

 int id_damage = EntGetByName("DAMAGE");
 if (id_damage != -1)
  {
   f_damage = EntMeterGet(id_damage);
  }


  if (f_percent_mowed < 1.0)
  {
   
   if (g_i_drunk > 2)
     {
  //didn't finish mowing
 	ShowMessage("You feel sleepy...");
	wait(2500);
	ShowMessage("You fall to the ground. ");
	wait(3000);
	g_i_time += 2; //extra two hours   
	CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
        g_mouse_active = 1;
    g_i_total_lawns_mowed -= 1;
 
    int i_tb = CreateDialog("");
   DialogSetStyleMow(i_tb);
   EntSetFadeUp(i_tb, 2.0); //fade speed.  
    DialogSet(i_tb, "You wake up in the backyard two hours later.\n\nThat’s funny; your shirt is on backwards.  You find your mower and push it home.");
    DialogWaitForOk(i_tb);
   EntKill(i_tb);
       goto load_house;
     }

   }

  CreateEntScript("", "script\mouse_on.c"); //turn the mouse on
  g_mouse_active = 1;
  id_con = CreateConversation(""); //no name for it, we don't need to keep track
  id_dialog = ConversationGetDialog(id_con);
  id_choice = ConversationGetChoice(id_con);


  ConversationSetPic(id_con, "interface\eve_smile.dds");

  if (f_percent_mowed < 1.0)
  {
 
  //didn't finish the job.
   DialogSet(id_dialog, "You stop the mower without finishing the lawn, Eve approaches.\n\n\"Why did you stop, sweetie?  You need to come in and use the bathroom?\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"No, I'm going home.  Bye.\"");

   CreateScriptFunction("script\rep_mod.c", "LoseTiny");  //reputation modification  
  //combat the total_lawns mowed that will get added in a sec
   g_i_total_lawns_mowed -= 1;
   DialogChoiceWait(id_choice);
   goto load_house;
  }

  
   //default 
    DialogSet(id_dialog, "\"Why, what a wonderful job you’ve done!  Here is your $$$g_i_client_pay$.\n\nDarlin’ you must be all hot and sweaty.  How would you like to come in and have some cookies?\"");
    DialogChoiceClear(id_choice);
    DialogChoiceAdd(id_choice, 1, "\"No, I very much wouldn’t, ma’am.\"");
    DialogChoiceAdd(id_choice, 2, "\"Sure, why not.\"");
    DialogChoiceWait(id_choice);  //let's wait for input, it will be put in a global var called "g_i_last_choice"
    CreateScriptFunction("script\rep_mod.c", "GainSmall");  //reputation modification  
    g_i_money += g_i_client_pay;
  

  if (g_i_last_choice == 1)
    {
          DialogSet(id_dialog, "\"Harrumph. Well, maybe next time.  You go home and take a nice long hot shower now ya, hear?\"");
          DialogChoiceClear(id_choice);
          DialogChoiceAdd(id_choice, 1, "Leave with a wink");
          DialogChoiceAdd(id_choice, 2, "Leave quickly");
          DialogChoiceWait(id_choice);   //let's wait for input, it will be put in a global var called "g_i_last_choice"
      goto load_house; 
   }

  if (g_i_last_choice == 2)
    {
          DialogSet(id_dialog, "You follow Eve into the house and have a few cookies.  Eve eyes you hungrily. \n\n\"You know... sometimes I get lonely all by my little 'ol self.  Would you like to make a few extra bucks?  It will only take an hour.\"");
          DialogChoiceClear(id_choice);
          DialogChoiceAdd(id_choice, 1, "\"Yes, I'll do it.\"");
     
        int i_rand = randomrange(1, 6);

        if (i_rand == 1)
        {
          DialogChoiceAdd(id_choice, 2, "\"Secretly drugging me (again) is the only way you'll get me to say yes.\"");
        }

        if (i_rand == 2)
        {
          DialogChoiceAdd(id_choice, 2, "\"Hell no you painted Jezebel!\"");
        }

        if (i_rand == 3)
        {
          DialogChoiceAdd(id_choice, 2, "\"Sorry, I'm a grape and you're a raisin.\"");
        }

        if (i_rand == 4)
        {
          DialogChoiceAdd(id_choice, 2, "\"I have no need for you woman - I already have an old bag on my mower.\"");
        }
 
         if (i_rand == 5)
        {
          DialogChoiceAdd(id_choice, 2, "\"Maybe if the Wrinkle Fairy hadn't tap-danced on your face.\"");
        }
         if (i_rand == 6)
        {
          DialogChoiceAdd(id_choice, 2, "\"Erm, no.  I think the right person for you is someone who takes batteries.\"");
        }
     DialogChoiceWait(id_choice);   //let's wait for input, it will be put in a global var called "g_i_last_choice"
    
        if (g_i_last_choice == 1)
     {    
       //lose one hour but make a bit of cash
        i_bonus = randomrange(5, 25);
         DialogSet(id_dialog, "The next few minutes are too ghastly to describe.");
  DialogChoiceClear(id_choice);
   DialogChoiceAdd(id_choice, 1, " ");
    
 SoundPlay("sound\tick.wav");
    wait(500);
    SoundPlay("sound\tock.wav");
      DialogAdd(id_dialog, ".");
     wait(500);
     SoundPlay("sound\tick.wav");
    DialogAdd(id_dialog, ".");
     wait(500);
    SoundPlay("sound\tock.wav");
     DialogAdd(id_dialog, ".");
     wait(500);
     DialogAdd(id_dialog, ".");
   SoundPlay("sound\cash register.wav");

  DialogAdd(id_dialog, "\n\nAn hour later, Eve tips you $$$i_bonus$ with a wink.  \"Let's do this again real soon, honey.\"");
     
  DialogChoiceClear(id_choice);

        g_i_money += i_bonus;        
       i_temp = randomrange(1, 3);
          if (i_temp == 1)
           {
           DialogChoiceAdd(id_choice, 1, "Go home and sterilize yourself");
            }
   
           if (i_temp == 2)
          {
           DialogChoiceAdd(id_choice, 1, "Go home and burn your clothes");
          }

           if (i_temp == 3)
          {
          DialogChoiceAdd(id_choice, 1, "Go home and let the healing process begin");
          }
          DialogChoiceWait(id_choice);   //let's wait for input, it will be put in a global var called "g_i_last_choice"
           goto load_house;
      }

     if (g_i_last_choice == 2)
        {    
               //decline  
    	 ConversationSetPic(id_con, "interface\eve_mad.dds");

                DialogSet(id_dialog, "\"Eve looks pissed.\n\n\"Why you little... !  Get the hell out of here!\"");
            DialogChoiceClear(id_choice);
            DialogChoiceAdd(id_choice, 1, "Escape home");
            DialogChoiceWait(id_choice);   //let's wait for input, it will be put in a global var called "g_i_last_choice"
    
         }

      }
   

  load_house:
  //kill stuff

  //add one ot the mowed #
  g_i_total_lawns_mowed += 1;

  if (id_con != -1)
  {
  EntKill(id_con); //kill all conversation elements at once//  wait(1);
  } 
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
