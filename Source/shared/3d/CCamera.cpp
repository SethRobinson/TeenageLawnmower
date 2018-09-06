/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */

//Based off a camera class by Neil Harran

#include "CCamera.h"
#include "CThing_util.h"
#include "d3dapp.h"	   
#include "CMap.h"
#include "CTerrain.h"
#include "..\\sound\\ISoundManager.h"
	   
#include "ent_utils.h"
#include "CVisualMulti.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCamera::CCamera()
{
    reset();
    set_sort(30000);
    set_thing_name("Main Camera");
    set_type(C_THING_CAMERA);
    SetSmartFadeActive(true);
    m_vec_last_position = g_vec_null;
}

CCamera::~CCamera()
{
    cls_tglo.p_camera = NULL; //let the global know we don't exist anymore
    
}
void CCamera::reset()
{
    f_dir_target = 0;
    CVisual::setAngleY( 0.5f );
    CVisual::setAngleZ( 0.03f );
    m_vLook =  g_vec_null;
    m_vUp = D3DXVECTOR3(0,1,0);
    m_fFov = m_fAspect = m_fNear = m_fFar = 0.0f;
    D3DXMatrixIdentity( &m_matProj );
    D3DXMatrixTranslation( &m_matOrientation, 0.0f, 0.0f, 0.0f );
    b_iso_view_active = false;
    m_b_use_old_style_cam = false;
    
    m_vVelocity		  = g_vec_null;
    m_fYawVelocity = 0;
    m_fPitchVelocity = 0;
    
    //set some defaults
    f_iso_zoom = 30; //default zoom level
    vec_iso_position_save = D3DXVECTOR3(1,200,1); //50 units above the map, lower left corner
    this->vec_angle_save = g_vec_null;
    this->vec_position_save = g_vec_null;
    b_third_person_view_active = false;	 //only applicable if tracking someone
    m_f_interpolation = 6;
    m_f_turn_interpolation = 6;
    m_f_cam_look_turn_lerp = 1;
    m_f_cam_lerp = 1;
    m_f_cam_turn_lerp = 2;
    m_f_follow_tilt_height_mod = 1.5f; //make higher to look higher up at the thing it's looking at
    m_b_ignoring_collision_timer_on = false;
    m_timer_update_3d_position.set_interval(1); //10 times a second
    vec_pos_target = g_vec_null;
    m_b_reset_velocity = true;
     m_fAngleX_target = m_fAngleY_target = 0;
}

int CCamera::get_camera_mode()
{
    if (this->b_iso_view_active) return C_CAMERA_MODE_ORTHOGRAPHIC;
    if (this->b_third_person_view_active) return C_CAMERA_MODE_THIRD_PERSON;
    
    //default
    return C_CAMERA_MODE_NORMAL; 
}

void CCamera::TurnOffThirdPersonViewByTimer(int i_time)
{
    if (i_time == 0)
    {
        //disable it
        m_b_timer_third_active = false;
        return;
    }
    
    m_b_timer_third_active = true;
    m_timer_turn_off_third.set_interval(i_time);
}
void CCamera::SetIgnoreCollisionTimer(int i_time)
{
    m_b_ignoring_collision_timer_on = true;
    m_timer_col_ignore.set_interval(i_time);
}


void CCamera::camera_third_person_track()
{
    
    bool b_on_retry = false;
    
    //code to actually set the tilt by looking
    
    f_dir = f_dir_target;
    
    D3DXVECTOR3 vec_old_position = m_vObjectPosition; //save this in case we have to go back
    vec_pos_target.x += vec_follow_offset.x*float(sin(f_dir));
    vec_pos_target.z += vec_follow_offset.x*float(cos(f_dir));
    vec_pos_target.y += vec_follow_offset.y;
    
    float m_f_ground_padding = 0.25f;
     if (vec_follow_offset.y > 1) m_f_ground_padding = 1;
    
    //don't let the camera dip too close to the ground
    float f_ground = cls_tglo.p_terrain->get_height(vec_pos_target.x, vec_pos_target.z);
    float f_roof =cls_tglo.p_map->GetCeilingHeight(vec_pos_target.x, vec_pos_target.z);
    if (vec_pos_target.y < f_ground+m_f_ground_padding) vec_pos_target.y = f_ground+m_f_ground_padding;
    if (vec_pos_target.y > f_roof) vec_pos_target.y = f_roof-0.5f;
   	//compute a good position
    D3DXVECTOR3 vec_frame_target; //goal of this frame
    lerp_vec3(&vec_frame_target,&vec_old_position, &vec_pos_target, m_f_interpolation*cls_tglo.time());
    vec_frame_target = ( vec_frame_target - vec_old_position); //the displacement we want
    
    
    float f_desired_distance;
    //first get the distance that it's supposed to be
    if (p_visual_look_at)
        f_desired_distance = D3DXVec3Length(&D3DXVECTOR3(p_visual_look_at->m_vObjectPosition-vec_pos_target));
    
    
    //is this really a good position or not?  Let's  test it.
    //process only one change at a time, this stops 
    float f_hold_x = vec_frame_target.x; 
    vec_frame_target.x = 0;
    
    //it was converted to a good position I guess.
    m_vObjectPosition.y += vec_frame_target.y;
    
    if (m_b_ignoring_collision_timer_on)
    {
        if (m_timer_col_ignore.interval_reached())
        {
            m_b_ignoring_collision_timer_on = false;
        }
    }
    
    if (!m_b_ignoring_collision_timer_on)
        ValidateCollisionCamera(&vec_frame_target,&m_vObjectPosition, 0.01f);
    
    //it was converted to a good position I guess.
    m_vObjectPosition.z += vec_frame_target.z;
    
    
    //now for the other one
    vec_frame_target.x = f_hold_x;
    vec_frame_target.z = 0;
    if (!m_b_ignoring_collision_timer_on)
        ValidateCollisionCamera(&vec_frame_target,&m_vObjectPosition, 0.01f);
    
    
    
    //it was converted to a good position I guess.
    m_vObjectPosition.x += vec_frame_target.x;
    
    //don't let the camera dip under the ground, possible because of the interpolation
    f_ground = cls_tglo.p_terrain->get_height(m_vObjectPosition.x, m_vObjectPosition.z);
    
    if ( m_vObjectPosition.y<  f_ground+m_f_ground_padding) m_vObjectPosition.y =  f_ground+m_f_ground_padding;
    
    
    float f_distance_current;
    
    
    if (p_visual_look_at)
    {
        
        f_distance_current = D3DXVec3Length(&D3DXVECTOR3(p_visual_look_at->m_vObjectPosition-m_vObjectPosition));
        if ( f_distance_current / f_desired_distance > 1.3)
        {
            
            //we hit something hard, signal a period to begin where we don't collision check anything walls (only ground)
            if (!m_b_ignoring_collision_timer_on)
            {
                SetIgnoreCollisionTimer(1000);
                //log_msg("Catch it up!");                      
            }
        }
        
    }
    
    
    if (p_visual_look_at)
    {
        //special hack to look somewhere while we're actually moving to/with someone else
        m_fAngleY_target  = get_theta_from_x_and_z(p_visual_look_at->get_pos_x()-get_pos_x(),
            p_visual_look_at->get_pos_z()-get_pos_z() );
        
        //m_fAngleY = lerp_float_direction(m_fAngleY, m_fAngleY_target, 70*cls_tglo.time());
        
        m_fAngleY = m_fAngleY_target;
    }	else
    {
        m_fAngleY = lerp_float_direction(m_fAngleY, m_fAngleY_target, m_f_turn_interpolation*cls_tglo.time());
        
    }
    
    
    
    if (p_visual_look_at)
    {
        //special hack to look somewhere while we're actually moving to/with someone else
        
        if (m_b_follow_tilt)           
        {
            vec_follow_offset.z  = -get_theta_from_x_and_z( (p_visual_look_at->get_pos_y() + 
                (((CVisualMulti*)p_visual_look_at)->GetRadius()*m_f_follow_tilt_height_mod))
                -get_pos_y(),
                f_distance_current) ;
            
            setAngleX(lerp_float_direction(getAngleX(), vec_follow_offset.z, (m_f_interpolation*2)*cls_tglo.time()));
            
            
            
        } else
        {
            setAngleX(lerp_float_direction(getAngleX(), vec_follow_offset.z, (m_f_interpolation*2)*cls_tglo.time()));
            
        }
          
    } else
    {
        setAngleX(lerp_float_direction(getAngleX(), vec_follow_offset.z, (m_f_interpolation*2)*cls_tglo.time()));
    }
     
    m_fAngleX_target = m_fAngleX; //save it in case we switch to our normal cam
   
    D3DXMatrixIdentity(&m_objectMatrix);
    D3DXMatrixRotationX( &m_matRotX, getAngleX());
    D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matRotX  );
    
    //performs the appropriate matrix rotation operations.
    D3DXMatrixRotationY( &m_matRotY, getAngleY() );
    D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matRotY  );
    
    //move the cam behind the object
    D3DXMatrixTranslation(&m_matTrans, m_vObjectPosition.x, m_vObjectPosition.y,
						  m_vObjectPosition.z);
    D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matTrans );
    
}

void CCamera::think()
{
    if (p_visual_follow)
    {
        //camera is attached to a visual
        
        if (b_third_person_view_active)
        {
            vec_pos_target = p_visual_follow->getObjectPosition();
            
            m_fAngleY_target = p_visual_follow->getAngleY()+D3DX_PI;
            this->setAngleZ(p_visual_follow->getAngleZ());
            this->set_tilt(p_visual_follow->get_tilt());
            this->f_dir_target = p_visual_follow->get_dir_in_rad();
            
            camera_third_person_track();
            m_inverse_view = m_objectMatrix; //save inverse
            D3DXMatrixInverse( &m_objectMatrix, NULL, &m_objectMatrix);
            cls_tglo.p_d3dDevice->SetTransform( D3DTS_VIEW, &m_objectMatrix );
            
            if (m_b_timer_third_active)
            {
                if (m_timer_turn_off_third.interval_reached())
                {
                    set_third_person_view(false);
                }
            }
            
            goto continue_think;
        }
        
        this->setObjectPosition(p_visual_follow->getObjectPosition());
        this->setAngleY(p_visual_follow->getAngleY()+D3DX_PI);
        this->setAngleZ(p_visual_follow->getAngleZ());
        this->set_tilt(p_visual_follow->get_tilt());
        
        this->m_vObjectPosition.y += vec_follow_offset.y; 
        
        this->f_dir = p_visual_follow->get_dir_in_rad();
        
        this->translateMovement();
        m_inverse_view = m_objectMatrix; //save inverse
        D3DXMatrixInverse( &m_objectMatrix, NULL, &m_objectMatrix);
        cls_tglo.p_d3dDevice->SetTransform( D3DTS_VIEW, &m_objectMatrix );
    }	else
        
    {
        //not following anything
        
        if ( (cls_tglo.GetProgram() == C_GLOBAL_EDITOR) || m_b_use_old_style_cam) 
        {
            f_dir = getAngleY();
            goto continue_think; //only run this code for the game
        }
        
        //basically, there are two separate movement systems.  the one that I clipped in the beginning
        //which the editor is based on and the more powerful one I added later with built in LERPing.
        //this complicates things but I really don't want to redo the camera controls of the editor...
        
        
        //lerp between target and current position
        
        if (p_visual_look_at)
        {
            //special hack to look somewhere while we're actually moving to/with someone else
            m_fAngleY_target  = get_theta_from_x_and_z(p_visual_look_at->get_pos_x()-get_pos_x(),
                p_visual_look_at->get_pos_z()-get_pos_z() );
            
            m_fAngleY = lerp_float_direction(m_fAngleY, m_fAngleY_target, m_f_cam_look_turn_lerp*cls_tglo.time());
        }	else
        {
            m_fAngleY = lerp_float_direction(m_fAngleY, m_fAngleY_target, m_f_cam_turn_lerp*cls_tglo.time());
        }
        
        
        setAngleX(lerp_float(getAngleX(), m_fAngleX_target, m_f_cam_lerp*cls_tglo.time()));
        //computer a good position
        lerp_vec3(&m_vObjectPosition,&m_vObjectPosition, &vec_pos_target, m_f_cam_lerp*cls_tglo.time());
        
        
        D3DXMatrixIdentity(&m_objectMatrix);
        D3DXMatrixRotationX( &m_matRotX, getAngleX());
        D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matRotX  );
        
        //performs the appropriate matrix rotation operations.
        D3DXMatrixRotationY( &m_matRotY, getAngleY() );
        D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matRotY  );
        
        //move the cam behind the object
        D3DXMatrixTranslation(&m_matTrans, m_vObjectPosition.x, m_vObjectPosition.y,
            m_vObjectPosition.z);
        D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matTrans );
        m_inverse_view = m_objectMatrix; //save inverse
        D3DXMatrixInverse( &m_objectMatrix, NULL, &m_objectMatrix);
        cls_tglo.p_d3dDevice->SetTransform( D3DTS_VIEW, &m_objectMatrix );
     }
    
continue_think:
    
    
    if (cls_tglo.p_sound)
    {
        if (m_timer_update_3d_position.interval_reached())
        {
            
            //update camera's listening position
            D3DXVECTOR3 vec_facing;
            float f_facing_direction;
            
            f_facing_direction = cls_tglo.p_camera->getAngleY();
            
            vec_facing.x = float(sin(f_facing_direction));  
            vec_facing.y = 0;
            vec_facing.z = float(cos(f_facing_direction));
            
            // if (m_b_reset_velocity)
            D3DXVECTOR3 vec_velocity;
            if (0)
            {
                m_b_reset_velocity = false;
                cls_tglo.p_sound->ListenerSetPosVelAndOrientation(&m_vObjectPosition[0], NULL, &vec_facing[0]);
            } else
            {        
                // vec_velocity = ((m_vObjectPosition - m_vec_last_position) * 0.25f) / cls_tglo.time();
                vec_velocity = ((m_vObjectPosition - m_vec_last_position) / cls_tglo.time()) * 0.1f;
                cls_tglo.p_sound->ListenerSetPosVelAndOrientation(&m_vObjectPosition[0],&vec_velocity[0], &vec_facing[0]);
            }
        
            //   LogMsg("velocity: %s", print_vector(vec_velocity));
            //   LogMsg("last pos: %s", print_vector(m_vec_last_position));
            //   LogMsg("cur pos: %s", print_vector(m_vObjectPosition));

            m_vec_last_position = m_vObjectPosition; //save for the next loop
        }
    }
    
}

void CCamera::update_matrix()
{
    static D3DXQUATERNION qR;
    D3DXQuaternionRotationYawPitchRoll( &qR, m_fAngleZ, m_fAngleY, 0.0f );
    D3DXMatrixAffineTransformation( &m_matOrientation, 1, NULL, &qR, &m_vObjectPosition );
    m_inverse_view = m_matOrientation; //save inverse
    D3DXMatrixInverse( &m_objectMatrix, NULL, &m_matOrientation );
    
    if (cls_tglo.p_d3dDevice)
    {
        cls_tglo.p_d3dDevice->SetTransform( D3DTS_VIEW, &m_objectMatrix );
    }
    
}


/******************************************************************************
Name:	lookAt
Desc:	Just sets the orientation parameters for the CCamera.
Param1: const D3DXVECTOR3* vEye  - The CCameras position in 3d space.
Param2: const D3DXVECTOR3* vLook - The position the CCamera is looking at.
Param3: const D3DXVECTOR3* vUp   - The CCameras up vector.
History:
*****************************************************************************/
void CCamera::lookAt( const D3DXVECTOR3* vEye, const D3DXVECTOR3* vLook, 
                     const D3DXVECTOR3* vUp )
{
    setObjectPosition( *vEye );
    setLookVector( *vLook );
    setUpVector( *vUp );
}

void CCamera::get_screen_to_world_point(D3DXVECTOR3 * vec_out, int x, int y,
                                        float f_distance)
{
    
    if (this->b_iso_view_active)
    {
        //get the ratio from 0
        D3DXVECTOR3 vec_temp;
        //apply mods
        
        *vec_out = m_vObjectPosition; //what we start with
        
        float f_width = float(cls_tglo.p_d3dapp->m_d3dsdBackBuffer.Width) / 2.0f;
        vec_temp.x = float(x)-f_width;
        vec_temp.x /= f_width;
        vec_out->x += ((m_fAspect * vec_temp.x)/2.0f)*f_iso_zoom;
        
        float f_height = float(cls_tglo.p_d3dapp->m_d3dsdBackBuffer.Height) / 2.0f;
        vec_temp.z = float(y)-f_height;
        vec_temp.z /= f_height;
        vec_out->z += -(((vec_temp.z)/2.0f)*f_iso_zoom);
        vec_out->y = vec_iso_position_save.y-f_distance;
        //	log_msg("Final click cords are %s.", print_vector(*vec_out));
        return;
    }
    
    
    ScreenToWorld(float(x), float(y), f_distance, this->m_fFov, this->m_inverse_view, 
        float(cls_tglo.p_d3dapp->m_d3dsdBackBuffer.Width),float(cls_tglo.p_d3dapp->m_d3dsdBackBuffer.Height),
        *vec_out);
    
    
}

void CCamera::set_iso_zoom(float f_zoom)
{
    f_iso_zoom = f_zoom;
    float f_width = f_iso_zoom * m_fAspect;
    float f_height = f_iso_zoom;
    D3DXMatrixIdentity( &m_matProj );
    D3DXMatrixTranslation( &m_matOrientation, 0.0f, 0.0f, 0.0f );
    
    D3DXMatrixOrthoLH(&m_matProj, f_width,
        f_height, this->m_fNear, this->m_fFar );
    cls_tglo.p_d3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProj );
    
}

void CCamera::set_third_person_view(bool b_new)
{
    this->b_third_person_view_active = b_new;
    
    //disable any switches we had in the pipeline
    
    TurnOffThirdPersonViewByTimer(0); //disable it
    
    if (b_third_person_view_active)
    {
        
        if (this->p_visual_follow == NULL)
        {
            log_error("Third person isn't going to work without a follow target.");
            
        }
    }
    
}


void CCamera::set_isometric_view()
{
     //stop all movement, could throw off our orientation
    m_vVelocity		  = D3DXVECTOR3(0,0,0);
    m_fYawVelocity = 0;
    m_fPitchVelocity = 0;
    setAngleY(D3DX_PI/2);
    setAngleX(0);
    setAngleZ(0); //look down
    perspective(m_fFov, m_fAspect, m_fNear, m_fFar);
    
    set_iso_zoom(f_iso_zoom);
    
}  

void CCamera::toggle_isometric_view()
{
    if (b_iso_view_active)
    {
        //save iso view position, ignore height, we use zoom instead.
        vec_iso_position_save.x = getObjectPosition().x;
        vec_iso_position_save.z = getObjectPosition().z;
        
        //copy back old settings
        setAngleX(vec_angle_save.x);
        setAngleY(vec_angle_save.y);
        setAngleZ(vec_angle_save.z);
        setObjectPosition(vec_position_save);
        
        perspective(m_fFov, m_fAspect, m_fNear, m_fFar);
        b_iso_view_active = false;
        
    } else
    {
        //switch to iso view
        //save old settings
        vec_angle_save.x = getAngleX();
        vec_angle_save.y = getAngleY();
        vec_angle_save.z = getAngleZ();
        vec_position_save = this->getObjectPosition();
        
        //copy back old iso settings
        setObjectPosition(vec_iso_position_save);
        log_msg("Iso pos: %s",print_vector(vec_iso_position_save));
        set_isometric_view();
        b_iso_view_active = true;
    }
    
}




/******************************************************************************
Name:	perspective
Desc:	Just sets the perspective parameters for the CCamera, the 
projection matrix, and orients the CCamera based on its orientation.

  Param1: FLOAT fov      - The CCameras field of view.
  Param2: FLOAT aspect   - The CCameras aspect ratio.
  Param3: FLOAT nearDist - The CCameras near clipping plane.
  Param4: FLOAT farDist  - The CCameras far clipping plane.
  
    
*****************************************************************************/
void CCamera::perspective( FLOAT fov, FLOAT aspect, FLOAT nearDist, 
                          FLOAT farDist )
{
    //sets the perspective parameters.
    setFieldOfView( fov );
    setAspectRatio( aspect );
    setNearClippingDistance( nearDist );
    setFarClippingDistance( farDist );
    
    //sets the projection matrix up with its perspective params.
    D3DXMatrixPerspectiveFovLH( &m_matProj, fov, aspect, nearDist, farDist );
    if (cls_tglo.p_d3dDevice)
        cls_tglo.p_d3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProj );
    
    //Orients the CCamera based on its orientation parameters.
    D3DXMatrixLookAtLH( &m_objectMatrix, &m_vObjectPosition, &m_vLook, &m_vUp );
    if (cls_tglo.p_d3dDevice)
        cls_tglo.p_d3dDevice->SetTransform( D3DTS_VIEW, &m_objectMatrix );
}

/******************************************************************************
Name:	decelerate
Desc:	Just sets the deceleration velocity for the CCamera.

  Param1: FLOAT decelVelocity - The rate at which the CCamera should decelerate.
  History:
*****************************************************************************/
void CCamera::decelerate( FLOAT decelVelocity )
{
    //multiplies the velocity properties by the rate of deceleration.
    m_vVelocity *= decelVelocity;
    m_fYawVelocity *= decelVelocity;
    m_fPitchVelocity *= decelVelocity;
}

/******************************************************************************
Name:	transform
Desc:	Just transforms the CCamera's movements, and orients it correctly.
Param1: FLOAT fSpeed - The the speed at which the CCamera is moving at.
Param2: FLOAT fAngularSpeed - The the speed at which the CCamera is rotating at.

  History:
*****************************************************************************/
void CCamera::transform( FLOAT fSpeed, FLOAT fAngularSpeed )
{
    if (p_visual_follow)
    {
        return; //ignore
        
    }
    
    // f_interpolation controls our smoothing
    
    // Update the position vector
    D3DXVECTOR3 vT = m_vVelocity * fSpeed;
    D3DXVec3TransformNormal( &vT, &vT, &m_matOrientation );
    m_vObjectPosition += vT;
    
    //limit how far they can go down
    //if( m_vObjectPosition.y < 1.0f ) m_vObjectPosition.y = 1.0f;
    
    // Update the yaw-pitch-rotation vector
    m_fAngleZ   += fAngularSpeed * m_fYawVelocity;
    m_fAngleY += fAngularSpeed * m_fPitchVelocity;
    
    //limit how far they can look up
    // if( m_fAngleY < 0.0f )      m_fAngleY = 0.0f;
    // if( m_fAngleY > D3DX_PI/2 ) m_fAngleY = D3DX_PI/2;
    
    // Set the view matrix
    update_matrix();
}



//=====================================================================
// Function:    ScreenToWorld
// Description: Convert mouse position and depth from camera to a world
//position
// Parameters:  mouse_x, mouse_y - raw mouse position
//              d - distance to observer
//              fov of camera, in radians
//              matViewInverse - inverse of view matrix
//              screen_w, screen_h - width and height of screen
//(m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height)
//
// Returns:             out - 3d position
// Notes:
//   Can be use to get a ray from eyepoint into scene
//
//      D3DXVECTOR3 v1;
//      D3DXVECTOR3 v2;
//      ScreenToWorld(mouse_event.x, mouse_event.y, 1,    D3DX_PI / 4,
//m_matViewInverse, v1);
//      ScreenToWorld(mouse_event.x, mouse_event.y, 1000, D3DX_PI / 4,
//m_matViewInverse, v2);
//      v2-v1 is vector from eyepoint for pick
//
//=====================================================================


void CCamera::get_world_to_screen_point(D3DXVECTOR3 *p_vec_out, D3DXVECTOR3 *p_point,
                                        D3DXMATRIX *p_matrix_world)
{
    
    //this crap doesn't work AT ALL
    
    
    D3DVIEWPORT8 vp;
    cls_tglo.p_d3dDevice->GetViewport( &vp );
    cls_tglo.p_d3dDevice->GetTransform( D3DTS_PROJECTION, &m_matProj );
    //  cls_tglo.p_d3dDevice->GetTransform( D3DTS_WORLD, &matWorld );
    cls_tglo.p_d3dDevice->GetTransform( D3DTS_VIEW, &m_objectMatrix );
    
    
    D3DXVec3Project(p_vec_out, p_point, &vp, &m_matProj, &m_objectMatrix, p_matrix_world);
    // log_msg("Real point is %s.",print_vector(*p_vec_out));
}

void ScreenToWorld(float mouse_x, float mouse_y, float d, float fov,
                   D3DXMATRIX matViewInverse,
                   float screen_w, float screen_h, D3DXVECTOR3 & out )
{
    float w2 = screen_w / 2.0f;
    float h2 = screen_h / 2.0f;
    
    // scale to [-1, 1]
    mouse_x -= w2;
    mouse_y -= h2;
    
    mouse_x /= w2;
    mouse_y /= h2;
    
    float fAspect = screen_w / screen_h;
    
    // correct for screen aspect ration
    mouse_x *= fAspect;
    
    float fov2 = float(tan(fov / 2));
    
    D3DXVECTOR3 pt(mouse_x * fov2 * d, -mouse_y * fov2 * d, d);
    
    D3DXVec3TransformCoord(&out, &pt, &matViewInverse);
}

