#include "FMSoundManager.h"
#include "..\\..\\all.h"
#include <fmod_errors.h> 

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FMSoundManager::FMSoundManager()
{
    m_b_ready = false;
    m_p_music_mod = NULL;
    m_p_music_sample = NULL;
    m_b_use_hardware = false;  //software works better and more reliably than hardware in all my tests
    SetAudioChannelsToInit(32);
}

void FMSoundManager::SetMute(bool b_new)
{
  FSOUND_SetMute(FSOUND_ALL, b_new);
}

bool FMSoundManager::Init()
{
    float f_dll_version = FSOUND_GetVersion();
    if ( f_dll_version < 3.72000f /*FMOD_VERSION*/) //The latest .dll still says 3.62?!
    {
        LogError("You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
        return false;
    }
    
    /*
    INITIALIZE
    */
    //FSOUND_SetBufferSize(200);  //causes major lag
       
      FSOUND_SetMixer(FSOUND_MIXER_QUALITY_FPU);

       if (!FSOUND_Init(44100, m_i_software_audio_channels, 0))
    {
        LogError("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        return false;
    }

    LogMsg("FMOD SoundManager initted.  FMOD.dll is v%.02f.", FSOUND_GetVersion());
    return m_b_ready = true;
}


FMSoundManager::~FMSoundManager()
{
   Kill(); //deallocate everything we had going
}


hSoundInstance FMSoundManager::Play(const char p_fname[])
{
  return PlayMixed(p_fname);
}


//case sensitive!  Always use lower case.
FSOUND_SAMPLE * FMSoundManager::GetSoundSampleByName(const char *p_fname)
{

    for (unsigned int i=0; i < m_a_sounds.size(); i++)
    {
         if (strcmp(m_a_sounds[i].st_fname.c_str(), p_fname) == 0)
        {
            //found it
            return m_a_sounds[i].p_sample;
        }
    }
   //couldn't find it.
    return NULL;
}


FMSound * FMSoundManager::FMPreloadSound(const char *p_fname, bool b_3d_sound)
{
   m_a_sounds.push_back(FMSound());

   int i_index = m_a_sounds.size()-1; //only works with push back, later we'll add insert where
   //LogMsg("Sound array is now %d.", i_index);
   //a vector has a null sound or something like that
   UINT ui_flags = FSOUND_LOOP_OFF;   //add this to all sounds
   
   if (b_3d_sound)
   {
       if (m_b_use_hardware)
       {
           //there is no point to using a 3d sound if it's stereo, so let's force mono
           ui_flags |= FSOUND_HW3D | FSOUND_FORCEMONO; //go ahead and load it into the 3d card
       }
   }   else
   {
       ui_flags |= FSOUND_2D; //tell it to not do 3d functions on this sample
   }

   if (!exist(p_fname))
   {
       //couldn't find the sample?  try as ogg
       char st_temp[MAX_PATH];
       strcpy(st_temp, p_fname);
       change_file_extension(st_temp, "ogg");
       m_a_sounds[i_index].p_sample = FSOUND_Sample_Load(FSOUND_FREE, st_temp, ui_flags,0,0);
       
   }  else
   {
       m_a_sounds[i_index].p_sample = FSOUND_Sample_Load(FSOUND_FREE, p_fname, ui_flags,0,0);
  }


    if (!m_a_sounds[i_index].p_sample)
    {
      //  LogError("SoundManager: Couldn't find %s.", p_fname);
    }
   
   m_a_sounds[i_index].st_fname = p_fname; //remember this for later
    
   return &m_a_sounds[i_index]; //do NOT save this and use it later, the vector could be resized which may
   //invalidate this pointer!
}


hSoundInstance FMSoundManager::PlayMixed(const char p_fname[])
{

 FSOUND_SAMPLE  *p_sample = GetSoundSampleByName(p_fname);

 if (!p_sample)
 {
   //load it, it has not been preloaded yet.
   p_sample = FMPreloadSound(p_fname, false)->p_sample;

   if (!p_sample)
   {
       //LogError("Unable to load sound %s.", p_fname);
       return -1;
   }
 }
     
 int i_channel = FSOUND_PlaySound(FSOUND_FREE, p_sample);
 FSOUND_SetLoopMode(i_channel, FSOUND_LOOP_OFF);
 
 return i_channel;
}


hSoundInstance FMSoundManager::PlayLoop(const char p_fname[])
{

 int i_channel = PlayMixed(p_fname);
 FSOUND_SetLoopMode(i_channel, FSOUND_LOOP_NORMAL);
 return i_channel;
}

void FMSoundManager::KillChannel(int i_channel)
{
   FSOUND_StopSound(i_channel);
}


//FMOD's version can only play non-looped or loop, it won't count and stop after a certain # of loops
//also it returns true or false, not a music channel!
hSoundInstance FMSoundManager::PlayMusic( const char p_fname[], int i_repeat)
{
    
    StopMusic();
    
    //if it's a digital music type format, we can play it with our normal sound player.  If it's a MOD, MID and such,
    //we need to use the special FMOD music functions.
    if (
           file_extension_is(p_fname, ".wav")
        || file_extension_is(p_fname, ".ogg")
        || file_extension_is(p_fname, ".mp3")
        || file_extension_is(p_fname, ".mp2")
        || file_extension_is(p_fname, ".raw")
        
        )
    {
        //use the sample method, rather than music
        
        m_p_music_sample = FSOUND_Sample_Load(FSOUND_FREE, p_fname, FSOUND_2D,0,0);

        if (!m_p_music_sample)
        {
             //couldn't find the song?  try as ogg
            char st_temp[MAX_PATH];
            strcpy(st_temp, p_fname);
            change_file_extension(st_temp, "ogg");
            m_p_music_sample = FSOUND_Sample_Load(FSOUND_FREE, st_temp, FSOUND_2D,0,0);
            
            if (!m_p_music_sample)
            {
               LogError("Unable to find sound music %s.", p_fname);
               return 0;
            }
        }
        
        int i_channel = FSOUND_PlaySound(FSOUND_FREE, m_p_music_sample);
        
        if (i_repeat == 1)
        {
            FSOUND_SetLoopMode(i_channel, FSOUND_LOOP_OFF);
        } else
        {
            FSOUND_SetLoopMode(i_channel, FSOUND_LOOP_NORMAL);
        }
        
      return 1;    
    }

    //if we got here, let's load it as music
    m_p_music_mod = FMUSIC_LoadSong(p_fname);
    
    if (!m_p_music_mod)
    {
        LogError("Error loading the music file %s.", p_fname);
        return 0;
    }
    FMUSIC_PlaySong(m_p_music_mod);
    
    if (i_repeat == 1)
    {
        FMUSIC_SetLooping(m_p_music_mod, FALSE);
    } else
    {
        FMUSIC_SetLooping(m_p_music_mod, TRUE); //play forever
    }
    
    
    return 1;
}

hSoundInstance FMSoundManager::SoundInstanceCreate()
{
    assert(0); //this should never get called
    return 0;
}

hSoundInstance FMSoundManager::GetMusicInstance()
{

    assert(0);
    LogMsg("GetMusicInstance:  Not implemented yet");
    return 0;
}

hSoundInstance FMSoundManager::SoundInstanceCreateAndLoad(const char stName[])
{
  FSOUND_SAMPLE  *p_sample = GetSoundSampleByName(stName);

 if (!p_sample)
 {
   //load it, it has not been preloaded yet.
   p_sample = FMPreloadSound(stName, false)->p_sample;

   if (!p_sample)
   {
       LogError("Unable to load sound %s.", stName);
       return -1;
   }
 }
     
 int i_channel = FSOUND_PlaySoundEx(FSOUND_FREE, p_sample, NULL,true);
 FSOUND_SetLoopMode(i_channel, FSOUND_LOOP_OFF);
 //LogMsg("Initted channel %d", i_channel);
 return i_channel;

}

bool FMSoundManager::SoundInstancePlay(hSoundInstance hSound, int iCount)
{
    //just in case it was paused, which is the default after initting the sample
       //unpause, which will start the playing, mofo!
    //why does unpausing turn off the reserved flag?  huh.  Well I'll just reserve it's ass right
    //back.
    //
    
  //  LogMsg("playing %d", hSound);
   bool m_reserved = (FSOUND_GetReserved(hSound) == TRUE);
 
    //pay attention to if we're supposed to loop or not
      
    //rewind it, in case it was already playing
    //FSOUND_SetCurrentPosition(hSound, 0);
     if (iCount == 1)
    {
    if (!FSOUND_SetLoopMode(hSound, FSOUND_LOOP_OFF))
    {
        LogError("SoundInstancePlay: %s", FMOD_ErrorString(FSOUND_GetError()));
    }
     ;
    }   else
    {
   if (!FSOUND_SetLoopMode(hSound, FSOUND_LOOP_NORMAL))
    {
        LogError("SoundInstancePlay: %s", FMOD_ErrorString(FSOUND_GetError()));
    }
   }
 
    FSOUND_SetReserved(hSound, m_reserved);
 
    bool b_check =   (FSOUND_GetReserved(hSound) == TRUE);
 
    //must set loop mode AFTER starting to play, limitation of FMOD
    //Loop mode will ONLY work with software samples, not hardware.  Limitation of FMOD
    

   if (!FSOUND_SetPaused(hSound, false))
    {
        LogError("SoundInstancePlay: %s", FMOD_ErrorString(FSOUND_GetError()));
    }

   // int i_channel = FSOUND_PlaySound(FSOUND_FREE, p_sample);


   return 1;
}


bool FMSoundManager::SoundInstanceSetPitch(hSoundInstance hSound, int iPitch)
{
  return (FSOUND_SetFrequency(hSound, iPitch) == TRUE);
}

bool FMSoundManager::SoundInstanceStop(hSoundInstance hSound)
{
     if (!FSOUND_SetPaused(hSound, true))
    {
        LogError("SoundInstanceStop Unpause: %s", FMOD_ErrorString(FSOUND_GetError()));
    }

      return true;
    //return (FSOUND_StopSound(hSound) == TRUE);
}

bool FMSoundManager::SoundInstanceKill(hSoundInstance hSound)
{
    //kill a sound no matter what
    FSOUND_SetReserved(hSound, false);
    return (FSOUND_StopSound(hSound) == TRUE);
}

bool FMSoundManager::SoundInstanceSetStayAlive(hSoundInstance hSound, bool b_new)
{
  return (FSOUND_SetReserved(hSound, b_new) == TRUE);
}


void FMSoundManager::StopSound(hSoundInstance hSound)
{
  FSOUND_StopSound(hSound);
}


void FMSoundManager::StopMusic()
{
    if (m_p_music_mod)
    {
        FMUSIC_FreeSong(m_p_music_mod);
        m_p_music_mod = NULL;
    }
    
    if (m_p_music_sample)
    {
        FSOUND_Sample_Free(m_p_music_sample);
        m_p_music_sample = NULL;
    }
}


//this actually DOESN'T return how many sounds were stopped, ignore the return value
u32 FMSoundManager::StopAllSounds()
{
    return (FSOUND_StopSound(FSOUND_ALL) == TRUE);
}

void FMSoundManager::Kill()
{
  
    
  StopMusic();
  FSOUND_Close();
  LogMsg("FMOD SoundManager killed.");
}

bool FMSoundManager::GetResourcesLoaded(char *out_pstName, hSoundResource *out_SoundResource)
{
    static unsigned int s_i_cur = 0;
    
    
    if (s_i_cur < m_a_sounds.size())
    {
        //get the sound info
        strcpy(out_pstName, m_a_sounds[s_i_cur].st_fname.c_str());
        *out_SoundResource = 0; //not used yet
        //move it to the next resource for the next time it's called
        s_i_cur++;
        return true;
    }   else
    {
        //all done
        s_i_cur = 0; //reset it for next time
        return false; //signal no more is coming
    }
   return false; //will never get here
}


//3D stuff


bool FMSoundManager::SoundInstance3DSetPos(hSoundInstance hSound, float f_x, float f_y, float f_z)
{
    FLOAT a_pos[3];
    a_pos[0] = f_x;
    a_pos[1] = f_y;
    a_pos[2] = f_z;

    return (FSOUND_3D_SetAttributes(hSound, &a_pos[0],NULL) == 1);
}

//FMOD doesn't really have a way to set per object 3d sensitivity<-- scratch that, 3.73 added this ability,
//this is now done per sound channel


/* from FMODS docs:

  maxdistance is effectively obsolete unless you need the sound to stop fading out at a certain point. Do not adjust this from the default if you dont need to.
  Some people have the confusion that maxdistance is the point the sound will fade out to, this is not the case.

  That's why I hardcoded max dist.
*/

bool FMSoundManager::SoundInstance3DSetDistance(hSoundInstance hSound, float f_min, float f_max)
{
  //old way (per sample, bad)
    
    /*
    FSOUND_SAMPLE *p_sample = FSOUND_GetCurrentSample(hSound);
    if (!p_sample)
    {
        LogError("SoundInstanceCreateAndLoad3D: %s", FMOD_ErrorString(FSOUND_GetError()));
        return false;
    }

    
    if (!FSOUND_Sample_SetMinMaxDistance(p_sample, f_min, 1000000000))
      LogError("SoundInstanceCreateAndLoad3D: %s", FMOD_ErrorString(FSOUND_GetError()));
  */
    
     if (!FSOUND_3D_SetMinMaxDistance(hSound, f_min, 1000000000.0f))
    {
      //  LogError("SoundInstance3DSetDistance: %s (channel %d)", FMOD_ErrorString(FSOUND_GetError()),hSound);
    }



 return true;  
};


//this are very slow because FMOD doesn't really work in a way compatible with the interface class
//(which was written for DirectSound's 3D sound methods)
//use, ListenerSetPosVelAndOrientation instead

void FMSoundManager::ListenerSetPos(float f_x, float f_y, float f_z)
{
    
    FLOAT a_pos[3];
    a_pos[0] = f_x;
    a_pos[1] = f_y;
    a_pos[2] = f_z;
   
    FLOAT fx,fy,fz,tx,ty,tz;
    FSOUND_3D_Listener_GetAttributes(NULL, NULL, &fx, &fy, &fz, &tx, &ty, &tz);
    FSOUND_3D_Listener_SetAttributes(&a_pos[0], NULL, fx,fy,fz,tx,ty,tz);
    //LogMsg("LIstener pos: x: %.2f, y: %.2f z: %.2f", f_x, f_y, f_z);
}


//Send it a pointers to vectors for the position and velocity.  Velocity is in meters per second, pass NULL
//to not use it (no doppler effects)
//a_dir is a normalized direction vector, it must be sent.

void FMSoundManager::ListenerSetPosVelAndOrientation(float *a_pos, float *a_vel, float *a_dir)
{
    FSOUND_3D_Listener_SetAttributes(a_pos, a_vel, a_dir[0], a_dir[1],a_dir[2],0,1,0);
    //LogMsg("LIstener pos: x: %.2f, y: %.2f z: %.2f", f_x, f_y, f_z);
}


//the up vector is hardcoded at the moment...
void FMSoundManager::ListenerSetOrientation(float f_x, float f_y, float f_z)
{
     //hardcoded up vector, I may want to make a version that let's us set this someday.
    FSOUND_3D_Listener_SetAttributes(NULL, NULL, f_x,f_y,-f_z,0,1,0);
    //LogMsg("LIstener dir is X: %.2f, Z: %.2f", f_x , -f_z);

}

//note, 3D functions will ONLY work with MONO sounds, but I force mono on the loading (for 3d snds) so even stereo
//sounds should actually work.

hSoundInstance FMSoundManager::SoundInstanceCreateAndLoad3D(hSoundInstance hSoundToUse, const char stName[])
{
    
    if (!IsActive())
    {
        LogMsg("Can't play %s, sound not initted.", stName);
        return -1;
    }

      FSOUND_SAMPLE  *p_sample = GetSoundSampleByName(stName);
      
      if (!p_sample)
      {
          //load it, it has not been preloaded yet.
          p_sample = FMPreloadSound(stName, true)->p_sample;
          
          if (!p_sample)
          {
              LogError("Unable to load sound %s.", stName);
              return -1;
          }
      }                            
 if (hSoundToUse != C_SOUND_NONE)    
 {
     float fmin, fmax;
     FSOUND_3D_GetMinMaxDistance(hSoundToUse, &fmin, &fmax);
    
     //for some reason when doing a new PlaySound on an existing channel FMOD loses the min/max settings, so we
     //manually put them back here
     int i_channel = FSOUND_PlaySoundEx(hSoundToUse, p_sample, NULL,true);
     
     FSOUND_3D_SetMinMaxDistance(hSoundToUse, fmin, fmax);
     return i_channel;
 }

 int i_channel = FSOUND_PlaySoundEx(FSOUND_FREE, p_sample, NULL,true);
    
 
 if (!FSOUND_SetLoopMode(i_channel, FSOUND_LOOP_OFF))
    {
        LogError("SoundInstanceCreateAndLoad3D: %s", FMOD_ErrorString(FSOUND_GetError()));
    }
 
 //LogMsg("Initted channel %d", i_channel);
 return i_channel;

}


void FMSoundManager::SetMetersPerUnit(float f_distance_factor)
 {
     FSOUND_3D_SetDistanceFactor(f_distance_factor);

 }

bool FMSoundManager::SoundExists(hSoundInstance h_sound)
{
    return (FSOUND_IsPlaying(h_sound) == TRUE);
}     




