/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CThingInput.h"
#include "..\\CScriptEngine.h"
#include "CThing_globals.h"
#include "..\\uni.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CThingInput::CThingInput()
{
    m_i_key_mask = 0;
    m_i_script = C_SCRIPT_NONE;
    m_i_operation = C_INPUT_NOTHING;

}

CThingInput::~CThingInput()
{


}


void CThingInput::SetScriptCallbackOnKeypress( int i_script, int i_mask)
{
    m_i_script = i_script;
    m_i_key_mask = i_mask; 
    m_i_operation = C_INPUT_WAIT_FOR_KEY;
}



void CThingInput::think()
{

   if (GetPauseLevel() < cls_tglo.GetPauseLevel()) return; //game paused for us

   if (m_i_operation == C_INPUT_WAIT_FOR_KEY)
   {
       //scan all keys being hit now, if any are, send it back in string form.
       char st_temp[256];
       if (cls_tglo.p_cls_input->BuildStringFromKeysDownNow((char*)&st_temp, m_i_key_mask))
       {
           //a key was hit!  Let's call back the script now.
           //we need to set a local var in the script too.
           int i_temp_var = cls_tglo.p_script_engine->GetScript(0)->FindVarByName("G_ST_LAST_STRING");
           cls_tglo.p_script_engine->GetScript(0)->GetVar(i_temp_var)->SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni(st_temp).us_data);
           cls_tglo.p_script_engine->GetScript(m_i_script)->StartUpAfterWait();
           b_delete_flag = true; //kill us, we're no longer needed
           m_i_operation = C_INPUT_NOTHING;

       }
     
   }
  
}