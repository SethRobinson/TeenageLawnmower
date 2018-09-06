/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CMod.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CMod::CMod()
{
    strcpy(m_st_name, "(no mod_name in mod_info.txt?)");
    strcpy(m_st_description, "Unknown");
    m_st_path[0] = 0;


}

void CMod::Clear()
{
 m_parse.Close();
 m_st_name[0] = 0;
 m_st_path[0] = 0;

}

bool CMod::LoadModInfoByDir(char *p_dir_name)
{
    char st_fname[256];
    sprintf(st_fname, "%s\\mod_info.txt", p_dir_name);
    if (!m_parse.open(st_fname))
    {
        return false;
    }

    char *p_line;
    while (m_parse.get_next_line(&p_line))
    {
     //  log_msg("Read %s.", p_line);
        if (compare(m_parse.get_word(1, '|'), "mod_name"))
        {
            strcpy(m_st_name, m_parse.get_word(2, '|'));
        }
    }


    return true; //success
}