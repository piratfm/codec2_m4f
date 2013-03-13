//==========================================================================
// Name:            varicode.h
// Purpose:         Varicode encoded and decode functions
// Created:         Nov 24, 2012
// Authors:         David Rowe
//
// To test:
//          $ gcc varicode.c -o varicode -DVARICODE_UNITTEST -Wall
//          $ ./varicode
// 
// License:
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.1,
//  as published by the Free Software Foundation.  This program is
//  distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//==========================================================================

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "varicode.h"
#include "varicode_table.h"


/*
  output is an unpacked array of bits of maximum size max_out.  Note
  unpacked arrays are a more suitable form for modulator input.
*/
#if 0
int varicode_get_bit(uint8_t varicode_out[], int bit_num)
{
	if(varicode_out[bit_num/8] & (1 << (bit_num % 8)))
		return 1;
	else
		return 0;
}
#endif

int varicode_encode(uint8_t varicode_out[], char ascii_in[], int max_out, int n_in)
{
    int            n_out, index, n_zeros, v_len;
    unsigned short byte1, byte2, packed;

    n_out = 0;

    while(n_in && (n_out < max_out)) {

        assert((unsigned int)(*ascii_in) < 128);

        index = 2*(unsigned int)(*ascii_in);
        byte1 = varicode_table[index];
        byte2 = varicode_table[index+1];
        packed = (byte1 << 8) + byte2;
        //printf("n_in: %d ascii_in: %c index: %d packed 0x%x\n", n_in, *ascii_in, index, packed);
        ascii_in++;

        n_zeros = 0;
        v_len = 0;
        while ((n_zeros < 2) && (n_out < max_out) && (v_len <= VARICODE_MAX_BITS)) {
            if (packed & 0x8000) {
                *varicode_out = 1;
                n_zeros = 0;
            }
            else {
                *varicode_out = 0;
                n_zeros++;
            }
            //printf("packed: 0x%x *varicode_out: %d n_zeros: %d v_len: %d\n", packed, *varicode_out, n_zeros,v_len );
            packed <<= 1;
            varicode_out++;

            n_out++;
            v_len++;
        }
        assert(v_len <= VARICODE_MAX_BITS);

        n_in--;
            
    }

    return n_out;
}
