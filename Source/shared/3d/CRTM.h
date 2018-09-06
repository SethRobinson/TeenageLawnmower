/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#pragma once

#pragma warning (disable:4786)
#include <vector>
using namespace std;

#include "d3dx8math.h"
#include "..\\all.h"
const float C_CRTM_VERSION = 1.0;

//Robinson Technologies Model format header.  Also tools for loading/saving and extracting
//model info.
class CRTMFrame
{
public:

    CRTMFrame()
	{
      p_verts = NULL;
	  i_vert_count = 0;
	}
	~CRTMFrame()
	{
	   //kill any memory we had allocated
		SAFE_FREE(p_verts);
	}

	long i_vert_count;
	D3DXVECTOR3 *p_verts;

};

class CRTMAnim
{
public:
	CRTMAnim()
	{
		memset(this, 0, sizeof(CRTMAnim));
	}
	
	char st_name[12];
	int i_start;
	int i_end;
	int i_length;
	char buffer[20]; //in case I want to add stuff later

};

typedef vector <CRTMFrame*> VertexVec;
typedef vector <CRTMAnim> AnimVec;


class CRTM
{
public:

	CRTM();
    ~CRTM();

  //this will copy some verts into our own internal buffer.  Does not change input data
  bool CRTM::AddVerts(int i_frame, D3DXVECTOR3 *p_verts, long i_count, D3DXVECTOR3 *p_vec_offset);
  bool CRTM::AddTexCoords(D3DXVECTOR2 *p_tex_verts, long i_count);
  bool CRTM::AddNormals(D3DXVECTOR3 *p_normals, long i_count);
  //16 bit face index.  I use 16 because many cards don't work with 32 (like geforce2)	and I
  //want to memcpy them into the buffer later.
  
  //auto offset adds the old number of faces to each number.  So you can send 0 based indexes
  //without everything effing up.
  bool CRTM::AddFaces(WORD *p_faces, long i_count, bool b_auto_offset);

  
  void CRTM::DeleteAnimations();
  bool CRTM::AddAnimation(char *st_name, int i_start, int i_end);
  int GetAnimations(){return m_i_animations;};
  int GetAnimIndexByName(char *st_name);
  CRTMAnim * GetPointerToAnim(int i_anim){return &(*m_p_a_anim)[i_anim];}
  
  //a "frame" is a global frame, not to any particular animation, it's all frames together as loaded.

  D3DXVECTOR3 * GetPointerToFrameVerts(int i_frame){return (*m_p_a_frame)[i_frame]->p_verts;};
  CRTMFrame * GetPointerToFrame(int i_frame){return (*m_p_a_frame)[i_frame];};

  int GetVertCountOfFrame(int i_frame){return (*m_p_a_frame)[i_frame]->i_vert_count;};


  int CRTM::GetFrameCount(){return m_i_total_frames;}; //total frames of all animations together
  void CRTM::SetFrameCount(int i_frames); //total frames of all animations together
  void CRTM::FreeAll();
  bool CRTM::SaveRTM(char *fname);
  bool CRTM::LoadRTM(char *fname);

  float m_f_version; 			
  long m_l_tex_verts; //generally will be the same number as the frame[0] verts, but if I try to use this
  long m_l_faces; //how many index we have.  devide by 3 to get triangles used
  //to save MD2 info this won't be true
  long m_l_normal_count;
  long m_i_total_frames;
 
  //extra info to save on loading time processing.  If 0, they are invalid and you
  //need to compute your own.

  D3DXVECTOR3 m_vec_bb_min, m_vec_bb_max; //bounding box info if we have it
  D3DXVECTOR3         m_vec_sphere_center;        // Center of bounding sphere of object
  FLOAT               m_vec_sphere_radius;        // Radius of bounding sphere of object


  
  WORD *m_p_a_faces; //pointer to an index buffer
  D3DXVECTOR2 *m_p_a_tex; //texturecoordinate info.  One copy for all frames.
  D3DXVECTOR3 *m_p_a_normals; //normals, matching face info. 
  VertexVec *m_p_a_frame;
  int m_i_animations; //how many anims we got
  AnimVec *m_p_a_anim; //pointer to our vector of animation data
  char st_name[50];
 };
