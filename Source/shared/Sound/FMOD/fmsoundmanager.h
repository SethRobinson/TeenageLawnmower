#pragma once

#include <fmod.h>
#include <vector>
#include <string>
#include "..\\..\\..\\shared\\all.h"
#include "..\\ISoundManager.h"

using namespace std;


class FMSound
{
public:
    FMSound()
    {
        p_sample = NULL;
    }
    FSOUND_SAMPLE *p_sample;
    string st_fname;
};



class FMSoundManager: public ISoundManager
{
public:

    FMSoundManager();
    ~FMSoundManager();
    
    virtual hSoundInstance PlayMixed(const char p_fname[]);
    virtual hSoundInstance Play(const char p_fname[]);
    virtual hSoundInstance PlayMusic(const char p_fname[], int i_repeat);
    
    virtual bool Init();
    virtual void UpdateSounds() {FSOUND_Update();}; //called every frame, only need for fading/3d
   

    virtual void Kill(); //kills everything, unloads all sounds and main driver
    virtual void StopMusic(); //unloads just the music

    FMSound * FMPreloadSound(const char *p_fname, bool b_3d_sound);
    virtual hSoundInstance PlayLoop(const char p_fname[]);
    virtual void KillChannel(int i_channel);
  
    virtual bool IsActive(){return m_b_ready;}
  
    //not implemented

    virtual u32 GetGlobalVolume() {LogMsg("Not Implemented yet"); return 0;};
    virtual void SetGlobalVolume(u32 uVolume) {LogMsg("Not Implemented yet");};

    	// sound controls
   	virtual void StopSound(hSoundInstance);
	virtual u32 StopAllSounds(); //returns the number of sounds killed
	virtual u32 StopSoundsByResource(hSoundResource sndResource) {LogMsg("StopSoundsByResource: Not Implemented yet"); return 0;};
    virtual void ClearCache() {LogMsg("ClearCache: Not Implemented yet");};

	//resource management
	virtual void PreloadSound(const char stName[]) {FMPreloadSound(stName, false);};
    virtual void Preload3DSound(const char stName[]) {FMPreloadSound(stName, true); };
	virtual void UnloadAllResources() {LogMsg("UnloadAllResources: Not Implemented yet"); };
	virtual void UnloadResource(hSoundResource sndResource) {LogMsg("UnloadResource: Not Implemented yet"); };

	//debug stuff
	
	//call this in a while() loop until it returns false, it will walk through each
	//resource loaded.
    virtual bool GetResourcesLoaded(char *out_pstName, hSoundResource *out_SoundResource);
	virtual u32 GetSoundsCurrentlyPlaying() {return FSOUND_GetChannelsPlaying();};
	//dealing with sound instances
	virtual hSoundInstance SoundInstanceCreate();

	virtual bool SoundInstanceSetEffect(hSoundInstance hSound, u32 uEffectType, bool bActive, int iSetting) {LogMsg("Not Implemented yet"); return false;};

    hSoundInstance SoundInstanceCreateAndLoad(const char stName[]);

	virtual bool SoundInstancePlay(hSoundInstance hSound, int iCount);
	virtual bool SoundInstanceLoad(hSoundInstance hSound,const char stName[] ) {LogMsg("SoundInstanceLoad: Not Implemented yet"); return false;};
    virtual bool SoundInstanceSetPitch(hSoundInstance hSound, int iPitch);
	virtual bool SoundInstanceStop(hSoundInstance hSound);
	virtual bool SoundInstanceSetForceMainSegment(hSoundInstance hSound, bool b_new) {LogMsg("SoundInstanceSetForceMainSegment: Not Implemented yet"); return false;}; //set to true and no sounds can overlap
	virtual bool SoundInstanceSetStayAlive(hSoundInstance hSound, bool b_new); //won't auto delete if true
	virtual bool SoundInstanceKill(hSoundInstance hSound); //kills a sound no matter what
    
    //3d specific functions
    hSoundInstance SoundInstanceCreateAndLoad3D(hSoundInstance hSoundToUse, const char stName[]);
    virtual bool SoundInstance3DSetPos(hSoundInstance hSound, float f_x, float f_y, float f_z);
    virtual bool SoundInstance3DSetDistance(hSoundInstance hSound, float f_min, float f_max);
    
    
    virtual void ListenerSetPos(float f_x, float f_y, float f_z);
    
    virtual void ListenerSetOrientation(float f_x, float f_y, float f_z);
    virtual void ListenerSetPosVelAndOrientation(float *a_pos, float *a_vel, float *a_dir);
  
	//misc
    virtual hSoundInstance GetMusicInstance();

    virtual void SetMute(bool b_new);
    virtual bool SoundExists(hSoundInstance h_sound);

    virtual void SetMetersPerUnit(float f_distance_factor);
 //for 3d sounds only
    virtual void SetRollOffFactor(float f_amount){LogMsg("SetRollOff Factor: Not Implemented yet"); };
 //for 3d sounds only
   
   virtual eSoundSystem GetSoundSystem() {return SOUND_SYSTEM_FMOD;}
   virtual void SetAudioChannelsToInit(int i_new){m_i_software_audio_channels = i_new;}
 
private:
  int m_i_software_audio_channels; //how many audio channels we should init in FMOD
  FSOUND_SAMPLE * GetSoundSampleByName(const char *p_fname);
  bool m_b_ready;

  FMUSIC_MODULE *m_p_music_mod; //used only for music
  FSOUND_SAMPLE *m_p_music_sample; //used only for music
  vector<FMSound> m_a_sounds; //array of our loaded sounds
  bool m_b_use_hardware; 
};