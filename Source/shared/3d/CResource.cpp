/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CResource.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CResource::CResource()
{

 i_last_used = 0;
 this->i_resource_type = 0;
 b_dynamic = false;
 i_resource_sub_type = ::C_RESOURCE_SUB_NONE;
 this->st_name[0] = 0;
 i_references = 0; //nobody is using us yet
 b_loaded = false; //not loaded yet is default
}

char * CResource::GetName()
{
	return st_name;

}
int CResource::add_ref()
{
  return ++i_references;
}

int CResource::delete_ref()
{
#ifdef _DEBUG
	if (i_references < 1)
	{
	  log_error("delete_ref says ref is already 0!");
	}
#endif
	i_references--;
 
   

	if (i_references == 0)
	{
	  	//unload
		dx_invalidate(); 
	}
   

	
	return i_references;
}



CResource::~CResource()
{
  
}


void CResource::dx_create()
{

}

void CResource::dx_restore()
{

}
void CResource::dx_invalidate()
{

}

void CResource::dx_kill()
{

}




