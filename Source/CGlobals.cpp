//******************************************
// copyright 2002	Seth A. Robinson

// This file is NOT public domain and may not be shared, distributed or used
// without specific permission in any way.

// seth@rtsoft.com - www.rtsoft.com
//******************************************

#include "CGlobals.h"

#include "sound\\ISoundManager.h"
#include "sound\\dx\\gdpSoundManagerDX.h"
#include "sound\\FMOD\\FMSoundManager.h"
#include "std_all.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CInput cls_input;
CThingList cls_things;
CMapZone cls_map;
CSettings g_settings; //global for settings

ISoundManager *p_cls_sound;
app_globals app_glo;
#include "3d\\CConsole.h"

app_globals::app_globals()
{
	  memset(this, 0, sizeof(app_globals));
	  	f_version = 1.16f;
		
		sprintf(st_version_long, "V%.2f - Compiled %s at %s.", f_version, __DATE__, __TIME__);
	    sprintf(st_version, "V1.16");
		sprintf(st_name, "Teenage Lawnmower");
		p_player = new CPlayer();
}

void app_globals::process_command_line(char *p_st_command_line)
{
	switch_to_current_dir();
	
	strcpy(st_command_line, p_st_command_line);


  if (st_command_line[0])
  {
	  char st_word[100];
	  int i=1;
	
	  while (i < 20) //emergency safety feature
	  {
		  seperate_string(st_command_line, i, ' ', (char*)&st_word);
		  i++;
	  if (!st_word[0]) break; //done
	   
	  //process commands
	
	  if (_stricmp((char*)&st_word, "-DEBUG") == 0)
	  {
	      unlink("debug.txt");
		  this->b_log_to_disk = true;
		  log_msg("Game started in debug mode.");
	  }
      if (_stricmp((char*)&st_word, "-WINDOWED") == 0)
	  {
	      g_settings.SetWindowed(1);
		  log_msg("Game started in windowed mode. (-windowed parm)");
	  }
      if (_stricmp((char*)&st_word, "-NOSOUND") == 0)
	  {
	      this->b_disable_sound = true;
		  log_msg("Sound disabled. (-nosound parm)");
	  }

      if (_stricmp((char*)&st_word, "-NOJOY") == 0)
	  {
	      this->b_disable_joystick = true;
		  log_msg("Joystick(s) disabled. (-nojoy parm)");
	  }


	  }
	 
  }
 

}




char szBuf[4048];
char st_final[4048];


void log_msg(LPCSTR lpFormat, ...)
{

	va_list Marker;
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	
	sprintf(st_final, "%s %s:%s\r\n",show_small_date(),show_time(),szBuf);
	OutputDebugString(st_final);
	if (app_glo.b_log_to_disk)
	{
		add_text(st_final, "debug.txt");
	}

		if (cls_tglo.p_console)
		{
			cls_tglo.p_console->AddText(st_final);
		}
}


void LogMsg(const TCHAR* lpFormat, ...)
{

	va_list Marker;
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	
	sprintf(st_final, "%s %s:%s\r\n",show_small_date(),show_time(),szBuf);
	OutputDebugString(st_final);
	if (app_glo.b_log_to_disk)
	{
		add_text(st_final, "debug.txt");
	}

		if (cls_tglo.p_console)
		{
			cls_tglo.p_console->AddText(st_final);
		}
}



void log_error(const LPCSTR lpFormat, ...)
{
	va_list Marker;
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	
	sprintf(st_final, "%s %s:%s\r\n",show_small_date(),show_time(),szBuf);
	OutputDebugString(st_final);

    
    add_text(st_final, "errors.txt");
	
    if (cls_tglo.p_console)
		{
			cls_tglo.p_console->AddText(st_final);
		}

}

void LogError(const TCHAR * lpFormat, ...)
{
	va_list Marker;
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	
	sprintf(st_final, "%s %s:%s\r\n",show_small_date(),show_time(),szBuf);
	OutputDebugString(st_final);
//	if (app_glo.b_log_to_disk)
//	{
	  //also writ this to disk
		add_text(st_final, "errors.txt");
//	}
	if (cls_tglo.p_console)
		{
			cls_tglo.p_console->AddText(st_final);
		}

}

