//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
}


void Main()
{
  g_i_total_lawns_mowed = 0;
  g_i_day = 1;
  g_i_money = 0;
  g_i_goal = 0;
 g_i_last_goal = 0;
  ReputationSet(0.0);
 g_i_money = 0; 
 g_i_cult = 0;
 g_i_last_level = 0;
 g_i_quick_tip = 0;
 g_i_cur_mower = 0; 

 g_i_thomas_story = 0;
 
  EntKill(i_my_id); //kill this script
}

void OnKill()
{
	
}
