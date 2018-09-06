
//******************************************
// copyright 2004	Seth A. Robinson

// This file is NOT public domain and may not be shared, distributed or used
// without specific permission.

// seth@rtsoft.com - www.rtsoft.com
//******************************************


//This thing changes a section of lawn to mowed.  But, more interestingly, it does it over a period of time.


#pragma once

#include "3d\\CThing.h"
#include "CTimer.h"
#include "sound\\ISoundManager.h"
#include "PrimeSearch.h"
#include "3d\\CThing_globals.h"


class CParticleEmitter; //forward for speed

using namespace std;

class CWeedKiller : public CThing
{

public:

  CWeedKiller();
  virtual ~CWeedKiller(); //clean up

  virtual void think();
  void Init(D3DXVECTOR3 *p_vec_pos, int i_range); //init this right away

private:

    bool MowArea(int i_num);

    CTimer m_timer_think;
    PrimeSearch m_prime;
    int m_i_target_x, m_i_target_y;
    int m_i_size; //how big of an area we're going to do
    int m_i_mow_zone;
    int m_i_mowed_tile_graphic;
};
