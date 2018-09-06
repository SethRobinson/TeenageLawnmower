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

#include "..\CTextParse.h"

//just an easy and quick way to load mod information using CParse

class CMod
{
public:

  CMod();
  void Clear();
  
  bool LoadModInfoByDir(char *p_dir_name);

  char m_st_name[256];
  char m_st_description[512];
  char m_st_path[256]; //not set by the ini
  CTextParse m_parse;


};