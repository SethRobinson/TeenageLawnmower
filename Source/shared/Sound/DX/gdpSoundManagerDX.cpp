/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "gdpSoundManagerDX.h"
#include "dxerr8.h"
#include "cguid.h"
#include "..\\..\\all.h"
#include <assert.h>
#include "objbase.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace gdp
{

//=============================================================================
// Constructors/Destructor
//=============================================================================
SoundManagerDX::SoundManagerDX()
{
  m_pLoader = NULL;
  m_pPerformance = NULL;
  this->m_pDSound = NULL;
  m_pDirectMusic = NULL;
  m_pPerformanceMusic = NULL;
}

SoundManagerDX::~SoundManagerDX()
{
   if (IsActive()) Kill();
   CoUninitialize();
 
}


bool SoundManagerDX::IsActive()
{
  if (this->m_pPerformance) return true;
  return false;
}


void SoundManagerDX::ClearCache()
{
    if (m_pLoader)
    {
    
    m_pLoader->ClearCache(GUID_DirectMusicAllTypes);
    m_pLoader->ClearCache(CLSID_DirectMusicSegment);
    m_pLoader->CollectGarbage();
   }
  
}

//=============================================================================
// Init
//=============================================================================
// This must be done before playing any sounds.  Make sure all settings are set
// before calling this. 
//
// Return: Returns true if there was no error.

bool SoundManagerDX::Init()
{
	
  HRESULT hr;	

#if (_WIN32_WINNT < 0x0400 )
  #ifndef _WIN32_DCOM
     #pragma message ("ERROR: - To compile this you need to add _WIN32_WINNT=0x400 to your preprocesser definitions so CoInitializeEx will be defined.")
  #endif
#endif

  if (IsActive()) Kill(); //first free everything if needed
  if (CoInitializeEx(NULL, COINIT_MULTITHREADED) == S_FALSE)
  {
      assert(0); //big time error
  }

    CoCreateInstance(CLSID_DirectMusicLoader, NULL, 
       CLSCTX_INPROC, IID_IDirectMusicLoader8,
       (void**)&m_pLoader);

 //  m_pLoader->EnableCache(CLSID_DirectMusicSegment, false);
 //  m_pLoader->EnableCache(CLSID_DirectMusicSegment, false);
 


   //m_pLoader->
    CoCreateInstance(CLSID_DirectMusicPerformance, NULL,
       CLSCTX_INPROC, IID_IDirectMusicPerformance8,
       (void**)&m_pPerformance );

 	 LogMsg("DirectMusic Based Sound Manager initialized.");

		if (FAILED(hr =m_pPerformance->InitAudio( 
    &m_pDirectMusic,      // IDirectMusic interface not needed.
   &m_pDSound,      // IDirectSound interface not needed.

  NULL,      // Window handle.
    0,  // Default audiopath type.
    64,        // Number of performance channels.
    DMUS_AUDIOF_ALL,   // Features on synthesizer.
    NULL       // Audio parameters; use defaults.
  )))
		{
		  LogError("Error creating main performance object.");
		  m_pPerformance = false;
		  return false;
		}

 CoCreateInstance(CLSID_DirectMusicPerformance, NULL,
       CLSCTX_INPROC, IID_IDirectMusicPerformance8,
       (void**)&m_pPerformanceMusic );
 
	

	//allocate our music performance too, we split them up so changing music
	//won't affect our sound effects

 if (FAILED(hr = m_pPerformanceMusic->InitAudio( 
  
	&m_pDirectMusic,      // IDirectMusic interface not needed.
	&m_pDSound,
 	NULL,      // Window handle.
    0,  // Default audiopath type.
    64,        // Number of performance channels.
    DMUS_AUDIOF_ALL,   // Features on synthesizer.
    NULL       // Audio parameters; use defaults.
  )))
		{
		 LogError("Error creating music performance object.");
		  m_pPerformanceMusic = false;
		  return false;
		}
 
 GUID guid = GUID_NOTIFICATION_SEGMENT;
 m_pPerformance->AddNotificationType(guid);


  
 m_pPerformanceMusic->AddNotificationType(guid);


 //init our music audio path and segment
	 hMainSegmentHandle = this->m_SoundMap.Create();
	 m_SoundMap.Get(hMainSegmentHandle)->Init(this->m_pPerformance, false);	
 	 m_SoundMap.Get(hMainSegmentHandle)->SetStayAlive(true); //will never auto kill itself

	 hMusicHandle = this->m_SoundMap.Create();
	 m_SoundMap.Get(hMusicHandle)->Init(this->m_pPerformanceMusic, false);	
	 m_SoundMap.Get(hMusicHandle)->SetStayAlive(true); //will never auto kill itself
	 m_SoundMap.Get(hMusicHandle)->SetForceMainSegment(true); //new songs overwrite eachother


	 //let's also init our 3d listener
	 if (FAILED(hr = m_SoundMap.Get(hMainSegmentHandle)->m_AudioPath.m_pAudioPath->GetObjectInPath( 
		 0, DMUS_PATH_PRIMARY_BUFFER, 0,
		 GUID_NULL, 0, IID_IDirectSound3DListener8, 
		 (LPVOID*) &m_pDSListener)))
	 {
		 
		LogError("Error creating 3d listener. (%s)", DXGetErrorString8(hr));
		 m_pPerformanceMusic = false;
		 return false;
		 
	 }

	 //success
	return true; 
}

//=============================================================================
// Kill
//=============================================================================
// Turns off all sounds, frees all resources and releases all objects that were used.

void SoundManagerDX::Kill()
{
 
  if (!IsActive())
  {
    LogError("Can't kill sound, not initted.");
	return;
  }

  if (m_pPerformanceMusic)
  m_pPerformanceMusic->Stop(
    NULL, // Stop all segments.
    NULL, // Stop all segment states.
    0,  // Do it immediately.
    0   // Flags.
  );

  m_pPerformance->Stop(
    NULL, // Stop all segments.
    NULL, // Stop all segment states.
    0,  // Do it immediately.
    0   // Flags.
  );

 
  //kill all sounds
  m_SoundMap.Kill();

  if (m_pPerformanceMusic)
   m_pPerformanceMusic->CloseDown();

  m_pPerformance->CloseDown();
  SAFE_RELEASE(m_pDirectMusic);
  SAFE_RELEASE(this->m_pDSound);
  
  
  SAFE_RELEASE(m_pPerformanceMusic);
  SAFE_RELEASE(m_pPerformance);
 
   
  if (m_pLoader)
  {
      m_pLoader->CollectGarbage();
  }
    
  SAFE_RELEASE(m_pLoader);
 
  CoUninitialize();

  LogMsg("DirectMusic based Sound Manager killed.");

}

//=============================================================================
// GetGlobalVolume
//=============================================================================
// Returns how loud the overall mixer is set at.

u32 SoundManagerDX::GetGlobalVolume()
{


 return 0;
}
   
//=============================================================================
// SetGlobalVolume
//=============================================================================
// Controls what the overall mixer volume is.

void SoundManagerDX::SetGlobalVolume(u32 uVolume)
{


 
}


//=============================================================================
// PlaySound
//=============================================================================
// Causes a sound to start playing.  Will be loaded into a resource if it's not
//
// Input: a filename of the sound to play. 

//
// Return: The hSoundInstance it returns will allow you to reference this specific
// sound in the future to turn it off or adjust it in realtime.

hSoundInstance SoundManagerDX::Play(const char stName[])
{
   
    if (!IsActive())
   {
//	 LogError("Cannot play sound %s, sound isn't initted.", stName);
	 return C_SOUND_NONE;
   }
 
    
 //   bool b_main_seg = true; //force this to play on the music seg
   hSoundInstance uInstance;

   uInstance = this->hMainSegmentHandle;

   if (!m_SoundMap.Get(uInstance)->Load(this->m_pLoader, stName))
   {
	 LogError("Error loading sound");
	 return 0;
   }
 
   //and finally, play it.
   m_SoundMap.Get(uInstance)->Play(1);
  return uInstance;						
}

hSoundInstance SoundManagerDX::PlayMixed(const char stName[])
{
    hSoundInstance h_sound = SoundInstanceCreate();
    SoundInstanceLoad(h_sound, stName);
    SoundInstancePlay(h_sound, 1);
    return h_sound;						
}



hSoundInstance SoundManagerDX::PlayMusic(const char stName[], int i_repeats)
{
      
    if (!IsActive())
   {
//	 LogError("Cannot play sound %s, sound isn't initted.", stName);
	 return C_SOUND_NONE;
   }
 
    
 //   bool b_main_seg = true; //force this to play on the music seg
   hSoundInstance uInstance;

   uInstance = this->hMusicHandle;

   if (!m_SoundMap.Get(uInstance)->Load(this->m_pLoader, stName))
   {
	 LogError("Error loading sound");
	 return 0;
   }
 
   //and finally, play it.
   m_SoundMap.Get(uInstance)->Play(i_repeats);
   
  return uInstance;						
}

void SoundManagerDX::StopMusic()
{
 m_SoundMap.Get(hMusicHandle)->Stop();
}

hSoundInstance SoundManagerDX::SoundInstanceCreate()
{
   if (!IsActive())
   {
//	 LogError("Can't create a sound instance, sound has not been initted.");
	 return 0;

   }
   hSoundInstance uInstance = m_SoundMap.Create();
   
   if (!m_SoundMap.Get(uInstance)->Init(this->m_pPerformance, false))
   {
       //error initting something, kill it and tell them they got nothun
       SoundInstanceKill(uInstance);
       return C_SOUND_NONE;
   }
   
   return uInstance;
}




hSoundInstance SoundManagerDX::SoundInstanceCreate3D()
{
   if (!IsActive())
   {
//	 LogError("Can't create a 3d sound instance, sound has not been initted.");
	 return 0;

   }
   hSoundInstance uInstance = m_SoundMap.Create();
   if (!m_SoundMap.Get(uInstance)->Init(this->m_pPerformance, true))
   {
        
       LogError("Failed to init sound, killed it.");
       SoundInstanceKill(uInstance);
       return C_SOUND_NONE;
   }
   return uInstance;
}



//=============================================================================
// PlaySoundLoop
//=============================================================================
// Same as PlaySound but enables looping mode.  The sound not stop by itself.
//
// Input: a filename of the sound to play.  Right now it must be a .wav but this
// might support.mp3 or other filetypes later.
//
// Return: The hSoundInstance it returns will allow you to reference this specific
// sound in the future to turn it off or adjust it in realtime.

hSoundInstance SoundManagerDX::PlayLoop(const char stName[])
{


 return 0;
}

//=============================================================================
// KillSound
//=============================================================================
//Pass it a sound instance and it will instantly stop the sound and take care of
//any resource clean up if required.

void SoundManagerDX::StopSound(hSoundInstance)
{


 
}

//=============================================================================
// KillAllSounds
//=============================================================================
// Kill every sound playing.  Note that music is not touched.
// Returns how many sounds were killed.

u32 SoundManagerDX::StopAllSounds()
{

   CSound *p_temp;
   	int iCounter = 0;
		while (p_temp = this->m_SoundMap.ScanAllSounds())
			   {

                 //kill this sound
                  p_temp->Stop();
                 iCounter++;
			   }

  return iCounter;

 return 0;
}

//=============================================================================
// KillSoundsByResource
//=============================================================================
// Allows you to kill all sounds of a specific type. Returns how many were killed.

u32 SoundManagerDX::StopSoundsByResource(hSoundResource sndResource)
{


  return 0;
}

//=============================================================================
// PreloadSound
//=============================================================================
// This will load a sound from disk into a resource.  Sounds loaded this way
// are not automatically freed when sounds are done with them - this let's you
// make sure all important sounds are always loaded throughout the game such as
// gun noises and screams.
//
// Returns the resource ID the sound happened to be loaded into.

void SoundManagerDX::PreloadSound(const char stName[])
{
  LogMsg("SoundManagerDX Preload sound not implemented.");

 
}

//=============================================================================
// UnloadAllResources
//=============================================================================
// Doing this make sure no sounds are loaded at all.  If a sound is currently using
// a resource when this is called it will be stopped.


void SoundManagerDX::UnloadAllResources()
{


}

//=============================================================================
// UnloadResource
//=============================================================================
// Unload a specific resource. Maybe handy if you know you won't be needed a large
// sound anymore and want to free the memory.  Remember, unless you preloaded a resource
// you never need to use this.

void SoundManagerDX::UnloadResource(hSoundResource sndResource)
{


}

//=============================================================================
// GetResourcesLoaded
//=============================================================================
// call this in a while() loop until it returns false, it will walk through each
// resource loaded.

bool SoundManagerDX::GetResourcesLoaded(const char *out_pstName, hSoundResource *out_SoundResource)
{

  return false;
}

//=============================================================================
// GetResourcesLoaded
//=============================================================================
// Returns how many sound instances are currently active.


u32 SoundManagerDX::GetSoundsCurrentlyPlaying()
{
 CSound *p_temp;
   	int iCounter = 0;
		while (p_temp = this->m_SoundMap.ScanAllSounds())
			   {
				iCounter++;
			   }

  return iCounter;
}

bool SoundManagerDX::SoundInstanceSetEffect(hSoundInstance hSound, u32 uEffectType, bool bActive, int iSetting)
{
 	if (!IsActive())
   {
 	return false;
	}
	
  CSound *sound;
  sound = m_SoundMap.Get(hSound);

  if (!sound)
  {
    LogMsg("SoundInstanceSetEffect: Sound handle %u doesn't exist.", hSound);
	return false;
  }

  //actually make the changes
  sound->m_AudioPath.SetEffect(uEffectType, bActive, iSetting); 
  
 return true;
}

bool SoundManagerDX::SoundInstancePlay(hSoundInstance hSound, int iCount)
{
   if (!IsActive())
   {
 	return false;
	}

  
	CSound *sound;
  sound = m_SoundMap.Get(hSound);

  if (!sound)
  {
     LogMsg("SoundInstancePlay: Sound handle %u doesn't exist.", hSound);
	return false;
  }

  //actually make the changes
  sound->Play(iCount);
  
  return true;
}

bool SoundManagerDX::SoundInstance3DSetPos(hSoundInstance hSound, float f_x, float f_y, float f_z)
{
   if (!IsActive())
   {
 	return false;
	}

	CSound *sound;
  sound = m_SoundMap.Get(hSound);

  if (!sound)
  {
    LogMsg("SoundInstance3DSetPos: Sound handle %u doesn't exist.", hSound);
	return false;
  }

  //actually make the changes
  return (sound->Set3DPosition(f_x, f_y, f_z));
}

hSoundInstance SoundManagerDX::SoundInstanceCreateAndLoad(const char stName[] )
{
    hSoundInstance hSound = SoundInstanceCreate();
    SoundInstanceLoad(hSound, stName);
    return hSound;
}
      

hSoundInstance SoundManagerDX::SoundInstanceCreateAndLoad3D(hSoundInstance hOldSound, char stName[] )
{
    //only create a new sound if an old one wasn't passed in
    
    hSoundInstance hSound = hOldSound;
    if (hSound == C_SOUND_NONE)
    {
       hSound = SoundInstanceCreate3D();
    }
    
    SoundInstanceLoad(hSound, stName);
    return hSound;
}



bool SoundManagerDX::SoundInstanceLoad(hSoundInstance hSound,const char stName[] )
{
  if (!IsActive())
   {
 	return false;
	}

  CSound *sound;
  sound = m_SoundMap.Get(hSound);

  if (!sound)
  {
    LogMsg("SoundInstanceLoad: Sound handle %u doesn't exist.", hSound);
	return false;
  }

  //actually make the changes
  if (!sound->Load(this->m_pLoader, stName)) return false;
  
  return true;
}


void SoundManagerDX::UpdateSounds()
{
  	if (!IsActive())
	{
	 //LogError("Sound not initted yet.");
     return;
	}
	
	CSound *p_temp = NULL;

  HRESULT hr; 
   DMUS_NOTIFICATION_PMSG* pPMsg;
   while (S_OK == m_pPerformance->GetNotificationPMsg(&pPMsg))
    {
      
	    switch( pPMsg->dwNotificationOption )
        {
        case DMUS_NOTIFICATION_SEGEND:
		    if( pPMsg->punkUser )
            {
                IDirectMusicSegmentState8* pSegmentState   = NULL;
                IDirectMusicSegment*       pNotifySegment   = NULL;
                // IDirectMusicSegment8*      pNotifySegment8  = NULL;
                
                // The pPMsg->punkUser contains a IDirectMusicSegmentState8, 
                // which we can query for the segment that the SegmentState refers to.
                if( FAILED( hr = pPMsg->punkUser->QueryInterface( IID_IDirectMusicSegmentState8,
                    (VOID**) &pSegmentState ) ) )
                {
                    DXTRACE_ERR( TEXT("QueryInterface"), hr );
                    continue;
                }
             
                if( FAILED( hr = pSegmentState->GetSegment( &pNotifySegment ) ) )
                {
                    // Sometimes the segend arrives after the segment is gone
                    // This can happen when you load another segment as 
                    // a motif or the segment is ending
                    if( hr == DMUS_E_NOT_FOUND )
                    {
                   //     LogMsg("Segment gone..");
                        SAFE_RELEASE( pSegmentState );
                        continue;
                    }
                    
                        SAFE_RELEASE( pSegmentState );
                    DXTRACE_ERR( TEXT("GetSegment"), hr );
                    continue;
                }
                
                //now deal with it
                while (p_temp = this->m_SoundMap.ScanAllSounds())
                {
                    if (pSegmentState == p_temp->m_pSegmentState8)
                    {
                        if (p_temp->GetStayAlive() == false)
                        {
                        
                         //   LogMsg("Killing sound.");
                            m_SoundMap.KillSound(p_temp->hMyInstance);
                            m_SoundMap.ResetSoundScan();
                            break;

                        } else
                        {
                           // LogMsg("Not erasing sound, it's a stay alive.");
                            m_SoundMap.ResetSoundScan();
                            break;

                        }
                    }
                }
                
               //in any case, free the dealies we had
                          SAFE_RELEASE( pSegmentState );
                        SAFE_RELEASE( pNotifySegment);
            }
	    }
	   
	   m_pPerformance->FreePMsg((DMUS_PMSG*)pPMsg); 
    }

	//commit 3d changes
	m_pDSListener->CommitDeferredSettings();

#ifdef _DSHOW
    //now deal with all DSound objects

    while (p_temp = this->m_SoundMap.ScanAllSounds())
    {
        //this will return TRUE if we should destroy the sound
        if (p_temp->UpdateDirectShow())
        {
            m_SoundMap.KillSound(p_temp->hMyInstance);
        }
    }
#endif    
  
}

bool SoundManagerDX::SoundInstanceSetPitch(hSoundInstance hSound, int iPitch)
{
  if (!IsActive())
   {
 	return false;
	}

  CSound *sound;
  sound = m_SoundMap.Get(hSound);

  if (!sound)
  {
    LogMsg("SoundInstanceSetPitch: Sound handle %u doesn't exist.", hSound);
	return false;
  }

  //actually make the changes
  return sound->m_AudioPath.SetPitch(iPitch);
  
}
	
bool SoundManagerDX::SoundInstanceSetStayAlive(hSoundInstance hSound, bool b_new)
{
  if (!IsActive())
   {
 	return false;
	}

  CSound *sound;
  sound = m_SoundMap.Get(hSound);

  if (!sound)
  {
    LogMsg("SoundInstanceSetStayAlive: Sound handle %u doesn't exist.", hSound);
	return false;
  }

  //actually make the changes
  sound->SetStayAlive(b_new);
  return true;
}

bool SoundManagerDX::SoundInstanceSetForceMainSegment(hSoundInstance hSound, bool b_new)
{
 
  CSound *sound;
  sound = m_SoundMap.Get(hSound);

  if (!sound)
  {
    LogMsg("SoundInstanceSetForceMainSegment: Sound handle %u doesn't exist.", hSound);
	return false;
  }

  //actually make the changes
  sound->SetForceMainSegment(b_new);
  return true;
}


bool SoundManagerDX::SoundInstanceStop(hSoundInstance hSound)
{
 if (!IsActive())
   {
 	return false;
	}
 
  CSound *sound;
  sound = m_SoundMap.Get(hSound);

  if (!sound)
  {
    LogMsg("SoundInstanceStop: Sound handle %u doesn't exist.", hSound);
	return false;
  }

  //actually make the changes
  sound->Stop();
  return true;
}


bool SoundManagerDX::SoundInstanceKill(hSoundInstance hSound)
{
if (!IsActive())
   {
 	return false;
	}
  
  CSound *sound;
  sound = m_SoundMap.Get(hSound);

  if (!sound)
  {
    LogMsg("SoundInstanceKill: Sound handle %u doesn't exist.", hSound);
	return false;
  }

  //actually make the changes
  m_SoundMap.KillSound(hSound);
  return true;
}


void SoundManagerDX::ListenerSetPos(float f_x, float f_y, float f_z)
{
 if (m_pDSListener)	
   {
	   m_pDSListener->SetPosition(f_x, f_y, f_z, DS3D_DEFERRED);
   }
}

void SoundManagerDX::ListenerSetOrientation(float f_x, float f_y, float f_z)
{

  if (m_pDSListener)	
   {
	   m_pDSListener->SetOrientation(f_x, f_y, f_z, 0,1,0, DS3D_DEFERRED);
   }

}


void SoundManagerDX::ListenerSetPosVelAndOrientation(float *a_pos, float *a_vel, float *a_dir)
{
if (m_pDSListener)	
   {
  	   m_pDSListener->SetPosition(a_pos[0], a_pos[1], a_pos[2], DS3D_DEFERRED);
  	 
/*       
       char st_crap[256];
       sprintf(st_crap, "dir X: %.2f, dir Y: %.2f, dir Z: %.2f", a_dir[0], a_dir[1], a_dir[2]);
       LogMsg(st_crap);
  */  
       m_pDSListener->SetOrientation(a_dir[0], a_dir[1], a_dir[2], 0,1,0, DS3D_DEFERRED);
  
  if (a_vel)
       m_pDSListener->SetVelocity(a_vel[0], a_vel[1], a_vel[2], DS3D_DEFERRED);
  
       
       char st_crap[256];
       sprintf(st_crap, "vel X: %.2f, vel Y: %.2f, vel Z: %.2f", a_vel[0], a_vel[1], a_vel[2]);
       LogMsg(st_crap);
  

}
}

bool SoundManagerDX::SoundExists(hSoundInstance h_sound)
{
if (!IsActive())
   {
 	return false;
	}
	//note:  change this to be faster later, this is a bad way to check if an stl
	//map exists, it actually creates it then deletes it again in the Get() function
	//if it doesn't exist
	
	if (m_SoundMap.Get(h_sound)) return true;
	return false;
}


//The GetDistanceFactor method retrieves the distance factor, which is the number
// of meters in a vector unit. Default is 1.


void SoundManagerDX::SetMetersPerUnit(float f_distance_factor)
{
	  if (m_pDSListener)
	  {
		  m_pDSListener->SetDistanceFactor(f_distance_factor, DS3D_DEFERRED);
	  }	  else
	  {
		  LogError("SetMetersPerUnit failed.");
	  }
}

//generally between 0 and 10, default is 1
void SoundManagerDX::SetRollOffFactor(float f_amount)
{
	  if (m_pDSListener)
	  {
		  m_pDSListener->SetRolloffFactor(f_amount, DS3D_DEFERRED);
	  }	  else
	  {
		  LogError("SetRolloffFactor failed.");
	  }
}

/*

  As a listener approaches a sound source, the sound gets louder; the volume doubles when the
  distance is halved. Past a certain point, however, it is not reasonable for the volume to
  continue to increase. This is the minimum distance for the sound source. 
  
 The minimum distance is especially useful when an application must compensate for the difference
 in absolute volume levels of different sounds. Although a jet engine is much louder than a bee,
 for practical reasons these sounds must be recorded at similar absolute volumes. An application
 might use a minimum distance of 100 meters for the jet engine and 2 centimeters for the bee.
 With these settings, the jet engine would be at half volume when the listener was 200 meters
 away, but the bee would be at half volume when the listener was 4 centimeters away. 

 The default minimum distance for a sound buffer, DS3D_DEFAULTMINDISTANCE, is defined as 1 unit,
 or 1 meter at the default distance factor. Unless you change this value, the sound is at full
 volume when it is 1 meter away from the listener, half as loud at 2 meters, a quarter as loud 
 at 4 meters, and so on. For most sounds you will probably want to set a larger minimum distance
 so that the sound does not fade so rapidly as it moves away.

 The maximum distance for a sound source is the distance beyond which the sound does not get
 any quieter. The default maximum distance for a DirectSound 3-D buffer (DS3D_DEFAULTMAXDISTANCE)
 is 1 billion, meaning that in most cases the attenuation will continue to be calculated long
 after the sound has moved out of hearing range. To avoid unnecessary processing, applications
 should set a reasonable maximum distance and include the DSBCAPS_MUTE3DATMAXDISTANCE flag when
 creating the buffer. This flag is automatically set on standard 3-D buffers created as part of
 an audiopath; see Standard Audiopaths.


*/

  bool SoundManagerDX::SoundInstance3DSetDistance(hSoundInstance hSound, float f_min, float f_max)
{
 if (!IsActive())
	{
 	return false;
	}
 
	CSound *sound;
  sound = m_SoundMap.Get(hSound);

  if (!sound)
  {
    LogMsg("SoundInstance3DSetDistance: Sound handle %u doesn't exist.", hSound);
	return false;
  }

  //actually make the changes
  return (sound->Set3DDistance(f_min, f_max));
}


}; // END namespace gdp
