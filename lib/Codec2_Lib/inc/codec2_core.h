/*
 * codec2_core.h
 *
 *  Created on: 16.05.2012
 *      Author: Admin
 */

#ifndef CODEC2_CORE_H_
#define CODEC2_CORE_H_

#include <stdint.h>
#include "comp.h"
#include "fdmdv.h"
#include "codec2.h"
#include "varicode.h"

#define N8                        160 /* procssing buffer size at 8 kHz */
#define N48             (N8*FDMDV_OS)
#define MEM8 (FDMDV_OS_TAPS/FDMDV_OS)

#define MAX_CALLSIGN         	 80


struct C2MOD {
//	uint8_t varicode[MAX_CALLSIGN*VARICODE_MAX_BITS];
//	int vc_bits;
//	int vc_current;

	uint8_t c2data[8];
    struct FDMDV *fdmdv;
    int           tx_bits[2*FDMDV_BITS_PER_FRAME];
    COMP          tx_fdm[2*FDMDV_NOM_SAMPLES_PER_FRAME];
    float in8k[MEM8 + N8];
    float out48k[N48*2];
    short         tx_fdm_scaled[2*FDMDV_NOM_SAMPLES_PER_FRAME];
  	int samplesModulator;
  	int rateModulator;

    void *codec2handler;
  	int samplesPerFrame;
  	int nbit;
  	int bytesPerFrame;
};

struct C2MOD *codec2_initialize_all(int oversample);
int codec2_modulate(short *input, short *output, int fake);

#endif /* CODEC2_CORE_H_ */
