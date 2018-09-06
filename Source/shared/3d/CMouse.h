/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



//let's override the pic class to make a mouse control.

#pragma once
#include "CPic.h"

//all purpose class to handle gui and 2d stuff

class CMouse : public CPic
{
public:

    CMouse();
	virtual ~CMouse();
 	virtual void CMouse::think();
    void ResetClickTimer();
    virtual void dx_restore();


private:
	int m_i_click_delay;
	CTimer m_timer_delay;
	bool m_b_wait;
};