/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */




#ifndef VERTEX3D_H
#define VERTEX3D_H

#include <D3DX8.h>

/******************************************************************************
	Globals.
*****************************************************************************/
// Our custom FVF, which describes our custom vertex structure
const DWORD D3DFVF_VERTEX3D = ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | 
							D3DFVF_TEX2);

/******************************************************************************
	The Vertex3D class.
 *****************************************************************************/
typedef class Vertex3D
{
public:
	inline Vertex3D()
			: m_fX(0), m_fY(0), m_fZ(0), m_fNX(0), m_fNY(1), m_fNZ(0), 
			  m_dwColor(0), m_fTU(0), m_fTV(0) {}

	inline Vertex3D( FLOAT x, FLOAT y, FLOAT z, FLOAT nx, FLOAT ny, FLOAT nz, 
					 DWORD color, FLOAT tu, FLOAT tv )
			: m_fX(x), m_fY(y), m_fZ(z), m_fNX(nx), m_fNY(ny), m_fNZ(nz), 
			  m_dwColor(color), m_fTU(tu), m_fTV(tv) {}

	inline ~Vertex3D() {}

	//Accessors for the XYZ coordinates of a vertex.
	inline void		setX( FLOAT x ) { m_fX = x; }
	inline void		setY( FLOAT y ) { m_fY = y; }
	inline void		setZ( FLOAT z ) { m_fZ = z; }

	inline FLOAT	getX() const { return m_fX; }
	inline FLOAT	getY() const { return m_fY; }
	inline FLOAT	getZ() const { return m_fZ; }
	inline D3DXVECTOR3 get_pos() {return D3DXVECTOR3(m_fX, m_fY, m_fZ);};
	//Accessors for the Normals
	inline void		setNormal( FLOAT nx, FLOAT ny, FLOAT nz ) 
	{ 
		//these are just accessors... they do not normalize anything.
		//you have to calculate the normals before passing them into here.
		m_fNX = nx; 
		m_fNY = ny;
		m_fNZ = nz;
	}

	inline void setNormal(D3DXVECTOR3 vec)
	{
	 	m_fNX = vec.x; 
		m_fNY = vec.y;
		m_fNZ = vec.z;

	}
		inline D3DXVECTOR3 getNormal(void)
	{
	 	D3DXVECTOR3 vec_temp;
		vec_temp.x = m_fNX;
		vec_temp.y = m_fNY;
		vec_temp.z = m_fNZ;
		return vec_temp;

	}

	inline FLOAT	getNX() const { return m_fNX; }
	inline FLOAT	getNY() const { return m_fNY; }
	inline FLOAT	getNZ() const { return m_fNZ; }

	//gets and sets the color
	inline DWORD	getColor() const { return m_dwColor; }
	inline void		setColor( DWORD color ) { m_dwColor = color; }

	//accessors for the Texture coordinates.
	inline void		setTexCoords( FLOAT tu, FLOAT tv ) 
	{ 
		m_fTU = tu;
		m_fTV = tv;
	}
 	inline void		setTexCoords2( FLOAT tu, FLOAT tv ) 
	{ 
		m_fTU2 = tu;
		m_fTV2 = tv;
	}


	inline FLOAT	getTU() const { return m_fTU; }
	inline FLOAT	getTV() const { return m_fTV; }
	inline FLOAT	getTU2() const { return m_fTU2; }
	inline FLOAT	getTV2() const { return m_fTV2; }



private:
	FLOAT m_fX, m_fY, m_fZ; // The transformed position for the vertex
	FLOAT m_fNX, m_fNY, m_fNZ; //The normals.
	DWORD m_dwColor; //The color
	//DWORD m_dwSpecular; //The color
	FLOAT m_fTU, m_fTV; //The texture coordinates.
	FLOAT m_fTU2, m_fTV2; //The texture coordinates for the second texture

} VERTEX3D, *LPVERTEX3D;

#endif // VERTEX3D_H


