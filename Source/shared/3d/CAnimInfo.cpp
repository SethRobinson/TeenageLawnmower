/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */

//******************************************


#include "CAnimInfo.h"
#include "..\\all.h"
#include "CManager.h" 
#include "CVisualMulti.h"
#include "d3dapp.h"
#include "CResourceAnim.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAnimInfo::CAnimInfo()
{
   memset(this, 0, sizeof(CAnimInfo));
   i_tex_id = C_RESOURCE_NONE;
   f_speed_mod = 1;
   
}

void CAnimInfo::SetSpeedMod(float f_new_speed_mod)
{
	f_speed_mod = f_new_speed_mod;
}

CAnimInfo::~CAnimInfo()
{
   //if we had a texture let's release our reference
	if (i_tex_id)
	{
	 	if (cls_tglo.p_manager)
		cls_tglo.p_manager->delete_ref(i_tex_id);
      
	}

}

void CAnimInfo::LoadOrChangeTexture(char *st_filename)
{
	if (i_tex_id != C_RESOURCE_NONE)
	{
		//release our reference count on the old one
		cls_tglo.p_manager->delete_ref(i_tex_id);
        i_tex_id = C_RESOURCE_NONE;
	}

	//get or create new texture
	i_tex_id = cls_tglo.p_manager->get_dynamic_resource_by_string(C_RESOURCE_TEXTURE, st_filename);
    p_tex = NULL;
       
}

//send -1 for the start frame and it will "auto set it".  (if anim is the same
//the frame won't change
void CAnimInfo::SetAnimByName(char *st_anim, int i_start_frame)
{
	//upcase it!
	strupr(st_anim);
	
	
	CResourceAnim *p_mesh = (CResourceAnim*)p_parent->GetResourcePointer();
	i_anim = p_mesh->GetAnimByName(st_anim);
	if (i_start_frame != -1)
	{
		//they want to set the frame to, reletive to the anim's frame
		f_frame = float(i_start_frame);
	}

}

void CAnimInfo::SetAnimByIndex(int i_new_anim, int i_start_frame)
{
	i_anim = i_new_anim;

	if (i_start_frame != -1)
	{
		//they want to set the frame to, reletive to the anim's frame
		f_frame = float(i_start_frame);
	}

}

void CAnimInfo::SetPause(bool b_paused)
{
	m_b_paused = b_paused;
}


void CAnimInfo::RefreshTextureInfo()
{
	if (p_tex) return;

	//well, no texture pointer exists so let's refresh it.

	if (!cls_tglo.IsRunning()) return;  //don't refresh it yet
	
	//cache out where the pointer is for speed
	CResourceTex* p_res = (CResourceTex*) cls_tglo.p_manager->get(i_tex_id);
	p_res->dx_create(); //load if needed
	p_tex = p_res->get_texture();

}

void CAnimInfo::Draw()
{
	//let's setup our texture and any special render states


	if (i_tex_id != C_RESOURCE_NONE)
	{
	  RefreshTextureInfo();
	  cls_tglo.p_d3dDevice->SetTexture(0, p_tex);
//	  cls_tglo.p_d3dDevice->SetTexture(0, NULL);
	}
  	
	CResourceAnim *p_mesh = (CResourceAnim*)p_parent->GetResourcePointer();

	if (m_b_paused)
	{
		//special code to calculate a good pause point
	  f_frame = float(int(f_frame)); //round off to nearest solid frame
	
	}	else
	{
	
	//calculate current frame
	f_frame += (cls_tglo.p_d3dapp->m_fElapsedTime *C_BASE_SPEED_MOD * f_speed_mod); 
	
	}

	if (f_frame > p_mesh->GetFramesInAnim(i_anim)) f_frame = 0;

	//if (f_frame > 10) f_frame = 0;
 /*
		char st _msg[255];
	sprintf(st_msg, "Anim %d, Frame %d, (interpolating with %d) T=%.4f\n",i_anim, (int)f_frame, 
		0, (f_frame-int(f_frame)));

		log_msg(st_msg);
   */

	if (p_mesh)
    {
    
        p_mesh->Draw(i_anim, (int) (f_frame), (f_frame-int(f_frame)),p_parent->GetVertexColor(), C_ANIM_INTERPOLATE_MODE_FORWARD);

       if (p_parent->GetShadowMode() == C_VISUAL_SHADOW_ACCURATE)
        {
            cls_tglo.p_d3dDevice->SetTransform( D3DTS_WORLD, p_parent->GetShadowMatrix());
             p_mesh->Draw(i_anim, (int) (f_frame), (f_frame-int(f_frame)),D3DCOLOR_ARGB(40,0,0,0), C_ANIM_INTERPOLATE_MODE_FORWARD);

       }

    }


}