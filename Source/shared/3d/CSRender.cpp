#include "CSRender.h"
#include "CSMap.h"
#include "CManager.h"
#include "d3dapp.h"
#include "CCamera.h"

CSRender::CSRender()
{
    set_thing_name("CSRender");
    m_p_cscamera = NULL;
    set_sort(-5);
}


CSRender::~CSRender()
{

}

void CSRender::RenderQuad(float f_x, float f_y, CSTile *p_tile, float f_width, float f_height)
{
       float z = 0.0f; //depth
        D3DCOLOR dw_color = D3DCOLOR_ARGB(255,255,255,255);

        float left = 0;
        float right = f_width;
        float top = 0;
        float bottom = f_height;

        float f_offset = 0.0f;


        CSVERTEX verts[4];
        
        verts[0].vec_pos = D3DXVECTOR3(left-f_offset,  top-f_offset,    z);
        verts[0].tu = 0.0f;
        verts[0].tv = 0.0f;
        
        verts[1].vec_pos = D3DXVECTOR3(right+f_offset, top-f_offset,    z);
        verts[1].tu = 1.0f;
        verts[1].tv = 0.0f;
        
        verts[2].vec_pos = D3DXVECTOR3(right+f_offset, bottom+f_offset,  z);
        verts[2].tu = 1.0f;
        verts[2].tv = 1.0f;
        
        verts[3].vec_pos = D3DXVECTOR3(left-f_offset,  bottom+f_offset, z);
        verts[3].tu = 0.0f;
        verts[3].tv = 1.0f;


         for (int i=0; i < 4; i++)
        {
       //     verts[i].rhw = 10;  //this is a little funky, but it works and we don't really need Z buffer placement
            verts[i].dw_color = dw_color;
            
            //apply the transform matrix to these 4 points for special effects and translation
          //  D3DXVec3TransformCoord(&verts[i].vec_pos, &verts[i].vec_pos, p_matrix);
          
            verts[i].vec_pos.x += f_x;
            
            
            verts[i].vec_pos.y += (cls_tglo.p_csmap->GetY()-f_y);


        }
     
         //set the right tex
         CResourceTex *p_tex = ((CResourceTex*)cls_tglo.p_manager->get(p_tile->i_resource_index));
         if ( p_tex->get_texture() == NULL)
         {
             //add ref and load it
             p_tex->add_ref();
             p_tex->dx_create();
         }
         
         

      cls_tglo.p_d3dDevice->SetTexture( 0,p_tex->get_texture());
        cls_tglo.p_d3dDevice->SetVertexShader(D3DFVF_CSVERTEX);
          
         //actually draw it
      cls_tglo.p_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,verts,sizeof(CSVERTEX));




}


void CSRender::dx_draw()
{
   
    //set identity matrix

  D3DXMATRIX viewmat;
  D3DXMatrixIdentity(&viewmat);
  cls_tglo.p_d3dDevice->SetTransform( D3DTS_WORLD, &viewmat );


    //draw the map out
    if (!cls_tglo.p_csmap) return;


//           cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
 //         cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
   
        
      cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
   
       // draw the rectangle
       // cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
       // cls_tglo.p_d3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
       // cls_tglo.p_d3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_LIGHTING,  FALSE );
      /*  
        // Enable alpha testing (skips pixels with less than a certain alpha.)
        if( cls_tglo.p_d3dapp->m_d3dCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL )
        {
            cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
            cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x00 );
            cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
        }
        */
   //let's draw a square for every map tile
   int i_starting_x = 0;
   int i_width = 30;
  


    i_starting_x = int(cls_tglo.p_camera->get_pos_x())-(i_width/2);

    //force the ranges
    force_range(&i_starting_x, 0, cls_tglo.p_csmap->GetX());

    if (i_starting_x + i_width > cls_tglo.p_csmap->GetX())
    {
        i_width =  cls_tglo.p_csmap->GetX() - i_starting_x;
    }

    for (int x = i_starting_x; x < i_starting_x+i_width; x++)
    {                                                                     
       for (int y = 0; y < cls_tglo.p_csmap->GetY(); y++)          
       {

           if (cls_tglo.p_csmap->GetTile(x,y,1)->i_resource_index != 1)
           {
           RenderQuad(x*1,y*1, cls_tglo.p_csmap->GetTile(x,y,1),1,1);
           } else
           {
           
           RenderQuad(x*1,y*1, cls_tglo.p_csmap->GetTile(x,y,0),1,1);
           }
       }
    }
   cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
         cls_tglo.p_d3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
        
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,    FALSE );
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE );
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_LIGHTING,    TRUE );
        cls_tglo.p_d3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );

}