#include "CRegistry.h"

CRegistry::CRegistry(char st_reg_key_new[], HKEY access)
{
   m_hkey_access = access;
  strcpy((char*)&st_reg_key, st_reg_key_new);

}	 


bool CRegistry::read_key_string(char st_key[], char *p_st_key)
{
   	HKEY key;
	long l_result = RegOpenKeyEx(m_hkey_access, st_reg_key, 0, KEY_READ, &key);

	if (l_result != ERROR_SUCCESS)
	{
		//bad path?
		return false;
	}
	
	//step two, read the key
	DWORD d_size = 255;

	l_result = RegQueryValueEx(key,  st_key, NULL, NULL, (unsigned char*)p_st_key,  &d_size);
	if (l_result != ERROR_SUCCESS)
	{
		//key doesn't exist?
	    RegCloseKey(key);
        return false;
	}
	
    
	    RegCloseKey(key);
    
    return true;
}


bool CRegistry::CreateKey(char st_key[])
{

    HKEY key;
    DWORD dw_status;
    LONG result = RegCreateKeyEx(m_hkey_access, st_key, 0,0, REG_OPTION_NON_VOLATILE, KEY_WRITE,
        NULL, &key, &dw_status);

    if (result == ERROR_SUCCESS)
    {
        RegCloseKey(key);
        return true;
    }   else
        return false;

}

bool CRegistry::WriteDWORD(char st_key[], DWORD *p_dw_key)
{
   	HKEY key;
	long l_result = RegOpenKeyEx(m_hkey_access, st_reg_key, 0, KEY_WRITE, &key);

	if (l_result != ERROR_SUCCESS)
	{
		//bad path?
		return false;
	}
	
	//step two, read the key
	DWORD d_size = sizeof(DWORD);

	l_result = RegSetValueEx(key,  st_key, NULL, REG_DWORD, (unsigned char*)p_dw_key,  d_size);
	if (l_result != ERROR_SUCCESS)
	{
		//key doesn't exist?
	    RegCloseKey(key);
        return false;
	}
	
    
	    RegCloseKey(key);
    
    return true;

}


bool CRegistry::read_key_dword(char st_key[], DWORD *p_dw_key)
{
   	HKEY key;
	long l_result = RegOpenKeyEx(m_hkey_access, st_reg_key, 0, KEY_READ, &key);

	if (l_result != ERROR_SUCCESS)
	{
		//bad path?
		return false;
	}
	
	//step two, read the key
	DWORD d_size = sizeof(DWORD);

	l_result = RegQueryValueEx(key,  st_key, NULL, NULL, (unsigned char*)p_dw_key,  &d_size);
	if (l_result != ERROR_SUCCESS)
	{
		//key doesn't exist?
	    RegCloseKey(key);
        return false;
	}
	
    
	    RegCloseKey(key);
    
    return true;
}