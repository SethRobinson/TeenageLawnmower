#include "dlg_security.h"
#include "resource.h"
#include "security_online.h"
#include "CProtection.h"
#include "std_all.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CALLBACK update_dlg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM );

CServerInfo server_info;

class security_glo
{
public:
    char m_st_class[30];
    char m_st_title[30];
   
    HWND m_hWnd;
    HINSTANCE m_hInst;
    
    bool m_b_done; //signal the finishing of everything
    security_glo()
    {
      
      memset(this, 0, sizeof(security_glo)); //blank us out
      strcpy(m_st_class, "TLMWINDOW");
      strcpy(m_st_title, "Teenage Lawnmower");
      
    }
   

} s_glo; //security globals



long FAR PASCAL MWndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
 
    // Handle messages
    
    switch (msg) 
    {
        // handle chat mode
        
    case WM_CHAR:
        break;

  
            
case WM_CREATE:
          
 

break;
      case WM_CLOSE:
            DestroyWindow(s_glo.m_hWnd);
             break;
            
        case WM_DESTROY:	
            
            PostQuitMessage(0);
             s_glo.m_b_done = true;
          break;
            
  
        
    }

   return DefWindowProc(hWnd, msg, wparam, lparam);

}   
    
    
//------------------------------------------------------------------
// 
// Function     : WinMain()
//
// Purpose      : Entry point to application
//
//------------------------------------------------------------------



BOOL InitClass(HINSTANCE hInst)
{
	
    // Fill out WNDCLASS info
	
	WNDCLASS    wndClass;
    
	wndClass.style              = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc        = MWndProc;
    wndClass.cbClsExtra         = 0;
    wndClass.cbWndExtra         = 0;
    wndClass.hInstance          = hInst;
    wndClass.hIcon              = LoadIcon(hInst,MAKEINTRESOURCE(IDI_MAIN_ICON));
    wndClass.hCursor            = LoadCursor( NULL, IDC_ARROW );
    wndClass.hbrBackground      = (HBRUSH)GetStockObject(GRAY_BRUSH);
    //wndClass.lpszMenuName       = MAKEINTRESOURCE(IDR_MENU1);
    wndClass.lpszMenuName       = NULL;
 

    wndClass.lpszClassName      = s_glo.m_st_class;
    
    if (!RegisterClass(&wndClass)) return FALSE;
	
    // Everything's perfect
    return TRUE;
}

BOOL InitWindow(HINSTANCE hInst)
{
	
//	char buff[120];
//	LoadString(hInst,APP_CAPTION,buff,sizeof(buff));
	
    // Create a window
int windowed_width = 600;
int windowed_height = 400;	
	s_glo.m_hWnd = CreateWindowEx(WS_EX_APPWINDOW,
		s_glo.m_st_class,
		s_glo.m_st_title,
		WS_OVERLAPPEDWINDOW,
		(GetSystemMetrics(SM_CXSCREEN)-windowed_width)/2,
		(GetSystemMetrics(SM_CYSCREEN)-windowed_height)/2,
		windowed_width,
		windowed_height,
		NULL,
		NULL,
		hInst,
		NULL);
	
	// Return false if window creation failed
    if (!s_glo.m_hWnd) return FALSE;
	
    // Show the window

    return TRUE;
}

bool SecurityCheck(HINSTANCE h_inst)
{
   s_glo.m_hInst = h_inst;
   if (!InitClass(h_inst))
   {
       log_error("Error with initclass.");
       return false;
   }

   if (!InitWindow(h_inst))
   {
       log_error("Error with initwindow.");
       return false;
  }
  bool b_result; 
  if (!g_settings.KeyInfoExists())
  {
  
  b_result =  (DialogBox(s_glo.m_hInst,MAKEINTRESOURCE(IDD_UNLOCK), s_glo.m_hWnd ,unlock_dlg) != 0);
   s_glo.m_b_done = true;
  } else
  {
      //they already have a valid key, let's just double check it though IF they are online.
   b_result =  (DialogBox(s_glo.m_hInst,MAKEINTRESOURCE(IDD_UPDATE), s_glo.m_hWnd ,update_dlg) != 0);
   s_glo.m_b_done = true;

  }
   
   
   
   MSG msg;

    while (1)
    {
    
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
           
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else
        {
      //a good time to quit?
          if (s_glo.m_b_done)
          {
              UnregisterClass(s_glo.m_st_class, h_inst);
              return b_result;
          }

        }
    }


}



BOOL CALLBACK unlock_dlg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM )
{
  

	static HBRUSH dialog_brush;

   // Handle messages
    switch (uMsg) 
    {

	case WM_CTLCOLOREDIT:

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORDLG:
	//	SetBkMode((HDC)wParam,TRANSPARENT);
		return (int)dialog_brush;

    case WM_DESTROY:
	 //   	DeleteObject(dialog_brush); 
		break; 

	case WM_INITDIALOG:
		{

             	SetWindowText(hWnd, s_glo.m_st_title);
              	SetDlgItemText(hWnd, IDC_STATUS, "Waiting for you.  :)");


		}
		break;


	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
		case IDCANCEL:

            EndDialog (hWnd, FALSE);
		
			  break;
			
		case IDOK:
	
            //start check procedure
                SetDlgItemText(hWnd, IDC_STATUS, "Finding RTsoft server...");
                
               if (!server_info.GetServerInfo("www.rtsoft.com", "tlm/server.txt"))
                {
                    //unable to connect, let them play anyway.
                          
                   int i_ret = MessageBox(hWnd,"Unable to connect with www.rtsoft.com.\n\nIf you are not currently connected to the internet you will need to connect to unlock the game.\n\nIt also may be down or rebooting, please try again later.  Sorry for the hassle!", "Error connecting", MB_OK);
                 EndDialog (hWnd, TRUE);
                    return false;

                }
                      SetDlgItemText(hWnd, IDC_STATUS, "Found server.  Contacting RTsoft, please wait...");
                if (server_info.m_f_version_required > app_glo.f_version)
                {
                    int i_ret = MessageBox(hWnd,server_info.m_st_upgrade_text, "New version released!", MB_OKCANCEL);
                    if (i_ret == IDOK)
                    {
                        winall_create_url_file(server_info.m_st_upgrade_url);
                        int result = (int)ShellExecute(NULL,"open","temp.url", NULL,NULL, SW_MAXIMIZE);
                        ProcessError(s_glo.m_hWnd, result, "temp.url",server_info.m_st_upgrade_url); 
                    }
                    
                    //in either case, quit out.
                    EndDialog (hWnd, FALSE);
                    return false;
                }
                
                if (server_info.m_f_version_latest > app_glo.f_version)
                {
                    
                    char st_temp[256];
                    sprintf(st_temp, "Hey, version v%.2f of Teenage Lawnmower has been released! If you wish to upgrade now, click yes.", server_info.m_f_version_latest);
                    int i_ret = MessageBox(hWnd,st_temp, "New version released!", MB_YESNO);
                    if (i_ret == IDYES)
                    {
                        winall_create_url_file(server_info.m_st_upgrade_url);
                        int result = (int)ShellExecute(NULL,"open","temp.url", NULL,NULL, SW_MAXIMIZE);
                        ProcessError(s_glo.m_hWnd, result, "temp.url",server_info.m_st_upgrade_url); 
                        //in either case, quit out.
                        EndDialog (hWnd, FALSE);
                        return false;
                    }
                    
                }
  
                char st_code[255];

                //get input

               	GetDlgItemText( hWnd, IDC_UNLOCK, (char*)&st_code, 254);
		

                //go ahead and communicate with the main server
                char st_message[256];
                char st_key[100];
                if (!server_info.GetAuthorization(st_code, st_key, st_message, false))
                {
                  	//write new settings
                    g_settings.ResetKey();
                    
                    MessageBox(hWnd,st_message, "Error", NULL);
                    EndDialog (hWnd, FALSE); //temop to test it
                    return false;
                }
                                
           //let's copy whatever it is into our global area
             g_settings.SetKey(st_code, st_key);
		      MessageBox(hWnd,"The game has been successfully unlocked.\n\nA huge thank you for buying it and supporting independent game development!", "Key activated!", MB_OK);
              	
		
		EndDialog (hWnd, TRUE);
			break;
		}
		
	}

    return FALSE;

}

const int WM_UPDATE = WM_USER+20;

BOOL CALLBACK update_dlg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM )
{
  
   
	//static HBRUSH dialog_brush;

   // Handle messages
    switch (uMsg) 
    {

	case WM_CTLCOLOREDIT:

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORDLG:
	//	SetBkMode((HDC)wParam,TRANSPARENT);
		//return (int)dialog_brush;

    case WM_DESTROY:
	 //   	DeleteObject(dialog_brush); 
		break; 
    case WM_TIMER:
        KillTimer(hWnd,1) ; //this this timer

        SendMessage(hWnd, WM_UPDATE, 0, 0);

        break;
	case WM_INITDIALOG:
		{

           		SetWindowText(hWnd, s_glo.m_st_title);
              	SetDlgItemText(hWnd, IDC_STATUS, "Initializing...");
              
                SetTimer(hWnd, 1, 300, NULL);
				//SendMessage(hWnd, WM_UPDATE, 0, 0);

		}
		break;

    case WM_UPDATE:
        {
        
            //start check procedure
                SetDlgItemText(hWnd, IDC_STATUS, "Finding update server...");
                
                if (!server_info.GetServerInfo("www.rtsoft.com", "tlm/server.txt"))
                {
                    //unable to connect, let them play anyway.
                    EndDialog (hWnd, TRUE);
                    return false;

                }
                SetDlgItemText(hWnd, IDC_STATUS, "Found server.  Checking for updates...");
                if (server_info.m_f_version_required > app_glo.f_version)
                {
                    int i_ret = MessageBox(hWnd,server_info.m_st_upgrade_text, "New version released!", MB_OKCANCEL);
                    if (i_ret == IDOK)
                    {
                        winall_create_url_file(server_info.m_st_upgrade_url);
                        int result = (int)ShellExecute(NULL,"open","temp.url", NULL,NULL, SW_MAXIMIZE);
                        ProcessError(s_glo.m_hWnd, result, "temp.url",server_info.m_st_upgrade_url); 
                    }
                    
                    //in either case, quit out.
                    EndDialog (hWnd, FALSE);
                    return false;
                }
                
                if (server_info.m_f_version_latest > app_glo.f_version)
                {
                    
                    char st_temp[256];
                    sprintf(st_temp, "Hey, version v%.2f of Teenage Lawnmower has been released! If you wish to upgrade now, click yes.", server_info.m_f_version_latest);
                    int i_ret = MessageBox(hWnd,st_temp, "New version released!", MB_YESNO);
                    if (i_ret == IDYES)
                    {
                        winall_create_url_file(server_info.m_st_upgrade_url);
                        int result = (int)ShellExecute(NULL,"open","temp.url", NULL,NULL, SW_MAXIMIZE);
                        ProcessError(s_glo.m_hWnd, result, "temp.url",server_info.m_st_upgrade_url); 
                        //in either case, quit out.
                        EndDialog (hWnd, FALSE);
                        return false;
                    }
                    
                    
                }
  
                  
                //go ahead and communicate with the main server
                char st_message[256];
                char st_key[100];
                char st_name[80];
                strcpy(st_name, g_settings.GetName());

                if (!server_info.GetAuthorization(st_name, st_key, st_message, true))
                {
                   
                    //error connecting, did not get the code checked.
                
                    /*
                    //write new settings
                    g_settings.ResetKey();
                    g_settings.Save("defaults.dat");
                    MessageBox(hWnd,"An error has occured.  Please restart TLM!", "Error", NULL);
                    EndDialog (hWnd, FALSE); //temop to test it
                    return false;
                    */
                    //good or bad it doesn't matter - it will get caught right before the game starts.
                } else
                {
                  //let's copy whatever it is into our global area
                  g_settings.SetKey(st_name, st_key);

                }
                
                
                
                EndDialog (hWnd, TRUE); //temop to test it
                break;
        }
        
        
    case WM_COMMAND:
        
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            
            EndDialog (hWnd, FALSE);
            
            break;
            
            
            
        }
        }
        return FALSE;
        
}
