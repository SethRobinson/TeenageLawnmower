/************************* RANROTA.CPP ****************** AgF 1999-03-03 *
*  Random Number generator 'RANROT' type A                               *
*                                                                        *
*  This is a lagged-Fibonacci type of random number generator with       *
*  rotation of bits.  The algorithm is:                                  *
*  X[n] = ((X[n-j] + X[n-k]) modulo 2^b) rotl r                          *
*                                                                        *
*  The last k values of X are stored in a circular buffer named          *
*  randbuffer.                                                           *
*  The code includes a self-test facility which will detect any          *
*  repetition of previous states.                                        *
*  The function uses a fast method for conversion to floating point.     *
*  This method relies on floating point numbers being stored in the      *
*  standard 64-bit IEEE format.                                          *
*                                                                        *
*  The theory of the RANROT type of generators and the reason for the    *
*  self-test are described at www.agner.org/random/ranrot.htm            *
*                                                                        *
*************************************************************************/

// names of header files may differ
#include "randbota.h"
// define if you want self-test:
//#define SELF_TEST

// define 32-bits unsigned integer

// If your system doesn't have a rotate function for 32 bits integers,
// then define it thus:
// uint32 _lrotl (uint32 x, int r) {
  // return (x << r) | (x >> (sizeof(x)*8-r));}




/*
//////////////////////////////////////////////////////////////////////////
// Example showing how to use the random number generator:
void main() {
  TRanrotAGenerator RG;    // define random number generator

  RG.RandomInit(time(0)); // use time as random seed

  RG.SetInterval(1, 10);  // set interval for integer output

  for (int i=0; i<20; i++) {
    // generate 10 random floating point numbers and 10 random integers
    printf("\n%14.10f   %2i", RG.Random(), RG.iRandom());}

  getch();  // wait for user to press any key
  }
*/
