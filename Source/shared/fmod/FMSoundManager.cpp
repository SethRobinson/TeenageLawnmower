#include "FMSoundManager.h"
#include "..\\shared\\all.h"
#include <fmod_errors.h> 


CFMSoundManager::CFMSoundManager()
{
    m_b_ready = false;
    m_p_music_mod = NULL;
    m_p_music_sample = NULL;
}

void CFMSoundManager::MuteAll(bool b_new)
{
  FSOUND_SetMute(FSOUND_ALL, b_new);
}

bool CFMSoundManager::Init()
{

  if (FSOUND_GetVersion() < FMOD_VERSION)
    {
        LogError("You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
        return false;
    }
    
    /*
        INITIALIZE
    */
   //FSOUND_SetBufferSize(200);  //causes major lag
       
       
       if (!FSOUND_Init(44100, 8, 0))
    {
        LogError("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        return false;
    }

    
    return m_b_ready = true;
}


CFMSoundManager::~CFMSoundManager()
{
   Kill(); //deallocate everything we had going
}


int CFMSoundManager::Play(char *p_fname)
{
  return PlayMixed(p_fname);
}


//case sensitive!  Always use lower case.
FSOUND_SAMPLE * CFMSoundManager::GetSoundSampleByName(char *p_fname)
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


CFMSound * CFMSoundManager::PreloadSound(char *p_fname)
{
   m_a_sounds.push_back(CFMSound());

   int i_index = m_a_sounds.size()-1; //only works with push back, later we'll add insert where
   //LogMsg("Sound array is now %d.", i_index);
   //a vector has a null sound or something like that
   m_a_sounds[i_index].p_sample = FSOUND_Sample_Load(FSOUND_FREE, p_fname, 0,0, 0);
   m_a_sounds[i_index].st_fname = p_fname; //remember this for later
   return &m_a_sounds[i_index]; //do NOT save this and use it later, the vector could be resized which may
   //invalidate this pointer!
}


int CFMSoundManager::PlayMixed(char *p_fname)
{

 FSOUND_SAMPLE  *p_sample = GetSoundSampleByName(p_fname);

 if (!p_sample)
 {
   //load it, it has not been preloaded yet.
   p_sample = PreloadSound(p_fname)->p_sample;

   if (!p_sample)
   {
       LogError("Unable to load sound %s.", p_fname);
       return -1;
   }
 }
     
 int i_channel = FSOUND_PlaySound(FSOUND_FREE, p_sample);
 FSOUND_SetLoopMode(i_channel, FSOUND_LOOP_OFF);
 return i_channel;
}


int CFMSoundManager::PlayLooping(char *p_fname)
{

 int i_channel = PlayMixed(p_fname);
 FSOUND_SetLoopMode(i_channel, FSOUND_LOOP_NORMAL);
 return i_channel;
}

void CFMSoundManager::KillChannel(int i_channel)
{
   FSOUND_StopSound(i_channel);
}


int CFMSoundManager::PlayMusic( char *p_fname, int i_loop_count)
{
    
    KillMusic();
    
    m_p_music_sample = FSOUND_Sample_Load(FSOUND_FREE, p_fname, 0,0, 0);
    int i_channel = FSOUND_PlaySound(FSOUND_FREE, m_p_music_sample);
    
    if (i_loop_count <= 1)
    {
        
        FSOUND_SetLoopMode(i_channel, FSOUND_LOOP_OFF);
    } else
    {
        FSOUND_SetLoopMode(i_channel, FSOUND_LOOP_NORMAL);
        
    }
    
    return 1;
}

void CFMSoundManager::KillMusic()
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

void CFMSoundManager::Kill()
{
    KillMusic();
    
  FSOUND_Close();

}