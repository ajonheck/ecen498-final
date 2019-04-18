/*
 * TSK_pam_tx.c
 *
 *  Created on: Apr 17, 2019
 *      Author: Andy
 */
#include <std.h>
#include <stdint.h>
#include <string.h>
#include <mbx.h>
#include "hellocfg.h"
#include "fir.h"

#define BITS_PER_INT16 16
#define SPB 16
#define LEN_PULSE_ARR ( BITS_PER_INT16 * SPB )

#define LEN_H 129
#define LEN_DL (LEN_PULSE_ARR - 1 + LEN_H)

#define BIT_TO_PULSE(num, pos) ( ( (num) & ( 0x01 << (pos) ) ) ? 32767 : -32768 )

extern MBX_Obj MBX_TSK_pam_tx_input;

int16_t h[] =
{
        97,    115,    125,    126,    117,     97,     68,     30,    -15,    -64,   -116,   -166,   -210,   -246,   -270,   -280,
      -271,   -244,   -198,   -133,    -51,     44,    150,    260,    369,    470,    557,    624,    663,    670,    641,    572,
       464,    317,    134,    -78,   -312,   -559,   -808,  -1045,  -1258,  -1433,  -1556,  -1615,  -1597,  -1494,  -1298,  -1005,
      -614,   -129,    447,   1102,   1824,   2599,   3407,   4228,   5042,   5825,   6558,   7218,   7787,   8248,   8587,   8795,
      8865,   8795,   8587,   8248,   7787,   7218,   6558,   5825,   5042,   4228,   3407,   2599,   1824,   1102,    447,   -129,
      -614,  -1005,  -1298,  -1494,  -1597,  -1615,  -1556,  -1433,  -1258,  -1045,   -808,   -559,   -312,    -78,    134,    317,
       464,    572,    641,    670,    663,    624,    557,    470,    369,    260,    150,     44,    -51,   -133,   -198,   -244,
      -271,   -280,   -270,   -246,   -210,   -166,   -116,    -64,    -15,     30,     68,     97,    117,    126,    125,    115,
        97,
};
int16_t pulse_arr[LEN_PULSE_ARR];
int16_t shaped_data[LEN_PULSE_ARR];
int16_t dl[LEN_DL];

tsk_pam_tx()
{
	memset(dl, 0, sizeof(int16_t) * LEN_DL);
	memset(pulse_arr, 0, sizeof(int16_t) * LEN_PULSE_ARR);
	int16_t pulse_index, i, x = 0;

	while(1)
	{
		// wait for an int to tx
		MBX_pend(&MBX_TSK_pam_tx_input, &x, ~0);

		// generate +/- pulses
		pulse_index = 0;
		for(i = 0; i < BITS_PER_INT16; i ++)
		{
			pulse_arr[pulse_index] = BIT_TO_PULSE(x, i);
			pulse_index += SPB;
		}

		// Shape data
		fir_filter(pulse_arr, LEN_PULSE_ARR, h, LEN_H, shaped_data, dl);
		memset(pulse_arr, 0, sizeof(int16_t) * LEN_PULSE_ARR);
	}
}

