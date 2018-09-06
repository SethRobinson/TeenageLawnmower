/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#ifndef __GDP_SOUND_MANAGER_DX_H__
#define __GDP_SOUND_MANAGER_DX_H__

//#define INITGUID
#include "..\\ISoundManager.h" //this interface we should be accessed through, rather than this one
#include <dmusici.h>
#include "gdpAudioPath.h" //manager of our effect channels
#include "gdpSound.h" //holds a sound instance
#include "gdpSoundMap.h" //this helps us control sound instances with an stl map
#include "gdpDirectShow.h"

namespace gdp
{

    class SoundManagerDX : public ISoundManager 
{

public:
	SoundManagerDX();
	virtual ~SoundManagerDX();

	virtual bool Init();
	virtual void Kill();
	virtual bool IsActive();
	virtual u32 GetGlobalVolume();
	virtual void SetGlobalVolume(u32 uVolume);

	// sound controls
    virtual hSoundInstance Play(const char stName[]);
    virtual hSoundInstance PlayMixed(const char stName[]); //like above, but forces separate channel
    virtual hSoundInstance PlayLoop(const char stName[]);
	virtual void StopSound(hSoundInstance);
	virtual u32 StopAllSounds(); //returns the number of sounds killed
	virtual u32 StopSoundsByResource(hSoundResource sndResource);
    virtual void ClearCache();

	//resource management
	virtual void PreloadSound(const char stName[]);
	virtual void UnloadAllResources();
	virtual void UnloadResource(hSoundResource sndResource);

	//debug stuff
	
	//call this in a while() loop until it returns false, it will walk through each
	//resource loaded.
	virtual bool GetResourcesLoaded(const char *out_pstName, hSoundResource *out_SoundResource);
	virtual u32 GetSoundsCurrentlyPlaying();
    virtual eSoundSystem GetSoundSystem() {return SOUND_SYSTEM_DIRECTMUSIC;};
 
    virtual hSoundInstance PlayMusic(const char stName[], int i_repeats);
    virtual void StopMusic();


	//dealing with sound instances
	virtual bool SoundInstanceSetEffect(hSoundInstance hSound, u32 uEffectType, bool bActive, int iSetting);
	virtual bool SoundInstancePlay(hSoundInstance hSound, int iCount);
	virtual hSoundInstance SoundInstanceCreateAndLoad(const char stName[] );
	virtual bool SoundInstanceLoad(hSoundInstance hSound, const char stName[] );
    virtual bool SoundInstanceSetPitch(hSoundInstance hSound, int iPitch);
	virtual bool SoundInstanceStop(hSoundInstance hSound);
	virtual bool SoundInstanceSetForceMainSegment(hSoundInstance hSound, bool b_new); //set to true and no sounds can overlap
	virtual bool SoundInstanceSetStayAlive(hSoundInstance hSound, bool b_new); //won't auto delete if true
	virtual bool SoundInstanceKill(hSoundInstance hSound); //kills a sound no matter what

    hSoundInstance SoundInstanceCreate(); //used internally
    hSoundInstance SoundInstanceCreateAndLoad3D(hSoundInstance hOldSound, char stName[]);


	//3d specific functions
	virtual hSoundInstance SoundInstanceCreate3D();
    virtual bool SoundInstance3DSetPos(hSoundInstance hSound, float f_x, float f_y, float f_z);
    virtual bool SoundInstance3DSetDistance(hSoundInstance hSound, float f_min, float f_max);

	
	virtual void ListenerSetPos(float f_x, float f_y, float f_z);
	virtual void ListenerSetOrientation(float f_x, float f_y, float f_z);
    virtual void ListenerSetPosVelAndOrientation(float *a_pos, float *a_vel, float *a_dir);
    

	//misc
	virtual void UpdateSounds();
    virtual hSoundInstance GetMusicInstance(){return hMusicHandle;};
    virtual bool SoundExists(hSoundInstance h_sound);
    virtual void SetMetersPerUnit(float f_distance_factor); //for 3d sounds only
    virtual void SetRollOffFactor(float f_amount); //for 3d sounds only
    
    
private:
    
    IDirectSound *m_pDSound; //maing DSound object we're using
    IDirectMusic* m_pDirectMusic; //main direct music object we're using
    IDirectMusicLoader8*  m_pLoader;
    IDirectMusicPerformance8* m_pPerformance;
    IDirectMusicPerformance8* m_pPerformanceMusic;
    SoundMap m_SoundMap;
    u32 hMusicHandle, hMainSegmentHandle; //handle to our main segment
    
    IDirectSound3DListener8* m_pDSListener;	 //where our ears are if we're using 3d sound
    
};

};

#endif
