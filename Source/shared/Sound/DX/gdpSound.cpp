/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "gdpSound.h"
#include "dxerr8.h"
#include "..\\..\\all.h"


namespace gdp
{


//=============================================================================
// Constructors/Destructor
//=============================================================================
CSound::CSound()
{
  memset(this, 0, sizeof(CSound));
 }

CSound::~CSound()
{
    Stop();
    
    
    Kill();

}

bool CSound::Init(IDirectMusicPerformance8* m_pPerformanceNew, bool bIs3d)
{
   this->m_pPerformance = m_pPerformanceNew;
   m_b3d = bIs3d;
  
  //if it's 3d, let's do a special create method
  if (m_b3d) return (m_AudioPath.Create3D(m_pPerformance));
  
  return (m_AudioPath.Create(m_pPerformance));
}

void CSound::Kill()
{
    HRESULT hr;

    //unload the sound if we had one
   SAFE_RELEASE(m_pSegmentState8);
   
        if( (m_p_loader) && (m_pSegment) )
        {
        
            hr = m_p_loader->ReleaseObjectByUnknown( m_pSegment );
        // if (FAILED (hr) )
      	//LogError("Couldn't release by unknown. (%s)", DXGetErrorString8(hr));
          }

    if (m_pSegment)
    {
 // hr = m_pSegment->Unload(m_pPerformance ); //I don't know if I really need to do this or not
     
     hr = m_pSegment->Unload(m_AudioPath.m_pAudioPath); //I don't know if I really need to do this or not
     // if ( FAILED( hr ) )
      //	LogError("Couldn't unload sound from Audiopath. (%s)", DXGetErrorString8(hr));
	
      
    }
    
  SAFE_RELEASE(m_pSegment);
	m_AudioPath.Kill(); 
    SAFE_DELETE(m_p_dshow);
    
}

//update the object, return TRUE if we want to destroy it now.

bool CSound::UpdateDirectShow()
{
  if (!m_p_dshow) return false; //it isn't a DShow object

  //check for messages
  if (m_p_dshow->HandleMessages())
  {
      //done playing it. 
      if (!m_bStayAlive) return true; //kill it

  }

 return false;

}

//the reason we pass in  the loader is the loader object needs to be global as it
//handles caching and the like.

bool CSound::Load(IDirectMusicLoader8*  m_pLoader, const char stName[MAX_PATH])
{

       	HRESULT hr;
    //if a sound was already loaded in this segment we need to release it
     //unload the sound if we had one
   SAFE_RELEASE(m_pSegmentState8);
   
        if( m_p_loader && m_pSegment )
        {
        
            hr = m_p_loader->ReleaseObjectByUnknown( m_pSegment );
       //  if (S_FALSE == hr  )
    //	LogError("Couldn't release by unknown. (%s)", DXGetErrorString8(hr));
           //    log_msg("LOAD: released object by unknown");
 
        }

    if (m_pSegment)
    {
    
   //   hr = m_pSegment->Unload(m_pPerformance ); //I don't know if I really need to do this or not
      hr = m_pSegment->Unload(m_AudioPath.m_pAudioPath); //I don't know if I really need to do this or not
    //  if ( FAILED( hr ) )
//   LogError("Couldn't unload sound from Audiopath. (%s)", DXGetErrorString8(hr));
  //      // log_msg("LOAD: unloaded segment");
 
      	
    }
    
  SAFE_RELEASE(m_pSegment);
//  m_AudioPath.Kill(); 
 //        Init(m_pPerformance,m_b3d);

  //NOTE:  DirectShow seems to be buggy, slow and causes memory leaks and random
       //framerate jerks.  Maybe because I shouldn't be using it this way, as it creates
       //more than one primary sound object... disabled for now.


    //if it's an mp3 let's let's directshow handle it
    if (file_extension_is(stName, ".mp3"))
    {
        SAFE_DELETE(m_p_dshow);
        m_p_dshow = new DirectShow();
        m_p_dshow->Init();
        return m_p_dshow->Load(stName);
        
    }
  

    //kill any old dshow objects if they existed
    SAFE_DELETE(m_p_dshow);

	if (!m_pPerformance)
   {
	 LogError("Can't load sound, sound instance not initted.");
	 return false;
   }

   WCHAR wstrName[MAX_PATH];
  MultiByteToWideChar( CP_ACP, 0, stName, -1, 
         wstrName, MAX_PATH );
  m_pLoader->CollectGarbage();

  if (FAILED(hr =m_pLoader->LoadObjectFromFile(
    CLSID_DirectMusicSegment, // Class identifier.
    IID_IDirectMusicSegment8, // ID of desired interface.
    wstrName,     // Filename.
    (LPVOID*) &m_pSegment   // Pointer that receives interface.
  )))
  {
    LogError("Cannot load %s to play. (%s)", stName, DXGetErrorString8(hr));
     return false;
  }
  
   m_p_loader = m_pLoader;
   // SMsg("Creating segment.");
   //it's now loaded, let's play it.
    m_pSegment->Download(m_AudioPath.m_pAudioPath );
   //if it's a song of some kind, we want to give it control of the tempo and not allow
   //playing over eachother

   m_bMainSegment = false;

   if (strstr(stName, ".mid") != NULL) m_bMainSegment = true;
   if (strstr(stName, ".rmi") != NULL) m_bMainSegment = true;
   if (strstr(stName, ".sgt") != NULL) m_bMainSegment = true;
   
   if (m_bMainSegmentOverride) m_bMainSegment = m_bMainSegmentOverride;
   
   return true; //loaded ok
}


bool CSound::Play(u32 iCount)
{
    //	Stop();

      //I don't want to call the real stop because I don't want to stop the
    //DShow sound.  It's really slow to stop and start it.

       if (m_pPerformance)
       {
       
           if (m_bMainSegmentOverride)
           {
           m_pPerformance->StopEx(this->m_AudioPath.m_pAudioPath, 0, 0);


           }   else
           {
           
           m_pPerformance->StopEx(this->m_AudioPath.m_pAudioPath, 0, 0);
           }
       }
    SAFE_RELEASE(m_pSegmentState8);
	
    if (m_p_dshow)
    {
        //let directshow handle it
       // log_msg("Playing DS handle %d. ", this->hMyInstance);
        return m_p_dshow->Play(iCount);
         
    }
        SAFE_DELETE(m_p_dshow);

    //default settings, sounds can overlap
	DWORD dwFlags = DMUS_SEGF_DEFAULT | DMUS_SEGF_SECONDARY;
	if (m_bMainSegment)
    {
        dwFlags = DMUS_SEGF_DEFAULT;		


    }
	
	if (!m_pSegment)
	{
		LogError("Can't play sound, m_pSegment isn't initted.");
		return false;
	}

	m_pSegment->SetRepeats(iCount-1);
	
	IDirectMusicSegmentState *pSegmentState;
	
	HRESULT hr;
	
	if (m_b3d)
	{
		//special code for 3d settings
		if( FAILED( hr = m_pPerformance->PlaySegmentEx(
			m_pSegment,  // Segment to play.
			NULL,        // Used for songs; not implemented.
			NULL,        // For transitions. 
			dwFlags,           // Flags.
			0,           // Start time; 0 is immediate.
			&pSegmentState,        // Pointer that receives segment state.
			NULL,        // Object to stop.
			this->m_AudioPath.GetAudioPath() // Audiopath.
					)))
		{
			
			//big time error, why would this fail though?
			LogError("PlaySegmentEx in gdpSound->Play failed to play 3d sound. (%s)", DXGetErrorString8(hr));
			return false;
			
		}
		
    }   else
	{
	
        /*
        if (m_bMainSegment)
        {

         	//standard sound
		if( FAILED( hr = m_pPerformance->PlaySegmentEx(
			m_pSegment,  // Segment to play.
			NULL,    // Used for songs; not implemented.
			NULL,    // For transitions. 
			dwFlags,     // Flags.
			0,     // Start time; 0 is immediate.
			&pSegmentState,    // Pointer that receives segment state.
			NULL,    // Object to stop.
			NULL  // Audiopath, if not default.
			)))
		{
			
			//big time error, why would this fail though?
			LogError("PlaySegmentEx in gdpSound->Play failed big time.(%s)", DXGetErrorString8(hr));
			//extact error will get printed to the debug output window
			//   DXTRACE_ERR( TEXT("PlaySegmentEx in gdpSound->Play"), hr );
			return false;
			
		}


        }   else
          */
          {

		//standard sound
		if( FAILED( hr = m_pPerformance->PlaySegmentEx(
			m_pSegment,  // Segment to play.
			NULL,    // Used for songs; not implemented.
			NULL,    // For transitions. 
			dwFlags,     // Flags.
			0,     // Start time; 0 is immediate.
			&pSegmentState,    // Pointer that receives segment state.
			NULL,    // Object to stop.
			this->m_AudioPath.GetAudioPath()  // Audiopath, if not default.
			)))
		{
			
			//big time error, why would this fail though?
			LogError("PlaySegmentEx in gdpSound->Play failed big time.(%s)", DXGetErrorString8(hr));
			//extact error will get printed to the debug output window
			//   DXTRACE_ERR( TEXT("PlaySegmentEx in gdpSound->Play"), hr );
			return false;
			
		}
        }
	
	}
  	//use the magic of com to convert to the newer interface
	
		if( FAILED( pSegmentState->QueryInterface(
			IID_IDirectMusicSegmentState8, (void **)&m_pSegmentState8 ) ) )
		{ 
			LogError("Couldn't get a new interface for SegmentState8");
			return false;
		}

    //release old one we're done with
     	SAFE_RELEASE(pSegmentState);


  // GUID guid = GUID_NOTIFICATION_SEGMENT;
  // m_pSegment->AddNotificationType(guid);

   
 
  //success
   return true;

} 

bool CSound::Set3DPosition(float f_x, float f_y, float f_z)
{
	if (m_AudioPath.m_lpDS3DB8)
	{
		
		m_AudioPath.m_lpDS3DB8->SetPosition(f_x, f_y, f_z, DS3D_DEFERRED);
//		m_AudioPath.m_lpDS3DB8->SetConeOrientation()
		return true;
	}
	
	//apparently something went wrong
	return false;
}

bool CSound::Set3DDistance(float f_min, float f_max)
{
	if(m_AudioPath.m_lpDS3DB8)
	{
		
		m_AudioPath.m_lpDS3DB8->SetMinDistance(f_min, DS3D_DEFERRED);
	
		if (f_max == 0) f_max = DS3D_DEFAULTMAXDISTANCE;
		
		m_AudioPath.m_lpDS3DB8->SetMaxDistance(f_max, DS3D_DEFERRED);
	
		//let's also use this info to set the rolloff.  It appears that the  DSBCAPS_MUTE3DATMAXDISTANCE
		//flag is being ignores under 2000 with some graphics cards.  Not sure why, but at least
		//by setting the rolloff you can't hear it rather than being a constant loud noise
		//everywhere on the level which could happen if your max distance was very short.
		//m_AudioPath.m_lpDS3DB8->SetR
		
		return true;
	}	

  return false; 
}


bool CSound::Stop()
{
 
  /*  
    if (!m_pPerformance)
   {
	 LogError("Can't load sound, sound instance not initted.");
	 return false;
   }
    */
  //old one that killed all of the same kind of sounds:   m_pPerformance->Stop(m_pSegment, m_pSegmentState8, 0, 0);


 // m_pPerformance->Stop(m_pSegment, , 0, 0);
  
  
  //this one kind of works, but kilsl all of the same sound again?
 // m_pPerformance->StopEx(m_pSegmentState8, 0, 0);
    if (m_p_dshow) m_p_dshow->Stop();
    
    
    if (m_pPerformance)
  m_pPerformance->StopEx(this->m_AudioPath.m_pAudioPath, 0, 0);
  
  return true;
}

bool CSound::IsPlaying()
{
  if (!m_pPerformance)
   {
	 LogError("sound instance not initted.");
	 return false;
   }

  if (S_OK == m_pPerformance->IsPlaying(m_pSegment,m_pSegmentState8)) return true;


  return false;
}


void CSound::SetForceMainSegment(bool bNew)
{
  m_bMainSegmentOverride = bNew;
  if (m_bMainSegmentOverride) m_bMainSegment = m_bMainSegmentOverride;
  
}


void CSound::SetStayAlive(bool bNew)
{
  this->m_bStayAlive = bNew;
//  if (m_bMainSegmentOverride) m_bMainSegment = m_bMainSegmentOverride;
  
}


} //end gdp namespace