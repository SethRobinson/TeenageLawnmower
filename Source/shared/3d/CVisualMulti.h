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

#include "CVisual.h"
#include "..\\CTimer.h"
class CAnimInfo;

const int C_COLLISION_SPHERICAL = 0;
const int C_COLLISION_BOX = 1;


const int C_VISUAL_ALPHA_SORT_UNKNOWN = 0;
const int C_VISUAL_ALPHA_SORT_YES = 1;
const int C_VISUAL_ALPHA_SORT_NO = 2;

enum
{
  C_RES_ADJUST_NONE = 0,
  C_RES_ADJUST_CENTER_BOTTOM,                                                 
  C_RES_ADJUST_NORMAL, //keep same proportions, but as if it was centered     
  C_RES_ADJUST_FULL, //remap completely to new space                            
  C_RES_ADJUST_FULL_NO_SCALE, //remap completely to new space, but don't scale the object                            

  //only add new types above this
  C_RES_ADJUST_COUNT
                                                                                       
}; 


const int C_VISUAL_SHADOW_NONE = 0;
const int C_VISUAL_SHADOW_ACCURATE = 1; //drop the Z and render again basically          
const int C_VISUAL_SHADOW_FAKE = 2; //draw a little circle under them, faster



class CCalControl; //forward
class CMessageRayPick; //forward
class CResourceTex;
class CVisualMulti : public CVisual
{
public:
    
    CVisualMulti();
    virtual ~CVisualMulti();
    
    virtual void dx_draw();
    virtual void dx_create();
    virtual void dx_restore();
    
    bool init_visual(char cst_name[256], int i_resource_type); //load a resource, any kind
    bool init_visual_sphere(char *st_name, float x_scale, 
        float y_scale, float z_scale, UINT num_segments, UINT num_rings, 
        float tex_scale, bool hemisphere, D3DCOLOR dcolor, char *p_st_tex );
    bool init_visual_sprite(const char p_st_name[], const char p_st_texture[]);
    bool init_visual_cal_instance(char p_cal_name[]);
    bool init_visual_sprite_blank(char p_st_name[], D3DXVECTOR2 pt_size, DWORD dw_color); //create a blank graphic
    void VertexColor(DWORD dw_color);
    void update_2d_matrix();
    void set_xy(int i_x, int i_y);
    void set_xy(float f_x, float f_y);
    void ReplaceTexture(char *p_st_texture_set, char *p_st_old, char *p_st_new);
    void UpdateSphereTexture(char *st_new);
    void SetSmartFade(float f_min_mod, float f_max_mod);
    void SetColorFadeTo(float f_r, float f_g, float f_b, float f_a, float f_speed);
    void SetShadowMode(int i_mode);
    int GetShadowMode(){return m_i_shadow_mode;}
    void BuildShadowMatrix();
    D3DXMATRIX * GetShadowMatrix(){return &m_mat_shadow;}
    
    void SetBlink(bool b_active);
    void ProcessBlink();
    void SetCollisionType(int i_new){e_collision_type = i_new;}
    
    char st_filename[256]; 
    bool b_lod_active; //if false, always draw full quality
    int m_i_visual_id;
    int m_i_visual_type;
    int m_i_lod;
    float f_height;
    int e_collision_type; //how we compute the bottom, sides and top
    float m_f_radius; //radius of object, used for collision detection
    bool b_follow_ground_tilt; //if true, we rotate to the grounds normal
    int m_i_draw_mode; //a billboarded sprite or what?
    
    CCalControl *p_cal; //used only for skeletal animation
    bool b_drag_active;
    bool b_is_cpic; //sometimes we perform different operations on Cpics
    bool b_clickable; //if yes, will respond to a raycheck
    bool b_got_clicked;
    bool b_update_matrix;
    bool b_floating;
    bool got_clicked() {return b_got_clicked;};
    
    void set_clicked(bool b_new){b_got_clicked = b_new;};
    void set_clickable(bool b_new){b_clickable = b_new;};
    
    void BuildRadius(float f_mod); //refigure the radius with a modifer (for scaling)
    virtual void setTranslate();
    bool DistanceCulled();
    CResourceTex * GetSpriteTex();	
    void set_follow_ground_tilt(bool b_new){b_follow_ground_tilt = b_new;};
    void ray_pick(CMessageRayPick *p_msg);
    void set_drag_mode(bool b_new);
    void process_drag();
    void CheckForAlphaSort();
    float GetRadius(){return m_f_radius;};
    float GetRadiusDevided(){return m_f_radius /2;}; //a devide by 2 will be optimized by any good compiler...
    void SetMipMap(int i_mip_map){m_i_mip_map = i_mip_map;};
    DWORD GetVertexColor(){return m_dw_vertex_color;};
    void SetIgnoreKillMessage(bool b_ignore_kill_message){m_b_ignore_kill_message = b_ignore_kill_message;};
    bool GetIgnoreKillMessage(){return m_b_ignore_kill_message;}
    void SetResAdjust(int i_type, int i_base_x, int i_base_y);
    void alpha_sort_smart_fade();
    void SetAnimTexture(char *st_name);
    void SetAnimByName(char *st_name, int i_frame);
    void SetAnimByIndex(int i_anim, int i_frame);
    void SetAnimPause(bool b_pause);
    void * GetResourcePointer();
    void ReleaseResourceIfWeHaveOne(); //call before using init_visual_sprite();
    void SetSpeedMod(float f_speed_mod);
    void SetRadiusMod(float f_new) {m_f_radius_mod = f_new; BuildRadius(f_scale_x);};
    
    void SetSphereTreeMod(float f_new) 
    {
        m_f_sphere_tree_mod = f_new;
        if (GetSpherePack())
            GetSpherePack()->SetRadius(m_f_radius*GetSphereTreeMod());
    }
    
    float GetSphereTreeMod(){return m_f_sphere_tree_mod;}
    void SetAlphaSort(int i_new) {m_i_alpha_sort = i_new;}
    int GetAlphaSort(){return m_i_alpha_sort;}
    
protected:
    
    void ExtractColorsFromVertexColor();


    D3DXMATRIX m_mat_shadow; //matrix just for the shadow, if we're using one.
    int m_i_shadow_mode; //do they want us to draw a shadow too?
    int m_i_alpha_sort;
    bool m_b_ignore_kill_message;
    int m_i_mip_map;
    DWORD m_dw_vertex_color;
    int m_i_texture_set;
    int m_i_res_adjust; //if not C_VISUAL_RES_ADJUST_NONE then we need to correct for res changes
    int m_i_res_correct_base_x;
    int m_i_res_correct_base_y;
    float m_f_res_adjust_save_x; //if a screen resize happens, this is used to rebuild the location
    float m_f_res_adjust_save_y; //if a screen resize happens, this is used to rebuild the location
    bool m_b_smart_fade; //indicates we want to use special alpha fading techniques
    float 	m_f_smart_fade_min; //a way to modify when the fades take place
    float 	m_f_smart_fade_max;
    CAnimInfo *m_anim_info; //only used for md2 and rtm's
    void *m_p_resource; //cache the pointer to the resource we're using.  This will require a typcast to use it.. sorry.
    float f_ground_height; //the ground's Y right beneath us
    CTimer m_timer_blink;
    DWORD m_dw_blink_color;
    bool m_b_blink_active;
    bool m_b_blink_toggle; //helps us keep track of the blink state for when blink is active
    float m_f_radius_mod; //usually 1, mod to overall radius used in collision detection
    float m_f_sphere_tree_mod;
    float m_f_fade_speed; //0 means not used
  
    float   m_f_fade_target_r;
    float   m_f_fade_target_g;
    float   m_f_fade_target_b;
    float   m_f_fade_target_a;

    float        m_f_color_r;
    float        m_f_color_g;
    float        m_f_color_b;
    float        m_f_color_a;
    float m_f_res_adjust_scale_x, m_f_res_adjust_scale_y;
 
};
