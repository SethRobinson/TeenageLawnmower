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
#include "..\\sound\\ISoundManager.h"

#include "d3dx8math.h"
class CThingList; //forward for speed
class CThing;

int CreateEntityFromTemplate(char st_class[256], char st_item[256]);
hSoundInstance Sound3DPlay(D3DXVECTOR3 *p_vec_pos, char *st_file, int i_repeat);
void EntKillConnectedPS(CThingList *p_thinglist, CThing *p_thing);
bool ValidateCollision(D3DXVECTOR3 *p_vec_force, D3DXVECTOR3 *p_vec_orig, float f_speed, float f_spacer);
bool ValidateCollisionCamera(D3DXVECTOR3 *p_vec_displacement, D3DXVECTOR3 *p_vec_orig,  float f_spacer);
bool ValidateCollisionAccurate(D3DXVECTOR3 *p_vec_force, D3DXVECTOR3 *p_vec_orig, float f_speed, float f_spacer);
bool ValidateCollisionSimple(D3DXVECTOR3 *p_vec_force, D3DXVECTOR3 *p_vec_orig, float f_speed, float f_spacer);

void KillByAddress(CThingList *p_thinglist, CThing *p_thing);
void Sound2DPlay(char *st_file);