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

class CEntPlayer : public CEnt
{

public:
  CEntPlayer();
  virtual ~CEntPlayer(); //clean up
  virtual void think();
  void SetPlayerView(); //handle toggled views

private:
	int m_i_view; //current camera view mode
	float m_f_max_speed;

	float m_f_camera_height_max;  
	float m_f_camera_height_min;  
	float m_f_camera_mouse_sensitivity;

};
