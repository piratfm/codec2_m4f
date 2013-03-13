/* ----------------------------------------------------------------------
*
* $Date:        15. February 2012
* $Revision: 	V1.1.0
*
* Project: 	    CMSIS DSP Library ADD-On
* Title:		arm_srsq_f32.c
*
* Description:	SquareRootOfSum value of a floating-point vector.
*
* Target Processor: Cortex-M4/Cortex-M3/Cortex-M0
*
* Version 1.1.0 2012/02/15
*    Updated with more optimizations, bug fixes and minor API changes.
*
* Version 1.0.10 2011/7/15
*    Big Endian support added and Merged M0 and M3/M4 Source code.
*
* Version 1.0.3 2010/11/29
*    Re-organized the CMSIS folders and updated documentation.
*
* Version 1.0.2 2010/11/11
*    Documentation updated.
*
* Version 1.0.1 2010/10/05
*    Production release and review comments incorporated.
*
* Version 1.0.0 2010/09/20
*    Production release and review comments incorporated.
* ---------------------------------------------------------------------------- */

#include "arm_math.h"

/**
 * @ingroup groupStats
 */

/**
 * @defgroup square root of sum SquareRootOfSum
 *
 * Calculates the square-rooted value of a vector sum.
 * The underlying algorithm is used:
 *
 * <pre>
 * 	Result = sqrt(pSrc[0] + pSrc[1] + pSrc[2] + ... + pSrc[blockSize-1]);
 * </pre>
 *
 */

/**
 * @addtogroup square root of sum
 * @{
 */


/**
 * @brief Square-rooted value of a floating-point vector sum.
 * @param[in]       *pSrc points to the input vector
 * @param[in]       blockSize length of the input vector
 * @param[out]      *pResult square root of sum value returned here
 * @return none.
 */


void arm_srsq_f32(
  float32_t * pSrc,
  uint32_t blockSize,
  float32_t * pResult)
{
  float32_t sum = 0.0f;                          /* Temporary result storage */
  uint32_t blkCnt;                               /* loop counter */

#ifndef ARM_MATH_CM0

  /* Run the below code for Cortex-M4 and Cortex-M3 */
  float32_t in1, in2, in3, in4;

  /*loop Unrolling */
  blkCnt = blockSize >> 2u;

  /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
   ** a second loop below computes the remaining 1 to 3 samples. */
  while(blkCnt > 0u)
  {
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
    in1 = *pSrc++;
    in2 = *pSrc++;
    in3 = *pSrc++;
    in4 = *pSrc++;

    sum += in1;
    sum += in2;
    sum += in3;
    sum += in4;

    /* Decrement the loop counter */
    blkCnt--;
  }

  /* If the blockSize is not a multiple of 4, compute any remaining output samples here.
   ** No loop unrolling is used. */
  blkCnt = blockSize % 0x4u;

#else

  /* Run the below code for Cortex-M0 */

  /* Loop over blockSize number of values */
  blkCnt = blockSize;

#endif /* #ifndef ARM_MATH_CM0 */

  while(blkCnt > 0u)
  {
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
    sum += *pSrc++;

    /* Decrement the loop counter */
    blkCnt--;
  }

  /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) / blockSize  */
  /* Store the result to the destination */
  arm_sqrt_f32(sum, pResult);
}

/**
 * @} end of square root of sum group
 */
