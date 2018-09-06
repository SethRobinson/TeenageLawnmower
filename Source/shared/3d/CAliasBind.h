/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


//why doesn't pragma once work here???

#ifndef __CALIASBIND
#define __CALIASBIND

#include "..\\all.h"

#pragma once

//note; changing this would screw up all CSetting alias config files
const int C_ALIAS_MAX_STRING_SIZE = 64;

bool ConvertDikToString(char *st_temp, byte b_dik);


class CAliasBind
{
public:

    CAliasBind();
	void CAliasBind::Clear();
	CAliasBind::~CAliasBind();
    bool CAliasBind::ConvertStringToKeys(char *p_st);
    bool CAliasBind::SetScript(char *p_st);
    void Update();
	void CAliasBind::SetRepeat(int i_repeat);
    void SetVirtualKey(int i_vk){m_i_vk_index = i_vk;}
	int GetVirtualKey(){return m_i_vk_index;};	//-1 means not used
    void GetStringFromKey(char *p_string);
private:
	
  void CAliasBind::SetKeyFromString(char *p_st);	

  int m_i_vk_index; //or -1 if not bound to a virtual key
	
	//if not bound to an alias, then we run this script instead.
	int m_i_script;
	
	byte m_b_key;
	bool m_b_shift;
	bool m_b_alt;
	bool m_b_control;
	int m_i_repeat; // -1 for no repeat, 0 for always respond
};
#endif

