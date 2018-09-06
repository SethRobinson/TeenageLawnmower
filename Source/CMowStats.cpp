//******************************************
// copyright 2002	Seth A. Robinson

// This file is NOT public domain and may not be shared, distributed or used
// without specific permission in any way.

// seth@rtsoft.com - www.rtsoft.com
//******************************************


#include "CMowStats.h"

#include "3d\\CThing_globals.h"
#include "3d\\CThingFont.h"
#include "3d\\CCamera.h"
#include "3d\\d3dapp.h"

CMowStats::CMowStats()
{
  this->set_sort(30000005); //make sure we're processed last
  set_thing_name("MowStats");

}
void CMowStats::think()
{
}

void CMowStats::dx_draw()
{
	
	
	D3DCOLOR fontColor        = D3DCOLOR_ARGB(255,255,255,0);
    D3DCOLOR fontWarningColor = D3DCOLOR_ARGB(255,0,255,255);
    char st_msg[255];
    float fNextLine;
	fNextLine = (FLOAT) cls_tglo.p_d3dapp->m_d3dsdBackBuffer.Height; 

	fNextLine -= 20.0f;
    sprintf( st_msg, "Press S to start/stop mower. V to change view. F2 to change resolutions." );
	cls_tglo.p_font->DrawFastText(0, 2, fNextLine, fontColor, st_msg);
	 
	fNextLine -= 20.0f;
    sprintf( st_msg, "Use Arrow keys and space to control the mower. Watch out for kids." );
  	cls_tglo.p_font->DrawFastText(0, 2, fNextLine, fontColor, st_msg);


}




