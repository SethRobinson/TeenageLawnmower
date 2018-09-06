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

#include "CPic.h"


class CPicMeter: public CPic
{
public:
    CPicMeter();

    void InitMeter(int i_meter_style);
    void SetMeterLerp(float f_lerp){m_f_meter_lerp = f_lerp;}
    virtual void think();
    void SetMeter(float f_percent){m_f_cur = f_percent;};
    void SetMeterNoLerp(float f_percent);
    virtual void CPicMeter::dx_draw();
 
    inline float GetMeter(){return m_f_cur;};
    void SetMeterMod(float f_mod);
 
private:
 
    int m_i_meter_style;
    float m_f_cur; //the real current position, use to for any life checks, etc.
    float m_f_visual_cur; //visually what our bar is up to
    float m_f_meter_lerp; //how fast we move the meter

  
};