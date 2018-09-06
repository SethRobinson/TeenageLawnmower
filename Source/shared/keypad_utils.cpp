/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */




#include "globals.h"
#include "keypad_utils.h"
#include "ce_all.h"
#include "math.h"

struct point_float
{
 float x;
 float y;
};

int get_optimal_direction(POINT p_target, POINT p_source)
{
   
  float a, b;
  int i_return = 0;
  float f_answer = 0;

 
 // p_target.y = -p_target.y;
 // p_source.y = -p_source.y;

 // a = sqrt( pow(p_source.x - p_target.x, 2) + pow(p_source.y - p_target.y,2));
 // b = sqrt( pow(p_source.x - p_target.x,2));

  //f_answer = 90-(((b/a)*90)/1);

  POINT final;

  final.x = p_target.x - p_source.x;
  final.y = p_target.y - p_source.y;

  f_answer = ::RADtoDEG(get_theta_from_point(final));
  f_answer -= 90;
  if (f_answer < 0) f_answer = 360+f_answer;

 // Msg("Real degree is %d.",int(f_answer));
  
  // i_return = 8;
   
   
  if (f_answer <= 90)
  {
   if ( (f_answer >= 0) && (f_answer < 22.5f)) i_return = 8;
   if ( (f_answer >= 22.5f) && (f_answer < 67.5f)) i_return = 9;
   if ( (f_answer >= 67.5f) && (f_answer <= 90)) i_return = 6;
  } else
  if (f_answer <= 180)
  {
   f_answer -= 90;
   if ( (f_answer >= 0) && (f_answer < 22.5f)) i_return = 6;
   if ( (f_answer >= 22.5f) && (f_answer < 67.5f)) i_return = 3;
   if ( (f_answer >= 67.5f) && (f_answer <= 90)) i_return = 2;
  } else
  if (f_answer <= 270)
   {
 	f_answer -= 180;
	if ( (f_answer >= 0) && (f_answer < 22.5f)) i_return = 2;
   if ( (f_answer >= 22.5f) && (f_answer < 67.5f)) i_return = 1;
   if ( (f_answer >= 67.5f) && (f_answer <= 90)) i_return = 4;
  }	else
  {
   f_answer -= 270;
   if ( (f_answer >= 0) && (f_answer < 22.5f)) i_return = 4;
   if ( (f_answer >= 22.5f) && (f_answer < 67.5f)) i_return = 7;
   if ( (f_answer >= 67.5f) && (f_answer <= 90)) i_return = 8;
  }

 /* 
  char st_crap[255];
   sprintf(st_crap,"Source: %d:%d, Target: %d:%d - answer %f. Ratio: A:%f, B:%f",
	   p_source.x, p_source.y, p_target.x, p_target.y, f_answer,a,b);
   Msg(st_crap);
   */
	 return i_return;
}



/*
int get_optimal_direction(POINT p_target, POINT p_source)
{
//	p_target.x = 370;
//	p_target.y = 362;
	
//	p_source.x = 439;
//	p_source.y = 313;




	POINT p_temp;
	p_temp.x = p_source.x - p_target.x;
	p_temp.y = p_source.y - p_target.y;
	
		Msg("At %d:%d want to go to %d:%d.  Checking %d:%d.",p_source.x,p_source.y, p_target.x,
		p_target.y, p_temp.x, p_temp.y);


	float i_deg = RADtoDEG(get_theta_from_point(p_temp));
	char st_crap[255];
	sprintf(st_crap, "Should point at %f. (%f radians)",i_deg, 	get_theta_from_point(p_temp));
	Msg(st_crap);
	return (convert_new_pad_to_pad((i_deg/45)+1));
}
*/

int get_8direction_right(int i_direction)
{

 	  //turn rightish
	if (i_direction == 1) i_direction = 4;
	else
	if (i_direction == 4) i_direction = 7;
	else
	if (i_direction == 7) i_direction = 8;
	else
	if (i_direction == 8) i_direction = 9;
	else
	if (i_direction == 9) i_direction = 6;
	else
	if (i_direction == 6) i_direction = 3;
	else
	if (i_direction == 3) i_direction = 2;
	else
	if (i_direction == 2) i_direction = 1;

 return i_direction;
}

int get_8direction_left(int i_direction)
{

 	//turn leftish
   	if (i_direction == 1) i_direction = 2;
	else
   	if (i_direction == 2) i_direction = 3;
	else
   	if (i_direction == 3) i_direction = 6;
	else
   	if (i_direction == 6) i_direction = 9;
	else
   	if (i_direction == 9) i_direction = 8;
	else
   	if (i_direction == 8) i_direction = 7;
	else
   	if (i_direction == 7) i_direction = 4;
	else
   	if (i_direction == 4) i_direction = 1;
	  

 return i_direction;
}

int force_dir_to_left_or_right(int i_dir)
{
 int i_fdir = 4; //final dir default
  
 if (i_dir == 6) i_fdir = 6;
 if (i_dir == 8)  i_fdir = 6;
 if (i_dir == 9)  i_fdir = 6;
 if (i_dir == 3)  i_fdir = 6;

 if (i_dir == 2)  i_fdir = 4;
 if (i_dir == 1)  i_fdir = 4;
 if (i_dir == 7)  i_fdir = 4;
 

  return i_fdir;
}



float get_theta_from_point(POINT crap)
{

	point_float junk_pos;
	junk_pos.x = crap.x;
	junk_pos.y = crap.y;

	
	float ttemp;
    //normal cases
	
	/*
	 float temp = junk_pos.y;
	 junk_pos.y = junk_pos.x;
	 junk_pos.x = junk_pos.y;
	  */

     if ( (junk_pos.x > 0) & (junk_pos.y > 0) )
       {
       ttemp = (float) atan(junk_pos.y / junk_pos.x);
       }

     if ( (junk_pos.x < 0) & (junk_pos.y > 0) )
       {
       junk_pos.x = (float) fabs(junk_pos.x);
       ttemp = (float) ( 3.1415926f - atan(junk_pos.y / junk_pos.x) );
       }

     if ( (junk_pos.x < 0) & (junk_pos.y < 0) )
       {
       junk_pos.y = (float) fabs(junk_pos.y);
       junk_pos.x = (float) fabs(junk_pos.x);
       ttemp = (float) ( 3.1415926f + atan(junk_pos.y / junk_pos.x) );
       }


     if ( (junk_pos.x > 0) & (junk_pos.y < 0) )
       {
       	
		 char st_hold[255];
		 junk_pos.y = (float) fabs(junk_pos.y);
//		 sprintf(st_hold, "Now it's %f and %f",junk_pos.x, junk_pos.y);
		// Msg(st_hold);
		 ttemp = (float) ((PI2) - atan(junk_pos.y / junk_pos.x) );
       }


       //special cases

     if ( (junk_pos.y == 0) & (junk_pos.x > 0) )
       {
       ttemp =0;       
       }

     if ( (junk_pos.y == 0) & (junk_pos.x < 0) )
       {
       ttemp = 3.1415926f;       
       }

     if ( (junk_pos.y > 0) & (junk_pos.x == 0) )
       {
       ttemp = 1.57f;       
       }

     if ( (junk_pos.x == 0) & (junk_pos.y < 0) )
       {
       ttemp = (4.71f);       
       }

   return ttemp;
}


int convert_pad_to_new_pad(int i_pad)
{
	if (i_pad == 8) return 1;
	if (i_pad == 9) return 2;
	if (i_pad == 6) return 3;
	if (i_pad == 3) return 4;
	if (i_pad == 2) return 5;
	if (i_pad == 1) return 6;
	if (i_pad == 4) return 7;
	if (i_pad == 7) return 8;

	Msg("Convert_pad_to_new_pad error! What do I do with %d?", i_pad);
	return 1; //error
}

int convert_new_pad_to_pad(int i_pad)
{
	
	if (i_pad == 1) return 8;
	if (i_pad == 2) return 9;
	if (i_pad == 3) return 6;
	if (i_pad == 4) return 3;
	if (i_pad == 5) return 2;
	if (i_pad == 6) return 1;
	if (i_pad == 7) return 4;
	if (i_pad == 8) return 7;
	Msg("Convert_new_pad_to_pad error!");
return 1;
}

int turn_towards_dir(int i_target, int i_source)
{
  
	if (i_target == i_source) return i_target; //nothing needs to be done

int i_new_pad_target = convert_pad_to_new_pad(i_target);
int i_new_pad_source = convert_pad_to_new_pad(i_source);

int i_left_distance = i_new_pad_source-i_new_pad_target;
if (i_left_distance < 1) i_left_distance += 8;

int i_right_distance = i_new_pad_target - i_new_pad_source;
if (i_right_distance < 1) i_right_distance += 8;


if (i_left_distance < i_right_distance)
{
 // Msg("To get to %d from %d, we turn left.",  i_new_pad_target,i_new_pad_source );
	
  //turning left
  i_new_pad_source--;
  if (i_new_pad_source < 1) i_new_pad_source += 8;
  return convert_new_pad_to_pad(i_new_pad_source);

} else
{
  	//  Msg("To get to %d from %d, we turn right.",  i_new_pad_target,i_new_pad_source );

	//turning right
  i_new_pad_source++;
  if (i_new_pad_source > 8) i_new_pad_source -= 8;
  return convert_new_pad_to_pad(i_new_pad_source);

}


return 0;
}
