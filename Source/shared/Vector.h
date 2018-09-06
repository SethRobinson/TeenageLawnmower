// vector.h

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

#ifndef __VECTOR_H_
#define __VECTOR_H_

#ifndef FC
#define FC __fastcall
#endif

//---------------------------------------------------------------------------

//****************************************************
//****************************************************

struct point	// the integer version of a VECTOR
{
	union 
	{
		int		x;
		int		pitch;
		int		theta;
		int		_1;
	};

	union 
	{
		int		y;
		int		yaw;
		int		psi;
		int		_2;
	};

	union 
	{
		int		z;
		int		roll;
		int		phi;
		int		_3;
	};
};

//****************************************************
//****************************************************

typedef class VECTOR
{
	//******************
public://variables

	union 
	{
		float	x;
		float	pitch;
		float	theta;
		float	_1;
	};

	union 
	{
		float	y;
		float	yaw;
		float	psi;
		float	_2;
	};

	union 
	{
		float	z;
		float	roll;
		float	phi;
		float	_3;
	};

	float	pad;// keeps structure on 128bit allignment
	// a float is 32 bit, if we are super worried about space, 
	// comment out this line

	//******************
	//******************

public:

	VECTOR ();
	VECTOR (const float a,const float b,const float c);
	VECTOR (const VECTOR& vect);
	VECTOR (const VECTOR* vect);
	VECTOR (const VECTOR& v1, const VECTOR& v2);
	VECTOR (const VECTOR* v1, const VECTOR* v2);

	~VECTOR();

	//******************

	void	FC Clear ();
	float	FC Length() const;
	float	FC Dist() const {return Length ();}
	void	FC Normalize ();
	bool	FC IsNonZero () {if ((x!=0) || (y!=0) || (z!=0)) return true; return false;}

	//******************

	void	FC Set (float xe, float ye, float ze);
	void	FC Get (float& xe, float& ye, float& ze);

	//******************

	VECTOR&	FC CrossProduct (const VECTOR* a) const;
	VECTOR&	FC CrossProduct (const VECTOR &a) const;

	float	FC DotProduct (const VECTOR* a) const;
	float	FC DotProduct (const VECTOR &a) const;
	
	//******************

	bool	FC IsPerpendicular (const VECTOR &a) const; // orthagonal
	bool	FC IsPerpendicular (const VECTOR* a) const;

	bool	FC IsParallel (const VECTOR &a) const; 
	bool	FC IsParallel (const VECTOR* a) const;

	//******************

	void	FC ConvertToPoint (const point* p) const;
	void	FC ConvertToPoint16 (const point* p) const;
	void	FC ConvertToAngle (const point* p) const;

	void	FC ConvertFromPoint (const point* p);
	void	FC ConvertFromPoint16 (const point* p);
	void	FC ConvertFromAngle (const point* p);


	//******************

	VECTOR&	FC operator -	() const;// negative

	//VECTOR&	FC operator =	(const VECTOR a);
	VECTOR&	FC operator =	(const VECTOR& a);
	VECTOR&	FC operator =	(const float s);

	VECTOR&	FC operator +	(const VECTOR& a) const;
	VECTOR&	FC operator -	(const VECTOR& a) const;

	float	FC operator * 	(const VECTOR& a) const;// dot product
	VECTOR&	FC operator * 	(const float& a)  const;
	VECTOR&	FC operator /	(const VECTOR& a) const;
	VECTOR&	FC operator /	(const float& a)  const;

	VECTOR&	FC operator *=	(const float& a);	
	VECTOR&	FC operator /=	(const float& a);
	VECTOR&	FC operator +=	(const VECTOR& a);
	VECTOR&	FC operator -=	(const VECTOR& a);	

	int		FC operator ==	(const VECTOR& a) const;
	int		FC operator !=	(const VECTOR& a) const;

	// we may need an array of these items
	//float	FC operator []	(const which) const;
	
	//******************

	void	FC	Project (short& x, short& y, float dist = 400.0f);// projection or screen coordinates

	void	FC	Angle ();// take all values and keep them between -pi an pi

}VECTOR, *VectorPtr, **VectorList;

//****************************************************
//****************************************************

typedef struct TEXTURE// basically a pointer to an uncompressed graphic
{
	U16ptr	Graphic;
	U16		Width, Height;
}TEXTURE, *TEXTUREptr, **TEXTURElist;

//****************************************************
//****************************************************

typedef struct TexturePoint
{
	short x, y;
}TexturePoint, *TexturePointPtr, **TexturePointList;

//****************************************************
//****************************************************
// this structure can be used for flat shading, gouraud shading, or texture mapped
// polygons, the TextureFlag tells us

typedef struct POLYGON
{
public:
	enum {FLAT_SHADED, GOURAUD_SHADED, TEXTURE_MAPPED};
	//******************
public://variables
	int		TextureFlag;// the Text member will either be texture coordinates
	// or it will be colors (RGB) per vertex for gouraud shading
	int		Vect[3];// generally 3 indices into the Frame3d->Vect array.
	int		Texture;// index in Frame3d->Texture array.
	TexturePoint	TextPt[3];// 3 points on the texture defining the texture map coordinates.
}*POLYGONPtr, **POLYGONList;

//****************************************************
//****************************************************
// this class has a series of Polys and Vectors. Those are connected
// by having the polys retain references (indices) to the vectors.
// each polgon can have a texture and they maintain an index into Frame3D->Texture.

class Frame3D
{
	//******************
public://variables
	int			NumPoly, NumVect;
	POLYGONPtr	Polys;
	VectorPtr	Vect;// generally 3	
	TEXTUREptr	Texture;
};

//****************************************************
//****************************************************
#undef FC
#endif