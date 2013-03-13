/* ----------------------------------------------------------------------
*
* $Date:        15. February 2012
* $Revision: 	V1.1.0
*
* Project: 	    CMSIS DSP Library
* Title:		arm_interleave_f32.c
*
* Description:	Floating-point vector addition & interleaving.
*
* Target Processor: Cortex-M4/Cortex-M3/Cortex-M0
* ---------------------------------------------------------------------------- */

#include "arm_math.h"

/**
 * @ingroup groupMath
 */

/**
 * @defgroup BasicInterleave Vector Addition
 *
 * Element-by-element addition & interleaveing of two vectors.
 *
 * <pre>
 *     pDst[2*n] += pSrcA[n], pDst[2*n+1] += pSrcB[n],  0 <= n < blockSize.
 * </pre>
 *
 */

/**
 * @addtogroup BasicInterleave
 * @{
 */

/**
 * @brief Floating-point vector addition with interleaving.
 * @param[in]       *pSrcA points to the first input vector
 * @param[in]       *pSrcB points to the second input vector
 * @param[out]      *pDst points to the output vector
 * @param[in]       blockSize number of samples in each vector
 * @return none.
 */

void arm_add_interleave_f32(
  float32_t * pSrcA,
  float32_t * pSrcB,
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
    /* C0 += A , C1 += B */
    /* interleave and store result to destination */
    *pDst += *pSrcA++;
    *(pDst + 2) += *pSrcA++;
    *(pDst + 4) += *pSrcA++;
    *(pDst + 6) += *pSrcA++;

    *(pDst + 1) += *pSrcB++;
    *(pDst + 3) += *pSrcB++;
    *(pDst + 5) += *pSrcB++;
    *(pDst + 7) += *pSrcB++;

    pDst += 8u;
    /* Decrement the loop counter */
    blkCnt--;
  }

  /* If the blockSize is not a multiple of 4, compute any remaining output samples here.
   ** No loop unrolling is used. */
  blkCnt = blockSize % 0x4u;

#else

  /* Run the below code for Cortex-M0 */

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#endif /* #ifndef ARM_MATH_CM0 */

  while(blkCnt > 0u)
  {
    /* C = A + B */
    /* Add and then store the results in the destination buffer. */
    *(pDst++) += *pSrcA++;
	*(pDst++) += *pSrcB++;

    /* Decrement the loop counter */
    blkCnt--;
  }
}

/**
 * @} end of BasicInterleave group
 */
