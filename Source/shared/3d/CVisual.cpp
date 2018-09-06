/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CVisual.h"
#include "CCamera.h"
#include "CThing_Util.h"
#include "spheretree\\spherepack.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CVisual::CVisual()
{
 	    m_fAngleX = 0;
 	    m_fAngleY = 0;
 	    m_fAngleZ = 0;
		b_translate_asap = false;
		D3DXMatrixIdentity(&m_mat_base);	
		D3DXMatrixIdentity( &m_objectMatrix );
		D3DXMatrixIdentity( &m_matTrans );
		D3DXMatrixIdentity( &m_matRotX );
		D3DXMatrixIdentity( &m_matRotY );
		D3DXMatrixIdentity( &m_matRotZ );
		m_vObjectPosition = D3DXVECTOR3(0,0,0);
		p_visual_follow = NULL; //if not null, will copy this guys cords
		p_visual_look_at = NULL; //if not null, our dir target will follow this guy
		b_no_lighting = false;
		i_fill_mode = D3DFILL_SOLID;
		f_scale_x = f_scale_y = f_scale_z = 1;
		f_dir = 0;
		b_no_render_next_frame = false;
		vec_follow_offset = D3DXVECTOR3(0,0,0);
		f_tilt = f_roll = 0;
		SetCentering(C_CENTERING_UPPER_LEFT);
        m_b_skip_draw_of_next_frame = false;
        SetZDisable(false);
        SetForceDoubleSidedTextures(false);

}


CVisual::~CVisual()
{
 
	
}


void CVisual::SetCentering(int i_new)
{
    assert(i_new < C_CENTERING_COUNT && "Invalid centering mode");
    e_centering = i_new;
}
void CVisual::DontLookAtMe(CThing *p_thing) 
{
  //if we were looking at this object, unregister it.

    if (p_thing == p_visual_follow) set_visual_follow(NULL);
    if (p_thing == p_visual_look_at) SetVisualLook(NULL);
}
   
void CVisual::dx_draw_prepare()
{
  //get ready to render
  if (this->i_fill_mode != D3DFILL_SOLID)
  {
   cls_tglo.p_d3dDevice->SetRenderState( D3DRS_FILLMODE, i_fill_mode );
  }
  if (this->b_no_lighting)
  {
   cls_tglo.p_d3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
 
  }
  if (m_b_zdisable)
  {
      cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
  }
  if (m_b_force_double_sided_texture)
  {
      cls_tglo.p_d3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_NONE);
  }

  if (b_translate_asap)
  {
   this->translateMovement();
   
 }
 
}

void CVisual::dx_draw()
{
  //run when rendering is finished
  if (this->i_fill_mode != D3DFILL_SOLID)
  {
   cls_tglo.p_d3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
  }
  if (this->b_no_lighting)
  {
   cls_tglo.p_d3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
  }

  if (m_b_zdisable)
  {
      cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
  }

  if (m_b_force_double_sided_texture)
  {
      cls_tglo.p_d3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW);
  }
  
}		  


//apply a rotation as the base, before all regular rotations.
void CVisual::RotateBaseFrameYX(float f_rotate_y_rad, float f_rotate_x_rad)
{
	//fix rotation problem
	D3DXMatrixRotationY( &m_mat_base, f_rotate_y_rad);
	D3DXMATRIX p_mat;
	D3DXMatrixRotationX( &p_mat, f_rotate_x_rad);
	D3DXMatrixMultiply(&m_mat_base,  &m_mat_base, &p_mat);
}

void CVisual::set_visual_follow(CVisual *p_follow)
{
	
	p_visual_follow = p_follow;

//	char st_crap[256];
	//fix any rotation problems with the dir

	if (p_follow)
	{
/*	sprintf (st_crap,"Old: Camera dir: %f, Object dir: %f", f_dir,
		p_visual_follow->f_dir);
	   log_msg(st_crap);*/
	p_visual_follow->f_dir = float(fmod(p_visual_follow->f_dir, 2*D3DX_PI));
	p_visual_follow->f_dir_goal = float(fmod(p_visual_follow->f_dir_goal, 2*D3DX_PI));
	}

	//otherwise just fix my own
	f_dir =  float(fmod(f_dir, 2*D3DX_PI));
	f_dir_goal =  float(fmod(f_dir_goal, 2*D3DX_PI));
	/*
	if (p_visual_follow)
	{
	sprintf (st_crap,"New: Camera dir: %f, Object dir: %f", f_dir,
		p_visual_follow->f_dir);
	   log_msg(st_crap);
	   }
	   */
} 

void CVisual::set_visual_follow_offset(D3DXVECTOR3 vec_new)
{
  vec_follow_offset = vec_new;
}


void CVisual::think()
{
    if (p_visual_follow)
    {
        this->setObjectPosition(p_visual_follow->getObjectPosition()+vec_follow_offset);
        set_translate_asap(true);
    }
    
//	this->translateMovement();
    CThing::think();
}

void CVisual::dx_restore()
{

}

void CVisual::dx_create()
{

}

void CVisual::dx_kill()
{

}


void CVisual::dx_invalidate()
{

}

//
//
///******************************************************************************
//	Name:	rotateXLeft (in radians)
//	Desc:	Rotates an object around the x axis in the left direction.
//	Param1: FLOAT fSpeed - The speed of the rotation.
//
// *****************************************************************************/
//void CVisual::rotateXLeft( FLOAT fSpeed ) 
//{ 
//	m_fAngleX += fSpeed;
//	//if past 360 degrees, start over from 0 again.
//	if(m_fAngleX > (D3DX_PI * 2)) m_fAngleX = 0.0f;
//}
//
///******************************************************************************
//	Name:	rotateXRight (in radians)
//	Desc:	Rotates an object around the x axis in the right direction.
//	Param1: FLOAT fSpeed - The speed of the rotation.
//
// *****************************************************************************/
//void CVisual::rotateXRight( FLOAT fSpeed ) 
//{ 
//	m_fAngleX = m_fAngleX - fSpeed; 
//	//if past -360 degrees, start over from 0 again.
//	if(m_fAngleX < -(D3DX_PI * 2)) m_fAngleX = 0.0f;
//}
//
///******************************************************************************
//	Name:	rotateYLeft (in radians)
//	Desc:	Rotates an object around the y axis in the left direction.
//	Param1: FLOAT fSpeed - The speed of the rotation.
//
//	History:
// *****************************************************************************/
//void CVisual::rotateYLeft( FLOAT fSpeed ) 
//{ 
//	m_fAngleY += fSpeed; 
//	//if past 360 degrees, start over from 0 again.
//	if(m_fAngleY > (D3DX_PI * 2)) m_fAngleY = 0.0f;
//}
//
///******************************************************************************
//	Name:	rotateYRight (in radians)
//	Desc:	Rotates an object around the y axis in the right direction.
//	Param1: FLOAT fSpeed - The speed of the rotation.
//
//
//	History:
// *****************************************************************************/
//void CVisual::rotateYRight( FLOAT fSpeed ) 
//{ 
//	m_fAngleY = m_fAngleY - fSpeed; 
//	//if past -360 degrees, start over from 0 again.
//	if(m_fAngleY < -(D3DX_PI * 2)) m_fAngleY = 0.0f;
//}
//
///******************************************************************************
//	Name:	rotateZLeft (in radians)
//	Desc:	Rotates an object around the z axis in the left direction.
//	Param1: FLOAT fSpeed - The speed of the rotation.
//
//	History:
// *****************************************************************************/
//void CVisual::rotateZLeft( FLOAT fSpeed ) 
//{ 
//	m_fAngleZ += fSpeed; 
//	//if past 360 degrees, start over from 0 again.
//	if(m_fAngleZ > (D3DX_PI * 2)) m_fAngleZ = 0.0f;
//}
//
///******************************************************************************
//	Name:	rotateZRight (in radians)
//	Desc:	Rotates an object around the z axis in the right direction.
//	Param1: FLOAT fSpeed - The speed of the rotation.
//
//	History:
// *****************************************************************************/
//void CVisual::rotateZRight( FLOAT fSpeed ) 
//{ 
//	m_fAngleZ = m_fAngleZ - fSpeed;
//	//if past -360 degrees, start over from 0 again.
//	if(m_fAngleZ < -(D3DX_PI * 2)) m_fAngleZ = 0.0f;
//}
//
///******************************************************************************
//	Name:	moveTo
//	Desc:	Stores a movement coordinate into a queue and processes it.
//			Allows each object to define their path, and make sure they stick
//			to their path.
//	Param1: float x		 - The x coordinate of the destination point.
//	Param2: float y		 - The y coordinate of the destination point.
//	Param3: float z		 - The z coordinate of the destination point.
//	Param4: float fSpeed - The speed at which to reach the destination point.
//
//	History:
// *****************************************************************************/
//
void CVisual::set_scale(float f_x, float f_y, float f_z)
{
  f_scale_x = f_x;
  f_scale_y = f_y;
  f_scale_z = f_z;
  set_translate_asap(true);
}

//uniform scale
void CVisual::set_scale_uniform(float f_scale)
{
  f_scale_x = f_scale;
  f_scale_y = f_scale;
  f_scale_z = f_scale;
  set_translate_asap(true);
}



//restricts it following on the Y axis only
void CVisual::setup_billboard_matrix()
{

//	D3DXMatrixIdentity(&m_objectMatrix);
	
	D3DXMatrixScaling(&m_objectMatrix, f_scale_x, f_scale_y, f_scale_z);
 
  //steal info from the camera...
  //get the angle we should point 
	
 float l_f_dir = get_theta_from_x_and_z(cls_tglo.p_camera->m_vObjectPosition.x - get_pos_x() ,
 cls_tglo.p_camera->m_vObjectPosition.z-get_pos_z());
 
 D3DXMatrixRotationY( &m_matRotY, l_f_dir);

 D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matRotY);

 m_objectMatrix._41 = get_pos_x();
 m_objectMatrix._42 = get_pos_y();
 m_objectMatrix._43 = get_pos_z();
 //setTranslate();
 set_translate_asap(false);

// set_translate_asap(true); 
}

//faces camera, no restrictions, faster than above
void CVisual::setup_billboard_matrix_simple()
{
 //steal info from the camera...
/*	
 D3DXMatrixScaling(&m_objectMatrix, f_scale_x, f_scale_y, f_scale_z);
 D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &cls_tglo.p_camera->m_inverse_view);
  */
 //add any additional rotation we need

    D3DXMatrixIdentity(&m_objectMatrix);
    static D3DXMATRIX matScale;
    

    //first add a tilt?
    if (f_tilt != 0)
    {
        //performs the appropriate matrix rotation operations.
        D3DXMatrixRotationY( &m_matRotY, getAngleY() );
        D3DXMatrixRotationX( &m_matRotX, f_tilt );
        
        D3DXMatrixScaling(&matScale, f_scale_x, f_scale_y, f_scale_z);
        D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &matScale );
        D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matRotX  );
        D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matRotY  );
    } else
    {
        
        //performs the appropriate matrix rotation operations.
        D3DXMatrixRotationY( &m_matRotY, getAngleY() );
        D3DXMatrixRotationX( &m_matRotX, getAngleX() );
        D3DXMatrixRotationZ( &m_matRotZ, getAngleZ() );
        
        D3DXMatrixScaling(&matScale, f_scale_x, f_scale_y, f_scale_z);
        D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &matScale );
        
        D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matRotY  );
        D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matRotZ  );
        D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matRotX  );
        
    }

    D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &cls_tglo.p_camera->m_inverse_view);
   
  
 //we got the correct matrix and scaled it the way we want, now let's move to our position..
 m_objectMatrix._41 = get_pos_x();
 m_objectMatrix._42 = get_pos_y();
 m_objectMatrix._43 = get_pos_z();
  
 //setTranslate();
 set_translate_asap(false);
 // set_translate_asap(true); 
}

void CVisual::alpha_sort()
{
/*
  static D3DXVECTOR3 vec_dist;
  vec_dist = this->m_vObjectPosition-cls_tglo.p_camera->getObjectPosition();
  set_sort(20000-Fast_Distance_3D(vec_dist.x, vec_dist.y, vec_dist.z));
  */

   static D3DXVECTOR2 vec_dist;
  vec_dist.x = this->m_vObjectPosition.x-cls_tglo.p_camera->getObjectPosition().x;
  vec_dist.y = this->m_vObjectPosition.z-cls_tglo.p_camera->getObjectPosition().z ;
  set_sort(int(20000-(D3DXVec2Length(&vec_dist)*10)));
  
}


void CVisual::translateMovement()
{

	m_objectMatrix = m_mat_base;
    static D3DXMATRIX matScale;
	
	//first add a tilt?
	if (f_tilt != 0)
	{
		//performs the appropriate matrix rotation operations.
		D3DXMatrixRotationY( &m_matRotY, getAngleY() );
		D3DXMatrixRotationX( &m_matRotX, f_tilt );
		
		D3DXMatrixScaling(&matScale, f_scale_x, f_scale_y, f_scale_z);
		D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &matScale );
		D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matRotX  );
		D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matRotY  );
	} else
	{
	
		//performs the appropriate matrix rotation operations.
		D3DXMatrixRotationY( &m_matRotY, getAngleY() );
		D3DXMatrixRotationX( &m_matRotX, getAngleX() );
		D3DXMatrixRotationZ( &m_matRotZ, getAngleZ() );
		
		D3DXMatrixScaling(&matScale, f_scale_x, f_scale_y, f_scale_z);
		D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &matScale );
		
		D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matRotY  );
		D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matRotZ  );
		D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matRotX  );
		
	}

    b_translate_asap = false;
    setTranslate();
 
}


/******************************************************************************
	Name:	setTranslate
	Desc:	Just a helper function to perform a translation on the world matrix
			in the correct manner.

	History:
 *****************************************************************************/
void CVisual::setTranslate()
{
	D3DXMatrixTranslation(&m_matTrans, m_vObjectPosition.x, m_vObjectPosition.y+vec_follow_offset.y,
						  m_vObjectPosition.z+vec_follow_offset.z);

	D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matTrans );

//position moved?
    if (m_p_sphere_pack)
    {
        m_p_sphere_pack->NewPos(* ((Vector3d<float>*)&m_vObjectPosition));
    // log_msg("Setting %s to %s.",get_thing_name(),  print_vector(m_vObjectPosition) );
    }
}


