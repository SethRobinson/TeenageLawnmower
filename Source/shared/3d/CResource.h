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
#include "CThing_globals.h"



const int C_RESOURCE_NONE = 0;
const int C_RESOURCE_TEXTURE = 1;
const int C_RESOURCE_PMESH = 2;
const int C_RESOURCE_3DS = 3;
const int C_RESOURCE_SPHERE = 4;
const int C_RESOURCE_SPRITE = 5;
const int C_RESOURCE_CAL_CORE = 6;
const int C_RESOURCE_CAL_RENDER = 7;
const int C_RESOURCE_MD2 = 8;
const int C_RESOURCE_RTM = 9;

const int C_RESOURCE_SUB_NONE = 0;
const int C_RESOURCE_SUB_LANDSCAPE = 1; //it's a texture for the land

const int C_RENDER_EXTRA_NONE = 0;
const int C_RENDER_EXTRA_SHADOW = 1; //enable shadow mode



class CResource
{
  int i_references;


public:
  int i_last_used; //in game time (100 = 1 second)
  bool b_dynamic; //if dynamic it means we can't count on it's id staying
  //the same because it was loaded on the fly
  int i_resource_sub_type; //keep track of extra information
  bool b_loaded; //is this resource actually loaded?
  int i_resource_type; //what kind of resource this is
  int add_ref();
  int delete_ref();

  virtual void dx_create();
  virtual void dx_invalidate();
  virtual void dx_restore();
  virtual void dx_kill();
  inline int get_refs(){return i_references;};
  CResource();
  virtual ~CResource();
  char * GetName();
  

private:
   char st_name[256]; //information to reload it if we needed to

};
