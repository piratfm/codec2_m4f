/*---------------------------------------------------------------------------*\
                                                 
  FILE........: fft.c                                                  
  AUTHOR......: Bruce Robertson                                      
  DATE CREATED: 20/11/2010                            
                                                         
  Bridging function to the kiss_fft package.      
                                                               
\*---------------------------------------------------------------------------*/
#include <assert.h>
#include <stdio.h>
#include "arm_math.h"

/*---------------------------------------------------------------------------*\
                                                                            
                                GLOBALS                                       
                                                                             
\*---------------------------------------------------------------------------*/

arm_cfft_radix2_instance_f32 arm_fft_reverse;
arm_cfft_radix2_instance_f32 arm_fft_forward;

/*---------------------------------------------------------------------------*\
                                                                             
  initialize_fft(int n)                                                                  
                                                                             
  Initialisation function for kiss_fft. This assumes that all calls to fft() 
  use the same datatypes and are one arrays of the same size.

\*---------------------------------------------------------------------------*/

__inline void
initialize_fft (int n)
{
	arm_status status;
	status = arm_cfft_radix2_init_f32(&arm_fft_forward, n, 0, 1);
	assert(status == ARM_MATH_SUCCESS);
	status = arm_cfft_radix2_init_f32(&arm_fft_reverse, n, 1, 1);
	assert(status == ARM_MATH_SUCCESS);
}

/*---------------------------------------------------------------------------*\
                                                                             
  fft(float x[], int n, int isign)                                                
  Function that calls kiss_fft with the signature of four1 from NRC.

\*---------------------------------------------------------------------------*/


__inline void
fft (float x[], int n, int isign)
{
  assert(n==512);
  //XXX:use conjugate only in forward?
  //arm_cmplx_conj_f32 (x, x, n);
  arm_cfft_radix2_f32 ((isign == -1) ? &arm_fft_reverse : &arm_fft_forward, x);
  //arm_cmplx_conj_f32 (x, x, n);
}

void cleanup_fft(void)
{
}

