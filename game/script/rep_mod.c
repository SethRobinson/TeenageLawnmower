//utility script to do simple things

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
  float f_rep_mod;
}

void Main()
{
 
}

void LoseTiny()
{
   f_rep_mod = FRandomRange(-0.05, -0.1);
   ReputationMod(f_rep_mod);
   EntKill(i_my_id);
}

void GainTiny()
{
   f_rep_mod = FRandomRange(0.05, 0.1);
   ReputationMod(f_rep_mod);
   EntKill(i_my_id);
}

void GainSmall()
{
   f_rep_mod = FRandomRange(0.09, 0.02);
   ReputationMod(f_rep_mod);
   EntKill(i_my_id);
}


void LoseMedium()
{
   f_rep_mod = FRandomRange(-0.2, -0.3);
   ReputationMod(f_rep_mod);
   EntKill(i_my_id);
}


void GainMedium()
{
   f_rep_mod = FRandomRange(0.2, 0.3);
   ReputationMod(f_rep_mod);
   EntKill(i_my_id);
}

void OnKill()
{
}



