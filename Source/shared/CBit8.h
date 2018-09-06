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

//simple thing to read and write 8 bools from a byte

#define D_BIT_0 1 	//00000001 in bin
#define D_BIT_1 2 	//00000001 in bin
#define D_BIT_2 4 	//00000001 in bin
#define D_BIT_3 8 	//00000001 in bin
#define D_BIT_4 16 	//00000001 in bin
#define D_BIT_5 32 	//00000001 in bin
#define D_BIT_6 64 	//00000001 in bin

#define D_BIT_7 128	//10000000 in bin

class CBit8
{
private:
	unsigned char byte_this;
public:


	void clear()
	{
	  byte_this = 0;
	};

	void set_bit(unsigned char d_bit, bool b_status)
	{
	  if (b_status)
	  {
	      //set bit
		  byte_this |= d_bit;
	  } else
	  {
	    //clear bit
 		  byte_this &= ~d_bit;
	  }

	};


   bool get_bit(unsigned char d_bit)
	{
	  if (byte_this & d_bit) return true; else return false;
	};


};