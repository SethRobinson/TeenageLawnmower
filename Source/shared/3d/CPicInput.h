#pragma once

#include "CPic.h"

class CPicInput: public CPic
{

public:
    CPicInput();
    virtual ~CPicInput();
    virtual void think();
    void SetInputSoundEffect(const char *p_st_sound);
   	virtual void receive_message(CMessageHeader *p_header, void *p_message);
    void ProcessAsciiKey(char c_key);
    virtual void dx_draw(); //override the draw so we can add our blinking cursor
private:
    
    int m_i_max_string_size; //how long the max input can be
    CTimer timer_blink;
    bool b_cursor_blink_on; 
    char m_st_sound[MAX_PATH]; //optional sound effect for input
    

};