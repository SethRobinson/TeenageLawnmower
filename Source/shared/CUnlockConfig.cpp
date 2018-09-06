/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CUnlockConfig.h"
#include <cstdio>

const char *C_ST_FNAME = "config.dat";

CConfig::CConfig()
{
    ZeroMemory(this, sizeof(CConfig));
}


void CConfig::SetKey(const char *p_name, const char *p_key)
{
    strcpy(m_st_name, p_name);
    strcpy(m_st_key, p_key);

}

bool CConfig::KeyInfoExists()
{
   if ( (m_st_key[0] == 0) || (m_st_name[0] == 0))
   {
      return false;
   }

  return true;
}


void CConfig::ResetKey()
{
 m_st_key[0] = 0;
 m_st_name[0] = 0;

}
bool CConfig::Save(bool b_force_directory)
{

    //if a mod is open, let's make sure we save to the correct dir
    char st_name[256];
   
    if (b_force_directory)
    {
    sprintf(st_name, "%s%s", GetExeDir(), C_ST_FNAME);
    } else
    {
    sprintf(st_name, "%s",C_ST_FNAME);
    }
    
    //open the file
    FILE *fp = fopen(st_name, "wb");

	if (!fp)
	{
		LogError("Error saving CConfig to %s.", st_name);
		return false;
	}
	
	int i_size_of_us = sizeof(CConfig);

   	//first write our data
	fwrite(&i_size_of_us, sizeof(int), 1, fp);

	//now write the real data
	fwrite(this, i_size_of_us, 1, fp);
 
    fclose(fp);
    return true;
 }


bool CConfig::Load(bool b_force_directory) //false if used by the unlock util
{
   //if a mod is open, let's make sure we save to the correct dir
    char st_name[256];
    if (b_force_directory)
    {
    sprintf(st_name, "%s%s", GetExeDir(), C_ST_FNAME);
    } else
    {
    sprintf(st_name, "%s", C_ST_FNAME);
    }
    
    FILE *fp = fopen(st_name, "rb");

	if (!fp)
	{
		log_msg("%s not found, using defaults.", st_name);
		return false;
	}

	//clear us
	ZeroMemory(this, sizeof(CConfig));

	//load it
    int i_our_size = 0;
  
  //first load the size we're supposed to be
  if (!fread(&i_our_size, sizeof(int), 1, fp))
  {
	  LogError("Couldn't read CConfig size info");
	  fclose(fp);
 return false;
  }

  if (!fread(this, i_our_size, 1, fp))
  {
	  fclose(fp);
        LogError("Couldn't read CConfig info");
	  return false;
  }
  
  //all done
    fclose(fp);
	return true;
}