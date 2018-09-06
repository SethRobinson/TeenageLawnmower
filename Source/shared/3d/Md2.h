#ifndef _MD2_H
#define _MD2_H

#include "..\\all.h"
#include <vector>
using namespace std;
#include <string>

#define MAX_TEXTURES 100								// The maximum amount of textures to load

// This is our 3D point class.  This will be used to store the vertices of our model.
class CVector3 
{
public:
	float x, y, z;
};

// This is our 2D point class.  This will be used to store the UV coordinates.
class CVector2 
{
public:
	float x, y;
};

// This is our face structure.  This is is used for indexing into the vertex 
// and texture coordinate arrays.  From this information we know which vertices
// from our vertex array go to which face, along with the correct texture coordinates.
struct tFace
{
	int vertIndex[3];			// indicies for the verts that make up this triangle
	int coordIndex[3];			// indicies for the tex coords to texture this face
};



// This holds all the information for our model/scene. 
// You should eventually turn into a robust class that 
// has loading/drawing/querying functions like:
// LoadModel(...); DrawObject(...); DrawModel(...); DestroyModel(...);
struct t3DObject 
{
	int  numOfVerts;			// The number of verts in the model
	int  numOfFaces;			// The number of faces in the model
	int  numTexVertex;			// The number of texture coordinates
	int  materialID;			// The texture ID to use, which is the index into our texture array
	bool bHasTexture;			// This is TRUE if there is a texture map for this object
	char strName[255];			// The name of the object
	CVector3  *pVerts;			// The object's vertices
	CVector3  *pNormals;		// The object's normals
	CVector2  *pTexVerts;		// The texture's UV coordinates
	tFace *pFaces;				// The faces information of the object
};


//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

// This holds our information for each animation of the Quake model.
// A STL vector list of this structure is created in our t3DModel structure below.
struct tAnimationInfo
{
	char strName[255];			// This stores the name of the animation (Jump, Pain, etc..)
	int startFrame;				// This stores the first frame number for this animation
	int endFrame;				// This stores the last frame number for this animation
};

// We added 4 new variables to our model structure.  These will help us handle
// the current animation.  As of now, the current animation will continue to loop
// from it's start from to it's end frame until we right click and change animations.
struct t3DModel 
{
	int numOfObjects;					// The number of objects in the model
	int numOfMaterials;					// The number of materials for the model
	int numOfAnimations;				// The number of animations in this model (NEW)
	int currentAnim;					// The current index into pAnimations list (NEW)
	int currentFrame;					// The current frame of the current animation (NEW)
	vector<tAnimationInfo> pAnimations; // The list of animations (NEW)
	vector<t3DObject> pObject;			// The object list for our model
};



// These are the needed defines for the max values when loading .MD2 files
#define MD2_MAX_TRIANGLES		4096
#define MD2_MAX_VERTICES		2048
#define MD2_MAX_TEXCOORDS		2048
#define MD2_MAX_FRAMES			512
#define MD2_MAX_SKINS			32
#define MD2_MAX_FRAMESIZE		(MD2_MAX_VERTICES * 4 + 128)


//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

// This stores the speed of the animation between each key frame
#define kAnimationSpeed			5.0f

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////


// This holds the header information that is read in at the beginning of the file
struct tMd2Header
{ 
   int magic;					// This is used to identify the file
   int version;					// The version number of the file (Must be 8)
   int skinWidth;				// The skin width in pixels
   int skinHeight;				// The skin height in pixels
   int frameSize;				// The size in bytes the frames are
   int numSkins;				// The number of skins associated with the model
   int numVertices;				// The number of vertices (constant for each frame)
   int numTexCoords;			// The number of texture coordinates
   int numTriangles;			// The number of faces (polygons)
   int numGlCommands;			// The number of gl commands
   int numFrames;				// The number of animation frames
   int offsetSkins;				// The offset in the file for the skin data
   int offsetTexCoords;			// The offset in the file for the texture data
   int offsetTriangles;			// The offset in the file for the face data
   int offsetFrames;			// The offset in the file for the frames data
   int offsetGlCommands;		// The offset in the file for the gl commands data
   int offsetEnd;				// The end of the file offset
};


// This is used to store the vertices that are read in for the current frame
struct tMd2AliasTriangle
{
   byte vertex[3];
   byte lightNormalIndex;
};

// This stores the normals and vertices for the frames
struct tMd2Triangle
{
   float vertex[3];
   float normal[3];
};

// This stores the indices into the vertex and texture coordinate arrays
struct tMd2Face
{
   unsigned short vertexIndices[3];
   unsigned short textureIndices[3];
};

// This stores UV coordinates
struct tMd2TexCoord
{
   short u, v;
};

// This stores the animation scale, translation and name information for a frame, plus verts
struct tMd2AliasFrame
{
   float scale[3];
   float translate[3];
   char name[16];
   tMd2AliasTriangle aliasVertices[1];
};

// This stores the frames vertices after they have been transformed
struct tMd2Frame
{
   char strName[16];
   tMd2Triangle *pVertices;
};

// This stores a skin name
typedef char tMd2Skin[64];


// This class handles all of the loading code
class CLoadMD2
{

public:
	CLoadMD2();								// This inits the data members

	// This is the function that you call to load the MD2
	bool ImportMD2(t3DModel *pModel, char *strFileName, char *strTexture);

private:
	
	// This reads in the data from the MD2 file and stores it in the member variables
	void ReadMD2Data();


//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

	// This parses the animations name and calculates the number of animations and info
	void ParseAnimations(t3DModel *pModel);

//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////


	// This converts the member variables to our pModel structure
	void ConvertDataStructures(t3DModel *pModel);

    void ComputeNormals(t3DModel *pModel);


	// This frees memory and closes the file
	void CleanUp();
	
	// The file pointer
	FILE *m_FilePointer;

	// Member variables		

	tMd2Header				m_Header;			// The header data
	tMd2Skin				*m_pSkins;			// The skin data
	tMd2TexCoord			*m_pTexCoords;		// The texture coordinates
	tMd2Face				*m_pTriangles;		// Face index information
	tMd2Frame				*m_pFrames;			// The frames of animation (vertices)
};


#endif


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
// 
// For the second .MD2 tutorial we added a animation speed for our model: kAnimationSpeed.
// This is used for interpolation between each key frame.  A function was also added to
// parse through the animation names and create a list of animations for our model
// to cycle through.  This function is called ParseAnimations().  We did however, take 
// out the code for calculating the face normals.  This is because the texture maps are
// desinged to create their own lighting for the model.  It just makes the model darker
// to turn lighting on.  More of this is explained at the top of Main.cpp.
// 
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
// The Quake2 .Md2 file format is owned by ID Software.  This tutorial is being used 
// as a teaching tool to help understand model loading and animation.  This should
// not be sold or used under any way for commercial use with out written conset
// from ID Software.
//
// Quake and Quake2 are trademarks of ID Software.
// All trademarks used are properties of their respective owners. 
//
//
