//utility script to do simple things

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
  float f_rep_mod;
  int id_damage;
  float f_temp;
}

void Main()
{
 
}

void CheckDamage()
{
 if (g_i_quit == 0)
  { 
  float f_damage =   EntMeterGet(id_damage);  
  EntMowerSetHealth(g_id_player, f_damage);
  if (f_damage <= 0)
  {
    
  int i_level_script = EntGetByName("LevelScript"); //our main script
  g_i_quit = 1; //disable the escape menu  
  EntRunFunction(i_level_script, "FinishLevel");
  EntKill(i_my_id); //kill this script
  }

 }
}
void DamageKill()
{
  id_damage = EntGetByName("DAMAGE");
  f_temp = -1.0;
  f_temp *= g_f_damage_mod;
  EntMeterMod(id_damage, f_temp);
  CheckDamage();
  EntKill(i_my_id);
}

void DamageMedium()
{
  id_damage = EntGetByName("DAMAGE");
 f_temp = -0.35;
  f_temp *= g_f_damage_mod;
  EntMeterMod(id_damage, f_temp);
   CheckDamage();
  EntKill(i_my_id);
}

void DamageSmall()
{
  id_damage = EntGetByName("DAMAGE");
 f_temp = -0.12;
  f_temp *= g_f_damage_mod;
  EntMeterMod(id_damage, f_temp);
   CheckDamage();
  EntKill(i_my_id);
}

void DamageTiny()
{
  id_damage = EntGetByName("DAMAGE");
 f_temp = -0.03;
  f_temp *= g_f_damage_mod;
  EntMeterMod(id_damage, f_temp);
   CheckDamage();
  EntKill(i_my_id);
}
void DamageQuarter()
{
  id_damage = EntGetByName("DAMAGE");
 f_temp = -0.25;
  f_temp *= g_f_damage_mod;
  EntMeterMod(id_damage, f_temp);
   CheckDamage();
  EntKill(i_my_id);
}

void HealDamageHalf()
{

  id_damage = EntGetByName("DAMAGE");
  f_temp = 0.5;
  f_temp *= g_f_damage_mod;
  EntMeterMod(id_damage, f_temp);
  CheckDamage();
  EntKill(i_my_id);

}

void OnKill()
{
}



