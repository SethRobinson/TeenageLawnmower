/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#ifndef __GDP_SOUND_AUDIO_PATH_H__
#define __GDP_SOUND_AUDIO_PATH_H__

#include "..\\SoundUtil.h"
#include <dmusici.h>
#include <dxErr8.h>

namespace gdp
{


//this class holds information about an audiopath instance	

class CAudioPath
{
	
private:
	
	bool CAudioPath::SetEcho(bool bActive);
    IDirectMusicPerformance8 *	m_p_performance; //save this for later
	
public:
	CAudioPath();
	~CAudioPath();
	
	void Kill(); //deinit it
	bool Create(IDirectMusicPerformance8 * p_Performance);
	void CAudioPath::SetEffect(u32 uEffectType, bool bActive, int iSetting);
	inline IDirectMusicAudioPath* GetAudioPath(){return m_pAudioPath;};
	bool SetPitch(int iPitch);
	bool CAudioPath::SetChorus(bool bActive, u32 uAmount);
   	LPDIRECTSOUNDBUFFER8	m_lpDSB8;
	LPDIRECTSOUND3DBUFFER8   m_lpDS3DB8; //used for 3d buffers
	bool CAudioPath::Create3D(IDirectMusicPerformance8 * p_Performance);
   	IDirectMusicAudioPath*  m_pAudioPath;

};
	
//this class is here to deal with the audio paths


}  //end gdp namespace
#endif