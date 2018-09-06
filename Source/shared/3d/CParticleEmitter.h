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

#pragma warning(disable:4786)
#include <map>
#include <string>
#include "CVisual.h"
#include "CommonFuncs.h"
#include "D3DHelperFuncs.h"
#include "CParticle.h"
#include "CParticleEventSequence.h"
#include "CParticleEmitterTokenizer.h" 
#include "..\\CTimer.h"

extern LPDIRECT3DVERTEXBUFFER8 m_vbParticles; //share this with all particle systems


class CParticleEmitter : public CVisual
{
public:
  std::string m_strLastError;
  
  CParticleEmitter();
  virtual ~CParticleEmitter();
  void load_and_compile_particle_system(char st_file_name[256]);
  virtual void dx_restore();
  virtual void dx_create();
  virtual void dx_draw();
  virtual void dx_kill();
  virtual void dx_invalidate();
  virtual void think();
  virtual void Update(float fElapsedTime, float fTimeDelta, D3DXVECTOR3 vec_vel);
  virtual HRESULT Render();
  virtual HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE8 pDev);

  virtual void InvalidateDeviceObjects();

  virtual bool Compile(const char *script);
  virtual void Init();

	virtual void Start() { m_bIsRunning = true; }
	virtual void Pause() { m_bIsRunning = false; }
	virtual void Stop()  { Pause(); DeleteAllParticles(); }
	virtual bool IsRunning() { return(m_bIsRunning); }
    void SetComplexRotation(bool b_new){m_b_complex_rotation_math = b_new;}
	void DeleteAllParticles(void) {
		for (std::vector<CParticleEventSequence *>::iterator i = m_Sequences.begin(); i != m_Sequences.end(); i++) {
			(*i)->DeleteAllParticles();
		}
	}
  // attributes
  
  CMinMax<D3DXVECTOR3> GetPosRange(void) const { return(m_vPosRange); }
  void SetPosRange(const CMinMax<D3DXVECTOR3> &data) { m_vPosRange = data; }

  D3DXVECTOR3 GetPos(void) const { return(m_vPos); }
  void SetPos(const D3DXVECTOR3 &data) { m_vPos = data; }
  
  int GetVBSize(void) const { return(m_iVBSize); }
  void SetVBSize(const int data) { m_iVBSize = data; }

  void Reset(void) {
    for (std::vector<CParticleEventSequence *>::iterator i = m_Sequences.begin(); i != m_Sequences.end(); i++) {
      (*i)->Reset();
    }
  }

  int GetNumActiveParticles(void) {
	  int count=0;
	  for (std::vector<CParticleEventSequence *>::iterator i = m_Sequences.begin(); i != m_Sequences.end(); i++) {
		  count += (*i)->GetNumActiveParticles();
	  }
	  return(count);
  }
  void set_no_new_particles(bool b_new){b_no_new_particles = b_new;};
  bool get_no_new_particles(){return b_no_new_particles;};
  void SetFrameFollow(bool b_new){b_frame_follow = b_new;}
  static CParticleEvent *CParticleEmitter::EventFactory(std::string EventName);
  void set_apply_momentum(bool b_new){b_apply_momentum = b_new;};
  void SetMomentumMod(float f_new){m_f_momentum_mod = f_new;}
 
private:
  
  bool ProcessParticleSystemBlock(
    std::vector<CParticleEmitterToken>::iterator &TokenIter,
    std::vector<CParticleEmitterToken>::iterator &EndIter);

  bool ProcessEventSequenceBlock(CParticleEventSequence &seq,
    std::vector<CParticleEmitterToken>::iterator &TokenIter,
    std::vector<CParticleEmitterToken>::iterator &EndIter);
  
  // particle system attributes
  CMinMax<D3DXVECTOR3> m_vPosRange; 
  D3DXVECTOR3 m_vPos; // actual position of emitter

  std::string m_strName;

  std::vector<CParticleEventSequence *> m_Sequences;

//  int m_iMaxParticles;
	bool m_bIsRunning;
   bool b_apply_momentum;
  bool b_no_new_particles; //if true, no new particles but will keep going
  int m_iVBSize; // size of the vertex buffer (can and should be less than NUMPARTICLES)
  LPDIRECT3DDEVICE8 m_pd3dDevice;
  char st_file[256]; //so we can remember which script we loaded
  bool b_frame_follow; //particle system rotates around thing  
  float m_f_momentum_mod; //9000 means full momentum is given from ent to particles  
  bool m_b_complex_rotation_math; //this will do extra rotation math when following an entity so
  //when the ent turns the velocities turn with it
  
};

