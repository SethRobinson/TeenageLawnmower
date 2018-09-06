/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "gdpDirectShow.h"
#include "..\\..\\all.h"
#include "..\\..\\uni.h"

//Define _DSHOW for mp3 support.  I had problems with DirectShow after installing dx9 debugs, so I
//stopped including this code as I don't actually use MP3's.  

#ifdef _DSHOW


DirectShow::DirectShow()
{
    ZeroMemory(this, sizeof(DirectShow));

}

DirectShow::~DirectShow()
{
    Stop();
    Kill();
}

void DirectShow::Init()
{
    //   CoInitialize(NULL);
    //get ready for fun.
    if (m_pGraph) return; //already initted
  // CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, 
  //     IID_IGraphBuilder, (void **)&m_pGraph);
 CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
      IID_IGraphBuilder, (void **)&m_pGraph);
    m_pGraph->QueryInterface(IID_IMediaControl, (void **)&m_pMediaControl);
    m_pGraph->QueryInterface(IID_IMediaEvent, (void **)&m_pEvent);
    m_pGraph->QueryInterface(IID_IMediaSeeking, (void **)&m_pMediaSeeking);
   // log_msg("initting dsound object");

}


void DirectShow::Kill()
{
     SAFE_RELEASE(m_pMediaControl);
    SAFE_RELEASE(m_pEvent);
    SAFE_RELEASE(m_pGraph);
    SAFE_RELEASE(m_pMediaSeeking);
    
  //  CoUninitialize();
 
}

//return TRUE to kill the object

bool DirectShow::HandleMessages()
{
    long evCode, param1, param2;
    HRESULT hr;
    
    if (m_pEvent == NULL) return false;


    while (hr = m_pEvent->GetEvent(&evCode, &param1, &param2, 0), SUCCEEDED(hr))
    {
        hr = m_pEvent->FreeEventParams(evCode, param1, param2);
        if (EC_COMPLETE == evCode)
        { 
            m_i_times_to_play--;
            if (m_i_times_to_play < 1) 
            {
                if (m_i_times_to_play < 0) m_i_times_to_play = 0;
                return true; //all done
            }

            //repeat!
           // Seek(0); //go to beginning
                Rewind();
     
            return false;

        } 
    }
   return false; 
}  

bool DirectShow::Load(const char *p_st_fname)
{
   LPOLESTR olestr = NULL;
   HRESULT hr;
  if (!m_pGraph) return false;
   
  uni fname(p_st_fname);
    // Build the graph
  
  //crashes here after installing dx9.  why?

  hr = m_pGraph->RenderFile(fname.us_data, NULL);

   if (hr != S_OK)
   {
       
       char st_error[256];
       AMGetErrorText(hr, (char*)&st_error, 256);
       LogError("Renderfile of %s failed. (%s)", p_st_fname, st_error);
       return false; //error
   }

   return true; 
}

                                                          
bool DirectShow::Play(int i_count)
{
    
    if (!m_pMediaControl) 
    {
        LogError("Can't play DShow object, not initted.");
        return false;
    }

    if (m_b_currently_running)
    {
         Rewind();
        //Seek(0); //just rewind it, our stream is already playing.
        return true;
    }

    Stop(); 
    Seek(0);
    m_i_times_to_play = i_count;
   
    m_pMediaControl->Run();
    m_b_currently_running = true;
  
  /*  
        IAMDirectSound *p_dsound = NULL;
   m_pMediaControl->QueryInterface(IID_IAMDirectSound, (void **)&p_dsound);
    if (p_dsound)
    p_dsound->SetFocusWindow(GetForegroundWindow(), true); else
    {
        LogError("Unable to get IID_IAMDirectSound interface in DirectShow.cpp.");
    }
    */

    //m_pEvent->WaitForCompletion(INFINITE, &evCode);
    
    return true; 
}

bool DirectShow::Rewind()
{
  IMediaPosition* pMP;
  
   if (!m_pGraph) return false;
 
  HRESULT hr = m_pGraph->QueryInterface(IID_IMediaPosition,(void**)&pMP);
  if (hr != S_OK)
   {
       char st_error[256];
       AMGetErrorText(hr, (char*)&st_error, 256);
       LogError("DShow Rewind() failed. (%s)", st_error);
       return false; //error
   }
 
  hr = pMP->put_CurrentPosition(0);
  pMP->Release();
 
  if (hr != S_OK)
   {
       char st_error[256];
       AMGetErrorText(hr, (char*)&st_error, 256);
       LogError("DShow Rewind() failed. (%s)", st_error);
       return false; //error
   }

   return true;

}


/*

  HRESULT CPlayerMP3::hrSetVolume(long lVolume)
  {
  if(!boIsInitialized()) return _MP3ERR_NOVOLUME_;

  IBasicAudio* pBA;
  HRESULT hrErr = m_lpGraph->QueryInterface(IID_IBasicAudio,(void**)&pBA);
  if(FAILED(hrErr)) return hrErr;

  if(lVolume >      0) lVolume = 0;
  if(lVolume < -10000) lVolume = -10000;
  hrErr = pBA->put_Volume(lVolume);
  pBA->Release();
  if(FAILED(hrErr)) return hrErr;

  return _OK_;
  }

  */

bool DirectShow::Seek(LONGLONG l_pos)
{
   
  if (!m_pGraph) return false;
    
    HRESULT hr;
 
     hr = m_pMediaSeeking->SetPositions
         
         ( (LONGLONG*)&l_pos, AM_SEEKING_AbsolutePositioning, 
        NULL, AM_SEEKING_NoPositioning );
           
     if (hr != S_OK)
   {
       
       char st_error[256];
       AMGetErrorText(hr, (char*)&st_error, 256);
       LogError("DShow Seek failed. (%s)", st_error);
       return false; //error
   }
  return true;
}

bool DirectShow::Stop()
{
    
    m_b_currently_running = false;
    if (!m_pMediaControl) 
    {
        LogError("Can't stop DShow object, not initted.");
        return false;
    }
    
    m_pMediaControl->Stop();
    m_i_times_to_play = 0;
    return true;
}

#endif