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

#include "CParticle.h"
#include "RecyclingArrayDyn.h"
#include "CParticleEvent.h"
#include <vector>

class CParticleEventSequence
{
public:

  HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE8 pDev);
  HRESULT InvalidateDeviceObjects();

  void Reset(void);
void CParticleEventSequence::Update(float fElapsedTime, float fTimeDelta, D3DXVECTOR3 m_vPartSysPos, D3DXVECTOR3 vec_added_vel, bool b_no_new_particles, float f_rotation);
void CParticleEventSequence::Render(LPDIRECT3DVERTEXBUFFER8 pVB, int iVBSize, float f_angle_offset);
  void CreateNewParticle(D3DXVECTOR3 m_vPartSysPos);
  void CreateFadeLists();

  void SortEvents(void);
  void NailDownRandomTimes(void);

  CParticleEventSequence();
  virtual ~CParticleEventSequence();

  CMinMax<float> GetLifetime(void) const { return(m_Lifetime); }
  void SetLifetime(const CMinMax<float> data) { m_Lifetime = data; }

  int GetSrcBlendMode(void) const { return(m_iSrcBlendMode); }
  void SetSrcBlendMode(const int data) { m_iSrcBlendMode = data; }

  int GetDestBlendMode(void) const { return(m_iDestBlendMode); }
  void SetDestBlendMode(const int data) { m_iDestBlendMode = data; }

  CMinMax<float> GetEmitRate(void) const { return(m_EmitRate); }
  void SetEmitRate(const CMinMax<float> data) { m_EmitRate = data; }

  CMinMax<D3DXVECTOR3> GetEmitRadius(void) const { return(m_vEmitRadius); }
  void SetEmitRadius(const CMinMax<D3DXVECTOR3> &data) { m_vEmitRadius = data; }

  CMinMax<D3DXVECTOR3> GetGravity(void) const { return(m_vGravity); }
  void SetGravity(const CMinMax<D3DXVECTOR3> &data) { m_vGravity = data; }

  std::string GetName(void) const { return(m_strName); }
  void SetName(const std::string &data) { m_strName = data; }
  
  int GetNumActiveParticles(void) { return(m_Particles ? m_Particles->GetNumUsedElements() : 0); }

  int GetNumParticles(void) const { return(m_iNumParticles); }
  void SetNumParticles(const int data) { m_iNumParticles = data; }

	int GetLoops(void) { return(m_Loops); }
	void SetLoops(const int data) { m_Loops = data; }

  CMinMax<D3DXVECTOR3> GetSpawnDir(void) const { return(m_vSpawnDir); }
  void SetSpawnDir(const CMinMax<D3DXVECTOR3> &data) { m_vSpawnDir = data; }

//  std::string GetTextureFilename(void) const { return(m_strTexFilename); }
  LPDIRECT3DTEXTURE8 GetTexture(void) const { return(m_texParticle); }
  void SetTexture(const char *strTexFilename);

  std::vector<CParticleEvent *> m_Events;

  void RunEvents(CParticle &part);
	void DeleteAllParticles(void) { m_Particles->DeleteAll(); m_iTotalParticleLives = 0; }
	D3DXVECTOR3 vec_attached_vel;
    float m_f_rotation;
   void CParticleEventSequence::RefreshTextureInfo();	

private:
  CRecyclingArrayDyn<CParticle> *m_Particles;

  CMinMax<float> m_Lifetime;
  CMinMax<float> m_EmitRate; // in particles/sec
  int m_Loops;
  CMinMax<D3DXVECTOR3> m_vSpawnDir;
  CMinMax<D3DXVECTOR3> m_vEmitRadius;
  CMinMax<D3DXVECTOR3> m_vGravity;

 // std::string m_strTexFilename;
  LPDIRECT3DTEXTURE8 m_texParticle;  
  LPDIRECT3DDEVICE8 m_pd3dDevice;
  int m_iNumParticles;
  int m_iDestBlendMode;
  int m_iSrcBlendMode;
	
	long int m_iTotalParticleLives;
  
  std::string m_strName;
  int m_i_tex_id;
  
  float m_fNumNewPartsExcess;
};

