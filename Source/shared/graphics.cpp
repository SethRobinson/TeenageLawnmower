/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */




//graphics.cpp - base DirectDraw wrapper class
//Author:  Seth A. Robinson
//Copyright 1998: RTsoft, Inc.
#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "stdio.h"
#include <io.h>
#include <stdlib.h>
#include <fcntl.h>
#include <windowsx.h>
#include "string.h"
#include <process.h>
#include <direct.h>
#include <ddraw.h>
#include "fastfile.h"
#include "all.h"
#include "graphics.h"
void dderror(HRESULT hErr);
int load_ff(IDirectDraw7 *lpDD, char name[9], turbo *seq, int count);


int getx( void )
{
return(		GetSystemMetrics(SM_CXSCREEN));		
}

int gety( void )
{
return(		GetSystemMetrics(SM_CYSCREEN));
		
}



void window_set_exact_size(HWND hw, int my_x, int my_y)
{
 //using this information, let's figure out how big the window REALLY has to be to
 //have a client area exactly x and y.
	RECT window_rect = {0,0,my_x,my_y};
	
	AdjustWindowRectEx(&window_rect,
		    GetWindowStyle(hw),
			GetMenu(hw) != NULL,
			GetWindowExStyle(hw));
  //resize the window
  MoveWindow(hw,
	   CW_USEDEFAULT,
	   CW_USEDEFAULT,
	   window_rect.right - window_rect.left, //width
	   window_rect.bottom - window_rect.top, //height
	   false);


}

void turbo_blit(IDirectDrawSurface7 *primary, IDirectDrawSurface7 *back)
{
	HRESULT             ddrval;

	again:
	ddrval = primary->Blt(NULL ,back, NULL,NULL, NULL);
    if (ddrval == DDERR_WASSTILLDRAWING) goto again;
      if (ddrval != DD_OK) dderror(ddrval);

}

LPDIRECTDRAWCLIPPER DDraw_Attach_Clipper(LPDIRECTDRAW7 lpdd7, LPDIRECTDRAWSURFACE7 lpdds,
                                         int num_rects,
                                         LPRECT clip_list)

{
// this function creates a clipper from the sent clip list and attaches
// it to the sent surface

int index;                         // looping var
LPDIRECTDRAWCLIPPER lpddclipper;   // pointer to the newly created dd clipper
LPRGNDATA region_data;             // pointer to the region data that contains
                                   // the header and clip list

// first create the direct draw clipper
if (FAILED(lpdd7->CreateClipper(0,&lpddclipper,NULL)))
   return(NULL);

// now create the clip list from the sent data

// first allocate memory for region data
region_data = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER)+num_rects*sizeof(RECT));

// now copy the rects into region data
memcpy(region_data->Buffer, clip_list, sizeof(RECT)*num_rects);

// set up fields of header
region_data->rdh.dwSize          = sizeof(RGNDATAHEADER);
region_data->rdh.iType           = RDH_RECTANGLES;
region_data->rdh.nCount          = num_rects;
region_data->rdh.nRgnSize        = num_rects*sizeof(RECT);

region_data->rdh.rcBound.left    =  64000;
region_data->rdh.rcBound.top     =  64000;
region_data->rdh.rcBound.right   = -64000;
region_data->rdh.rcBound.bottom  = -64000;

// find bounds of all clipping regions
for (index=0; index<num_rects; index++)
    {
    // test if the next rectangle unioned with the current bound is larger
    if (clip_list[index].left < region_data->rdh.rcBound.left)
       region_data->rdh.rcBound.left = clip_list[index].left;

    if (clip_list[index].right > region_data->rdh.rcBound.right)
       region_data->rdh.rcBound.right = clip_list[index].right;

    if (clip_list[index].top < region_data->rdh.rcBound.top)
       region_data->rdh.rcBound.top = clip_list[index].top;

    if (clip_list[index].bottom > region_data->rdh.rcBound.bottom)
       region_data->rdh.rcBound.bottom = clip_list[index].bottom;

    } // end for index

// now we have computed the bounding rectangle region and set up the data
// now let's set the clipping list

if (FAILED(lpddclipper->SetClipList(region_data, 0)))
   {
   // release memory and return error
   free(region_data);
   return(NULL);
   } // end if

// now attach the clipper to the surface
if (FAILED(lpdds->SetClipper(lpddclipper)))
   {
   // release memory and return error
   free(region_data);
   return(NULL);
   } // end if

// all is well, so release memory and send back the pointer to the new clipper
free(region_data);
return(lpddclipper);

} // end DDraw_Attach_Clipper


void blit_normal(IDirectDrawSurface7 *back, graphics_struct * pic, int left, int top,
				 IDirectDraw7 *lpDD)
{
	RECT box_crap;
	int ddrval;
				box_crap.top = top;
				box_crap.bottom = top + pic->box.bottom;
				box_crap.left = left;
				box_crap.right = left + pic->box.right;
     RECT box_src;
	 CopyRect(&box_src, &pic->box);

	 //custom clipping

	 if (box_crap.left < 0)
	 {
       box_src.left += (box_crap.left - (box_crap.left * 2));
	   box_crap.left = 0;
	 }

	if (box_crap.top < 0)
	 {
       box_src.top += (box_crap.top - (box_crap.top * 2));
	   box_crap.top = 0;
	 }

start: 
	 ddrval = back->Blt( &box_crap, pic->pic, &box_src, DDBLT_KEYSRC| DDBLT_WAIT, NULL);
	if (ddrval != DD_OK) 
	{
	
		
		dderror(ddrval);
		if (ddrval == DDERR_SURFACELOST)
		{
			turbo *t_temp;
			t_temp = (turbo *)pic->turbo;
			Msg("Reloading graphic %s.", t_temp->filename);
			//reload graphic.
		 if (load_ff(lpDD,t_temp->filename,t_temp, 0) == 0) 
		 {
			 Msg("Error reloading graphics.");
			 return;
		 }	else
		 {
		  Msg("Reloading graphic ok.");
		  goto start;
		 }

		}
	}
  
}

//example usage:  	blit_normal_custom_color_key(back, garrow.pic[1], global.mouse_x, global.mouse_y, RGB(0,0,0));


void blit_normal_custom_color_key(IDirectDrawSurface7 *back, graphics_struct * pic, int left, int top, DWORD dw_color)
{
	RECT box_crap;
	int ddrval;
				box_crap.top = top;
				box_crap.bottom = top + pic->box.bottom;
				box_crap.left = left;
				box_crap.right = left + pic->box.right;
     RECT box_src;
	 CopyRect(&box_src, &pic->box);

	 //custom clipping

	 if (box_crap.left < 0)
	 {
       box_src.left += (box_crap.left - (box_crap.left * 2));
	   box_crap.left = 0;
	 }

	if (box_crap.top < 0)
	 {
       box_src.top += (box_crap.top - (box_crap.top * 2));
	   box_crap.top = 0;
	 }

	DDBLTFX fx;
	memset( &fx, 0, sizeof(fx) ); 
		fx.dwSize = sizeof( fx );
	    fx.ddckSrcColorkey.dwColorSpaceHighValue = dw_color;
		fx.ddckSrcColorkey.dwColorSpaceLowValue = dw_color;


	 ddrval = back->Blt( &box_crap, pic->pic, &box_src, DDBLT_KEYSRCOVERRIDE | DDBLT_WAIT
		 , &fx);
	if (ddrval != DD_OK) dderror(ddrval);
}


/* Blits a solid block from one surface to another using RECT structs - if the
   RECTS are different sizes, the image will be scaled to fit */

void blit_block(IDirectDrawSurface7 *primary, RECT Dest, IDirectDrawSurface7 *back, RECT Source)
{
DDBLTFX     ddbltfx;


	ddbltfx.dwSize = sizeof(ddbltfx);
	 ddbltfx.dwDDFX = DDBLTFX_NOTEARING;
	
	int ddrval = primary->Blt( &Dest, back, &Source,  DDBLT_DDFX | DDBLT_WAIT, &ddbltfx);
	if (ddrval != DD_OK) dderror(ddrval);

}


/* Same as above but has the KEYSRC flag, works with transparency */

void blit_stretch(IDirectDrawSurface7 *primary, RECT Dest, IDirectDrawSurface7 *back,
				  RECT Source)
{
	int ddrval = primary->Blt( &Dest, back, &Source,DDBLT_KEYSRC| DDBLT_WAIT, NULL);
	if (ddrval != DD_OK) dderror(ddrval);

}



 void draw_box(IDirectDrawSurface7 *surface, RECT box, int r, int g, int b)
{
	HRESULT             ddrval;
	DDBLTFX     ddbltfx;
	memset( &ddbltfx, 0, sizeof(ddbltfx) ); 
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = RGB(r,g,b);
	again:
	ddrval = surface->Blt(&box ,NULL, NULL, DDBLT_COLORFILL| DDBLT_WAIT , &ddbltfx);
    if (ddrval == DDERR_WASSTILLDRAWING) goto again;
    if (ddrval == DDERR_SURFACELOST)
		  {
		//let's replace the surface
		ddrval = surface->Restore();
	}
	if (ddrval != DD_OK) dderror(ddrval);
}


void Say(IDirectDrawSurface7 *lpDDSBack, char thing[500], int px, int py, int color)
{
	RECT rcRect;
    HDC         hdc;
	if (lpDDSBack->GetDC(&hdc) == DD_OK)
	{      
		SetBkMode(hdc, TRANSPARENT); 
				SetRect(&rcRect,px,py,px+200,py+40);

		//SelectObject (hdc, hfont_small);
        //if (hfont == NULL) Msg("CreafontIndirect failed. haha.");
		SetTextColor(hdc,color);
		DrawText(hdc,thing,lstrlen(thing),&rcRect,DT_WORDBREAK);	
		
		lpDDSBack->ReleaseDC(hdc);
	}   
	
	
}


void Say_extended(IDirectDrawSurface7 *lpDDSBack, char thing[500], int px, int py, int px2, int py2, int color)
{
	RECT rcRect;
    HDC         hdc;
	
	if (lpDDSBack->GetDC(&hdc) == DD_OK)
	{      
		SetBkMode(hdc, TRANSPARENT); 
				SetRect(&rcRect,px,py,px2,py2);

		//SelectObject (hdc, hfont_small);
        //if (hfont == NULL) Msg("CreafontIndirect failed. haha.");
		SetTextColor(hdc,color);
		DrawText(hdc,thing,lstrlen(thing),&rcRect,DT_WORDBREAK);	
		
		lpDDSBack->ReleaseDC(hdc);
	}   else Msg("Couldn't get the HDC.. ");
	
	
}



IDirectDrawSurface7 * init_surface(IDirectDraw7 *lpDD, int x, int y)
{
		DDSURFACEDESC2       ddsd;
		HRESULT             ddrval;
    	LPDIRECTDRAWSURFACE7  lpSurf; 
        LPDIRECTDRAWSURFACE7  lpDDSBack; 
		memset( &ddsd, 0, sizeof(ddsd) ); 
		ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; 
		ddsd.dwWidth = x;
		ddsd.dwHeight = y;    // create the backbuffer separately
		
		if (lpDD == NULL) 
		{
			Msg("Can't create surface, DD main is not initted!");
		return(NULL);
		}
			ddrval = lpDD->CreateSurface( &ddsd, &lpSurf, NULL );

				
		if( ddrval != DD_OK )
		{   
		  
			Msg("Failed creating ddraw surface..");
			dderror(ddrval);
			
			return(NULL);
		}   


		ddrval = lpSurf->QueryInterface( IID_IDirectDrawSurface7, (void **)&lpDDSBack); 
    
		
		if( ddrval != DD_OK )
		{   
		     Msg("Failed creating ddraw surface..");
			return(NULL);
		}   
 
ddrval = lpDDSBack->PageLock(0); 
		
		if( ddrval != DD_OK )
		{   
		     Msg("Failed creating ddraw surface..");
			return(NULL);
		}   
 
ddrval = lpDDSBack->PageUnlock(0); 
		
		if( ddrval != DD_OK )
		{   
		     Msg("Failed creating ddraw surface..");
			return(NULL);
		}   

lpSurf->Release();


if (lpDDSBack == NULL) Msg("But it equals null, unfortunatly.");
		return(lpDDSBack);
}

IDirectDraw7 * init_draw_primary(HWND hwnd)
{
    LPDIRECTDRAW crap;
    LPDIRECTDRAW7 lpDD;
    HRESULT             ddrval;

	ddrval = DirectDrawCreate( NULL, &crap, NULL );
    if( ddrval != DD_OK )
    {
        show(hwnd,"Display error", "Couldn't use DirectX 7+...  Install it first.");
		return(NULL);
    }
	
       // using DDSCL_NORMAL means we will coexist with GDI
		ddrval = crap->SetCooperativeLevel( hwnd, DDSCL_NORMAL );


		if( ddrval != DD_OK )  
		{        
		crap->Release(); 
        show(hwnd, "Display error", "Couldn't make windowed screen.");
		return(NULL);
		}   
		    //convert to id2 interface
		ddrval = crap->QueryInterface(IID_IDirectDraw7, 
       (void **)&lpDD); 
 
		if( ddrval != DD_OK )    
		{ 
			lpDD->Release(); 
	        show(hwnd, "Display error", "Couldn't make primary surface.");
	        return(NULL);
	
		} 

		
		crap->Release(); 
       	return(lpDD);

}

IDirectDraw7 * init_draw_primary_exlusive(HWND hwnd)
{
    LPDIRECTDRAW crap;
    LPDIRECTDRAW7 lpDD;
    HRESULT             ddrval;

	ddrval = DirectDrawCreate( NULL, &crap, NULL );
    if( ddrval != DD_OK )
    {
        show(hwnd,"Display error", "Couldn't use DirectX 7+...  Install it first.");
		return(NULL);
    }
	
       // using DDSCL_NORMAL means we will coexist with GDI
		ddrval = crap->SetCooperativeLevel( hwnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE );


		if( ddrval != DD_OK )  
		{        
		crap->Release(); 
        show(hwnd, "Display error", "Couldn't make windowed screen.");
		return(NULL);
		}   
		    //convert to id2 interface
		ddrval = crap->QueryInterface(IID_IDirectDraw7, 
       (void **)&lpDD); 
 
		if( ddrval != DD_OK )    
		{ 
			lpDD->Release(); 
	        show(hwnd, "Display error", "Couldn't make primary surface.");
	        return(NULL);
	
		} 

		
		crap->Release(); 
       	return(lpDD);

}




IDirectDrawSurface7 * init_surface_primary(IDirectDraw7 *lpDD, HWND hwnd)
{

	    DDSURFACEDESC2       ddsd;
    HRESULT             ddrval;
    
    LPDIRECTDRAWSURFACE7  lpSurf; 
    LPDIRECTDRAWSURFACE7  lpDDSPrimary; 

				
	memset( &ddsd, 0, sizeof(ddsd) );
		ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		// The primary surface is not a page flipping surface this time
		ddrval = lpDD->CreateSurface( &ddsd, &lpSurf, NULL );

		if( ddrval != DD_OK )    
		{ 
			lpDD->Release();  
	        show(hwnd, "Display error", "Couldn't make primary surface.");
	        return NULL;
			} 
 	       

		ddrval = lpSurf->QueryInterface( 
    IID_IDirectDrawSurface7, (void **)&lpDDSPrimary); 
//old   IID_IDirectDrawSurface3, (LPVOID *)&lpDDSPrimary); 

	//(void **)&lpDD); 
 	
		if( ddrval != DD_OK )
		{   
		     Msg("Failed creating ddraw surface. (Query Interface).");
			return NULL;
		}   
 
ddrval = lpDDSPrimary->PageLock(0); 
		
		if( ddrval != DD_OK )
		{   
		     Msg("Failed creating ddraw surface..(Query Interface)");
			return NULL;
		}   
 
ddrval = lpDDSPrimary->PageUnlock(0); 
		
		if( ddrval != DD_OK )
		{   
		     Msg("Failed creating ddraw surface..");
			return NULL;
		}   

        lpSurf->Release();

        return lpDDSPrimary;

}


IDirectDrawSurface7 * init_surface_double_buffer(IDirectDraw7 *lpDD, HWND hwnd)
{

	DDSURFACEDESC2       ddsd;
    HRESULT             ddrval;
    
    LPDIRECTDRAWSURFACE7  lpSurf; 
    LPDIRECTDRAWSURFACE7  lpDDSPrimary; 

				
	memset( &ddsd, 0, sizeof(ddsd) );
	ddsd.dwSize = sizeof( ddsd );
		
	    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP |
			                  DDSCAPS_COMPLEX;
		ddsd.dwBackBufferCount = 1;

		// The primary surface is not a page flipping surface this time
		ddrval = lpDD->CreateSurface( &ddsd, &lpSurf, NULL );

		if( ddrval != DD_OK )    
		{ 
			lpDD->Release();  
	        show(hwnd, "Display error", "Couldn't make primary surface.");
	        return NULL;
			} 
 	       
		
		ddrval = lpSurf->QueryInterface( 
        IID_IDirectDrawSurface7, (void **)&lpDDSPrimary); 
 	
		if( ddrval != DD_OK )
		{   
		     Msg("Failed creating ddraw surface. (Query Interface).");
			return NULL;
		}   
        lpSurf->Release();
		 
        return lpDDSPrimary;

}



IDirectDrawSurface7 * init_surface_back_buffer(IDirectDraw7 *lpDD, HWND hwnd,
												 IDirectDrawSurface7 *lpDDSPrimary)
{

	DDSURFACEDESC2       ddsd;
    HRESULT             ddrval;
    
    LPDIRECTDRAWSURFACE7  lpDDSBack; 
    LPDIRECTDRAWSURFACE7  lpSurf; 
    			
	memset( &ddsd, 0, sizeof(ddsd) );
	ddsd.dwSize = sizeof( ddsd );
		
	    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP |
			                  DDSCAPS_COMPLEX;
		ddsd.dwBackBufferCount = 1;
		
		ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;

		if (FAILED(lpDDSPrimary->GetAttachedSurface(&ddsd.ddsCaps, &lpSurf)))
		{
		     Msg("Failed creating back buffer");
		return NULL;
		}

		 ddrval = lpSurf->QueryInterface( 
    IID_IDirectDrawSurface7, (void **)&lpDDSBack); 
 	
		if( ddrval != DD_OK )
		{   
		     Msg("Failed creating backbuffer ddraw surface. (Query Interface).");
			return NULL;
		}   
        lpSurf->Release();

		 if (lpDDSBack == NULL)
		 {
		  Msg("Back buffer is null?");
		 }
        return lpDDSBack;

}


IDirectDrawClipper * init_clipper(IDirectDraw7 *lpDD, IDirectDrawSurface7 *lpDDSPrimary 
   , HWND hwnd)

{
    HRESULT             ddrval;
  LPDIRECTDRAWCLIPPER lpClipper;
		
		
		// Create a clipper to ensure that our drawing stays inside our window
		
		ddrval = lpDD->CreateClipper( 0, &lpClipper, NULL );
		if( ddrval != DD_OK )   
		{      			
	      show(hwnd,"Graphics error", "Couldn't make a Clipper object, god knows why.");
        return(false);	
		}
	
	  
		// setting it to our hwnd gives the clipper the coordinates from our window
		ddrval = lpClipper->SetHWnd( 0, hwnd );   
		if( ddrval != DD_OK )  
		{
			lpClipper-> Release();   
			show(hwnd,"Graphics error",  "Couldn't give Clipper window cords.");
	       return(false);
		}
		// attach the clipper to the primary surface
		ddrval = lpDDSPrimary->SetClipper( lpClipper ); 
		if( ddrval != DD_OK )
		{  
			lpClipper-> Release();   
		
			show(hwnd, "Graphics error", "Couldn't attach Clipper to primary buffer.");
		return(false);
		}


	//yay, everything worked
 	return lpClipper;
}





bool prepare_memory(turbo *seq, int count)
{
	while(seq->num < count)
	{
	seq->num++;
    seq->pic[seq->num] = (struct graphics_struct *) malloc( sizeof(struct graphics_struct));
     if (seq->pic[seq->num] == NULL)
	 {
      Msg("Prepare Mem: Can't get mem for a graphic.");
	 }

	 //clear structure
	 memset(seq->pic[seq->num], 0, sizeof(struct graphics_struct));
	}

return(true);
}




/*
 * Creates a DirectDraw palette for a given bitmap on disk.
 * The parameter szBitmap is the file name of the bitmap.
 *
 */
IDirectDrawPalette * DDLoadPalette(IDirectDraw *pdd, LPCSTR szBitmap)
{    int i;
    int n;
    IDirectDrawPalette* ddpal;

    int fh;
    PALETTEENTRY ape[256];

    if (szBitmap && (fh = _lopen(szBitmap, OF_READ)) != -1)
    {
        BITMAPFILEHEADER bf;
        BITMAPINFOHEADER bi;

        _lread(fh, &bf, sizeof(bf));
        _lread(fh, &bi, sizeof(bi));
        _lread(fh, ape, sizeof(ape));
        _lclose(fh);

        if (bi.biSize != sizeof(BITMAPINFOHEADER))
            n = 0;
        else if (bi.biBitCount > 8)
            n = 0;
        else if (bi.biClrUsed == 0)
            n = 1 << bi.biBitCount;
        else
            n = bi.biClrUsed;

      //
      // a DIB color table has its colors stored BGR not RGB
      // so flip them around.
      //
        for(i=0; i<n; i++ )
        {
            BYTE r = ape[i].peRed;
            ape[i].peRed = ape[i].peBlue;
            ape[i].peBlue = r;
        }
    }
    if (pdd->CreatePalette(DDPCAPS_8BIT, ape, &ddpal, NULL) != DD_OK)
    {
        return NULL;
    } else {
        return ddpal;
    }
}





/* map_bmp - this takes a HFASTFILE pointer to a file, an unused int, the main directdraw
object and a box to put the graphic demensions it finds in.  It returns a pointer to the
initted IDirectDrawSurface3 object, or null if it failed. */

IDirectDrawSurface7 * map_bmp(HFASTFILE  * pfile,  int count, IDirectDraw7 *lpDD, RECT * box)
{
	//lets take the bmp from pfile, and put it into the DD surface
	BITMAPFILEHEADER UNALIGNED *pbf;
    BITMAPINFOHEADER UNALIGNED *pbi;
	DDSURFACEDESC2       ddsd;
	
	LPDIRECTDRAWSURFACE7  lptemp; 
	LPDIRECTDRAWSURFACE7  final; 
    RGBQUAD *prgb;
    
   DDCOLORKEY          ddck;
	   
	   
	   pbf = (BITMAPFILEHEADER *)FastFileLock(*pfile, 0, 0);
	   pbi = (BITMAPINFOHEADER *)(pbf+1);
	   
	   if (pbf->bfType != 0x4d42 ||
		   pbi->biSize != sizeof(BITMAPINFOHEADER))
	   {
		   Msg("Failed to load");
		   return NULL;
	   }
	   //Msg("Palette info is %d in size, bitcount is %d, colors are %d", sizeof(ape), 
//		   pbi->biBitCount, pbi->biClrUsed);
	  
	   byte *pic;
	PVOID lpBits;

	prgb = (RGBQUAD *)((LPBYTE)pbi + pbi->biSize);
    lpBits = (LPBYTE)(prgb + pbi->biClrUsed);

    if( pbi->biClrUsed == 0 && pbi->biBitCount <= 8 )
    {
        lpBits = (LPBYTE)(prgb + (1<<pbi->biBitCount));
    }


	   pic = (byte *)pbf + 1078;
	   pic = (byte *)lpBits;
	   Msg("Pic's size is now %d.",sizeof(pic));
	   
	//
	// create a DirectDrawSurface for this bitmap
	//

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth = pbi->biWidth;
	ddsd.dwHeight = pbi->biHeight;
	
	HRESULT	ddrval = lpDD->CreateSurface(&ddsd, &lptemp, NULL);
	if ( ddrval != DD_OK)
	{
		if (ddrval == DDERR_INVALIDPARAMS)
			Msg("Invalid parms");
		if (ddrval == DDERR_INVALIDPIXELFORMAT)		 
			Msg("Invalid Pixel format");
		if (ddrval == DDERR_UNSUPPORTEDMODE )		 
			Msg("Unsupported mode");
		Msg("Failed to create pdd surface description");
		return NULL;
	} else
	{

  //convert to new interface		

	ddrval = lptemp->QueryInterface( IID_IDirectDrawSurface7,(void **) &final); 
		
		if( ddrval != DD_OK )
		{   
		
			if (ddrval == DDERR_INVALIDOBJECT)
				Msg("DDERR_INVALIDOBJECT");			
			if (ddrval == DDERR_INVALIDPARAMS)
				Msg("DDERR_INVALIDPARAMS");			
			if (ddrval == DDERR_OUTOFMEMORY)
				Msg("DDERR_OUTOFMEMORY");			

			Msg("Can't query interface..");
			lptemp->Release();
			return NULL;
		}   
 
	   ddrval = final->PageLock(0); 
		
		if( ddrval != DD_OK )
		{   
		     Msg("Can't lock interface..");
	         lptemp->Release();
			 return NULL;
		}   
 
ddrval = final->PageUnlock(0); 
		
		if( ddrval != DD_OK )
		{   
		     Msg("Can't unlock interface..");
			 lptemp->Release();
			 return NULL;
		}   

lptemp->Release();


//get handle to surface so we can do a stretch to it

    HDC                 hdc;

	if( IDirectDrawSurface3_GetDC(final,&hdc) == DD_OK )
    {
		
		SetStretchBltMode(hdc, COLORONCOLOR);
		int res =   StretchDIBits(hdc, 0, 0,pbi->biWidth, pbi->biHeight,
            0, 0, pbi->biWidth, pbi->biHeight,
            lpBits, (BITMAPINFO *)pbi, DIB_RGB_COLORS, SRCCOPY);
		if (res == -1)
			Msg("Uh oh.. StretchDIBits got an error of %d", GetLastError());
		
        IDirectDrawSurface7_ReleaseDC(final,hdc);
	} else
	{
		Msg("Failed getting DC for image");
		
	}
	
	
			box->top = 0;
			box->left = 0;
			box->right = ddsd.dwWidth;
			box->bottom =ddsd.dwHeight;
			
	
	}


    //here is a thought, let's apply a transparency mask

	ddck.dwColorSpaceLowValue  = RGB(255,255,255);	    
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    final->SetColorKey(DDCKEY_SRCBLT, &ddck);

    //all done, return the new surface

return(final);
}





int load_ff(IDirectDraw7 *lpDD, char name[9], turbo *seq, int count)
{

	HFASTFILE                  pfile;   
 char crap2[5];
char crap[20];	
int start_pic = 1;
 if (count != 0) start_pic = count;
  strcpy(seq->filename, name);	
 if( !FastFileInit(  "graphics.wad", 5 ) )
    {
        Msg("Couldn't find graphics.wad file.");
		return(0);
	}

	//opened fast file graphics pack, now lets locate the .bmps:
        while(1)
		{
			  //loop forever
			
				//add leading 0 if needed
			if (start_pic < 10) strcpy(crap2, "0"); else strcpy(crap2, "");
    		wsprintf(crap, "%s%s%d.bmp", name,crap2, start_pic);
         
			//open
			pfile = FastFileOpen((LPSTR) crap);
			if( pfile == NULL )
			{
			   if (start_pic == 1)
			   {
               Msg("Load_ff: Couldn't load %s.", crap);
			   }
				//file doesn't exist in pack
				FastFileClose( pfile );
				FastFileFini();
			      return(1);
			}

          if ( prepare_memory(seq, start_pic))
		  {
              //memory is ok, go for it
			  
			  if (seq->pic[start_pic]->pic != NULL) seq->pic[start_pic]->pic->Release();
   		 seq->pic[start_pic]->pic = map_bmp(&pfile, count, lpDD, &seq->pic[start_pic]->box);
		 //let this individual pic know it's origin
		 seq->pic[start_pic]->turbo = (void **) seq;

		   }
		  else
		  {
		        FastFileClose( pfile );
				FastFileFini();
			      return(1);
		
		  }

          if (count == start_pic)
		  {
          //got the one pic ok
			  FastFileClose( pfile );
				FastFileFini();
		     return(1);
		  }
              	FastFileClose( pfile );

		  start_pic++;
		}

	FastFileFini();

return(1);

	

}



void dderror(HRESULT hErr)
{       
    switch (hErr)
    {
	case DDERR_ALREADYINITIALIZED:
		Msg("DDERR_ALREADYINITIALIZED"); break;
	case DDERR_CANNOTATTACHSURFACE:
		Msg("DDERR_CANNOTATTACHSURFACE"); break;
	case DDERR_CANNOTDETACHSURFACE:
		Msg("DDERR_CANNOTDETACHSURFACE"); break;
	case DDERR_CURRENTLYNOTAVAIL:
		Msg("DDERR_CURRENTLYNOTAVAIL"); break;
	case DDERR_EXCEPTION:
		Msg("DDERR_EXCEPTION"); break;
	case DDERR_GENERIC:
		Msg("DDERR_GENERIC"); break;
	case DDERR_HEIGHTALIGN:
		Msg("DDERR_HEIGHTALIGN"); break;
	case DDERR_INCOMPATIBLEPRIMARY:
		Msg("DDERR_INCOMPATIBLEPRIMARY"); break;
	case DDERR_INVALIDCAPS:
		Msg("DDERR_INVALIDCAPS"); break;
	case DDERR_INVALIDCLIPLIST:
		Msg("DDERR_INVALIDCLIPLIST"); break;
	case DDERR_INVALIDMODE:
		Msg("DDERR_INVALIDMODE"); break;
	case DDERR_INVALIDOBJECT:
		Msg("DDERR_INVALIDOBJECT"); break;
	case DDERR_INVALIDPARAMS:
		Msg("DDERR_INVALIDPARAMS"); break;
	case DDERR_INVALIDPIXELFORMAT:
		Msg("DDERR_INVALIDPIXELFORMAT"); break;
	case DDERR_INVALIDRECT:
		Msg("DDERR_INVALIDRECT"); break;
	case DDERR_LOCKEDSURFACES:
		Msg("DDERR_LOCKEDSURFACES"); break;
	case DDERR_NO3D:
		Msg("DDERR_NO3D"); break;
	case DDERR_NOALPHAHW:
		Msg("DDERR_NOALPHAHW"); break;
	case DDERR_NOCLIPLIST:
		Msg("DDERR_NOCLIPLIST"); break;
	case DDERR_NOCOLORCONVHW:
		Msg("DDERR_NOCOLORCONVHW"); break;
	case DDERR_NOCOOPERATIVELEVELSET:
		Msg("DDERR_NOCOOPERATIVELEVELSET"); break;
	case DDERR_NOCOLORKEY:
		Msg("DDERR_NOCOLORKEY"); break;
	case DDERR_NOCOLORKEYHW:
		Msg("DDERR_NOCOLORKEYHW"); break;
	case DDERR_NODIRECTDRAWSUPPORT:
		Msg("DDERR_NODIRECTDRAWSUPPORT"); break;
	case DDERR_NOEXCLUSIVEMODE:
		Msg("DDERR_NOEXCLUSIVEMODE"); break;
	case DDERR_NOFLIPHW:
		Msg("DDERR_NOFLIPHW"); break;
	case DDERR_NOGDI:
		Msg("DDERR_NOGDI"); break;
	case DDERR_NOMIRRORHW:
		Msg("DDERR_NOMIRRORHW"); break;
	case DDERR_NOTFOUND:
		Msg("DDERR_NOTFOUND"); break;
	case DDERR_NOOVERLAYHW:
		Msg("DDERR_NOOVERLAYHW"); break;
	case DDERR_NORASTEROPHW:
		Msg("DDERR_NORASTEROPHW"); break;
	case DDERR_NOROTATIONHW:
		Msg("DDERR_NOROTATIONHW"); break;
	case DDERR_NOSTRETCHHW:
		Msg("DDERR_NOSTRETCHHW"); break;
	case DDERR_NOT4BITCOLOR:
		Msg("DDERR_NOT4BITCOLOR"); break;
	case DDERR_NOT4BITCOLORINDEX:
		Msg("DDERR_NOT4BITCOLORINDEX"); break;
	case DDERR_NOT8BITCOLOR:
		Msg("DDERR_NOT8BITCOLOR"); break;
	case DDERR_NOTEXTUREHW:
		Msg("DDERR_NOTEXTUREHW"); break;
	case DDERR_NOVSYNCHW:
		Msg("DDERR_NOVSYNCHW"); break;
	case DDERR_NOZBUFFERHW:
		Msg("DDERR_NOZBUFFERHW"); break;
	case DDERR_NOZOVERLAYHW:
		Msg("DDERR_NOZOVERLAYHW"); break;
	case DDERR_OUTOFCAPS:
		Msg("DDERR_OUTOFCAPS"); break;
	case DDERR_OUTOFMEMORY:
		Msg("DDERR_OUTOFMEMORY"); break;
	case DDERR_OUTOFVIDEOMEMORY:
		Msg("DDERR_OUTOFVIDEOMEMORY"); break;
	case DDERR_OVERLAYCANTCLIP:
		Msg("DDERR_OVERLAYCANTCLIP"); break;
	case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
		Msg("DDERR_OVERLAYCOLORKEYONLYONEACTIVE"); break;
	case DDERR_PALETTEBUSY:
		Msg("DDERR_PALETTEBUSY"); break;
	case DDERR_COLORKEYNOTSET:
		Msg("DDERR_COLORKEYNOTSET"); break;
	case DDERR_SURFACEALREADYATTACHED:
		Msg("DDERR_SURFACEALREADYATTACHED"); break;
	case DDERR_SURFACEALREADYDEPENDENT:
		Msg("DDERR_SURFACEALREADYDEPENDENT"); break;
	case DDERR_SURFACEBUSY:
		Msg("DDERR_SURFACEBUSY"); break;
	case DDERR_CANTLOCKSURFACE:
		Msg("DDERR_CANTLOCKSURFACE"); break;
	case DDERR_SURFACEISOBSCURED:
		Msg("DDERR_SURFACEISOBSCURED"); break;
	case DDERR_SURFACELOST:
		Msg("DDERR_SURFACELOST"); break;
	case DDERR_SURFACENOTATTACHED:
		Msg("DDERR_SURFACENOTATTACHED"); break;
	case DDERR_TOOBIGHEIGHT:
		Msg("DDERR_TOOBIGHEIGHT"); break;
	case DDERR_TOOBIGSIZE:
		Msg("DDERR_TOOBIGSIZE"); break;
	case DDERR_TOOBIGWIDTH:
		Msg("DDERR_TOOBIGWIDTH"); break;
	case DDERR_UNSUPPORTED:
		Msg("DDERR_UNSUPPORTED"); break;
	case DDERR_UNSUPPORTEDFORMAT:
		Msg("DDERR_UNSUPPORTEDFORMAT"); break;
	case DDERR_UNSUPPORTEDMASK:
		Msg("DDERR_UNSUPPORTEDMASK"); break;
	case DDERR_VERTICALBLANKINPROGRESS:
		Msg("DDERR_VERTICALBLANKINPROGRESS"); break;
	case DDERR_WASSTILLDRAWING:
		Msg("DDERR_WASSTILLDRAWING"); break;
	case DDERR_XALIGN:
		Msg("DDERR_XALIGN"); break;
	case DDERR_INVALIDDIRECTDRAWGUID:
		Msg("DDERR_INVALIDDIRECTDRAWGUID"); break;
	case DDERR_DIRECTDRAWALREADYCREATED:
		Msg("DDERR_DIRECTDRAWALREADYCREATED"); break;
	case DDERR_NODIRECTDRAWHW:
		Msg("DDERR_NODIRECTDRAWHW"); break;
	case DDERR_PRIMARYSURFACEALREADYEXISTS:
		Msg("DDERR_PRIMARYSURFACEALREADYEXISTS"); break;
	case DDERR_NOEMULATION:
		Msg("DDERR_NOEMULATION"); break;
	case DDERR_REGIONTOOSMALL:
		Msg("DDERR_REGIONTOOSMALL"); break;
	case DDERR_CLIPPERISUSINGHWND:
		Msg("DDERR_CLIPPERISUSINGHWND"); break;
	case DDERR_NOCLIPPERATTACHED:
		Msg("DDERR_NOCLIPPERATTACHED"); break;
	case DDERR_NOHWND:
		Msg("DDERR_NOHWND"); break;
	case DDERR_HWNDSUBCLASSED:
		Msg("DDERR_HWNDSUBCLASSED"); break;
	case DDERR_HWNDALREADYSET:
		Msg("DDERR_HWNDALREADYSET"); break;
	case DDERR_NOPALETTEATTACHED:
		Msg("DDERR_NOPALETTEATTACHED"); break;
	case DDERR_NOPALETTEHW:
		Msg("DDERR_NOPALETTEHW"); break;
	case DDERR_BLTFASTCANTCLIP:
		Msg("DDERR_BLTFASTCANTCLIP"); break;
	case DDERR_NOBLTHW:
		Msg("DDERR_NOBLTHW"); break;
	case DDERR_NODDROPSHW:
		Msg("DDERR_NODDROPSHW"); break;
	case DDERR_OVERLAYNOTVISIBLE:
		Msg("DDERR_OVERLAYNOTVISIBLE"); break;
	case DDERR_NOOVERLAYDEST:
		Msg("DDERR_NOOVERLAYDEST"); break;
	case DDERR_INVALIDPOSITION:
		Msg("DDERR_INVALIDPOSITION"); break;
	case DDERR_NOTAOVERLAYSURFACE:
		Msg("DDERR_NOTAOVERLAYSURFACE"); break;
	case DDERR_EXCLUSIVEMODEALREADYSET:
		Msg("DDERR_EXCLUSIVEMODEALREADYSET"); break;
	case DDERR_NOTFLIPPABLE:
		Msg("DDERR_NOTFLIPPABLE"); break;
	case DDERR_CANTDUPLICATE:
		Msg("DDERR_CANTDUPLICATE"); break;
	case DDERR_NOTLOCKED:
		Msg("DDERR_NOTLOCKED"); break;
	case DDERR_CANTCREATEDC:
		Msg("DDERR_CANTCREATEDC"); break;
	case DDERR_NODC:
		Msg("DDERR_NODC"); break;
	case DDERR_WRONGMODE:
		Msg("DDERR_WRONGMODE"); break;
	case DDERR_IMPLICITLYCREATED:
		Msg("DDERR_IMPLICITLYCREATED"); break;
	case DDERR_NOTPALETTIZED:
		Msg("DDERR_NOTPALETTIZED"); break;
	case DDERR_UNSUPPORTEDMODE:
		Msg("DDERR_UNSUPPORTEDMODE"); break;
	case DDERR_NOMIPMAPHW:
		Msg("DDERR_NOMIPMAPHW"); break;
	case DDERR_INVALIDSURFACETYPE:
		Msg("DDERR_INVALIDSURFACETYPE"); break;
	case DDERR_DCALREADYCREATED:
		Msg("DDERR_DCALREADYCREATED"); break;
	case DDERR_CANTPAGELOCK:
		Msg("DDERR_CANTPAGELOCK"); break;
	case DDERR_CANTPAGEUNLOCK:
		Msg("DDERR_CANTPAGEUNLOCK"); break;
	case DDERR_NOTPAGELOCKED:
		Msg("DDERR_NOTPAGELOCKED"); break;
	case DDERR_NOTINITIALIZED:
		Msg("DDERR_NOTINITIALIZED"); break;
	default:
		Msg("Unknown Error"); break;
	}
	Msg("\n");
}
