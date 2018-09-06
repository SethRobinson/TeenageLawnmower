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


#include "CManager.h" 

//handles textures

#define D3DFVF_TLVERTEX D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1

typedef struct _D3DTLVERTEX {
    D3DXVECTOR3 vec_pos; //position
    float rhw; /* Reciprocal of homogeneous w */
    D3DCOLOR color; /* Vertex color */
    float tu; /* Texture coordinates */
    float tv;
    _D3DTLVERTEX() { }
  
} D3DTLVERTEX, *LPD3DTLVERTEX;

 



// Custom vertex type for the trees
struct TREEVERTEX
{
    D3DXVECTOR3 p;      // Vertex position
    D3DXVECTOR3 n;      // The normal
 	DWORD       color;  // Vertex color
    FLOAT       tu, tv; // Vertex texture coordinates
};

#define D3DFVF_TREEVERTEX (D3DFVF_XYZ| D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)




class CResourceSprite: public CResource
{
    
public:
  
    CResourceSprite();
    virtual ~CResourceSprite();
    
    
    D3DXVECTOR3 vec_bb_min; //bounding box
    D3DXVECTOR3 vec_bb_max;
    D3DXVECTOR3         m_vObjectCenter;        // Center of bounding sphere of object
    FLOAT               m_fObjectRadius;        // Radius of bounding sphere of object
    
    TREEVERTEX  v[4];           // Four corners of billboard quad
    
    int i_tex_id; //texture resource id we're using
    char st_texture[256];
    virtual void dx_create();
    virtual void dx_restore();
    virtual void dx_invalidate();
    virtual void dx_draw();

    void RefreshTexture();
    void setup_sprite(const char p_st_tex[]);
    bool alpha_test(float f_x, float f_y);
    void dx_draw_blit(D3DXMATRIX *p_matrix, int e_centering, DWORD dw_color,
        int i_meter_mode, float f_meter_amount, float f_scale_x, float f_scale_y);
    
    LPDIRECT3DVERTEXBUFFER8 m_pTreeVB;  // Vertex buffer for rendering a tree

    bool alpha_test_2d(D3DXMATRIX *p_matrix, int e_centering, D3DXVECTOR3 *p_hit);
    bool HasAlpha(); //returns true if image is dxt3 or 16 bit with alpha
    int setup_sprite_create_new(D3DXVECTOR2 pt_size, DWORD dw_color);
    
    void SetMipMap(int i_mip_map){m_i_mip_map = i_mip_map;}

private:

	int m_i_mip_map; //how many mip maps we want to use or 0 for default
};	 

