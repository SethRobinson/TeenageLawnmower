#pragma once

#include "CThing.h"

const C_ENT_EDIT_MODE_CREATE = 0;
const C_ENT_EDIT_MODE_MOVE = 1;
const C_ENT_EDIT_MODE_DELETE = 2;
const C_ENT_EDIT_MODE_EDIT = 3;

class CControlEntEdit : public CThing
{
int i_mode;
unsigned int ui_cur_category;
int i_cur_item_index;
CThing *p_last_thing; //to keep track of the last guy we clicked
void (*func_ret) (); //return callback function pointer


private:
public:
	CControlEntEdit(); //init   
  virtual void think();

  void set_category_id(unsigned int ui_category){ui_cur_category = ui_category;};
  void set_item_index(int i_new_item_index){i_cur_item_index = i_new_item_index;};
  void set_mode(int i_new){i_mode = i_new;};
  void CControlEntEdit::process_create();
  void CControlEntEdit::process_delete();
  void CControlEntEdit::process_move(); 
  void CControlEntEdit::process_edit(); 
  //will be NULL if last raycheck failed
  CThing * get_last_thing_clicked(){return p_last_thing;}; 
  void set_edit_callback(void (*func)());
};