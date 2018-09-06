// VECTOR.cpp

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

#include "c:/library/struct.h"
#include "c:/library/vector.h"
#include <math.h>

#ifndef __VECTOR__
#define __VECTOR__

//---------------------------------------------------------------------------

// the preceeding letter k is for the word constant but we don't want to use C
// to avoid the MFC confusion

static float	k256          = 256.0F;
static float	k65536        = 65536.0F;

static float	kONEOVER256   = 1.0F / 256.0F;
static float	kONEOVER65536 = 1.0F / 65536.0F;

static float	k15BIT_OVER_PI = 32768.0F / (float) PI;
static float	kPI_OVER_15BIT = (float) PI / 32768.0F;

// visual C++ has some problems with inline code
#define INLINE 
//_inline

//************************************
//************************************

VECTOR :: VECTOR () 
{
	Clear ();
}

	//******************

VECTOR :: VECTOR (const float a, const float b, const float c) 
{
	x = a; y = b; z = c; //pad = 0.0F;
}

	//******************

VECTOR :: VECTOR (const VECTOR& vect)
{
	x = vect.x, 
	y = vect.y, 
	z = vect.z;
}

	//******************

VECTOR :: VECTOR (const VECTOR* vect)
{
	x = vect->x, 
	y = vect->y, 
	z = vect->z;
}

	//******************

VECTOR :: VECTOR (const VECTOR& a, const VECTOR& b)
{
	x = a.x - b.x;
	y = a.y - b.y;
	z = a.z - b.z;
}

	//******************

VECTOR :: VECTOR (const VECTOR* a, const VECTOR* b)
{
	x = a->x - b->x;
	y = a->y - b->y;
	z = a->z - b->z;
}

	//******************

VECTOR :: ~VECTOR() {}// nothing happening here

	//******************

INLINE void	VECTOR ::  Clear () 
{
	x = 0.0F, y = 0.0F, z = 0.0F;
}

	//******************

INLINE void	VECTOR ::  Set (float xe, float ye, float ze)
{
	x = xe, y = ye, z = ze;
}

	//******************

INLINE void	VECTOR ::  Get (float& xe, float& ye, float& ze)
{
	xe = x, ye = y, ze = z;
}
//************************************
//************************************

	//******************

INLINE	VECTOR&	VECTOR :: operator - () const 
{
	VECTOR temp;
	temp.x = -x;
	temp.y = -y;
	temp.z = -z;
	return VECTOR(temp);
}

	//******************

INLINE VECTOR&	VECTOR :: operator = (const VECTOR& a) 
{
	if ( this !=& a ) 
	{
		x = a.x;
		y = a.y;
		z = a.z;
	}
	return (*this);
}

	//******************
/*
INLINE VECTOR&	FC  VECTOR :: operator =	(const VECTOR a)
{
		if ( this !=& a ) 
	{
		x = a.x;
		y = a.y;
		z = a.z;
	}
	return (*this);
}*/

	//******************

INLINE VECTOR&	VECTOR :: operator =	(const float s)
{
	x = s;
	y = s;
	z = s;
	return (*this);
}

	//******************

INLINE VECTOR&	VECTOR :: operator + (const VECTOR& a) const
{
	return (VECTOR(*this) -= a);
}

	//******************

INLINE VECTOR&	VECTOR :: operator - (const VECTOR& a) const 
{
	return (VECTOR(*this) -= a);
}

	//******************

INLINE float	VECTOR :: operator * (const VECTOR& a) const 
{
   	return (x * a.x + y * a.y + z * a.z);
}

	//******************

INLINE VECTOR&	VECTOR :: operator * (const float& a) const 
{
	return (VECTOR(*this) *= a);
}

	//******************

INLINE VECTOR&	VECTOR :: operator / (const float& a) const 
{
	return (VECTOR(*this) /= a);
}

	//******************

INLINE VECTOR&	VECTOR :: operator / (const VECTOR& a) const 
{
	VECTOR v (*this);
	v.x /= a.x;
	v.y /= a.y;
	v.z /= a.z;
	return VECTOR(v);
}

	//******************

INLINE VECTOR&	VECTOR :: operator *= (const float& a) 
{
	x *= a;
	y *= a;
	z *= a;
	return (*this);
}

	//******************

INLINE VECTOR&	VECTOR :: operator /= (const float& a) 
{
	x /= a;
	y /= a;
	z /= a;
	return (*this);
}
	//******************

INLINE VECTOR&	VECTOR :: operator += (const VECTOR& a) 
{
	x += a.x;
	y += a.y;
	z += a.z;
	return (*this);
}

	//******************

INLINE VECTOR&	VECTOR :: operator -= (const VECTOR& a) 
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return (*this);
}

	//******************

INLINE int	VECTOR :: operator == (const VECTOR& a) const 
{
	if (x == a.x && y == a.y && z == a.z) return(1);
	return (0);
}

	//******************

INLINE	int	VECTOR :: operator != (const VECTOR& a) const 
{
	return !(*this == a);
}

	//******************
/*
INLINE	float VECTOR :: operator [] (const int which) const
{
	if (which<0 || which>2) return 0.0;
	if (which == 0) return x;
	if (which == 1) return y;
	return z;
}*/

//************************************
//************************************

bool	VECTOR :: IsPerpendicular (const VECTOR& a) const
{
	if (DotProduct (a) == 0.0F) return true; 
	return false;
}

	//******************

bool	VECTOR :: IsPerpendicular (const VECTOR* a) const
{
	if (DotProduct (a) == 0.0F) return true; 
	return false;
}

	//******************

bool	VECTOR :: IsParallel (const VECTOR& a) const
{
	VECTOR v (a, *this);
	if (v.Length () == 0.0F) return true;
	return false;
}
	//******************

bool	VECTOR :: IsParallel (const VECTOR* a) const
{
	VECTOR v (a, *this);
	if (v.Length () == 0.0F) return true;
	return false;
}

	//******************

INLINE VECTOR&	VECTOR :: CrossProduct (const VECTOR* a) const
{
	VECTOR	temp;
	temp.x = (y*  a->z) - (z*  a->y);
	temp.y = (z*  a->x) - (x*  a->z);
	temp.z = (x*  a->y) - (y*  a->x);
	return VECTOR (temp);
}

	//******************

INLINE VECTOR&	VECTOR :: CrossProduct (const VECTOR& a)  const
{
	VECTOR	temp ;
	temp.x = (y*  a.z) - (z*  a.y);
	temp.y = (z*  a.x) - (x*  a.z);
	temp.z = (x*  a.y) - (y*  a.x);
	return VECTOR (temp);
}

	//******************

INLINE float	VECTOR :: DotProduct (const VECTOR* a) const
{
	float val = (x*  a->x + y*  a->y + z*  a->z);
	return val;
}

	//******************

INLINE float	VECTOR :: DotProduct (const VECTOR& a) const
{
	float val = (x*  a.x + y*  a.y + z*  a.z);
	return val;
}

	//******************

INLINE float	VECTOR :: Length () const 
{
	float val = x*  x + y*  y + z*  z;
	// i've tried assembly here and it is slower
	// the code runs much faster written like this, about 5.5 times as fast
	return (float)sqrt(val);
}

	//******************

INLINE	void	VECTOR :: Normalize () 
{
	float	l = Length();
	if (l > 0.0F) // can never be negative
	{
		l = 1.0F / l;
	/*	x = x*  l;
		y = y*  l;
		z = z*  l;*/
		_asm// the assembly makes the math faster, about 30%
		{
		//	x = x*  l;
			mov         eax, dword ptr [this]

			fld         dword ptr [eax]
			fmul        dword ptr [l]
			fstp        dword ptr [eax]

		//	y = y*  l;
			fld         dword ptr [eax+4]
			fmul        dword ptr [l]
			fstp        dword ptr [eax+4]

		//  z = z*  l;
			fld         dword ptr [eax+8]
			fmul        dword ptr [l]
			fstp        dword ptr [eax+8]
		}
	} 
	else 
	{
		Clear ();
	}
}

	//******************

INLINE void	VECTOR :: ConvertToPoint (const point* pt) const 
{
	// this assembly speeds up the code significantly
	_asm 
	{
		mov		eax, this
		mov		ebx, pt

		fld		dword ptr [eax+0]
		fmul	k256
		fistp	dword ptr [ebx+0]

		fld		dword ptr [eax+4]
		fmul	k256
		fistp	dword ptr [ebx+4]

		fld		dword ptr [eax+8]
		fmul	k256
		fistp	dword ptr [ebx+8]
	}
}

	//******************

INLINE void	VECTOR :: ConvertToPoint16 (const point* pt) const 
{
	// this assembly speeds up the code significantly
	_asm 
	{
		mov		eax, this
		mov		ebx, pt

		fld		dword ptr [eax+0]
		fmul	k65536
		fistp	dword ptr [ebx+0]

		fld		dword ptr [eax+4]
		fmul	k65536
		fistp	dword ptr [ebx+4]

		fld		dword ptr [eax+8]
		fmul	k65536
		fistp	dword ptr [ebx+8]
	}
}

	//******************

INLINE void	VECTOR :: ConvertToAngle (const point* pt) const 
{
	// this assembly speeds up the code significantly
	_asm 
	{
		mov		eax, this
		mov		ebx, pt

		fld		dword ptr [eax+0]
		fmul	k15BIT_OVER_PI
		fistp	dword ptr [ebx+0]

		fld		dword ptr [eax+4]  
		fmul	k15BIT_OVER_PI      
		fistp	dword ptr [ebx+4]

		fld		dword ptr [eax+8]
		fmul	k15BIT_OVER_PI
		fistp	dword ptr [ebx+8]
	}
}

	//******************

INLINE void	VECTOR :: ConvertFromPoint (const point* pt) 
{
	// this assembly speeds up the code significantly
	_asm 
	{
		mov		eax, pt
		mov		ebx, this

		fild	dword ptr [eax+0]
		fmul	kONEOVER256
		fstp	dword ptr [ebx+0]

		fild	dword ptr [eax+4]
		fmul	kONEOVER256
		fstp	dword ptr [ebx+4]

		fild	dword ptr [eax+8]
		fmul	kONEOVER256
		fstp	dword ptr [ebx+8]
	}
}

	//******************

INLINE void	VECTOR :: ConvertFromPoint16 (const point* pt) 
{
	// this assembly speeds up the code significantly
	_asm 
	{
		mov		eax, pt
		mov		ebx, this

		fild	dword ptr [eax+0]
		fmul	kONEOVER65536
		fstp	dword ptr [ebx+0]

		fild	dword ptr [eax+4]
		fmul	kONEOVER65536
		fstp	dword ptr [ebx+4]

		fild	dword ptr [eax+8]
		fmul	kONEOVER65536
		fstp	dword ptr [ebx+8]
	}
}

	//******************

INLINE void	VECTOR :: ConvertFromAngle (const point* pt) 
{
	// this assembly speeds up the code significantly
	_asm 
	{
		mov		eax, pt
		mov		ebx, this

		fild	dword ptr [eax+0]
		fmul	kPI_OVER_15BIT
		fstp	dword ptr [ebx+0]

		fild	dword ptr [eax+4]  
		fmul	kPI_OVER_15BIT      
		fstp	dword ptr [ebx+4]

		fild	dword ptr [eax+8]
		fmul	kPI_OVER_15BIT
		fstp	dword ptr [ebx+8]

	}
}

	//******************

void VECTOR :: Project (short& px, short& py, float dist)// projection or screen coordinates
{
	float dz = RECIP (dist-z) * dist;
	px = static_cast <short> (x * dz);
	py = static_cast <short> (y * dz);
}

	//******************

void VECTOR :: Angle ()// take all values and keep them between -pi an pi
{
	x = WRAP_IT (x, -PI, PI);
	y = WRAP_IT (y, -PI, PI);
	z = WRAP_IT (z, -PI, PI);
}

//****************************************************
//****************************************************

#endif // __VECTOR__


