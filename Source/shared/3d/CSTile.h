#pragma once
#include "..\all.h"

const C_CSTILE_BLOCKTYPE_CLEAR = 0;
const C_CSTILE_BLOCKTYPE_HARD = 1;
const C_CSTILE_BLOCKTYPE_BREAKABLE = 2;


class CSTile
{
public:
    CSTile()
    {
        memset(this, 0, sizeof(this));
        i_resource_index = 1; //blue block
    }
    
    int i_block_type;
    int i_resource_index;

};