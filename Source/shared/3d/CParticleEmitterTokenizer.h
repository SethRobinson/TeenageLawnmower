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

#include "MinMax.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonFuncs.h"
#include "MinMax.h"
#include <vector>

typedef struct {
  char name[25];
  int mode;
} BLENDINGMODE;

enum TokenType {
  RealNumber = 1, 
  Equals, Comma, OpenParen, CloseParen, OpenBrace, CloseBrace, Quote, 

  KeywordParticleSystem, KeywordEventSequence,
  KeywordRandom, KeywordXYZ, KeywordColor, KeywordTexture, AlphaBlendMode,

  // system property tokens
  SysVectorProp, SeqAlphaBlendModeProp,
  
  // event time tokens
  KeywordFade, KeywordSo, KeywordAt, KeywordInitial, KeywordFinal,
  
  // particle properties (event types)
  ParticleNumericProp, ParticleVectorProp, ParticleColorProp,
  
  // event sequence props
  SeqNumericProp, SeqVectorProp, 

  PSUnknown

};

class CParticleEmitterToken { 
public:
  std::string m_strValue;
  enum TokenType m_Type;

  bool IsEmitRate(void)      { return(m_strValue.find("EMITRATE")        != std::string::npos); }
  bool IsPosition(void)      { return(m_strValue.find("POSITION")        != std::string::npos); }
  bool IsSpawnDir(void)      { return(m_strValue.find("SPAWNDIR")        != std::string::npos); }
  bool IsEmitRadius(void)    { return(m_strValue.find("EMITRADIUS")      != std::string::npos); }
  bool IsLifeTime(void)      { return(m_strValue.find("LIFETIME")        != std::string::npos); }
  bool IsNumParticles(void)  { return(m_strValue.find("NUMPARTICLES")    != std::string::npos); }
	bool IsLoops(void)         { return(m_strValue.find("LOOPS")           != std::string::npos); }
  bool IsGravity(void)       { return(m_strValue.find("GRAVITY")         != std::string::npos); }
  bool IsSrcBlendMode(void)  { return(m_strValue.find("SOURCEBLENDMODE") != std::string::npos); }
  bool IsDestBlendMode(void) { return(m_strValue.find("DESTBLENDMODE")   != std::string::npos); }

  std::string TypeAsString(void) {
    switch(m_Type) {
      case RealNumber: return("RealNumber");
      case Equals: return("Equals");
      case Comma: return("Comma");
      case OpenParen: return("OpenParen");
      case CloseParen: return("CloseParen");
      case OpenBrace: return("OpenBrace");
      case CloseBrace: return("CloseBrace");
      case Quote: return("Quote");

      case KeywordParticleSystem: return("KeywordParticleSystem");
      case KeywordEventSequence: return("KeywordEventSequence");
      case KeywordRandom: return("KeywordRandom");
      case KeywordXYZ: return("KeywordXYZ");
      case KeywordColor: return("KeywordColor");
      case KeywordTexture: return("KeywordTexture");

      case SysVectorProp: return("SysVectorProp");
      case SeqAlphaBlendModeProp: return("SeqAlphaBlendModeProp");
      case AlphaBlendMode: return("AlphaBlendMode");

      case KeywordFade: return("KeywordFade");
      case KeywordSo: return("KeywordSo");
      case KeywordAt: return("KeywordAt");
      case KeywordInitial: return("KeywordInitial");
      case KeywordFinal: return("KeywordFinal");

      case ParticleNumericProp: return("ParticleNumericProp");
      case ParticleVectorProp: return("ParticleVectorProp");
      case ParticleColorProp: return("ParticleColorProp");

      case SeqNumericProp: return("SeqNumericProp");
      case SeqVectorProp: return("SeqVectorProp");
    }
    return("**Unknown!**");
  }
};

class CParticleEmitterTokenizer {
public:
  static const int NUMBLENDINGMODES;
  static const BLENDINGMODE m_BlendingModes[11];

  std::vector<CParticleEmitterToken> m_TokenVector;
  
  void Tokenize(const char *str);

  static bool ProcessAlphaBlendMode(int &alphablendmode, 
    std::vector<CParticleEmitterToken>::iterator &TokenIter,
    std::vector<CParticleEmitterToken>::iterator &EndIter);

  static bool ProcessColor(CMinMax<D3DXCOLOR> &c, 
    std::vector<CParticleEmitterToken>::iterator &TokenIter,
    std::vector<CParticleEmitterToken>::iterator &EndIter);

  static bool ProcessTime(CMinMax<float> &TimeRange, bool &IsFade, 
    const float InitialTime, const float FinalTime,
    std::vector<CParticleEmitterToken>::iterator &TokenIter, 
    std::vector<CParticleEmitterToken>::iterator &EndIter);

  static bool ProcessVector(CMinMax<D3DXVECTOR3> &v, 
    std::vector<CParticleEmitterToken>::iterator &TokenIter, 
    std::vector<CParticleEmitterToken>::iterator &EndIter);

  static bool ProcessNumber(CMinMax<float> &number, 
    std::vector<CParticleEmitterToken>::iterator &TokenIter,
    std::vector<CParticleEmitterToken>::iterator &EndIter);

  static void NextToken(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
    std::vector<CParticleEmitterToken>::iterator &EndIter) {
    if (TokenIter == EndIter) throw("Unexpected end-of-file.");
    TokenIter++;
  }

private:
  void AddToken(CParticleEmitterToken token);
  void DetermineTokenType(CParticleEmitterToken &token);

};

