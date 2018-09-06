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
#pragma warning (disable:4786)

#include "CThing.h"
#include "CThing_globals.h"
#include "..\\std_all.h"
#include "math.h"

#include <vector>


//this class can't actually be used, but you should derive from it real objects

class CVisual : public CThing
{
public:
    inline void setObjectPosition( const D3DXVECTOR3& vPos ) { m_vObjectPosition = vPos; set_translate_asap(true);}

	inline D3DXVECTOR3 getObjectPosition() const { return m_vObjectPosition; }

	inline void setObjectMatrix( D3DXMATRIX mat ) { m_objectMatrix = mat; }
	inline D3DXMATRIX getObjectMatrix() const { return m_objectMatrix; }

	inline void setAngleX( FLOAT angleX ) { m_fAngleX = angleX; set_translate_asap(true);}
	inline void setAngleY( FLOAT angleY ) { m_fAngleY = angleY;set_translate_asap(true); }
	inline void setAngleZ( FLOAT angleZ ) { m_fAngleZ = angleZ; set_translate_asap(true);}
	inline void set_pos_x(float f_new){m_vObjectPosition.x = f_new;set_translate_asap(true);};
	inline void set_pos_y(float f_new){m_vObjectPosition.y = f_new;set_translate_asap(true);};
	inline void set_pos_z(float f_new){m_vObjectPosition.z = f_new;set_translate_asap(true);};

	inline float get_pos_x(){return m_vObjectPosition.x;};
	inline float get_pos_y(){return m_vObjectPosition.y;};
	inline float get_pos_z(){return m_vObjectPosition.z;};
	void setup_billboard_matrix();
	void setup_billboard_matrix_simple();

	inline float get_tilt(){return f_tilt;};
	inline void set_tilt(float f_new){f_tilt = f_new;set_translate_asap(true);};
	inline float get_roll(){return f_roll;};
	inline void set_roll(float f_new){f_roll = f_new;};
	inline FLOAT getAngleX() const { return m_fAngleX; }
	inline FLOAT getAngleY() const { return m_fAngleY; }
	inline FLOAT getAngleZ() const { return m_fAngleZ; }
	void set_world_transform() { cls_tglo.p_d3dDevice->SetTransform( D3DTS_WORLD, &m_objectMatrix);};
	float get_dir_in_rad(){return f_dir;};
	void set_dir_in_rad(float f_new){f_dir = f_new;}; //use set_dir for ents to do gradual change
    void set_no_lighting(bool b_new){b_no_lighting = b_new;};
    virtual void set_scale(float f_x, float f_y, float f_z);
    inline float get_scale_x(){return  f_scale_x;};
    inline float get_scale_y(){return  f_scale_y;};
    inline float get_scale_z(){return  f_scale_z;};
    
    void translateMovement();
    void set_visual_follow(CVisual *p_follow);
    CThing* get_visual_follow() { return p_visual_follow;};
    void SetVisualLook(CVisual *p_look){p_visual_look_at = p_look;}
    CThing* GetVisualLook() { return p_visual_look_at;};
    inline void set_fill_mode(int i_new){i_fill_mode = i_new;};
    inline int get_fill_mode(){return i_fill_mode;};
    inline void set_translate_asap(bool b_new){b_translate_asap = b_new;};
    void set_visual_follow_offset(D3DXVECTOR3 vec_new);
    virtual void dx_draw();
    virtual void think();
    virtual void dx_restore();
    virtual void dx_create();
    virtual void dx_kill();
    virtual void dx_invalidate();
    virtual void dx_draw_prepare();
    void set_scale_uniform(float f_scale);
    void set_no_draw(bool b_new){b_no_render_next_frame = b_new;};
    bool get_no_draw(){return b_no_render_next_frame;};
    void alpha_sort();
    void RotateBaseFrameYX(float f_rotate_y_rad, float f_rotate_x_rad);
    void DontLookAtMe(CThing *p_thing)  ;
    void SetZDisable(bool b_new){m_b_zdisable = b_new;}
    bool GetZDisable(){return m_b_zdisable;}
    
    D3DXVECTOR3				m_vObjectPosition; // the objects position in 3d space.
    D3DXVECTOR3             vec_follow_offset;
    D3DXMATRIX				m_objectMatrix; //the objects matrix.
    D3DXMATRIX				m_matTrans;     //the objects translation matrix.
   	D3DXMATRIX				m_matRotX, m_matRotY, m_matRotZ, m_mat_base;
    void SetCentering(int i_new);
    void SetForceDoubleSidedTextures(bool b_new) {m_b_force_double_sided_texture = b_new;}
    bool GetForceDoubleSidedTextures(){return m_b_force_double_sided_texture;}

protected:
 
    virtual void setTranslate();
    CVisual *p_visual_follow;
	CVisual *p_visual_look_at;
	bool b_no_lighting;
	int i_fill_mode;
	bool b_translate_asap;
	float f_scale_x;
	float f_scale_y;
	float f_scale_z;
    float f_dir; //the direction we are pointing in rads
    float f_dir_goal;  //allow smooth turns
    bool m_b_zdisable; //Zcheck?  if no, we'll draw over anything.
	bool b_no_render_next_frame;
	int e_centering; 

	FLOAT					m_fAngleX;
	FLOAT					m_fAngleY;
	FLOAT					m_fAngleZ;

	float f_tilt;
	float f_roll;

    bool m_b_skip_draw_of_next_frame; 
    bool m_b_force_double_sided_texture;
  
    
    CVisual();
	virtual ~CVisual();

};