#pragma once

#include "all.h"
#include "CThing_globals.h"
#include "CResource.h"
#include "assert.h"
#include "CManager.h"
#include "CTextParse.h"

#include "..\..\cal3d\cal3d\src\cal3d\cal3d.h"

//handles textures

class CResourceCalCore: public CResource
{
private:
  float m_scale;
public:

 CalCoreModel m_calCoreModel;
bool CResourceCalCore::parseModelConfiguration(char st_file[256]);

 virtual void dx_create();
 virtual void dx_restore();
 virtual void dx_invalidate();
 virtual void dx_kill();

 CResourceCalCore();
 virtual ~CResourceCalCore();
};	 


