//-----------------------------------------------------------------------------
// File: Vectormath.h
//
// Desc: A collection of vector math related functions.
//
// Copyright (c) 2000 Telemachos of Peroxide
// www.peroxide.dk
//-----------------------------------------------------------------------------

#ifndef VECTORMATH_H
#define VECTORMATH_H

#include <d3dx8.h>

#define PLANE_BACKSIDE 0x000001
#define PLANE_FRONT    0x000010
#define ON_PLANE       0x000100


// basic vector operations (inlined)
inline float dot(D3DVECTOR& v1, D3DVECTOR& v2)
 {
 return ( v1.x * v2.x + v1.y * v2.y + v1.z * v2.z );	
}

inline void normalizeVector(D3DVECTOR& v) 
{
 float len = float(sqrt(v.x*v.x + v.y*v.y + v.z*v.z));
 v.x /= len;
 v.y /= len;
 v.z /= len;
}

inline double lengthOfVector(D3DVECTOR v) {
 return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

inline void setLength(D3DVECTOR& v, float l) {
 float len = float(sqrt(v.x*v.x + v.y*v.y + v.z*v.z));
 v.x *= l/len;
 v.y *= l/len;
 v.z *= l/len;
} 

inline BOOL isZeroVector(D3DVECTOR& v) {
 if ((v.x == 0.0f) && (v.y == 0.0f) && (v.z == 0.0f))
   return TRUE;
   
  return FALSE;	
}

inline D3DVECTOR wedge(D3DVECTOR v1, D3DVECTOR v2) {
 D3DVECTOR result;
 
 result.x = (v1.y * v2.z) - (v2.y * v1.z);
 result.y = (v1.z * v2.x) - (v2.z * v1.x);
 result.z = (v1.x * v2.y) - (v2.x * v1.y); 	
 
 return (result);	
}



// ray intersections. All return -1.0 if no intersection, otherwise the distance along the 
// ray where the (first) intersection takes place
double      intersectRayPlane(D3DVECTOR rOrigin, D3DVECTOR rVector, D3DVECTOR pOrigin, D3DVECTOR pNormal); 
double      intersectRaySphere(D3DXVECTOR3 rO, D3DXVECTOR3 rV, D3DXVECTOR3 sO, double sR);

// Distance to line of triangle
D3DVECTOR  closestPointOnLine(D3DXVECTOR3& a, D3DXVECTOR3& b, D3DXVECTOR3& p);
D3DVECTOR  closestPointOnTriangle(D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c, D3DXVECTOR3 p);

// point inclusion
BOOL CheckPointInTriangle(D3DXVECTOR3 point ,D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c);
BOOL CheckPointInSphere(D3DXVECTOR3 point, D3DXVECTOR3 sO, double sR);

// Normal generation
D3DVECTOR tangentPlaneNormalOfEllipsoid(D3DXVECTOR3 point,D3DXVECTOR3 eO, D3DXVECTOR3 eR);

// Point classification
DWORD classifyPoint(D3DXVECTOR3 point, D3DXVECTOR3 pO, D3DXVECTOR3 pN);

int intersect_triangle(float orig[3], float dir[3],
                   float vert0[3], float vert1[3], float vert2[3],
                   float *t, float *u, float *v);


#endif // VECTORMATH_H