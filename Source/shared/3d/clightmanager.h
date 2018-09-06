#pragma once

#include "CThing.h"
#include "CThing_util.h"
#include "..\\CTimer.h"

const int C_LIGHT_START = 1; //start on light 1, leaves light 0 free for other things
const int C_LIGHT_END = 32; //allow up to 7 dynamic lights

#include <vector>
using namespace std;


class CLightInfo
{
public:
    D3DXVECTOR3 vec_pos, vec_color;
    float f_power, f_trigger_distance;
    bool b_active;
};

typedef vector<CLightInfo> vec_light;

class CLightManager : public CThing
{
public:
   CLightManager();
   ~CLightManager();

   int AddLight(D3DXVECTOR3 * p_vec_pos, D3DXVECTOR3 * p_vec_color, float f_power, float f_trigger_distance);
   void RemoveLight(int i_light);
   void CLightManager::UpdateLights();
   virtual void CLightManager::think();

private:
   void UpdateD3DLight(CLightInfo & light_info, int i_d3d_light);
    
    CTimer m_timer_update;
    vec_light m_vec_light; //array of stl vector for the lights we want to track
};
