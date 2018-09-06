#include "CThing.h"
#include "CCamera.h"
#include "CTerrain.h"
#include "CThing_util.h"

class CControlTopView : public CThing
{

public:

  CControlTopView(); //init   
  virtual CControlTopView::~CControlTopView();
  virtual void think();
 
};