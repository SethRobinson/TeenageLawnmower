#include "CThing.h"
#include "CCamera.h"
#include "CTerrain.h"
#include "CThing_util.h"

class CFloatCam : public CThing
{

public:

  CFloatCam(); //init   
  virtual void think();

};