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

#include "CEnt.h"
#include "..\\CTimer.h"

class CEntHuman : public CEnt
{
 CTimer timer_die;

public:
  CEntHuman();
  virtual void think();

};