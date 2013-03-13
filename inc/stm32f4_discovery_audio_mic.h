/*
 * stm32f4_discovery_audio_mic.h
 *
 *  Created on: 12.03.2013
 *      Author: tipok
 */

#ifndef STM32F4_DISCOVERY_AUDIO_MIC_H_
#define STM32F4_DISCOVERY_AUDIO_MIC_H_


/* Use PDM filter, provided by ST */
//#define USE_ST_FILTER

/* PDM buffer input size */
#define REC_BUFFERS            40

#ifdef USE_ST_FILTER
/* PCM buffer output size */
#define PCM_OUT_SIZE           16
#else
#define PCM_OUT_SIZE           8
#endif


uint32_t MicListenerInit(uint32_t AudioFreq, uint32_t BitRes, uint32_t ChnlNbr);
uint8_t MicListenerStart(uint16_t* pbuf, uint32_t size);
uint32_t MicListenerStop(void);


#endif /* STM32F4_DISCOVERY_AUDIO_MIC_H_ */
