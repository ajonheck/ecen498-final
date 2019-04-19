/*
 * TSK_pam_tx.c
 *
 *  Created on: Apr 17, 2019
 *      Author: Andy
 */
#include <std.h>
#include <string.h>
#include <mbx.h>
#include "hellocfg.h"
#include "TSK_pam.h"
#include "fir.h"

#define BIT_TO_PULSE(num, pos) ( ( (num) & ( 0x01 << (pos) ) ) ? 32767 : -32768 )

extern MBX_Obj MBX_TSK_pam_rx_input;
extern MBX_Obj MBX_TSK_pam_symbol_in;
extern int16_t rrc_h[LEN_H];

tsk_pam_tx()
{
	int16_t pulse_arr[LEN_CHANNEL_FRAME];
	int16_t shaped_data[LEN_CHANNEL_FRAME];
	int16_t dl[LEN_DL];
	memset(dl, 0, sizeof(int16_t) * LEN_DL);
	memset(pulse_arr, 0, sizeof(int16_t) * LEN_CHANNEL_FRAME);
	int16_t pulse_index, i, x = 0;

	while(1)
	{
		// wait for an int to tx
		MBX_pend(&MBX_TSK_pam_symbol_in, &x, ~0);

		// generate +/- pulses
		pulse_index = 0;
		for(i = 0; i < BITS_PER_INT16; i ++)
		{
			pulse_arr[pulse_index] = BIT_TO_PULSE(x, i);
			pulse_index += SPB;
		}

		// Shape data
		fir_filter(pulse_arr, LEN_CHANNEL_FRAME, rrc_h, LEN_H, shaped_data, dl);
    	MBX_post(&MBX_TSK_pam_rx_input, &shaped_data, ~0);
	}
}

