/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#ifndef __GDP_SOUND_MAP_H__
#define __GDP_SOUND_MAP_H__

#ifdef _MSC_VER 
#pragma warning (disable:4786)
#endif

#include "..\\SoundUtil.h"
#include "gdpSound.h"
#include <map>
#include <iostream>

using namespace std;

typedef map<u32, gdp::CSound*> mapSound;
typedef mapSound::iterator itorSound;


namespace gdp
{

class SoundMap
{
  mapSound sound; //the stl map of our sounds 	
  u32 uCounter; //unique id counter
  itorSound scan_itor;
  bool b_scan_active;
 
public:
   SoundMap();
   ~SoundMap();
   void Kill();
   u32 Create();
   CSound*Get(u32 uIndex);
   CSound * SoundMap::ScanAllSounds();
   void SoundMap::KillSound(hSoundInstance hSound);
   void SoundMap::ResetSoundScan();

};




} //end namespace

#endif
