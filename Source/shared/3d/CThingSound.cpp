/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CThingSound.h"
#include "..\\sound\\ISoundManager.h"
#include "CThing_globals.h"
#include "CThing_util.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CThingSound::CThingSound()
{
	this->m_p_visual_attach = NULL;
    m_h_sound_id = C_SOUND_NONE;
	set_thing_name("3dSnd");
    m_i_kill_at = 60000; //six seconds
    m_i_time_playing = 0;
    m_timer_update_check.set_interval(1000); //this might be a member function later
    //update once a second for now
} 

hSoundInstance CThingSound::Init3DSound(const D3DXVECTOR3 *p_vec_pos, const char * p_st_file)
{
    
    if (!cls_tglo.p_sound)
    {
        assert(0);
        return C_SOUND_NONE;
    }
    
  
    if ((m_h_sound_id = cls_tglo.p_sound->SoundInstanceCreateAndLoad3D(C_SOUND_NONE, CDir(p_st_file))) == C_SOUND_NONE)
    {
        //failed to load sound. kill this thing
        cls_tglo.p_sound->SoundInstanceKill(m_h_sound_id);
        assert(0);
        return C_SOUND_NONE;
    }
   
    SetDistance(25, 0); //default settings, good for my strong listener rolloff I have
   
     if (!cls_tglo.p_sound->SoundInstance3DSetPos(m_h_sound_id, p_vec_pos->x, p_vec_pos->y, p_vec_pos->z))
    {
        assert(0);
    }
   
    //	log_msg("Creating sound %s.", p_st_file);
    

    return m_h_sound_id; //success?
}

bool CThingSound::Play(int i_repeat)
{
	return (cls_tglo.p_sound->SoundInstancePlay(m_h_sound_id, i_repeat));
}


void CThingSound::think()
{

	//did our sound stop playing?  If so, kill us.
   if (m_timer_update_check.interval_reached())
   {
     m_i_time_playing += m_timer_update_check.GetInterval();

	if (!cls_tglo.p_sound->SoundExists(m_h_sound_id))
	{
		b_delete_flag = true;
	}

    if (m_i_time_playing > m_i_kill_at)
    {
        //emergency 3d sound killer, due to directaudio bugs, messages being missed? 
	    cls_tglo.p_sound->SoundInstanceKill(m_h_sound_id);
        b_delete_flag = true;
    }

   }

}

void CThingSound::SetDistance(float f_min, float f_max)
{
 cls_tglo.p_sound->SoundInstance3DSetDistance(m_h_sound_id, f_min, f_max); //0 means default
}




