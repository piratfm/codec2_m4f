/*
 * arm_mav_f32.c
 *
 *  Created on: 12.05.2012
 *      Author: Admin
 */


#include "arm_math.h"
#include "arm_basic_op.h"

/**
 * @ingroup groupMath
 */

/**
 * @defgroup BasicMultAdd Vector Multiplication and Addition
 *
 * Element-by-element multiplication of two vectors with addition to destination.
 *
 * <pre>
 *     pDst[n] += pSrcA[n] * pSrcB[n],   0 <= n < blockSize.
 * </pre>
 *
 */

/**
 * @addtogroup BasicMultAdd
 * @{
 */

/**
 * @brief Floating-point vector multiplication with addition.
 * @param[in]       *pSrcA points to the first input vector
 * @param[in]       *pSrcB points to the second input vector
 * @param[out]      *pDst points to the output vector
 * @param[in]       blockSize number of samples in each vector
 * @return none.
 */

void arm_mav_f32(
  float32_t * pSrcA,
  float32_t * pSrcB,
  float32_t * pDst,
  uint32_t blockSize)
{
  uint32_t blkCnt;                               /* loop counters */
#ifndef ARM_MATH_CM0

  /* Run the below code for Cortex-M4 and Cortex-M3 */
  float32_t inA1, inA2, inA3, inA4;              /* temporary input variables */
  float32_t inB1, inB2, inB3, inB4;              /* temporary input variables */
  float32_t out1, out2, out3, out4;              /* temporary output variables */

  /* loop Unrolling */
  blkCnt = blockSize >> 2u;

  /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
   ** a second loop below computes the remaining 1 to 3 samples. */
  while(blkCnt > 0u)
  {
    /* C = A * B */
    /* Multiply the inputs and store the results in output buffer */
    /* read sample from sourceA */
    inA1 = *pSrcA;
    /* read sample from sourceB */
    inB1 = *pSrcB;
    /* read sample from sourceA */
    inA2 = *(pSrcA + 1);
    /* read sample from sourceB */
    inB2 = *(pSrcB + 1);

    /* out = sourceA * sourceB */
    out1 = inA1 * inB1;

    /* read sample from sourceA */
    inA3 = *(pSrcA + 2);
    /* read sample from sourceB */
    inB3 = *(pSrcB + 2);

    /* out = sourceA * sourceB */
    out2 = inA2 * inB2;

    /* read sample from sourceA */
    inA4 = *(pSrcA + 3);

    /* store result to destination buffer */
    *pDst += out1;

    /* read sample from sourceB */
    inB4 = *(pSrcB + 3);

    /* out = sourceA * sourceB */
    out3 = inA3 * inB3;

    /* store result to destination buffer */
    *(pDst + 1) += out2;

    /* out = sourceA * sourceB */
    out4 = inA4 * inB4;
    /* store result to destination buffer */
    *(pDst + 2) += out3;
    /* store result to destination buffer */
    *(pDst + 3) += out4;


    /* update pointers to process next samples */
    pSrcA += 4u;
    pSrcB += 4u;
    pDst += 4u;

    /* Decrement the blockSize loop counter */
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
    /* C = A * B */
    /* Multiply the inputs and store the results in output buffer */
    *pDst++ += (*pSrcA++) * (*pSrcB++);

    /* Decrement the blockSize loop counter */
    blkCnt--;
  }
}

/**
 * @} end of BasicMultAdd group
 */
