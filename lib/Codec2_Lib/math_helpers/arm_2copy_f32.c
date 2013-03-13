/* ----------------------------------------------------------------------------
* $Date:        15. February 2012
* $Revision: 	V1.1.0
*
* Project: 	    CMSIS DSP Library Add-On
* Title:		arm_2copy_f32.c
*
* Description:	Copies the elements of a floating-point vector with stride of 2.
* ---------------------------------------------------------------------------- */

#include "arm_math.h"
#include "arm_basic_op.h"

/**
 * @ingroup groupSupport
 */

/**
 * @defgroup 2copy Vector Copy With stride of 2
 *
 * Copies samples from source to the destination whtn destination vector interval of 2.
 *
 * <pre>
 * 	pDst[2*n] = pSrc[n];   0 <= n < blockSize.
 * </pre>
 *
 */

/**
 * @addtogroup 2copy
 * @{
 */

/**
 * @brief Copies the elements of a floating-point vector, dst stride = 2.
 * @param[in]       *pSrc points to input vector
 * @param[out]      *pDst points to output vector
 * @param[in]       blockSize length of the input vector
 * @return none.
 *
 */


void arm_2copy_f32(
  float32_t * pSrc,
  float32_t * pDst,
  uint32_t blockSize)
{
  uint32_t blkCnt;                               /* loop counter */

#ifndef ARM_MATH_CM0

  /* Run the below code for Cortex-M4 and Cortex-M3 */

  /*loop Unrolling */
  blkCnt = blockSize >> 2u;

  /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
   ** a second loop below computes the remaining 1 to 3 samples. */
  while(blkCnt > 0u)
  {
    /* C = A */
    /* Copy and then store the results in the destination buffer */
    *pDst = *pSrc++;
    *(pDst+2) = *pSrc++;
    *(pDst+4) = *pSrc++;
    *(pDst+6) = *pSrc++;

    pDst += 8u;
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
    /* C = A */
    /* Copy and then store the results in the destination buffer */
    *pDst++ = *pSrc++; pDst++;

    /* Decrement the loop counter */
    blkCnt--;
  }
}

/**
 * @} end of BasicCopy group
 */
