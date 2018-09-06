
#include <windows.h>
#include "stdio.h"
#include <io.h>
#ifdef _DEBUG
#include <stdlib.h>
#include <crtdbg.h>
#endif
#include <fcntl.h>
#include <windowsx.h>
#include "string.h"
#include <process.h>
#include <direct.h>
#include "all.h"


//these are used to launch a url.  call like this:

//  winall_create_url_file("www.rtsoft.com/smartmute/chatwall");
//  int result = (int)ShellExecute(NULL,"open","temp.url", NULL,NULL, SW_MAXIMIZE);
//  ProcessError(this->m_hWnd,result, "smcw.url","www.rtsoft.com/smartmute/chatwall"); //show an error message if we couldn't open this
  
void winall_create_url_file(char url[255])
{
 //create temp.url
 
    //delete old file if applicable
	unlink("temp.url");
    char st_file[30], st_text[255]; 
	strcpy(st_file, "temp.url");
	sprintf(st_text, "[InternetShortcut]\n");
	add_text(st_text, st_file);
    sprintf(st_text, "URL=http://%s\n",url);
 	add_text(st_text, st_file);

}

void ProcessError(HWND hw, int result, char filename[255], char url[255])
{
if (result > 32)
	{ 
      //no error here.
      return;
	}

 if (result == ERROR_FILE_NOT_FOUND)
 {
	 char st_text[255];
	 sprintf(st_text, "Could not find the file %s in the current dir.\n\nPoint your browser to %s instead.\n",
		 filename,url);
	 MessageBox(hw,st_text, "Error", MB_OK);
     return;
 }

 if (result == SE_ERR_NOASSOC)
 {
	 char st_text[255];
	 sprintf(st_text, "Your system doesn't know how to handle .URL files.  (pre  Win98?)\n\nPoint your browser to %s instead.\n",
		 url);
	 MessageBox(hw, st_text, "Error", MB_OK);
     return;
 }


 	 char st_text[255];
	 sprintf(st_text, "Your system had an unknown error opening the link.\n\nPoint your browser to %s instead.\n",
		 url);
	 MessageBox(hw, st_text, "Error", MB_OK);
}



