void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
int i_temp;

}


//this is run everytime they get a phone call, figure out what level they can play.
void Main()
{
}

void SetupLevel()
{
int i_tries = 0;
//******  DAY 1 through 3 ********
  

try_again:

//default
 g_i_level = randomrange(1, 4); 


if (g_i_day > 3)
{
    g_i_level = randomrange(1, 7); 

  if (g_i_level == 7)
   {
     g_i_level = 8;  //weird I know, but I want eve available this early
   }
}

if (g_i_day > 6)
{
    i_temp = randomrange(1, 3);
   //only a 1 in 3 chance of doing an old level

    g_i_level = randomrange(5, 9); 

   if (i_temp == 1)
    {

    g_i_level = randomrange(1, 4); 

    }
}

  if (g_i_day > 10)
{
  i_temp = randomrange(1, 3);
   //only a 1 in 3 chance of doing an old level

    g_i_level = randomrange(5, 10); 

   if (i_temp == 1)
    {

    g_i_level = randomrange(1, 4); 

    }

}

if (g_i_day > 13)
{
  i_temp = randomrange(1, 3);
   //only a 1 in 3 chance of doing an old level
//add bolt montana
    g_i_level = randomrange(5, 11); 

   if (i_temp == 1)
    {

    g_i_level = randomrange(1, 4); 

    }

}

if (g_i_day > 15)
{
  i_temp = randomrange(1, 8);
   //only a 1 in 8 chance of doing an old level

  //add second graveyard

    g_i_level = randomrange(5, 15); 

  //hack to make 9 happen more often.

   if (g_i_level == 13)
    {
      g_i_level = 9;
    }
   if (g_i_level == 14)
    {
      g_i_level = 9;
    }
   if (g_i_level == 15)
    {
      g_i_level = 9;
    }
   


   if (i_temp == 1)
    {
    g_i_level = randomrange(1, 4); 
    }
}

if (g_i_day > 18)
{
  i_temp = randomrange(1, 8);
   //only a 1 in 8 chance of doing an old level

  //add second graveyard

    g_i_level = randomrange(5, 17); 


//hack to make 9 happen more often.

  
   if (g_i_level == 15)
    {
      g_i_level = 9;
    }
   if (g_i_level == 16)
    {
      g_i_level = 9;
    }
   if (g_i_level == 17)
    {
      g_i_level = 9;
    }
  
   

   if (i_temp == 1)
    {

    g_i_level = randomrange(1, 4); 

    }
}





//******* activate cheats
 if (g_i_force_level_script != 0)
  {
    g_i_level = g_i_force_level_script;

  }  

//******** some levels have more than one story, check for it here
  if (g_i_level == 3)
   {
     //park level.  Pick a random story.
     if (g_i_day > 2)
      { 
     i_temp = randomrange(1, 2);
      if (i_temp == 2)
      {
        g_i_level = 200;
      }
    }
   }

  if (g_i_level == 9)
    {
     
    logmsg("Choosing thomas level (day $g_i_day$)");

     //the rich guy.  let's randomize it up a bit depending on the day
      if (g_i_day > 16)
      {
         //add one level
        i_temp = randomRange(1, 2);
        if (i_temp == 1)
        {
          g_i_level = 92; //the first dirt level
        }
      }
   //the rich guy.  let's randomize it up a bit depending on the day
      if (g_i_day > 20)
      {
         //add two levels
        i_temp = randomRange(1, 3);
       if (i_temp == 1)
        {
          g_i_level = 92; //the first dirt level
        }
        if (i_temp == 2)
        {
          g_i_level = 93; //the first dirt level
        }

      }

       if (g_i_day > 29)
       {
          if (g_i_thomas_story < 2)
          {
            g_i_level = 91; //the final level
          }
          if (g_i_thomas_story == 2)
          {
           //let him play any level for old times sake
   		 i_temp = randomRange(1, 4);
      
              if (i_temp == 1)
             {
                g_i_level = 92; //the first dirt level
               }
                if (i_temp == 2)
               {
                 g_i_level = 93; //the second dirt level
                }
                 if (i_temp == 3)
                {
                  g_i_level = 94; //the final level with different text
                }
    
          }
       }
    }


  //if it looks like they got the same level as last time, try again

  if (g_i_level == g_i_last_level)
   {
     //let's avoid an infinit loop in the case that we only have one choice
     if (i_tries < 10)
      {
         i_tries += 1;
         goto try_again;

      }

   }


//remember what this was for next time we run this script
g_i_last_level = g_i_level;

 //also set the defaults

g_i_accidents = 0;
g_i_cleaned = 0;
g_i_drunk = 0;
g_i_bonus = 0;
g_id_player = -1; //assigned when the mower is created
g_i_mole_kill = 0; //by default moles can't be killed without registering an 'accident'
g_i_spawn_count = 0; 
}



void OnKill()
{
	
}
