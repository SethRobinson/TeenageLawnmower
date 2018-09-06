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

#include "CThing.h"

//basically, this class holds a buffer of the last 50 or so debug/error messages and then
//rotates them out as we get more.  It handles the ability to toggle it on and off visually, and
//controls the textbox used to show it.

const int C_CONSOLE_LINES = 20;
const int C_CONSOLE_LINE_SIZE = 255;

class CConsole:public CThing
{
public:
	CConsole();
	~CConsole();
	virtual void think();
    virtual void CConsole::dx_kill();
	void CConsole::AddText(char *p_text);
	void CConsole::DrawConsole();


private:
	int m_i_console_id;
	void CConsole::InitConsole();
	void CConsole::CloseConsole();
	char m_a_text[C_CONSOLE_LINES][C_CONSOLE_LINE_SIZE];
	int m_i_start_line;
	int m_i_new_line;
	bool m_b_refresh;
};