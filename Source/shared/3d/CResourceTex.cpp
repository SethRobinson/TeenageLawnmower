/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CResourceTex.h"
#include "D3DHelperFuncs.h"
#include "CThingFont.h"		 
#include "..\\std_all.h"
#include "d3dapp.h" 
#include "..\\uni.h"
#include <DXErr8.h>


#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CResourceTex::CResourceTex()
{
	i_mip_maps = 0;
	p_resource = NULL; 
	m_pt_size.x = 0;
	m_pt_size.y = 0;

}

CResourceTex::~CResourceTex()
{
 dx_invalidate();
 dx_kill();
}

void CResourceTex::CreateTexture(D3DXVECTOR2 pt_size, DWORD dw_color)
{
	m_pt_size.x = pt_size.x;
	m_dw_color = dw_color;
	m_pt_size.y = pt_size.y;
}




void CResourceTex::dx_create()
{
    if (p_resource) return; //already loaded
	if (get_refs() < 1) return; //nobody needs us, why should we load?
//	if (this->b_dynamic == false) if (get_refs() < 2) return; //ignore the first ref, this is
	//just to stop us from being removed when no longer needed
	HRESULT hr;
	//log_msg("loading tex %s in id %d.", GetName());
	if (i_resource_type == ::C_RESOURCE_TEXTURE)
	{                                   
		
		if (m_pt_size.x != 0)
		{
			//let's create the texture rather than load it
	
			//fill this in with info since there there is no other way to get it when doing a custom tex and
			//out code may rely on this info later
			m_source_image_info.Height = DWORD( m_pt_size.y);
			m_source_image_info.Width = DWORD( m_pt_size.x);
		
	        m_source_image_info.Format = D3DFMT_A8R8G8B8; //uh, hopefully that's what we get
           
            m_source_image_info.MipLevels = 1;
            m_source_image_info.Depth = 1;
            m_source_image_info.ResourceType = D3DRTYPE_TEXTURE;
       

			if( FAILED( hr =  D3DXCreateTexture( cls_tglo.p_d3dDevice,  unsigned int(m_pt_size.x),unsigned int( m_pt_size.y), i_mip_maps,
				D3DUSAGE_RENDERTARGET, m_source_image_info.Format , D3DPOOL_DEFAULT, (IDirect3DTexture8 **) &p_resource  )) )
					{
						log_error("CResourceTex: Unable to load texture %s. (%s)",GetName(), DXGetErrorString8(hr));
						SAFE_DELETE(p_resource);
						return;
					}
	
			//let's fill the color

			FillWithColor(m_dw_color);
			
		}
		else
		{
		
            char st_temp[256];

            if (!exist(GetName()))
            {
               sprintf(st_temp, "%s%s", GetExeDir(), GetName());
            }  else
            {
                strcpy(st_temp, GetName());
            }

			//	log_msg("Creating texture for %s.",st_name);
			
		
				if (i_mip_maps == 1)
				{
					//let's assume we're going to use this texture as a render target later.  Why else wouldn't
					//we want mip maps?
					if( FAILED( hr =  D3DXCreateTextureFromFileEx(  cls_tglo.p_d3dDevice,  st_temp, 
						D3DX_DEFAULT, D3DX_DEFAULT, i_mip_maps, D3DUSAGE_RENDERTARGET, D3DFMT_UNKNOWN /*D3DFMT_UNKNOWN*/, 
						D3DPOOL_DEFAULT, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
						D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 0, &m_source_image_info, NULL, (IDirect3DTexture8 **) &p_resource  )) )
					{
						log_error(" (MM1) CResourceTex: Unable to load texture %s. (%s)",st_temp, DXGetErrorString8(hr));
						SAFE_DELETE(p_resource);
						return;
					}
					
					
				} else
				{
			/*		
					if( FAILED( hr =  D3DXCreateTextureFromFileEx(  cls_tglo.p_d3dDevice,  GetName(), 
						D3DX_DEFAULT, D3DX_DEFAULT, i_mip_maps, 0, D3DFMT_UNKNOWN, 
						D3DPOOL_DEFAULT, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
						D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 0, &m_source_image_info, NULL, (IDirect3DTexture8 **) &p_resource  )) )
					{
						log_error("CResourceTex: Unable to load texture %s. (%s)",GetName(), DXGetErrorString8(hr));
						SAFE_DELETE(p_resource);
						return;
					}
			*/	
                
              
                	if( FAILED( hr =  D3DXCreateTextureFromFileEx(  cls_tglo.p_d3dDevice,  st_temp, 
						D3DX_DEFAULT, D3DX_DEFAULT, i_mip_maps, 0, D3DFMT_UNKNOWN, 
						D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
						D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 0, &m_source_image_info, NULL, (IDirect3DTexture8 **) &p_resource  )) )
					{
						log_error("CResourceTex: Unable to load texture %s. (%s)",st_temp, DXGetErrorString8(hr));
						SAFE_DELETE(p_resource);
						return;
					}

                
                }



				//	log_msg("Real pic dems: w=%d h=%d ", m_source_image_info.Width, m_source_image_info.Height);
		}
		
	(IDirect3DTexture8 *) p_resource->GetLevelDesc(0, &surface_desc);
	   
		 
		   if (! (&p_resource)) log_msg("p_resource is null after loading %s.",GetName());
	  }

     b_loaded = true; 

}


void CResourceTex::dx_invalidate()
{
    //if not managed we will need to kill it ourself
    if (!p_resource) return;
    
    SAFE_RELEASE(p_resource);
    b_loaded = false;
    
}

void CResourceTex::dx_kill()
{
	if (!p_resource) return;

 	if (i_resource_type == ::C_RESOURCE_TEXTURE)
	{
     	if (!p_resource)
		{
			log_msg("p_resource is null?!! %s", GetName());
		    return;
		}
		log_msg("Removing texture %s.",GetName());
	    IDirect3DTexture8 *  p_crap = (IDirect3DTexture8 *)p_resource;
	    SAFE_RELEASE(p_crap);
		p_resource = NULL;
	}

   b_loaded = false;
}

void CResourceTex::dx_restore()
{
 	if (p_resource) return;
	dx_create();
}				 

bool CResourceTex::HasAlpha()
{
	
	
	if (
		(surface_desc.Format != D3DFMT_DXT3) &&
		(surface_desc.Format != D3DFMT_A4R4G4B4) && ( surface_desc.Format !=D3DFMT_A8R8G8B8) 
		)
	{
		return false;
	}

	//well, we 'think' this texture loaded with custom alpha info...
	return true;
}

bool CResourceTex::alpha_test(int i_x, int i_y)
{
	//we only support DXT3 format right now

	if ( (surface_desc.Format != D3DFMT_DXT3) &&
		(surface_desc.Format != D3DFMT_A4R4G4B4) )
{
  //no alpha info for this kind of texture, assume true
	return true;
}


  if (surface_desc.Format == D3DFMT_DXT3)
  {
	  //actually we don't support this format either.  Yet.
	  return true; //just assume it's not transparent here
  }

  //get the surface
  IDirect3DSurface8	* p_surface;
  this->p_resource->GetSurfaceLevel(0, &p_surface);
  
  D3DLOCKED_RECT p_lrect; 
  HRESULT hr;

  if( FAILED(  hr = p_surface->LockRect(&p_lrect, NULL, D3DLOCK_READONLY)))
	{
	  log_msg("Failed to lock texture %s.",GetName());
	  return false;
	}

  
//I don't know how to decode dtx3 compressed so for this to work is has to be a pure 16 bit
//format.  I only check the second 8 bits.  

byte * pdwDest = (byte *)p_lrect.pBits;
static byte byte_pixel;
UINT uiDestPitch = p_lrect.Pitch;      // 4 bytes per pixel
byte_pixel = pdwDest[(uiDestPitch*i_y)+(i_x*2)+1]; //the 1 is to get the second 8 bits
 // log_msg("Doing hit test of %d:%d on %s. (%d)",i_x, i_y, this->st_name, byte_pixel);
p_surface->UnlockRect();

p_surface->Release();

 if (byte_pixel == 0) return false; //this is actually a hack, the alpha must be 0
 //AND the blue must be 0.. as the 8 bits we're checking are blue and alpha.

  return true;
}


bool CResourceTex::RenderText(int i_font, RECT *p_rect, WCHAR *p_wchar)
{
 
    if (!p_resource)
    {
        log_msg("Creating resoruce %s, wasn't ready for render text.", GetName());
        dx_create();
    }

// log_msg("Surface of %s is %s.", this->st_name, SurfFormatToString(surface_desc.Format));
   if (p_wchar == NULL)
   {
       log_error("Rendertext passed a null");
       return false;
   }
   D3DSURFACE_DESC desc;
   LPDIRECT3DSURFACE8 p_surface;
   p_resource->GetSurfaceLevel(0, &p_surface);
   p_surface->GetDesc(&desc);

   
   LPD3DXRENDERTOSURFACE  m_pRenderToSurface;
   HRESULT hr;

    if(FAILED(hr = D3DXCreateRenderToSurface(cls_tglo.p_d3dDevice, desc.Width, desc.Height, 
        desc.Format, FALSE, D3DFMT_UNKNOWN, &m_pRenderToSurface)))
    {
        log_error("Error using D3DXCreateRenderToSurface");
		return false;
    }
  
	if(SUCCEEDED(m_pRenderToSurface->BeginScene(p_surface, NULL)))
	{

		
		//special render states for the text
      /*
		 // Set diffuse blending for alpha set in vertices.
	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_SPECULARENABLE,  FALSE );
    
	//cls_tglo.p_d3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    // Enable alpha testing (skips pixels with less than a certain alpha.)
    */

	cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );

    if( cls_tglo.p_d3dapp->m_d3dCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL )
    {
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHAREF,        0);
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
    }

   
	RECT FontRect;
	FontRect.left = (get_width()*p_rect->left) / m_source_image_info.Width;
	FontRect.top  = (get_height()*p_rect->top) / m_source_image_info.Height;
	if (p_rect->right == 0) FontRect.right = get_width(); else FontRect.right = p_rect->right;
	if (p_rect->bottom == 0) FontRect.bottom = get_height(); else FontRect.bottom = p_rect->bottom;
	
	if (lforce_range(&FontRect.left, 0L, get_width()))
	{
		log_error("Can't draw text to point %d,%d in texture %s, moving it.", p_rect->left, p_rect->top, this->GetName());
	}
	
	if (lforce_range(&FontRect.top, 0, get_height()))
	{
		log_error("Can't draw text to point %d,%d in texture %s, moving it.", p_rect->left, p_rect->top, this->GetName());
	}
    //let's temporarily turn off antialiasing
    cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
    cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
    
    
    cls_tglo.p_font->RenderText(i_font, p_wchar, &FontRect);
    
    cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
    cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
    
    m_pRenderToSurface->EndScene();
    }
   
    
    p_surface->Release();
	m_pRenderToSurface->Release();
	
 return true; //success
}


	
bool CResourceTex::FillWithColor(DWORD dw_color)
{

	//use the texture as a render target to do the fill with a simple clear
	
    if (!p_resource)
    {
        log_error("Can't do fill with color, texture resource %s not initted.", GetName());
    }
	
	  D3DSURFACE_DESC desc;
   LPDIRECT3DSURFACE8 p_surface;
   p_resource->GetSurfaceLevel(0, &p_surface);
   p_surface->GetDesc(&desc);

   
   LPD3DXRENDERTOSURFACE  m_pRenderToSurface;
   HRESULT hr;

    if(FAILED(hr = D3DXCreateRenderToSurface(cls_tglo.p_d3dDevice, desc.Width, desc.Height, 
        desc.Format, FALSE, D3DFMT_UNKNOWN, &m_pRenderToSurface)))
    {
        log_error("Error using D3DXCreateRenderToSurface");
		return false;
    }
  	if(SUCCEEDED(m_pRenderToSurface->BeginScene(p_surface, NULL)))
	{
	cls_tglo.p_d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET , dw_color, 255, 0L);
	m_pRenderToSurface->EndScene();
	}
  	
	p_surface->Release();
	m_pRenderToSurface->Release();

	
	
	//LOCK AND WRITE A VALUE IN IT

/*	
  HRESULT hr;
  D3DLOCKED_RECT lockedrect;
  ZeroMemory(&lockedrect, sizeof(lockedrect));
  if (FAILED(hr = p_resource->LockRect(0, &lockedrect, NULL, 0)))
  {
	  log_error("Unable to lockrect of %s. (%s)",st_name, DXGetErrorString8(hr));
	  return false;
  }

  //time to change the bits!
   
  //this could be a lot faster.. a memset probably won't work because of the
  //pitch but I could at least do memsets per line, etc

   DWORD *pSurfBits = static_cast<DWORD*>(lockedrect.pBits);
   
   lockedrect.Pitch /= 4;
   int index = 0;
 	
   for (int y = 0; y < get_height(); y++)
   {
	   for (int x = 0; x < get_width(); x++)
	   {
		   pSurfBits[(lockedrect.Pitch*y) + x] = dw_color;
	   }
   }

  if (FAILED(hr = p_resource->UnlockRect(0)))
  {
	  log_error("Unable to unlockrect of %s. (%s)",st_name, DXGetErrorString8(hr));
	  return false;
  }
  */



 return true;
}