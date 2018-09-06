#include "CThing.h"

class CSCamera : public CThing
{

public:
    CSCamera();
     virtual void think();
     virtual void dx_restore();
     void SetupOrthogonalView();

private:


};