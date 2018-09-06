#undef D_LOAD_FUNCTION_NAME
#undef D_SAVE_FUNCTION_NAME
#undef D_DATA_NAME
#undef D_FILE_NAME
#undef D_FILE_NAME_LOADER

//******* SET THESE ********

#define D_LOAD_FUNCTION_NAME file_swed_load
#define D_SAVE_FUNCTION_NAME file_swed_save

//name of the class
#define D_DATA_NAME CSwed
//name of the class loader
#define D_DATA_NAME_LOADER CSwedLoader
//file name
#define D_FILE_NAME "swed.dat"

//********************************

#pragma once
#include "stdafx.h"

//never change the contents of this struct

struct D_DATA_NAME_LOADER
{
	float f_version; //version number
	int i_file_size; //easier to do upgrades
};

//customize as needed

struct D_DATA_NAME
{
	float f_version; //version number
  	int i_file_size; //easier to do upgrades
	RECT rect_main; //store position and size of main window
	RECT rect_3d; //store position and size of the 3d window
	RECT rect_log;  //store position and size of the log window
 	RECT rect_tool;  //store position and size of the curren tool window
   	char st_file_name[256];	   //filename of last loaded map
   	char st_weather_file_name[256];	   //filename of last weather loaded
    char st_main_path[256]; //usually NULL or a mod path name

};

//these will get preprocessed into the correct declarations

bool D_LOAD_FUNCTION_NAME (D_DATA_NAME *sys);
bool D_SAVE_FUNCTION_NAME (D_DATA_NAME *sys);