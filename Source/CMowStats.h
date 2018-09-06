//******************************************
// copyright 2002	Seth A. Robinson

// This file is NOT public domain and may not be shared, distributed or used
// without specific permission in any way.

// seth@rtsoft.com - www.rtsoft.com
//******************************************



#pragma once

#include "3d\\CThing.h"


class CMowStats : public CThing
{
 	bool b_draw_active;
  	byte byte_key;
public:
 
  CMowStats(); //init   
  virtual void dx_draw();
  virtual void think();
};

