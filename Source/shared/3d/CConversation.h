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
#include "d3dx8math.h"


class CPic;
class CTextBox;
class CTextChoice;

class CConversation:public CThing
{
public:
	CConversation();
	~CConversation();
	virtual void think();
    virtual void dx_kill();
	int SetPanelPic(char *p_name);
	void ClearDialogText();
	void AddDialogText(WCHAR *p_ws);
	void AddChoiceText(int i_choice, WCHAR *p_ws);
	void ClearChoiceText();
    void InitChoiceText();
	void InitDialogText();
	int GetDialogId();
	int GetChoiceId();
    void set_xy(float f_x, float f_y);

    virtual void dx_restore();

private:

	void UpdatePositions();
    void SetResAdjust(int i_type, int i_base_x, int i_base_y);
 
	
	CPic *m_p_pic_panel;
	CTextBox *m_p_tb_dialog;
	CTextChoice *m_p_tb_choice;

	D3DXVECTOR2 m_pt_panel;
	D3DXVECTOR2 m_pt_pos;
	D3DXVECTOR2 m_pt_dialog;
	D3DXVECTOR2 m_pt_choice;
    int m_i_res_adjust; //if not C_VISUAL_RES_ADJUST_NONE then we need to correct for res changes
    int m_i_res_correct_base_x;
    int m_i_res_correct_base_y;
    float m_f_res_adjust_save_x; //if a screen resize happens, this is used to rebuild the location
    float m_f_res_adjust_save_y; //if a screen resize happens, this is used to rebuild the location
    


	int m_i_layer;
};