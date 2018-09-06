//data for one tile block
#pragma once
#include "..\\CBit8.h"

const unsigned char C_MT_BLOCKED = D_BIT_0;
const unsigned char C_MT_WALL = D_BIT_1;


class CMapTile
{
public:
  CBit8 bits_texture;
  int i_sub_zone; //-1 for none
};