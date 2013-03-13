/* ----------------------------------------------------------------------
* $Date:        15. February 2012
* $Revision: 	V1.1.0
*
* Project: 	    CMSIS DSP Library Add-On
* Title:		arm_cmplx_ersq_f32.c
*
* Description:	Floating-point complex energy root squared with division.
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
 * @defgroup cmplx_ersq Complex Energy
 *
 * Computes the energy divided by energyDiv of the elements of a complex
 * with root square at the end.
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
 * sum += energyDiv/(pSrc[(2*n)+0]^2 + pSrc[(2*n)+1]^2); 0<n<numVal;
 * energyVal = sqrt(sum);
 * </pre>
 *
 */

/**
 * @addtogroup cmplx_ersq
 * @{
 */
/**
 * @brief Floating-point complex energy where each complex sum divider for energyDiv and root square at final.
 * @param[in]       *pSrc points to complex input buffer
 * @param[in]       energyDiv division argument
 * @param[in]       numSamples number of complex samples in the input vector
 * @param[out]      *energyVal points to output buffer
 * @return none.
 *
 */


void arm_cmplx_ersq_f32(
  float32_t * pSrc,
  float32_t energyDiv,
  const uint32_t numSamples,
  float32_t * energyVal)
{
  float32_t realIn, imagIn, sum = 0.0;                      /* Temporary variables to hold input values */
  uint32_t blkCnt;                               /* loop counter */
#ifndef ARM_MATH_CM0

  /* Run the below code for Cortex-M4 and Cortex-M3 */

  /*loop Unrolling */
  blkCnt = numSamples >> 2u;

  /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
   ** a second loop below computes the remaining 1 to 3 samples. */
  while(blkCnt > 0u)
  {

    /* C[0] = energyDiv/(A[0] * A[0] + A[1] * A[1]) */
    realIn = *pSrc++;
    imagIn = *pSrc++;
    /* store the result in the destination buffer. */
    sum += energyDiv/((realIn * realIn) + (imagIn * imagIn));

    realIn = *pSrc++;
    imagIn = *pSrc++;
    sum += energyDiv/((realIn * realIn) + (imagIn * imagIn));

    realIn = *pSrc++;
    imagIn = *pSrc++;
    sum += energyDiv/((realIn * realIn) + (imagIn * imagIn));

    realIn = *pSrc++;
    imagIn = *pSrc++;
    sum += energyDiv/((realIn * realIn) + (imagIn * imagIn));


    /* Decrement the loop counter */
    blkCnt--;
  }

  /* If the numSamples is not a multiple of 4, compute any remaining output samples here.
   ** No loop unrolling is used. */
  blkCnt = numSamples % 0x4u;

  while(blkCnt > 0u)
  {
    /* C[0] = energyDiv/(A[0] * A[0] + A[1] * A[1]) */
    realIn = *pSrc++;
    imagIn = *pSrc++;
    /* store the result in the destination buffer. */
    sum += energyDiv/((realIn * realIn) + (imagIn * imagIn));

    /* Decrement the loop counter */
    blkCnt--;
  }

#else

  /* Run the below code for Cortex-M0 */
  blkCnt = numSamples;

  while(blkCnt > 0u)
  {
    /* out = energyDiv/((real * real) + (imag * imag)) */
    realIn = *pSrc++;
    imagIn = *pSrc++;
    /* store the result in the destination buffer. */
    sum += energyDiv/((realIn * realIn) + (imagIn * imagIn));

    /* Decrement the loop counter */
    blkCnt--;
  }

#endif /* #ifndef ARM_MATH_CM0 */

  arm_sqrt_f32(fabs(sum/numSamples), energyVal);
}

/**
 * @} end of cmplx_ersq group
 */
