#include <fmod.h>
#include <vector>
#include <string>

using namespace std;


class CFMSound
{
public:
    CFMSound()
    {
        p_sample = NULL;
    }
    FSOUND_SAMPLE *p_sample;
    string st_fname;
};



class CFMSoundManager
{
public:

    CFMSoundManager();
    ~CFMSoundManager();
    
    int PlayMixed(char *p_fname);
    int Play(char *p_fname);
    int PlayMusic(char *p_fname, int i_loop_count);
    
    bool Init();
    void UpdateSounds() {}; //called every frame, only need for fading/3d
    void MuteAll(bool b_new);


    void Kill(); //kills everything, unloads all sounds and main driver
    void KillMusic(); //unloads just the music

    FSOUND_SAMPLE * GetSoundSampleByName(char *p_fname);
    CFMSound * PreloadSound(char *p_fname);
    int PlayLooping(char *p_fname);
    void KillChannel(int i_channel);

private:

  bool m_b_ready;

  FMUSIC_MODULE *m_p_music_mod; //used only for music
  FSOUND_SAMPLE *m_p_music_sample; //used only for music
  vector<CFMSound> m_a_sounds; //array of our loaded sounds
};