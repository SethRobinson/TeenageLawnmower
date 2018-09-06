/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



//This class supports grabbing a single key or set of keys, to set custom key
//bindings for game functions.  (ctrl-a, or b as examples)
//Use CPicInput for things like entering whole words.

#include "CThing.h"

const int C_INPUT_NOTHING = 0;
const int C_INPUT_WAIT_FOR_KEY = 1;


class CThingInput : public CThing
{

public:
    CThingInput();
    virtual ~CThingInput();
    void SetScriptCallbackOnKeypress(int i_script, int i_mask);
    virtual void think(); 
  
protected:

    int m_i_script; //script to call back when they key(s) are hit
    int m_i_key_mask; //specify the key mask, not used yet
    int m_i_operation;
  
 };
