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

#include "CVisualMulti.h"

class CVisualSky : public CVisualMulti
{
public:
  CVisualSky();
  virtual void CVisualSky::dx_draw();
  void CVisualSky::Init(char *p_st_tex);
  void CVisualSky::UpdateTexture(char *p_st_tex);

protected:

};