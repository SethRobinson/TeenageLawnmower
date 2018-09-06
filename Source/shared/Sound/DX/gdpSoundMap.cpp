/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


//This controls add/removing resources and maintains the stl map.

#include "gdpSoundMap.h"
#include "..\\..\\all.h"
namespace gdp
{


	
//contruction/deconstruction
SoundMap::SoundMap()
{
  uCounter = 1000; 
  this->b_scan_active = false;
 
}

SoundMap::~SoundMap()
{
  Kill();
}

void SoundMap::Kill()
{
  //go through our stuff and kill each one
		for (itorSound itor = sound.begin(); itor != sound.end();)
	{
	   //delete everything
		//first delete the class the pointer was pointing at
		delete itor->second;
		sound.erase(itor++);
	}

}


void SoundMap::KillSound(hSoundInstance hSound)
{
  //sound[hSound]->Stop();
    
  SAFE_DELETE (sound[hSound]);
  sound.erase(hSound);
  //SMsg("Sound %u deleted.",hSound);
}


u32 SoundMap::Create()
{
  //add a new sound object
  uCounter++;
  sound[uCounter] = new CSound();
//  SMsg("Created handle %u.",uCounter);
  sound[uCounter]->hMyInstance = uCounter;
  return uCounter;
}


CSound * SoundMap::Get(u32 uIndex)
{
  
    
   itorSound iitor = sound.find(uIndex);

    if (iitor == sound.end())
    {
        //not in hash.
        return NULL;
    }
    
    return iitor->second;
    
    /*
    
  static CSound *p_sound;
  p_sound = sound[uIndex];

  if (!p_sound)
  {
    //not initted yet, bad handle.
	sound.erase(uIndex);
	return NULL;
  }
  return p_sound;
  */

}
   
//if this is called, it MUST either cycle through ALL sounds or call ResetSoundScan
//when done.  So it will be ready for the next call.  Not multithreadable!

CSound * SoundMap::ScanAllSounds()
{
  	CSound *p_sound_temp = NULL;
	
	if (!b_scan_active)
   {
	 scan_itor = sound.begin();
	 b_scan_active = true;
   }

   if (scan_itor == sound.end())
   {
	 b_scan_active = false;
	 return NULL; //all done
   }

   //found one we want to return
   p_sound_temp = scan_itor->second;
  
   //inc the item
   ++scan_itor;
   return p_sound_temp; //keep scanning

}

void SoundMap::ResetSoundScan()
{
    b_scan_active = false;
}


}  //end gdp


