/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */




#include "CThing_globals.h"

//a little extra info especiall for MD2 things

class CVisualMulti; //foward for speed

const float C_BASE_SPEED_MOD = 17.0f;

class CAnimInfo
{
public:
  CAnimInfo();
  ~CAnimInfo();

  CVisualMulti *p_parent;

  float f_frame;
  int i_anim;
  int i_tex_id;
  float f_speed_mod; //1 for normal speed
  bool m_b_paused;

  LPDIRECT3DTEXTURE8 p_tex; //pointer to the text, for speed.  The CManager->CResourceTex
  //class handles lost surfaces, etc, not us.
  void CAnimInfo::RefreshTextureInfo();
  void CAnimInfo::LoadOrChangeTexture(char *st_filename);
  void CAnimInfo::Draw();
  void CAnimInfo::SetAnimByName(char *st_anim, int i_start_frame);
  void CAnimInfo::SetAnimByIndex(int i_new_anim, int i_start_frame);
  void CAnimInfo::SetSpeedMod(float f_new_speed_mod);

  void CAnimInfo::SetPause(bool b_paused);
};