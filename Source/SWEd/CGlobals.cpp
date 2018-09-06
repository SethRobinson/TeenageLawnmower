#include "CGlobals.h"

CInput cls_input;
CThingList cls_things; //3d objects, entities
CMapZone cls_map;  //world's map data

#include "..\\..\\..\\shared\3d\\CControlPlayer.h"
#include "..\\..\\..\\shared\3d\\CFloatCam.h"
#include "..\\..\\..\\shared\3d\\CControlTopView.h"
#include "..\\..\\..\\shared\3d\\CEntPlayer.h"

class_globals glo;



void log_msg(LPCSTR lpFormat, ...)
{



	va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	
	//Let's send it via a post to the window, safer this way.
		CString cst_final;
//	cst_final.Format("%s %s:%s",show_small_date(),show_time(),szBuf);

    	cst_final.Format("%s",szBuf);

    if (!glo.p_dlg_log)
	{
		
         OutputDebugString(cst_final);		
		 OutputDebugString("\n");
		 return;
	}



	char *st_buffer;
  	st_buffer = (char *) calloc(1,4048); //allocate memory on the heap
	//don't worry, when the post is recieved it will deallocate it
	      OutputDebugString(cst_final);		
		 OutputDebugString("\n");

	strcpy(st_buffer, cst_final);
	PostMessage(glo.p_dlg_log->m_hWnd,WM_ADD_TO_LOG, 0, (LONG) st_buffer);

}

void LogMsg(LPCSTR lpFormat, ...)
{



	va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	
	//Let's send it via a post to the window, safer this way.
		CString cst_final;
//	cst_final.Format("%s %s:%s",show_small_date(),show_time(),szBuf);

    	cst_final.Format("%s",szBuf);

    if (!glo.p_dlg_log)
	{
		
         OutputDebugString(cst_final);		
		 OutputDebugString("\n");
		 return;
	}



	char *st_buffer;
  	st_buffer = (char *) calloc(1,4048); //allocate memory on the heap
	//don't worry, when the post is recieved it will deallocate it
	      OutputDebugString(cst_final);		
		 OutputDebugString("\n");

	strcpy(st_buffer, cst_final);
	PostMessage(glo.p_dlg_log->m_hWnd,WM_ADD_TO_LOG, 0, (LONG) st_buffer);

}
void log_error(LPCSTR lpFormat, ...)
{

	va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	//Let's send it via a post to the window, safer this way.
	CString cst_final;
	cst_final.Format("%s %s: ERROR - %s",show_small_date(),show_time(),szBuf);
if (!glo.p_dlg_log)
	{
		
		OutputDebugString(cst_final);		
		OutputDebugString("\n");
		return;

	}
   	char *st_buffer;
  
	st_buffer = (char *) malloc(4048); //allocate memory on the heap
	//don't worry, when the post is recieved it will deallocate it
	if (st_buffer)
	{
	strcpy(st_buffer, cst_final);
	PostMessage(glo.p_dlg_log->m_hWnd,WM_ADD_TO_LOG, 0, (LONG) st_buffer);
	OutputDebugString(cst_final);		
	OutputDebugString("\n");
	
	} else OutputDebugString("Error allocating memory in log_error");

}

void LogError(char const * lpFormat, ...)
{

	va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	//Let's send it via a post to the window, safer this way.
	CString cst_final;
	cst_final.Format("%s %s: ERROR - %s",show_small_date(),show_time(),szBuf);
if (!glo.p_dlg_log)
	{
		
		OutputDebugString(cst_final);		
		OutputDebugString("\n");
		return;

	}
   	char *st_buffer;
  
	st_buffer = (char *) malloc(4048); //allocate memory on the heap
	//don't worry, when the post is recieved it will deallocate it
	if (st_buffer)
	{
	strcpy(st_buffer, cst_final);
	PostMessage(glo.p_dlg_log->m_hWnd,WM_ADD_TO_LOG, 0, (LONG) st_buffer);
	OutputDebugString(cst_final);		
	OutputDebugString("\n");
	
	} else OutputDebugString("Error allocating memory in log_error");

}
