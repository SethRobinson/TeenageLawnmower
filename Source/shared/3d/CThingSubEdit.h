#pragma once

#include "CThing.h"
#include "CCamera.h"
#include "CTerrain.h"

const C_EDIT_SUB_HARDNESS = 0;
const C_EDIT_SUB_SUB_ZONE = 1;
const C_EDIT_WALL = 2;

class CThingSubEdit : public CThing
{

private:
  int i_edit_mode; //current operation
  int i_sub_zone; //current sub zone we're working with if applicable
  int i_default_tex; //new walls get this everywhere
  bool b_block_mode; //paint 9 tiles at a time
public:

	void CThingSubEdit::process_zone_edit();
	void CThingSubEdit::set_edit_mode(int i_new);
	void CThingSubEdit::set_sub_zone(int i_new);
	void CThingSubEdit::process_sub_zone_edit();
	void CThingSubEdit::process_edit_wall();
	void set_default_tex(int i_new_tex_id){i_default_tex = i_new_tex_id;};
	void CThingSubEdit::set_block_mode(bool b_new){b_block_mode = b_new;};
  CThingSubEdit(); //init   
  virtual void think();

};