void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{
//if this function exists, it will be run when the script is loaded. Do NOT use Wait() in this.
int i_ps = -1;
int id_pic = -1;
float f_y;
int i;
int id_progress_temp;
}

 void UseWeedKiller()
 {
   //let's apply a cool particle system to us
   i_ps = CreateParticle("script\weedkiller.pss");
   EntSetFollow(i_ps, g_id_player);
  //Let's have the effect move with the mower, rather than leave a trail (1 is true)
  //  ParticleSetFrameFollow(i_ps, 1);

 Vector3 vec_pos = EntGetPosition(g_id_player);
 SoundEntPlay(g_id_player, "sound\weedkiller.ogg");
 int i_mow_tile = GetMowedTile();
 if (i_mow_tile != 228)
 {
  //well, it ain't snow, so let's do it
 MowSection(vec_pos, 9.2);
 }
 
 
  wait(2000);
  ParticleSetPause(i_ps, 1);
if (i_mow_tile == 228)
{
 ShowMessage("Weed killer has no effect!");
 	
}
	
 wait(2000);
 EntKill(i_ps);
 EntKill(i_my_id);
}

void RemoveWeedKiller()
{
  //remove the last added weed killer.  Make sure you check g_i_weed_killer to see if one exists before calling this.
 g_i_weed_killer -= 1; //remove it
  int id_weed = EntGetByName("WEEDKLR$g_i_weed_killer$");
  if (id_weed != 0)
  {
    EntSetColorFade(id_weed, 255,255,255,0,820);
   EntSetUniformScale(id_weed, 0.06);
    EntSetKillTimer(id_weed, 2000);
    EntSetName(id_weed, "CRAP");
  }
 UseWeedKiller();
 EntKill(i_my_id);
 }

void AddWeedKiller()
{
   //add rotating weed killer graphics
 id_progress_temp = EntGetByName("PROGRESS");
f_y = -50;

float f_temp = g_i_weed_killer;
f_temp *= 50;
f_y -= f_temp;
 
  id_pic = CreateEntPic("WEEDKLR$g_i_weed_killer$"); 
  g_i_weed_killer += 1;
  //EntSetXY(id_pic, 50,f_y);
//EntSetResAdjust(id_pic, 0, 800, 600); //if screen is resized, this scales with it.  1 means middle_bottom
  EntSetModel(id_pic, "model\weedkiller.x");
  EntSetLayer(id_pic, 50); //show up in front 
  EntSetUniformScale(id_pic, 0.03);
  EntSetFollow(id_pic, id_progress_temp);
  EntSetFollowOffset(id_pic, 40.0, f_y, 0.0);
  EntSetPicVisualEffect(id_pic, C_PIC_VISUAL_EFFECT_BOB);
	
}


void KillWeedKillers()
{
  i = 0;
  again:
  EntKillByName("WEEDKLR$i$"); 
  i += 1;
  if (i > C_MAX_WEED_KILLERS)
   {
    return;
   }  
  goto again;
}

void AddWeedKillers()
{
  KillWeedKillers();	

  //add rotating weed killer graphics
 id_progress_temp = EntGetByName("PROGRESS");
 
 i = 0; 
  f_y = -50;
 add_another:
  
  if (i == g_i_weed_killer)
   {
     //all done
      EntKill(i_my_id);
      return;
    }	

//add one
 
  id_pic = CreateEntPic("WEEDKLR$i$"); 
  //EntSetXY(id_pic, 50,f_y);
//EntSetResAdjust(id_pic, 0, 800, 600); /if screen is resized, this scales with it.  1 means middle_bottom
  EntSetModel(id_pic, "model\weedkiller.x");
  EntSetLayer(id_pic, 50); //show up in front 
  EntSetUniformScale(id_pic, 0.03);
  EntSetFollow(id_pic, id_progress_temp);
  EntSetFollowOffset(id_pic, 40.0, f_y, 0.0);
  EntSetPicVisualEffect(id_pic, C_PIC_VISUAL_EFFECT_BOB);

 f_y -= 50;
 i += 1;
 goto add_another;
 	
}

void Main()
{
}

void OnKill()
{	
	if (i_ps != -1)
	{
	EntKill(i_ps);
 	}
 }
