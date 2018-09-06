/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


//modified from code by Mason Mckusky.


#include "CParticleEmitter.h"
#include "CParticleEmitterTokenizer.h"
#include "CEnt.h"
#include "d3dapp.h"
#include "..\\CScriptEngine.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int g_particle_system_ref_count = 0;

LPDIRECT3DVERTEXBUFFER8 m_vbParticles; //share this with all particle systems

CParticleEmitter::CParticleEmitter()
{
 
	//share this between all particle systems

	if (g_particle_system_ref_count == 0)
	{
	  m_vbParticles = NULL;	  
	} else
	{
		//it's already initted, don't fool with it
	}
	
	g_particle_system_ref_count++;


  // initialize misc. other things
  m_pd3dDevice = NULL;
  set_thing_name("Emitter");
  set_type(C_THING_PARTICLE_SYSTEM);
  set_sort(-1);  //on top of everything
  b_apply_momentum = false;
  b_no_new_particles = false;  
  b_frame_follow = false; //if true, system will follow around the thing without leaving a trail
  // default particle system configuration - boring!							   dx_
  b_translate_asap = false; //we will do our own update
  SetComplexRotation(false);
  Init();
  this->translateMovement();
  SetMomentumMod(5000); //full momemtum, set to 4000 for like smoke that should have partial momentum
 }


void CParticleEmitter::Init()
{
   set_sort(100000);  //on top of everything
 
//	InvalidateDeviceObjects();

	m_strName = "";
  for (std::vector<CParticleEventSequence *>::iterator i = m_Sequences.begin(); i != m_Sequences.end(); i++) {
    delete (*i);
  }
  
  m_Sequences.clear();

  SetPosRange(CMinMax<D3DXVECTOR3>(D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f)));
  
  SetVBSize(6000);   //6 verts for each 1 particle, so 1000 max
	Stop();
}


CParticleEmitter::~CParticleEmitter()
{
  Init();

  g_particle_system_ref_count--;

  if (g_particle_system_ref_count == 0)
  {
	  SAFE_RELEASE(m_vbParticles);
  }
}

void CParticleEmitter::think()
{
 	//we do our own custom follow and set matrix code here, for a little extra speed

	
    CVisual::think();
	
	if (p_visual_follow && b_apply_momentum)
	{
		Update(cls_tglo.time(), cls_tglo.time(), ((CEnt*)p_visual_follow)->get_velocity()*cls_tglo.time()*m_f_momentum_mod);
 	}
	else
	{
		Update(cls_tglo.time(), cls_tglo.time(), D3DXVECTOR3(0,0,0));
 	}

 


}


void CParticleEmitter::load_and_compile_particle_system(char st_file_name[256])
{

    int i_size;
    char *p_buff = GetApp()->GetCFL()->getFile(st_file_name, i_size);
   
    if (!p_buff)
    {
       log_error("Couldn't open %s from regular or packed file.", st_file_name);
       return;
    }

    Compile(p_buff);
	Start();
    SAFE_DELETE_ARRAY(p_buff);
}


CParticleEvent *CParticleEmitter::EventFactory(std::string EventName)
{
  CParticleEvent *event = NULL;

  if (EventName.find("SIZE") != std::string::npos) event = new CParticleEvent_Size();
  else if (EventName.find("EVENTTIMER") != std::string::npos) event = new CParticleEvent_EventTimer();
  else if (EventName.find("REDCOLOR") != std::string::npos) event = new CParticleEvent_RedColor();
  else if (EventName.find("GREENCOLOR") != std::string::npos) event = new CParticleEvent_GreenColor();
  else if (EventName.find("BLUECOLOR") != std::string::npos) event = new CParticleEvent_BlueColor();
  else if (EventName.find("ALPHA") != std::string::npos) event = new CParticleEvent_Alpha();
  else if (EventName.find("COLOR") != std::string::npos) event = new CParticleEvent_Color();
  else if (EventName.find("VELOCITYX") != std::string::npos) event = new CParticleEvent_VelocityX();
  else if (EventName.find("VELOCITYY") != std::string::npos) event = new CParticleEvent_VelocityY();
  else if (EventName.find("VELOCITYZ") != std::string::npos) event = new CParticleEvent_VelocityZ();
  else if (EventName.find("VELOCITY") != std::string::npos) event = new CParticleEvent_Velocity();

  return(event);
}

bool CParticleEmitter::ProcessEventSequenceBlock(CParticleEventSequence &seq,
                                                 std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                                 std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  bool StartedProcessingEvents = false;

  // move past the event sequence keyword...
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

  // next token should be the name of the sequence...
  if (TokenIter->m_Type != Quote) throw("Must name particle sequence block!");
  seq.SetName(TokenIter->m_strValue);
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

  // next token should be opening brace...
  if (TokenIter->m_Type != OpenBrace) throw("Expected opening brace for particle sequence block!");
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

  while (TokenIter->m_Type != CloseBrace) {

    CParticleEmitterToken savedtoken;
    savedtoken = *TokenIter;

    // the first token here can be a SysNumberProperty, SysAlphaBlendModeProperty, SysVectorProperty,
    // or an EventSequence.
    switch (TokenIter->m_Type) {
      case SeqNumericProp:
      {
        if (StartedProcessingEvents) throw("Cannot specify any sequence properties after specifying events.");
        CMinMax<float> number;
        // the next 2 tokens should be an equals, and a number.
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        CParticleEmitterTokenizer::ProcessNumber(number, TokenIter, EndIter);

        if (savedtoken.IsEmitRate()) { seq.SetEmitRate(number); }
        else if (savedtoken.IsLifeTime()) { seq.SetLifetime(number); }
        else if (savedtoken.IsNumParticles()) { seq.SetNumParticles(int(number.m_Max)); }
				else if (savedtoken.IsLoops()) { seq.SetLoops(int(number.m_Max)); }
        else {
          throw("Unknown sequence numeric property!");
        }
      }
      break;

      case SeqVectorProp:
      {
        if (StartedProcessingEvents) throw("Cannot specify any sequence properties after specifying events.");
        CMinMax<D3DXVECTOR3> v;
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        CParticleEmitterTokenizer::ProcessVector(v, TokenIter, EndIter);

        if (savedtoken.IsSpawnDir())        { seq.SetSpawnDir(v); }
        else if (savedtoken.IsEmitRadius()) { seq.SetEmitRadius(v); }
        else if (savedtoken.IsGravity())    { seq.SetGravity(v); }
        else {
          throw("Unknown sequence vector property!");
        }
      }
      break;

      case SeqAlphaBlendModeProp:
      {
        if (StartedProcessingEvents) throw("Cannot specify any sequence properties after specifying events.");
        int alphablendmode;
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        CParticleEmitterTokenizer::ProcessAlphaBlendMode(alphablendmode, TokenIter, EndIter);

        if (savedtoken.IsSrcBlendMode())       { seq.SetSrcBlendMode(alphablendmode); }
        else if (savedtoken.IsDestBlendMode()) { seq.SetDestBlendMode(alphablendmode); }
        else {
          throw("Unknown sequence alpha blending mode property!");
        }
      }
      break;

      case KeywordTexture:
      {
        // TODO
        
        if (StartedProcessingEvents) throw("Cannot specify any sequence properties after specifying events.");
        CMinMax<D3DXVECTOR3> v;
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 

        if (TokenIter->m_Type != Quote) throw("Expected filename after texture sequence property.");

        seq.SetTexture(RemoveQuotes(TokenIter->m_strValue).c_str());
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 

      }
      break;

      case KeywordFade: case KeywordAt: case KeywordInitial: case KeywordFinal:
      {
        StartedProcessingEvents = true;

        bool IsFade = false;
        CMinMax<float> TimeRange;

        // parse the time range section of the event line
        CParticleEmitterTokenizer::ProcessTime(TimeRange, IsFade, 0, seq.GetLifetime().m_Max, TokenIter, EndIter);

        if (TokenIter->m_Type != ParticleNumericProp &&  TokenIter->m_Type != ParticleVectorProp &&
            TokenIter->m_Type != ParticleColorProp) 
          throw("Expecting particle property after time specifier!");

        CParticleEvent * NewEvent = NULL;
        try {
          // create the appropriate event
          NewEvent = EventFactory(TokenIter->m_strValue);

          if (!NewEvent) throw("Unknown event type, or there was an error creating this event.");

          // let the event parse the rest of its properties from the token stream.
          if (IsFade && !NewEvent->FadeAllowed()) throw("Fading is not supported on this event.");
          NewEvent->ProcessTokenStream(TokenIter, EndIter);
          NewEvent->SetTimeRange(TimeRange);
          NewEvent->SetFade(IsFade);
          seq.m_Events.push_back(NewEvent);

        }
        catch(char *e) {
          SAFE_DELETE(NewEvent);
          throw(e);
        }
        catch(...) {
          SAFE_DELETE(NewEvent);
          throw("Unhandled exception creating event!");
        }
      }
      break;

      default: 
      {
        char buf[256]; _snprintf(buf, sizeof(buf), "Unexpected \"%s\" in Sequence Block!", TokenIter->m_strValue.c_str());
        throw(&buf[0]);
      }
      break;
    }
  }
  seq.NailDownRandomTimes();
  seq.SortEvents();
  seq.CreateFadeLists();
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
  return(true);
}

bool CParticleEmitter::ProcessParticleSystemBlock(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                                  std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  bool StartedProcessingSequences = false;
  // move past the opening brace...
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

  while (TokenIter->m_Type != CloseBrace) {
    

    CParticleEmitterToken savedtoken;
    savedtoken = *TokenIter;

    // the first token here can be a SysNumberProperty, SysAlphaBlendModeProperty, SysVectorProperty,
    // or an EventSequence.
    switch (TokenIter->m_Type) {
      
      case SysVectorProp:
      {
        if (StartedProcessingSequences) throw("Cannot specify any particle system properties after specifying sequences.");
        CMinMax<D3DXVECTOR3> v;
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        CParticleEmitterTokenizer::ProcessVector(v, TokenIter, EndIter);
        if (savedtoken.IsPosition()) {
          m_vPosRange = v;
        }
        else {
                log_msg("Unknown particle system property!");
  throw("Unknown particle system property!");
        }
      }
      break;

      case KeywordEventSequence:
      {
        StartedProcessingSequences = true;
        CParticleEventSequence *newseq = new CParticleEventSequence;
        m_Sequences.push_back(newseq);
        CParticleEventSequence *seq = *(m_Sequences.end()-1);
        seq->Reset();

        if (!ProcessEventSequenceBlock(*seq, TokenIter, EndIter)) {
          delete seq;
          m_Sequences.pop_back();
        }
      }
      break;

      default: 
      {
        char buf[256]; _snprintf(buf, sizeof(buf), "Unexpected \"%s\" in Particle System Block!", TokenIter->m_strValue.c_str());
        log_msg(buf);
		throw(&buf[0]);
      }
      break;
    }
  }
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
  return(true);
}

bool CParticleEmitter::Compile(const char *strScript)
{
  m_strLastError = "0 error(s), you're good to go!";

  try {
    CParticleEmitterTokenizer tokenizer;
    InvalidateDeviceObjects();
                             
    Init();

    // parse the character string into tokens.
    tokenizer.Tokenize(strScript);

    std::vector<CParticleEmitterToken>::iterator TokenIter = tokenizer.m_TokenVector.begin();
    std::vector<CParticleEmitterToken>::iterator EndIter = tokenizer.m_TokenVector.end();

    if (tokenizer.m_TokenVector.size() < 2) throw("This script is too small to be valid.");

    // the first three tokens out of the gate should be
    // ParticleSystem, followed by a name and version number, then
    // an open brace.
    if (TokenIter->m_Type != KeywordParticleSystem) { throw("First word must be ParticleSystem"); }
    CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
    if (TokenIter->m_Type != Quote) { throw("Must name particle system"); }
    m_strName = RemoveQuotes(TokenIter->m_strValue);
    CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

    if (TokenIter->m_Type != RealNumber) { throw("Must have version number"); }
    CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
    if (TokenIter->m_Type != OpenBrace) { throw("Missing opening brace for ParticleSystem block"); }

    ProcessParticleSystemBlock(TokenIter, EndIter);

  } catch(char *e) {
    m_strLastError = e; Init(); log_error(m_strLastError.c_str()); return(false);
  } catch(const char *e) {
    m_strLastError = e; Init(); log_error(m_strLastError.c_str()); return(false);
  } catch (...) {
    m_strLastError = "Unhandled exception in CParticleEmitter::Compile.";
    log_msg(m_strLastError.c_str());
	OutputDebugString(m_strLastError.c_str());
    Init();
    return(false);
  }

  // do misc. processing and calcuations
  {
    m_vPos = m_vPosRange.GetRandomNumInRange();
  }

  RestoreDeviceObjects(m_pd3dDevice);
  return(true);
}
void CParticleEmitter::dx_create()
{

 	RestoreDeviceObjects(cls_tglo.p_d3dDevice);

}

void CParticleEmitter::dx_draw()
{

	this->Render();
}
void CParticleEmitter::dx_restore()
{
	RestoreDeviceObjects(cls_tglo.p_d3dDevice);
}


HRESULT CParticleEmitter::RestoreDeviceObjects(LPDIRECT3DDEVICE8 pDev)
{
  
  HRESULT hr;
  if (!pDev) return S_OK; // nothing to do!
  m_pd3dDevice = pDev;
  
  if (!m_vbParticles)
  {

  // create vertex buffer
  if(FAILED(hr = m_pd3dDevice->CreateVertexBuffer(m_iVBSize * sizeof(VERTEX_PARTICLE), 
    D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
    D3DFVF_PARTICLE, D3DPOOL_DEFAULT, &m_vbParticles)))
  {
  	  log_error("Error creating particle VB");
	  return(hr);
  }
  }



  // restore device objects for each event sequence
  for (std::vector<CParticleEventSequence *>::iterator i = m_Sequences.begin(); i != m_Sequences.end(); i++) {
    (*i)->RestoreDeviceObjects(pDev);
  }
  
  Start();

  return S_OK;	
}

void CParticleEmitter::dx_kill()
{
  
	//all done
	InvalidateDeviceObjects();
}


void CParticleEmitter::dx_invalidate()
{
 InvalidateDeviceObjects();

}

void CParticleEmitter::InvalidateDeviceObjects()
{
  // invalidate device objects for each event sequence
  for (std::vector<CParticleEventSequence *>::iterator i = m_Sequences.begin(); i != m_Sequences.end(); i++) {
    (*i)->InvalidateDeviceObjects();
  }
  
  SAFE_RELEASE(m_vbParticles);
}

void CParticleEmitter::Update(float fElapsedTime, float fTimeDelta, D3DXVECTOR3 vec_vel)
{
	
	if (IsRunning()) {
	
    //    alpha_sort();

        for (std::vector<CParticleEventSequence *>::iterator i = m_Sequences.begin(); i != m_Sequences.end(); i++) {
//			(*i)->Update(fElapsedTime, fTimeDelta, m_vPos);
//SETH, changed	
		
			
		if (b_frame_follow)
		{

		(*i)->Update(fElapsedTime, fTimeDelta, m_vPos, vec_vel, b_no_new_particles, 0);

		}	else
		{
			if (p_visual_follow && m_b_complex_rotation_math)
		
                //feed in  the matrix too
			(*i)->Update(fElapsedTime, fTimeDelta, this->m_vObjectPosition+m_vPos, vec_vel, b_no_new_particles, p_visual_follow->getAngleY());
			else
			(*i)->Update(fElapsedTime, fTimeDelta, this->m_vObjectPosition+m_vPos, vec_vel, b_no_new_particles, 0);

		}
		}
	}
}


HRESULT CParticleEmitter::Render()
{									
    if (IsRunning())
    {
        
        if (b_frame_follow && p_visual_follow)
        {
            //move us to the correct position
            this->setAngleY(p_visual_follow->getAngleY());
            set_world_transform();
        } 
        else
        {
            set_world_transform();
        }
        
        static bool b_fog_save;
        
        cls_tglo.p_d3dDevice->GetRenderState(D3DRS_FOGENABLE, (DWORD*)&b_fog_save);
        cls_tglo.p_d3dDevice->SetRenderState(D3DRS_FOGENABLE, false);
        
        m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,  FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        
        if( cls_tglo.p_d3dapp->m_d3dCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL )
        {
            cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
            cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x01 );
            cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
        }
        
        m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_NONE);
        
        for (std::vector<CParticleEventSequence *>::iterator i = m_Sequences.begin(); i != m_Sequences.end(); i++) 
        {
            (*i)->Render(m_vbParticles, m_iVBSize, getAngleY());
        }
        
        m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW);
        
        m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD  );
        m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,  TRUE );
        m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
        
        //    cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
        //  cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
        cls_tglo.p_d3dDevice->SetRenderState(D3DRS_FOGENABLE, b_fog_save);
        
    }
    
    CThing::think();
    
    
    return S_OK;
}

