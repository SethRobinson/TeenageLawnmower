/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



//Thing class.  All animals and sprites are based on this class.
#pragma once
#include "..\\all.h"
#include "Spheretree\\spherepack.h"
class CMessageHeader; //forward for speed

//#include "globals.h"
class CThingList;

const int C_THING_UNKNOWN = 0;
const int C_THING_CONTROL = 1;
const int C_THING_EDIT = 2;
const int C_THING_TILE = 3;
const int C_THING_ENT_GENERIC = 4;
const int C_THING_CPIC = 5;
const int C_THING_TEXTBOX = 6;
const int C_THING_CONSOLE = 7;
const int C_THING_TEXTBOX_CHOICE = 8;
const int C_THING_CAMERA = 9;
const int C_THING_PARTICLE_SYSTEM = 10;
const int C_THING_INPUT = 11;

enum
{
    C_CENTERING_UPPER_LEFT,
        C_CENTERING_CENTER,
        C_CENTERING_BOTTOM_MIDDLE,
        
        //add new centering types above here
        C_CENTERING_COUNT
};

const int C_MAX_THING_NAME_LENGTH = 32;
#include <windows.h>

//known things
const int c_thing_unknown = 0;

class SpherePack; //forward dec for speed


class CThing: public SpherePackCallback
{
private:
	int i_zsort; //lower number means it will be drawn first
	int i_id;  //what is our unique id number?
	int i_type; //what is this thing?
	int i_user_data; //can be used for anything
	char st_thing_name[C_MAX_THING_NAME_LENGTH];
	protected:
	public:
	bool b_delete_flag; //will kill this soon as possible
	bool b_sort_request;  //true, and it will be sorted on the linked list.
	int m_i_pause_level; //priority we get
	CThingList * p_my_thing_list; //thing list that we are on, if not null
    SpherePack             *m_p_sphere_pack;
    int m_i_thing_kill_timer; //if != 0 then active
    
    public:

	//access for the public
	    SpherePack * GetSpherePack(void) { return m_p_sphere_pack; };
        void SetSpherePack(SpherePack *p_new_sphere_pack) {m_p_sphere_pack = p_new_sphere_pack;}

        int get_id() {return i_id;};
		int get_type() {return i_type;};
		virtual void SetPauseLevel(int i_pause_level){m_i_pause_level = i_pause_level;};
		int GetPauseLevel() {return m_i_pause_level;}
		inline int get_sort() {return i_zsort;};
		void set_id(int i_new_id) {i_id = i_new_id;};
		void set_my_thing_list(CThingList * p_temp){p_my_thing_list = p_temp;};
		void set_type(int i_new_type) {i_type = i_new_type;};
		virtual void think();
		virtual void dx_draw() {};
		virtual void dx_restore() {};
		virtual void dx_create() {};
		virtual void dx_kill() {};
		virtual void dx_invalidate() {};
		virtual void receive_message(CMessageHeader *p_header, void *p_message);
		virtual char * get_thing_name() {return (char*)&st_thing_name;};
		void set_thing_name(char st_new[C_MAX_THING_NAME_LENGTH]);

		CThing(); //construction
	    virtual ~CThing(); //deconstruction	..virtual so derived classes's
		//destructors will get called, even if deleted with a base pointer
		
        
        void SetDeleteFlag(bool b_new){b_delete_flag = b_new;}
        
        bool requires_sort(){return b_sort_request;};
		void set_sort(int i_new_sort);	//Let's us know the sorting info might
		//have changed.  If it did, will set the b_sort_request flag to true.
		void move_to_top();
		void set_user_data(int i_new){i_user_data = i_new;};
		int get_user_data(){return i_user_data;};
        void SetKillTimer(int i_ms);

};
