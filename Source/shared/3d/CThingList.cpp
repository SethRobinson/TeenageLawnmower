/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



//1/19/03  written to use an STL list instead of my custom linked list library

//this class keeps track of all the "things" in the game. This could be a 
//tree, a deer or a menu element.  Things can be created and destroyed.

#include "CThingList.h"
#include "CPic.h"
#include "CCamera.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CThingList::CThingList()
{
  m_i_things_created = 0;
  m_b_reset_next = true;
  b_deleting_list = false;
  m_p_sphere_tree = new SpherePackFactory(4000,256,64,8);
  m_p_sphere_tree->Reset();
  SetCullingDistance(0.0f); //0 means disabled
}

CThingList::~CThingList()
{
  SAFE_DELETE(m_p_sphere_tree);

}

void CThingList::reset_get_next()
{
	m_b_reset_next = true;
}

CThing * CThingList::get_next()
{
  if (m_b_reset_next) 
  {
	  m_itor = m_object_list.begin();
	  m_b_reset_next = false;
  }

  if (m_itor != m_object_list.end())
  {
      CThing *p_temp = (*m_itor);
      ++m_itor;  //advance it for next time
      return p_temp;
  }

    //end of the list
    m_b_reset_next = true;
    return NULL;
  
}




//returns things deleted
//if -1, count all
int CThingList::get_thing_count(int i_type_to_count)
{
  int i_things_counted = 0;

      for (CThingItor itor=m_object_list.begin(); itor != m_object_list.end(); ++itor)
    {
       if (!(*itor)->b_delete_flag)
        if ((*itor)->get_type() == i_type_to_count)
       {
         //yup, count it
           i_things_counted++;
    }
    }
  
    return i_things_counted;
}

int CThingList::add_new_thing(CThing *cls_new_thing)
{
  
   cls_new_thing->set_id(++m_i_things_created);
   cls_new_thing->set_my_thing_list(this);
   cls_new_thing->SetSpherePack(m_p_sphere_tree->AddSphere(Vector3d<float>(0,0,0),1, cls_new_thing));
   
   //insert packet into list, while sorting with the CThing's i_zsort.
   m_object_list.push_back(cls_new_thing);
  return m_i_things_created;
}


void CThingList::think()
{
   for (CThingItor itor=m_object_list.begin(); itor != m_object_list.end(); ++itor)
    {
         if (!(*itor)->b_delete_flag)
         (*itor)->think();
    }

    process_delete_requests();

    //update spheretree

   // m_p_sphere_tree->Reset(); // reset visibility state
   m_p_sphere_tree->Process(); // controls how much CPU to give up to reintegration and recomputation fifos

}

 //returns things deleted
int CThingList::delete_things_by_type(int i_type_to_delete)
{
   int i_things_deleted = 0;

     for (CThingItor itor=m_object_list.begin(); itor != m_object_list.end(); ++itor)
    {
       if ((*itor)->get_type() == i_type_to_delete)
       {
         //yup, delete it
         //log_msg("Deleting %s", (*itor)->get_thing_name());
             (*itor)->b_delete_flag = true;
         i_things_deleted++;
       }
    }
 
    // log_msg("Deleting %d of type %d", i_things_deleted, i_type_to_delete);

  return i_things_deleted;
}

//this needs to be done after every think() cycle now!

void CThingList::process_delete_requests()
{
   for (CThingItor itor=m_object_list.begin(); itor != m_object_list.end();)
    {
         if ((*itor)->b_delete_flag)
         {
            
            //remove from tree if applicable
            if ((*itor)->GetSpherePack()) m_p_sphere_tree->Remove((*itor)->GetSpherePack());
            delete (*itor); //delete the object
            m_object_list.erase(itor++);  //actually remove it from our list
            continue; //skip the rest
         }
    
         //if we got here, no removal was done.
         ++itor;
     }

}



void CThingList::dx_draw()
{
    /*   
    for_each (m_object_list.begin(), m_object_list.end(),
            mem_fun(CThing::dx_draw));
  */ //this would work if I change dx_draw to return something.  But it would work in VC7.

        for (CThingItor itor=m_object_list.begin(); itor != m_object_list.end(); ++itor)
        {
            (*itor)->dx_draw();
        }
}


void CThingList::dx_restore()
{
  for (CThingItor itor=m_object_list.begin(); itor != m_object_list.end(); ++itor)
    {
      (*itor)->dx_restore();
    }


}

void CThingList::dx_create()
{
  for (CThingItor itor=m_object_list.begin(); itor != m_object_list.end(); ++itor)
    {
      (*itor)->dx_create();
    }


}


void CThingList::dx_kill()
{
  for (CThingItor itor=m_object_list.begin(); itor != m_object_list.end(); ++itor)
    {
      (*itor)->dx_kill();
    }


}

 void CThingList::dx_invalidate()
{
  for (CThingItor itor=m_object_list.begin(); itor != m_object_list.end(); ++itor)
    {
      (*itor)->dx_invalidate();
    }


}

int CThingList::get_sort_of_highest_thing()
{

    int i_sort = -1;
    for (CThingItor itor=m_object_list.begin(); itor != m_object_list.end(); ++itor)
    {
        (*itor)->dx_invalidate();
        if ((*itor)->get_sort() > i_sort)
            i_sort = (*itor)->get_sort();
    }
    
    return i_sort;
}

//Note:  this method is the only way this will work with dinkumware's MSVC6 STL even with the latest
//service pack I guess.??  Grr!

template<> bool 
std::greater<CThing*>::operator()(CThing* const &x, CThing* const &y) const
{
  return x->get_sort() < y->get_sort();  //don't worry, these will get inlined
} 
 



void CThingList::zsort()
{
  m_object_list.sort(std::greater<CThing*>());
}

CThing * CThingList::get_thing_by_id(int i_request_id)
{
    for (CThingItor itor=m_object_list.begin(); itor != m_object_list.end(); ++itor)
    {
     	   if ((*itor)->get_id() == i_request_id)
	   {
		 //this is the guy we want.
		 return (*itor);
	   }
	  
   }
  //couldn't find 'em.
  return NULL;

}

 CThing * CThingList::get_thing_by_name(char *p_st_name)
{
    for (CThingItor itor=m_object_list.begin(); itor != m_object_list.end(); ++itor)
    {
 	   if (stricmp((*itor)->get_thing_name(), p_st_name) == 0)
	   {
		 //this is the guy we want.
		 return (*itor);
	   }
	
   }
  //couldn't find 'em.

   return NULL;

}

void CThingList::list_all_things()
{
   char st_name[32];	
   char st_final[255];
   int i_count = 0;

   for (CThingItor itor=m_object_list.begin(); itor != m_object_list.end(); ++itor)
    {
 	    i_count++;
        st_name[0] = 0;
		strcpy(st_name, (*itor)->get_thing_name());
		if (st_name[0] == 0)
		{
		 strcpy(st_name, "Unknown");
		}
	    sprintf(st_final, "%s at sort level %d",st_name,
			(*itor)->get_sort());
		log_msg(st_final);
   
   }

   log_msg("%d objects listed.",i_count);

}


//send a message to everybody or a group of things.  Sending -1 as the type to
//send to means send to everybody.  p_custom is a custom struct pointer.
void CThingList::broadcast_message(CMessageHeader *p_msg, void * p_custom, int i_type_to_send_to)
{
		
    for (CThingItor itor=m_object_list.begin(); itor != m_object_list.end(); ++itor)
    {
        if ( (i_type_to_send_to == -1) || (i_type_to_send_to == (*itor)->get_type()) )
        {
             (*itor)->receive_message(p_msg, p_custom);
        }
        
    }
    
}


bool CThingList::SendMessageByName(CMessageHeader *p_msg, void * p_custom, char *p_name)
{
   CThing *p_thing = this->get_thing_by_name(p_name);
   if (p_thing)
   {
       p_thing->receive_message(p_msg, p_custom);
       return true;
   }   else
   {
       log_msg("Unable to send message to thing %s.", p_name);
   }

   return false; //couldn't send it
}


void CThingList::delete_list()
{
    b_deleting_list = true;
  
    for (CThingItor itor=m_object_list.begin(); itor != m_object_list.end(); ++itor)
    {
       //remove from tree if applicable
            if ((*itor)->GetSpherePack()) m_p_sphere_tree->Remove((*itor)->GetSpherePack());
            delete (*itor);
    }
    
    m_object_list.clear();
    b_deleting_list = false;
}
       


void CThingDrawHandler::RangeTestCallback(const Vector3d<float> &p,float distance,SpherePack *sphere,ViewState state)
{
       ((CThing*)sphere->GetUserData())->dx_draw();
  
}

