//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
//-1 means no mole has been created for the hole yet
int i_mole = -1;
}


void Main()
{
 
EntSetNoDraw(i_my_id, 1);

//give us time to orient to the ground
wait (400);
EntSetNoDraw(i_my_id, 0);

EntSetPhysics(i_my_id, 0); //turn off physics

 //let's respond to taking damage.
 SetOnHit(i_my_id, 1);  //1 is true


//scale the dirt up

 float f_scale = 0.0f;

 scale_start:
 f_scale += 0.01f;
 
// logmsg("scaling to $f_scale$.");
EntSetUniformScale(i_my_id, f_scale); 
 wait (50);

 if (f_scale < 0.50)
 {
   //loop around
   goto scale_start;  
 }

//ok, now make the mole pop up

i_mole = CreateEntityFromTemplate( "Creatures" , "MoleBig");   


  //move him to our exact position
 vector3 vec_pos = EntGetPosition(i_my_id);

 EntSetPosition(i_mole, vec_pos);


 
}

void OnHit()
{
  //we got hit.  let's die


  //play sound effect, EntPlay does a 3d sound
  SoundEntPlay(i_my_id, "sound\drop.wav");

  //first hide our self

  //don't allow any more hits
  SetOnHit(i_my_id, 0); 

  //don't draw ourself anymore
  EntSetNoDraw(i_my_id, 1);
 
  //if the mole is under ground, let's kill him
 if (i_mole != -1)
 {
   int i_exist = EntGetExist(i_mole);

 if (i_exist == 1)
  {
 //logmsg("Mole exists still."); 
   int i_hidden = EntGetNoDraw(i_mole);   
 
   if (i_hidden == 1)
   {
 // logmsg("Mole is hidden, killing it.");
     EntKill(i_mole);
   }
 }

 }
  //chalk one up to the score
  // g_i_cleaned += 1; 
 
  //create particle system
  
  int i_ps = CreateParticle("script\big_dirt.pss");
  vec_pos = EntGetPosition(i_my_id);
  //move particle system to correct place
  EntSetPosition(i_ps, vec_pos);
 CreateScriptFunction("script\damage_mod.c", "DamageTiny");

  wait(80);
  //turn off any new particles
  ParticleSetPause(i_ps, 1);

  //let the particles that are there fade out or whatever
  wait(1500);

  //end script 
  EntKill(i_ps);
  EntKill(i_my_id);

}


void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.
//  logmsg("Mole hold killed.");
}