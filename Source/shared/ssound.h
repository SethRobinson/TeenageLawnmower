#pragma once

#define WIN32_LEAN_AND_MEAN  

#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>
#include <iostream.h> // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>

#include <ddraw.h>  // directX includes
#include <dsound.h>

#include "all.h"

#define SOUND_NULL     0 // " "
#define SOUND_LOADED   1
#define SOUND_PLAYING  2
#define SOUND_STOPPED  3

typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

// this holds a single sound
typedef struct pcm_sound_typ
	{
	LPDIRECTSOUNDBUFFER dsbuffer;   // the ds buffer containing the sound
	int state;                      // state of the sound
	int rate;                       // playback rate
	int size;                       // size of sound
	int id;                         // id number of the sound
	} pcm_sound, *pcm_sound_ptr;


#define MAX_SOUNDS     256 // max number of sounds in system at once 

extern pcm_sound sound_fx[MAX_SOUNDS];    // the array of secondary sound buffers

bool init_direct_sound(HWND hw);
void dsound_play(int sound_id);
void kill_direct_sound (void);
int DSound_Load_WAV(char *filename, int control_flags);

