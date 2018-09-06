#include "CGlobals.h" //to get the log functions and version

#include "file_swed.h"



bool D_LOAD_FUNCTION_NAME (D_DATA_NAME *sys)
{
 	memset(sys,0,sizeof(D_DATA_NAME)); //zero out everything
	//Set any defaults
	
	FILE * fp;

    if ( (fp=fopen(D_FILE_NAME, "rb")) == NULL)
	{
	  //Not able to create sys file. Let's set the defaults
	  log_msg("Can't load %s file, creating a new one with defaults.", D_FILE_NAME);
	  //version and filesize will be set on save each time.
	
	  //set some defaults
	  strcpy(sys->st_file_name, "new.zon");
	  strcpy(sys->st_weather_file_name, "data\\editor.wet");
	  return true; //no error
	}

	//load header
	D_DATA_NAME_LOADER sys_loader;

	if (fread(&sys_loader, sizeof(sys_loader), 1, fp) <= 0)
	{
	  log_error("Couldn't read %s header.  Corrupted file?  Delete it maybe.",
		  D_FILE_NAME);
	  fclose(fp);
	  return false;
	}

	char st_crap[255];
	sprintf(st_crap, "Loading file %s. (version %.3f)",D_FILE_NAME, sys_loader.f_version);
	log_msg(st_crap);

	//Load real thing.
	if (fseek(fp, 0, SEEK_SET ))
	{
	 log_error("Unable to perform seek on sys.dat file.");
	}
	if (!fread(sys, sys_loader.i_file_size, 1,fp))
	{
	 log_error("Error with fread loading sys.dat file.");
	}

	fclose(fp);
	return true;
}


bool D_SAVE_FUNCTION_NAME (D_DATA_NAME *sys)
{
	
	//Set some values
 sys->f_version = C_F_VERSION;
 sys->i_file_size = sizeof(D_DATA_NAME);


 //Now let's actually save the bitch.
 FILE * fp;
  
 char st_temp[256];
 getdir(st_temp);
 
 char st_file[256];
 sprintf(st_file, "%s\\%s", st_temp, D_FILE_NAME);
 
 if ( (fp=fopen(st_file, "wb")) == NULL)
	 {
	  log_error("Error saving %s.  Out of HD space or something?", D_FILE_NAME);
	  return false;
	 }

	 if (fwrite(sys, sizeof(D_DATA_NAME), 1, fp) <= 0)
	 {
		log_error("Wrote zero bytes to %s file?  Something is wrong.",
			D_FILE_NAME);
		fclose(fp);
		return false;
	 }
  
 	//done saving.
	 fclose(fp);
//	 log_msg("sys file saved.");
	 return true;


}