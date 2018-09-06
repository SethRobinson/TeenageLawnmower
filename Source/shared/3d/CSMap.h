#pragma once




#include "CSTile.h"
class CSMap
{
public:
    CSMap();        
    ~CSMap();

    bool InitMap(int i_x, int i_y, int i_layers);

    void KillMap();

    int GetX(){return m_i_x;}
    int GetY(){return m_i_y;}
   
    bool CSMap::IsHardWorld(float f_x, float f_y, float f_padding);
    CSTile * GetTileWorld(int x, int y, int layer)
    {
      //first get the tile they are on
    return (GetTile(int(x), m_i_y - int (y), layer));
    }
    CSTile * GetTile(int i_x, int i_y, int i_layer) 
    {
#ifdef _DEBUG
        if (
            (i_x < 0)
            || (i_x > m_i_x)
            || (i_y < 0)
            || (i_y > m_i_y)
            )
        {
            log_error("Can't get CSTile %d, %d!", i_x, i_y);
            return &m_p_tiles[0];
        }
#endif

        //return it
        return (&m_p_tiles[(i_x+(m_i_x*i_y))+(i_layer*m_i_x*m_i_y)]);

    }
    
private:

  CSTile *m_p_tiles; //a giant array of tiles as needed
  int m_i_x; //how many tiles X we have
  int m_i_y; //how many tiles tall we have
  int m_i_layers; //how many layers deep we can have tiles (for tile on a tile, like break one)
  int m_i_total_tiles;
};