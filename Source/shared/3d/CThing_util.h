/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#pragma once

#include "..\\all.h"
#include <D3DX8.h>

struct D3DXVECTOR3;

class CThing; //foward for speed
class CThingList; //forward for speed;

char * print_vector(D3DXVECTOR3 vec);
BOOL IntersectTriangle( D3DXVECTOR3 * p_vec_out,  const D3DXVECTOR3& orig,
                                       const D3DXVECTOR3& dir, D3DXVECTOR3& v0,
                                       D3DXVECTOR3& v1 , D3DXVECTOR3& v2/*,
                                       FLOAT* t, FLOAT* u, FLOAT* v */);
float get_theta_from_x_and_z(float f_x, float f_z);
void get_x_and_z_from_radian(D3DXVECTOR3 *p_vec, float f_angle);
void set_float_with_target(float *p_float, float f_target, float f_friction);
float Fast_Distance_3D(float fx, float fy, float fz);
int Fast_Distance_2D(int x, int y);
bool thetas_are_facing_same_way(float f_theta1, float f_theta2, float f_epsilon);

float lerp_float(float f_origin, float f_target, float f_percent);
float lerp_float_direction(float f_origin, float f_target, float f_percent);
void lerp_vec3(D3DXVECTOR3 *vec_out, D3DXVECTOR3 *vec_origin, D3DXVECTOR3 *vec_target, float f_percent);
const char * CDir(const char * p_fname);



static inline bool AABBIntersect(const D3DXVECTOR3 &Min1, const D3DXVECTOR3 &Max1, const D3DXVECTOR3 &Min2, const D3DXVECTOR3 &Max2)
 {
 	if (Max1.x < Min2.x || Max1.y < Min2.y || Max1.z < Min2.z) return false;
 	if (Max2.x < Min1.x || Max2.y < Min1.y || Max2.z < Min1.z) return false;
 	return true;
 }


// used for swapping algorithm
#define SWAP(a,b,t) {t=a; a=b; b=t;}

#define SETH_PI_OVER2    ((FLOAT)  1.570796327f)

inline float Slow_Distance_2D(float x, float y)
{
		 return (float)sqrt(x*x + y*y);
}

inline DWORD GetAlphaFromD3DCOLOR(D3DCOLOR dw_color)
{
    return ((dw_color&0xff000000)>>24);
}

inline DWORD GetRFromD3DCOLOR(D3DCOLOR dw_color)
{
    return ((dw_color&0x00ff0000)>>16);
}

inline DWORD GetGFromD3DCOLOR(D3DCOLOR dw_color)
{
    return ((dw_color&0x0000ff00)>>8);
}

inline DWORD GetBFromD3DCOLOR(D3DCOLOR dw_color)
{
    return ((dw_color&0x000000ff));
}

extern D3DXVECTOR3 g_vec_up;
extern D3DXVECTOR3 g_vec_down;
extern D3DXVECTOR3 g_vec_null;
CThing * ray_pick_objects(CThingList *p_thing_list, D3DXVECTOR3 *p_vec_point_a, D3DXVECTOR3 * p_vec_point_b, int i_type, float *p_f_dist, float f_range, D3DXVECTOR3 *p_vec_hit,  D3DXVECTOR2 *vec_mouse);
bool get_3d_mouse_position(D3DXVECTOR3 *p_vec);
char * print_point(D3DXVECTOR2 *vec);
void SetupPixelFog(DWORD Color, DWORD Mode, float Start, float End);
float lerp_float_direction_test(float f_origin, float f_target, float f_percent);
void switch_to_top_view_cam();
void switch_to_car_cam();
void switch_to_player_walk_cam();
void switch_to_play_cam();
void switch_to_free_cam();
void set_reset_light_and_fog();
void set_random_fog();
void set_random_sun();
void set_landscape_wire_frame_toggle();
void init_engine_from_base_path(bool b_switch_dirs_and_handle_scripts_too);
void ShutDownScript(int & i_script);