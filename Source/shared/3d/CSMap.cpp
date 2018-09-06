#include "csmap.h"
#include "..\all.h"
#include "cstile.h"


CSMap::CSMap()
{
    memset(this, 0, sizeof(CSMap));
    InitMap(260,30,2);

   
    log_msg("Map initted.");
}

CSMap::~CSMap()
{
    KillMap();
}

bool CSMap::IsHardWorld(float f_x, float f_y, float f_padding)
{

    //first get the tile they are on
    int i_x = int(f_x);
    int i_y = m_i_y - int (f_y);

    if (GetTile(i_x, i_y, 1)->i_resource_index == 3)
    {
        return true; //hard as hell
    }

    if (GetTile(i_x, i_y, 0)->i_resource_index == 1)
    {
        return true; //hard as hell
    }
    
    return false;  //not hard

}


bool CSMap::InitMap(int i_x, int i_y, int i_layers)
{
    KillMap();

  //init the memory we need
  
    m_i_x = i_x;
    m_i_y = i_y;
    m_i_layers = i_layers;

    m_i_total_tiles = m_i_y*m_i_x*i_layers;
    
    m_p_tiles = new CSTile[m_i_total_tiles];

    //blank out most of them

    for (int x = 10; x < m_i_x-10; x++)
    {
        for (int y=2; y < m_i_y-8; y++)
        {
            GetTile(x, y, 0)->i_resource_index = 2; //blank it out
        }

    }

   


    //now add random blocks

    
    int i_random_blocks = m_i_total_tiles / random_range(1, 130);
    int i_x_temp;
    int i_y_temp;

    while (i_random_blocks > 0)
    {
        i_random_blocks--;
        i_x_temp = random_range(30, m_i_x);

         i_y_temp = random(m_i_y);

         if (GetTile(i_x_temp, i_y_temp , 1)->i_resource_index == 1)
         if (GetTile(i_x_temp, i_y_temp , 0)->i_resource_index == 2)
         {
         
          
             if (random(2) == 1)
             {
             
             GetTile(i_x_temp, i_y_temp , 1)->i_resource_index = 3;
             } else
             {
             GetTile(i_x_temp, i_y_temp , 0)->i_resource_index = 1;

             }
         }


    }


    log_msg("Map initted.");
    return true;
}

void CSMap::KillMap()
{
    SAFE_DELETE_ARRAY(m_p_tiles);
}