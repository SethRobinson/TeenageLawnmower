#include "CWeedKiller.h"
#include "3d\\CThing_globals.h"
#include "3d\\CMap.h"
#include "3d\\CEnt.h"
#include "3d\\CTerrain.h"
#include "CEntMower.h"
#include "3d\\ent_utils.h"
#include "3d\\CThing_util.h"


#define C_WEEDKILLER_HOW_MANY_TILES_TO_MOW_AT_ONCE 1
#define C_WEEDKILLER_DELAY_BEFORE_START_MS 800

CWeedKiller::CWeedKiller()
{
    m_i_size = 0;
}

   
void CWeedKiller::Init(D3DXVECTOR3 *p_vec_pos, int i_range)
{
    m_timer_think.set_interval(C_WEEDKILLER_DELAY_BEFORE_START_MS);
    if (!cls_tglo.p_player || !cls_tglo.p_map)
    {
        assert(!"This should never get called when a valid player or map isn't on the screen");
        b_delete_flag = true;
        return;
    }
    
    m_i_size = i_range;
    
    m_i_target_x = cls_tglo.p_map->get_block_x(p_vec_pos->x);
    m_i_target_y = cls_tglo.p_map->get_block_y(p_vec_pos->z);
    
    //LogMsg("Center of target is %d, %d.", m_i_target_x, m_i_target_y);
    
    m_i_mow_zone = ((CEntMower*)cls_tglo.p_player)->GetMowZone();
    m_i_mowed_tile_graphic =  ((CEntMower*)cls_tglo.p_player)->GetMowedTile();
    //let's initialize the area we want to change
    m_prime.Init(m_i_size *m_i_size);

}

CWeedKiller::~CWeedKiller()
{
}

//returns true if we actually mowed something

bool CWeedKiller::MowArea(int i_num)
{

    //convert to the real block #
    int block_y = i_num / m_i_size;
    int block_x = i_num - (block_y*m_i_size);
    
    int x = (m_i_target_x + block_x)- (m_i_size/2);
    int y = (m_i_target_y + block_y) - (m_i_size/2);

    //let's actually see if we can mow it now

    //LogMsg("checking. %d.. (%d, %d)", i_num, x, y);

    if (!cls_tglo.p_map->is_valid_block(x, y))
    {
        //LogMsg("Invalid block..");
        return false;
    }
     int i_tls_per_blk = cls_tglo.p_map->GetTilesPerBlock();
    float f_block_size = cls_tglo.p_map->get_block_size();

     //there are 9 tiles in a block, we just need to check one of them to see if it's mowable or not.
     if (m_i_mow_zone != (cls_tglo.p_map->get_tile_pointer(cls_tglo.p_map->get_tile_x( (x*f_block_size)+1),
         cls_tglo.p_map->get_tile_y((y*f_block_size))+1)->i_sub_zone))
    {
       // LogMsg("Not a mow tile.");
    }   else
    {
        
        //let's make the mow pattern more roundish by throwing out far tiles
        float dist = Slow_Distance_2D(m_i_target_x-x, m_i_target_y-y);
        
         // LogMsg("Tile distance was %d.", int(dist*100));
          if (dist > (float(m_i_size-1)/2)) return false;

        if (cls_tglo.p_map->p_block[GET_BLOCK_SMART(x,y)].i_tex_id != m_i_mowed_tile_graphic)
        {
            //LogMsg("Let's mow it!");
            cls_tglo.p_map->p_block[GET_BLOCK_SMART(x,y)].i_tex_id = m_i_mowed_tile_graphic;
            cls_tglo.p_terrain->update_terrain_block_from_map(x,y);
	        
            D3DXVECTOR3 pos;
            pos.x = x*f_block_size;
            pos.z = y*f_block_size;
            pos.y = 0; //y is height, I don't want to waste the cycles on getting the actual height

            //how about a sfx?
             Sound3DPlay(&pos, "sound\\pop.wav", 1);
             if (cls_tglo.p_player) 
             {
                  ((CEntMower*)cls_tglo.p_player)->AddTileToMowed();

             }    else
             {
                 assert(0);
             }
             return true;
        }  else
        {
         //   LogMsg("Let's not mow it.");
        }
    }         
    return false;
}

void CWeedKiller::think()
{
    assert(m_i_size != 0 && "You need to call Init() first your majesty");
    
    if (m_timer_think.interval_reached())
    {
        m_timer_think.set_interval(30); //gets faster after the initial delay
        
        int i_num;
      
        int i_mowed = 0;
        while ( (i_num = m_prime.GetNext()) != -1)
        {
            if (MowArea(i_num))
            {
                //it was a valid area, that's all for now
                i_mowed++;
                if (i_mowed >= C_WEEDKILLER_HOW_MANY_TILES_TO_MOW_AT_ONCE)
               return;
            }
        }
        
        //if we got here, we must be totally done
        SetDeleteFlag(true); //all done
        //tell it to recalc everything
    }
    
}