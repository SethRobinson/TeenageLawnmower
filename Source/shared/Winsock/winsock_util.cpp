#include "winsock_util.h"

bool GetLocalAddress(char * p_st_out_final)
{
    struct in_addr *pinAddr;
    LPHOSTENT	lpHostEnt;
	int			nRet;


	// Get our local name
	//
    char st_buff[MAX_PATH];
    
    nRet = gethostname((char*)&st_buff, MAX_PATH);
	if (nRet == SOCKET_ERROR)
	{
	    log_error("Unable to figure out our own IP address.");
        strcpy(st_buff, "(no ip?)");
		return false;
	}

	//
	// "Lookup" the local name
	//
	lpHostEnt = gethostbyname((char*)&st_buff);
    if (lpHostEnt == NULL)
	{
	       log_error("Unable to figure out our own IP address.");
      		strcpy(st_buff, "(no ip?)");
		    return false;
	}

	//
    // Format first address in the list
	//
	
    //first count how many addresses we have
    int i_nics = 0;
  
    for (int i=0; lpHostEnt->h_addr_list[i]; i++ )
    {
      i_nics++;
    }

    p_st_out_final[0] = 0; //clear this
    //ok, now that we know how many let's do this
    int i_chosen_nic = -1; //-1 will signal an error
    for (i=0; lpHostEnt->h_addr_list[i]; i++ )
    {
        
        if (byte(lpHostEnt->h_addr_list[i][0]) == 192) if (byte(lpHostEnt->h_addr_list[i][1]) == 168)
       {
           //this is some kind of local address, ignore it if we have a choice
           if (i_nics > 1) continue;
       }
        if (byte(lpHostEnt->h_addr_list[i][0]) == 169) if (byte(lpHostEnt->h_addr_list[i][1]) == 254)
       {
           //this is some kind of local address, ignore it if we have a choice
           if (i_nics > 1) continue;
       }
        if (byte(lpHostEnt->h_addr_list[i][0]) == 6) if (byte(lpHostEnt->h_addr_list[i][1]) == 0)
        {
            //this is some kind of local address, ignore it if we have a choice
            if (i_nics > 1) continue;
        }
        
        i_chosen_nic = i;
 
    }
    if (i_chosen_nic == -1)
    {
        LogMsg(_T("Unable to find an IP address."));
         strcpy(st_buff, "(no ip?)");
         return false;
    }


  //  log_msg("Found %d IP's, choosing #%d.", i_nics, i_chosen_nic);
    pinAddr = ((LPIN_ADDR)lpHostEnt->h_addr_list[i_chosen_nic]);
    
    
	strcpy(p_st_out_final, inet_ntoa(*pinAddr));
    return true;
}


