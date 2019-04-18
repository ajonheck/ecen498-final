/*
 * TSK_pam_rx.c
 *
 *  Created on: Apr 17, 2019
 *      Author: Andy
 */
#include <std.h>
#include <string.h>
#include <mbx.h>
#include "hellocfg.h"
#include "TSK_pam.h"

extern MBX_Obj MBX_TSK_pam_tx_input;

extern int16_t rrc_h[LEN_H];

tsk_pam_rx()
{
	int16_t rx[LEN_INT16_FRAME];
	int16_t y[LEN_INT16_FRAME];
	int16_t dl[LEN_DL];
	memset(dl, 0, sizeof(int16_t)*LEN_DL);
	while(1)
	{
		// wait for an rx frame
		MBX_pend(&MBX_TSK_pam_rx_input, &rx, ~0);

		// apply match filter
		fir_filter(rx, LEN_INT16_FRAME, rrc_h, LEN_H, y, dl);

	}
}
