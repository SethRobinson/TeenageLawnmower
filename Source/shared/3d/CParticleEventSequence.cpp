/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



 #include "CManager.h"

#include "CParticleEventSequence.h"
#include "..\\all.h"
#include <algorithm>
#include "CThing_globals.h"
#include "d3dapp.h"
#include "CThing_Util.h" 
#include "CCamera.h"
#include <DXErr8.h>
#include <typeinfo>

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _NO_BATCH_POINT

CParticleEventSequence::CParticleEventSequence()
{
  m_pd3dDevice = NULL;
  m_texParticle = NULL;
  m_Particles = NULL;
  m_i_tex_id = C_RESOURCE_NONE;
  Reset();
}

CParticleEventSequence::~CParticleEventSequence()
{
  Reset();
  SAFE_DELETE(m_Particles);
}

void CParticleEventSequence::Reset()
{
	m_iTotalParticleLives = 0;
	m_Loops = -1;

  m_iNumParticles = 100;
  m_fNumNewPartsExcess = 0.0f;
  m_vGravity = CMinMax<D3DXVECTOR3>(D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.0f, 0.0f, 0.0f));
  m_vEmitRadius = CMinMax<D3DXVECTOR3>(D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.0f, 0.0f, 0.0f));
  for (std::vector<CParticleEvent *>::iterator i = m_Events.begin(); i != m_Events.end(); i++) {
    // can't use safe delete here, because it will set i = NULL.
    if (*i) delete (*i);
  }
  m_Events.clear();

  //SAFE_DELETE(m_Particles);
 // SAFE_RELEASE(m_texParticle);

   //if we had a texture let's release our reference
	if (m_i_tex_id)
	{
	 	if (cls_tglo.p_manager)
		cls_tglo.p_manager->delete_ref(m_i_tex_id);
       m_i_tex_id = C_RESOURCE_NONE; 

	}

       m_texParticle = NULL;
}

void CParticleEventSequence::SortEvents(void)
{
  // sort the events by the time they occur (initial = 1st, final = last)
  // a testament to the power of STL: one line to accomplish this.  Pretty sweet, huh?
  CParticleEvent_CompareFunc cf;
  std::sort(m_Events.begin(), m_Events.end(), cf);
 
  //too annoying, even for debug
  /*
#ifdef _DEBUG
  // spit out the list of events, for debugging purposes.
  OutputDebugString("\n\nSorted Events:");
  for (std::vector<CParticleEvent *>::iterator i = m_Events.begin(); i != m_Events.end(); i++) {
    char buf[256];
   _snprintf(buf, sizeof(buf), "\nTime: %0.2f Type: %20s Fade: %c",
     (*i)->GetActualTime(), typeid(*(*i)).name(), (*i)->IsFade() ? 'Y' : 'N');
    OutputDebugString(buf);
  }

#endif
  */
}

void CParticleEventSequence::CreateFadeLists()
{
  // for each event,
  for (std::vector<CParticleEvent *>::iterator i = m_Events.begin(); i != m_Events.end(); i++) {
    // try to find the next fade event of the same type.
    for (std::vector<CParticleEvent *>::iterator j = i; j != m_Events.end(); j++) {
      if (j != i && typeid(**j) == typeid(**i) && (*j)->IsFade()) {
        // we've found a fade event further in the future.  make a note that
        // this event needs to be linked to this future fade event (so that we
        // can calculate the deltas later).
        (*i)->m_NextFadeEvent = (*j);
        break;
      }
    }
  }

}
void CParticleEventSequence::NailDownRandomTimes(void)
{
  for (std::vector<CParticleEvent *>::iterator i = m_Events.begin(); i != m_Events.end(); i++) {
    (*i)->SetActualTime((*i)->GetTimeRange().GetRandomNumInRange());
  }
}

HRESULT CParticleEventSequence::RestoreDeviceObjects(LPDIRECT3DDEVICE8 pDev)
{

	
 //   SAFE_DELETE(m_Particles);
// assert(!m_Particles && "This shouldn't be initted..");
    m_pd3dDevice = pDev;
  
    if (!m_Particles)
    m_Particles = new CRecyclingArrayDyn<CParticle>(m_iNumParticles);
  
//  SetTexture(m_strTexFilename.c_str()); // trick ourselves into loading the texture

  // restore device objects for each event in this sequence
  for (std::vector<CParticleEvent *>::iterator i = m_Events.begin(); i != m_Events.end(); i++) {
    (*i)->RestoreDeviceObjects(pDev);
  }

  return S_OK;
}

void CParticleEventSequence::RefreshTextureInfo()
{
	if (m_texParticle) return;

	//well, no texture pointer exists so let's refresh it.

	if (!cls_tglo.IsRunning()) return;  //don't refresh it yet
	
	//cache out where the pointer is for speed
	CResourceTex* p_res = (CResourceTex*) cls_tglo.p_manager->get(m_i_tex_id);
	p_res->dx_create(); //load if needed
	m_texParticle = p_res->get_texture();

}


HRESULT CParticleEventSequence::InvalidateDeviceObjects()
{
  //SAFE_DELETE(m_Particles);
  //SAFE_RELEASE(m_texParticle);
    m_texParticle = NULL; //uses Seth's texture cache system
 
  // invalidate device objects for each event in this sequence
  for (std::vector<CParticleEvent *>::iterator i = m_Events.begin(); i != m_Events.end(); i++) {
    (*i)->InvalidateDeviceObjects();
  }

  return S_OK;
}

void CParticleEventSequence::SetTexture(const char *strTexFilename)
{
  //m_strTexFilename = strTexFilename;
 /*
  if (m_pd3dDevice) {
    SAFE_RELEASE(m_texParticle);
    if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice, 
    m_strTexFilename.c_str(), &m_texParticle))) 
	{
    	//failed to load it
		m_texParticle = NULL;
		log_msg("Couldn't find particle texture %s.",strTexFilename);
    }
  }
   */
  
  	if (m_i_tex_id != C_RESOURCE_NONE)
	{
		//release our reference count on the old one
		cls_tglo.p_manager->delete_ref(m_i_tex_id);
        m_i_tex_id = C_RESOURCE_NONE;
	}

	//get or create new texture
	m_i_tex_id = cls_tglo.p_manager->get_dynamic_resource_by_string(C_RESOURCE_TEXTURE, (char*)strTexFilename);
    m_texParticle = NULL;



	}

void CParticleEventSequence::CreateNewParticle(D3DXVECTOR3 m_vPartSysPos)
{
	
  CParticle *part = m_Particles->New();
  static float f_length;

  part->m_fLifetime = m_Lifetime.GetRandomNumInRange();
  static float f_rot_temp;
 if (m_f_rotation == 0)
  {
  
    //no rotation needed
     //Seth changed to work with standard billboarding
    part->m_vPos = m_vPartSysPos + m_vEmitRadius.GetRandomNumInRange();
   // part->m_vDir = m_vSpawnDir.GetRandomNumInRange();
 

   

    //part->m_vPos =  m_vEmitRadius.GetRandomNumInRange();
  }
  //uncomment this to also rotate the starting spot with the end.. I don't think I really need this so why waste
  //the processer power
  
  else
  {
	  //we must be following an ent
   static D3DXVECTOR3 vec_temp;
    vec_temp = m_vEmitRadius.GetRandomNumInRange();
 f_rot_temp = m_f_rotation + get_theta_from_x_and_z(vec_temp.x,  vec_temp.z);
 f_length = -(Slow_Distance_2D(vec_temp.x, vec_temp.z));


  //now rotate it.  very slow
  vec_temp.z = f_length*(float) cos(f_rot_temp);
  vec_temp.x = f_length*(float) sin(f_rot_temp);

  part->m_vPos =  m_vPartSysPos +vec_temp;
  
  }

 std::vector<CParticleEvent *>::iterator i;

  // process any initial events
  for (i = m_Events.begin(); i != m_Events.end() && !(*i)->GetActualTime(); i++) {
    (*i)->DoItToIt(*part);
  }
  //modify the direction to match our rotation
  
  if (m_f_rotation != 0)
  {
 	//convert what we have into a power
	  
	f_length = -(Slow_Distance_2D(part->m_vDir.x, part->m_vDir.z));

  	  //rotate the velocity
     part->m_vDir.z = f_length* (float) cos(f_rot_temp);
     part->m_vDir.x = f_length* (float) sin(f_rot_temp);
  }

  part->m_vDir += this->vec_attached_vel;
  

//  log_msg("New vector created, adding %s to vel", print_vector(vec_attached_vel));
  part->m_CurEvent = i;
	m_iTotalParticleLives++;
}

void CParticleEventSequence::RunEvents(CParticle &part)
{
  // apply any other events to this particle
	std::vector<CParticleEvent *>::iterator i;

  for (i = part.m_CurEvent; 
       i != m_Events.end() && (*i)->GetActualTime() <= part.m_fEventTimer; i++) {
    float oldeventtimer = part.m_fEventTimer;
    (*i)->DoItToIt(part);
    if (part.m_fEventTimer != oldeventtimer)
	{
		std::vector<CParticleEvent *>::iterator RecalcIter;
      // event timer has changed, we need to recalc m_CurEvent.
      for (RecalcIter = m_Events.begin(); 
           RecalcIter != m_Events.end() && (*RecalcIter)->GetActualTime() < part.m_fEventTimer; 
           RecalcIter++);

      // set our main iterator to the recalculated iterator
      // the -1 just compensates for the i++ in the main for loop
      i = RecalcIter-1; 
    }
  }
  part.m_CurEvent = i;
}
void CParticleEventSequence::Update(float fElapsedTime, float fTimeDelta, D3DXVECTOR3 m_vPartSysPos, D3DXVECTOR3 vec_added_vel, bool b_no_new_particles, float f_rotation)
{
  if (!m_Particles) return;

  //6-15-02 added support so particle system can follow the dir of the attached ent
  
  /*
  char st_crap[50];
  sprintf(st_crap, "Rotation is %.3f", f_rotation);
  log_msg(st_crap);
   */
  m_f_rotation = f_rotation; //remember this
   this->vec_attached_vel = vec_added_vel;
   
   //it's already rotated before we get here I think, can't remember
   
   /*
  if (m_f_rotation != 0)
  {
	  //rotate the velocity
     vec_attached_vel.x *= (float) cos(m_f_rotation);
     vec_attached_vel.z *= (float) sin(m_f_rotation);
  }
   */
   // update existing particles
  {
    for (int q=0; q < m_Particles->GetTotalElements(); q++) {
      if (m_Particles->IsAlive(q)) {
        CParticle &part = m_Particles->GetAt(q);

        if (!part.Update(fTimeDelta)) {
          m_Particles->Delete(&part);
        }
        else {
          // apply gravity to this particle.
          part.m_vDir += fTimeDelta * m_vGravity.GetRandomNumInRange();
          // run all the particle's events
          RunEvents(part);
        }
      } // is alive
    } // next particle
  }

  float fEmitRateThisFrame = m_EmitRate.GetRandomNumInRange();
  int iNumNewParts = int(fEmitRateThisFrame * fTimeDelta);
  m_fNumNewPartsExcess += (float)(fEmitRateThisFrame * fTimeDelta)-iNumNewParts;
  
  if (m_fNumNewPartsExcess > 1.0f) {
    iNumNewParts += (int)m_fNumNewPartsExcess;
    m_fNumNewPartsExcess -= (int)m_fNumNewPartsExcess;
  }

	if (m_Loops > 0 && m_iTotalParticleLives+iNumNewParts > m_Loops * m_iNumParticles) {
		iNumNewParts = (m_Loops*m_iNumParticles)-m_iTotalParticleLives;
		if (iNumNewParts <= 0) {
			iNumNewParts = 0;
		}
	}
  for (int q=0; q < iNumNewParts && m_Particles->GetNumFreeElements(); q++) {
    try {
      	if (!b_no_new_particles)
		CreateNewParticle(m_vPartSysPos);
    } catch(...) { q = iNumNewParts; }
  }
    
  
}



void CParticleEventSequence::Render(LPDIRECT3DVERTEXBUFFER8 pVB, int iVBSize, float f_angle_offset)
{
  HRESULT hr;

  m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, GetSrcBlendMode());
  m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, GetDestBlendMode());

  // Set up the vertex buffer to be rendered
  RefreshTextureInfo();
  m_pd3dDevice->SetTexture(0, m_texParticle);
  

  VERTEX_PARTICLE *pVertices;
  DWORD dwNumParticlesToRender = 0;

  // Lock the vertex buffer.  We fill the vertex buffer in small
  // chunks, using D3DLOCK_NOOVERWRITE.  When we are done filling
  // each chunk, we call DrawPrim, and lock the next chunk.  When
  // we run out of space in the vertex buffer, we start over at
  // the beginning, using D3DLOCK_DISCARD.

  int iCurStart = 0;

  
  if(FAILED(hr = pVB->Lock(iCurStart * sizeof(VERTEX_PARTICLE), iVBSize * sizeof(VERTEX_PARTICLE),
      (BYTE **) &pVertices, D3DLOCK_DISCARD)))
  {
      log_error("CParticleEventSequence> %s", DXGetErrorString8(hr));
      return;
  }
  
  
  
   D3DXMATRIX mat_y;
   D3DXMatrixRotationY( &mat_y, cls_tglo.p_camera->getAngleY() - f_angle_offset);

   //ok, what's the story here?  Basically, I was using point sprites and found some problems - the biggest
   //one was when the camera was close them, they wouldn't scale correctly.  So I changed to regular billboards.
   //However, I can't use one fancy view matrix - because that frame can be connected to an object and needs
   //to rotate independently of where we face for the billboard effect.  (also point sprites won't handle
   //scaling in the FVF (d3d bug? driver bug?) so it has to be set globaly by changing the state between
   //each sprite render which is horrible.

  //So, I have to transform each vertex.  :(
  D3DXVECTOR3 vec_temp;

  int i_prim_count = 0;
  for (int q=0; q < m_iNumParticles; q++) 
  {
      if (m_Particles->IsAlive(q))
      {
          CParticle &part = m_Particles->GetAt(q);
          float f_size_mod = part.m_fSize;
          vec_temp = part.m_vPos;

          part.m_vPos = g_vec_null;
          part.m_vPos.x -= f_size_mod;  
          part.m_vPos.y -= f_size_mod;  
          
          
          D3DXVec3TransformCoord(&pVertices[0].position, &part.m_vPos, &mat_y);
          pVertices[0].position += vec_temp;
          pVertices->tu = 0.0f;
          pVertices->tv = 0.0f;
          pVertices->color = (DWORD)part.m_Color;
           
          
          part.m_vPos.x += f_size_mod*2 ; 
          
          D3DXVec3TransformCoord(&pVertices[1].position, &part.m_vPos, &mat_y);
          pVertices[1].position += vec_temp;
          pVertices[1].tu = 1.0f;
          pVertices[1].tv = 0.0f;
          pVertices[1].color = (DWORD)part.m_Color;
        
          part.m_vPos.y += f_size_mod*2; 
          
          D3DXVec3TransformCoord(&pVertices[2].position, &part.m_vPos, &mat_y);
          pVertices[2].position += vec_temp;
          pVertices[2].tu = 1.0f;
          pVertices[2].tv = 1.0f;
          pVertices[2].color = (DWORD)part.m_Color;
          
          
          part.m_vPos.x -= f_size_mod*2; 
 
          D3DXVec3TransformCoord(&pVertices[5].position, &part.m_vPos, &mat_y);
          pVertices[5].position += vec_temp;
          pVertices[5].tu = 0.0f;
          pVertices[5].tv = 1.0f;
          pVertices[5].color = (DWORD)part.m_Color;
       
          memcpy(&pVertices[3], &pVertices[0], sizeof(VERTEX_PARTICLE));
          memcpy(&pVertices[4], &pVertices[2], sizeof(VERTEX_PARTICLE));
  
          
          part.m_vPos = vec_temp;
           
          pVertices+=6;
          i_prim_count += 2; //add two triangles to our list
     
      }
  }  
      pVB->Unlock();
      
  m_pd3dDevice->SetStreamSource( 0, pVB, sizeof(VERTEX_PARTICLE) );
  m_pd3dDevice->SetVertexShader( D3DFVF_PARTICLE );
      
  if (i_prim_count > 0)
  {
  
  //actually render it
 m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, i_prim_count);
  }
  /* 

  
 
  static float size_sprite_buffer[5000];
  static i_sprite_buf_counter;
  i_sprite_buf_counter = 0;



  // Render each particle
  for (int q=0; q < m_iNumParticles; q++) 
  {
    
    // Render each particle a bunch of times to get a blurring effect
    if (m_Particles->IsAlive(q)) {
      CParticle &part = m_Particles->GetAt(q);
    
      pVertices->position = part.m_vPos;
      //log_msg("size is %f",part.m_fSize);
	//  pVertices->pointsize = part.m_fSize;
//	  pVertices->pointsize = 1;
      pVertices->color = (DWORD)part.m_Color;
      size_sprite_buffer[i_sprite_buf_counter] = part.m_fSize;
	  i_sprite_buf_counter++;
	  pVertices++;

      if( ++dwNumParticlesToRender == iVBSize )
	  {
        // Done filling this chunk of the vertex buffer.  Lets unlock and
        // draw this portion so we can begin filling the next chunk.

        pVB->Unlock();



#ifdef _NO_BATCH_POINT

		//to force scaling to work on cards that don't support point size in  the vertex info:
		for (int i=iCurStart; i < iCurStart+ dwNumParticlesToRender; i++)
		{
       	    CParticle &part = m_Particles->GetAt(i);
    	   //log_msg("size is %f",part.m_fSize);

		m_pd3dDevice->SetRenderState(D3DRS_POINTSIZE, FtoDW(size_sprite_buffer[i]));


			
			if(FAILED(hr = m_pd3dDevice->DrawPrimitive( 
          D3DPT_POINTLIST, i, 1)))
			{
		  log_error("CParticleEventSequence> %s", DXGetErrorString8(hr));

	        return;
			}
		}
#else
       if(FAILED(hr = m_pd3dDevice->DrawPrimitive( 
          D3DPT_POINTLIST, iCurStart, dwNumParticlesToRender)))
	        return;
#endif
 
        // Lock the next chunk of the vertex buffer.  If we are at the 
        // end of the vertex buffer, DISCARD the vertex buffer and start
        // at the beginning.  Otherwise, specify NOOVERWRITE, so we can
        // continue filling the VB while the previous chunk is drawing.
        iCurStart = 0;

        if(FAILED(hr = pVB->Lock(0, iVBSize * sizeof(VERTEX_PARTICLE),
		      (BYTE **) &pVertices, D3DLOCK_DISCARD)))
		{
			  log_error("CParticleEventSequence> %s", DXGetErrorString8(hr));

          return;
		}
        dwNumParticlesToRender = 0;
      }
    }
  }

  // Unlock the vertex buffer
  pVB->Unlock();

  // Render any remaining particles
  if( dwNumParticlesToRender )
  {
  
#ifdef _NO_BATCH_POINT

		//to force scaling to work on cards that don't support point size in  the vertex info:
		for (int i=iCurStart; i < iCurStart+ dwNumParticlesToRender; i++)
		{
       	    CParticle &part = m_Particles->GetAt(i);
    	//    log_msg("size is %f",part.m_fSize);

		m_pd3dDevice->SetRenderState(D3DRS_POINTSIZE, FtoDW(size_sprite_buffer[i]));

			
			if(FAILED(hr = m_pd3dDevice->DrawPrimitive( 
          D3DPT_POINTLIST, i, 1)))
			{
			  log_error("CParticleEventSequence> %s", DXGetErrorString8(hr));

	
	        return;
			}
		}
#else
       if(FAILED(hr = m_pd3dDevice->DrawPrimitive( 
          D3DPT_POINTLIST, iCurStart, dwNumParticlesToRender)))
	   {
	   		  log_error("CParticleEventSequence> %s", DXGetErrorString8(hr));

	        return;
	   }
#endif
	  
	  
	//  if(FAILED(hr = m_pd3dDevice->DrawPrimitive( D3DPT_POINTLIST, iCurStart, dwNumParticlesToRender )))
	//	return;
  
  
  
  
  }
  */


  // Reset render states
 }




