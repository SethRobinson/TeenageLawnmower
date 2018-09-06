//******************************************
// copyright 2002	Seth A. Robinson

// This file is NOT public domain and may not be shared, distributed or used
// without specific permission in any way.

// seth@rtsoft.com - www.rtsoft.com
//******************************************

#include "CPlayer.h"

#include "std_all.h"
#include "3d\\CThing_globals.h"
#include "CScriptEngine.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPlayer::CPlayer()
{
	//clear it.  Don't use any virtuals or classes within this class or this
	//could be a problem...
	ZeroMemory(this, sizeof(class CPlayer));
	//Set items to default.
	m_f_file_version = D_CPLAYER_VERSION;
	m_i_money = 1; //start with 1 buck
	strcpy(st_save_description, "No description");
}

void CPlayer::ModifyMoney(	int i_amount)
{
	m_i_money += i_amount;
	//make sure it's within range
	force_range(&m_i_money, 0, 2000000000);
}


void CPlayer::ModifyReputation(float f_amount)
{
	m_f_reputation += f_amount;
	//within range?
	fforce_range(&m_f_reputation, 0, 100);
}



bool CPlayer::Save(char *p_st_filename)
{
	//open the file
	FILE *fp = fopen(p_st_filename, "wb");

	if (!fp)
	{
		log_error("Error saving player data file %s.", p_st_filename);
		return false;
	}
	
	
	int i_size_of_us = sizeof(CPlayer);

   	//first write our data
	fwrite(&i_size_of_us, sizeof(int), 1, fp);

	//now write the real data
	fwrite(this, i_size_of_us, 1, fp);

		//now let's write our global variables

	cls_tglo.p_script_engine->SaveScriptVars(0, fp); //save globals

	
	
	fclose(fp);
   return true;
}

bool CPlayer::LoadDescription(char *p_st_out, char *p_st_filename)
{
    //load a description of a file into a buffer and return it
    CPlayer p_temp;
  	//clear us
    FILE *fp = fopen(p_st_filename, "rb");

	if (!fp)
	{
	//	log_error("Error finding player data file %s.", p_st_filename);
	    strcpy(p_st_out, "Empty");
        return false;
	}

	//clear us
	ZeroMemory(&p_temp, sizeof(CPlayer));


	//load it
    int i_our_size = 0;
  
  //first load the size we're supposed to be
  if (!fread(&i_our_size, sizeof(int), 1, fp))
  {
	  log_error("Couldn't read player size info");
	  return false;
  }

  if (!fread(&p_temp, i_our_size, 1, fp))
  {
	  log_error("Couldn't read player size info");
	  return false;
  }
  
    fclose(fp);

    strcpy(p_st_out, p_temp.st_save_description);
    return true;
}


bool CPlayer::Load(char *p_st_filename)
{
	FILE *fp = fopen(p_st_filename, "rb");

	if (!fp)
	{
		log_error("Error finding player data file %s.", p_st_filename);
		return false;
	}

	//clear us
	ZeroMemory(this, sizeof(CPlayer));

	//load it
    int i_our_size = 0;
  
  //first load the size we're supposed to be
  if (!fread(&i_our_size, sizeof(int), 1, fp))
  {
	  log_error("Couldn't read player size info");
	  return false;
  }

  if (!fread(this, i_our_size, 1, fp))
  {
	  log_error("Couldn't read player size info");
	  return false;
  }
  	//add//replace extra var values in the main script
   	cls_tglo.p_script_engine->LoadScriptVars(0, fp); //load all vars into our globals



	//all done
   fclose(fp);
	
	return true;
}