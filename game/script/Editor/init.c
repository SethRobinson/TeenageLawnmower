//note:  i_my_id will already exist and contain  the id of the entity this script is attached too.

void OnCreate() //always put this FIRST in your script if you are going to use it.  Declare script-wide globals here.
{

   global int g_id_player = -1; 
   global int g_mouse_active = 1;
   global int true = 1;
   global int false = 0;
   global int g_id_camera = -1;
   global int g_id_weather = -1;

  //Used with EntSetForceType()
   global int C_FORCE_GROUND_ONLY = 0;
   global int C_FORCE_GROUND_AND_AIR = 1; 

  //Used with EntSetGravityType()
   global int C_GRAVITY_WORLD = 0;
   global int C_GRAVITY_OVERRIDE = 1;
   global int C_GRAVITY_MOD = 2;

   //used with EntSetWallBounce()
   global int C_HITWALL_NOTHING = 0;
   global int C_HITWALL_REVERSE = 1;

   global int C_ENT_PHYSICS_NONE = 0;
   global int C_ENT_PHYSICS_NORMAL = 1;

  //Used with EntSetShadow()
  global int C_VISUAL_SHADOW_NONE = 0;
  global int C_VISUAL_SHADOW_ACCURATE = 1; //accurate shadow based on the mesh, works with .x, .md2 and .rts 
  global int C_VISUAL_SHADOW_FAKE = 2; //draw a little fake circle under them for a shadow, fast but crappy looking (implemented yet? I forget)

  //used with EntSetCollisionType()
  global int C_COLLISION_SPHERICAL = 0; //good for round objects like boulders
  global int C_COLLISION_BOX = 1; //default

  //used with EntSetBroadcastType()
  global int C_ENT_BROADCAST_NONE = 0;
  global int C_ENT_BROADCAST_MOVEMENT = 1;
  
  //used with EntSetHardness()
  global int C_ENT_HARDNESS_NONE = 0;
  global int C_ENT_HARDNESS_NORMAL = 1;

  //used with GetHost()
  global int C_HOST_EDITOR = 0;

}


//this script is run only once when the game starts.

void Main()
{

   //Set Some defaults (same ones that get set when creating a new game)
  // CreateEntScript("DEFAULTS", "script\setdefaults.c"); //this will set the defaults and then kill itself
  g_id_camera = EntGetByName("Main Camera");
   //let's create the main menu to kick things off
    //kill this script
   EntKill(i_my_id); //kill this script
}

void OnKill()
{
	
}
