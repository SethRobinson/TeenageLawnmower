/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CAliasBind.h"
#include "..\\CScriptEngine.h"
#include "CThing_globals.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int scan2ascii(DWORD scancode, unsigned short * result)
{
   static HKL layout=GetKeyboardLayout(0);
   static unsigned char State[256];

   if (GetKeyboardState(State)==FALSE)
      return 0;
   UINT vk=MapVirtualKeyEx(scancode,1,layout);
   return ToAsciiEx(vk,scancode,State,result,0,layout);
}


const byte C_ALIAS_NOKEY = 255; //special hack so you can do "control,nokey" and it
//will run only with tapping control.

static const char EnglishAsciiTable[] = {
   // Standard translation table
     0,   0, '1', '2', '3', '4', '5', '6', '7', '8',  //   0
   '9', '0', '-', '=',   0,   0, 'q', 'w', 'e', 'r',  //  10
   't', 'y', 'u', 'i', 'o', 'p', '[', ']',   0,   0,  //  20
   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',  //  30
  '\'', '`',   0,'\\', 'z', 'x', 'c', 'v', 'b', 'n',  //  40
   'm', ',', '.', '/',   0, '*',   0, ' ',   0,   0,  //  50
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  60
     0, '7', '8', '9', '-', '4', '5', '6', '+', '1',  //  70
   '2', '3', '0', '.',   0,   0,'\\',   0,   0,   0,  //  80
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  90
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  // 100
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  // 110
     0,   0,   0,   0,   0,   0,   0,   0            // 120

	 /* I'm actually not using these...

  /*
   // Shift translation table
    , 0,   0, '!', '@', '#', '$', '%', '^', '&', '*',  //   0
   '(', ')', '_', '+',   0,   0, 'Q', 'W', 'E', 'R',  //  10
   'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',   0,   0,  //  20
   'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',  //  30
   '"', '~',   0,'\'', 'Z', 'X', 'C', 'V', 'B', 'N',  //  40
   'M', '<', '>', '?',   0, '*',   0, ' ',   0,   0,  //  50
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  60
     0, '7', '8', '9', '-', '4', '5', '6', '+', '1',  //  70
   '2', '3', '0', ',',   0,   0, '|',   0,   0,   0,  //  80
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  90
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  // 100
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  // 110
     0,   0,   0,   0,   0,   0,   0,   0,            // 120

   // Capslock translation table
     0,   0, '1', '2', '3', '4', '5', '6', '7', '8',  //   0
   '9', '0', '-', '=',   0,   0, 'Q', 'W', 'E', 'R',  //  10
   'T', 'Y', 'U', 'I', 'O', 'P', '[', ']',   0,   0,  //  20
   'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',  //  30
  '\'', '`',   0,'\\', 'Z', 'X', 'C', 'V', 'B', 'N',  //  40
   'M', ',', '.', '/',   0, '*',   0, ' ',   0,   0,  //  50
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  60
     0, '7', '8', '9', '-', '4', '5', '6', '+', '1',  //  70
   '2', '3', '0', '.',   0,   0,'\\',   0,   0,   0,  //  80
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  90
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  // 100
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  // 110
     0,   0,   0,   0,   0,   0,   0,   0,            // 120

   // Capslock shift translation table
     0,   0, '!', '@', '#', '$', '%', '^', '&', '*',  //   0
   '(', ')', '_', '+',   0,   0, 'q', 'w', 'e', 'r',  //  10
   't', 'y', 'u', 'i', 'o', 'p', '{', '}',   0,   0,  //  20
   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':',  //  30
   '"', '~',   0,'\'', 'z', 'x', 'c', 'v', 'b', 'n',  //  40
   'm', '<', '>', '?',   0, '*',   0, ' ',   0,   0,  //  50
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  60
     0, '7', '8', '9', '-', '4', '5', '6', '+', '1',  //  70
   '2', '3', '0', ',',   0,   0, '|',   0,   0,   0,  //  80
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  90
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  // 100
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  // 110
     0,   0,   0,   0,   0,   0,   0,   0,            // 120

   // AltGr translation table
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //   0
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  10
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  20
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  30
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  40
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  50
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  60
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  70
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  80
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  //  90
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  // 100
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  // 110
     0,   0,   0,   0,   0,   0,   0,   0             // 120
	 */

 };



byte GetDikFromChar(char ch_char)
{

	//run through our list and find a matching char

	for (int i=0; i < 112; i++)
	{
		if (ch_char == EnglishAsciiTable[i])
			return i; //found it!
	}

	//couldn't find our dik
	return 0;

}

char GetCharFromDik(byte dik)
{

	//run through our list and find a matching char
    if ( (dik < 1) || (dik > 84) )
    {
        //not a regular char, it's probably a weird one like f1 or up.
        return 0;  //don't know what it is
    }

    return EnglishAsciiTable[dik];
}


bool ConvertDikToString(char *st_temp, byte b_dik)
{

     st_temp[0] = 0;
          
      if (b_dik == DIK_ESCAPE) strcpy(st_temp, "escape");
   
   if (b_dik == DIK_UP) strcpy(st_temp, "up");
   if (b_dik == DIK_DOWN) strcpy(st_temp, "down");
   if (b_dik == DIK_LEFT) strcpy(st_temp, "left");
   if (b_dik == DIK_RIGHT) strcpy(st_temp, "right");

   if (b_dik == DIK_SPACE) strcpy(st_temp, "space");

   
     if (b_dik == DIK_F1) strcpy(st_temp, "f1");
     if (b_dik == DIK_F2) strcpy(st_temp, "f2");
     if (b_dik == DIK_F3) strcpy(st_temp, "f3");
     if (b_dik == DIK_F4) strcpy(st_temp, "f4");
     if (b_dik == DIK_F5) strcpy(st_temp, "f5");
     if (b_dik == DIK_F6) strcpy(st_temp, "f6");
     if (b_dik == DIK_F7) strcpy(st_temp, "f7");
     if (b_dik == DIK_F8) strcpy(st_temp, "f8");
     if (b_dik == DIK_F9) strcpy(st_temp, "f9");
     if (b_dik == DIK_F10) strcpy(st_temp, "f10");
     if (b_dik == DIK_F11) strcpy(st_temp, "f11");
     if (b_dik == DIK_F12) strcpy(st_temp, "f12");
     if (b_dik == DIK_NUMPAD0) strcpy(st_temp, "numpad0");
     if (b_dik == DIK_NUMPAD1) strcpy(st_temp, "numpad1");
     if (b_dik == DIK_NUMPAD2) strcpy(st_temp, "numpad2");
     if (b_dik == DIK_NUMPAD3) strcpy(st_temp, "numpad3");
     if (b_dik == DIK_NUMPAD4) strcpy(st_temp, "numpad4");
     if (b_dik == DIK_NUMPAD5) strcpy(st_temp, "numpad5");
     if (b_dik == DIK_NUMPAD6) strcpy(st_temp, "numpad6");
     if (b_dik == DIK_NUMPAD7) strcpy(st_temp, "numpad7");
     if (b_dik == DIK_NUMPAD8) strcpy(st_temp, "numpad8");
     if (b_dik == DIK_NUMPAD9) strcpy(st_temp, "numpad9");
     if (b_dik == DIK_DECIMAL) strcpy(st_temp, "numpaddec");
     if (b_dik == DIK_SUBTRACT) strcpy(st_temp, "numpadsub");
     if (b_dik == DIK_RETURN) strcpy(st_temp, "enter");
   
 
   
   //is it a regular known dik?
   if (st_temp[0] == 0)
   {
       
       char ch_temp = GetCharFromDik(b_dik);
       if (ch_temp != 0)
       {
           //great, we knew what it was, add it to our master deal
           sprintf(st_temp, "%c", ch_temp);
          
       }
   }

   if (st_temp[0] != 0) return true;

   return false; //found nothing

}



CAliasBind::CAliasBind()
	{
		m_i_script = C_SCRIPT_NONE;
		//init
		Clear();
	}



void CAliasBind::Clear()
	{
		m_i_vk_index = -1;
		
		//clear key info
	
		m_b_key = 0;      
		m_b_shift = 0;    
		m_b_alt = 0;      
		m_b_control = 0;  
		m_i_repeat = -1; //this means no auto repeat
		//kill the script if we had one loaded
		if (m_i_script != C_SCRIPT_NONE)
		{
			if (cls_tglo.p_script_engine)
			{
			
			cls_tglo.p_script_engine->UnloadScript(m_i_script);
			}
			m_i_script = C_SCRIPT_NONE;
		}
	}

CAliasBind::~CAliasBind()
{
	Clear();

}

void CAliasBind::SetRepeat(int i_repeat)
{

	m_i_repeat = i_repeat;
}

void CAliasBind::SetKeyFromString( char *p_st)
{
	
//	log_msg("Setting key %s.", p_st);

    //catch any known phrases

	
	//the three special cases 
	
	if (stricmp(p_st, "control") == 0)
	{
	   m_b_control = true;
	} else

	if (stricmp(p_st, "shift") == 0)
	{
	   m_b_shift = true;
	} else

	if (stricmp(p_st, "alt") == 0)
	{
	   m_b_alt = true;
	}  else


   //another assload of special cases, but theses can be real keys


	if (stricmp(p_st, "space") == 0)
	{
	   m_b_key = DIK_SPACE;
	}  else
	if (stricmp(p_st, "escape") == 0)
	{
	   m_b_key = DIK_ESCAPE;
	}  else
		
		if (stricmp(p_st, "up") == 0)
	{
	   m_b_key = DIK_UP;
	}  else

	if (stricmp(p_st, "down") == 0)
	{
	   m_b_key = DIK_DOWN;
	}  else

	if (stricmp(p_st, "left") == 0)
	{
	   m_b_key = DIK_LEFT;
	}  else

	if (stricmp(p_st, "right") == 0)
	{
	   m_b_key = DIK_RIGHT;
	}  else

	if (stricmp(p_st, "f1") == 0)
	{
	   m_b_key = DIK_F1;
	}  else
		
	if (stricmp(p_st, "f2") == 0)
	{
	   m_b_key = DIK_F2;
	}  else
	if (stricmp(p_st, "f3") == 0)
	{
	   m_b_key = DIK_F3;
	}  else
	if (stricmp(p_st, "f4") == 0)
	{
	   m_b_key = DIK_F4;
	}  else
	if (stricmp(p_st, "f5") == 0)
	{
	   m_b_key = DIK_F5;
	}  else
	if (stricmp(p_st, "f6") == 0)
	{
	   m_b_key = DIK_F6;
	}  else
	if (stricmp(p_st, "f7") == 0)
	{
	   m_b_key = DIK_F7;
	}  else
	if (stricmp(p_st, "f8") == 0)
	{
	   m_b_key = DIK_F8;
	}  else
	if (stricmp(p_st, "f9") == 0)
	{
	   m_b_key = DIK_F9;
	}  else
	if (stricmp(p_st, "f10") == 0)
	{
	   m_b_key = DIK_F10;
	}  else
	if (stricmp(p_st, "f11") == 0)
	{
	   m_b_key = DIK_F11;
	}  else
	if (stricmp(p_st, "f12") == 0)
	{
	   m_b_key = DIK_F12;
	}  else
  	if (stricmp(p_st, "nokey") == 0)
	{
	   m_b_key = C_ALIAS_NOKEY;
	}  else

 	if (stricmp(p_st, "numpad1") == 0)
	{
	   m_b_key = DIK_NUMPAD1;
	}  else
 	if (stricmp(p_st, "numpad2") == 0)
	{
	   m_b_key = DIK_NUMPAD2;
	}  else
  	if (stricmp(p_st, "numpad3") == 0)
	{
	   m_b_key = DIK_NUMPAD3;
	}  else
	if (stricmp(p_st, "numpad4") == 0)
	{
	   m_b_key = DIK_NUMPAD4;
	}  else
	if (stricmp(p_st, "numpad5") == 0)
	{
	   m_b_key = DIK_NUMPAD5;
	}  else
	if (stricmp(p_st, "numpad6") == 0)
	{
	   m_b_key = DIK_NUMPAD6;
	}  else
	if (stricmp(p_st, "numpad7") == 0)
	{
	   m_b_key = DIK_NUMPAD7;
	}  else
	if (stricmp(p_st, "numpad8") == 0)
	{
	   m_b_key = DIK_NUMPAD8;
	}  else
	if (stricmp(p_st, "numpad9") == 0)
	{
	   m_b_key = DIK_NUMPAD9;
	}  else
	if (stricmp(p_st, "numpad0") == 0)
	{
	   m_b_key = DIK_NUMPAD0;
	}  else
	if (stricmp(p_st, "numpaddec") == 0)
	{
	   m_b_key = DIK_DECIMAL;
	}  else
	if (stricmp(p_st, "numpadsub") == 0)
	{
	   m_b_key = DIK_SUBTRACT;
	}  else
        if (stricmp(p_st, "enter") == 0)
        {
            m_b_key = DIK_RETURN;
        }  else
            

	{
	
    	//else assume it's a regular key
       m_b_key = GetDikFromChar(p_st[0]);
//   log_msg("Dik of %s is %c", p_st, EnglishAsciiTable[m_b_key]);
	
	}


}


void CAliasBind::GetStringFromKey(char *p_string)
{

   p_string[0] = 0; //clear it

    //first add special chars like control
      char st_temp[64];
      st_temp[0] = 0;
   
      ConvertDikToString((char*)&st_temp, m_b_key);


   //build final string
    strcat(p_string, st_temp);


   if (p_string[0] == 0)
   {
   
      strcpy(p_string, "none");
   }
}


//convert comma deliminated string to individual keys

bool CAliasBind::ConvertStringToKeys(char *p_st)
{
   char st_key[64];

//   log_msg("Setting keys.");

   st_key[0] = 0;

   for (int i=0; /*infinite loop*/; i++)
   {
	  
	   if (
		   (p_st[i] == 0) //we hit the end of the string
		   || (p_st[i] == ',') //we hit a deliminator
		   )
	   {
		   //either way, we want to convert this puppy
		 //convert what we have so far
		   SetKeyFromString(st_key);
		   st_key[0] = 0; //reset this
		   if (p_st[i] == 0)
		   {

			   //actually we're done now
			   break;
		   }
	     //else it's a comma.  Let's not include the comma in our string
		 i++;
	   }
	   
	   strncat((char*)&st_key, &p_st[i], 1); //add one char
   }


   return true;
}


//load a script, run it when the key is hit.
bool CAliasBind::SetScript(char *p_st)
{
	m_i_script = cls_tglo.p_script_engine->CreateScript();
	//let's add any script specific variables it needs
	//int i_temp_var = cls_tglo.p_script_engine->GetScript(m_i_script)->AddVar("i_my_id", ::C_SCRIPT_VAR_INT);
	//cls_tglo.p_script_engine->GetScript(m_i_script)->GetVar(i_temp_var)->SetInt(this->get_id());
	
	m_i_script = cls_tglo.p_script_engine->load_and_compile_script(p_st, m_i_script);
	if (m_i_script == C_SCRIPT_NONE)
	{
		//big time error
		log_msg("Error binding alias to script %s.", p_st);
		return false;
	}

	return true;
}


void CAliasBind::Update()
{
	//a tick has passed, let's update our key information

    
	//ok, and finally, is it ok for us to get hit?
	if (m_i_vk_index != -1)
	{
		bool b_pressed = true;
	
        if (m_i_repeat != -1)
    {
        //get guess what!  This is now used for joystick stuff...

        if (cls_tglo.p_cls_input->GetAnyJoystickButtonTapped(m_i_repeat))
        {
            goto force_activate;
        }

    }


		//do all conditions get met to signal a keypress?
		if (cls_tglo.p_cls_input->is_key_down(m_b_key) == false)
		{
			//definately a no
			b_pressed = false;
		}
		
		
		if (m_b_control)
			if ( (!cls_tglo.p_cls_input->is_key_down(DIK_LCONTROL)) &&
				(!cls_tglo.p_cls_input->is_key_down(DIK_RCONTROL))
				)
			{
				//sorry, we need a control down
				b_pressed = false;
				
			}
			
			if (m_b_alt)
				if ( (!cls_tglo.p_cls_input->is_key_down(DIK_LALT)) &&
					(!cls_tglo.p_cls_input->is_key_down(DIK_RALT))
					)
				{
					//sorry, we need an alt down
					b_pressed = false;
					
				}
				if (m_b_shift)
					if ( (!cls_tglo.p_cls_input->is_key_down(DIK_LSHIFT)) &&
						(!cls_tglo.p_cls_input->is_key_down(DIK_RSHIFT))
						)
					{
						//sorry, we need a shift down
						b_pressed = false;
						
					}
	
force_activate:
                    cls_tglo.p_cls_input->SetVirtualKey(m_i_vk_index , b_pressed); 
					
	}
	
	
  if (m_i_repeat != -1)
    {
        //get guess what!  This is now used for joystick stuff...

        if (cls_tglo.p_cls_input->GetAnyJoystickButtonTapped(m_i_repeat))
        {
            goto force_activate_script;
        }

    }


	//do all conditions get met to signal a keypress?
	if (cls_tglo.p_cls_input->is_key_down(m_b_key) == false)
	{
      //definately a no
	   return;
	}


    if (m_b_control)
		if ( (!cls_tglo.p_cls_input->is_key_down(DIK_LCONTROL)) &&
			(!cls_tglo.p_cls_input->is_key_down(DIK_RCONTROL))
			)
		{
			//sorry, we need a control down
			return;
		}

    if (m_b_alt)
		if ( (!cls_tglo.p_cls_input->is_key_down(DIK_LALT)) &&
			(!cls_tglo.p_cls_input->is_key_down(DIK_RALT))
			)
		{
			//sorry, we need an alt down
			return;
		}
	 if (m_b_shift)
		if ( (!cls_tglo.p_cls_input->is_key_down(DIK_LSHIFT)) &&
			(!cls_tglo.p_cls_input->is_key_down(DIK_RSHIFT))
			)
		{
			//sorry, we need a shift down
			return;
		}


  
	

		//let's assume taps only for now
		if (cls_tglo.p_cls_input->is_key_tapped(m_b_key))
		{
force_activate_script:
			//register a hit
			if (m_i_script != C_SCRIPT_NONE)
			{
				//let's run this script
				cls_tglo.p_script_engine->RunScriptFunction(m_i_script, "MAIN");
			}
           
		}

}