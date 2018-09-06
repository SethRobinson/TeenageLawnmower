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

#include "CThing.h"


class CThingStats : public CThing
{

public:
  void activate_draw_toggle(byte dik_key);
 
  CThingStats(); //init   
  virtual void dx_draw();
  virtual void think();
  void SetShowSimpleFPS(bool b_new) {m_b_show_simple_fps = b_new;};

private:
    bool b_draw_active;
  	byte byte_key;
	bool m_b_show_fps;
    bool m_b_show_simple_fps; //if yes, doesn't show graphics card info
};

