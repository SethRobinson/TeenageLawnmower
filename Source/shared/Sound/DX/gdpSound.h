/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */

#ifndef __GDP_SOUND_H__
#define __GDP_SOUND_H__



#include "..\\SoundUtil.h"
#include <dmusici.h>
#include "gdpAudioPath.h"
#include "gdpDirectShow.h"


namespace gdp
{

//Holds a sound instance
	
class CSound
{

	
public:

	IDirectMusicSegmentState8 *m_pSegmentState8;

	
	CSound();
	~CSound();
	void Kill();
	bool CSound::Load(IDirectMusicLoader8*  m_pLoader, const char stName[MAX_PATH]);
    bool CSound::Init(IDirectMusicPerformance8* m_pPerformanceNew, bool bIs3d);
  	bool CSound::Play(u32 iCount);
	hSoundInstance hMyInstance;
	bool CSound::Stop();
	bool CSound::IsPlaying();
	void CSound::SetForceMainSegment(bool bNew);
	void CSound::SetStayAlive(bool bNew);
	bool GetStayAlive(){return m_bStayAlive;};
	bool CSound::Set3DPosition(float f_x, float f_y, float f_z);
	bool CSound::Set3DDistance(float f_min, float f_max);
    bool CSound::UpdateDirectShow();

   	CAudioPath m_AudioPath;

private:

	IDirectMusicSegment8*   m_pSegment;
	IDirectMusicPerformance8* m_pPerformance;
	bool m_bMainSegment;
	bool m_b3d;
	bool m_bMainSegmentOverride;
	bool m_bStayAlive;
    DirectShow *m_p_dshow; //if not null then we're using directshow, not directaudio.
    IDirectMusicLoader8* m_p_loader; //keep track of the loader we are passed
};


} //end namespace

#endif