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

#define BIT_TO_PULSE(num, pos) ( ( (num) & ( 0x01 << (pos) ) ) ? 0x7FFF : 0x8000 )

#define BITS_PER_INT16 16
#define SPB 4
#define LEN_PULSE_ARR ( BITS_PER_INT16 * SPB )

extern MBX_Obj MBX_TSK_pam_tx_input;

tsk_pam_tx()
{
	int16_t pulse_arr[LEN_PULSE_ARR];
	memset(pulse_arr, 0, sizeof(int16_t) * LEN_PULSE_ARR);
	int16_t pulse_index, i, x = 0;
	while(1)
	{
		MBX_pend(&MBX_TSK_pam_tx_input, &x, ~0);
		pulse_index = 0;
		for(i = 0; i < LEN_PULSE_ARR; i ++)
		{
			pulse_arr[pulse_index] = BIT_TO_PULSE(x, i);
			pulse_index += SPB;
		}
	}
}
