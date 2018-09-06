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
#pragma warning (disable:4786)
#include <vector>
using namespace std;

#include "CTextBox.h"
#include "..\\CTimer.h"



const int C_TEXTCHOICE_LINE_SIZE = 512; //in wchar's, so actually double in bytes

//helps us keep track the choices available to us
class CChoice
{

public:

  CChoice()
  {
	  i_lines_used = 0;
	  i_return = 0;
	  ws_choice[0] = 0;
  }
	
	
  int i_return; //if they choose us, this is the id # we'll return
  WCHAR ws_choice[C_TEXTCHOICE_LINE_SIZE];

  int i_lines_used;	   //how many lines this option uses

  int CalculateLinesUsed(int i_font, int i_width);

};




typedef vector<CChoice*> ChoiceVec;
typedef ChoiceVec::iterator ChoiceItor;



class CTextChoice : public CTextBox
{

public:

    CTextChoice();
    ~CTextChoice();
    void AddChoice(int i_return, WCHAR *p_wchar);
    void ClearChoices(); 
    virtual void think();
    void UpdateText(bool b_scroll_to_end);
    virtual void UpdateScrollDownPosition();
    virtual void UpdateScrollUpPosition();
    virtual void UpdateTextPosition();
    virtual void UpdatePositions();	
    void SetScriptCallbackOnChoice(int i_script);
    bool HasBeenClicked();
    void SetMoveSound(char *st_name);
    void SetCurrentSelection(int i_choice); //to move the default, based
    //on the id's given when the choices were made, not lines.
    void AutoScrollNeighbor(CTextBox *p_new); //set to NULL to disable

private:

	int ConvertLinesToChoices(int i_start, int i_lines_to_count);
	int ConvertLinesToChoicesBackwards(int i_start, int i_lines_to_count);
    int GetLinesFromChoices(int i_choices);
	int GetLastLineListedNow();
	int ComputeClick(D3DXVECTOR2 *p_pt);
	int GetChoiceFromLine(int i_line);
	void ProcessManualUpClick();
	void ProcessManualDownClick();
	void SetChoiceClick(int i_choice);
	void ProcessManualChoiceClick();

	DWORD m_dw_alt_color; //we alternate colors to keep the options clearer
	int m_i_cur_sel; //which choice is currently highlighted, 0 if none.
	ChoiceVec m_vec_choice; //keep track of all our choices dynamically
	DWORD m_dw_sel_color; //color and alpha of what our selected text looks like
	char m_st_move_sound[256];

	int m_i_final_sel; //-1 means no selection has been made
	int m_i_final_return; //no return
	CTimer m_timer_choice; //how long we wait showing the correct choice, basically.
    CTimer m_timer_flash; //how often we should flash after they pick the correct choice
    bool m_b_flash_highlighted; //keep track of our flash
    int m_i_timer_flash_ms;
    int m_i_timer;
	bool m_b_got_initial_click; //using this you can found out when it really got clicked (early)
    CTextBox *m_p_textbox_neighbor; //used to auto scroll neighbor, so we don't miss anything when clicking
};

