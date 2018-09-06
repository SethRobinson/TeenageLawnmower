/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CVisualMulti.h"
#include "CManager.h"
#include "CCamera.h"
#include "CThing_util.h"
//#include "CCalControl.h"
#include "CMessageHeader.h" 

#include "CThingList.h"
#include "CBaseEntData.h"
#include "CEntItem.h"
#include "CResourceMD2.h"
#include "CResourceRTM.h"
#include "CAnimInfo.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CVisualMulti::CVisualMulti()
{
    
     m_f_radius = 1; //default radius to prevent devide by 0's
    m_f_radius_mod = 1.0f; //no change
    b_lod_active = true; //by default draw full quality
   	m_i_lod = 10;
   	f_height = 0;
	m_i_visual_id = ::C_RESOURCE_NONE;
	m_i_visual_type = ::C_RESOURCE_NONE;
	this->e_collision_type = ::C_COLLISION_BOX; //default
	b_follow_ground_tilt = true;
	SetIgnoreKillMessage(false);
	p_cal = NULL;
	b_drag_active = false;
	b_clickable = false;
	b_got_clicked = false;
	b_update_matrix = false;   
	b_floating = false;
	m_i_alpha_sort = C_VISUAL_ALPHA_SORT_UNKNOWN; 
	m_i_draw_mode = C_DRAW_MODE_NORMAL;
	m_i_mip_map = 0; //default of 0 means create as many mip maps as needed
	m_dw_vertex_color = D3DCOLOR_XRGB(255,255,255); //default to all color, no translucent
    st_filename[0] = 0;
	m_i_texture_set = 0; //some .x files have multiple texture configurations
	m_i_res_adjust = C_RES_ADJUST_NONE;
	m_f_res_adjust_save_x = 0;
	m_f_res_adjust_save_y = 0;	   
    SetShadowMode(C_VISUAL_SHADOW_NONE);
    SetBlink(false);
    SetSmartFade(0,0); //disable smart fade
	m_anim_info = NULL;
	m_p_resource = NULL;
    SetSphereTreeMod(1);
    m_f_fade_speed = 0; //0 means not active
    m_f_res_adjust_scale_x = m_f_res_adjust_scale_y = 1;

}

CVisualMulti::~CVisualMulti()
{
   //release our hold on the resources we were using

	  if (cls_tglo.p_manager)
	{
		if (m_i_visual_id != ::C_RESOURCE_NONE)
		{
		  //  log_msg("%s is Dropping ref to %s.", get_thing_name(), ((CResource*)GetResourcePointer())->GetName());
            cls_tglo.p_manager->delete_ref(m_i_visual_id);
		}
	}
  SAFE_DELETE(m_anim_info); //would only be active if we were using an md2

#ifdef _CAL3D
  SAFE_DELETE(p_cal); //would only be active if we were using a cal3d object
#endif

}


void CVisualMulti::SetShadowMode(int i_mode)
{
    m_i_shadow_mode = i_mode;
}


void CVisualMulti::ExtractColorsFromVertexColor()
{
    m_f_color_r = GetRFromD3DCOLOR(m_dw_vertex_color);
    m_f_color_g = GetGFromD3DCOLOR(m_dw_vertex_color);
    m_f_color_b = GetBFromD3DCOLOR(m_dw_vertex_color);
    m_f_color_a = GetAlphaFromD3DCOLOR(m_dw_vertex_color);
}


//this includes alpha!

void CVisualMulti::SetColorFadeTo(float f_r, float f_g, float f_b, float f_a, float f_speed)
{
    m_f_fade_target_r = f_r;
    m_f_fade_target_g = f_g;
    m_f_fade_target_b = f_b;
    m_f_fade_target_a = f_a;

    m_f_fade_speed = f_speed;
    
    ExtractColorsFromVertexColor();
     
 
    SetSmartFade(0,0); //disable smart fade
}

void CVisualMulti::dx_restore()
{

	if (m_i_res_adjust != C_RES_ADJUST_NONE)
   {
        SetResAdjust(m_i_res_adjust, m_i_res_correct_base_x, m_i_res_correct_base_y);
        set_xy(m_f_res_adjust_save_x, m_f_res_adjust_save_y);
   }
	if (m_anim_info)
	{
		//reset our texture pointer
		m_anim_info->p_tex = NULL; //this will force it to refresh;
	}
}


void CVisualMulti::SetSmartFade(float f_min_mod, float f_max_mod)
{
	if ( (f_min_mod == 0) && (f_max_mod == 0))
	{
		//it's being turned off.
		m_b_smart_fade = false;
		return;
	}	else
	{
			m_b_smart_fade = true;
	}

        ExtractColorsFromVertexColor();
    


	   m_f_smart_fade_min = f_min_mod;
	   m_f_smart_fade_max = f_max_mod;

	  //force an alpha sort to happen
	   m_i_alpha_sort = C_VISUAL_ALPHA_SORT_YES;
}

void CVisualMulti::set_drag_mode(bool b_new)
{
  b_drag_active = b_new;  
}

//this is really only for CPic's I think...
void CVisualMulti::update_2d_matrix()
{
	//setup stuff in advance to blit in the correct spot
	D3DXMatrixScaling(&m_objectMatrix, f_scale_x*m_f_res_adjust_scale_x, f_scale_y*m_f_res_adjust_scale_y, f_scale_z);
	D3DXMatrixRotationZ( &m_matRotZ, getAngleZ() );
	D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matRotZ  );
	D3DXMatrixTranslation(&m_matTrans, m_vObjectPosition.x, m_vObjectPosition.y,
						  m_vObjectPosition.z);
	D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matTrans );
}

void CVisualMulti::SetResAdjust(int i_type, int i_base_x, int i_base_y)
{

    assert(i_type < C_RES_ADJUST_COUNT && "Unknown type of res adjust");
    if (m_i_res_adjust == C_RES_ADJUST_NONE)
    {
        //first time, imprint current x/y as original
        //save originals
        m_f_res_adjust_save_x = m_vObjectPosition.x;
        m_f_res_adjust_save_y = m_vObjectPosition.y;

    }
    
    m_i_res_adjust = i_type;
	m_i_res_correct_base_x = i_base_x;
	m_i_res_correct_base_y = i_base_y;
    
    switch (i_type)
    {

    case C_RES_ADJUST_FULL:
     //remember the ratios, we may need to do scaling
      m_f_res_adjust_scale_x = float(cls_tglo.get_screen_width()) / float(m_i_res_correct_base_x);
      m_f_res_adjust_scale_y = float(cls_tglo.get_screen_height()) / float(m_i_res_correct_base_y);
        break;
    
    default:

        m_f_res_adjust_scale_x = 1;
        m_f_res_adjust_scale_y = 1;

    }

	//activate it
	set_xy(m_f_res_adjust_save_x, m_f_res_adjust_save_y);

}
void CVisualMulti::set_xy(float f_x, float f_y)
{
  m_vObjectPosition.x = f_x;
  m_vObjectPosition.y = f_y;
  
  //add any special offsets, these are used for interface items so they won't look too
  //funky at different resolutions
    
  switch (m_i_res_adjust)
  {

  case C_RES_ADJUST_CENTER_BOTTOM:
      //save originals
      m_f_res_adjust_save_x = m_vObjectPosition.x;
      m_f_res_adjust_save_y = m_vObjectPosition.y;
      
      if (cls_tglo.get_screen_width() != m_i_res_correct_base_x)
      {
          //adjust x
          
          m_vObjectPosition.x += (cls_tglo.get_screen_width() - m_i_res_correct_base_x) / 2;
          
          m_vObjectPosition.y += 	(cls_tglo.get_screen_height() - m_i_res_correct_base_y);
          
      }
      break;

  case C_RES_ADJUST_NORMAL:

      //save originals
      m_f_res_adjust_save_x = m_vObjectPosition.x;
      m_f_res_adjust_save_y = m_vObjectPosition.y;
      
      if (cls_tglo.get_screen_width() != m_i_res_correct_base_x)
      {
          //adjust x
          
          m_vObjectPosition.x += (cls_tglo.get_screen_width() - m_i_res_correct_base_x) / 2;
          
          m_vObjectPosition.y += 	(cls_tglo.get_screen_height() - m_i_res_correct_base_y) /2;
          
      }

      break;

  case C_RES_ADJUST_FULL:
  case C_RES_ADJUST_FULL_NO_SCALE:
      
      //save originals
      m_f_res_adjust_save_x = m_vObjectPosition.x;
      m_f_res_adjust_save_y = m_vObjectPosition.y;
      
      if (cls_tglo.get_screen_width() != m_i_res_correct_base_x)
      {
          //adjust x
          
          
          m_vObjectPosition.x = (f_x / float(m_i_res_correct_base_x)) * cls_tglo.get_screen_width();
          
          m_vObjectPosition.y = (f_y / float(m_i_res_correct_base_y)) * cls_tglo.get_screen_height();
          
      }
    break;

  }

      
  update_2d_matrix();
}

void CVisualMulti::set_xy(int i_x, int i_y)
{
  set_xy(float(i_x), float(i_y));
}

void CVisualMulti::ReplaceTexture(char *p_st_texture_set, char *p_st_old, char *p_st_new)
{

	//ok, are we currently using something that we can replace the texture with?

    if (m_i_visual_type == C_RESOURCE_PMESH)
    {
        CResourcePMesh *p_pmesh = (CResourcePMesh*)GetResourcePointer();
        
        m_i_texture_set = p_pmesh->ReplaceTexture(p_st_texture_set, p_st_old, p_st_new);
        return;
    }
    
    
	//if we got here there was a problem
	log_error("Unable to force texture %s on this ent (%s), wrong type.", p_st_new,
		get_thing_name());
}



void CVisualMulti::ray_pick(CMessageRayPick *p_msg)
{
		static D3DXPLANE plane;
	
 	if (m_i_visual_type == ::C_RESOURCE_SPRITE)
	{
      	//get the RECT making the pic (each D3DXVECTOR2 being a 3d vertice
	   	CResourceSprite *p_sprite = (CResourceSprite*)GetResourcePointer();
	   //check collision against first tri of the box
	   static D3DXVECTOR3 vec_hit;
	
		//tranforms our coordinates
		static D3DXVECTOR3 v[4]; //transformed vertexes go here
	
		//setup_billboard_matrix(); //we'll let this remain whatever it was, hopefully updated from the
		//last frame.


		for (int i=0; i < 4; i++)
		{
		   D3DXVec3TransformCoord(&v[i], &p_sprite->v[i].p, &m_objectMatrix);
		}

	    bool b_hit = false;
		
		//second part is start, third part is direction, doesn't need to be normalized													
		if (IntersectTriangle(&vec_hit, *p_msg->p_vec_origin, *p_msg->p_vec_dir, v[0], v[3],v[1]))
		{
			b_hit = true;
			//let's grab the exact D3DXVECTOR2 we hit
			
			D3DXPlaneFromPoints(&plane, &v[0], &v[3],&v[1]);
			D3DXPlaneIntersectLine(&vec_hit,&plane,p_msg->p_vec_origin,p_msg->p_vec_end);
			
		} else
			
			if (IntersectTriangle(&vec_hit, *p_msg->p_vec_origin, *p_msg->p_vec_dir, v[0], v[2],v[3]))
			{
				b_hit = true;
				D3DXPlaneFromPoints(&plane, &v[0], &v[2],&v[3]);
				D3DXPlaneIntersectLine(&vec_hit,&plane,p_msg->p_vec_origin,p_msg->p_vec_end);
			}
			
			else
			{
				//no hit
			}
			

			if (b_hit)
			{
			    //a hit has been registered.  But we have two questions, A, is the sprite transparent and B
				//convert the hit coords back to local space for the image
				
				static D3DXMATRIX mat_inverse;
				D3DXMatrixInverse(&mat_inverse, NULL, &m_objectMatrix);
				D3DXVECTOR3 vec_local; //hit spot in local coords
			    D3DXVec3TransformCoord(&vec_local, &vec_hit, &mat_inverse);
				
				//we now have the coord, let's ask the resource if a hit was made. 
				if (p_sprite->alpha_test(vec_local.x, vec_local.y))
				{
		  	     //log_msg("Hit sprite %s.", this->get_thing_name());
				 //let's get the distance to this sprite
				 D3DXVECTOR3 vec_dist = *p_msg->p_vec_origin - vec_hit;
				 //get vector length
				 float f_dist = D3DXVec3Length(&vec_dist);
				 bool b_valid_hit = false;

				 if (!p_msg->p_thing) 
				 {
					 //no other contenders, it's all us
					 b_valid_hit = true;
				 } else
				 {
				   //are we closer than the last guy?
					if (f_dist < p_msg->f_dist)
					{
					  //yes!
						b_valid_hit = true;
					}

				 }
				
				 if (b_valid_hit)
				 {
				   //save the data
					 p_msg->f_dist = f_dist; //exact distance away
					 *p_msg->p_vec_hit = vec_hit; //exact spot of impact
					 p_msg->p_thing = this; //save us, let them figure out what kind of thing we are
				 }


				}
				/*
				CEntTree *p_dot = new CEntTree();
				this->p_my_thing_list->add_new_thing(p_dot);
				p_dot->setObjectPosition(vec_hit);
				p_dot->dx_create();
				*/

			}

	}

	
	if (m_i_visual_type == C_RESOURCE_PMESH)
	{
	   	CResourcePMesh *p_mesh = (CResourcePMesh*)GetResourcePointer();
 
		D3DXVECTOR3 vec_sphere_middle = m_vObjectPosition;
		
		//figure radius
	

		vec_sphere_middle.y += m_f_radius/2;

		if (D3DXSphereBoundProbe(&vec_sphere_middle, m_f_radius, p_msg->p_vec_origin, p_msg->p_vec_dir))
		{
		 
			//we hit the sphere.  but did we hit the actual mesh?  We need to do a very accurate test
			//now.
		    D3DXMATRIX  mat_inverse;
			void *pres = D3DXMatrixInverse(&mat_inverse, NULL, &m_objectMatrix);
			#ifdef _DEBUG
			if (!pres)
			{
				log_error("Can't inverse matrix in raypic");
			}
			#endif
			//first convert our ray to world model space, let's work with copies, not the originals
			D3DXVECTOR3  vec_orig;
			D3DXVECTOR3  vec_dest;
			D3DXVECTOR3  vec_dir;
			D3DXVECTOR3 vec_hit; //return info

			D3DXVec3TransformCoord( &vec_orig,  p_msg->p_vec_origin,&mat_inverse);
			D3DXVec3TransformCoord( &vec_dest,  p_msg->p_vec_end,&mat_inverse);
   
			D3DXVECTOR3 vec_dir_temp = ( vec_dest - vec_orig);
			//normalize result
			
			
			
			D3DXVec3Normalize(&vec_dir, &vec_dir_temp);
	
			if (p_mesh->AccurateRayCheck(&vec_orig, &vec_dir, &vec_hit))
			{
			
				//convert the hit back into world space
				
		  	D3DXVec3TransformCoord( &vec_hit,  &vec_hit, &m_objectMatrix);
			//log_msg("Real hit is at %s.", print_vector(vec_hit));
			//collision was made, but are we the closest?
			D3DXVECTOR3 vec_dist = *p_msg->p_vec_origin - m_vObjectPosition;
			
			float f_dist = 	D3DXVec3Length(&vec_dist);
		//log_msg("HIT! Dist is %f.", f_dist);
				 bool b_valid_hit = false;

				 if (!p_msg->p_thing) 
				 {
					 //no other contenders, it's all us
					 b_valid_hit = true;
				 } else
				 {
				   //are we closer than the last guy?
					if (f_dist < p_msg->f_dist)
					{
					  //yes!
						b_valid_hit = true;
					}

				 }
				
				 if (b_valid_hit)
				 {
				   //save the data
					 p_msg->f_dist = f_dist; //exact distance away
					 *p_msg->p_vec_hit = m_vObjectPosition; //exact spot of impact
					 p_msg->p_thing = this; //save us, let them figure out what kind of thing we are
				 }
			}
		}

	}


  
		if ( (m_i_visual_type == C_RESOURCE_MD2) || (m_i_visual_type == C_RESOURCE_RTM)	)
	{
	   	CResourceAnim *p_mesh = (CResourceAnim*)GetResourcePointer();
 
		D3DXVECTOR3 vec_sphere_middle = m_vObjectPosition;
		
		//figure radius
		vec_sphere_middle.y += m_f_radius;
//		char st_crap[255];
//		sprintf(st_crap, "%s's rad is %.4f.", get_thing_name(), f_radius);
//		log_msg(st_crap);

		if (D3DXSphereBoundProbe(&vec_sphere_middle, m_f_radius, p_msg->p_vec_origin, p_msg->p_vec_dir))
		{
				//collision was made, but are we the closest?
			D3DXVECTOR3 vec_dist = *p_msg->p_vec_origin - m_vObjectPosition;
		
			float f_dist = 	D3DXVec3Length(&vec_dist);
			//log_msg("HIT! Dist is %f.", f_dist);
			bool b_valid_hit = false;
			
			if (!p_msg->p_thing) 
			{
				//no other contenders, it's all us
				b_valid_hit = true;
			} else
			{
				//are we closer than the last guy?
				if (f_dist < p_msg->f_dist)
				{
					//yes!
					b_valid_hit = true;
				}
				
			}
			
			if (b_valid_hit)
			{
				//save the data
				p_msg->f_dist = f_dist; //exact distance away
				*p_msg->p_vec_hit = m_vObjectPosition; //exact spot of impact
				p_msg->p_thing = this; //save us, let them figure out what kind of thing we are
			}
		}


	}

}


void CVisualMulti::BuildShadowMatrix()
{
   //build the matrix, assume it's setup to be the shadow already
        
}

void CVisualMulti::SetBlink(bool b_active)
{
    m_b_blink_active = b_active;

    if (m_b_blink_active)
    {
        //turn it on
        m_timer_blink.set_interval(350); //change to settable var later?  yeah if we need it.
        m_b_blink_toggle = false;
    }   else
    {
        //turn it off
        VertexColor(D3DCOLOR_ARGB(255,255,255,255));
    }

}

void CVisualMulti::ProcessBlink()
{
    if (!m_b_blink_active) return;

    if (m_timer_blink.interval_reached())
    {
        if (m_b_blink_toggle)
        {
            VertexColor(D3DCOLOR_ARGB(255,255,255,255));
        } else
        {
            VertexColor(D3DCOLOR_ARGB(155,255,255,255));
        }
      m_b_blink_toggle = !m_b_blink_toggle;
    }


}
void CVisualMulti::VertexColor(DWORD dw_color)
{
   m_dw_vertex_color = dw_color;
}

bool CVisualMulti::DistanceCulled()
{
    //do we have a 'force draw' flag?
 
    if (cls_tglo.p_thing_list->GetCullingDistance() == 0) return false; //culling disabled
    static D3DXVECTOR3 vec_temp;
  
  vec_temp = m_vObjectPosition-cls_tglo.p_camera->m_vObjectPosition;
  
  //how far are we from the camera?
  if (D3DXVec3LengthSq(&vec_temp)
      < pow(cls_tglo.p_thing_list->GetCullingDistance() + m_f_radius,2))
  {
      return false; //draw it

  }

  return true; //don't draw the sucker
}

void CVisualMulti::dx_draw()
{
   	if (b_no_render_next_frame || b_delete_flag || m_b_skip_draw_of_next_frame)
	{
	    m_b_skip_draw_of_next_frame = false;
       translateMovement(); 
        return; //don't draw it this time
	}


    //is culling active?
    if (DistanceCulled())
    {
        return;
    }


    ProcessBlink();

	CVisual::dx_draw_prepare();

	if (m_i_visual_type == ::C_RESOURCE_PMESH)
	{
		m_i_lod = cls_tglo.i_mesh_detail_mod;
		set_world_transform();
        
        CResourcePMesh *p_mesh = (CResourcePMesh*)GetResourcePointer();
	
		p_mesh->draw(b_lod_active, m_i_lod, m_i_texture_set, GetVertexColor(), C_RENDER_EXTRA_NONE);
	
        //also draw shadow

        if (m_i_shadow_mode == C_VISUAL_SHADOW_ACCURATE)
        {
            cls_tglo.p_d3dDevice->SetTransform( D3DTS_WORLD, &m_mat_shadow);
            p_mesh->draw(b_lod_active, m_i_lod, m_i_texture_set, D3DCOLOR_ARGB(30,0,0,0), C_RENDER_EXTRA_SHADOW);
        }
        
        
    } else 
        
    if ( (m_i_visual_type == ::C_RESOURCE_MD2) || (m_i_visual_type == ::C_RESOURCE_RTM))
    {
        set_world_transform();
        //let our special md2 class thingie handle it
        m_anim_info->Draw();
    }  else
    
	if (m_i_visual_type == ::C_RESOURCE_SPHERE)
	{
		set_world_transform();
		CResourceSphere *p_sphere = (CResourceSphere*)GetResourcePointer();
		p_sphere->dx_draw();
    }  else
	if (m_i_visual_type == ::C_RESOURCE_SPRITE)
	{
		set_world_transform();
		CResourceSprite *p_sprite = (CResourceSprite*)GetResourcePointer();
		p_sprite->dx_draw();
		
    } else
	
	if (m_i_visual_type == ::C_RESOURCE_CAL_CORE)
	{
			if (p_cal)
			{
				set_world_transform();
#ifdef _CAL3d
				p_cal->draw();
#else
assert(!"Cal 3D is disabled right now.");
#endif
			} else
		{
			assert(0);
			
       }
       
   } else
       if (m_i_visual_type == C_RESOURCE_NONE)
       {
           
       }   else
       {
           assert(!"Don't know how to render this visual type.  Ass.");
       }
       
      CVisual::dx_draw();
		
}

void CVisualMulti::dx_create()
{
	
	if (m_i_visual_id != ::C_RESOURCE_NONE)
	{
		//load if needed
		cls_tglo.p_manager->load_if_needed(m_i_visual_id);
		//cache it


		if (m_i_visual_type == ::C_RESOURCE_PMESH)
		{
			BuildRadius(this->f_scale_x);
		} else
			if ((m_i_visual_id == C_RESOURCE_MD2) || (m_i_visual_id == C_RESOURCE_RTM))
			{
				BuildRadius(this->f_scale_x);
			}
			
			if (m_i_visual_type == ::C_RESOURCE_CAL_CORE)
			{
				//is the core model already loaded?
				cls_tglo.p_manager->load_if_needed(m_i_visual_id);
				
				//actually load info into the animation instance
#ifdef _CAL3d
				if (p_cal)
				{
					if (!p_cal->p_render->b_loaded) p_cal->p_render->dx_create();
					p_cal->init_and_attach_meshes();
				}
#endif
			}
			
	}
	
	
	
	CVisual::dx_create();
}

void * CVisualMulti::GetResourcePointer()
{
	if (!m_p_resource)
	{
		//one time lookup of the real resource
		m_p_resource = cls_tglo.p_manager->get(m_i_visual_id);
	}

	return m_p_resource;
}


bool CVisualMulti::init_visual(char cst_name[255], int i_resource_type)
{
    //if we already have a visual loaded, let's remove our ref to it.
    
    if (cls_tglo.p_manager)
	{
		if (m_i_visual_type != ::C_RESOURCE_NONE)
		{
		   // log_msg("%s is Dropping ref to %s.", get_thing_name(), ((CResource*)GetResourcePointer())->GetName());
    
            //if the new thing we're loading is not the old thing, let's delete it
            if (
                (i_resource_type == m_i_visual_type)
                && (strcmp(st_filename, cst_name) == 0) )
            {
                //it's the exact same thing we had loaded, no need to load it again
               return true;

            }   else
            {
           
             cls_tglo.p_manager->delete_ref(m_i_visual_id);
	         m_i_visual_id = C_RESOURCE_NONE;
             m_i_visual_type = C_RESOURCE_NONE;
             m_i_texture_set = 0; //remove any special texture settings we had
            }
         }
	}
 
    
    m_p_resource = NULL;

 strcpy(st_filename, cst_name);
  m_i_visual_type = i_resource_type;
  m_i_visual_id = cls_tglo.p_manager->get_dynamic_resource_by_string(m_i_visual_type, st_filename);
  
  this->translateMovement(); //perform full translation with movement
  set_sort(m_i_visual_id); //sort it so we can draw all these together
//  e_collision_type = C_COLLISION_SPHERICAL; //default for 3d models

  //if it's an mdl let's deal with it
  if (i_resource_type == C_RESOURCE_PMESH)
  {
      //load it now?
      if (cls_tglo.IsRunning())
      {
         ((CResourcePMesh*)GetResourcePointer())->dx_create(); //load the mesh now if needed
	   
      }
  }
  
  if ( (i_resource_type == C_RESOURCE_MD2) || (i_resource_type == C_RESOURCE_RTM))
  {
	  //special code for MD2's
	  //init our MD2 control struct
	  
     // SAFE_DELETE(m_anim_info);
	  
      if (!m_anim_info)
      {
         m_anim_info = new CAnimInfo();
      }
	  m_anim_info->p_parent = this;
	  ((CResourceAnim*)GetResourcePointer())->InitMesh(); //load the mesh now if needed
	 //m_anim_info->p_tex = NULL; //force texture to be refreshed
      BuildRadius(this->f_scale_x); //this is safe cause I say so
  }

  return true;
}




//overriden from the one in CVisual, this one also takes into account
//the radius when setting the spherepack pos
void CVisualMulti::setTranslate()
{
	D3DXMatrixTranslation(&m_matTrans, m_vObjectPosition.x+vec_follow_offset.x, m_vObjectPosition.y+vec_follow_offset.y,
						  m_vObjectPosition.z+vec_follow_offset.z);

	D3DXMatrixMultiply( &m_objectMatrix, &m_objectMatrix, &m_matTrans );

//position moved?
    if (m_p_sphere_pack)
    {
       
     Vector3d<float> sphere_pos;
     sphere_pos.x = m_vObjectPosition.x;
     sphere_pos.z = m_vObjectPosition.z;
     sphere_pos.y = m_vObjectPosition.y + GetRadiusDevided(); 
        m_p_sphere_pack->NewPos(sphere_pos);
 //  log_msg("Setting VisMulti %s to %s.",get_thing_name(),  print_vector(*(D3DXVECTOR3*)&sphere_pos) );
    }
}


void CVisualMulti::BuildRadius(float f_mod)
{

	if (m_i_visual_type == ::C_RESOURCE_PMESH)
	{
	
        CResourcePMesh *p_mesh = (CResourcePMesh*)GetResourcePointer();
	//calculate our radius from the original model and this modifer
	m_f_radius = p_mesh->m_fObjectRadius*f_mod*m_f_radius_mod; 
   
  /* 
    char st_crap[256];
    sprintf(st_crap, "Built pmesh radius. (%s - %f)", this->get_thing_name(),
     f_radius);
    log_msg(st_crap);
  */ 


    }  else 
        
        if ( (m_i_visual_type == C_RESOURCE_MD2) || (m_i_visual_type == C_RESOURCE_RTM) )
        {
            CResourceMD2 *p_mesh = (CResourceMD2*)GetResourcePointer();
            //calculate our radius from the original model and this modifer
            m_f_radius = p_mesh->m_fObjectRadius*f_mod*m_f_radius_mod; 
        }	else
        {
            //not a model, so let's just use the mod as the real radius.
            m_f_radius = f_mod*m_f_radius_mod;
            
        }

    
      //now that we have the radius, be sure to update the sphere tree with it's info
        if (GetSpherePack())
      GetSpherePack()->SetRadius(m_f_radius*GetSphereTreeMod());
}



void CVisualMulti::UpdateSphereTexture(char *st_new)
{

}


bool CVisualMulti::init_visual_sphere(char *st_name, float x_scale, 
					 float y_scale, float z_scale, UINT num_segments, UINT num_rings, 
					 float tex_scale, bool hemisphere, D3DCOLOR dcolor, char *p_st_tex )
{
  strcpy(st_filename, st_name);
  m_i_visual_type = ::C_RESOURCE_SPHERE;
  m_i_visual_id = cls_tglo.p_manager->get_dynamic_resource_by_string(m_i_visual_type, st_filename);
  m_p_resource = NULL;


  //create the sphere with the info we got
  	CResourceSphere *p_temp = (CResourceSphere*)GetResourcePointer();
 
	//load it if it doesn't already exist
	if (!p_temp->sphere_has_been_generated())
	{
	  //set texture
		p_temp->SetSkyTexture(p_st_tex);
	p_temp->Create(x_scale,y_scale,z_scale,num_segments,num_rings,tex_scale, hemisphere,
	dcolor);
	} else
	{
	  //get true y_scale here later maybe?
    }
    
    m_f_radius = y_scale;
    f_height = y_scale*2;
    this->e_collision_type = ::C_COLLISION_SPHERICAL;
    
    this->translateMovement(); //perform full translation with movement
    set_sort(m_i_visual_id); //sort it so we can draw all these together
    return true;
}


void CVisualMulti::ReleaseResourceIfWeHaveOne()
{
    if (m_i_visual_id != C_RESOURCE_NONE)
    {
        cls_tglo.p_manager->delete_ref(m_i_visual_id);
        m_i_visual_id = 0;
    }
    
}

//this will not deref any sprite that is already loaded, so be careful when calling this.  I can't really change
//the functionality now because the CPic class needs it this way when it deals with buttons.  (it hands it's own dereffing
//of the buttons)



bool CVisualMulti::init_visual_sprite(const char p_st_name[], const char p_st_texture[])
{
   
  //if we have already initted this sprite, we have to do some fancy footwork to make sure it
	//gets updated
  bool b_force_reinit = false;

  //if we already are linked to a sprite, let's remove our ref

 m_p_resource = NULL;

	if (stricmp(p_st_name, st_filename) == 0)
	{
		//we already have something with the same name loaded.  Did the actual image inside of it
		//change though?  Let's reinit it.
		b_force_reinit = true;
	}

  strcpy(st_filename, p_st_name);
  m_i_visual_type = ::C_RESOURCE_SPRITE;
  m_i_visual_id = cls_tglo.p_manager->get_dynamic_resource_by_string(m_i_visual_type, st_filename);
  CResourceSprite *p_temp = (CResourceSprite*)GetResourcePointer();
  p_temp->SetMipMap(m_i_mip_map);
 
  
  p_temp->setup_sprite(p_st_texture);
  this->e_collision_type = C_COLLISION_BOX;
  this->translateMovement(); //perform full translation with movement
  if (get_type() != C_THING_CPIC)
  {
  
  set_sort(m_i_visual_id); //sort it so we can draw all these together
  } else
  {
      //i'm not real sure about this
      update_2d_matrix();
  }
  
  if (b_force_reinit)
  {
	  if (cls_tglo.IsRunning())
	  {
		  p_temp->RefreshTexture();
	  }
  }
  
  
  

  return true;
}

bool CVisualMulti::init_visual_sprite_blank(char p_st_name[], D3DXVECTOR2 pt_size, DWORD dw_color)
{
   if (cls_tglo.p_manager)
	{
		if (m_i_visual_id != ::C_RESOURCE_NONE)
		{
		   // log_msg("%s is Dropping ref to %s.", get_thing_name(), ((CResource*)GetResourcePointer())->GetName());
            cls_tglo.p_manager->delete_ref(m_i_visual_id);
	         m_i_visual_id = C_RESOURCE_NONE;
        }
	}

  strcpy(st_filename, p_st_name);
  m_i_visual_type = ::C_RESOURCE_SPRITE;
  m_p_resource = NULL;
  //send it blank, so it won't try to load a texture yet
  m_i_visual_id = cls_tglo.p_manager->get_dynamic_resource_by_string(m_i_visual_type, st_filename);
  CResourceSprite *p_temp = (CResourceSprite*)GetResourcePointer();
  SetMipMap(1); //no extra mip maps, it complicates text writing and such
  p_temp->setup_sprite_create_new(pt_size, dw_color);
  p_temp->SetMipMap(m_i_mip_map);
 
  this->e_collision_type = C_COLLISION_BOX;
  this->translateMovement(); //perform full translation with movement
  if (get_type() != C_THING_CPIC)
  set_sort(m_i_visual_id); //sort it so we can draw all these together (Cpic's we don't
  //mess with because they have their own draw order
 return true;
}



CResourceTex * CVisualMulti::GetSpriteTex()
{
  if (m_i_visual_type == C_RESOURCE_SPRITE)
  {
   CResourceSprite *p_temp = (CResourceSprite*)GetResourcePointer();
   assert(p_temp);
   return (CResourceTex*)(cls_tglo.p_manager->get(p_temp->i_tex_id));
  }

  return NULL;
}



bool CVisualMulti::init_visual_cal_instance(char p_cal_name[])
{
  strcpy(st_filename, p_cal_name);
  m_i_visual_type = ::C_RESOURCE_CAL_CORE; //kind of a fake resource, we store this outself
  m_p_resource = NULL;
  if (p_cal)
	{
    log_error("p_cal already initted!??? NooooO!");
	}
#ifdef _CAL3d
  
  this->p_cal = new CCalControl();
  
  p_cal->init_cal_instance(this, st_filename);
#endif
  
  this->e_collision_type = C_COLLISION_BOX;
  this->translateMovement(); //perform full translation with movement
  set_sort(m_i_visual_id); //sort it so we can draw all these together
 
  //let a custom base matrix handle the effed up rotations we get from Max
 D3DXMatrixRotationY( &m_mat_base, D3DX_PI);
 D3DXMATRIX p_mat;
 D3DXMatrixRotationX( &p_mat, D3DX_PI/2);
 D3DXMatrixMultiply(&m_mat_base,  &m_mat_base, &p_mat);
 
  return true;
}

void CVisualMulti::process_drag()
{
  if (!b_drag_active) return; //not dragging right now

  float f_movement = cls_tglo.time()*0.15f;
 
   if ( (cls_tglo.p_cls_input->is_key_down(DIK_LSHIFT))
	   ||
	   (cls_tglo.p_cls_input->is_key_down(DIK_RSHIFT))
	   )
 		{
		 f_movement *= 14;
	   //speed things up
 		}

  if (cls_tglo.p_cls_input->is_key_down(DIK_NUMPAD6))
  {
	//rotate right
	  setAngleY(getAngleY()+f_movement);
	 f_dir_goal += f_movement;

  }
  
  if (cls_tglo.p_cls_input->is_key_down(DIK_NUMPAD4))
  {
	//rotate right
	  setAngleY(getAngleY()-f_movement);
	  //entities need their goal set too or they just turn back..
	  f_dir_goal -= f_movement;
  }
 
bool b_force_float_mode = false;
    if (cls_tglo.p_cls_input->is_key_down(DIK_NUMPAD8))
  {
	//rotate right
	 	//force this to not orient with the ground
        b_force_float_mode = true;		
		this->m_vObjectPosition.y += f_movement;
 }

    if (cls_tglo.p_cls_input->is_key_down(DIK_NUMPAD2))
  {
	//rotate right
        b_force_float_mode = true;		
	  this->m_vObjectPosition.y -= f_movement;
 }


  if (b_force_float_mode)
  {
	  //if this is an Ent let's turn on floating mode so the up/down movement won't
	  //be killed by the follow ground flag
	  if (get_type() == ::C_THING_ENT_GENERIC)
	  {
		  ((CEntItem*)this)->SetFloatingMode(true);
	  }
  }

   if (cls_tglo.p_cls_input->is_key_down(DIK_G))
   {
	  if (get_type() == ::C_THING_ENT_GENERIC)
	  {
		  ((CEntItem*)this)->SetFloatingMode(false);
	  }
   }
 


  //get position of mouse
 static D3DXVECTOR3 vec_ground;
  if (get_3d_mouse_position(&vec_ground))
  {
  	//move us to there
	 if (b_floating) 
	 {
	  this->m_vObjectPosition.x = vec_ground.x;
	   this->m_vObjectPosition.z = vec_ground.z;

	 } else
	 {
	  this->setObjectPosition(vec_ground);

	 }
//	if (this->i_visual_type == ::C_RESOURCE_PMESH)
	b_update_matrix = true;
  }


  if (!cls_tglo.p_cls_input->is_mouse_down(::C_MOUSE_LEFT))
  {
    //no longer dragging
	  b_drag_active = false;
  }

}		


void CVisualMulti::CheckForAlphaSort()
{

    //if a fade is active, that will override anything else
    if (m_f_fade_speed != 0)
    {
        //set_float_with_target(&f_)
        set_float_with_target(&m_f_color_a, m_f_fade_target_a, m_f_fade_speed*cls_tglo.time());
        set_float_with_target(&m_f_color_r, m_f_fade_target_r, m_f_fade_speed*cls_tglo.time());
        set_float_with_target(&m_f_color_g, m_f_fade_target_g, m_f_fade_speed*cls_tglo.time());
        set_float_with_target(&m_f_color_b, m_f_fade_target_b, m_f_fade_speed*cls_tglo.time());

        //LogMsg("alpha is %d",int(m_f_color_a));
        VertexColor(D3DCOLOR_ARGB((int)(int)m_f_color_a,(int)m_f_color_r,(int)m_f_color_g,(int)m_f_color_b));

        if (
            (m_f_color_a == m_f_fade_target_a)
           && (m_f_color_r == m_f_fade_target_r)
           && (m_f_color_g == m_f_fade_target_g)
           && (m_f_color_b == m_f_fade_target_b)
            )
        {
           //all done fading
            m_f_fade_speed = 0;
        }
             
    }  


    //does the image we're using need alpha sorting?
   	if (m_i_alpha_sort == C_VISUAL_ALPHA_SORT_UNKNOWN)
	{
		
		//check to see if we should do alpha sorting or not
		
		
		if (m_i_visual_type == ::C_RESOURCE_PMESH)
		{
		 	CResourcePMesh *p_mesh = (CResourcePMesh*)GetResourcePointer();
		
			if (p_mesh->HasAlpha(m_i_texture_set))
			{
				m_i_alpha_sort = C_VISUAL_ALPHA_SORT_YES;
			} else
			{
				m_i_alpha_sort = C_VISUAL_ALPHA_SORT_NO;
			}
		
        }  else
		
		
		if (m_i_visual_type == ::C_RESOURCE_SPRITE)
		{
			
		
			CResourceSprite *p_sprite = (CResourceSprite*)GetResourcePointer();
			
			if (p_sprite->HasAlpha())
			{
				m_i_alpha_sort = C_VISUAL_ALPHA_SORT_YES;
			} else
			{
				m_i_alpha_sort = C_VISUAL_ALPHA_SORT_NO;
			}
			
		}
	}

  if (m_i_alpha_sort == C_VISUAL_ALPHA_SORT_YES)
  {
	  if (m_b_smart_fade)
	  {
		  alpha_sort_smart_fade();
	  }	  else
	  {
	  
	  alpha_sort(); //located in CVisual
	  }
  }

}

void CVisualMulti::alpha_sort_smart_fade()
{
 if (!cls_tglo.p_camera->GetSmartFadeActive())
 {
   //ignore smart fade, camera is probably in 1st person view
     VertexColor(D3DCOLOR_ARGB((int)m_f_color_a,(int)m_f_color_r,(int)m_f_color_g,(int)m_f_color_b));
     return;

 }
  //if you change the 20000 in this, make sure you change it in CVisual.  
	static D3DXVECTOR3 vec_dist;
  vec_dist = this->m_vObjectPosition-cls_tglo.p_camera->getObjectPosition();
  
  /*
  float f_dist =  Fast_Distance_3D(vec_dist.x, vec_dist.y, vec_dist.z);
  */
  
  //using this code the object will still go transparent even if the camera
  //is far above it.  Later I may want to modify it so it actually computes
  //height of the object (radius) into the mix.

  float f_dist =  float(Fast_Distance_2D(int(vec_dist.x), int(vec_dist.z)));
  
  
  set_sort(20000-int(f_dist));
 
  //at this point we change our alpha to match the distance away we are.
  f_dist -= GetRadiusDevided(); //what results is

  //if f_dist < 0 then we're inside the object

  if (f_dist < 0)
  {
	  //house is completely invisible
	  VertexColor(D3DCOLOR_ARGB(0,(int)m_f_color_r,(int)m_f_color_g,(int)m_f_color_b));
  }	  else
  if (f_dist < (20*m_f_smart_fade_min))
  {
	  //partially visible, figure it out

	  float f_alpha = 0+(255* (f_dist/(20*m_f_smart_fade_min)));
	 
//	  log_msg("Dist: %d. Alpha is %d.", int(f_dist), int(f_alpha));
	  VertexColor(D3DCOLOR_ARGB((int)f_alpha,(int)m_f_color_r,(int)m_f_color_g,(int)m_f_color_b));


  }	  else
  {
	  //completely visible
	  VertexColor(D3DCOLOR_ARGB(255,(int)m_f_color_r,(int)m_f_color_g,(int)m_f_color_b));

  }
  
  
}	

void CVisualMulti::SetAnimTexture(char *st_name)
{
	if (!m_anim_info)
	{
		log_error("Can't change MD2 texture to %s because no anim has been loaded yet.", st_name);
		return;
	}

	m_anim_info->LoadOrChangeTexture(st_name);
}

 void CVisualMulti::SetAnimByName(char *st_name, int i_frame)
{
	if (!m_anim_info)
	{
		log_error("Can't change to animation %s because no animation has been loaded yet.", st_name);
		return;
	}

	m_anim_info->SetAnimByName(st_name, i_frame);
}


 void CVisualMulti::SetAnimByIndex(int i_anim, int i_frame)
{
	if (!m_anim_info)
	{
		log_error("Can't set animation to index %d because no animation exists yet for ent %s.", i_anim, get_thing_name());
		return;
	}

	m_anim_info->SetAnimByIndex(i_anim, i_frame);
} 

 void CVisualMulti::SetAnimPause(bool b_pause)
 {
	if (!m_anim_info)
	{
		log_error("Can't set animation pause mode because no animation exists yet for ent %s.", get_thing_name());
		return;
	}

	m_anim_info->SetPause(b_pause);
 }


 void CVisualMulti::SetSpeedMod(float f_speed_mod)
{
	if (!m_anim_info)
	{
		log_error("Can't set animation speed mode because no animation exists yet for ent %s.",  get_thing_name());
		return;
	}

	m_anim_info->SetSpeedMod(f_speed_mod);
} 





