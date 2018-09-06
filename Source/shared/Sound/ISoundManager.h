//Interface class for our sound system, so we can easily switch the sound implentation


#ifndef _ISOUNDMANAGER_H
#define _ISOUNDMANAGER_H

#include "SoundUtil.h" //just has some types and things we need
#include "assert.h"


class ISoundManager
{
public:
    ISoundManager();
    virtual ~ISoundManager();
    
    virtual bool Init() = 0;
    virtual void Kill() = 0;
    
   	virtual bool IsActive() = 0;

	virtual u32 GetGlobalVolume() = 0;
	virtual void SetGlobalVolume(u32 uVolume) = 0;

	// sound controls
    virtual hSoundInstance Play(const char stName[]) = 0;
    virtual hSoundInstance PlayMixed(const char stName[]) = 0; //like above, but forces separate channel
    virtual hSoundInstance PlayLoop(const char stName[]) = 0;
	virtual void StopSound(hSoundInstance) = 0;
	virtual u32 StopAllSounds() = 0; //returns the number of sounds killed
	virtual u32 StopSoundsByResource(hSoundResource sndResource) = 0;
    virtual void ClearCache() = 0;

	//resource management
	virtual void PreloadSound(const char stName[]) = 0;
    virtual void Preload3DSound(const char stName[]) = 0;
	virtual void UnloadAllResources() = 0;
	virtual void UnloadResource(hSoundResource sndResource) = 0;

	//debug stuff
	
	//call this in a while() loop until it returns false, it will walk through each
	//resource loaded.
	virtual bool GetResourcesLoaded(char *out_pstName, hSoundResource *out_SoundResource) = 0;
	virtual u32 GetSoundsCurrentlyPlaying() = 0;
 	virtual eSoundSystem GetSoundSystem()=0; //are we FMOD, DX, or what?

    virtual hSoundInstance PlayMusic(const char stName[], int i_repeats) = 0;
    virtual void StopMusic() = 0;


	//dealing with sound instances
	virtual bool SoundInstanceSetEffect(hSoundInstance hSound, u32 uEffectType, bool bActive, int iSetting) = 0;
	virtual bool SoundInstancePlay(hSoundInstance hSound, int iCount) = 0;
    virtual hSoundInstance  SoundInstanceCreateAndLoad(const char stName[] ) = 0;
   
    virtual bool SoundInstanceSetPitch(hSoundInstance hSound, int iPitch) = 0;
	virtual bool SoundInstanceStop(hSoundInstance hSound) = 0;
	virtual bool SoundInstanceSetForceMainSegment(hSoundInstance hSound, bool b_new) = 0; //set to true and no sounds can overlap
	virtual bool SoundInstanceSetStayAlive(hSoundInstance hSound, bool b_new) = 0; //won't auto delete if true
	virtual bool SoundInstanceKill(hSoundInstance hSound) = 0; //kills a sound no matter what


	//3d specific functions
	virtual hSoundInstance SoundInstanceCreateAndLoad3D(hSoundInstance hSoundToUse, const char stName[]) = 0;
    virtual bool SoundInstance3DSetPos(hSoundInstance hSound, float f_x, float f_y, float f_z) = 0;
    virtual bool SoundInstance3DSetDistance(hSoundInstance hSound, float f_min, float f_max) = 0;

	
	virtual void ListenerSetPos(float f_x, float f_y, float f_z) = 0;
	virtual void ListenerSetOrientation(float f_x, float f_y, float f_z) = 0;
    
    //faster than setting them individually, especially optimized for FMOD.  Each parm should be
    //a pointer to 3 floats (a vector3)
    virtual void ListenerSetPosVelAndOrientation(float *a_pos, float *a_vel, float *a_dir) = 0;

	//misc
	virtual void UpdateSounds() = 0;
    virtual hSoundInstance GetMusicInstance() = 0;
    virtual bool SoundExists(hSoundInstance h_sound) = 0;
    virtual void SetMetersPerUnit(float f_distance_factor) = 0; //for 3d sounds only
    virtual void SetRollOffFactor(float f_amount) = 0; //for 3d sounds only
    virtual void SetMute(bool b_new) = 0;
   
private:
    
    
};

#endif