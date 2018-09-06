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
#include "..\\CTimer.h"


class CVisual;

class CLightning : public CThing
{
public:

	CLightning();
	~CLightning();
	
	virtual void dx_draw();
	void LoadOrChangeTexture(char *st_filename);
	void RefreshTextureInfo();
	virtual void dx_restore();

    void SetStartPoint(D3DXVECTOR3 *vec_start) {m_vec_start =  *vec_start;}
    void SetEndPoint(D3DXVECTOR3 *vec_end) {m_vec_end =  *vec_end;}
    void SetFollowTarget(int i_id) {m_i_follow_target = i_id;}

private:

	D3DXVECTOR3 m_vec_start;
	D3DXVECTOR3 m_vec_end;
	void CalculateLightningPath();
    LPDIRECT3DTEXTURE8 m_p_tex; //pointer to the text, for speed.  The CManager->CResourceTex
 	int m_i_tex_id;  //keep track of what texture we're using
	int m_i_line_segments;
	D3DXVECTOR3 *m_p_a_segs; //the points of the lightning
	CTimer m_timer_recalc;
    float m_f_size; //how big the lightning quads are
    int m_i_follow_target; //-1 to for none
};