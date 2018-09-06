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
#include "..\\all.h"
#include "CEnt.h"

struct D3DXVECTOR3; //for speed
struct D3DXVECTOR2; //for speed

class CThing; //for speed

const int C_THING_MSG_NONE = 0;
const int C_THING_MSG_SAVE_GAME = 1; //save yourself to a file, uses CMessageFile for second parm
const int C_THING_MSG_KILL = 2; //delete yourself!
const int C_THING_RAY_PICK = 3; //did this ray hit you?
const int C_THING_MSG_MOVE = 4; //movement, possible damage
const int C_THING_MSG_INPUT = 5; //a key was hit
const int C_THING_MSG_GENERIC = 6; 
const int C_THING_MSG_TOUCHED_WALL = 7; 
const int C_THING_MSG_TOUCHED_GROUND = 8; 
const int C_THING_MSG_TOUCHED_CEILING = 9; //went higher than the ceiling

const int C_COLLISION_SPECIAL_NONE = 0;
 
const int C_COLLISION_SPECIAL_SAME_OR_LOWER = 1;

const int C_MESSAGE_COMMAND_TOGGLE_WIREFRAME = 1;
const int C_MESSAGE_COMMAND_DELETE_THINGS_BY_TYPE = 2;
const int C_MESSAGE_COMMAND_SWITCH_TO_PLAYER_WALK_CAM = 3;
const int C_MESSAGE_COMMAND_SWITCH_TO_TOP_VIEW_CAM = 4;
const int C_MESSAGE_COMMAND_LIST_ALL_THINGS = 5;
const int C_MESSAGE_COMMAND_CREATE_SOME_SPHERES = 6;
const int C_MESSAGE_COMMAND_SWITCH_TO_FREE_CAM = 7;
const int C_MESSAGE_COMMAND_SWITCH_TO_PLAY_CAM = 8;
const int C_MESSAGE_COMMAND_RESET_LIGHT_AND_FOG = 9;
const int C_MESSAGE_COMMAND_RANDOM_FOG = 10;
const int C_MESSAGE_COMMAND_RANDOM_SUN = 11;
const int C_MESSAGE_COMMAND_TOGGLE_LAND_WIREFRAME = 12;
const int C_MESSAGE_COMMAND_TOGGLE_SHOW_TILE = 13;
const int C_MESSAGE_COMMAND_REINIT_MAP = 14;
const int C_MESSAGE_COMMAND_TERRAFORM_MAP = 15;
const int C_MESSAGE_COMMAND_FORCE_GRID_TILES_ON = 16;
const int C_MESSAGE_COMMAND_REBUILD_WALLS = 17;
const int C_MESSAGE_COMMAND_REBUILD_ALL_WALLS = 18;
const int C_MESSAGE_COMMAND_LIST_ALL_RESOURCES = 19;
const int C_MESSAGE_COMMAND_INIT_ENGINE_FROM_PATH = 20;



class CMessageHeader
{
public:
 int i_message;

 //time and delivery information here?

};
 class CMessageInput
{
  //this struct is used for sending info needed to load and save
  //files
public:
	char c_key; //the key the was hit.
    unsigned short c_wkey; //if we're using unicode for INPUT this is used (not supported)
};


class CMessageFile
{
  //this struct is used for sending info needed to load and save
  //files
public:
	FILE *fp; //the file handle to use

};

class CMessageRayPick
{
  //used to ask people "did this ray hit you?"
public:

	D3DXVECTOR3 *p_vec_origin; //beginning of ray
	D3DXVECTOR3 *p_vec_dir; //direction of the ray
	D3DXVECTOR3 *p_vec_end; //end point of ray
	float f_range; //how far the ray can reach
	D3DXVECTOR3 *p_vec_hit; //where the hit was, NULL if none yet
	float f_dist; //distance of last hit
	CThing *p_thing; //pointer to base class of who we hit
	D3DXVECTOR2 *p_vec_mouse; //actual mouse hit on the screen, helps for checking CPic interface items
};


//let's us tell other ents that we moved, our size and how much damage we did.

class CMessageMove
{
public:
   D3DXVECTOR3 *p_vec_pos; //position
   float f_damage; //if not 0, this is doing damage.
   int e_collision_type; //specified in CVisualMulti.h
   int i_collision_special; //for sending an extra instruction
   float f_radius; //used for circle collision
   float f_enemy_rad_mod; //change how sensitive the sphere/sphere check is
   CEnt *p_me; //guy who issued this
   CEnt *p_blocked_by;
};

//used for regular windows messages
class CMessageGeneric
{
public:
    //used mostly for the editor to pass messages to the engine in a threadsafe
    //way.
    int i_message_command;
    int i_thing_type;    //optionally used
    float f_scale;   //optionally used
    
    
    //for initting random terrain
    float f_height;
    float f_height_scale;
    float f_hill_freq;
    float f_hill_scale;
    int i_run_times;

    int i_building; 
};


class CEngineMessage
{
public:
    
    CEngineMessage()
    {
        ZeroMemory(this, sizeof(CEngineMessage));
    }

    int m_i_entity_type; //if name is NULL this will be used.  -1 is send to all
    char m_st_entity_name[32]; //if null, will use above instead
    bool m_b_system_message; //if true, the main system should handle it, not an object
    int m_i_message_type;
    void *m_p_message; //the actual message

};

void ProcessWindowsEngineMessage(DWORD dw_message, CEngineMessage * p_message);
void SendWindowsEngineMessage(CEngineMessage *p_message);
void SendSimpleWindowsEngineMessage(int i_command, int i_parm);
