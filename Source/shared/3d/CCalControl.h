//the skeletal based character animation system

#pragma once
#include "CThing_globals.h"
#include "..\..\cal3d\cal3d\src\cal3d\cal3d.h"
#include "CVisualMulti.h"
#include "CManager.h"
#include "CResourceCalRender.h"



class CCalControl
{
private:
  int m_currentAnimationId;
  float m_leftAnimationTime;
  float m_blendTime;
  float m_lodLevel;
   unsigned int m_lastTick;
  int i_rend_id; //remember the resource ID of the Cal renderer

public:
	CCalControl();
	virtual ~CCalControl();

	CalModel m_calModel;
    CVisualMulti *p_vmulti;
	CResourceCalRender *p_render;
	bool CCalControl::init_cal_instance(CVisualMulti *p_vmulti_new, char st_cal_name[]);
	void CCalControl::draw();
	void CCalControl::init_and_attach_meshes();
	void CCalControl::update();
};