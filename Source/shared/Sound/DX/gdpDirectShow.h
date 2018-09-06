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

#ifdef _DSHOW
#include "dshow.h"
#endif

class DirectShow
{

public:

    #ifdef _DSHOW

    DirectShow();
    ~DirectShow();
    void DirectShow::Kill();
    void Init();
    bool DirectShow::Load(const char *p_st_fname);
    bool DirectShow::Stop();
    bool DirectShow::Play(int i_count);
    bool DirectShow::Seek(LONGLONG l_pos);
    bool DirectShow::HandleMessages();
    bool DirectShow::Rewind();

private:

IGraphBuilder *m_pGraph;
    IMediaControl *m_pMediaControl;
    IMediaEvent   *m_pEvent;
    IMediaSeeking   *m_pMediaSeeking;
    int m_i_times_to_play;
    bool m_b_currently_running;


#else


    void DirectShow::Kill() {};
    void Init() {};
    bool DirectShow::Load(char *p_st_fname) {return false;};
    bool DirectShow::Stop() {return false;};
    bool DirectShow::Play(int i_count) {return false;};
    bool DirectShow::Seek(int l_pos) {return false;};
    bool DirectShow::HandleMessages() {return false;};
    bool DirectShow::Rewind() {return false;};

#endif

    
};