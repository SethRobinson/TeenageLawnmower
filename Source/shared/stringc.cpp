// stringc.cpp

//******************************************
// copyright 1998	Mickey Kawick
// This file may be used anywhere for free, but this
// comment info must be left here.

// If you use this code in any shape or form or even
// as a reference for profit send me a dollar. 
// Not a dollar per copy, just a single US dollar. 

// If you are not in the US, send me
// a few different coins or the equivalent of a dollar.
// I love foreign currency, and I have a little collection.
// MKawick@sprintmail.com
//******************************************

//---------------------------------------------------------------------------

#if defined (_MSC_VER)  
#define  strlwr  _strlwr
#define  strupr  _strupr
#endif

//#ifndef _CONSOLE
//#if defined __AFX_H_
//#include "stdafx.h"
//#endif
//#endif

#include <string.h>
#ifdef _DEBUG
#include <stdlib.h>
#include <crtdbg.h>
#endif
//#include "c:/library/stringc.h"  //SETH: what the hell is this?

//---------------------------------------------------------------------------

//#endif	
			//*********************************
			//*********************************

STRINGC:: STRINGC ()
{
	Clear ();
}
				

			//*********************************

STRINGC:: STRINGC (const char* s)
{
    Clear ();
	int len = strlen (s);
	if (len>=TEXTLEN) len=TEXTLEN-1;
	memcpy (str, s , len);
	str[len] = 0;
}

			//*********************************

STRINGC:: STRINGC (const char* s, STRINGC* ptr)// init and store list following
{
    Clear ();
	next = ptr;
	int len = strlen (s);
	if (len>=TEXTLEN) len=TEXTLEN-1;
	memcpy (str, s , len);
	str[len] = 0;
}


			//*********************************

STRINGC:: STRINGC (const int s)
{
	char ptr[15]; //limited to 15 digits
    Clear ();
	itoa (s, ptr, 10);
	strcpy (str, ptr);
}

			//*********************************

STRINGC:: STRINGC (const unsigned long s)
{
	char ptr[15]; //limited to 15 digits
    Clear ();
	ultoa (s, ptr, 10);
	strcpy (str, ptr);
}
			//*********************************

STRINGC:: STRINGC (const float s)
{
    Clear ();
	char ptr[25]; //limited to 15 digits
	char * temp = gcvt ((double) s, 15, ptr);
	int len = strlen (temp);
	if (temp[len-1] == '.') strcat (temp, "0");

	char* last = PointToLastCharacter (str, '.');// keep it short
	if (len - (last-str) >8) last[8] = 0;
	strcpy (str, temp);
}

			//*********************************

STRINGC:: STRINGC (const STRINGC* s)
{
    Clear ();
	strcpy (str, s->str);
}

			//*********************************

STRINGC:: STRINGC (const STRINGC & s)
{
	Clear ();
	strcpy (str, s.str);
}

			//*********************************
/*
STRINGC:: STRINGC (STRINGC s)
{
	Clear();
	strcpy (str, s.str);
}
*/
			//*********************************

STRINGC:: STRINGC (const STRINGC * s, const int num)// number of characters to copy
{
	Clear ();
	int lnum = num;
	if (lnum >= TEXTLEN) lnum = TEXTLEN;
	if (s->Len () < lnum) lnum = s->Len ();
	memcpy (str, s->str, lnum);
	str[lnum] = 0;
}

			//*********************************

STRINGC:: STRINGC (const STRINGC & s, const int num)
{
	Clear ();
	int lnum = num;
	if (lnum >= TEXTLEN) lnum = TEXTLEN;
	if (s.Len () < lnum) lnum = s.Len ();
	memcpy (str, s.str, lnum);
	str[lnum] = 0;
}

			//*********************************

STRINGC:: STRINGC (const char* s, const int num)
{
	Clear ();
	int lnum = num;
	if (lnum >= TEXTLEN) lnum = TEXTLEN;
	int len = strlen (s);
	if (len < lnum) lnum = len;
	memcpy (str, s, lnum);
	str[lnum] = 0;
}

			//*********************************
/*
STRINGC:: STRINGC (STRINGC s, int num)
{
	Clear();
	if (num >= TEXTLEN) num = TEXTLEN;
	if (s.Len () < num) num = s.Len ();
	strcpy (str, s.str);
}
*/
			//*********************************

STRINGC:: ~STRINGC ()
{
    if (next) delete next;
}

			//*********************************
			//*********************************
//inline
void STRINGC::  Clear ()
{
	str[0] = NULL;
    next = NULL;
}

			//*********************************

void STRINGC::  Zero ()
{
	memset (str, 0, TEXTLEN);
    next = NULL;
}		
			//*********************************
			//*********************************
/*inline
char * STRINGC:: GetString ()
{
    if (str[0] == 0) return NULL;
	return str;
} */
		
int   STRINGC:: GetStringLength () const
{
	if (str[0] == 0) return 0;
	return strlen (str);
}

			//*********************************

int   STRINGC:: Len () const
{
	return GetStringLength ();
}

			//*********************************
			//*********************************

char* STRINGC:: PointToLastCharacter (char* str, char search)// points to last of 'search'
{
	char * original = str, * last = str;

	while (1)// continue until the last
	{
		str = PointToNextCharacter (last, search);
		if (last == str) break;
		last = str;
	}
	if (original == str) return original;// this character did not exist in this string
	return str;
}
		
			//*********************************

char* STRINGC:: PointToNextCharacter (char* str, char search)// returns same if no more exist
{
	if (*(str+1)==0) return str;

	char * original = str;
	if (*str == search) str++;
	

	while (*str != 0)
	{
		if (*str == search) return str;
		str++;
	}
	return original;
}
		
			//*********************************
		
char* STRINGC:: GetPositionFromEnd (int pos)
{
	int x = strlen (str);
	if (pos >= x) return str;
	char * ptr = GotoEnd ();
	ptr -= pos-1;// because we are already pointing to the last char
	return ptr;
}
		
			//*********************************

char* STRINGC:: GotoEnd ()
{
	if ((str == 0) || (*str == 0)) return NULL;
	char * ptr, * lptr;
	ptr = lptr = str;

	ptr++;// advance the pointer to the next for the start. If this falls out of the
	// loop, it will return normally
	while (*ptr != 0)
	{
		ptr++; lptr++;
	}
	return lptr;// return 1 behind the test object
}
		
			//*********************************

			//*********************************
			//*********************************

void  STRINGC:: ConvertAllType1ToType2 (char * str, char type1, char type2)// conversion		
{
	while (*str != 0)
	{
		if (*str == type1) *str = type2; 
		str++;
	}
}
			//********************************* file handling ***********************
			//********************************* ************* ***********************

STRINGC  STRINGC:: ExtractFileName () // returns a chunk of memory to be deleted
{
	EliminateExtension () ;
	char * file = PointToLastCharacter (str, '/');
	if (file == str) return STRINGC ();

	char * path = new char[32];
	file++; // advance to the actual filename
	
	strcpy (path, file);
	strcpy (str, path);
	return STRINGC(path);
}

			//*********************************

STRINGC STRINGC:: ExtractExtension () // returns a chunk of memory to be deleted
{
	char * ext = PointToLastCharacter (str, '.');
	if (ext == str) return STRINGC();
	ext++;

	char * ret = new char [6];
	strcpy (ret, ext);
	strcpy (str, ret);
	return STRINGC (ret);
}
		
			//*********************************

STRINGC STRINGC:: ExtractPath () 
{
	char path [TEXTLEN];
	strcpy (path, str);

	char * file = PointToLastCharacter (path, '/');	
	if (file == path) {str[0] = 0; return STRINGC ();}// path is the name so no path
	*file = 0;
	strcpy (str, path);
	return  STRINGC (path);
}

			//*********************************

void  STRINGC:: EliminateExtension () // eliminates '.'
{
	char * file = PointToLastCharacter (str, '.');
	if (file == str) return;
	*file = 0;
}

			//*********************************

void  STRINGC:: EliminateFilename () // replaces last '/' with 0
{
	char * file = PointToLastCharacter (str, '/');
	if (file == str) return;
	if (file == 0) return;
	*file = 0;
}

			//*********************************

bool STRINGC:: HasExtension ()
{
    char * file = PointToLastCharacter (str, '.');
    if (file == str) return false;
    return true;
}

			//*********************************
			//*********************************

void  STRINGC:: Lower ()
{
	if (str[0] == 0) return;
	char * temp = strlwr ((char *)str);
	strcpy (str, temp);
}

void  STRINGC:: ConvertToLowerCase ()
{
	char * st = str;
	while (*st != 0)
	{
		char val = *st;

		if ((val >= 'A') && (val <='Z')) 
			*st = (char)(val | 32);// lower case

		st++;
	}
}

			//*********************************

void  STRINGC:: Upper()
{
	if (str[0] == 0) return;
	char * temp = strupr ((char *)str);
	strcpy (str, temp);
}

void  STRINGC:: ConvertToUpperCase ()
{
	char * st = str;
	while (*st != 0)
	{
		char val = *st;

		if ((val >= 'a') && (val <='z')) 
			*st = (char)(val & 223);// lower case

		st++;
	}
}

			//*********************************
			//*********************************

void  STRINGC:: ConvertAllBackslashesToForwardslashes ()
{
	ConvertAllType1ToType2 (str, '\\', '/');
}
void  STRINGC:: ConvertBackslash ()
{
	ConvertAllType1ToType2 (str, '\\', '/');
}


			//********************************* Operators  ************************
			//********************************* *********  ************************

STRINGC  STRINGC:: operator = (const int x)//assignment
{
	char ptr[15]; 
	itoa (x, ptr, 10);
	strcpy (str, ptr);
	return STRINGC (ptr);
}
	
			//*********************************

STRINGC  STRINGC:: operator = (const unsigned long s)
{
	char ptr[15]; //limited to 15 digits
	ultoa (s, ptr, 10);
	strcpy (str, ptr);
	return STRINGC (ptr);
}

			//*********************************

STRINGC  STRINGC:: operator = (const char* x)
{
	if (x == NULL) return STRINGC ();
	int len = strlen (x);
	if (len>=TEXTLEN) len=TEXTLEN-1;
	memcpy (str, x , len);
	str[len] = 0;
	return STRINGC (this);
}
	
			//*********************************

STRINGC  STRINGC:: operator = (const float x)
{
	char ptr[25]; 
	char * temp = gcvt ((double) x, 15, ptr);
	int len = strlen (temp);
	if (temp[len-1] == '.') strcat (temp, "0");
	strcpy (str, temp);
	
	char* last = PointToLastCharacter (str, '.');// keep it short
	if (len - (last-str) >8) last[8] = 0;
	return STRINGC (temp);
}

			//*********************************

STRINGC  STRINGC:: operator = (const STRINGC * x)
{
	if (x == NULL) return STRINGC ();
	strcpy (str, x->str);
	//next = x->next; // do not copy pointer
	return STRINGC (str);
}

			//*********************************

STRINGC  STRINGC:: operator =  (const STRINGC & x)
{
	if (x.str == NULL) return STRINGC ();
	strcpy (str, x.str);
	//next = x.next;   // do not copy pointer
	return STRINGC (str);
}

			//*********************************
			//*********************************
// the following 4 functions are for assignment of a STRINGC
// class to a char*, int, float, or unsigned long
/*
example:

  STRINGC	str (10);
  int x = str;
  // x now equals 10.
  char* ptr = str;
  // ptr now equals "10"
  unsigned long t = str;
  // t now equals 10
  float f = str;
  // f now equals 10.0F;
*/

STRINGC:: operator char* ()
{
	 if (str[0] == 0) return NULL;
	//char * ptr = new char[MAXLEN];
	//strcpy (ptr, str);
	return str;
}
		
			//*********************************

STRINGC:: operator int ()
{
	int x = atoi (str);
	return x;
}

			//*********************************

STRINGC:: operator float ()
{
	float f = (float) atof (str);
	return f;
}

			//*********************************

STRINGC:: operator unsigned long ()
{
	char * end = str+Len();
	long x = strtoul (str, &end, 10);
		//atol (str);
	return x;
}

			//*********************************
			//*********************************

bool  STRINGC:: operator == (const char* x)//comparison
{
	if (x == NULL) return false;
	int val = (stricmp (str, x));
	if (val == 0) return true;
	return false;
}
			//*********************************

bool  STRINGC:: operator != (const char* x)//comparison
{
	if (x == NULL) return true;
	int val = (stricmp (str, x));
	if (val == 0) return false;
	return true;
}

			//*********************************

bool  STRINGC:: operator < (const char* x)
{
	if (x == NULL) return false;
	int val = (stricmp (str, x));
	if (val < 0) return true;
	return false;
}

			//*********************************

bool  STRINGC:: operator > (const char* x)
{
	if (x == NULL) return false;
	int val = (stricmp (str, x));
	if (val > 0) return true;
	return false;
}


			//*********************************
			//*********************************

bool  STRINGC:: operator == (const STRINGC * x)//comparison
{
	if (x == NULL) return false;
	int val = (stricmp (str, x->str));
	if (val == 0) return true;
	return false;
}

			//*********************************

bool  STRINGC:: operator != (const STRINGC * x)//comparison
{
	if (x == NULL) return true;
	int val = (stricmp (str, x->str));
	if (val == 0) return false;
	return true;
}

			//*********************************

bool  STRINGC:: operator < (const STRINGC * x)
{
	if (x == NULL) return false;
	int val = (stricmp (str, x->str));
	if (val < 0) return true;
	return false;
}
			//*********************************

bool  STRINGC:: operator > (const STRINGC * x)
{
	if (x == NULL) return false;
	int val = (stricmp (str, x->str));
	if (val > 0) return true;
	return false;
}

			//*********************************

bool  STRINGC:: operator == (const STRINGC & x)//comparison
{
	int val = (stricmp (str, x.str));
	if (val == 0) return true;
	return false;
}

			//*********************************

bool  STRINGC:: operator != (const STRINGC & x)
{
	int val = (stricmp (str, x.str));
	if (val == 0) return false;
	return true;
}

			//*********************************

bool  STRINGC:: operator < (const STRINGC & x)
{
	int val = (stricmp (str, x.str));
	if (val < 0) return true;
	return false;
}
			//*********************************

bool  STRINGC:: operator > (const STRINGC & x)
{
	int val = (stricmp (str, x.str));
	if (val > 0) return true;
	return false;
}

			//*********************************
			//*********************************

STRINGC& STRINGC:: ToDec ()
{
	char newchar[TEXTLEN], **end;
	strcpy (newchar, "0x");
	strcat (newchar, str);
	int len = strlen (newchar);
	char* e = &newchar[len+1];
	end = &e;
	long  val = strtol (newchar, end, 16);
	ltoa (val, str, 10);
	return (*this);
}

			//*********************************

STRINGC& STRINGC:: ToHex ()
{
	long  val = atol (str);
	ltoa (val, str, 16);
	return (*this);
}

			//*********************************
			//*********************************

STRINGC& STRINGC:: operator += (const int x) // concatenate
{
	char ptr[15]; 
	itoa (x, ptr, 10);
	strcat (str, ptr);
	return (*this);
}

			//*********************************

STRINGC& STRINGC:: operator += (const unsigned long   s)
{
	char ptr[15]; //limited to 15 digits
	ultoa (s, ptr, 10);
	strcat (str, ptr);
	return (*this);
}
	
			//*********************************

STRINGC& STRINGC:: operator += (const char* x)
{
	if (x == NULL) return (*this);
	int currlen = Len ();
	if (currlen < TEXTLEN)
	{
		int len = strlen(x);
		if (len>=TEXTLEN) len=TEXTLEN-1;
		if (len+currlen >=TEXTLEN) len = TEXTLEN-currlen-1;
		strcat (str, x);
	}
	return (*this);
}
	
			//*********************************

STRINGC& STRINGC:: operator += (const float x)
{
	char ptr[25];
	char * temp = gcvt((double) x, 15, ptr);
	int len = strlen (temp);
	if (temp[len-1] == '.') strcat (temp, "0");
	strcat (str, temp);

	char* last = PointToLastCharacter (str, '.');// keep it short
	if (len - (last-str) >8) last[8] = 0;
	return (*this);
}

			//*********************************
            
STRINGC& STRINGC:: operator += (const STRINGC * x)
{
    if (x == NULL) return (*this);
	strcat (str, x->str);
	return (*this);
}

			//*********************************

STRINGC& STRINGC:: operator += (const STRINGC & x)
{
	if (x.str[0] == NULL) return (*this);
	strcat (str, x.str);
	return (*this);
}

			//*********************************
			//*********************************

STRINGC  STRINGC:: operator + (const int x) const
{
	return (STRINGC (*this)) += x;
}

			//*********************************

STRINGC  STRINGC:: operator + (const unsigned long x) const
{
	return (STRINGC (*this)) += x;
}	

			//*********************************

STRINGC  STRINGC:: operator + (const char* x) const
{
	return (STRINGC (*this)) += x;
}
	
			//*********************************

STRINGC  STRINGC:: operator + (const float x) const
{
	return (STRINGC (*this)+= x);
}
	
			//*********************************

STRINGC  STRINGC:: operator + (const STRINGC * st) const
{
	return (STRINGC (*this)+= st);
}

			//*********************************

STRINGC  STRINGC:: operator + (const STRINGC & st) const
{
	return (STRINGC (*this)+= st);
}
	
			//*********************************
			//*********************************

STRINGC  STRINGC:: operator << (const int x)// move the front 'x' characters off
{
	if (x<0) return STRINGC ();
	if (x > (int)strlen(str)) {str[0] = 0; return STRINGC ();}
	char * ptr = str+x, *pt = str;

	while (*ptr)
	{
		*pt++ = *ptr++;
	}
	*pt = 0;
	return STRINGC (str);
}
	
			//*********************************
// does not check for overages
STRINGC  STRINGC:: operator >> (const int x)// add 'x' number of blank spaces  
{
	char ptr[TEXTLEN], * ptr2 = ptr+x;
	memset (ptr, 32, x);// put in spaces as a leader
	strcpy (ptr2, str);
	strcpy (str, ptr);
	return STRINGC (ptr2);
}

			//*********************************

char	 STRINGC:: operator [] (const int x) const
{
	if (x<0 || x>=TEXTLEN) return 0;
	return str[x];
}

			//*********************************

void	 STRINGC:: Set (const char c, const int which)// set a value
{	
	if (which<0 || which>=TEXTLEN) return;
	str[which] = c;
}

			//*********************************

void	STRINGC:: Copy (const char* c, const int num)// copy a range
{
	int n = num;
	if (n>=TEXTLEN) n = TEXTLEN-1;
	memcpy (str, c, n);
	str[n] = 0;
}


			//*********************************
			//*********************************

char* STRINGC:: ConvertFloatStringToRealString (char* temp, int decimal, int sign)
{
	char* st = new char[TEXTLEN], *ptr = st;
	if (sign)
	{
		*ptr++ = '-';
	}
	if (decimal == 0)
	{
		*ptr++ = '0';
		*ptr++ = '.'; 
	}
	if (decimal < 0)
	{
		*ptr++ = '.';
		while (decimal ++)
		{
			*ptr++ = '0';
		}		
		decimal = 0;
	}
	if (decimal > 0)
	{
		while (decimal --)
		{
			*ptr++ = *temp++;
		}
		//decimal = 0;
		*ptr++ = '.';
	}
	
	while (*temp)
	{
		*ptr++ = *temp++;
	}
	*ptr = 0;
	return st;
}
	
			//*********************************
			//*********************************

void   STRINGC:: AllignLeft ()
{
	char * st = str, *ptr = str;
	while (*st == ' ') st++;
	while (*ptr) *st++ = *ptr++;
	*st = 0;
}
	
			//*********************************

void   STRINGC:: AllignRight (int BufferSize)
{
	int x = strlen (str);
	int off = BufferSize-x;
	if (off <=0) return;

	char ptr[TEXTLEN], * ptr2 = ptr+off;
	memset (ptr, 32, off);
	strcpy (ptr2, str);
	strcpy (str, ptr);
}
	
			//*********************************

void   STRINGC::  AllignCenter (int BufferSize)
{
	int x = strlen (str);
	int off = (BufferSize-x)/2;
	if (off <=0) return;

	char ptr[TEXTLEN], * ptr2 = ptr+off;
	memset (ptr, 32, off);
	strcpy (ptr2, str);
	strcpy (str, ptr);
}

STRINGC	 STRINGC::  LeftStr (int num)
{
	char ptr[TEXTLEN];
	if (num >= TEXTLEN) num = TEXTLEN-1;
	memcpy (ptr, str, num);
	ptr[num] = 0;

	return STRINGC (ptr);
}
	
			//*********************************

STRINGC	 STRINGC::  RightStr (int num)
{
	char ptr[TEXTLEN], *pt;
	int len = strlen (str);
	if (len == 0) return STRINGC ();
	if (num >= TEXTLEN) num = TEXTLEN-1;
	pt = GetPositionFromEnd (num);
	memcpy (ptr, pt, num);

	ptr[num] = 0;
	return STRINGC (ptr);
}
	
			//*********************************

STRINGC	 STRINGC::  MidStr (int left, int right)
{
	char ptr[TEXTLEN];
	int len = strlen (str);
	if ((left < 0) || (right < 0)) return STRINGC ();
	if ((left >= len) || (right >= len)) return STRINGC ();
	if (right<= left) return STRINGC ();

	int num = right - left;
	char * pt = str+left;
	memset (ptr, 0, TEXTLEN);
	memcpy (ptr, pt, num);
	ptr[num] = 0;

	return STRINGC (ptr);
}
	
			//*********************************

void STRINGC:: Erase (char type)// replace all values of this type with 0
{
	int len = Len ();
	for (int i=0; i<len; i++)
		if (str[i] == type) str[i] = 0;
}

			//*********************************

void STRINGC:: Replace (char item, char with)//replace all 
{
	int len = Len ();
	for (int i=0; i<len; i++)
		if (str[i] == item) str[i] = with;
}
	// instances of "item" with "with"
	
			//*********************************
			//*********************************

void STRINGC:: AppendToList (STRINGC* item)
{
	if (item == 0L) return;
	if (next == 0L)
    {
		next = item;
        return;
    }

	STRINGC* ptr1 = this, *ptr2;

	while (ptr1)// go to the end
	{
		ptr2 = ptr1;
		ptr1 = ptr1->next;
	}	
	ptr2->next = item;
}

			//*********************************

void STRINGC:: AppendToList (STRINGC item)
{
	if (item.str == 0L) return;
	STRINGC* str = new STRINGC;
	*str = item;

	if (next == 0L)
    {
		next = str;
        return;
    }

	STRINGC* ptr1 = this, *ptr2;

	while (ptr1)// go to the end
	{
		ptr2 = ptr1;
		ptr1 = ptr1->next;
	}	
	ptr2->next = str;
}

			//*********************************

STRINGC* STRINGC:: PlaceInList (STRINGC* list)
{
        if (list == 0L) return this;
        if (list->next == 0L)
        {
           int x = strcmp ((char *) str, (char *)list->str);
           if (x > 0) list->next = this;
           else
           {
              next = list;
              list = this;
           }
           return list;
        }
        STRINGC* top = list;
        STRINGC* last = NULL;
        while (list)
        {
           int x = strcmp ((char *) str, (char *)list->str);
           if (x > 0) // greater than
           {
              last = list;
              list = list->next;
              continue;
           }
           if (x <= 0)// we insert here
           {
              if (last == NULL)// insert in the begining
              {
                 next = list;
                 top = this;
                 break;
              }
              next = last->next; //insert before the current top
              last->next = this;

              break;
           }
        }
        if (list == 0L) // this means we got to the end and we need to insert
        // this item at the end
        {
           last->next = this;
        }
        return top;
}
			//*********************************
			//*********************************
            
STRINGC* STRINGC:: GetNthItem (int which)
{
    if (which<0) return NULL;
    STRINGC* ptr = this;
    while (ptr != NULL && which-->0)
    {
        ptr = ptr->next;
    }
    return ptr;
}
	
			//*********************************
			//*********************************




void STRING_POINTER:: operator = (char* s)	
{
	ptr = s;
}
			//*********************************	

void STRING_POINTER:: operator + (char * s)	
{
	strcpy (ptr, s); ptr += strlen (s);
}
			//*********************************	

void STRING_POINTER:: operator + (int i)	
{
	*ptr++ = static_cast <char> (i);
}
			//*********************************	

void STRING_POINTER:: operator + (STRINGC&  s)
{
	strcpy (ptr, s); ptr += s.GetStringLength();
}

 			//*********************************
			//*********************************	

void STRING_POINTER:: operator += (int i)	
{
	*ptr++ = static_cast <char> (i);
}
			//*********************************	

void STRING_POINTER:: operator += (char* s)	
{
	strcpy (ptr, s); ptr += strlen (s);
}
 			//*********************************	

void STRING_POINTER:: operator += (STRINGC&  s)
{
	strcpy (ptr, s); ptr += s.GetStringLength();
}


			//*********************************
			//*********************************

