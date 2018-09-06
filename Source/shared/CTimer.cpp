/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */

#include "CTimer.h"

CTimer::CTimer()
{
  //init with defaults.  Taking time from a global, faster then getting it
  //ourselves.
  	this->i_creation_time = GetTickCount();
 	reset();
}

void CTimer::reset()
{
    this->i_timer_start = GetTickCount();
    this->i_interval = C_INTERVAL_DISABLED;
    i_stop_time = -1;
}
void CTimer::stop_time()
{
  i_stop_time = GetTickCount() -i_timer_start;
}

int CTimer::get_elapsed_milliseconds()
{
  	if (i_stop_time != -1)
	return i_stop_time;
	return (GetTickCount() -i_timer_start);
}



CTimer::~CTimer()
{

}

void CTimer::set_interval(int i_new_interval)
{
  this->i_interval = i_new_interval;
  i_timer_start = GetTickCount();
}


void CTimer::ResetInterval()
{
   	i_timer_start = GetTickCount();

}

void CTimer::TriggerInterval()
{
   	i_timer_start = -2; //guarantee this will get triggered on
    //the next interval_reached
}


bool CTimer::interval_reached()
{
	if (i_timer_start+i_interval <= (int)GetTickCount())
  {
    //interval has been reached.  Reset it
	i_timer_start = GetTickCount();
	return true;
  }
	//not yet!
 	return false;
}