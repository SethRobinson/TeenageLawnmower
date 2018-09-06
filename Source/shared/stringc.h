// stringc.h

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

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifndef  __STRINGC_H_
#define  __STRINGC_H_

//typedef  int Bool;

#ifndef _MSC_VER 
#ifndef false
#define false 0
#define true 1
#endif
#endif

//---------------------------------------------------------------------------

//********************************************
//********************************************
//********************************************
// this class maintains strings, rips apart filenames, and has a linked list for storing
// lists of strings.
typedef class  STRINGC
{
	enum      {TEXTLEN = 120};
	char      str[TEXTLEN];
	STRINGC*  next;

		//************************

	char*		PointToLastCharacter (char* str, char search);
	char*		PointToNextCharacter (char* str, char search);	

	// returns string
	char*		ConvertFloatStringToRealString (char* temp, int decimal, int sign); 

	char*		GetPositionFromEnd (int pos);
	char*		GotoEnd (); // points to last character

public: 
		//************************

	STRINGC ();
 	STRINGC (const STRINGC * s);
	STRINGC (const STRINGC & s);
	STRINGC (const char* s, STRINGC* ptr);// for a sequence
	STRINGC (const char* s);
	STRINGC (const int   s);
	STRINGC (const unsigned long   s);
	STRINGC (const float s);
	STRINGC (const STRINGC* s, const int num);// number of characters to copy
	STRINGC (const STRINGC & s, const int num);
	//STRINGC (STRINGC s, int num);
	STRINGC (const char* s, const int num);


	~STRINGC ();

	void		Clear ();
	void		Zero ();// sets full string to 0
	char*		GetString () {return str;}
	int			GetStringLength () const;
	int			Len () const;
	bool		IsValid () const { if (str[0] == 0)return false; return true; }

		//************************

	STRINGC		operator =  (const int x);//assignment
	STRINGC		operator =  (const unsigned long s);
	STRINGC		operator =  (const char * x);
	STRINGC		operator =  (const float x);
	STRINGC		operator =  (const STRINGC * x);
	STRINGC		operator =  (const STRINGC & x);

	bool		operator == (const char* x);//comparison
	bool		operator != (const char* x);
	bool		operator <  (const char* x);
	bool		operator >  (const char* x);

	bool		operator == (const STRINGC * x);//comparison
	bool		operator != (const STRINGC * x);
	bool		operator <  (const STRINGC * x);
	bool		operator >  (const STRINGC * x);
	bool		operator == (const STRINGC & x);
	bool		operator != (const STRINGC & x);
	bool		operator <  (const STRINGC & x);
	bool		operator >  (const STRINGC & x);

	STRINGC&	operator += (const int x);// concatenate
	STRINGC&	operator += (const unsigned long x);
	STRINGC&	operator += (const char* x);
	STRINGC&	operator += (const float x);
	STRINGC&	operator += (const STRINGC * x);
	STRINGC&	operator += (const STRINGC & x);

	STRINGC		operator +  (const int x) const;
	STRINGC		operator +  (const unsigned long x) const;
	STRINGC		operator +  (const char* x) const;
	STRINGC		operator +  (const float x) const;
	STRINGC		operator +  (const STRINGC * x) const;
	STRINGC		operator +  (const STRINGC & x) const;

	STRINGC		operator << (const int x);// move the front 'x' characters off
	STRINGC		operator >> (const int x);// add 'x' number of blank spaces

	char		operator [] (const int x) const;

	void		Set (const char c, const int which);// set a value
	void		Copy (const char* c, const int num);// set a value

                       // returning a converted value
	operator	char* (); 
	operator	int ();
	operator	float ();
	operator	unsigned long ();

	STRINGC&	ToDec ();// conversion to and from decimal and hex
	STRINGC&	ToHex ();

		//************************ file functions

	STRINGC		ExtractFileName ();// these change the internal value	
	STRINGC		ExtractName () { return ExtractFileName (); }
	STRINGC		ExtractExtension ();
	STRINGC		ExtractPath (); // path without the last forward slash
	STRINGC		ExtractFilePath () { return ExtractPath (); }
	void		EliminateExtension ();
	void		EliminateFilename ();
	bool		HasExtension ();

		//************************

	void    	Lower ();
	void    	ConvertToLowerCase ();// case changes
	void    	Upper ();
	void    	ConvertToUpperCase ();

	//complex
	void    	ConvertAllType1ToType2 (char* str, char type1, char type2); 
	void    	ConvertAllBackslashesToForwardslashes ();// simplified
	void    	ConvertBackslash ();

		//************************

	void    	AllignLeft ();
	void    	AllignRight (int BufferSize);
	void    	AllignCenter (int BufferSize);

		//************************

	STRINGC		LeftStr (int num);
	STRINGC		RightStr (int num);
	STRINGC		MidStr (int left, int right);

		//************************

	void    	Erase (char type);// replace all values of this type with 0
	void		Replace (char item, char with);//replace all 
	// instances of "item" with "with"

		//************************// linked list related
		//************************

	STRINGC*	GetNext () { return next; }
	STRINGC*	PlaceInList (STRINGC* list);
	STRINGC*	GetNthItem (int which);
	STRINGC*	AppendList (STRINGC* list);// not implemented
	void		AppendToList (STRINGC* item);
	void		AppendToList (STRINGC item);

		//************************
}
STR, *STRptr, **STRlist;

//********************************************
//********************************************
//********************************************

typedef
struct STRING_POINTER
{
	char* ptr;

	STRING_POINTER() {ptr = 0L;}
	STRING_POINTER(char * s) {Set (s);}

	void  Set (char* s) {ptr = s;}
	void  endl () {*ptr++ = 13, *ptr++ = 10;}
	void  endl (int num) {while (num--){*ptr++ = 13, *ptr++ = 10;}}	

	void  operator = (char * s);
	
	void  operator + (int i);
	void  operator + (char * s);
	void  operator + (STRINGC& s);

	void  operator += (int i);
	void  operator += (char * s);
	void  operator += (STRINGC&  s);

	void  operator ++ (){*ptr++ = 13, *ptr++ = 10;}
} STR_PTR;

//********************************************
//********************************************
//********************************************
#endif