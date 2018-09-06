void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 int id_dialog;
 int i_tb;
int i_temp;
}


void Main()
{

}

void OnKill()
{
	
}

// st_temp = "Earn $$$g_i_goal_money$ by the morning of day $g_i_goal_day$ for $g_st_goal$";

void Setup()
{
logmsg("Setting up for goal $g_i_goal$.");
    g_i_goal_day = g_i_day;  
   g_i_goal_money = 0;
  
   //uncomment this and basically have extra money doesn't help
  //g_i_goal_money = g_i_money;
  
if (g_i_last_goal == 0)
  {
     //set the very first goal
    g_st_goal = "groceries";
    g_i_goal_money += 77; //how much money is needed to make  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for $g_st_goal$.";
    g_st_goal_lose_string = "You and your mother starve to death.  The end.";
    g_i_goal_day += 1; //how many days they have from now
    goto fin;
  }

if (g_i_last_goal == 1)
  {
     //given on day 2
    g_st_goal = "the electric bill";
    g_i_goal_money += 150; //how much money is needed to make  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for $g_st_goal$.";
    g_st_goal_lose_string = "With no electricty you are unable to get online to promote your business.  You and your mother have to move in with Todd.\n\nThe end.";
    g_st_goal_start = "You notice a note on your desk.  \"We need $g_i_goal_money$ bucks to pay the electric bill in a couple days.. I'm a bit short this month..sorry hun.\" - Mom";  
    g_i_goal_day += 2; //how many days they have from now
    goto fin;
  }

if (g_i_last_goal == 2)
  {
     
   i_temp = randomrange(1, 3);

  //let's mix it up a bit

  if (i_temp == 1)
   {

     //goal three, given on day 4 
    g_st_goal = "the air conditioner";
    g_i_goal_money += 325; //how much money is needed to make  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for $g_st_goal$.";
    g_st_goal_lose_string = "A heat wave occurs before you are able to fix the AC.  You suffer a stroke and die.\n\nThe end.";
    g_st_goal_start = "You notice a note on your desk.  \"The air conditioner broke last night.  We really need $g_i_goal_money$ bucks to buy a new one.\" - Mom";  
    g_i_goal_day += 3; //how many days they have from now
   
    }    
  if (i_temp == 2)
   {

     //goal three, given on day 4 
    g_st_goal = "the stove";
    g_i_goal_money += 315; //how much money is needed to make  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for $g_st_goal$.";
    g_st_goal_lose_string = "The gas stove has started a fire during the night.  You and your mother are burned alive.\n\nThe end.";
    g_st_goal_start = "You notice a note on your desk.  \"The stove barely works.  We really need $g_i_goal_money$ bucks to buy a new one.\" - Mom";  
    g_i_goal_day += 3; //how many days they have from now
   
    }    
  if (i_temp == 3)
   {

     //goal three, given on day 4 
    g_st_goal = "the water heater";
    g_i_goal_money += 325; //how much money is needed to make  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for $g_st_goal$.";
    g_st_goal_lose_string = "As you try to take a shower you are surprised with icy water.  Your heart can't take it and you die.\n\nThe end.";
    g_st_goal_start = "You notice a note on your desk.  \"The water heater is only half working.  We really need $g_i_goal_money$ bucks to buy a new one.\" - Mom";  
    g_i_goal_day += 3; //how many days they have from now
   
    }    
 goto fin;

  }
 
if (g_i_last_goal == 3)
  {
     //goal three, given on day 7 
    g_st_goal = "an ultrasound";
    g_i_goal_money += 105; //how much money is needed to make  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for $g_st_goal$.";
    g_st_goal_lose_string = "Your mother is unable to pay for the ultrasound.  You are killed in a freak accident.\n\nThe end.";
    g_st_goal_start = "You notice a note on your desk.  \"I need to get an ultrasound tomorrow.  Can you help me out?  I need $g_i_goal_money$ bucks.\" - Mom";  
    g_i_goal_day += 1; //how many days they have from now
    goto fin;
  }


if (g_i_last_goal == 4)
  {
     //given on day 8 
    g_st_goal = "car repairs";
    g_i_goal_money += 250; //how much money is needed to make  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for $g_st_goal$.";
    g_st_goal_lose_string = "Your mother is killed in a car accident.  Probably those faulty brakes.\n\nThe end.";
    g_st_goal_start = "You notice a note on your desk.  \"Sweetie, my car needs some work.  Can you come up with $g_i_goal_money$ bucks?\" - Mom";  
    g_i_goal_day += 2; //how many days they have from now
    goto fin;
  }

if (g_i_last_goal == 5)
  {
     //given on day 10 
    g_st_goal = "new clothes";
    g_i_goal_money += 105; //how much money is needed to make  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for $g_st_goal$.";
    g_st_goal_lose_string = "Trying to conduct business without clothes fails miserably, except at Eve's.\n\nThe end.";
    g_st_goal_start = "Your clothes don't seem to fit very well anymore.  You'll need to buy some new stuff.";  
    g_i_goal_day += 1; //how many days they have from now
    goto fin;
  }
if (g_i_last_goal == 6)
  {
     //given on day 11 
    g_st_goal = "a spark plug";
    g_i_goal_money += 2; //how much money is needed to make  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for $g_st_goal$.";
    g_st_goal_lose_string = "You couldn't afford 2 bucks?!!!You had better never see this message!\n\nThe end.";
    g_st_goal_start = "You need to replace your mower's spark plug very soon.";  
    g_i_goal_day += 1; //how many days they have from now
    goto fin;
  }

if (g_i_last_goal == 7)
  {
     //given on day 12 
    g_st_goal = "the mortgage";
    g_i_goal_money += 425; //how much money is needed to make  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for $g_st_goal$.";
    g_st_goal_lose_string = "After missing a series or mortgage payments the property is repossessed.  Your new home is the street.\n\nThe end.";
    g_st_goal_start = "You notice a note on your desk.  \"Sweetie, I need help with the rent.  Can you come up with $g_i_goal_money$ bucks?\" - Mom";  
    g_i_goal_day += 3; //how many days they have from now
    goto fin;
  }

if (g_i_last_goal == 8)
  {
     //given on day 15 
    g_st_goal = "the visa payment";
    g_i_goal_money += 280; //how much money is needed to make  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for $g_st_goal$.";
    g_st_goal_lose_string = "After missing the visa payment the interest is so high you are immediately killed.\n\nThe end.";
    g_st_goal_start = "You notice a note on your desk.  \"I really have to make a $g_i_goal_money$ dollar payment on my visa, can you help me out?\" - Mom";  
    g_i_goal_day += 2; //how many days they have from now
    goto fin;
  }
if (g_i_last_goal == 9)
  {
     //given on day 17 
    g_st_goal = "a new hard drive";
    g_i_goal_money += 150; //how much money is needed to make  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for a new 120 gig drive.";
    g_st_goal_lose_string = "Your HD died and you had no backups.  Treat your hard drive as if every day might be its last.\n\nThe end.";
    g_st_goal_start = "Your hard drive is making clicking noises.  You need to buy a new one, FAST.";  
    g_i_goal_day += 1; //how many days they have from now
    goto fin;
  }

if (g_i_last_goal == 10)
  {
     //given on day 18 
    g_st_goal = "legal fees";
    g_i_goal_money += 570; //how much money is needed to make  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay your lawyer.  You win the case!";
    g_st_goal_lose_string = "Without professional representation your accuser is able to prove you mauled his animals.  You're ruined.\n\nThe end.";
    g_st_goal_start = "One of your clients is suing you.  You need $g_i_goal_money$ dollars for legal fees FAST.";  
    g_i_goal_day += 4; //how many days they have from now
    goto fin;
  }
if (g_i_last_goal == 11)
  {
     //given on day 22 
    g_st_goal = "a winning bid";
    g_i_goal_money += 375; //how much money is needed to make  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay the seller for the Atari.";
    g_st_goal_lose_string = "Oh oh, you are labeled a dead-beat bidder and your online reputation is ruined.\n\nThe end.";
    g_st_goal_start = "You see a mint Atari 2600 on ebay and must have it.  You now owe 375 for it!";  
    g_i_goal_day += 3; //how many days they have from now
    goto fin;
  }

 //if we got here let's just make a random thingie for them

//first pick the name of the thing

 int i_per_day_price;

 if (g_i_difficulty == 0)
  {
    i_per_day_price = RandomRange(80, 110);
  } 

 if (g_i_difficulty == 1)
  {
    i_per_day_price = RandomRange(80, 130);
  } 

 if (g_i_difficulty == 2)
  {
    i_per_day_price = RandomRange(80, 150);
  } 

  
  int i_days = RandomRange(1, 3);
  g_i_goal_money = i_per_day_price;
  g_i_goal_money *= i_days; 
  g_i_goal_day += i_days; //how many days they have from now


 if (i_days == 1)
  {
   //******** 1 DAY EVENTS ********
  i_temp = RandomRange(1, 3);
 
  if (i_temp == 1)
  {
    g_st_goal = "a new RTSOFT game";
    g_st_goal_start = "You see an ad for a spectacular new RTSOFT game.. you must own it!  Wow, expensive, it's $g_i_goal_money$ bucks.";  
    g_st_goal_win_string = "You use $g_i_goal_money$ dollars to purchase an RTSOFT game.  It rocks!";
    g_st_goal_lose_string = "Without playing games you get too stressed out and go postal.  But with a mower.\n\nThe end.";
      }

  if (i_temp == 2)
    {
    g_st_goal = "a book on mowing";
    g_st_goal_start = "Your craft is getting stale.  You decide you need to purchase a book on Lawn Mowing for only $g_i_goal_money$ dollars.";  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to buy a book on the art of the mow.";
    g_st_goal_lose_string = "Your customers get tired of your old techniques and begin to drift away.\n\nThe end.";
    }
  
  if (i_temp == 3)
    {
    g_st_goal = "a new mowing outfit";
    g_st_goal_start = "Your affluent and debonair lifestyle requires you to buy a new trendy mowing outfit tomorrow. ($g_i_goal_money$ dollars)";  
     g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for $g_st_goal$.";
     g_st_goal_lose_string = "You go out mowing with bellbottoms.  You're laughed off the lawn.\n\nThe end.";
    }
  }


if (i_days == 2)
  {
   //******** 2 DAY EVENTS ********
  i_temp = RandomRange(1, 2);
 
  if (i_temp == 1)
  {
    g_st_goal = "caffiene drinks";
    g_st_goal_start = "To your horror your minifridge has no ice coffee or soda.  You'll need $g_i_goal_money$ bucks to restock.";  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for $g_st_goal$.";
    g_st_goal_lose_string = "Without caffiene you start waking up at 3 PM.  Only Krell will hire you.\n\nThe end.";
      }

  if (i_temp == 2)
    {
    g_st_goal = "a new game console";
    g_st_goal_start = "Woah, MicroSony has released a new game console this week.  You have 2 days to make $g_i_goal_money$ dollars for it.";  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for $g_st_goal$.";
    g_st_goal_lose_string = "You attempt to steal the new console.. AND ARE CAUGHT AND CASTRATED.\n\nThe end.";
    }
  
  }

if (i_days == 3)
  {
   //******** 3 DAY EVENTS ********
  i_temp = RandomRange(1, 2);
 
  if (i_temp == 1)
  {
    g_st_goal = "a mower lube";
    g_st_goal_start = "The mower needs a tune up, compression check and lube in three days. It will be $g_i_goal_money$ bucks.";  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for $g_st_goal$.";
    g_st_goal_lose_string = "As you are taking down a giant mole your mower explodes killing all.\n\nThe end.";
      }

  if (i_temp == 2)
    {
    g_st_goal = "an RC heli";
    g_st_goal_start = "You need a new hobby.  A used RC Helicopter with radio is only $g_i_goal_money$ bucks!";  
    g_st_goal_win_string = "You use $$$g_i_goal_money$ to pay for $g_st_goal$.";
    g_st_goal_lose_string = "You try to make a custom RC Heli using your mower upside down.  It flies, but you are decapitated.\n\nThe end.";
    }
  
  }
fin:
if (g_i_goal_money != 0)
 {
g_i_last_goal += 1;
g_i_goal = 1; //goal is active
logmsg("Set goal to $g_i_goal_day$, last goal is now $g_i_last_goal$..");
  }
EntKill(i_my_id); //kill this script
}
