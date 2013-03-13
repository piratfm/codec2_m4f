/*
 * codec2_core.c
 *
 *  Created on: 16.05.2012
 *      Author: Admin
 */
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "codec2_core.h"
#include "varicode.h"

#define BITS_PER_CODEC_FRAME (2*FDMDV_BITS_PER_FRAME)
#define BYTES_PER_CODEC_FRAME (BITS_PER_CODEC_FRAME/8)

static struct C2MOD c2mod;

struct C2MOD *codec2_initialize_all(int oversample)
{
	int i;
	memset(&c2mod, 0x00, sizeof(struct C2MOD));
	c2mod.fdmdv = fdmdv_create();
	assert(c2mod.fdmdv != NULL);
	if(oversample) {
		c2mod.samplesModulator = (N48*2)*2; //num_samples x 2 channels
		c2mod.rateModulator = 48000;
	} else {
		c2mod.samplesModulator = 2*FDMDV_NOM_SAMPLES_PER_FRAME*2;
		c2mod.rateModulator = 8000;
	}

	c2mod.codec2handler = codec2_create(CODEC2_MODE_1400);
	assert(c2mod.codec2handler != NULL);
	c2mod.samplesPerFrame = codec2_samples_per_frame(c2mod.codec2handler);
	c2mod.nbit = codec2_bits_per_frame(c2mod.codec2handler);
	c2mod.bytesPerFrame = (c2mod.nbit + 7) / 8;

    /* clear filter memories */
    for(i=0; i<MEM8; i++)
    	c2mod.in8k[i] = 0.0;

    //c2mod.vc_bits = 0;

	return &c2mod;
}

#if 0
int codec2_set_callsign(const char *callsign)
{
    // send Callsign ----------------------------------------------------
	char callsigncr[MAX_CALLSIGN+1];
	strcpy(callsigncr, callsign);
	callsigncr[strlen(callsign)] = 13;

	// varicode encode and write to tx data fifo
	c2mod.vc_bits = varicode_encode(c2mod.varicode, callsigncr, MAX_CALLSIGN*VARICODE_MAX_BITS, strlen(callsign)+1);
	//printf("Callsign sending: %s nout: %d\n", callsign, nout);
	c2mod.vc_current=0;
	return c2mod.vc_bits;
}
#endif

#define FRAMES                     50
#define TWO_PI            6.283185307
#define FS                       8000

int codec2_modulate(short *input, short *output, int fake)
{
	int i, bit, byte, sync_bit;

	/* do not modulate, send voice instead */
	if(fake) {
		if(c2mod.rateModulator == 8000) {
			for(i=0; i<2*FDMDV_NOM_SAMPLES_PER_FRAME; i++) {
			//	tx_fdm_scaled[i] = FDMDV_SCALE * tx_fdm[i].real;
				output[i*2] = output[i*2+1] = input[i];
			}
		} else {
			for(i=0; i<N8; i++)
				c2mod.in8k[MEM8+i] = input[i];

			fdmdv_8_to_48(c2mod.out48k, &c2mod.in8k[MEM8], N8);
			for(i=0; i<MEM8; i++)
				c2mod.in8k[i] = c2mod.in8k[i+N8];

			for(i=0; i<N8; i++)
				c2mod.in8k[MEM8+i] = input[i+N8];

			fdmdv_8_to_48(&c2mod.out48k[N48], &c2mod.in8k[MEM8], N8);
			for(i=0; i<MEM8; i++)
				c2mod.in8k[i] = c2mod.in8k[i+N8];

			for(i=0; i<N48*2; i++)
				output[i*2] = output[i*2+1] = c2mod.out48k[i];
			}
		return 1;
	}

	codec2_encode(c2mod.codec2handler, c2mod.c2data, input);

	/* unpack bits, MSB first */
	bit = 7; byte = 0;
	for(i=0; i<BITS_PER_CODEC_FRAME; i++) {
		c2mod.tx_bits[i] = (c2mod.c2data[byte] >> bit) & 0x1;
		bit--;
		if (bit < 0) {
			bit = 7;
			byte++;
		}
	}
	assert(byte == BYTES_PER_CODEC_FRAME);

#if 0
    /* add data bit  ----------------------------------*/
    // spare bit in 1400 bit/s frame that codec defines.  Use this 1
    // bit/frame to send call sign data
	// bit 10 (11th bit) is v2 (third voicing bit)
    if (c2mod.vc_bits) {
    	c2mod.tx_bits[10] = c2mod.varicode[c2mod.vc_current];
    	c2mod.vc_current++;
    	if(c2mod.vc_current >= c2mod.vc_bits)
    		c2mod.vc_current=0;
    }
    //else
    //	c2mod.tx_bits[10] = 0;
#endif

	/* modulate even and odd frames */
	fdmdv_mod(c2mod.fdmdv, c2mod.tx_fdm, c2mod.tx_bits, &sync_bit);
	assert(sync_bit == 1);

	fdmdv_mod(c2mod.fdmdv, &c2mod.tx_fdm[FDMDV_NOM_SAMPLES_PER_FRAME], &c2mod.tx_bits[FDMDV_BITS_PER_FRAME], &sync_bit);
	assert(sync_bit == 0);

	if(c2mod.rateModulator == 8000) {
		for(i=0; i<2*FDMDV_NOM_SAMPLES_PER_FRAME; i++) {
		//	tx_fdm_scaled[i] = FDMDV_SCALE * tx_fdm[i].real;
		//	output[i*2+1] = FDMDV_SCALE * c2mod.tx_fdm[i].real;
		//	output[i*2] = 0;
			output[i*2] = output[i*2+1] = FDMDV_SCALE * c2mod.tx_fdm[i].real;
		}
		return 1;
	}

	/* scale and save as shorts */
	for(i=0; i<N8; i++)
		c2mod.in8k[MEM8+i] = c2mod.tx_fdm[i].real;

	fdmdv_8_to_48(c2mod.out48k, &c2mod.in8k[MEM8], N8);
	for(i=0; i<MEM8; i++)
		c2mod.in8k[i] = c2mod.in8k[i+N8];

	for(i=0; i<N8; i++)
		c2mod.in8k[MEM8+i] = c2mod.tx_fdm[i+N8].real;

	fdmdv_8_to_48(&c2mod.out48k[N48], &c2mod.in8k[MEM8], N8);
	for(i=0; i<MEM8; i++)
		c2mod.in8k[i] = c2mod.in8k[i+N8];

	for(i=0; i<N48*2; i++)
		output[i*2] = output[i*2+1] = FDMDV_SCALE * c2mod.out48k[i];

	return 1;
}
