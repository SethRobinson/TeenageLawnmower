/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CThing.h"
#include "CThing_globals.h"
#include "CMessageHeader.h"
#include "CThingList.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CThing::CThing()
{
 // Msg("CThing constructed.");
	i_type = ::c_thing_unknown; //default thing is an uknown..no it has nothing
	i_zsort = 0;
	i_user_data = 0;
   	b_sort_request = true;
	b_delete_flag = false;
	p_my_thing_list = NULL; //if not null, this is the thinglist
	this->st_thing_name[0] = 0;
	m_i_pause_level = cls_tglo.GetPauseLevel(); //default, whatever is current
    m_i_thing_kill_timer = 0;
    //we're on
    SetSpherePack(NULL);
}
void CThing::set_thing_name(char st_new[C_MAX_THING_NAME_LENGTH])
{
 strcpy(st_thing_name, st_new);
}

CThing::~CThing()
{
//  Msg("Freeing base CThing.");
  
}

void CThing::move_to_top()
{
	
	if (p_my_thing_list)
	{
        int i_top = p_my_thing_list->get_sort_of_highest_thing();
		//	Msg("Setting new sort to %d.",i_top);
		set_sort(i_top+1);
	} else
	{
		log_msg("ERROR:  move_to_top can't be called in constructor!");
	}
	
}

void CThing::set_sort(int i_new_sort)
{
     i_zsort = i_new_sort;
}


void CThing::receive_message(CMessageHeader *p_header, void *p_message) //default handler
{

	if (p_header->i_message == ::C_THING_MSG_KILL)
	{
	     if (GetPauseLevel() <= cls_tglo.GetPauseLevel())
         {
         
        //we've been instructed to kill ourself
              this->b_delete_flag = true;
         }
	  return;
	}

}

void CThing::think()
{
    if (m_i_thing_kill_timer != 0)
    {
        if (m_i_thing_kill_timer < cls_tglo.get_game_age())
        {
            SetDeleteFlag(true);
            m_i_thing_kill_timer = 0;
        }
    }
}

void CThing::SetKillTimer(int i_ms)
{
    m_i_thing_kill_timer = cls_tglo.get_game_age()+(i_ms / (1000 / (1.0f/cls_tglo.time())));
}
