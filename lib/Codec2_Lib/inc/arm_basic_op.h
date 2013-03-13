/*
 * arm_basic_op.h
 *
 *  Created on: 12.05.2012
 *      Author: Admin
 */

#ifndef ARM_BASIC_OP_H_
#define ARM_BASIC_OP_H_

#include "arm_math.h"

/* pDst[2*n] += pSrcA[n], pDst[2*n+1] += pSrcB[n],  0 <= n < blockSize. */
void arm_add_interleave_f32(
  float32_t * pSrcA,
  float32_t * pSrcB,
  float32_t * pDst,
  uint32_t blockSize);

/* pDst[n] += pSrcA[n] * pSrcB[n],   0 <= n < blockSize. */
void arm_mav_f32(
  float32_t * pSrcA,
  float32_t * pSrcB,
  float32_t * pDst,
  uint32_t blockSize);


/*
for(n=0; n<numSamples; n++) {
    pDst[n] = powDiv/(pSrc[(2*n)+0]^2 + pSrc[(2*n)+1]^2);
}
*/
void arm_cmplx_pwr_f32(
  float32_t * pSrc,
  float32_t * pDst,
  float32_t powDiv,
  uint32_t numSamples);

/* energy = sqrt( SUM(powDiv/(pSrc[(2*n)+0]^2 + pSrc[(2*n)+1]^2)); */
void arm_cmplx_ersq_f32(
  float32_t * pSrc,
  float32_t energyDiv,
  const uint32_t numSamples,
  float32_t * energyVal);


/* Result = sqrt(pSrc[0] + pSrc[1] + pSrc[2] + ... + pSrc[blockSize-1]); */
void arm_srsq_f32(
  float32_t * pSrc,
  uint32_t blockSize,
  float32_t * pResult);

/* pDst[2*n] = pSrc[n];   0 <= n < blockSize. */
void arm_2copy_f32(
  float32_t * pSrc,
  float32_t * pDst,
  uint32_t blockSize);

/* pDst[2*n] = pSrcA[n] * pSrcB[n];   0 <= n < blockSize. */
void arm_2mult_f32(
  float32_t * pSrcA,
  float32_t * pSrcB,
  float32_t * pDst,
  uint32_t blockSize);

#endif /* ARM_BASIC_OP_H_ */
