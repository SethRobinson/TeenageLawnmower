/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



//code to interface our scripting library with our 3d stuff

#pragma once

void add_global_functions_to_script_0(); //run once at beginning so the scripting
//can use extended global functions that we want to be built in

extern int g_i_touched_by_id;