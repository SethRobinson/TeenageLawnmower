//******************************************
// copyright 2002	Seth A. Robinson

// This file is NOT public domain and may not be shared, distributed or used
// without specific permission in any way.

// seth@rtsoft.com - www.rtsoft.com
//******************************************

//Hold information for the players save game

#pragma once

#include "3d\\CThing_globals.h"

#define D_CPLAYER_VERSION 1.0f

class CPlayer
{

public:
	CPlayer();
	
	
	bool CPlayer::Load(char *p_st_filename);
	bool CPlayer::Save(char *p_st_filename);

	//access
	float GetReputation() {return m_f_reputation;}
	int GetMoney() {return m_i_money;}

	void ModifyReputation(float f_amount);
	void SetReputation(float f_reputation) {m_f_reputation = f_reputation;}
	
	void ModifyMoney(int i_amount);
	void SetMoney(int i_money) { m_i_money = i_money;}
	void SetSaveDescription(char *p_st){strcpy(st_save_description, p_st);};
    bool LoadDescription(char *p_st_out, char *p_st_filename);

private:
	float m_f_file_version; //what version this file was when we saved it
	float m_f_reputation;
	int m_i_money;
	char st_save_description[256];
	//items they have?
};
