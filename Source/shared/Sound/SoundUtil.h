// ============================================================================
// DESCRIPTION: Community GameDev Project - Global sound utilities
// AUTHOR:      mrfun
// DATE:        2002-02-15
// Misc defines/function needed by multiple sound files
// ============================================================================
#ifndef __GDP_SOUND_UTIL_H__
#define __GDP_SOUND_UTIL_H__


const int C_SOUND_NONE = -1;

typedef unsigned long int u32;

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

const u32 C_SOUND_EFFECT_ECHO = 1;
const u32 C_SOUND_EFFECT_REVERB = 2;
const u32 C_SOUND_EFFECT_CHORUS = 3;
const u32 C_SOUND_EFFECT_DISTORTION = 4;

typedef int hSoundInstance;	//handle to sound instances
typedef int hSoundResource;	//handle to sound resources

enum eSoundSystem
{
  SOUND_SYSTEM_FMOD,
  SOUND_SYSTEM_DIRECTMUSIC,

};


#endif


