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

const int C_CAMERA_MODE_NORMAL = 0;
const int C_CAMERA_MODE_ORTHOGRAPHIC = 1; //overhead view
const int C_CAMERA_MODE_THIRD_PERSON = 2; //floating cam

#include "CVisual.h"
#include "..\\CTimer.h"

class CCamera : public CVisual  
{
public:
	CCamera();
	virtual ~CCamera();


	//accessors.
	
	inline void setUpVector( const D3DVECTOR& vUp ) { m_vUp = vUp; }
	//inline void setRightVector( const D3DVECTOR& vRight ) { m_vRight = vRight; }
	inline void setLookVector( const D3DVECTOR& vLook ) { m_vLook = vLook; }

	inline D3DXVECTOR3 getUpVector() const { return m_vUp; }
//inline D3DXVECTOR3 getRightVector() const { return m_vRight; }
	inline D3DXVECTOR3 getLookVector() const { return m_vLook; }
	  
	inline void setFieldOfView( FLOAT fov ) { m_fFov = fov; }
	inline void setAspectRatio( FLOAT aspect ) { m_fAspect = aspect; }
	inline void setNearClippingDistance( FLOAT fNear ) { m_fNear = fNear; }
	inline void setFarClippingDistance( FLOAT fFar ) { m_fFar = fFar; }

	inline FLOAT getFieldOfView() const { return m_fFov; }
	inline FLOAT getAspectRatio() const { return m_fAspect; }
	inline FLOAT getNearClippingDistance() const { return m_fNear; }
	inline FLOAT getFarClippingDistance() const { return m_fFar; }

	inline void setProjMatrix( const D3DXMATRIX& matProj ) { m_matProj = matProj; }
	inline D3DXMATRIX getProjMatrix() const { return m_matProj; }

	inline void setVelocityVector( const D3DXVECTOR3& vVelocity ) { m_vVelocity = vVelocity; }
	inline D3DXVECTOR3 getVelocityVector() const { return m_vVelocity; }

	inline void setYawVelocity( FLOAT yawVelocity ) { m_fYawVelocity = yawVelocity; }
	inline FLOAT getYawVelocity() const { return m_fYawVelocity; }

	inline void setPitchVelocity( FLOAT pitchVelocity ) { m_fPitchVelocity = pitchVelocity; }
	inline FLOAT getPitchVelocity() const { return m_fPitchVelocity; }

    void SetOldStyleCam(bool b_new){m_b_use_old_style_cam = b_new;}
	//non-accessor Functions
	void lookAt( const D3DXVECTOR3* vEye, const D3DXVECTOR3* vLook, 
				 const D3DXVECTOR3* vUp );

	void perspective( FLOAT fov, FLOAT aspect, FLOAT nearDist, FLOAT farDist );

	void decelerate( FLOAT decelVelocity );
	 void CCamera::set_isometric_view();
	 void CCamera::set_third_person_view(bool b_new);
	 //CCameras movement and rotation functions.
	inline void slideRight  ( FLOAT fSpeed ) { m_vVelocity.x	+= fSpeed; }
	inline void slideLeft   ( FLOAT fSpeed ) { m_vVelocity.x	-= fSpeed; }
	inline void slideUp     ( FLOAT fSpeed ) { m_vVelocity.y	+= fSpeed; }
	inline void slideDown   ( FLOAT fSpeed ) { m_vVelocity.y	-= fSpeed; }
	inline void moveForward ( FLOAT fSpeed ) { m_vVelocity.z	+= fSpeed; }
	inline void moveBackward( FLOAT fSpeed ) { m_vVelocity.z	-= fSpeed; }
	inline void turnRight   ( FLOAT fSpeed ) { m_fYawVelocity	+= fSpeed; }
	inline void turnLeft    ( FLOAT fSpeed ) { m_fYawVelocity   -= fSpeed; }
	inline void turnDown    ( FLOAT fSpeed ) { m_fPitchVelocity += fSpeed; }
	inline void turnUp      ( FLOAT fSpeed ) { m_fPitchVelocity -= fSpeed; }

	void transform( FLOAT fSpeed, FLOAT fAngularSpeed );

	void CCamera::set_iso_zoom(float f_zoom);
	inline float get_iso_zoom(){return f_iso_zoom;};

	void get_screen_to_world_point(D3DXVECTOR3 * vec_out, int x, int y,
										float f_distance);
	virtual void CCamera::think();

	void CCamera::toggle_isometric_view();
	D3DXMATRIX	m_inverse_view; //inverse of view matrix
	int get_camera_mode();
	void camera_third_person_track();
	void get_world_to_screen_point(D3DXVECTOR3 *p_vec_out, D3DXVECTOR3 *p_point,
									D3DXMATRIX *p_matrix_world);

	void SetTargetPosition(D3DXVECTOR3 *p_vec){vec_pos_target = *p_vec;};
	void SetTargetTilt(float f_tilt){m_fAngleX_target = f_tilt;};
	void SetTargetDirection(float f_direction){m_fAngleY_target = f_direction;};
	void TurnOffThirdPersonViewByTimer(int i_time);
    void SetTiltFollowHeightMod(float f_new){m_f_follow_tilt_height_mod = f_new;}
    void SetIgnoreCollisionTimer(int i_time);		
    void SetFollowTilt(bool b_new){m_b_follow_tilt = b_new;}
    void SetMoveLerp(float f_move_lerp) {m_f_interpolation = f_move_lerp;};
    void SetFreeMoveLerp(float f_move_lerp) {m_f_cam_lerp = f_move_lerp;};
    void SetFreeTurnLerp(float f_turn_lerp) {m_f_cam_turn_lerp = f_turn_lerp;};
    void SetSmartFadeActive(bool b_new){b_smart_fade_enabled = b_new;}
    bool GetSmartFadeActive (){return b_smart_fade_enabled;}
    
    void update_matrix();

private:
	D3DXVECTOR3	m_vUp,		//The vertical Vector
				m_vLook;	//The CCameras Look (in the z direction) vector.

	FLOAT		m_fFov,		//Width of the vertical field-of-view in radians.
				m_fAspect,	//Aspect ratio of the CCamera's angle of view. 
				m_fNear,	//The distance to the near clipping plane.
				m_fFar;		//The distance to the far clipping plane.

	D3DXVECTOR3	m_vVelocity;
	FLOAT		m_fYawVelocity;
	FLOAT		m_fPitchVelocity;
	float f_inverse_angle;
	D3DXVECTOR3 vec_inverse;
	D3DXMATRIX	m_matProj;	//The projection matrix.
	D3DXMATRIX	m_matOrientation; //The orientation matrix.
	bool b_third_person_view_active;

	float f_dir_target;
	D3DXVECTOR3 vec_pos_target;
	//for isometric view
	float f_iso_zoom;
	D3DXVECTOR3 vec_iso_position_save, vec_position_save, vec_angle_save;
	bool b_iso_view_active;
	float m_fAngleX_target, m_fAngleY_target;
	float m_f_interpolation;
    float m_f_turn_interpolation;
	float m_f_cam_lerp;
	float m_f_cam_turn_lerp;
	float m_f_cam_look_turn_lerp; //sometimes we only look at an object but we don't
	CTimer m_timer_turn_off_third;
	bool m_b_timer_third_active;
    float m_f_follow_tilt_height_mod;
    bool m_b_ignoring_collision_timer_on;
    CTimer m_timer_col_ignore; //ignore collision for a certain amount of time
    //want to screw up our regular lerp settings
    bool m_b_follow_tilt; //if true, will tilt up and down to look at target exactly
    CTimer m_timer_update_3d_position; 
    bool m_b_use_old_style_cam; //needed for the editor style cameras
	void reset();
    bool b_smart_fade_enabled; //if false, ignore smart alpha commands and draw opaque
    D3DXVECTOR3 m_vec_last_position; //used for figuring out velocity for the sound engine
    bool m_b_reset_velocity; //to stop large jumps of velocity


};

void ScreenToWorld(float mouse_x, float mouse_y, float d, float fov,
D3DXMATRIX matViewInverse,
                   float screen_w, float screen_h, D3DXVECTOR3 & out );
