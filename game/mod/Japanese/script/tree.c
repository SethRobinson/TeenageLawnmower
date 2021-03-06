//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
 vector3 vec_pos;
 int h_fire = -1; //handle to our fire sound
 int i_ps = -1;
 int i_ps_smoke = -1;
 int i_hp = 12; //how many pees it needs to die
 string st_ps; //which particle system we should use
}

void Main()
{
 
st_ps =  "script\fire.pss";

string st_temp = EntGetName(i_my_id);


if (st_temp == "Mytree3")
   {
 //    logmsg("Detected tree3.");
   st_ps =  "script\huge_fire.pss";
    
   }

if (st_temp == "Mytree2")
   {
  //   logmsg("Detected mytree2");
   st_ps =  "script\huge_fire.pss";
    
   }

if (st_temp == "tree_rich")
   {
  //   logmsg("Detected tree_rich.");
   st_ps =  "script\big_fire.pss";
    
   }
if (st_temp == "tree_manyleaves")
   {
  //   logmsg("Detected tree_manyleaves.");
   st_ps =  "script\big_fire.pss";
    
   }
if (st_temp == "tree_rich_brown")
   {
 //logmsg("Detected tree_rich_brown.");
EntReplaceTexture(i_my_id, "tree_rich_brown", "rich_leaves", "rich_leaves_brown.dds");
 EntReplaceTexture(i_my_id, "tree_rich_brown", "tree_bark", "tree_bark_2.dds");
 st_ps =  "script\big_fire.pss";
 }

if (st_temp == "tree_manyleaves_brown")
   {
 
 EntReplaceTexture(i_my_id, "tree_manyleaves_brown", "rich_leaves", "rich_leaves_brown.dds");
 EntReplaceTexture(i_my_id, "tree_manyleaves_brown", "tree_bark_2", "tree_bark.dds");

   // logmsg("Detected tree_manyleaves_brown.");
   st_ps =  "script\big_fire.pss";
    
   }

 EntSetName(i_my_id, "TREE"); 
 //don't have to specify the file extension.  the second part is the NAME of the texture set, used internally to figure out
 //how to let items share texture sets

 //turn off physics
EntSetPhysics(i_my_id, 0); 

 EntSetSmartFade(i_my_id, 1.0, 0.0); //normal ratio on close fade, don't do far fade.
}




void KillParticles()
{
 SetOnHit(i_my_id, 0);  //can't be hit
  //turn off any new particles
  ParticleSetPause(i_ps, 1);
  //let the particles that are there fade out or whatever
  wait(3000);
  EntSetPhysics(i_my_id, 0);  //turn phsyics back off
  SoundKill(h_fire);
  h_fire = -1;  
  wait(5000);

  //end script 
  EntKill(i_ps);
  EntKill(i_ps_smoke);
  i_ps = -1;
  i_ps_smoke = -1;

 //EntSetName(i_my_id, "TREE"); 

}


void WaitToKill()
{
  wait(30000);
  EntSetName(i_my_id, "BURNED");  
 SoundEntPlay(i_my_id, "sound\tree_dead.wav");
  EntSetModel(i_my_id, "model\mytree3_burned.x");
 //turn off physics
  EntSetPhysics(i_my_id, 1);  //let the new tree model fall, sometimes it's floating
  KillParticles();
  

 }

void OnBurn()
{
  EntSetName(i_my_id, "BURN"); //can't get hit again right now
  //we should catch on fire now, we got hit by lightning or something
  //create particle system

  //let's respond to taking damage, now that we're on fire.  In this case the damage is human urine of course.
 // SetOnHit(i_my_id, 1);  //1 is true

  i_ps = CreateParticle(st_ps);
  vec_pos = EntGetPosition(i_my_id);
//  logmsg("Started fire at $vec_pos$.");  
//move particle system to correct place
  EntSetPosition(i_ps, vec_pos);

 //get smoke ready to go
  i_ps_smoke = CreateParticle("script\big_smoke.pss");
  //move particle system to correct place
  EntSetPosition(i_ps_smoke, vec_pos);
  ParticleSetPause(i_ps_smoke, 1);

//get sound going too
  h_fire = Sound3DPlay(vec_pos, "sound\fire.wav", 99999);


WaitToKill();

}



void OnHit()
{
i_hp -= 1;
  
//pee hit the fire, start smoker
 SetOnHit(i_my_id, 0);  //can't be hit
 SoundPlay("sound\sizzle.wav");
//  logmsg("Ent $i_my_id$ got hit.");
  ParticleSetPause(i_ps_smoke, 0);
 if (i_hp == 0)
  {
     //all fire is gone, let it drift off
   ParticleSetPause(i_ps, 1);

  }
wait(100);
  ParticleSetPause(i_ps_smoke, 1);
 if (i_hp > 0)
  {
   SetOnHit(i_my_id, 1);  //can be hit
 }

if (i_hp == 0)
 {
   i_hp = 12;
  SoundPlay("sound\fire_out.wav");
  EntSetName(i_my_id, "TREE"); 
   KillParticles();
    return;
 } 
WaitToKill();
}

void OnKill()
{
  //run when entity is unloaded.  Do NOT use Wait() in this either.

  
  if (h_fire != -1)
  {
    SoundKill(h_fire);
  }
  if (i_ps != -1)
  {
     EntKill(i_ps);
  }
  if (i_ps != -1)
  {
    EntKill(i_ps_smoke);
  }

}
