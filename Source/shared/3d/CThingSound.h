/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#pragma once

#include "CThing.h"
#include "d3dx8math.h"
#include "..\\sound\\ISoundManager.h"
#include "..\\CTimer.h"

class CVisual;

class CThingSound : public CThing
{
public:

	CThingSound();

	hSoundInstance Init3DSound(const D3DXVECTOR3 *p_vec_pos, const char * p_st_file);
	bool Play(int i_repeat);
    virtual void think();
	void CThingSound::SetDistance(float f_min, float f_max);

private:

	CVisual *m_p_visual_attach; //if not null, the sound follows this
	hSoundInstance  m_h_sound_id; //save the sound we are in  the sound manager index
    CTimer m_timer_update_check; //how often we should check about this sound
    int m_i_kill_at;
    int m_i_time_playing;
};