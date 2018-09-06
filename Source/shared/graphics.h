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

struct graphics_struct
{		
	LPDIRECTDRAWSURFACE7     pic;       //graphic data
	RECT                box;	//the box	
    int x; //x offset, optional
	int y; //y offset, optional
	int special; //for misc
	LPVOID *turbo;
};

struct turbo
{
int num; //how many graphics are in this structure
graphics_struct *pic[20];
char filename[9];
};



int getx( void );
int gety( void );
void window_set_exact_size(HWND hw, int my_x, int my_y);
void turbo_blit(IDirectDrawSurface7 *primary, IDirectDrawSurface7 *back);
LPDIRECTDRAWCLIPPER DDraw_Attach_Clipper(LPDIRECTDRAW7 lpdd7, LPDIRECTDRAWSURFACE7 lpdds,
                                         int num_rects,
                                         LPRECT clip_list);
void blit_normal(IDirectDrawSurface7 *back, graphics_struct * pic, int left, int top,
				 IDirectDraw7 *lpDD);
void blit_normal_custom_color_key(IDirectDrawSurface7 *back, graphics_struct * pic, int left, int top, DWORD dw_color);
void blit_block(IDirectDrawSurface7 *primary, RECT Dest, IDirectDrawSurface7 *back, RECT Source);
void blit_stretch(IDirectDrawSurface7 *primary, RECT Dest, IDirectDrawSurface7 *back,
				  RECT Source);
void draw_box(IDirectDrawSurface7 *surface, RECT box, int r, int g, int b);
void Say(IDirectDrawSurface7 *lpDDSBack, char thing[500], int px, int py, int color);
void Say_extended(IDirectDrawSurface7 *lpDDSBack, char thing[500], int px, int py, int px2, int py2, int color);
IDirectDrawSurface7 * init_surface(IDirectDraw7 *lpDD, int x, int y);
IDirectDraw7 * init_draw_primary(HWND hwnd);
IDirectDraw7 * init_draw_primary_exlusive(HWND hwnd);
IDirectDrawSurface7 * init_surface_primary(IDirectDraw7 *lpDD, HWND hwnd);
IDirectDrawSurface7 * init_surface_double_buffer(IDirectDraw7 *lpDD, HWND hwnd);
IDirectDrawSurface7 * init_surface_back_buffer(IDirectDraw7 *lpDD, HWND hwnd,
												 IDirectDrawSurface7 *lpDDSPrimary);
IDirectDrawClipper * init_clipper(IDirectDraw7 *lpDD, IDirectDrawSurface7 *lpDDSPrimary 
   , HWND hwnd);
bool prepare_memory(turbo *seq, int count);
IDirectDrawPalette * DDLoadPalette(IDirectDraw *pdd, LPCSTR szBitmap);
IDirectDrawSurface7 * map_bmp(HFASTFILE  * pfile,  int count, IDirectDraw7 *lpDD, RECT * box);
int load_ff(IDirectDraw7 *lpDD, char name[9], turbo *seq, int count);
void dderror(HRESULT hErr);
