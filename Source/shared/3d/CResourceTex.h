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

#include "CThing_globals.h"
#include "CResource.h"
#include "assert.h"
//handles textures

class CResourceTex: public CResource
{

public:
 D3DXIMAGE_INFO m_source_image_info;

 inline LPDIRECT3DTEXTURE8 get_texture(){ return p_resource;};
 virtual void dx_create();
 virtual void dx_restore();
 virtual void dx_invalidate();
 virtual void dx_kill();
 inline UINT get_height(){return surface_desc.Height;};
 inline UINT get_width(){return surface_desc.Width;};
 
 inline UINT get_src_height(){return m_source_image_info.Height;};
 inline UINT get_src_width(){return m_source_image_info.Width;};
 
 bool CResourceTex::alpha_test(int i_x, int i_y);
 bool CResourceTex::HasAlpha();
 bool CResourceTex::RenderText(int i_font, RECT *p_rect, WCHAR *p_wchar);
 void SetMipMaps(int i_new){i_mip_maps = i_new;}
 bool CResourceTex::FillWithColor(DWORD dw_color);
 void CResourceTex::CreateTexture(D3DXVECTOR2 pt_size, DWORD dw_color);
 CResourceTex();
 virtual ~CResourceTex();

private:
 LPDIRECT3DTEXTURE8 p_resource; //the texture
 D3DSURFACE_DESC surface_desc; //information about the texture
 int i_mip_maps; //leave 0 for auto which creates like 5.  For buttons/menus you want
 //0 mip maps so we can easily change them.
 D3DXVECTOR2 m_pt_size; //if not 0,0 it means we're creating a tex, not loading it
 DWORD m_dw_color; //we init a created tex to this color
};	 


