/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


//data for one map block
#pragma once
#include "..\\CBit8.h"


//USED IN GUI ONLY
const unsigned char C_MB_TEXTURE_MODE_NONE = 0;
const unsigned char C_MB_TEXTURE_MODE_FLIP_X = 1;
const unsigned char C_MB_TEXTURE_MODE_FLIP_Z = 2;
const unsigned char C_MB_TEXTURE_MODE_FLIP_X_AND_Z = 3;
// ***************


const unsigned char C_MB_BITS_TEX_FLIP_X = D_BIT_0;
const unsigned char C_MB_BITS_TEX_FLIP_Z = D_BIT_1;

//used for overlays
const unsigned char C_MB_BITS_TEX2_FLIP_X = D_BIT_2;
const unsigned char C_MB_BITS_TEX2_FLIP_Z = D_BIT_3;


const unsigned char C_MB_BITS_TEX_FACE_MAP = D_BIT_4;
const unsigned char C_MB_BITS_TEX2_FACE_MAP = D_BIT_5;


class CMapBlock
{
  
public:
   
  unsigned short i_tex_id; //texture id of this block
  CBit8 bits_texture;
  unsigned short i_overlay_tex_id; //texture id of this block
};