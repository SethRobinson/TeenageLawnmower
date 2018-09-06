/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



int get_8direction_right(int i_direction);
int get_8direction_left(int i_direction);
int force_dir_to_left_or_right(int i_dir);
int turn_towards_dir(int i_target, int i_source);
float get_theta_from_point(POINT junk_pos);
int convert_new_pad_to_pad(int i_pad);
int get_optimal_direction(POINT p_target, POINT p_source);
