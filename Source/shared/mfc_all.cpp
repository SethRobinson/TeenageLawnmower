/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */

#include "uni.h"

#ifdef _UNICODE
#include "mfc_all_u.cpp"
#else
#include "mfc_all.h"


//this file assumes you have a "LogMsg" procedure somewhere, if you don't
//just copy this one into YOUR code:
/*
void LogMsg(LPCSTR lpFormat, ...)
{
	va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	CString cst_final;
	cst_final.Format("%s %s:%s",show_small_date(),show_time(),szBuf);
	OutputDebugString(cst_final);

//	g_dlg_main->Log(cst_final);
}
*/


//Don't send this a pointer a char array smaller than 256 or prepare for the worst.


//changes a number to be in a range of required.  If it is, it returns true.
bool force_range(int * i_original, int i_min, int i_max, CString cst_context)
{
 if (in_range(*i_original, i_min, i_max)) return false;
#ifdef _DEBUG
 LogMsg("force_range activated - var %s was %d.  Put in range.",cst_context,*i_original);
#endif 
 if (*i_original < i_min) *i_original = i_min;
 if (*i_original > i_max) *i_original = i_max;

 return false; //changed number

}

bool fforce_range(float * i_original, float i_min, float i_max, CString cst_context)
{
 if (in_range_float(*i_original, i_min, i_max)) return false;
#ifdef _DEBUG
 LogMsg("force_range activated - var %s was %d.  Put in range.",cst_context,int(*i_original));
#endif 
 if (*i_original < i_min) *i_original = i_min;
 if (*i_original > i_max) *i_original = i_max;

 return false; //changed number

}


bool separate_string_mfc (CString cst_input, int num, char liney, CString *cst_return) 
{
	//it's an old routine, I don't want to rewrite, I just wanted to use
	//cstring passing
	char return1[800];
	char str[1000];
	if (cst_input.GetLength() > 999) 
	{
	 LogMsg("Avoided a crash - separate_string_cst got a too long line");
	 return false;
	}
	strcpy(str,cst_input); 
	
	int l;
	unsigned int k;
	
	l = 1;
	strcpy(return1 ,"");
	
	for (k = 0; k <= strlen(str); k++)
	{
		
		if (str[k] == liney)
		{
			l++;
			if (l == num+1)
				goto done;
			
			if (k < strlen(str)) strcpy(return1,"");
		}
		if (str[k] != liney)
			sprintf(return1, "%s%c",return1 ,str[k]);
	}
	if (l < num)  strcpy(return1,"");
	
	replace("\n","",return1); //Take the /n off it.
	cst_return->Format("%s", return1);
	
	return(false);
	
done:
	
	if (l < num)  strcpy(return1,"");
	
	replace("\n","",return1); //Take the /n off it.
	cst_return->Format("%s", return1);
	//Msg("Took %s and turned it to %s.",str, return1);
	return(true);
}



//these are used to launch a url.  call like this:

//  winall_create_url_file("www.rtsoft.com/smartmute/chatwall");
//  int result = (int)ShellExecute(NULL,"open","temp.url", NULL,NULL, SW_MAXIMIZE);
//  ProcessError(this->m_hWnd,result, "smcw.url","www.rtsoft.com/smartmute/chatwall"); //show an error message if we couldn't open this
  
   CString load_txt_file_dialog(CString cst_default)
{
/*
       char command_line[400];
	char my_path[400];

	//strcpy(command_line, GetCommandLine());
	getdir(command_line, my_path);
  */
       
	 CFileDialog cfd(true, //true if open, false if close
		 ".TXT", //default file extension
		 cst_default, OFN_HIDEREADONLY,
		 "Text files (*.txt)|*.txt|All files (*.*)|*.*|",//file filter
		 AfxGetApp()->m_pMainWnd);

		 if(cfd.DoModal() == IDOK)
		 {
			return cfd.GetPathName();
		 } else

    return "";
}



  CString save_txt_file_dialog(CString cst_default)
{
/*
      char command_line[400];
	char my_path[400];

	//strcpy(command_line, GetCommandLine());
	getdir(command_line, my_path);
  */
      
	 CFileDialog cfd(false, //true if open, false if close
		 ".TXT", //default file extension
		 cst_default, OFN_HIDEREADONLY,
		 "Text files (*.txt)|*.txt|All files (*.*)|*.*|",//file filter
		 AfxGetApp()->m_pMainWnd);

		 if(cfd.DoModal() == IDOK)
		 {
			return cfd.GetPathName();
		 } else

    return "";
}

 FILE * open_file_cst(CString file)
{

	return(fopen( file, "r" ));

}

//get next line from a stream (opened with above)

bool get_line_from_file_cst(FILE * fp, CString * dest)
{
    char buff[256];
	buff[0] = 0; 	
   if( fgets( buff, 255, fp ) == NULL)
	{
		dest->Format("");
		return(false);
	}
			

	dest->Format("%s",buff);
	return true;
}

/* does file exist? */
bool exist_cst(CString name)
{
	FILE *fp;
	fp = fopen(name, "rb");
	if (!fp)
	{
		//	  fclose(fp);
		return(false);
	}
	
	fclose(fp);
	return(true);
}


bool load_file_into_cstring(CString cst_file, CString *cst_text)
{

  CString cst_temp;
//does the file exist?
  if (!exist_cst(cst_file)) return false;

  FILE *fp = open_file_cst(cst_file);

  if (!fp)
  {
	  LogError("Error opening file to load into string.");
	  return false;
  }

  while (get_line_from_file_cst(fp, &cst_temp))
  {
    *cst_text += cst_temp;
  }

  fclose(fp);

  return true;

}

#endif
 


double get_time_difference_in_text(CString *cst_temp, time_t t_last_packet)
{
  	static time_t t_now;
	time(&t_now);
	static double dbl_elapsed_seconds;

	dbl_elapsed_seconds = difftime(t_now, t_last_packet);
	if (dbl_elapsed_seconds < 1) dbl_elapsed_seconds = 1; //avoid devide by zero, JIC.
	
	if (dbl_elapsed_seconds >= 60*60*24)
	cst_temp->Format(_T("%6.0f Day"), dbl_elapsed_seconds / (60*60*24));
     else    
	if (dbl_elapsed_seconds >= 60*60)
	cst_temp->Format(_T("%6.0f Hr"), dbl_elapsed_seconds / (60*60));
     else    
    if (dbl_elapsed_seconds >= 60) 
	cst_temp->Format(_T("%6.0f Min"), dbl_elapsed_seconds / 60);
	 else
	cst_temp->Format(_T("%6.0f Sec"), dbl_elapsed_seconds);

	  return dbl_elapsed_seconds;
}	


double get_time_difference_in_text_long(CString *cst_temp, time_t t_last_packet)
{
  	static time_t t_now;
	time(&t_now);
	static double dbl_elapsed_seconds;

	dbl_elapsed_seconds = difftime(t_now, t_last_packet);
	if (dbl_elapsed_seconds < 1) dbl_elapsed_seconds = 1; //avoid devide by zero, JIC.
	
	if (dbl_elapsed_seconds >= 60*60*24)
	cst_temp->Format(_T("%.0f days"), dbl_elapsed_seconds / (60*60*24));
     else    
	if (dbl_elapsed_seconds >= 60*60)
	cst_temp->Format(_T("%.0f hours"), dbl_elapsed_seconds / (60*60));
     else    
    if (dbl_elapsed_seconds >= 60) 
	cst_temp->Format(_T("%.0f minutes"), dbl_elapsed_seconds / 60);
	 else
	cst_temp->Format(_T("%.0f seconds"), dbl_elapsed_seconds);

	  return dbl_elapsed_seconds;
}	

CString int_to_cash(int i_amount)
{
	CString sAmount;
	sAmount.Format(_T("%d"), i_amount);
	if(sAmount.IsEmpty()) return "Error!";
	CString sAmount2;
	CString sComma(_T(","));
	int nDotPos = sAmount.Find('.');
	if(nDotPos == -1) sAmount += _T(".00");
	else 
	{	
		int nLenAfterDot = nDotPos + 3;	
		sAmount = sAmount.Left(nLenAfterDot);
		if(sAmount.GetLength() < nLenAfterDot) sAmount += _T("0");
	}
	
	int nAmtLen = sAmount.GetLength();
	nDotPos = sAmount.Find('.');
	int nNumThouSep = nDotPos/3;
	int nStep = nDotPos%3;
	if(nStep) sAmount2 = sAmount.Left(nStep) + sComma;
	for(int i = nStep; i < nDotPos; i+= 3)
	{
		sAmount2 += sAmount.Mid(i,3) + sComma;
	}
	
	sAmount = sAmount2.Left(sAmount2.GetLength()-1);
	//+ sAmount.Right(3);
	//uh, actually why don't remove the cents part.. 
  	//sAmount = sAmount.Left(3);

	return sAmount;
}


CString ulong_to_cash(unsigned long i_amount)
{
	CString sAmount;
	sAmount.Format(_T("%d"), i_amount);
	if(sAmount.IsEmpty()) return "Error!";
	CString sAmount2;
	CString sComma(_T(","));
	int nDotPos = sAmount.Find('.');
	if(nDotPos == -1) sAmount += _T(".00");
	else 
	{	
		int nLenAfterDot = nDotPos + 3;	
		sAmount = sAmount.Left(nLenAfterDot);
		if(sAmount.GetLength() < nLenAfterDot) sAmount += _T("0");
	}
	
	int nAmtLen = sAmount.GetLength();
	nDotPos = sAmount.Find('.');
	int nNumThouSep = nDotPos/3;
	int nStep = nDotPos%3;
	if(nStep) sAmount2 = sAmount.Left(nStep) + sComma;
	for(int i = nStep; i < nDotPos; i+= 3)
	{
		sAmount2 += sAmount.Mid(i,3) + sComma;
	}
	
	sAmount = sAmount2.Left(sAmount2.GetLength()-1);
	//+ sAmount.Right(3);
	//uh, actually why don't remove the cents part.. 
  	//sAmount = sAmount.Left(3);

	return sAmount;
}


CString float_to_cash(float i_amount)
{
	CString sAmount;
	sAmount.Format(_T("%f"), i_amount);
	if(sAmount.IsEmpty()) return "Error!";
	CString sAmount2;
	CString sComma(_T(","));
	int nDotPos = sAmount.Find('.');
	if(nDotPos == -1) sAmount += _T(".00");
	else 
	{	
		int nLenAfterDot = nDotPos + 3;	
		sAmount = sAmount.Left(nLenAfterDot);
		if(sAmount.GetLength() < nLenAfterDot) sAmount += _T("0");
	}
	
	int nAmtLen = sAmount.GetLength();
	nDotPos = sAmount.Find('.');
	int nNumThouSep = nDotPos/3;
	int nStep = nDotPos%3;
	if(nStep) sAmount2 = sAmount.Left(nStep) + sComma;
	for(int i = nStep; i < nDotPos; i+= 3)
	{
		sAmount2 += sAmount.Mid(i,3) + sComma;
	}
	
	sAmount = sAmount2.Left(sAmount2.GetLength()-1) + sAmount.Right(3);
  return sAmount;
}


//FOR unicode and normal

//set's a combo box's selection by matching it's data, not id
int set_selection_by_data(CComboBox * m_list, int i_sel)
{
	int i_count = m_list->GetCount();

	for (int i=0; i < i_count; i++)
	{
	  if (m_list->GetItemData(i) == DWORD(i_sel))
	  {
	    //found it
		  m_list->SetCurSel(i);
		  return i;
	  }

	}
  //error can't find anything with that data
	return -1;

}

int set_selection_by_data(CListBox * m_list, int i_sel)
{
	int i_count = m_list->GetCount();

	for (int i=0; i < i_count; i++)
	{
	  if (m_list->GetItemData(i) == DWORD(i_sel))
	  {
	    //found it
		  m_list->SetCurSel(i);
		  return i;
	  }

	}
  //error can't find anything with that data
	return -1;

}

 int set_selection_by_data(CListCtrl * m_list, int i_sel)
{
	int i_count = m_list->GetItemCount();

	for (int i=0; i < i_count; i++)
	{
	  if (m_list->GetItemData(i) == DWORD(i_sel))
	  {
	    //found it
		 // m_list->SetSelectionMark(i);
	      m_list->SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED |
        LVIS_FOCUSED);

          return i;
	  }

	}
  //error can't find anything with that data
	return -1;

}



bool AddText(CString cst_message, CString cst_filename)
{

   FILE *          fp = NULL;
     if (exist( (LPCTSTR) cst_filename) == false)
    {
        
        fp = _tfopen(cst_filename, _T("wb"));
        if (!fp)
        {
            LogError(_T("Can't open %s to write"),cst_filename);
            return false;
        }
        
        //let's mark it so text editors can understand that it is unicode
        CHAR *p_st_unicode = "ÿþ";

        fwrite(p_st_unicode, 2, 1, fp);
        
#ifdef _UNICODE
        fwrite( cst_message, wcslen(cst_message) * sizeof(WCHAR), 1, fp);       /* current player */
#else
        
        fwrite( uni(cst_message).us_data, _tcslen(cst_message) * sizeof(WCHAR), 1, fp);       /* current player */
#endif
        
        fclose(fp);
        return true;
    } else
    {
        fp = _tfopen(cst_filename, _T("ab"));
  #ifdef _UNICODE
      fwrite( cst_message, wcslen(cst_message)* sizeof(WCHAR), 1, fp);       /* current player */
 #else
            fwrite( uni(cst_message).us_data, _tcslen(cst_message)* sizeof(WCHAR), 1, fp);       /* current player */
  #endif

      fclose(fp);
  }



    return true;
}


