//-----------------------------------------------------------------------------
// File: Vectormath.cpp
//
// Desc: A collection of vector math related functions.
//
// Copyright (c) 2000 Telemachos of Peroxide
// www.peroxide.dk
//-----------------------------------------------------------------------------


#define STRICT
#include <math.h>
#include <stdio.h>
#include "vectormath.h"


#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define VEC_PI    ((FLOAT)  3.141592654f)

// ----------------------------------------------------------------------
// Name  : intersectRayPlane()
// Input : rOrigin - origin of ray in world space
//         rVector - vector describing direction of ray in world space
//         pOrigin - Origin of plane 
//         pNormal - Normal to plane
// Notes : Normalized directional vectors expected
// Return: distance to plane in world units, -1 if no intersection.
// -----------------------------------------------------------------------  
double intersectRayPlane(D3DVECTOR rOrigin, D3DVECTOR rVector, D3DVECTOR pOrigin, D3DVECTOR pNormal) {
  
  double d = - (dot(pNormal,pOrigin));
 
  double numer = dot(pNormal,rOrigin) + d;
  double denom = dot(pNormal,rVector);
  
  if (denom == 0)  // normal is orthogonal to vector, cant intersect
   return (-1.0f);
   
  return -(numer / denom);	
}


// ----------------------------------------------------------------------
// Name  : intersectRaySphere()
// Input : rO - origin of ray in world space
//         rV - vector describing direction of ray in world space
//         sO - Origin of sphere 
//         sR - radius of sphere
// Notes : Normalized directional vectors expected
// Return: distance to sphere in world units, -1 if no intersection.
// -----------------------------------------------------------------------  

double intersectRaySphere(D3DXVECTOR3 rO, D3DXVECTOR3 rV, D3DXVECTOR3 sO, double sR) {
	
   D3DXVECTOR3 Q = sO-rO;
   
   double c = lengthOfVector(Q);
   double v = dot(Q,rV);
   double d = sR*sR - (c*c - v*v);

   // If there was no intersection, return -1
   if (d < 0.0) return (-1.0f);

   // Return the distance to the [first] intersecting point
   return (v - sqrt(d));
}


#define EPSILON 0.000001
#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest,v1,v2)	\
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 

int intersect_triangle(float orig[3], float dir[3],
                   float vert0[3], float vert1[3], float vert2[3])
{
   float edge1[3], edge2[3],  pvec[3];
   float det;

   /* find vectors for two edges sharing vert0 */
   SUB(edge1, vert1, vert0);
   SUB(edge2, vert2, vert0);

   /* begin calculating determinant - also used to calculate U parameter */
   CROSS(pvec, dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = DOT(edge1, pvec);

   if (det < EPSILON)
      return 0;

   return 1; //inside

  
}



// ----------------------------------------------------------------------
// Name  : CheckPointInTriangle()
// Input : point - point we wish to check for inclusion
//         a - first vertex in triangle
//         b - second vertex in triangle 
//         c - third vertex in triangle
// Notes : Triangle should be defined in clockwise order a,b,c
// Return: TRUE if point is in triangle, FALSE if not.
// -----------------------------------------------------------------------  

//I think this is buggy.  Misses some cases.

BOOL CheckPointInTriangle(D3DXVECTOR3 point, D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c) {
  
  float total_angles = 0.0f;
  //point.y = 0;     
  
  //add support for some special cases..
 // if (point == a) return true;
  //if (point == b) return true;
  //if (point == c) return true;
	
  
  
  // make the 3 vectors
  D3DXVECTOR3 v1 = point-a;
  D3DXVECTOR3 v2 = point-b;
  D3DXVECTOR3 v3 = point-c;
  
  normalizeVector(v1);
  normalizeVector(v2);
  normalizeVector(v3);

  total_angles += float(acos(dot(v1,v2)));
  total_angles += float(acos(dot(v2,v3)));
  total_angles += float(acos(dot(v3,v1)));
     
  if (fabs(total_angles-(2*VEC_PI)) <= 0.005)
//  if (fabs(total_angles-(2*VEC_PI)) <= 0.05)
   return (TRUE);
     
  return(FALSE);
}




//doesn't work!
/*
BOOL CheckPointInTriangle(D3DVECTOR p, D3DVECTOR a, D3DVECTOR b, D3DVECTOR c) {
  
  double total_angles = 0.0f;
       
  // make the 3 vectors
  D3DVECTOR c1 = (p - a) * (b-a);
  D3DVECTOR c2 = (p-b)*(c-b);
  D3DVECTOR c3 = (p-c)*(a-c);
  
  if ( (dot(c1,c2) * dot(c2,c3)) > 0) return true; //inside tri
  return(FALSE);
}

*/
/*
BOOL CheckPointInTriangle(D3DVECTOR v, D3DVECTOR v1, D3DVECTOR v2, D3DVECTOR v3) {
  
  // make the 3 vectors
  D3DVECTOR n = (v3 - v1) * (v2 - v1); //get normal
  
  if (  dot( (v - v1) * (v2 - v1), n) >= 0)
  if (  dot( (v - v2) * (v3 - v2), n) >= 0)
  if (  dot( (v - v3) * (v1 - v3), n) >= 0)
  return true; //inside tri

  return(FALSE);
}
*/


// ----------------------------------------------------------------------
// Name  : closestPointOnLine()
// Input : a - first end of line segment
//         b - second end of line segment
//         p - point we wish to find closest point on line from 
// Notes : Helper function for closestPointOnTriangle()
// Return: closest point on line segment
// -----------------------------------------------------------------------  

D3DVECTOR closestPointOnLine(D3DXVECTOR3& a, D3DXVECTOR3& b, D3DXVECTOR3& p) {
	
   // Determine t (the length of the vector from ‘a’ to ‘p’)
   D3DXVECTOR3 c = p-a;
   D3DXVECTOR3 V = b-a; 
      
   double d = lengthOfVector(V);
      
   normalizeVector(V);  
   double t = dot(V,c);

   
   // Check to see if ‘t’ is beyond the extents of the line segment
   if (t < 0.0f) return (a);
   if (t > d) return (b);
 
  
   // Return the point between ‘a’ and ‘b’
   //set length of V to t. V is normalized so this is easy
   V.x = float(V.x * t);
   V.y = float(V.y * t);
   V.z = float(V.z * t);
           
   return (a+V);	
}




// ----------------------------------------------------------------------
// Name  : closestPointOnTriangle()
// Input : a - first vertex in triangle
//         b - second vertex in triangle 
//         c - third vertex in triangle
//         p - point we wish to find closest point on triangle from 
// Notes : 
// Return: closest point on line triangle edge
// -----------------------------------------------------------------------  

D3DVECTOR closestPointOnTriangle(D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c, D3DXVECTOR3 p) {
	
  D3DXVECTOR3 Rab = closestPointOnLine(a, b, p);
  D3DXVECTOR3 Rbc = closestPointOnLine(b, c, p);
  D3DXVECTOR3 Rca = closestPointOnLine(c, a, p);
    
  double dAB = lengthOfVector(p-Rab);
  double dBC = lengthOfVector(p-Rbc);
  double dCA = lengthOfVector(p-Rca);
  
  
  double min = dAB;
  D3DXVECTOR3 result = Rab;
  
  if (dBC < min) {
    min = dBC;
    result = Rbc;
    }
 
  if (dCA < min)
   result = Rca;
  
    
  return (result);	
}



// ----------------------------------------------------------------------
// Name  : CheckPointInTriangle()
// Input : point - point we wish to check for inclusion
//         sO - Origin of sphere
//         sR - radius of sphere 
// Notes : 
// Return: TRUE if point is in sphere, FALSE if not.
// -----------------------------------------------------------------------  

BOOL CheckPointInSphere(D3DXVECTOR3 point, D3DXVECTOR3 sO, double sR)
 {
	
 float d = float(lengthOfVector(point-sO));
 
 if(d<= sR) return TRUE;
 return FALSE;	
}




// ----------------------------------------------------------------------
// Name  : tangentPlaneNormalOfEllipsoid()
// Input : point - point we wish to compute normal at 
//         eO - Origin of ellipsoid
//         eR - radius vector of ellipsoid 
// Notes : 
// Return: a unit normal vector to the tangent plane of the ellipsoid in the point.
// -----------------------------------------------------------------------  
D3DVECTOR tangentPlaneNormalOfEllipsoid(D3DXVECTOR3 point, D3DXVECTOR3 eO, D3DXVECTOR3 eR) {

 D3DXVECTOR3 p = point - eO;

 double a2 = eR.x * eR.x;
 double b2 = eR.y * eR.y;
 double c2 = eR.z * eR.z;

 
 D3DXVECTOR3 res;
 res.x = float(p.x / a2);
 res.y = float(p.y / b2);
 res.z = float(p.z / c2);

 normalizeVector(res);	
 return (res);	
}



// ----------------------------------------------------------------------
// Name  : classifyPoint()
// Input : point - point we wish to classify 
//         pO - Origin of plane
//         pN - Normal to plane 
// Notes : 
// Return: One of 3 classification codes
// -----------------------------------------------------------------------  

DWORD classifyPoint(D3DXVECTOR3 point, D3DXVECTOR3 pO, D3DXVECTOR3 pN) {

 D3DVECTOR dir = pO - point;
 double d = dot(dir, pN);
 
 if (d<-0.001f)
  return PLANE_FRONT;	
 else
 if (d>0.001f)
  return PLANE_BACKSIDE;	

return ON_PLANE;	
}




