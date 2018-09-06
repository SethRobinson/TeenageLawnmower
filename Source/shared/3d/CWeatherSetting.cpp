/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CWeatherSetting.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWeatherSetting::CWeatherSetting()
{
	//clear it out
	ZeroMemory(this, sizeof(CWeatherSetting));

    //give a few defaults so if used it won't cause exceptions in the d3d renderer
	vec_light_dir.y = -1;
}


bool CWeatherSetting::Save(char *p_st_fname)
{
	//open the file
	FILE *fp = fopen(p_st_fname, "wb");

	if (!fp)
	{
		log_error("Error saving weather to %s.", p_st_fname);
		return false;
	}
	
	
	int i_size_of_us = sizeof(CWeatherSetting);

   	//first write our data
	fwrite(&i_size_of_us, sizeof(int), 1, fp);

	//now write the real data
	fwrite(this, i_size_of_us, 1, fp);
	
	
	
	fclose(fp);

   return true;
}

bool CWeatherSetting::Load(char *p_st_fname)
{

    char st_fname[256];
    strcpy(st_fname, p_st_fname);
    if (!exist(st_fname))
    {
        sprintf(st_fname, "%s%s", GetExeDir(), p_st_fname);
    }
    
    
    FILE *fp = fopen(st_fname, "rb");

	if (!fp)
	{
		LogError("Error finding file %s to load as weather.", p_st_fname);
		return false;
	}

	//clear us
	ZeroMemory(this, sizeof(CWeatherSetting));

	//load it
    int i_our_size = 0;
  
  //first load the size we're supposed to be
  if (!fread(&i_our_size, sizeof(int), 1, fp))
  {
	  log_error("Couldn't read weather size info");
	  return false;
  }

  if (!fread(this, i_our_size, 1, fp))
  {
	  log_error("Couldn't read weather size info");
	  return false;
  }
 
	//all done
    fclose(fp);

	
	return true;
}