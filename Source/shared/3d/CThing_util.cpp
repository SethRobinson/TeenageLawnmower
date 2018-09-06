/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CThing_util.h"
#include "CMessageHeader.h"
#include "CThingList.h"
#include "CThing_Globals.h"
#include "CCamera.h"
#include "CTerrain.h"
#include "CControlPlayer.h"
#include "CFloatCam.h"
#include "CControlTopView.h"
#include "CEntPlayer.h"
#include "d3dutil.h"
#include "..\\CScriptEngine.h"
#include "Script_utils.h"
#include "CEntLib.h"       
#include "CThingFont.h"
#include <direct.h> //for chdir

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

D3DXVECTOR3 g_vec_up = D3DXVECTOR3(0,1,0);
D3DXVECTOR3 g_vec_down = D3DXVECTOR3(0,-1,0);
D3DXVECTOR3 g_vec_null = D3DXVECTOR3(0,0,0);




 //takes a script, runs it's shutdown command and returns the var initted to C_SCRIPT_NONE
void ShutDownScript(int & i_script)
{
   	if (i_script != ::C_SCRIPT_NONE)
	{
		 	cls_tglo.p_script_engine->RunScriptFunction(i_script, "ONKILL");
	
			//now actually remove the script from memoty
			cls_tglo.p_script_engine->UnloadScript(i_script);
			i_script = C_SCRIPT_NONE;
	}

}




float lerp_float_direction(float f_origin, float f_target, float f_percent)
{
  
   	//let's make sure we turn the shortest way to reach our direction
   
	if (f_origin > f_target)
	{
		if (  ((f_target + 2*D3DX_PI) - f_origin) < D3DX_PI )
		{
			//the target would be closer if we added 2pi
			f_target += 2*D3DX_PI;
		}


	}	else
	{
		//check the other way
		if (  ((f_origin + 2*D3DX_PI) - f_target) < D3DX_PI )
		{
			//the target would be closer if we added 2pi
			f_origin += 2*D3DX_PI;
		}


	}


   return (f_origin - ((f_origin-f_target)*f_percent));


}



float lerp_float_direction_test(float f_origin, float f_target, float f_percent)
{
 /* 
   	char st_crap[255];

	sprintf(st_crap, "Origin %.3f  Target: %.3f", f_origin, f_target);
	log_msg(st_crap);
*/
	
	//extra accurate safe version

	f_origin = float(fmod(f_origin, D3DX_PI*2))+D3DX_PI*2;
	f_target = float(fmod(f_target, D3DX_PI*2))+D3DX_PI*2;
	//let's make sure we turn the shortest way to reach our direction
   
	if (f_origin > f_target)
	{
		
		
		if (  ((f_target + 2*D3DX_PI) - f_origin) < D3DX_PI )
		{
			//the target would be closer if we added 2pi
			f_target += 2*D3DX_PI;
		}


	}	else
	{
		//check the other way
		if (  ((f_origin + 2*D3DX_PI) - f_target) < D3DX_PI )
		{
			//the target would be closer if we added 2pi
			f_origin += 2*D3DX_PI;
		}


	}


   return (f_origin - ((f_origin-f_target)*f_percent));


}

void SetupPixelFog(DWORD Color, DWORD Mode, float Start, float End)
{
        float  Density = 0.66f;  // For exponential modes
 
    // Enable fog blending.
    cls_tglo.p_d3dDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
 
    // Set the fog color.
    cls_tglo.p_d3dDevice->SetRenderState(D3DRS_FOGCOLOR, Color);
    
    // Set fog parameters.
    if(D3DFOG_LINEAR == Mode)
    {
        cls_tglo.p_d3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, Mode);
        cls_tglo.p_d3dDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
        cls_tglo.p_d3dDevice->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&End));
    }
    else
    {
        cls_tglo.p_d3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, Mode);
        cls_tglo.p_d3dDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&Density));
    }
 
}


 bool bToReturn = true;

//this basically works by rotating the initial angle around 180 degrees (1*pi) and checking whether
//  the new angle is within that rotation of the initial angle... it helps if you draw a picture

  // just this small change
 




void lerp_vec3(D3DXVECTOR3 *vec_out, D3DXVECTOR3 *vec_origin, D3DXVECTOR3 *vec_target, float f_percent)
{
  *vec_out = (*vec_origin - ((*vec_origin-*vec_target)*f_percent));
}





char * print_vector(D3DXVECTOR3 vec)
{

  static char st_text[120];
  sprintf(st_text, "X: %.4f Y: %.4f Z: %.4f",
	  vec.x, vec.y, vec.z);
  return st_text;
}

char * print_point(D3DXVECTOR2 *vec)
{

  static char st_text[120];
  sprintf(st_text, "X: %.4f Y: %.4f",
	  vec->x, vec->y);
  return st_text;
}


void get_x_and_z_from_radian(D3DXVECTOR3 *p_vec, float f_angle)
{
	p_vec->x = (float) cos(f_angle);
	p_vec->z = (float) sin(f_angle);
}



//-----------------------------------------------------------------------------
// Name: IntersectTriangle()
// Desc: Given a ray origin (orig) and direction (dir), and three vertices of
//       of a triangle, this function returns TRUE and the interpolated texture
//       coordinates if the ray intersects the triangle
//-----------------------------------------------------------------------------
BOOL IntersectTriangle( D3DXVECTOR3 * p_vec_out,  const D3DXVECTOR3& orig,
                                       const D3DXVECTOR3& dir, D3DXVECTOR3& v0,
                                       D3DXVECTOR3& v1 , D3DXVECTOR3& v2/*,
                                       FLOAT* t, FLOAT* u, FLOAT* v */)
{
   	static float u, v;
	
	// Find vectors for two edges sharing vert0
    D3DXVECTOR3 edge1 = v1 - v0;
    D3DXVECTOR3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    D3DXVECTOR3 pvec;
    D3DXVec3Cross( &pvec, &dir, &edge2 );

    // If determinant is near zero, ray lies in plane of triangle
    FLOAT det = D3DXVec3Dot( &edge1, &pvec );

    D3DXVECTOR3 tvec;
    if( det > 0 )
    {
        tvec = orig - v0;
    }
    else
    {
        tvec = v0 - orig;
        det = -det;
    }

    if( det < 0.0001f )
        return FALSE;

    // Calculate U parameter and test bounds
    u = D3DXVec3Dot( &tvec, &pvec );
    if( u < 0.0f || u > det )
        return FALSE;

    // Prepare to test V parameter
    D3DXVECTOR3 qvec;
    D3DXVec3Cross( &qvec, &tvec, &edge1 );

    // Calculate V parameter and test bounds
    v = D3DXVec3Dot( &dir, &qvec );
    if( v < 0.0f || u + v > det )
        return FALSE;
	/*
    // Calculate t, scale parameters, ray intersects triangle
    *t = D3DXVec3Dot( &edge2, &qvec );
    FLOAT fInvDet = 1.0f / det;
    *t *= fInvDet;
    *u *= fInvDet;
    *v *= fInvDet;
	  */
    return TRUE;
}


//this is for comparing two angles

bool thetas_are_facing_same_way(float f_theta1, float f_theta2, float f_epsilon)
{
 	
	if (f_theta1 < (f_theta2+f_epsilon)) 
	if (f_theta1 > (f_theta2-f_epsilon)) return true;

	
	//same check inced
	if (f_theta1 < f_theta2) f_theta1 += (D3DX_PI*2); else f_theta2 += (D3DX_PI*2);
	if (f_theta1 < (f_theta2+f_epsilon)) 
	if (f_theta1 > (f_theta2-f_epsilon)) return true;

	//special check for rolling angles

    return false;
}

//um, for the record atan2 does the exact same thing as this...

float get_theta_from_x_and_z(float f_x, float f_z)
{
#ifdef _DEBUG
	if (f_x == 0) if (f_z == 0)
	{
	  //bad case here..
	//	return 0;
		//log_error("Bad theta.");
	}
#endif
	float ttemp;
  	
	if ( (f_z > 0) && (f_x > 0) )
	{
		ttemp = (float) atan(f_x / f_z);
	}
	
	if ( (f_z < 0) && (f_x > 0) )
	{
		f_z = (float) fabs(f_z);
		ttemp = (float) ( 3.1415926f - atan(f_x / f_z) );
	}
	
	if ( (f_z < 0) && (f_x < 0) )
	{
		f_x = (float) fabs(f_x);
		f_z = (float) fabs(f_z);
		ttemp = (float) ( 3.1415926f + atan(f_x / f_z) );
	}
	
	
	if ( (f_z > 0) && (f_x < 0) )
	{
		f_x = (float) fabs(f_x);
		ttemp = (float) ((6.28f) - atan(f_x / f_z) );
	}
	
	
	//special cases
	
	if ( (f_x == 0) && (f_z > 0) )
	{
		ttemp =0;       
	}
	
	if ( (f_x == 0) && (f_z < 0) )
	{
		ttemp = 3.1415926f;       
	}
	
	if ( (f_x > 0) && (f_z == 0) )
	{
		ttemp = 1.57f;       
	}
	
	if ( (f_z == 0) && (f_x < 0) )
	{
		ttemp = (4.71f);       
	}
	
	return ttemp;
} 



int Fast_Distance_2D(int x, int y)
{
// this function computes the distance from 0,0 to x,y with 3.5% error

// first compute the absolute value of x,y
x = abs(x);
y = abs(y);

// compute the minimum of x,y
int mn = min(x,y);

// return the distance
return(x+y-(mn>>1)-(mn>>2)+(mn>>4));

} // end Fast_Distance_2D

///////////////////////////////////////////////////////////////////////////////

float Fast_Distance_3D(float fx, float fy, float fz)
{
// this function computes the distance from the origin to x,y,z

int temp;  // used for swaping
int x,y,z; // used for algorithm

// make sure values are all positive
x = int(fabs(fx) * 1024);
y = int(fabs(fy) * 1024);
z = int(fabs(fz) * 1024);

// sort values
if (y < x) SWAP(x,y,temp)
if (z < y) SWAP(y,z,temp)
if (y < x) SWAP(x,y,temp)

int dist = (z + 11*(y >> 5) + (x >> 2) );

// compute distance with 8% error
return((float)(dist >> 10));

} // end Fast_Distance_3D



//send this a point A and a point B and it will conbert it into an origin and direction and check all the ents
//you specify. -1 for all ents.  If the thing doesn't know how to handle a ray pick it will ignore it.

//returns NULL if no hit is made

//if p_vec_hit isn't null the position of the hit will be put there

CThing * ray_pick_objects(CThingList *p_thing_list, D3DXVECTOR3 *p_vec_point_a, D3DXVECTOR3 * p_vec_point_b, int i_type, float *p_f_dist, float f_range, D3DXVECTOR3 *p_vec_hit,  D3DXVECTOR2 *vec_mouse)
{

 //let's ask each object if they've been hit or not, and where, if they have.

 	//also save all items, monsters, people and entities
	 CMessageHeader msg;
	 msg.i_message = C_THING_RAY_PICK;
	 CMessageRayPick custom; //the custom message
	 
	 custom.f_dist = 0;
	 custom.f_range = f_range;
	 D3DXVECTOR3 vec_dir, vec_hit;
	 
//	 vec_dir = (*p_vec_point_a - *p_vec_point_b);
	 
	 	 vec_dir = (*p_vec_point_b - *p_vec_point_a);

	 
	 D3DXVec3Normalize(&vec_dir,  &vec_dir);


	 
	 custom.p_vec_origin = p_vec_point_a;
	 custom.p_vec_end = p_vec_point_b;
	 custom.p_vec_dir = &vec_dir;
 	 custom.p_thing = NULL; 
	 custom.p_vec_hit = &vec_hit;
 	 custom.p_vec_mouse = vec_mouse;

	 
	 p_thing_list->broadcast_message(&msg, (void*)&custom, i_type); 
 
	 if (!custom.p_thing) return NULL; //didn't hit anything

	 //a hit was made!
	 *p_f_dist = custom.f_dist;

	 if (p_vec_hit)
	 {
	   //copy in the actual hit location
		 *p_vec_hit = *custom.p_vec_hit;

	 }
 return custom.p_thing;
}


bool get_3d_mouse_position(D3DXVECTOR3 *p_vec)
{
	    D3DXVECTOR3 vec_point_a, vec_point_b;
	    //get close point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_a, cls_tglo.p_cls_input->GetMouseX(),
		cls_tglo.p_cls_input->GetMouseY(), 0.001f); //close to screen
		
		//get far point
		cls_tglo.p_camera->get_screen_to_world_point(&vec_point_b, cls_tglo.p_cls_input->GetMouseX(),
		cls_tglo.p_cls_input->GetMouseY(), 1000); //close to screen

		D3DXVECTOR3 vec_ground;
		D3DXPLANE plane_ground;
		if (!cls_tglo.p_terrain->find_ground_from_ray(&vec_point_a, &vec_point_b, &vec_ground, &plane_ground))
		{
		  //didn't touch anything
			return false;
		}
  
		//we touched something...
		*p_vec = vec_ground; //this is the real hit position

		return true;
}

void switch_to_play_cam()
{
    //switch camera
  	
	//kill current control system
	cls_tglo.p_thing_list->delete_things_by_type(C_THING_CONTROL);
	//install new one
	CControlPlayer *p_play = new CControlPlayer();
	cls_tglo.p_thing_list->add_new_thing(p_play);
}

void switch_to_free_cam()
{
    //switch camera
	//kill current control system
	cls_tglo.p_thing_list->delete_things_by_type(C_THING_CONTROL);
	//install new one
	CFloatCam *p_play = new CFloatCam();
	cls_tglo.p_thing_list->add_new_thing(p_play);
}


void switch_to_top_view_cam()
{
    //switch camera
	//kill current control system
	cls_tglo.p_thing_list->delete_things_by_type(C_THING_CONTROL);
	//install new one
	CControlTopView *p_play = new CControlTopView();
	cls_tglo.p_thing_list->add_new_thing(p_play);
}

void switch_to_car_cam()
{
    log_msg("DISABLED!");
}


void switch_to_player_walk_cam()
{
    //switch camera
	//kill current control system
	cls_tglo.p_thing_list->delete_things_by_type(C_THING_CONTROL);
	//install new one
	CEntPlayer *p_play = new CEntPlayer();
	cls_tglo.p_thing_list->add_new_thing(p_play);
	p_play->dx_create();
}


void set_random_sun()
{
     D3DLIGHT8 light;
//	   D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, 0-frandom(1), 0-frandom(1), 0.f );
	   D3DUtil_InitLight( light, D3DLIGHT_POINT, 0,0,0);
	   light.Position.x = random(120)-60.0f;
	   light.Position.y =  70;
	   light.Position.z = random(120)-60.0f;
   light.Range = 1000;
      light.Specular.a = 1;
	 light.Specular.r = 1;
	 light.Specular.g = 1;
	 light.Specular.b = 1;
	 light.Attenuation0 = 1.0f;
	 light.Ambient.a = 1;
	 light.Ambient.r = 0.3f;
	 light.Ambient.g = 0.3f;
	 light.Ambient.b = 0.3f;


   light.Attenuation0 = 1;
	   light.Attenuation1 = 0;
	  //ight.Attenuation2 = 0.0001f;
        cls_tglo.p_d3dDevice->SetLight( 0, &light );
	
}

void set_random_fog()
{
   	   SetupPixelFog(D3DCOLOR_ARGB(255,255,255,255), D3DFOG_LINEAR, frandom(20), frandom(120));

}

void set_reset_light_and_fog()
{
   	 	 cls_tglo.p_d3dDevice->SetRenderState(D3DRS_FOGENABLE, false);
	  D3DLIGHT8 light;
    //D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, -1.0f, -1.0f, 2.0f );
    
	D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, -0.3f, -1.0f, 0.f );
	 light.Specular.a = 1;
	 light.Specular.r = 1;
	 light.Specular.g = 1;
	 light.Specular.b = 1;
	 light.Attenuation0 = 1.0f;
	 light.Ambient.a = 1;
	 light.Ambient.r = 0.3f;
	 light.Ambient.g = 0.3f;
	 light.Ambient.b = 0.3f;

	cls_tglo.p_d3dDevice->SetLight( 0, &light );
    cls_tglo.p_d3dDevice->LightEnable( 0, TRUE );
    cls_tglo.p_d3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

}   

void set_landscape_wire_frame_toggle()
{
    //toggle draw mode
	  if (cls_tglo.p_terrain->get_fill_mode() == D3DFILL_SOLID)
	  {
	     cls_tglo.p_terrain->set_fill_mode(D3DFILL_WIREFRAME);

	  } else
	  {
	     cls_tglo.p_terrain->set_fill_mode(D3DFILL_SOLID);

	  }


}


void init_engine_from_base_path(bool b_switch_dirs_and_handle_scripts_too)
{
    if (b_switch_dirs_and_handle_scripts_too)
    {
        
        cls_tglo.p_thing_list->delete_things_by_type(C_THING_ENT_GENERIC);
        
        //move to the mod dir.  I tack on the full path that way if the mod dir is null
        //move back to the main dir.
        
        char st_temp[256];
        sprintf(st_temp, "%s%s", GetExeDir(), cls_tglo.mod.m_st_path);
        chdir(st_temp);
        
        // log_msg("Moved to %s.", st_temp);
        //we need to kill everything and load the mod
        //reload/setup  a few things
        
        cls_tglo.p_script_engine->InitGlobalScript();
        add_global_functions_to_script_0(); //add the 3d engine specific functions to the scripting engine
    }
    
    
    
    cls_tglo.p_camera->setObjectPosition(D3DXVECTOR3(  3*5, 3.0f ,3*5 ));
    cls_tglo.p_camera->setAngleY(0);
   	//p_manager->
  // cls_tglo.p_manager->delete_all();
	cls_tglo.p_font->ResetFastFonts();
    cls_tglo.p_manager->DeleteAllNonDynamic(C_RESOURCE_TEXTURE);

    cls_tglo.p_manager->load_resource_list("data\\textures.txt");
 
    cls_tglo.p_ent_lib->destroy_all();
    cls_tglo.p_ent_lib->load_category_wildcard("*.ent"); 

    //clear terrain
    cls_tglo.p_map->generate_map(15,15,5,3);
    cls_tglo.p_map->send_map_to_terrain(); //must do this to init everything

}

//if file doesn't exist in mod dir, return the base dir
const char * CDir(const char * p_fname)
{
    static char s_st_fname[256];

    if (exist(p_fname))
    {
        //don't need to change anything, just pass on the filename we got
        return p_fname;
    }

  //file didn't exist, convert and send our alternate dir fname
   sprintf((char*)s_st_fname, "%s%s", GetExeDir(), p_fname);
   return s_st_fname;
}




