/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CBaseEntData.h"

#include "..\\all.h"
CBaseEntData::CBaseEntData()
{
 	//don't do a memset or you erase the other classes's constructors
	memset(this, 0, sizeof(CBaseEntData));
	strcpy(this->st_name, "Name me!");
	strcpy(this->st_visual, "texture\\unknown.dds");
	this->b_follow_ground_mode = false;
	this->f_temp = 0;
	this->i_draw_mode = 0;
	this->vec_scale.x = vec_scale.y = vec_scale.z = 1;
	//make default item
	p_ent_default = new CEntData();
}



CBaseEntData::~CBaseEntData()
{

	kill();
}

void CBaseEntData::kill()
{
 	SAFE_DELETE(this->p_ent_default); 
}

bool CBaseEntData::save(FILE *fp)
{

 //save out this template

 //first write the size of the record to allow dynamic changes later 
 int i_size = sizeof(CBaseEntData);
 log_msg("Saveing base item %s.. %d bytes.", this->st_name, i_size);
 fwrite(&i_size, sizeof(int), 1, fp);

 if (fwrite(this, i_size, 1, fp) <= 0)
	{
		log_error("Error writing template %s.",st_name);
   return false;	
 }
	
  //now write  the default item we have attached.  We'll let the item
  //write it as it may recursively write items it holds as well. (nobody said it was fast)
  this->p_ent_default->save(fp);
	
   return true;
}

bool CBaseEntData::load(FILE *fp)
{
 	  kill();
   
	   int i_size = 0;

	   fread(&i_size, sizeof(int), 1, fp);

	   if (i_size != sizeof(CBaseEntData))
	   {
		 //something changed in the data structure, clear it before loading
		 //well, we don't need to, it's cleared in the init... 
	   }

	   if (fread(this, i_size, 1, fp) <= 0)
		{
			log_error("Error reading class");
			return false;
		}

	   //now let's load the one item we have and put set our pointer to a valid place
	   //with it (it's garbage now)

	   	this->p_ent_default = new CEntData();

		//now load it's item from the file
		p_ent_default->load(fp);
	return false;
}