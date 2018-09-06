#pragma once

#include "stdafx.h"
#include "dlg_log.h"
#include "file_swed.h"


#include "..\\..\\..\\shared\\CInput.h"
#include "..\\..\\..\\shared\\all.h"
#include "..\\..\\..\\shared\\3d\\CThingList.h"
#include "..\\..\\..\\shared\\3d\\CThing_globals.h"
#include "..\\..\\..\\shared\\3d\\CMap.h"

#define WM_ADD_TO_LOG WM_USER+2
const float C_F_VERSION = 0.03f;

void log_msg(LPCSTR lpFormat, ...);
void log_error(LPCSTR lpFormat, ...);


class dlg_swed;
class dlg_3d;
class dlg_log;
class dlg_world_edit;
class dlg_landscape;
class dlg_edit_sub;
class dlg_edit_entity;

class class_globals
{
   
public:
	 
	 //global pointers to windows
	 dlg_swed *p_dlg_main;
	 dlg_3d *p_dlg_3d;
	 dlg_log *p_dlg_log;
	 CSwed cls_swed;
	 dlg_world_edit *p_dlg_world_edit; 
	 dlg_landscape *p_dlg_landscape;
	 dlg_edit_sub *p_dlg_edit_sub;
	 dlg_edit_entity *p_dlg_edit_entity;

	 
	 
	 class_globals()
	{
	 ZeroMemory(this, sizeof(class_globals));
	}


};

extern class_globals glo;


//handle DX8 input
extern CInput cls_input;

extern CMapZone cls_map; 
//handle map data (but not the drawing part)

//handle creation of all objects
extern CThingList cls_things;





