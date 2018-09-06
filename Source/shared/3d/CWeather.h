//Loads the sky and handles other atmosphere related effects (Seth A. Robinson '02)

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

class CVisualSky; //sphere sky
class CSkyBox; //skybox style

#include "CThing.h"
#include "d3d8types.h"
#include "CWeatherSetting.h"
#include "..\CTimer.h"

class CWeather : public CThing
{
public:
	void CWeather::Init();
	CWeather();
	~CWeather();
	 virtual void dx_restore();
	 virtual void think();
    void CWeather::UpdateFromWeatherSetting(CWeatherSetting *p_weather, bool b_update_script);
	bool CWeather::LoadWeather(char *p_file);
	void CWeather::GetWeatherSettings(CWeatherSetting * p_out_weather);
    void SetRandomFlash(int i_time);
     bool CWeather::LoadAndFadeToWeather(char *p_file, int i_delay, float f_lerp);

private:

	void CWeather::UpdateLights();
    void CWeather::ProcessDelayToSetToHoldTimer();
    void CWeather::SetDelayToSetToHold(int i_time);
    void CWeather::ProcessFade();
    void CWeather::KillWeatherScriptIfActive();

    CVisualSky *m_p_sky; //our skydome
    CSkyBox *m_p_skybox; //if not null, we're using a 6 sided skybox
	D3DLIGHT8 m_light; //light 0, always our main light.
	CWeatherSetting m_weather;
    bool m_b_set_to_hold_active;
    CTimer m_timer_set_to_hold_wait;
    CWeatherSetting m_weather_hold; //for effects
    bool m_b_fade_to_hold; //true if we're fading the weather to what is in hold
    float m_f_fade_lerp; //hold quickly we fade.
    int m_i_script_ent; //optional weather script item
};