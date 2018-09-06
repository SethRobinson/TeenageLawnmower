/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "gdpAudioPath.h"

#include "..\\..\\all.h"

namespace gdp
{
	
CAudioPath::CAudioPath()
{
	memset(this, 0, sizeof(CAudioPath));
}

CAudioPath::~CAudioPath()
{
	Kill();
}


void CAudioPath::Kill()
{
    //stop playing
  //  if (m_p_performance) 
  //  m_p_performance->StopEx(m_pAudioPath, 0, 0);

    
    //shut everything down if we need to
	SAFE_RELEASE(m_lpDSB8);
	SAFE_RELEASE(m_lpDS3DB8);
	SAFE_RELEASE(m_pAudioPath);
}

bool CAudioPath::SetEcho(bool bActive)
{
	if (!m_pAudioPath)
	{
		LogError("Can't activate echo, audio path not initted");
		return false;
	}
	m_pAudioPath->Activate(false);
	
	// Describe the effect.
	
	DSEFFECTDESC dsEffect;
	dsEffect.dwSize = sizeof(DSEFFECTDESC);
	dsEffect.dwFlags = 0;
	dsEffect.guidDSFXClass = GUID_DSFX_STANDARD_ECHO;
	dsEffect.dwReserved1 = 0;
	dsEffect.dwReserved2 = 0;
	
	DWORD dwResults;
	HRESULT hr;
	
	// Set the effect
	if (FAILED(hr = m_lpDSB8->SetFX(1, &dsEffect, &dwResults)))
	{  
		LogError("Unable to set effect on audio path (%s)", DXGetErrorString8(hr));	
		return false;
	}
	
	// You can check the value of dwResults here to see if and how 
	// the effect was allocated.
	
	// Activate the path.
	
	m_pAudioPath->Activate(TRUE);
	/*
	
	
	  */
	
	return true;
}

bool CAudioPath::SetChorus(bool bActive, u32 uAmount)
{
	if (!m_pAudioPath)
	{
		LogError("Can't activate effect, audio path not initted");
		return false;
	}
	m_pAudioPath->Activate(false);
	
	// Describe the effect.
	
	DSEFFECTDESC dsEffect;
	dsEffect.dwSize = sizeof(DSEFFECTDESC);
	dsEffect.dwFlags = 0;
	dsEffect.guidDSFXClass = GUID_DSFX_STANDARD_CHORUS;
	dsEffect.dwReserved1 = 0;
	dsEffect.dwReserved2 = 0;
	
	DWORD dwResults;
	HRESULT hr;
	
	// Set the effect
	if (FAILED(hr = m_lpDSB8->SetFX(1, &dsEffect, &dwResults)))
	{  
		LogError("Unable to set effect on audio path (%s)", DXGetErrorString8(hr));	
		return false;
	}
	
	// You can check the value of dwResults here to see if and how 
	// the effect was allocated.
	
	// Activate the path.
	
	m_pAudioPath->Activate(TRUE);
	/*
	
	  */
	
	return true;
}


bool CAudioPath::Create(IDirectMusicPerformance8 * p_Performance)
{
	//add an audio path
	HRESULT hr;
	
	
	if( FAILED( hr = p_Performance->CreateStandardAudioPath( DMUS_APATH_DYNAMIC_STEREO, 64, 
		TRUE, &m_pAudioPath ) ) )
	{
	LogError("Max audio paths already initted, can't create a new one? (%s)", DXGetErrorString8(hr));	
		return false;
	}
	
	//get DSoundbuffer handle to the default audio path to use later 
	if( FAILED( hr = m_pAudioPath->GetObjectInPath( DMUS_PCHANNEL_ALL,
		DMUS_PATH_BUFFER, 0, GUID_NULL, 0, IID_IDirectSoundBuffer8, 
		(LPVOID*) &m_lpDSB8 )))
	{
		LogError("(Create) Error getting handle to sound buffer of audio path. (%s)", DXGetErrorString8(hr));			
		return false;
	}
	

	
	return true;	
}

bool CAudioPath::Create3D(IDirectMusicPerformance8 * p_Performance)
{
	//add an audio path	.. 3d style
	HRESULT hr;
	
	m_p_performance = p_Performance;
	if( FAILED( hr = p_Performance->CreateStandardAudioPath( DMUS_APATH_DYNAMIC_3D, 64, 
		TRUE, &m_pAudioPath ) ) )
	{
		LogMsg("Max audio paths already initted, can't create a new one? (%s)", DXGetErrorString8(hr));	
		return false;
	}
	
	//get DSoundbuffer handle to the default audio path to use later 
	if( FAILED( hr = m_pAudioPath->GetObjectInPath( 
		DMUS_PCHANNEL_ALL,
		DMUS_PATH_BUFFER, 
		0,
		GUID_NULL, 
		0,
		//IID_IDirectSoundBuffer8, 
		IID_IDirectSound3DBuffer,
		(LPVOID*) &m_lpDS3DB8 ))
		)
	{
		LogError("Error getting handle to 3d sound buffer of audio path. (%s)", DXGetErrorString8(hr));			
		return false;
	}
	
	return true;	
}



void CAudioPath::SetEffect(u32 uEffectType, bool bActive, int iSetting)
{

	
		if (m_lpDS3DB8)
	{
		LogError("Cannot set an effect on a 3d sound buffer."); //this may or may not be true
		return;
	} 
	

	
	if (uEffectType == ::C_SOUND_EFFECT_ECHO)
	{
		//setup echo on the channel
		SetEcho(bActive);
	} else
	if (uEffectType == ::C_SOUND_EFFECT_CHORUS)
	{
		//setup echo on the channel
		SetChorus(bActive, iSetting);
	} else
	{
		//unknown
		LogError("Unsupported effect type, can't set effect.");
	}
	
}

bool CAudioPath::SetPitch(int iPitch)
{
	HRESULT hr;

	if (m_lpDS3DB8)
	{
		LogError("Cannot set pitch on a 3d sound buffer."); //this may or may not be true
		return false;
	} 
	

if( FAILED( hr= m_lpDSB8->SetFrequency(iPitch)))
	{
		LogError("Failed setting freq");
		return false;
	}
 return true;
}

} //end gdp namespace