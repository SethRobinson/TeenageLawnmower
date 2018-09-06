#pragma once
#include "CThing.h"
#include "CThing_globals.h"    

class CSTile;
class CSCamera;
// Custom vertex type for the trees
struct CSVERTEX
{
    D3DXVECTOR3 vec_pos;      // Vertex position
    D3DXVECTOR3 vec_normal;      // The normal
 	DWORD       dw_color;  // Vertex color
    FLOAT       tu, tv; // Vertex texture coordinates
};

#define D3DFVF_CSVERTEX (D3DFVF_XYZ| D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)




class CSRender: public CThing
{
public:
    CSRender();
    virtual ~CSRender();

    virtual void dx_draw();
    void RenderQuad(float f_x, float f_y, CSTile *p_tile, float f_width, float f_height);
    void SetCSCameraPointer(CSCamera *p_new){m_p_cscamera = p_new;}

private:

    CSCamera *m_p_cscamera;

};