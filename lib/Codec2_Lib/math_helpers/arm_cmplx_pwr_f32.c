/* ----------------------------------------------------------------------
* $Date:        15. February 2012
* $Revision: 	V1.1.0
*
* Project: 	    CMSIS DSP Library Add-On
* Title:		arm_cmplx_pwr_f32.c
*
* Description:	Floating-point complex power.
*
* Target Processor: Cortex-M4/Cortex-M3/Cortex-M0
*
* ---------------------------------------------------------------------------- */

#include "arm_math.h"
#include "arm_basic_op.h"

/**
 * @ingroup groupCmplxMath
 */

/**
 * @defgroup cmplx_pwr Complex Power
 *
 * Computes the power of the elements of a complex data vector.
 *
 * The <code>pSrc</code> points to the source data and
 * <code>pDst</code> points to the where the result should be written.
 * <code>numSamples</code> specifies the number of complex samples
 * in the input array and the data is stored in an interleaved fashion
 * (real, imag, real, imag, ...).
 * The input array has a total of <code>2*numSamples</code> values;
 * the output array has a total of <code>numSamples</code> values.
 * The underlying algorithm is used:
 *
 * <pre>
 * for(n=0; n<numSamples; n++) {
 *     pDst[n] = powDiv/(pSrc[(2*n)+0]^2 + pSrc[(2*n)+1]^2);
 * }
 * </pre>
 *
 * There are separate functions for floating-point, Q15, and Q31 data types.
 */

/**
 * @addtogroup cmplx_pwr
 * @{
 */
/**
 * @brief Floating-point complex power.
 * @param[in]       *pSrc points to complex input buffer
 * @param[out]      *pDst points to real output buffer
 * @param[in]       powDiv division argument
 * @param[in]       numSamples number of complex samples in the input vector
 * @return none.
 *
 */


void arm_cmplx_pwr_f32(
  float32_t * pSrc,
  float32_t * pDst,
  float32_t powDiv,
  uint32_t numSamples)
{
  float32_t realIn, imagIn;                      /* Temporary variables to hold input values */

#ifndef ARM_MATH_CM0

  /* Run the below code for Cortex-M4 and Cortex-M3 */
  uint32_t blkCnt;                               /* loop counter */

  /*loop Unrolling */
  blkCnt = numSamples >> 2u;

  /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
   ** a second loop below computes the remaining 1 to 3 samples. */
  while(blkCnt > 0u)
  {

    /* C[0] = powDiv/(A[0] * A[0] + A[1] * A[1]) */
    realIn = *pSrc++;
    imagIn = *pSrc++;
    /* store the result in the destination buffer. */
    *pDst++ = powDiv/((realIn * realIn) + (imagIn * imagIn));

    realIn = *pSrc++;
    imagIn = *pSrc++;
    *pDst++ = powDiv/((realIn * realIn) + (imagIn * imagIn));

    realIn = *pSrc++;
    imagIn = *pSrc++;
    *pDst++ = powDiv/((realIn * realIn) + (imagIn * imagIn));

    realIn = *pSrc++;
    imagIn = *pSrc++;
    *pDst++ = powDiv/((realIn * realIn) + (imagIn * imagIn));


    /* Decrement the loop counter */
    blkCnt--;
  }

  /* If the numSamples is not a multiple of 4, compute any remaining output samples here.
   ** No loop unrolling is used. */
  blkCnt = numSamples % 0x4u;

  while(blkCnt > 0u)
  {
    /* C[0] = powDiv/(A[0] * A[0] + A[1] * A[1]) */
    realIn = *pSrc++;
    imagIn = *pSrc++;
    /* store the result in the destination buffer. */
    *pDst++ = powDiv/((realIn * realIn) + (imagIn * imagIn));

    /* Decrement the loop counter */
    blkCnt--;
  }

#else

  /* Run the below code for Cortex-M0 */

  while(numSamples > 0u)
  {
    /* out = powDiv/((real * real) + (imag * imag)) */
    realIn = *pSrc++;
    imagIn = *pSrc++;
    /* store the result in the destination buffer. */
    *pDst++ = powDiv/((realIn * realIn) + (imagIn * imagIn));

    /* Decrement the loop counter */
    numSamples--;
  }

#endif /* #ifndef ARM_MATH_CM0 */

}

/**
 * @} end of cmplx_pwr group
 */
